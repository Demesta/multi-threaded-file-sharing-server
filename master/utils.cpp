#include <iostream>
#include <unistd.h>
#include "utils.h"
#include "PatientRecord.h"
#include "Records_list.h"
#include "Dir_list.h"

string ReverseDateFormat(string s)
{
    if(s == "-")
    {
        return s;
    }

    string token;
    stringstream tokenize(s);
    string elements[3];
    s.clear();  //empty string "entry" to add the new one without the "-"

    int i = 0;
    while(getline(tokenize, token, '-'))  //tokenize the string keeping only the numbers
    {
        elements[i] = token;
        i++;
    }
    s+= elements[2];   //add each string in reverse to achieve the YY-MM-DD format for correct comparing
    s+= "-";
    s+= elements[1];
    s+= "-";
    s+= elements[0];

    return s;
}

bool datesAreWrong(string entry, string exit)
{
    //reverse entry date
    entry = ReverseDateFormat(entry);

    //Repeat for exit date:
    exit = ReverseDateFormat(exit);

    //Compare:
    if(exit != "-")
    {
        if(entry > exit)  //if entry is greater than exit then it is wrong and it returns true for "dates are wrong" question
        {
            cout<<"Error: Exit date can not be before entry date\n";
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

}

void send_message(string message_to_send, int fifo, int buffer_size)
{
    int message_size = message_to_send.length();
    message_size++;
    write(fifo, &message_size, sizeof(int));

    char message[buffer_size];
    strcpy(message, message_to_send.c_str());
    write(fifo, message, message_size);
}

void receive_message(int fifo, char* message, int buffsize)
{
    int bytes_to_read = 0;
    int bytes_read = 0;
    char part_read[buffsize];
    bzero(message, buffsize);

    while( read(fifo, &bytes_to_read, sizeof(int))<= 0){}

    while((bytes_read = read(fifo, part_read, bytes_to_read))<= 0){}
    strcpy(message, part_read);

    while(bytes_read < bytes_to_read)
    {
        while((bytes_read += read(fifo, part_read, bytes_to_read))<= 0){}
        strcat(message, part_read);
    }

}

void diseaseFrequency(string disease, string date1, string date2, string country, ListNode *&L)
{
    date1 = ReverseDateFormat(date1);
    date2 = ReverseDateFormat(date2);

    int count = 0;

    ListNode *P = L;  //List with all the records this child works on

    while(P != nullptr)  //search every record
    {
        string this_country = P->item->getCountry();
        string this_disease = P->item->getDiseaseId();
        string this_date = P->item->GetDate();

        this_date = ReverseDateFormat(this_date);

        if(this_country == country)
        {
            if(this_disease == disease)
            {
                if(this_date >= date1 && this_date <= date2)
                {
                    count++;
                }
            }
        }
        P=P->nextNode;
    }

    cout<<country<<" "<<count<<endl;

}

void num_Patient_Status(string status, string disease, string date1, string date2, string country, ListNode *&L)
{
    date1 = ReverseDateFormat(date1);
    date2 = ReverseDateFormat(date2);

    int count = 0;

    ListNode *P = L;  //List with all the records this child works on

    while(P != nullptr)  //search every record
    {
        string this_country = P->item->getCountry();
        string this_disease = P->item->getDiseaseId();
        string this_date = P->item->GetDate();
        string this_status = P->item->getStatus();

        this_date = ReverseDateFormat(this_date);

        if(this_country == country)
        {
            if(this_disease == disease)
            {
                if(this_date >= date1 && this_date <= date2)
                {
                    if(this_status == status)
                    {
                        count++;
                    }

                }
            }
        }
        P=P->nextNode;
    }

    cout<<country<<" "<<count<<endl;

}

void age_ranges(int k, string country, string disease, string date1, string date2, ListNode *&L)
{
    date1 = ReverseDateFormat(date1);
    date2 = ReverseDateFormat(date2);

    int ages_20 = 0;
    int ages_40 = 0;
    int ages_60 = 0;
    int ages_60plus = 0;
    int sort_array[4];

    ListNode *P = L;  //List with all the records this child works on

    while(P != nullptr)  //search every record
    {
        string this_country = P->item->getCountry();
        string this_disease = P->item->getDiseaseId();
        string this_date = P->item->GetDate();
        string this_age1 = P->item->getAge();
        int this_age = stoi(this_age1);

        this_date = ReverseDateFormat(this_date);

        if(this_country == country)
        {
            if(this_disease == disease)
            {
                if(this_date >= date1 && this_date <= date2)
                {
                    if(this_age <= 20)
                    {
                        ages_20++;
                    }
                    else if(this_age > 20 && this_age <= 40)
                    {
                        ages_40++;
                    }
                    else if(this_age > 40 && this_age <= 60)
                    {
                        ages_60++;
                    }
                    else if(this_age > 60)
                    {
                        ages_60plus++;
                    }
                }
            }

        }
        P=P->nextNode;
    }   //after this while i have the amount of patients (with same country, disease, date) for each age category

    int sum = ages_20 + ages_40 + ages_60 + ages_60plus;

    sort_array[0] = ages_20;
    sort_array[1] = ages_40;
    sort_array[2] = ages_60;
    sort_array[3] = ages_60plus;

    int a1=ages_20;
    int a2=ages_40;
    int a3=ages_60;
    int a4=ages_60plus;

    //cout<<ages_20<<" "<<ages_40<<" "<<ages_60<<" "<<ages_60plus<<" "<<sum<<endl;

    int s = sizeof(sort_array) / sizeof(sort_array[0]);
    sort(sort_array, sort_array + s); //sort the array of numbers

    for (int z = 3; z > (3-k); z--)
    {
        if(sort_array[z] == a1)
        {
            float result = ( (float)  sort_array[z] / (float)  sum   );
            cout<<"0-20: "<< result * 100<<"%"<<endl;
        }
        else if(sort_array[z] == a2)
        {
            float result = ( (float)  sort_array[z] / (float)  sum   );
            cout<<"21-40: "<< result * 100<<"%"<<endl;
        }
        else if(sort_array[z] == a3)
        {
            float result = ( (float)  sort_array[z] / (float)  sum   );
            cout<<"41-60: "<< result * 100<<"%"<<endl;
        }
        else if(sort_array[z] == a4)
        {
            float result = ( (float)  sort_array[z] / (float)  sum   );
            cout<<"60+: "<< result * 100 <<"%"<<endl;
        }

    }
}

ListNode* readFromFile(string file_name, string dir, ListNode *&L, string country)
{
    ListNode *Temp = nullptr;

    int nl = file_name.length();
    int cl = country.length();
    char char_name[nl+1];
    char char_country[cl+1];
    strcpy(char_name, file_name.c_str());
    strcpy(char_country, country.c_str());
    char dire[100];
    strcpy(dire, dir.c_str());
    strcat(dire, char_country);
    strcat(dire, "/");
    strcat(dire, char_name);

    ifstream file(dire);

    string s;

    while(getline(file, s))// read each line
    {
        PatientRecord *record = new PatientRecord(s, country, file_name); //create new record  (file name is the date)

        insertListNode(record, L);    //store it to list

        insertListNode(record, Temp);  //local list for this txt
    }


    return Temp;
}

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

string getLastWord(string s)
{
    std::string token ;

    std::istringstream( { s.rbegin(), s.rend() } ) >> token ; // #include <sstream>

    return { token.rbegin(), token.rend() } ;
}

string removeFirstWord(string s)
{
    string helper;
    string space = " ";
    string elements[8]; //will have each element of the string
    stringstream tokenize(s);
    s.clear();

    int i = 0;
    while(getline(tokenize, helper, ' '))  //cut string to parts
    {
        elements[i] = helper;
        i++;
    }
    //ignore elements[0] cause it has the "insertPatientRecord" word
    s+= elements[1];
    s+= space;         //space prevents all words being stuck together
    s+= elements[2];
    s+= space;
    s+= elements[3];
    s+= space;
    s+= elements[4];
    s+= space;
    s+= elements[5];
    s+= space;
    s+= elements[6];
    s+= space;

    if(elements[7] == "")  //if there is no exit date
    {
        s+= "-";
    }
    else
    {
        s+= elements[7];
    }

    return s;
}

