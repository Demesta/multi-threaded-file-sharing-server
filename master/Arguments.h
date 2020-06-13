#ifndef SYSPRO2_ARGUMENTS_H
#define SYSPRO2_ARGUMENTS_H

#include <stdlib.h>
#include <string>

using namespace std;

typedef struct arguments
{
    string input_dir;
    int num_workers;
    int buffer_size;
    string server_ip;
    int server_port;
}Arguments;

void initializeArguments(Arguments &args, char* argv[])
{
    int i;

    for (i = 1; i < 11; i += 2)
    {
        switch (argv[i][1])
        {
            case 'w':
                args.num_workers = atoi(argv[i + 1]);
                break;

            case 'b':
                args.buffer_size = atoi(argv[i + 1]);
                break;

            case 's':
                args.server_ip = argv[i + 1];
                break;

            case 'p':
                args.server_port = atoi(argv[i + 1]);
                break;

            case 'i':
                args.input_dir = argv[i + 1];
                break;

            default:
                exit(1);
                break;
        }
    }
}

#endif //SYSPRO2_ARGUMENTS_H
