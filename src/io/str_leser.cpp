#include <fstream>
#include <string>

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
        dateiInfo->formatVersion = Z2Leser::liesZeile("Dateiformat-Version", datei);

        if (dateiInfo->formatVersion == "1.1" || dateiInfo->formatVersion == "2.0" || dateiInfo->formatVersion == "2.1") {
            dateiInfo->formatMinVersion = "1.1";
        } else {
            dateiInfo->formatMinVersion = dateiInfo->formatVersion;
        }

        // Autor-Information.
        unique_ptr<AutorInfo> autorInfo(new AutorInfo());
        autorInfo->name = Z2Leser::liesZeile("Autor-Name", datei);
        dateiInfo->autorInfo.push_back(std::move(autorInfo));

        result->breitengrad = Z2Leser::liesGanzzahl("Breitengrad", datei);
        result->rekursionstiefe = Z2Leser::liesGanzzahl("Rekursionstiefe", datei);
        result->gebietsschema = Z2Leser::liesZeile("Gebietsschema", datei);

        dateiInfo->beschreibung = Z2Leser::liesMehrzeiligenString("Dateibeschreibung", datei);
        result->dateiInfo = std::move(dateiInfo);

        if (result->dateiInfo->formatMinVersion != "1.1") {
            Z2Leser::liesMehrzeiligenString("UTM-Info", datei);
        }

        // Geschwindigkeits-Multiplikator 3,6 (ohne Funktion); ab Zusi 2.4.5.0: Signalhaltabstand.
        try {
            result->signalHaltabstand =
                Z2Leser::liesGanzzahl("Geschwindigkeitsmultiplikator/Haltabstand", datei);
        } catch (invalid_argument) {
            result->signalHaltabstand = 0;
        }

        Z2Leser::liesZeile("Pfad Landschaftsdatei", datei);

        // Referenzpunkte: Einsatzelemente
        tmp = liesZeile("Aufgleispunkte", datei);
        while (tmp != "#") {
            unique_ptr<Referenzpunkt> referenzpunkt(new Referenzpunkt());
            if (result->dateiInfo->formatMinVersion != "1.1") {
                referenzpunkt->referenzNr = konvertiereInGanzzahl("Aufgleisreferenz-Nr.", tmp);
                referenzpunkt->referenzNrInModul = referenzpunkt->referenzNr;
                liesZeile("Aufgleiselement-Nr.", datei);
                referenzpunkt->beschreibung = liesZeile("Aufgleispunkt-Beschreibung", datei);

                if (result->referenzpunkte.size() <= referenzpunkt->referenzNr) {
                    result->referenzpunkte.resize(referenzpunkt->referenzNr + 1);
                }
                result->referenzpunkte.at(referenzpunkt->referenzNr) = std::move(referenzpunkt);
            } else {
                referenzpunkt->referenzNr = result->referenzpunkte.size();
                referenzpunkt->referenzNrInModul = referenzpunkt->referenzNr;
                konvertiereInGanzzahl("Aufgleiselement-Nr.", tmp);
                result->referenzpunkte.push_back(std::move(referenzpunkt));
            }

            tmp = liesZeile("Aufgleispunkte", datei);
        }

        // Streckenfeste Standorte (Blickpunkte).
        tmp = Z2Leser::liesZeile("Blickpunkte", datei);
        while (tmp != "#") {
            unique_ptr<Blickpunkt> blickpunkt(new Blickpunkt());
            blickpunkt->position.loc.x = Z2Leser::konvertiereInGleitkommazahl("Blickpunkt-Position X", tmp);
            blickpunkt->position.loc.y = Z2Leser::liesGleitkommazahl("Blickpunkt-Position Y", datei);
            blickpunkt->position.loc.z = Z2Leser::liesGleitkommazahl("Blickpunkt-Position Z", datei);

            // Zwischenspeichern der Koordinaten in lokale Variablen -- wenn man die Aufrufe an
            // liesGleitkommazahl in die Konstruktorparameter packt, ist die Aufrufreihenfolge
            // undefiniert.
            koordinate_t x, y, z;
            x = Z2Leser::liesGleitkommazahl("Blickpunkt-Rotation X", datei);
            y = Z2Leser::liesGleitkommazahl("Blickpunkt-Rotation Y", datei);
            z = Z2Leser::liesGleitkommazahl("Blickpunkt-Rotation Z", datei);
            blickpunkt->position.setRichtungAlsRot(Punkt3D(x, y, z));
            blickpunkt->name = Z2Leser::liesZeile("Blickpunkt-Name", datei);

            result->blickpunkte.push_back(std::move(blickpunkt));
            tmp = Z2Leser::liesZeile("Blickpunkte", datei);
        }

        // Streckenelemente
        this->aktElement = "Streckenelemente";
        this->liesStreckenelemente(datei, result);

        return result;
    } catch (const std::exception& ex) {
        throw invalid_argument("Fehler beim Einlesen von Zeile " + to_string(this->zeilenNr) + " ("
            + this->aktElement + "): " + ex.what());
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

        string tmp = liesZeile("Streckenelement", datei);
        if (datei.eof()) {
            // EOF wird nach dem ersten fehlgeschlagenen Lesen gesetzt.
            break;
        }

        element->nr = konvertiereInGanzzahl("Streckenelement-Nummer", tmp);
        element->nrInModul = element->nr;

        liesZeile("Kilometrierung", datei);
        liesZeile("Zählrichtung Kilometrierung", datei);
        liesZeile("Landschaftsbezeichnung", datei); // # wiederholt von Vorgänger

        ereignis_nr_t ereignisNr = liesGanzzahl("Ereignis-Nummer", datei);
        if (ereignisNr != 0) {
            richtung.ereignisse.push_back(neuesEreignis(ereignisNr));
        }

        // Element-Koordinaten
        element->p1.x = liesGleitkommazahl("Element-Koordinate X1", datei);
        element->p1.y = liesGleitkommazahl("Element-Koordinate Y1", datei);
        element->p1.z = liesGleitkommazahl("Element-Koordinate Z1", datei);
        element->p2.x = liesGleitkommazahl("Element-Koordinate X2", datei);
        element->p2.y = liesGleitkommazahl("Element-Koordinate Y2", datei);
        element->p2.z = liesGleitkommazahl("Element-Koordinate Z2", datei);

        liesZeile("Überhöhung", datei);

        // Nachfolger 1-3
        for (size_t i = 0; i < 3; i++) {
            streckenelement_nr_t nachfolgerNr = liesGanzzahl("Nachfolger " + to_string(i), datei);
            if (nachfolgerNr != 0) {
                if (nachfolgerNrs.size() <= element->nr + 1) {
                    nachfolgerNrs.resize(element->nr + 1);
                }
                nachfolgerNrs.at(element->nr).resize(i + 1);
                nachfolgerNrs.at(element->nr).at(i) = nachfolgerNr;
            }
        }

        liesZeile("Element-vMax", datei);
        if (strecke->dateiInfo->formatMinVersion != "1.1") {
            liesZeile("Reservierter Streckenelement-Eintrag", datei);
        }
        liesZeile("Name Bahnsteig/Betriebsstelle", datei);

        string flags = liesZeile("Funktionsflags", datei);
        for (char& flag : flags) {
            switch (flag) {
                case 'T':
                    element->flags.insert(StreckenelementFlag::Tunnel);
                    break;
                case 'W':
                    element->flags.insert(StreckenelementFlag::Weichenbausatz);
                    break;
                case 'S':
                    element->flags.insert(StreckenelementFlag::Steinbruecke);
                    break;
                case 'B':
                    element->flags.insert(StreckenelementFlag::Stahlbruecke);
                    break;
                case 'K':
                    element->flags.insert(StreckenelementFlag::KeineGleisfunktion);
                    break;
                case 'H':
                    element->flags.insert(StreckenelementFlag::KeinSchienenbau);
                    break;
                case 'G':
                    element->flags.insert(StreckenelementFlag::KeinWeichenbau);
                    break;
            }
        }

        liesZeile("Oberleitungsspannung", datei);

        this->aktElement = "Fahrstraßensignal";
        liesFahrstrSignal(datei, element, strecke);

        if (strecke->dateiInfo->formatMinVersion != "1.1") {
            richtung.signal = liesKombiSignal(datei);
        } else {
            this->aktElement = "Vorsignal";
            liesVorsignal(datei, element);
            this->aktElement = "Hauptsignal";
            liesHauptsignal(datei, element);
        }

        liesZeile("Fahrstraßenregister-Nummer", datei);

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

void StrLeser::liesFahrstrSignal(istream& datei, shared_ptr<Streckenelement>& element,
        unique_ptr<Strecke>& strecke) {
    string tmp = liesZeile(datei);
    if (tmp == "#") {
        return;
    }

    konvertiereInGleitkommazahl("Fahrstraßensignal-Standort X", tmp);
    liesGleitkommazahl("Fahrstraßensignal-Standort Y", datei);
    liesGleitkommazahl("Fahrstraßensignal-Standort Z", datei);
    liesGleitkommazahl("Fahrstraßensignal-Standort Rotation X", datei);
    liesGleitkommazahl("Fahrstraßensignal-Standort Rotation Y", datei);
    liesGleitkommazahl("Fahrstraßensignal-Standort Rotation Z", datei);

    if (strecke->dateiInfo->formatMinVersion != "1.1") {
        liesZeile("Fahrstraßensignal: ohne Funktion 1", datei);
        liesZeile("Fahrstraßensignal: ohne Funktion 2", datei);
        liesZeile("Fahrstraßensignal: ohne Funktion 3", datei);
        liesZeile("Fahrstraßensignal: ohne Funktion 4", datei);
        liesZeile("Fahrstraßensignal: ohne Funktion 5", datei);
        liesZeile("Fahrstraßensignal: ohne Funktion 6", datei);
    }

    liesZeile("Fahrstraßensignal: Landschaftsdatei", datei);

    if (strecke->dateiInfo->formatMinVersion != "1.1") {
        liesZeile("Fahrstraßensignal: ohne Funktion 7", datei);
    }

    liesMehrzeiligenString("Fahrstraßensignal: Signalbilder", datei);

    liesZeile("Fahrstraßensignal: Ereignis", datei);
    liesZeile("Fahrstraßensignal: Angekündigte Geschwindigkeit", datei);

    if (strecke->dateiInfo->formatMinVersion != "1.1") {
        liesZeile("Fahrstraßensignal: Verweis auf Masterelement", datei);
    }
}

unique_ptr<KombiSignal> StrLeser::liesKombiSignal(istream& datei) {
    string tmp = liesZeile("Kombinationssignal", datei);
    if (tmp == "#") {
        return nullptr;
    }

    unique_ptr<KombiSignal> signal(new KombiSignal());

    konvertiereInGleitkommazahl("Kombinationssignal-Standort X1", tmp);
    liesGleitkommazahl("Kombinationssignal-Standort Y1", datei);
    liesGleitkommazahl("Kombinationssignal-Standort Z1", datei);
    liesGleitkommazahl("Kombinationssignal-Standort Rotation X1", datei);
    liesGleitkommazahl("Kombinationssignal-Standort Rotation Y1", datei);
    liesGleitkommazahl("Kombinationssignal-Standort Rotation Z1", datei);
    liesGleitkommazahl("Kombinationssignal-Standort X2", datei);
    liesGleitkommazahl("Kombinationssignal-Standort Y2", datei);
    liesGleitkommazahl("Kombinationssignal-Standort Z2", datei);
    liesGleitkommazahl("Kombinationssignal-Standort Rotation X2", datei);
    liesGleitkommazahl("Kombinationssignal-Standort Rotation Y2", datei);
    liesGleitkommazahl("Kombinationssignal-Standort Rotation Z2", datei);

    liesMehrzeiligenString("Kombinationssignal-Landschaftsdateien", datei);
    signal->betriebsstelle = liesZeile("Kombinationssignal-Blockname", datei);
    signal->signalbezeichnung = liesZeile("Kombinationssignal-Gleis", datei);

    size_t anzahlHsigZeilen = liesGanzzahl("Kombinationssignal Anzahl Hsig-Zeilen", datei) + 1;
    size_t anzahlVsigSpalten = liesGanzzahl("Kombinationssignal Anzahl Vsig-Spalten", datei) + 1;

    this->aktElement = "Kombinationssignal: Fahrziele, Vsig-Geschwindigkeiten, Matrix, Ersatzzignal";
    for (size_t i = 0; i < 5 * anzahlHsigZeilen + anzahlVsigSpalten + 1 +
            6 * anzahlHsigZeilen * anzahlVsigSpalten + 7; i++) {
        liesZeile(datei);
    };

    liesMehrzeiligenString("Kombinationssignal: Zugeordnete Vorsignale", datei);
    liesZeile("Kombinationssignal: Reservierter Eintrag", datei);

    return signal;
}

void StrLeser::liesHauptsignal(istream& datei, shared_ptr<Streckenelement>& element) {
    string tmp = liesZeile("Hauptsignal", datei);
    if (tmp == "#") {
        return;
    }

    konvertiereInGleitkommazahl("Hauptsignal-Standort X", tmp);
    liesGleitkommazahl("Hauptsignal-Standort Y", datei);
    liesGleitkommazahl("Hauptsignal-Standort Z", datei);
    liesGleitkommazahl("Hauptsignal-Standort Rotation X", datei);
    liesGleitkommazahl("Hauptsignal-Standort Rotation Y", datei);
    liesGleitkommazahl("Hauptsignal-Standort Rotation Z", datei);
    liesMehrzeiligenString("Hauptsignal-Landschaftsdateien", datei);

    liesZeile("Hauptsignal: Reservierter Eintrag 1", datei);
    liesZeile("Hauptsignal-Blockname", datei);
    liesZeile("Hauptsignal-Gleis", datei);

    tmp = liesZeile("Hauptsignal-Fahrstraßen: Blockname oder Endmarke", datei);
    while (tmp != "#") {
        // tmp == Blockname
        liesZeile("Hauptsignal-Fahrstraße: Gleis", datei);
        liesZeile("Hauptsignal-Fahrstraße: Signalbild-Nr", datei);
        liesZeile("Hauptsignal-Fahrstraße: vMax", datei);
        liesZeile("Hauptsignal-Fahrstraße: Reservierter Eintrag", datei);
        tmp = liesZeile("Hauptsignal-Fahrstraßen", datei);
    }

    liesMehrzeiligenString("Hauptsignal: Zugeordnete Vorsignale", datei);
    liesZeile("Hauptsignal: Reservierter Eintrag 2", datei);
}

void StrLeser::liesVorsignal(istream& datei, shared_ptr<Streckenelement>& element) {
    string tmp = liesZeile("Vorsignal", datei);
    if (tmp == "#") {
        return;
    }

    konvertiereInGleitkommazahl("Vorsignal-Standort X", tmp);
    liesGleitkommazahl("Vorsignal-Standort Y", datei);
    liesGleitkommazahl("Vorsignal-Standort Z", datei);
    liesGleitkommazahl("Vorsignal-Standort Rotation X", datei);
    liesGleitkommazahl("Vorsignal-Standort Rotation Y", datei);
    liesGleitkommazahl("Vorsignal-Standort Rotation Z", datei);
    liesZeile("Vorsignal-Landschaftsdatei", datei);

    tmp = liesZeile("Vorsignal-Signalbilder: Landschaft oder Endmarke", datei);
    while (tmp != "#") {
        // tmp == Landschaft
        liesGanzzahl("Vorsignal-Signalbild: Ereignis", datei);
        liesGanzzahl("Vorsignal-Signalbild: vMax", datei);
        tmp = liesZeile("Vorsignal-Signalbilder", datei);
    }

    liesZeile("Vorsignal dunkel bei Halt am Hsig", datei);
}

unique_ptr<Ereignis> StrLeser::neuesEreignis(ereignis_nr_t ereignisNr) {
    unique_ptr<Ereignis> ereignis(new Ereignis());
    if (ereignisNr >= 1 && ereignisNr <= 499) {
        ereignis->ereignisTyp = EreignisTyp::BedingteEntgleisung;
        ereignis->wert = float(ereignisNr) / 3.6;
    } else if (ereignisNr >= 1000 && ereignisNr <= 1500) {
        ereignis->ereignisTyp = EreignisTyp::Indusi1000Hz;
        ereignis->wert = float(ereignisNr - 1000) / 3.6;
    } else if (ereignisNr >= 2000 && ereignisNr <= 2500) {
        ereignis->ereignisTyp = EreignisTyp::Indusi2000Hz;
        ereignis->wert = float(ereignisNr - 2000) / 3.6;
    } else if (ereignisNr == 4000 || (ereignisNr >= 4004 && ereignisNr <= 4500)) {
        ereignis->ereignisTyp = EreignisTyp::GntGeschwindigkeitsErhoehung;
        ereignis->wert = float(ereignisNr - 4000) / 3.6;
    } else {
        switch (ereignisNr) {
            case 3001: ereignis->ereignisTyp = EreignisTyp::FahrstrAnfordern; break;
            case 3002: ereignis->ereignisTyp = EreignisTyp::FahrstrAufloesen; break;
            case 3003: ereignis->ereignisTyp = EreignisTyp::ZugEntfernen; break;
            case 3004: ereignis->ereignisTyp = EreignisTyp::Zwangshalt; break;
            case 3005: ereignis->ereignisTyp = EreignisTyp::LangsamfahrtEnde; break;
            case 3006: ereignis->ereignisTyp = EreignisTyp::Betriebsstelle; break;
            case 3007: ereignis->ereignisTyp = EreignisTyp::HaltepunktErwarten; break;
            case 3008: ereignis->ereignisTyp = EreignisTyp::Bahnsteigmitte; break;
            case 3009: ereignis->ereignisTyp = EreignisTyp::Bahnsteigende; break;
            case 3010: ereignis->ereignisTyp = EreignisTyp::LangsamfahrtAnfang; break;
            case 3011: ereignis->ereignisTyp = EreignisTyp::Pfeifen; break;
            case 3012: ereignis->ereignisTyp = EreignisTyp::LzbAnfang; break;
            case 3013: ereignis->ereignisTyp = EreignisTyp::LzbEnde; break;
            case 3021: ereignis->ereignisTyp = EreignisTyp::VorherKeineFahrstrEntfernung; break;
            case 3022: ereignis->ereignisTyp = EreignisTyp::Abfahrsignal; break;
            case 3023: ereignis->ereignisTyp = EreignisTyp::WeiterfahrtNachHalt; break;
            case 3024: ereignis->ereignisTyp = EreignisTyp::SignumWarnung; break;
            case 3025: ereignis->ereignisTyp = EreignisTyp::SignumHalt; break;
            case 3026:
                ereignis->ereignisTyp = EreignisTyp::VorherKeineFahrstrEntfernung;
                ereignis->wert = 1000;
                break;
            case 3027:
                ereignis->ereignisTyp = EreignisTyp::VorherKeineFahrstrEntfernung;
                ereignis->wert = 2000;
                break;
            case 3028:
                ereignis->ereignisTyp = EreignisTyp::VorherKeineFahrstrEntfernung;
                ereignis->wert = 3000;
                break;
            case 3029: ereignis->ereignisTyp = EreignisTyp::VorherKeineVsigVerknuepfung; break;
            case 3030: ereignis->ereignisTyp = EreignisTyp::OhneFunktion; break;
            case 3031: ereignis->ereignisTyp = EreignisTyp::BefehlAEinblenden; break;
            case 3032: ereignis->ereignisTyp = EreignisTyp::BefehlAEinblendenStillstand; break;
            case 3033: ereignis->ereignisTyp = EreignisTyp::BefehlBEinblenden; break;
            case 3034: ereignis->ereignisTyp = EreignisTyp::BefehlBEinblendenStillstand; break;
            case 3035: ereignis->ereignisTyp = EreignisTyp::LangsamfahrtEndeZuganfang; break;
            case 3036: ereignis->ereignisTyp = EreignisTyp::Wendepunkt; break;
            case 3037: ereignis->ereignisTyp = EreignisTyp::WendepunktAufAnderenBlocknamen; break;
            case 3038: ereignis->ereignisTyp = EreignisTyp::SignalIstZugbedient; break;
            case 3039: ereignis->ereignisTyp = EreignisTyp::ZugbedientesSignalSchalten; break;
            case 3040: ereignis->ereignisTyp = EreignisTyp::Streckensound; break;
            case 3041: ereignis->ereignisTyp = EreignisTyp::Abrupthalt; break;
            case 3042: ereignis->ereignisTyp = EreignisTyp::RegisterNichtBelegen; break;
            case 4001: ereignis->ereignisTyp = EreignisTyp::GntAnfang; break;
            case 4002: ereignis->ereignisTyp = EreignisTyp::GntEnde; break;
            case 4003: ereignis->ereignisTyp = EreignisTyp::GntIndusiUnterdrueckung; break;
            default:
                ereignis->ereignisTyp = (EreignisTyp)ereignisNr;
        }
    }

    return ereignis;
}
