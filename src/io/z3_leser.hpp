#ifndef SRC_IO_Z3_LESER_HPP
#define SRC_IO_Z3_LESER_HPP

#include <iostream>
#include <fstream>
#include <memory>

#include "model/dateiinfo.hpp"
#include "lib/rapidxml-1.13/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

// Basisklasse für Dateileser, die das Zusi-3-Datenformat lesen.
template<typename R>
class Z3Leser {

protected:
    DateiInfo liesDateiInfo(xml_node<>& wurzel) {
        DateiInfo result = { };
        xml_node<> *dateiInfoNode = wurzel.first_node("Info");
        if (dateiInfoNode != nullptr) {
            xml_attribute<> *versionAttr = dateiInfoNode->first_attribute("Version");
            if (versionAttr != nullptr) {
                result.formatVersion = versionAttr->value();
            }
            xml_attribute<> *minVersionAttr = dateiInfoNode->first_attribute("MinVersion");
            if (minVersionAttr != nullptr) {
                result.formatMinVersion = minVersionAttr->value();
            }
        }

        return result;
    }

    virtual unique_ptr<R> parseWurzel(xml_node<>& wurzel) = 0; // Hier wird das dateispezifische Parsen implementiert.

public:
    unique_ptr<R> liesDatei(istream& stream) {
        stream.seekg(0, ios::end);
        size_t size = stream.tellg();
        stream.seekg(0);

        vector<char> buffer(size + 1);
        stream.read(&buffer.front(), size);
        buffer[size] = '\0';

        xml_document<> dok;
        dok.parse<0>(&buffer[0]);

        xml_node<> *wurzel = dok.first_node("Zusi");
        if (wurzel == nullptr)
        {
            throw std::invalid_argument("Keine gültige Zusi-Datei (Zusi-Wurzelknoten nicht gefunden)");
        }

        return this->parseWurzel(*wurzel);
    }

    unique_ptr<R> liesDateiMitDateiname(const string dateiname) {
        ifstream datei(dateiname, std::ios::binary);
        return this->liesDatei(datei);
    }
};

#endif
