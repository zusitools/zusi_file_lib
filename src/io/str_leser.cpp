#include <string>
#include <iostream>

#include <model/autorinfo.hpp>
#include <model/dateiinfo.hpp>

#include "str_leser.hpp"

using namespace std;

unique_ptr<Strecke> StrLeser::liesStrDatei(istream& datei) {
    unique_ptr<Strecke> result(new Strecke());

    string tmp;

    // Dateiformat-Version.
    unique_ptr<DateiInfo> dateiInfo(new DateiInfo());
    dateiInfo->formatVersion = Z2Leser::liesZeile(datei);
    dateiInfo->formatMinVersion = dateiInfo->formatVersion;

    // Autor-Information.
    unique_ptr<AutorInfo> autorInfo(new AutorInfo());
    autorInfo->name = Z2Leser::liesZeile(datei);
    dateiInfo->autorInfo.push_back(std::move(autorInfo));

    result->breitengrad = Z2Leser::liesGanzzahl(datei);  // Breitengrad
    result->rekursionstiefe = Z2Leser::liesGanzzahl(datei);  // Rekursionstiefe
    result->gebietsschema = Z2Leser::liesZeile(datei);  // Gebietsschema

    dateiInfo->beschreibung = Z2Leser::liesMehrzeiligenString(datei); // Freier Text
    tmp = Z2Leser::liesMehrzeiligenString(datei); // UTM-Info, nicht Version 1.1

    // Geschwindigkeits-Multiplikator 3,6 (ohne Funktion); ab Zusi 2.4.5.0: Signalhaltabstand.
    try {
        result->signalHaltabstand = Z2Leser::liesGanzzahl(datei);
    } catch (invalid_argument) {
        result->signalHaltabstand = 0;
    }

    getline(datei, tmp);  // Pfad Landschaftsdatei

    // Referenzpunkte: Einsatzelemente
    // Streckenfeste Standorte
    // Streckenelemente

    result->dateiInfo = std::move(dateiInfo);
    return result;
}
