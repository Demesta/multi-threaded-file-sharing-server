
#ifndef SYSPRO2_DIR_COUNTRIES_H
#define SYSPRO2_DIR_COUNTRIES_H
#include <string>
#include "diseaseHashTable.h"
using namespace std;

class Dir_countries {

    private:
        string country_name;
        //diseaseHashTable d_table;

    public:
        const string &getCountryName() const;

        void setCountryName(const string &countryName);

        Dir_countries(string);

};


#endif //SYSPRO2_DIR_COUNTRIES_H
