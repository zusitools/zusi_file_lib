#include <string>
#include <fstream>

#include <model/autorinfo.hpp>
#include <model/dateiinfo.hpp>

#include "str_leser.hpp"

using namespace std;

unique_ptr<Strecke> StrLeser::liesStrDatei(istream& datei) {
    try {
        this->zeilenNr = 1;
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
        Z2Leser::liesMehrzeiligenString(datei); // UTM-Info, nicht Version 1.1

        // Geschwindigkeits-Multiplikator 3,6 (ohne Funktion); ab Zusi 2.4.5.0: Signalhaltabstand.
        try {
            result->signalHaltabstand = Z2Leser::liesGanzzahl(datei);
        } catch (invalid_argument) {
            result->signalHaltabstand = 0;
        }

        Z2Leser::liesZeile(datei);  // Pfad Landschaftsdatei

        // Referenzpunkte: Einsatzelemente
        Z2Leser::liesMehrzeiligenString(datei);

        // Streckenfeste Standorte (Blickpunkte).
        tmp = Z2Leser::liesZeile(datei);
        while (tmp.compare("#") != 0) {
            unique_ptr<Blickpunkt> blickpunkt(new Blickpunkt());
            blickpunkt->position.loc.x = Z2Leser::konvertiereInGleitkommazahl(tmp);
            blickpunkt->position.loc.y = Z2Leser::liesGleitkommazahl(datei);
            blickpunkt->position.loc.z = Z2Leser::liesGleitkommazahl(datei);

            // Zwischenspeichern der Koordinaten in lokale Variablen -- wenn man die Aufrufe an
            // liesGleitkommazahl in die Konstruktorparameter packt, ist die Aufrufreihenfolge
            // undefiniert.
            koordinate_t x, y, z;
            x = Z2Leser::liesGleitkommazahl(datei);
            y = Z2Leser::liesGleitkommazahl(datei);
            z = Z2Leser::liesGleitkommazahl(datei);
            blickpunkt->position.setRichtungAlsRot(Punkt3D(x, y, z));

            blickpunkt->name = Z2Leser::liesZeile(datei);

            result->blickpunkte.push_back(std::move(blickpunkt));
            tmp = Z2Leser::liesZeile(datei);
        }

        // Streckenelemente
        this->liesStreckenelemente(datei, result);

        result->dateiInfo = std::move(dateiInfo);
        return result;
    } catch (const std::exception& ex) {
        throw invalid_argument("Fehler beim Einlesen von Zeile " + to_string(this->zeilenNr) + ": "
            + ex.what());
    }
}

unique_ptr<Strecke> StrLeser::liesStrDateiMitDateiname(const string dateiname) {
    ifstream datei(dateiname);
    return StrLeser::liesStrDatei(datei);
}

void StrLeser::liesStreckenelemente(istream& datei, unique_ptr<Strecke>& strecke) {

}

