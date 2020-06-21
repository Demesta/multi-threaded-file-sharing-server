#ifndef SYSPRO2_PATIENTRECORD_H
#define SYSPRO2_PATIENTRECORD_H

#include <string>

using namespace std;

class PatientRecord
{
private:
    string recordID;
    string status;
    string patientFirstName;
    string patientLastName;
    string diseaseID;
    string age;

    string country;
    string date;

public:
    PatientRecord(string, string, string);
    void printPatientRecord(int);

    const string &getDiseaseId() const;
    const string &getStatus() const;
    void setStatus(const string &status);
    const string &getAge() const;
    void setAge(const string &age);
    const string &getCountry() const;
    const string &GetRecordId() const;
    const string &GetDate() const;
    void SetDate(const string &date);
};



#endif //SYSPRO2_PATIENTRECORD_H
