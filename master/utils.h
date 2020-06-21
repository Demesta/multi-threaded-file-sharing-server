
#ifndef SYSPRO2_UTILS_H
#define SYSPRO2_UTILS_H

#include <iostream>
#include <string>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;
typedef struct listNode ListNode;

string ReverseDateFormat(string);
bool datesAreWrong(string, string);
ListNode* readFromFile(string, string, ListNode *&L, string);
string getFirstWord(string);
string getLastWord(string);
string removeFirstWord(string);
void send_message(string, int, int);
void receive_message(int, char*, int);
void age_ranges(int, string, string, string, string, ListNode *&L, int);
void diseaseFrequency(string, string, string, string, ListNode *&L, int);
void num_Patient_Status(string, string, string, string, string, ListNode *&L, int);


#endif //SYSPRO2_UTILS_H
