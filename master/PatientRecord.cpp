#include <bits/stdc++.h>
#include "PatientRecord.h"
#include "sockets.h"


PatientRecord::PatientRecord(string s, string file_country, string file_date)
{
    string helper;
    string elements[6]; //will have each element of the string
    stringstream tokenize(s);

    int i = 0;
    while(getline(tokenize, helper, ' '))  //cut string to parts
    {
        elements[i] = helper;
        i++;
    }

    //each element is stored to the new record
    recordID = elements[0];
    status = elements[1];
    patientFirstName = elements[2];
    patientLastName = elements[3];
    diseaseID = elements[4];
    age = elements[5];
    date = file_date;
    country = file_country;

}
const string &PatientRecord::GetRecordId() const
{
    return recordID;
}

void PatientRecord::printPatientRecord(int sock)
{
    string message = this->patientFirstName + " " + this->patientLastName + " " + this->diseaseID + " " + this->country + " " + this->date;
    socket_write_string(sock, message);
    cout<<this->patientFirstName<<" "<<this->patientLastName<<" "<<this->diseaseID<<" "<<this->country<<" "<<this->date<<" "<<endl;
}

const string &PatientRecord::getDiseaseId() const {
    return diseaseID;
}

const string &PatientRecord::getCountry() const
{
    return country;
}

const string &PatientRecord::getStatus() const {
    return status;
}

void PatientRecord::setStatus(const string &status) {
    PatientRecord::status = status;
}

const string &PatientRecord::getAge() const {
    return age;
}

void PatientRecord::setAge(const string &age) {
    PatientRecord::age = age;
}

const string &PatientRecord::GetDate() const {
    return date;
}

void PatientRecord::SetDate(const string &date) {
    PatientRecord::date = date;
}
