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

void task(string query, int port, string ip, int id)
{
    cout<<"i am thread "<<id<< " and i have the message: "<<query<<endl;

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

 //   write(sock, &id, sizeof(int));
  //  cout<<"Wrote "<<id<<endl;

    int length = query.length();
    char query_send[length+1];
    strcpy(query_send, query.c_str());

    //int size = length+1;
    int size = 100;
//    write(sock, &size, sizeof(int));
//    write(sock, query_send, sizeof(length+1));

    int l=3;
    char m[3]="hi";
    string me = "hi";
//    write(sock, &l, sizeof(int));
//    write(sock, m, sizeof(3));

    socket_write_string(sock, query_send);

    printf("Connecting to port %d\n", port);

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

    while(getline(file, query))// read each line
    {
        if(curr_threads <= n) //no more than n threads
        {
            threads[curr_threads] = new thread(task, query, server_port, server_ip, curr_threads);
            curr_threads++;
        }

    }

    for (int i = 1; i < curr_threads; i++)   //join
    {
        threads[i]->join();
    }

    sleep(10);
    return 0;
}