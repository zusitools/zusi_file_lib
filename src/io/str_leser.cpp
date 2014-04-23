#include <string>
#include <iostream>

#include <model/autorinfo.hpp>
#include <model/dateiinfo.hpp>

#include "str_leser.hpp"

using namespace std;

unique_ptr<Strecke> StrLeser::liesStrDatei(istream& datei) {
    unique_ptr<Strecke> result(new Strecke());

    string tmp;
    getline(datei, tmp);  // Zusi-Version

    // Autor-Information.
    unique_ptr<AutorInfo> autorInfo(new AutorInfo());
    autorInfo->name = Z2Leser::liesZeile(datei);

    unique_ptr<DateiInfo> dateiInfo(new DateiInfo());
    dateiInfo->autorInfo.push_back(std::move(autorInfo));

    getline(datei, tmp);  // Breitengrad
    getline(datei, tmp);  // Rekursionstiefe
    result->gebietsschema = Z2Leser::liesZeile(datei);  // Gebietsschema

    dateiInfo->beschreibung = Z2Leser::liesMehrzeiligenString(datei); // Freier Text
    tmp = Z2Leser::liesMehrzeiligenString(datei); // UTM-Info, nicht Version 1.1

    getline(datei, tmp);  // Geschwindigkeits-Multiplikator
    getline(datei, tmp);  // Pfad Landschaftsdatei

    // Referenzpunkte: Einsatzelemente
    // Streckenfeste Standorte
    // Streckenelemente

    result->dateiInfo = std::move(dateiInfo);
    return result;
}
