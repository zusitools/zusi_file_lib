#include "st3_leser.hpp"

#include <iostream>

#include "lib/rapidxml-1.13/rapidxml.hpp"

unique_ptr<Strecke> St3Leser::liesSt3Datei(istream& datei) {
    unique_ptr<Strecke> result(new Strecke());

    vector<char> buffer((istreambuf_iterator<char>(datei)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    xml_document<> dok;
    dok.parse<0>(&buffer[0]);

    xml_node<> *wurzel = dok.first_node("Zusi");
    if (wurzel != nullptr)
    {
        result->dateiInfo = this->liesDateiInfo(*wurzel);
    }

    return result;
}
