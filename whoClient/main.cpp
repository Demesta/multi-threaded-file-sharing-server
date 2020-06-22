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

using namespace std;

mutex safe_print;

mutex ready_mutex;
int ready=0;

mutex done_mutex;
int done=0;

int curr_threads_global;

void task(string query, int port, string ip, int id)
{
    safe_print.lock();
    //cout<<"i am thread "<<id<< " and i have the message: "<<query<<endl;
    safe_print.unlock();

    int sock;
    char buf[256];
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr *) &server;
    struct hostent *rem;

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        cout<<"error: socket\n";

    int len = ip.length();
    char hostname[len];
    strcpy(hostname, ip.c_str());
    rem = gethostbyname(hostname);

    server.sin_family = AF_INET;
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(port);

    if (connect(sock, serverptr, sizeof(server)) < 0)
        cout<<"error: connect\n";

    int length = query.length();
    char query_send[length+1];
    strcpy(query_send, query.c_str());

    ready_mutex.lock();
    ready++;
    ready_mutex.unlock();

    while(ready != curr_threads_global){}   //wait for every thread to be ready

    socket_write_string(sock, query_send);  //send query

    safe_print.lock();   //lock while it gets whole package of lines/answers
    cout<<query<<endl;

    while(1)
    {
        char answer[100];
        socket_read_str(sock, answer, 100);  //get answer

        if(strcmp(answer, "/Done") == 0)
            break;
        cout<<answer<<endl;
    }
    safe_print.unlock();

    done_mutex.lock();
    done++;
    done_mutex.unlock();
}

int main(int argc, char *argv[])
{
    Arguments args;
    initializeArguments(args, argv);

    int server_port = args.server_port;
    string server_ip = args.server_ip;
    int n = args.num_threads;
    string query_file = args.query_file;

    thread *threads[n];

    int length = query_file.length();
    char qfile[length+1];
    strcpy(qfile, query_file.c_str());
    ifstream file(qfile);
    string query;
    int curr_threads=1;
    int queries=0;
    int done_queries=0;


    while(getline(file, query))// read each line
    {
        if(curr_threads <= n) //no more than n threads
        {
            threads[curr_threads] = new thread(task, query, server_port, server_ip, curr_threads);
            curr_threads++;
            done_queries++;
        }
        queries++;  //how many lines the file has
    }
    curr_threads_global = curr_threads-1;

    int times=1;

    while(1)
    {
        if(queries > done_queries)  //if lines are more than the threads we need to create more threads for the rest of the lines
        {
            int lines_done=0;

            while(done != n){}  //wait until all threads are done

            done=0;
            ready=0;
            curr_threads=1;
            curr_threads_global=0;

            for (int i = 1; i < curr_threads; i++)   //join
            {
                threads[i]->join();
            }

            file.clear();
            file.seekg(0,ios::beg); //start reading file from beginning

            while(getline(file, query))
            {
                lines_done++;
                if(lines_done > (n*times))  //start creating threads after passed n * times lines (if this is done 2 times then there are 2 * num_threads made)
                {
                    if(curr_threads <= n) //no more than n threads
                    {
                        threads[curr_threads] = new thread(task, query, server_port, server_ip, curr_threads);
                        curr_threads++;
                        done_queries++;
                    }
                }
            }
            curr_threads_global = curr_threads-1;

            times++;
        }
        else   //threads are enough for all the queries
            break;
    }


    for (int i = 1; i < curr_threads; i++)   //join last threads
    {
        threads[i]->join();
    }

    return 0;
}