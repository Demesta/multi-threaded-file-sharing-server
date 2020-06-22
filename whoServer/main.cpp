#include <stdio.h>
#include <iostream>
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
#include <arpa/inet.h>
#include "Arguments.h"
#include "Worker_list.h"
#include "Hash.h"
#include "List.h"
#include "utils.h"
#include "logging.h"
#include "sockets.h"

int thread_id = 0;

void child_server(int newsock);
void perror_exit(char *message);
void sigchld_handler(int sig);

using namespace std;

bool running = true;

struct work_item
{
  int socket;
  int type;
  int peer_address;
  int peer_port;
};

work_item *socket_buffer;    //circular buffer
mutex socket_buffer_mutex;
condition_variable non_empty;
condition_variable non_full;

int buffer_tail = 0;
int buffer_head = 0;
int buffer_capacity = 0;
int buffer_size = 0;

Worker_list_node *master_sockets;

mutex cout_mutex;
mutex write_socket;

int open_socket(int port)
{
    // listen on ports
    int sock;
    struct sockaddr_in server;
    struct sockaddr *server_ptr = (struct sockaddr *) &server;

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)  //create socket
        perror_exit("socket");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    if (bind(sock, server_ptr, sizeof(server)) < 0)  //bind socket
        perror_exit("bind");

    if (listen(sock, 10) < 0) perror_exit("listen");   //listen for connections
    return sock;
}

void perror_exit(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

int buffer_read(work_item &item)    //read socket from buffer
{
    {
        unique_lock<mutex> lock(socket_buffer_mutex);

        non_empty.wait(lock, []
        { return buffer_size > 0; });

        item.socket = socket_buffer[buffer_tail].socket;
        item.type = socket_buffer[buffer_tail].type;

        buffer_tail = (buffer_tail + 1) % buffer_capacity;
        buffer_size--;
    }

    non_full.notify_all();

    return 0;
}

int buffer_write(work_item &value)    //write socket to buffer
{
    {
        unique_lock<mutex> lock(socket_buffer_mutex);

        non_full.wait(lock, []
        { return buffer_size < buffer_capacity; });

        socket_buffer[buffer_head].socket = value.socket;
        socket_buffer[buffer_head].type = value.type;
        buffer_head = (buffer_head + 1) % buffer_capacity;
        buffer_size++;
    }

    non_empty.notify_all();

    return 0;
}

int statistics_connection(int sock)
{
    char country[32];
    int country_size;
    int date_size;
    char date_str[11];

    while (1)
    {
        if ((date_size = socket_read_str(sock, date_str, 11)) < 0)
        {
            cout << "failed to read" << endl;
            return -1;
        }

        if (strcmp(date_str, "/Done") == 0)
            break;

        cout << date_str << endl;

        if ((country_size = socket_read_str(sock, country, 32)) < 0)
        {
            cout << "failed to read" << endl;
            return -1;
        }

        cout << country << endl;

        char disease[32];
        int disease_size;
        while ((disease_size = socket_read_str(sock, disease, 32)) > 0)
        {
            if (strcmp(disease, "/Done") == 0)
                break;

            cout << disease << endl;
            int age20, age40, age60, age60Plus;
            if (socket_read_int(sock, &age20) < 0)
            {
                cout << "failed to read" << endl;
                return -1;
            }
            if (socket_read_int(sock, &age40) < 0)
            {
                cout << "failed to read" << endl;
                return -1;
            }
            if (socket_read_int(sock, &age60) < 0)
            {
                cout << "failed to read" << endl;
                return -1;
            }
            if (socket_read_int(sock, &age60Plus) < 0)
            {
                cout << "failed to read" << endl;
                return -1;
            }

            {
                lock_guard<mutex> cout_lock(cout_mutex);
                cout << "Age 0-20 : " << age20 << endl;
                cout << "Age 20-40 : " << age40 << endl;
                cout << "Age 40-60 : " << age60 << endl;
                cout << "Age 60+ : " << age60Plus << endl << endl;
            }
        }

    }

    if (date_size < 0)
    {
        LOG("statistics connection failed: error code %d", date_size);
        return -1;
    }

    return 0;
}

mutex open_socket_mutex;

int query_connection(int client_socket)
{
    char query[100];
    int query_size;

    if ((query_size = socket_read_str(client_socket, query, 100)) < 0)
    {
        LOG("failed to read query: error code %d", query_size);
        return -1;
    }

    //read query for master from client
    string message(query, query_size);
    string type = getFirstWord(message);

    Worker_list_node *current_worker = master_sockets;
    while (current_worker != nullptr)    //get answers from workers
    {
        lock_guard<mutex> lock(current_worker->connection_mutex);
        int worker_socket = current_worker->connection_socket;
        current_worker = current_worker->nextNode;

        socket_write_string(worker_socket, type);
        socket_write_string(worker_socket, message);

        while (1)
        {
            char answer[100];
            int answer_size;
            //get answer from master
            if ((answer_size = socket_read_str(worker_socket, answer, 100)) < 0)
            {
                LOG("failed to read answer: error code %d", answer_size);
                return -1;
            }

            if (strcmp(answer, "/Done") == 0)
                break;

            // send answer to client
            int error_code;
            if((error_code = socket_write_str(client_socket, answer, (size_t) answer_size)) < 0)
            {
                LOG("write failed: error code %d", error_code);
                return -1;
            }

            cout<< "Sending to client:" << answer << endl;
        }
    }

    socket_write_str(client_socket, (char*)"/Done", 5);

    return 0;
}

mutex work_variable_mutex;
condition_variable work_variable;

void task(int i)    //slave threads' work
{
    thread_id = i;
    char message[100];

    while (running)
    {
        work_item item{};
        buffer_read(item);

        if (item.type == 0)   //something from master
        {
            statistics_connection(item.socket);

        } else if (item.type == 1)   //query from client
        {
            query_connection(item.socket);
        }

    }
}

void helper_main(int query_socket)
{
    while (running)
    {
        int sock;
        if ((sock = accept(query_socket, nullptr, nullptr)) > 0)    //TODO edw kati ginetai
        {
            work_item item{.socket=sock, .type=1};
            buffer_write(item);
        }
    }

}

int main(int argc, char *argv[])       //main thread
{
    Arguments args;
    initializeArguments(args, argv);

    int stats_port = args.statistics_port;
    int query_port = args.query_port;
    int n = args.num_threads;

    buffer_capacity = args.buffer_size;
    socket_buffer = new work_item[args.buffer_size];

    cout << "creating " << n << " threads" << endl;
    thread *threads[n];
    for (int i = 1; i <= n; i++)   //create n threads
    {
        threads[i] = new thread(task, i);
    }

    // listen on ports
    int statistics_socket = open_socket(args.statistics_port);
    int query_socket = open_socket(args.query_port);

    auto helper = thread(helper_main, query_socket);
    while (running)
    {
        int socket;
        if ((socket = accept(statistics_socket, nullptr, nullptr)) > 0)
        {
            insertListNode(socket, master_sockets);
            ///GET-IP////
            sockaddr_in addr;
            socklen_t addr_size;
            char masterip[100];
            getpeername(socket, (struct sockaddr *) &addr, &addr_size);
            strcpy(masterip, inet_ntoa(addr.sin_addr));
            cout << "the ip is " << masterip << endl;
            /////////////

            work_item item{.socket=socket, .type=0};
            buffer_write(item);
        }
    }

    for (int i = 1; i <= n; i++)   //join
    {
        threads[i]->join();
    }
    helper.join();

    return 0;

}