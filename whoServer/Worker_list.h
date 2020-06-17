#ifndef WHOSERVER__WORKER_LIST_H_
#define WHOSERVER__WORKER_LIST_H_

typedef struct worker_list
{
  int port;
  struct worker_list* nextNode;
}Worker_list_node;

void insertListNode(int n, Worker_list_node *&L)
{
    Worker_list_node* N = new Worker_list_node;           //dhmiourgw enan neo komvo

    if(L == nullptr)   //if first node
    {
        N->port = n;
        N->nextNode = nullptr;
        L = N;   //N is the new first node of the list
    }
    else
    {
        N->port = n;
        N->nextNode = L;         //vazw ton neo komvo na deixnei ekei pou deixnei o prwtos komvos
        L = N;
    }
}

void print(Worker_list_node *&L)
{
    Worker_list_node *N = L;
    while(N != nullptr)
    {
        cout<<N->port<<endl;
        N = N->nextNode;
    }
}

#endif //WHOSERVER__WORKER_LIST_H_
