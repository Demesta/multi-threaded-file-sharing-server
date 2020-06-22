#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <fstream>
#include <poll.h>
#include <sys/types.h>         /* sockets */
#include <sys/socket.h>         /* sockets */
#include <netinet/in.h>         /* internet sockets */
#include <unistd.h>          /* read, write, close */
#include <netdb.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <mutex>
#include <fstream>
#include <bits/stdc++.h>
#include <fcntl.h>
#include "Arguments.h"
#include "Dir_list.h"
#include "Records_list.h"
#include "Dir_countries.h"
#include "AVLTree.h"
#include "diseaseHashTable.h"
#include "countryHashTable.h"
#include "utils.h"
#include "Bucket.h"
#include "PatientRecord.h"
#include "signal_handler.h"
#include "sockets.h"
#include "logging.h"

using namespace std;

#define CHILD 0;
#define READ 0;
#define WRITE  1;

mutex write_to_socket;

int main(int argc, char *argv[])
{
    Arguments args;
    initializeArguments(args, argv);

    //about files :

    struct dirent *de;  // Pointer for directory entry

    DIR *dr = opendir(".");

    if (dr == NULL)
    {
        printf("Could not open current directory");
        return 0;
    }

    int files_num = 0;

    while ((de = readdir(dr)))
    {
        int check1 = strcmp(de->d_name, reinterpret_cast<const char *>("."));
        int check2 = strcmp(de->d_name, reinterpret_cast<const char *>(".."));

        if (check1 != 0 && check2 != 0)
        {
            files_num++;        //count how many files the folder contains
        }
    }

    closedir(dr);

    //about processes :

    int type = 1;
    pid_t childpid;
    pid_t mypid;

    int buffer_size = args.buffer_size; //like buffer_size = 100
    string input_dir = args.input_dir;  //string dir = "input_dir/";
    int n = args.num_workers;

    int i, id;
    pid_t childpids[n];

    char bufin[20] = {0};   //size with for loop and **
    char bufout[20] = {0};

    for (i = 0; i < n; i++)   //make n workers
    {
        if ((childpids[i] = fork()) <= 0)   //using fork
        {
            id = i + 1;    //id of worker is the number of the loop (first loop -> worker_id = 1)
            type = CHILD;

            //make the 2 pipes for the process just created:
            string first_half1 = "_to_parent";
            string first_half2 = "parent_to_";

            string id_string = to_string(id);   //id to string to concatenate

            first_half1 = id_string + first_half1;  //example 2_to_parent with 2=worker number 2
            first_half2 = first_half2 + id_string;

            int length1 = first_half1.length();
            int length2 = first_half2.length();

            char fifo1[length1 + 1];
            char fifo2[length2 + 1];

            strcpy(fifo1, first_half1.c_str());
            strcpy(fifo2, first_half2.c_str());

            mkfifo(fifo1, 0666);
            mkfifo(fifo2, 0666);

            break;
        }
    }

    int division = files_num % n;   //helps divide the files for the workers

    if (type == 0)   //child code
    {

        cout<<"child "<<getpid()<<endl;
        static struct sigaction killAction;
        killAction.sa_handler = killProcess;
        sigfillset(&(killAction.sa_mask));
        sigaction(SIGKILL, &killAction, NULL);

        string first_half1 = "_to_parent";
        string first_half2 = "parent_to_";

        string id_string = to_string(id);   //id to string to concatenate

        first_half1 = id_string + first_half1;
        first_half2 = first_half2 + id_string; //example 2_to_parent with 2=worker number 2

        int length1 = first_half1.length();
        int length2 = first_half2.length();

        char fifo1[length1 + 1];
        char fifo2[length2 + 1];

        strcpy(fifo1, first_half1.c_str());   //ready to use
        strcpy(fifo2, first_half2.c_str());

        int w_to_p = open(fifo1, 0666);
        int p_to_w = open(fifo2, 0666);

        DirListNode *country_list = nullptr;
        ListNode *List = nullptr;
        diseaseHashTable d_table = diseaseHashTable(10);
        countryHashTable c_table = countryHashTable(10);
        int countries = 0;  //holds the number of dirs assigned

        int port;
        string ip;

        ListNode *temp2;
        while (1)   //get commands from parent
        {
            char get_command[buffer_size];
            receive_message(p_to_w, get_command, buffer_size);

            if (strcmp(get_command, "/Read Directories") == 0)   //read and save the directories' names to work on
            {
                while (1)
                {
                    bzero(get_command, buffer_size);
                    receive_message(p_to_w, get_command, buffer_size);

                    if ((strcmp(get_command, "/Done") == 0))
                    {
                        break;
                    }

                    Dir_countries *country = new Dir_countries(get_command);
                    insertListNode(country, country_list);
                    countries++;

                }
            }
            else if(strcmp(get_command, "/Server IP") == 0)
            {
                receive_message(p_to_w,get_command,buffer_size);  //get ip
                ip = "";
                for(int i = 0; i<sizeof(get_command); i++)
                {
                    ip = ip + get_command[i];
                }
            }
            else if(strcmp(get_command, "/Server Port") == 0)
            {
                receive_message(p_to_w,get_command,buffer_size);  //get ip
                port = atoi(get_command);
            }
            else if(strcmp(get_command, "/Done All") == 0)
            {
                break;
            }
        } //now the worker knows folders + port + ip

        //===================CONNECT TO SERVER==============
        int my_port = 8000 + id;   //if id=2 then the worker listens at port 8002
        int sock;
        char buf[256];
        struct sockaddr_in server, me;
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
        me.sin_family = AF_INET;
        me.sin_addr.s_addr = INADDR_ANY;
        me.sin_port = htons(my_port);

        bind(sock, (struct sockaddr*) &me, sizeof(struct sockaddr_in));

        if (connect(sock, serverptr, sizeof(server)) < 0)
            cout<<"error: connect\n";

        //==================================================================

        //loops for every country/folder
        DirListNode *L = country_list; //Statistics
        while (L != nullptr)
        {
            write_to_socket.lock();

            struct dirent *de;
            string name = L->item->getCountryName();  //folder name
            int nl = name.length();
            char char_name[nl + 1];
            strcpy(char_name, name.c_str());
            string dir = input_dir;   //begin folder
            dir = dir + "/";
            char dire[11];
            strcpy(dire, dir.c_str());
            strcat(dire, char_name);   // make the path input_dir/country

            DIR *countrydir = opendir(dire);

            while ((de = readdir(countrydir))) //open every date file
            {
                int check1 = strcmp(de->d_name, reinterpret_cast<const char *>("."));
                int check2 = strcmp(de->d_name, reinterpret_cast<const char *>(".."));

                if (check1 != 0 && check2 != 0)
                {

                    ListNode *temp = readFromFile(de->d_name, dir, List, L->item->getCountryName());  //create list

                    diseaseHashTable temp_table = diseaseHashTable(10);

                    temp_table.initializeHashTable(temp,100);  //this hash table is exclusively for current date file and helps for stats

                    //find stats:
                    socket_write_str(sock, de->d_name, strlen(de->d_name)); //de->d_name = date
                    socket_write_string(sock, name);  //name = country
                    temp_table.statistics(sock);
                }
            }
            L = L->nextNode;

            write_to_socket.unlock();

        }
        //after this while, worker has a full list of patient records

        write_to_socket.lock();
        socket_write_str(sock, (char*)"/Done", 5);
        write_to_socket.unlock();

        while(1)   //get commands from server
        {
            write_to_socket.lock();

            char get_command[100];
            socket_read_str(sock, get_command, 50);
            cout<<"Command: "<<get_command<<endl;

            LOG("received command '%s' from client", get_command);

            if (strcmp(get_command, "/topk-AgeRanges") == 0)
            {
                socket_read_str(sock, get_command, 100);   //get the whole "/topk-AgeRanges k country disease date1 date2" string

                string elements[6];
                string country, disease, date1, date2, token;
                int k;
                int d = 0;

                for (int k = 0; k < strlen(get_command); k++)   //cut string to parts
                {
                    if (get_command[k] == ' ' || get_command[k] == '\n' || get_command[k] == '\0')
                    {
                        d++;
                    } else
                    {
                        elements[d] += get_command[k];
                    }
                }

                k = stoi(elements[1]);  //elements[0] = "/topk-AgeRanges" so i skip it
                country = elements[2];
                disease = elements[3];
                date1 = elements[4];
                date2 = elements[5];

                string my_country;
                DirListNode *K = country_list;
                while (K != nullptr)
                {
                    my_country = K->item->getCountryName();

                    if (my_country == country) //found the country so this child has to work
                    {
                        age_ranges(k, country, disease, date1, date2, List, sock);
                        break; //there is no reason to search further as we look for only one country
                    }
                    K = K->nextNode;
                }

                socket_write_str(sock, (char*)"/Done", 5);
            }
            else if(strcmp(get_command, "/diseaseFrequency") == 0)
            {
                socket_read_str(sock, get_command, 100);

                string elements[5];
                string country, disease, date1, date2, token;
                int k;
                int d = 0;

                for (int k = 0; k < strlen(get_command); k++)   //cut string to parts
                {
                    if (get_command[k] == ' ' || get_command[k] == '\n' || get_command[k] == '\0')
                    {
                        d++;
                    } else
                    {
                        elements[d] += get_command[k];
                    }
                }

                disease = elements[1];
                date1 = elements[2];
                date2 = elements[3];

                if(elements[4] == "") //no country given
                {
                    string my_country;
                    DirListNode *K = country_list;
                    while (K != nullptr)  //for every country of every worker
                    {
                        my_country = K->item->getCountryName();

                        diseaseFrequency(disease, date1, date2, my_country, List, sock);
                        K = K->nextNode;
                    }
                }
                else
                {
                    country = elements[4];

                    string my_country;
                    DirListNode *K = country_list;
                    while (K != nullptr)
                    {
                        my_country = K->item->getCountryName();

                        if (my_country == country) //found the country so this child has to work
                        {
                            diseaseFrequency(disease, date1, date2, country, List, sock);
                            break; //there is no reason to search further as we look for only one country
                        }
                        K = K->nextNode;
                    }

                }
                socket_write_str(sock, (char*)"/Done", 5);
            }
            else if(strcmp(get_command, "/searchPatientRecord") == 0)
            {
                socket_read_str(sock, get_command, 100);

                string elements[2];
                string country, disease, date1, date2, token;
                string record_id;
                int d = 0;

                for (int k = 0; k < strlen(get_command); k++)   //cut string to parts
                {
                    if (get_command[k] == ' ' || get_command[k] == '\n' || get_command[k] == '\0')
                    {
                        d++;
                    } else
                    {
                        elements[d] += get_command[k];
                    }
                }

                record_id = elements[1];
                ListNode *N = List;
                while(N != nullptr)
                {
                    if(N->item->GetRecordId() == record_id)
                    {
                        N->item->printPatientRecord(sock);
                    }
                    N = N->nextNode;
                }
                socket_write_str(sock, (char*)"/Done", 5);
            }
            else if(strcmp(get_command, "/numPatientAdmissions") == 0 || strcmp(get_command, "/numPatientDischarges") == 0)
            {
                string status;

                if(strcmp(get_command, "/numPatientAdmissions") == 0)
                    status = "ENTRY";
                else if(strcmp(get_command, "/numPatientDischarges") == 0)
                    status = "EXIT";

                socket_read_str(sock, get_command, 100);

                string elements[5];
                string country, disease, date1, date2, token;
                int k;
                int d = 0;
                for (int k = 0; k < strlen(get_command); k++)   //cut string to parts
                {
                    if (get_command[k] == ' ' || get_command[k] == '\n' || get_command[k] == '\0')
                    {
                        d++;
                    } else
                    {
                        elements[d] += get_command[k];
                    }
                }
                disease = elements[1];
                date1 = elements[2];
                date2 = elements[3];

                if(elements[4] == "") //no country given
                {
                    string my_country;
                    DirListNode *K = country_list;
                    while (K != nullptr)  //for every country of every worker
                    {
                        my_country = K->item->getCountryName();
                        num_Patient_Status(status, disease, date1, date2, my_country, List, sock);
                        K = K->nextNode;
                    }
                }
                else
                {
                    country = elements[4];
                    string my_country;
                    DirListNode *K = country_list;
                    while (K != nullptr)
                    {
                        my_country = K->item->getCountryName();

                        if (my_country == country) //found the country so this child has to work
                        {
                            num_Patient_Status(status, disease, date1, date2, country, List, sock);
                            break; //there is no reason to search further as we look for only one country
                        }
                        K = K->nextNode;
                    }

                }
                socket_write_str(sock, (char*)"/Done", 5);
            }

            write_to_socket.unlock();
        }
    }
    //===================================PARENT=========================================
    else   //parent code
    {
        // welcome to the 400-line hell,
        // you  shall not escape from the doom that
        // will become you if you try to understand this
        //
        // the following code is a fine example of
        // abject oriented programming, only true
        // programming paradigm practised through
        // generations of programmers.
        DirListNode *countries;
        string countrs[files_num];
        int fifos[n];  //keeps the fifos of each parent_to_worker
        struct pollfd fdarray[n];
        int status;
        int count = 0;
        int server_port = args.server_port;
        string server_ip = args.server_ip;
        printf("I am the parent process with ID: %lu\n", (long) getpid());

        // divide the files to each worker:
        for (i = 1; i <= n; i++)
        {
            char read_dir[buffer_size];
            strcpy(read_dir, "/Read Directories");

            string first_half = "parent_to_";

            string id_string = to_string(i);   //id to string to concatenate

            first_half = first_half + id_string;  //example 2_to_parent with 2=worker number 2

            int length = first_half.length();

            char fifo[length + 1];

            strcpy(fifo, first_half.c_str());

            int p_to_w = open(fifo, O_WRONLY);
            fifos[i - 1] = p_to_w;   //because worker ids begin from 1

            int message_size = strlen(read_dir) + 1;

            send_message(read_dir, fifos[i - 1], buffer_size); //tell each worker that it is time to get directories

        }

        for (i = 0; i < n; i++)
        {
            //first i need to count the number of country-folders that exist in the input_dir

            string input = "./" + input_dir;  // "./input_dir"
            int len = input.length();
            char input_name[len + 1];
            strcpy(input_name, input.c_str());
            DIR *dr = opendir(input_name);   //input_dir

            if (dr == NULL)
            {
                printf("Could not open current directory");
                return 0;
            }

            struct dirent *de;  // Pointer for directory entry
            int counter = 0;
            int worker;
            while ((de = readdir(dr)))
            {
                int check1 = strcmp(de->d_name, reinterpret_cast<const char *>("."));
                int check2 = strcmp(de->d_name, reinterpret_cast<const char *>(".."));

                if (check1 != 0 && check2 != 0)
                {
                    counter++;        //count how many files the folder contains

                    worker = counter % n;   //the id of the worker that will work on current folder
                    worker++;

                    //tell process with id=worker to work on current folder (de) through pipe:

                    char folder_name[30];
                    strcpy(folder_name, de->d_name);

                    string first_half = "parent_to_";

                    string id_string = to_string(worker);   //id to string to concatenate

                    first_half = first_half + id_string;  //example 2_to_parent with 2=worker number 2

                    int length = first_half.length();

                    char fifo[length + 1];

                    strcpy(fifo, first_half.c_str());

                    int p_to_w = fifos[worker - 1];

                    char bytes[buffer_size];
                    int message_size = strlen(de->d_name) + 1;
                    memcpy(bytes, &message_size, sizeof(int));

                    send_message(folder_name, fifos[worker - 1], buffer_size);  //tell the worker to work on that folder

                }
            }

            for (i = 1; i <= n; i++)
            {
                char done_dir[18] = "/Done";

                string first_half = "parent_to_";

                string second_half = "_to_parent";

                string id_string = to_string(i);   //id to string to concatenate

                first_half = first_half + id_string;  //example parent_to_2 with 2=worker number 2
                second_half = id_string + second_half;  //example 2_to_parent

                int length = first_half.length();

                char fifo1[length + 1];
                char fifo2[length + 1];

                strcpy(fifo1, first_half.c_str());
                strcpy(fifo2, second_half.c_str());

                int p_to_w = fifos[i - 1];
                int w_to_p = open(fifo2, O_RDONLY);

                fdarray[i - 1].fd = w_to_p;   //-1 to start from 0
                fdarray[i - 1].events = POLLIN;

                char bytes[buffer_size];
                int message_size = strlen(done_dir) + 1;
                memcpy(bytes, &message_size, sizeof(int));

                send_message(done_dir, fifos[i - 1], buffer_size);   //tell each worker that it has all direcotires given

            }
            for (i = 1; i <= n; i++)
            {
                char server_command[15] = "/Server IP";

                int l= server_ip.length();
                char ip[l + 1];
                strcpy(ip, server_ip.c_str());

                string first_half = "parent_to_";

                string second_half = "_to_parent";

                string id_string = to_string(i);   //id to string to concatenate

                first_half = first_half + id_string;  //example parent_to_2 with 2=worker number 2
                second_half = id_string + second_half;  //example 2_to_parent

                int length = first_half.length();

                char fifo1[length + 1];
                char fifo2[length + 1];

                strcpy(fifo1, first_half.c_str());
                strcpy(fifo2, second_half.c_str());

                int p_to_w = fifos[i - 1];
                int w_to_p = open(fifo2, O_RDONLY);

                fdarray[i - 1].fd = w_to_p;   //-1 to start from 0
                fdarray[i - 1].events = POLLIN;

                send_message(server_command,fifos[i - 1],buffer_size);
                send_message(ip,fifos[i - 1],buffer_size);
            }
            for (i = 1; i <= n; i++)
            {
                char server_command[18] = "/Server Port";

                int l= sizeof(int);
                char port[l + 1];
                string p = to_string(server_port);
                strcpy(port, p.c_str());

                string first_half = "parent_to_";

                string second_half = "_to_parent";

                string id_string = to_string(i);   //id to string to concatenate

                first_half = first_half + id_string;  //example parent_to_2 with 2=worker number 2
                second_half = id_string + second_half;  //example 2_to_parent

                int length = first_half.length();

                char fifo1[length + 1];
                char fifo2[length + 1];

                strcpy(fifo1, first_half.c_str());
                strcpy(fifo2, second_half.c_str());

                int p_to_w = fifos[i - 1];
                int w_to_p = open(fifo2, O_RDONLY);

                fdarray[i - 1].fd = w_to_p;   //-1 to start from 0
                fdarray[i - 1].events = POLLIN;

                send_message(server_command,fifos[i - 1],buffer_size);
                send_message(port,fifos[i - 1],buffer_size);
            }
            for (i = 1; i <= n; i++)
            {
                char done_dir[18] = "/Done All";

                string first_half = "parent_to_";

                string second_half = "_to_parent";

                string id_string = to_string(i);   //id to string to concatenate

                first_half = first_half + id_string;  //example parent_to_2 with 2=worker number 2
                second_half = id_string + second_half;  //example 2_to_parent

                int length = first_half.length();

                char fifo1[length + 1];
                char fifo2[length + 1];

                strcpy(fifo1, first_half.c_str());
                strcpy(fifo2, second_half.c_str());

                int p_to_w = fifos[i - 1];
                int w_to_p = open(fifo2, O_RDONLY);

                fdarray[i - 1].fd = w_to_p;   //-1 to start from 0
                fdarray[i - 1].events = POLLIN;

                char bytes[buffer_size];
                int message_size = strlen(done_dir) + 1;
                memcpy(bytes, &message_size, sizeof(int));

                send_message(done_dir, fifos[i - 1], buffer_size);   //tell each worker that it has all the info
            }

        }  //gave the workers the needed info

        char stats[buffer_size];
        int bytes_read;
        int rc, h;
        int counter = 0;

        while (counter < n)
        {
            rc = poll(fdarray, n, -1);

            for (i = 0; i < n; i++)
            {
                int k = 0;

                if (fdarray[i].revents == POLLIN)
                {
                    k++;

                    while (1)
                    {
                        receive_message(fdarray[i].fd, stats, buffer_size);

                        if ((strcmp(stats, "/Done") == 0))
                            break;

                        //cout << stats << endl; //print statistics
                    }

                    counter++;
                }

                if (k == rc)
                    break;
            }

        }   //end statistics


        int total=0, success=0;
        while (true)
        {
            string choice = "";
            cout << "Ready to get queries\n";

            char input[100];  //get input from user
            cin.getline(input,sizeof(input));   //i dont use the classic cin because it will stop reading at the first space
            total++;
            for (int i = 0; i <= 100; i++)   //convert the array into string
            {
                if (input[i] == '\n' || input[i] == '\0')   //read until the end of the line
                {
                    input[i] = '\0';
                    break;
                }
                choice = choice + input[i];
            }
            string copy = choice;
            string type = getFirstWord(copy);

            if (type == "/exit")
            {
                for(int j=0; j < n ; j++)
                {
                    kill(childpids[j], SIGKILL);
                }

                for (count = 0; count < n;)
                {
                    if ((waitpid(childpids[count], &status, WNOHANG)) == 0)     //wait for childs
                    {
                        cout<<"Still waiting for child " <<childpids[count]<<" to return\n";
                        sleep(1);
                    } else
                    {
                        count++;
                    }
                }

                for(int l=0; l<n; l++)
                {
                    close(fifos[l]);
                }

                total--;
                int fails = total - success;

                string file = "log_file.";
                string id_s = to_string(getpid());
                string logfile = file + id_s;
                ofstream outfile (logfile);

//                DirListNode *N = countries;
//                while(N != nullptr)
//                {
//                    outfile << N->item->getCountryName() << std::endl;
//                    N = N->nextNode;
//                }

                outfile << "TOTAL "<< total << std::endl;
                outfile << "SUCCESS " << success << std::endl;
                outfile << "FAIL " << fails << std::endl;
                outfile.close();

                cout << "exiting\n";
                break;
            }
            else if (type == "/listCountries")
            {
                for (i = 0; i < n; i++)
                {
                    send_message("/listCountries", fifos[i], buffer_size);
                }
                success++;
            }
                ///topk-AgeRanges 3 China disease date1 date2
            else if (type == "/topk-AgeRanges")
            {
                for (i = 0; i < n; i++)
                {
                    send_message("/topk-AgeRanges", fifos[i], buffer_size);
                    send_message(choice, fifos[i], buffer_size);
                }
                success++;
            }
            else if (type == "/diseaseFrequency")
            {
                for (i = 0; i < n; i++)
                {
                    send_message("/diseaseFrequency", fifos[i], buffer_size);
                    send_message(choice, fifos[i], buffer_size);
                }
                success++;
            }
            else if (type == "/numPatientAdmissions")
            {
                for (i = 0; i < n; i++)
                {
                    send_message("/numPatientAdmissions", fifos[i], buffer_size);
                    send_message(choice, fifos[i], buffer_size);
                }
                success++;
            }
            else if (type == "/numPatientDischarges")
            {
                for (i = 0; i < n; i++)
                {
                    send_message("/numPatientDischarges", fifos[i], buffer_size);
                    send_message(choice, fifos[i], buffer_size);
                }
                success++;
            }
            else if(type == "/searchPatientRecord")
            {
                for (i = 0; i < n; i++)
                {
                    send_message("/searchPatientRecord", fifos[i], buffer_size);
                    send_message(choice, fifos[i], buffer_size);
                }
                success++;
            }
        }
    }

    return 0;
}
