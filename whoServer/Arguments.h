#ifndef WHOSERVER__ARGUMENTS_H_
#define WHOSERVER__ARGUMENTS_H_


#include <stdlib.h>
#include <string>

using namespace std;

typedef struct arguments
{
  int query_port;
  int statistics_port;
  int num_threads;
  int buffer_size;
}Arguments;

void initializeArguments(Arguments &args, char* argv[])
{
    int i;

    for (i = 1; i < 9; i += 2)
    {
//        cout << "processing argument " << argv[i][1] << endl;
        switch (argv[i][1])
        {
            case 'q':
                args.query_port = atoi(argv[i + 1]);
                break;

            case 's':
                args.statistics_port = atoi(argv[i + 1]);
                break;

            case 'w':
                args.num_threads = atoi(argv[i + 1]);
                break;

            case 'b':
                args.buffer_size = atoi(argv[i + 1]);
                break;

            default:
                cout << "invalid usage: unknown flag " << argv[i] << endl;
                exit(1);
                break;
        }
    }
}


#endif //WHOSERVER__ARGUMENTS_H_
