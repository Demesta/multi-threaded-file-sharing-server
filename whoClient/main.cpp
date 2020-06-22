/* inet_str_client.c: Internet stream sockets client */
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/wait.h>         /* sockets */
#include <sys/types.h>         /* sockets */
#include <sys/socket.h>         /* sockets */
#include <netinet/in.h>         /* internet sockets */
#include <netdb.h>             /* gethostbyaddr */
#include <unistd.h>             /* fork */
#include <stdlib.h>             /* exit */
#include <ctype.h>             /* toupper */
#include <signal.h>          /* signal */
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include "Arguments.h"
#include "sockets.h"
#include "logging.h"

using namespace std;

mutex safe_print;

mutex ready_mutex;
int ready = 0;

mutex done_mutex;
int done = 0;

int curr_threads_global;

int task(string *query, int port, string ip, int id)
{
    LOG("received query '%s'", query->c_str());
    int sock;
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        LOG("failed to open socket: error code %d", sock);
        return -1;
    }

    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr *) &server;
    int len = ip.length();
    char hostname[len];
    strcpy(hostname, ip.c_str());
    struct hostent *rem = gethostbyname(hostname);

    server.sin_family = AF_INET;
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(port);

    int error_code;
    if ((error_code = connect(sock, serverptr, sizeof(server))) < 0)
    {
        LOG("server connection failed:  error code %d", error_code);
        return -1;
    }

    ready_mutex.lock();
    ready++;
    ready_mutex.unlock();

    //wait for every thread to be ready
    while (ready != curr_threads_global);
    //send query
    if ((error_code = socket_write_string(sock, *query)) < 0)
    {
        LOG("failed to write query: error code %d", error_code);
    }

    //lock while it gets whole package of lines/answers
    safe_print.lock();
    LOG("thread %d reading for answer ...", id);

    while (1)
    {
        char answer[100];
        //get answer
        if ((error_code = socket_read_str(sock, answer, 100)) < 0)
        {
            LOG("read failed: error code %d", error_code);
            return -1;
        }

        LOG("thread %d received '%s'", id, answer);

        if (strcmp(answer, "/Done") == 0)
            break;

        cout << answer << endl;
    }
    LOG("thread %d finished reading answer!", id);

    safe_print.unlock();

    LOG("finished parsing answer");

    done_mutex.lock();
    done++;
    done_mutex.unlock();

    delete query;
    return 0;
}

int main(int argc, char *argv[])
{
    LOG("client starting");
    Arguments args;
    initializeArguments(args, argv);

    int server_port = args.server_port;
    string server_ip = args.server_ip;
    int num_threads = args.num_threads;

    thread *threads[num_threads];

    ifstream file(args.query_file);
    string query;
    int curr_threads = 0;
    int queries = 0;
    int done_queries = 0;

    LOG("client scheduling queries");
    while (getline(file, query))
    {
        if (curr_threads < num_threads) //no more than n threads
        {
            LOG("creating thread for execution of '%s'", query.c_str());
            string *query_copy = new string(query);
            threads[curr_threads] = new thread(task, query_copy, server_port, server_ip, curr_threads + 1);
            curr_threads++;
            done_queries++;
        } else
        {
            LOG("query '%s' will be schedules later: not enough threads ", query.c_str());
        }
        queries++;  //how many lines the file has
    }
    curr_threads_global = curr_threads;

//    int times = 1;
//
//    LOG("client scheduling more queries");
//    while(1)
//    {
//        if(queries > done_queries)  //if lines are more than the threads we need to create more threads for the rest of the lines
//        {
//            int lines_done=0;
//
//            while(done != n){}  //wait until all threads are done
//
//            done=0;
//            ready=0;
//            curr_threads=1;
//            curr_threads_global=0;
//
//            for (int i = 1; i < curr_threads; i++)   //join
//            {
//                threads[i]->join();
//            }
//
//            file.clear();
//            file.seekg(0,ios::beg); //start reading file from beginning
//
//            while(getline(file, query))
//            {
//                lines_done++;
//                if(lines_done > (n*times))  //start creating threads after passed n * times lines (if this is done 2 times then there are 2 * num_threads made)
//                {
//                    if(curr_threads <= n) //no more than n threads
//                    {
//                        string* query_copy = new string(query);
//                        threads[curr_threads] = new thread(task, query_copy, server_port, server_ip, curr_threads);
//                        curr_threads++;
//                        done_queries++;
//                    }
//                }
//            }
//            curr_threads_global = curr_threads-1;
//
//            times++;
//        }
//        else   //threads are enough for all the queries
//            break;
//    }

    for (int i = 0; i < curr_threads; i++)   //join last threads
    {
        threads[i]->join();
    }

    return 0;
}