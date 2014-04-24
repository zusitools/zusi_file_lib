#include <string>
#include <fstream>

#include <model/autorinfo.hpp>
#include <model/dateiinfo.hpp>

#include "str_leser.hpp"

using namespace std;

unique_ptr<Strecke> StrLeser::liesStrDatei(istream& datei) {
    try {
        this->zeilenNr = 0;
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
    // Die Nachfolger-Nummern jedes Streckenelements.
    vector<vector<streckenelement_nr_t>> nachfolgerNrs;

    while (!datei.eof()) {
        shared_ptr<Streckenelement> element(new Streckenelement());
        StreckenelementRichtungsInfo& richtung =
            element->richtungsInfo[Streckenelement::RICHTUNG_NORM];

        string tmp = liesZeile(datei);
        if (datei.eof()) {
            // EOF wird nach dem ersten fehlgeschlagenen Lesen gesetzt.
            break;
        }

        // Nummer
        element->nr = konvertiereInGanzzahl(tmp);
        element->nrInModul = element->nr;

        // Kilometrierung
        liesZeile(datei);

        // Zählrichtung Kilometrierung
        liesZeile(datei);

        // Landschaftsbezeichnung, # wiederholt von Vorgänger
        liesZeile(datei);

        // Ereignis
        ereignis_nr_t ereignisNr = liesGanzzahl(datei);
        if (ereignisNr != 0) {
            unique_ptr<Ereignis> ereignis(new Ereignis());
            ereignis->ereignisNr = ereignisNr;
        }

        // xyz1, xyz2
        liesZeile(datei);
        liesZeile(datei);
        liesZeile(datei);
        liesZeile(datei);
        liesZeile(datei);
        liesZeile(datei);

        // Überhöhung
        liesZeile(datei);

        // Nachfolger 1-3
        for (size_t i = 0; i < 3; i++) {
            streckenelement_nr_t nachfolgerNr = liesGanzzahl(datei);
            if (nachfolgerNr != 0) {
                if (nachfolgerNrs.size() <= element->nr + 1) {
                    nachfolgerNrs.resize(element->nr + 1);
                }
                nachfolgerNrs.at(element->nr).resize(i + 1);
                nachfolgerNrs.at(element->nr).at(i) = nachfolgerNr;
            }
        }

        // vmax
        liesZeile(datei);

        // reserviert
        liesZeile(datei);

        // Bahnsteig/Betriebsstelle
        liesZeile(datei);

        // Flags
        liesZeile(datei);

        // Oberleitungsspannung
        liesZeile(datei);

        // Signal
        liesFahrstrSignal(datei, element);

        // Kombisignal
        liesKombiSignal(datei, element);

        // Register
        liesZeile(datei);

        if (strecke->streckenelemente.size() <= element->nr + 1) {
            strecke->streckenelemente.resize(element->nr + 1);
        }
        strecke->streckenelemente.at(element->nr) = element;
    }

    // Verknüpfe Streckenelemente mit ihren Nachfolgern und Vorgängern.
    for (size_t i = 0; i < strecke->streckenelemente.size(); i++) {
        if (!strecke->streckenelemente.at(i) || i >= nachfolgerNrs.size()) {
            continue;
        }

        shared_ptr<Streckenelement>& element = strecke->streckenelemente.at(i);
        element->nachfolger[Streckenelement::RICHTUNG_NORM].resize(nachfolgerNrs.at(i).size());
        for (size_t j = 0; j < nachfolgerNrs.at(i).size(); j++) {
            streckenelement_nr_t elemNr = nachfolgerNrs.at(i).at(j);
            if (elemNr == 0) {
                continue;
            }
            element->nachfolger[Streckenelement::RICHTUNG_NORM].at(j) =
                strecke->streckenelemente.at(elemNr);
            strecke->streckenelemente.at(elemNr)->nachfolger[Streckenelement::RICHTUNG_GEGEN].
                push_back(element);
        }
    }
}

void StrLeser::liesFahrstrSignal(istream& datei, shared_ptr<Streckenelement>& element) {
    string tmp = liesZeile(datei);
    if (tmp.compare("#") == 0) {
        return;
    }
}

void StrLeser::liesKombiSignal(istream& datei, shared_ptr<Streckenelement>& element) {
    string tmp = liesZeile(datei);
    if (tmp.compare("#") == 0) {
        return;
    }
}
