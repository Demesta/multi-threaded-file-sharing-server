
#include "utils.h"

string getFirstWord(string s)
{
    string token;
    stringstream tokenize(s);
    string elements[100];

    int i = 0;
    while(getline(tokenize, token, ' '))
    {
        elements[i] = token;
        break;
    }
    return token;
}
