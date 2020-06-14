// fuser -k -n tcp 8002
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
#include "Arguments.h"
#include "Worker_list.h"
#include "Hash.h"
#include "List.h"

int thread_id = 0;

#define DEBUG

#ifdef DEBUG
mutex log_mut;
#define LOG(...) do{ lock_guard<mutex> lock(log_mut); fprintf(stderr, "[INFO] thread %d - ", thread_id); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); } while(false)
#else
#define LOG(...)
#endif

void child_server(int newsock);
void perror_exit(char *message);
void sigchld_handler(int sig);

using namespace std;

bool running = true;

//struct Worker
//{
//  int id;
//  char *address;
//  int port;
//
//  size_t topk_age_ranges(int k, char *country, char *date_from, char *date_to, char *buffer, int buffer_size)
//  {
//      return 0;
//  }
//};
//
//mutex table_mutex;
////HashTable *worker_table;
//mutex list_mutex;
////ArrayList *worker_list;
//
//Worker *create_worker(char *address, int port)
//{
//    char *address_copy = new char[strlen(address)];
//    strcpy(address_copy, address);
//
//    lock_guard<mutex> lock(list_mutex);
//    int id = worker_list->size();
//
//    Worker *worker = new Worker();
//    worker->id = id;
//    worker->address = address_copy;
//    worker->port = port;
//    worker_list->push(worker);
//
//    return worker;
//}
//
//Worker *find_worker(char *country)
//{
//    lock_guard<mutex> lock(table_mutex);
//    //return (Worker *) worker_table->get(country);
//}
//
//void worker_add_country(Worker *worker, char *country, size_t country_size)
//{
//    char *country_copy = new char[country_size];
//    strncpy(country_copy, country, country_size);
//
//    lock_guard<mutex> lock(table_mutex);
//    //worker_table->put(country_copy, worker);
//}
//
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

Worker_list_node *master_workers;

mutex cout_mutex;

int __socket_safe_read(int socket, void *buffer, size_t buffer_size)
{
    char *buffer_ptr = (char *) buffer;
    size_t current_index = 0;

    while (current_index < buffer_size)
    {
        int read_size = read(socket, buffer_ptr + current_index, buffer_size - current_index);
        if (read_size < 0)
        {
            return -1;
        }
        current_index += read_size;
    }

    return buffer_size;
}

int __socket_safe_write(int socket, void *buffer, size_t buffer_size)
{
    char *buffer_ptr = (char *) buffer;
    size_t current_index = 0;

    while (current_index < buffer_size)
    {
        int write_size = write(socket, buffer_ptr + current_index, buffer_size - current_index);
        if (write_size < 0)
        {
            return -1;
        }
        current_index += write_size;
    }

    return buffer_size;
}

int socket_read_int(int socket, int *value)
{
    return __socket_safe_read(socket, value, sizeof(int));
}

int socket_read_str(int socket, char *buffer, size_t buffer_size)
{
    size_t str_size = 0;
    if (__socket_safe_read(socket, &str_size, sizeof(size_t)) < 0)
    {
        return -1;
    }

    if ((str_size + 1) > buffer_size)
    {
        LOG("buffer overflow");
        return -1;
    }

    if (str_size == 0) {
        return 0;
    }

    if (__socket_safe_read(socket, buffer, str_size) < 0)
    {
        return -1;
    }

    buffer[str_size] = '\0';
    return str_size;
}

int socket_write_int(int socket, int *value)
{
    return __socket_safe_write(socket, value, sizeof(int));
}

int socket_write_str(int socket, char *buffer, size_t buffer_size)
{
    if (__socket_safe_write(socket, &buffer_size, sizeof(size_t)) < 0)
    {
        return -1;
    }

    return __socket_safe_write(socket, buffer, buffer_size);
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

int statistics_connection(int socket)
{
//    char worker_address[32];
//    if (socket_read_str(socket, worker_address, 32) < 0)
//    {
//        cout << "failed to read worker address" << endl;
//        return -1;
//    }
//
//    int worker_port = 0;
//    if (socket_read_int(socket, &worker_port) < 0)
//    {
//        cout << "failed to read worker port" << endl;
//        return -1;
//    }

//    Worker *worker = create_worker(worker_address, worker_port);
    char country[32];
    int country_size;
    int date_size;
    char date_str[11];

    while ((date_size = socket_read_str(socket, date_str, 11)) > 0)
    {
        if ((country_size = socket_read_str(socket, country, 32)) < 0)
        {
            return -1;
        }

//            worker_add_country(worker, country, country_size);

        char disease[32];
        int disease_size;
        while ((disease_size = socket_read_str(socket, disease, 32)) > 0)
        {
            cout << disease << endl;
            int age20, age40, age60, age60Plus;
            if (socket_read_int(socket, &age20) < 0)
            {
                cout << "failed to read" << endl;
                return -1;
            }
            if (socket_read_int(socket, &age40) < 0)
            {
                cout << "failed to read" << endl;
                return -1;
            }
            if (socket_read_int(socket, &age60) < 0)
            {
                cout << "failed to read" << endl;
                return -1;
            }
            if (socket_read_int(socket, &age60Plus) < 0)
            {
                cout << "failed to read" << endl;
                return -1;
            }

            {
                lock_guard<mutex> cout_lock(cout_mutex);
                cout << "Age 0-20 : " << age20 << endl;
                cout << "Age 20-40 : " << age40 << endl;
                cout << "Age 40-60 : " << age60 << endl;
                cout << "Age 60+ : " << age60Plus << endl;
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

//int query_connection(int socket)
//{
//    char query[128];
//    size_t query_size;
//
//    if ((query_size = socket_read_str(socket, query, 128)) < 0)
//    {
//        cout << "failed to read query" << endl;
//        return -1;
//    }
//
//   // int type = query_type(query, query_size);
//
////    if (type == 0)
////    {
////        // handle /diseaseFrequency
////    } else if (type == 1)
////    {
////        // handle /topk-AgeRanges
////        int k;
////        char disease[32];
////        char country[32];
////        char dateFrom[16];
////        char dateTo[16];
////
////        Worker *worker = find_worker(country);
////        char result[1024];
////        size_t result_size;
////        if ((result_size = worker->topk_age_ranges(k, disease, dateFrom, dateTo, result, 1024)) < 0)
////        {
////            cout << "query failed on worker" << endl;
////        }
////
////        socket_write_str(socket, result, result_size);
////    }
//
//    return 0;
//}

mutex work_variable_mutex;
condition_variable work_variable;

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

    if (listen(sock, 5) < 0) perror_exit("listen");   //listen for connections
    return sock;
}

void perror_exit(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

void task(int i)    //slave threads' work
{
    thread_id = i;
    char message[100];

    while (running)
    {
        work_item item{};
        buffer_read(item);

        cout << "thread " << i << " accepting connection from socket " << item.socket << endl;

        if (item.type == 0)   //something from master
        {
            statistics_connection(item.socket);
//            int j;
//            while(read(item.socket, &j, sizeof(int)) <= 0);
//            LOG("%d", j);
        }
//        else if (item.type == 1)   //query from client
//        {
//            query_connection(item.socket);
//        }
//
//        close(item.socket);
    }
}

int main(int argc, char *argv[])       //main thread
{
    Arguments args;
    initializeArguments(args, argv);

    int stats_port = args.statistics_port;
    int query_port = args.query_port;
    int n = args.num_threads;

    cout << "initializing socket buffer" << endl;
    buffer_capacity = args.buffer_size;
    socket_buffer = new work_item[args.buffer_size];

    cout << "creating " << n << " threads" << endl;
    thread *threads[n];
    for (int i = 1; i <= n; i++)   //create n threads
    {
        threads[i] = new thread(task, i);
    }

//    usleep(15 * 1000000);
//    work_variable.notify_one();

    // listen on ports
    int statistics_socket = open_socket(args.statistics_port);
    int query_socket = open_socket(args.query_port);

    while (running)
    {
        int socket;
        if ((socket = accept(statistics_socket, nullptr, nullptr)) > 0)
        {
            work_item item{.socket=socket, .type=0};
            buffer_write(item);
        }

//        if ((socket = accept(query_socket, nullptr, nullptr)) > 0)
//        {
//            work_item item{.socket=socket, .type=1};
//            buffer_write(item);
//        }
    }

    for (int i = 1; i <= n; i++)   //join
    {
        threads[i]->join();
    }

    return 0;

}