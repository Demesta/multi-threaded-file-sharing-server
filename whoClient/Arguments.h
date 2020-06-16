#ifndef SYSPRO2_ARGUMENTS_H
#define SYSPRO2_ARGUMENTS_H

#include <stdlib.h>
#include <string>

using namespace std;

typedef struct arguments
{
  string query_file;
  int num_threads;
  int server_port;
  string server_ip;

}Arguments;

void initializeArguments(Arguments &args, char* argv[])
{
    int i;

    for (i = 1; i < 9; i += 2)
    {
        switch (argv[i][1])
        {
            case 'q':
                args.query_file = argv[i + 1];
                break;

            case 'w':
                args.num_threads = atoi(argv[i + 1]);
                break;

            case 's':
                if (argv[i][2] == 'p')
                {
                    args.server_port = atoi(argv[i + 1]);
                }
                else
                {
                    args.server_ip = argv[i + 1];
                }
                break;

            default:
                exit(1);
                break;
        }
    }
}

#endif //SYSPRO2_ARGUMENTS_H
