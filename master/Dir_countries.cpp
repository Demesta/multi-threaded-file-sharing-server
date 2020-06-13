//
// Created by demesta on 26/5/20.
//
#include "Dir_countries.h"

Dir_countries::Dir_countries(string s)
{
    country_name = s;
}

const string &Dir_countries::getCountryName() const {
    return country_name;
}

void Dir_countries::setCountryName(const string &countryName) {
    country_name = countryName;
}
