#include <fstream>
#include <string>
#include <clocale>

#include <model/autorinfo.hpp>
#include <model/dateiinfo.hpp>

#include "str_leser.hpp"

using namespace std;

std::string mkstring(std::pair<std::vector<char>::const_iterator, std::vector<char>::const_iterator> p) {
    return std::string(p.first, p.second);
}

unique_ptr<Strecke> StrLeser::parse() {
    try {
        this->zeilenNr = 0;

        // Dateiformat-Version.
        string formatVersion = mkstring(liesZeile("Dateiformat-Version"));

        string formatMinVersion;
        if (formatVersion == "1.1" || formatVersion == "2.0" || formatVersion == "2.1") {
            formatMinVersion = "1.1";
        } else {
            formatMinVersion = formatVersion;
        }
        auto result = std::make_unique<Strecke>(formatVersion, formatMinVersion);

        // Autor-Information.
        unique_ptr<AutorInfo> autorInfo(new AutorInfo());
        autorInfo->name = mkstring(liesZeile("Autor-Name"));
        result->autorInfo.push_back(std::move(autorInfo));

        result->breitengrad = Z2Leser::liesGanzzahl("Breitengrad");
        result->rekursionstiefe = Z2Leser::liesGanzzahl("Rekursionstiefe");
        result->gebietsschema = mkstring(liesZeile("Gebietsschema"));

        result->beschreibung = Z2Leser::liesMehrzeiligenString("Dateibeschreibung");

        if (result->formatMinVersion != "1.1") {
            Z2Leser::liesMehrzeiligenString("UTM-Info");
        }

        // Geschwindigkeits-Multiplikator 3,6 (ohne Funktion); ab Zusi 2.4.5.0: Signalhaltabstand.
        if (*this->pos == '3' && *(this->pos + 1) == ',' && *(this->pos + 2) == '6') {
            this->pos += 3;
            if (!this->liesZeilenende()) {
                throw invalid_argument("Zusaetzliche Zeichen am Ende der Zeile");
            }
        } else {
            result->signalHaltabstand = Z2Leser::liesGanzzahl("Signal-Haltabstand");
        }

        Z2Leser::liesZeile("Pfad Landschaftsdatei");

        // Referenzpunkte: Einsatzelemente
        while (!this->liesRauteZeilenende()) {
            unique_ptr<Referenzpunkt> referenzpunkt(new Referenzpunkt());
            referenzpunkt->elementRichtung.richtung = Streckenelement::RICHTUNG_NORM;
            if (result->formatMinVersion != "1.1") {
                referenzpunkt->referenzNr = liesGanzzahl("Aufgleisreferenz-Nr.");
                referenzpunkt->streckenelementNr = liesGanzzahl("Aufgleiselement-Nr.");
                referenzpunkt->beschreibung = mkstring(liesZeile("Aufgleispunkt-Beschreibung"));

                if (result->referenzpunkte.size() <= referenzpunkt->referenzNr) {
                    result->referenzpunkte.resize(referenzpunkt->referenzNr + 1);
                }
                result->referenzpunkte.at(referenzpunkt->referenzNr) = std::move(referenzpunkt);
            } else {
                referenzpunkt->referenzNr = result->referenzpunkte.size();
                referenzpunkt->streckenelementNr = liesGanzzahl("Aufgleiselement-Nr.");
                result->referenzpunkte.push_back(std::move(referenzpunkt));
            }
        }

        // Streckenfeste Standorte (Blickpunkte).
        while (!this->liesRauteZeilenende()) {
            unique_ptr<Blickpunkt> blickpunkt(new Blickpunkt());
            blickpunkt->position.loc.x = Z2Leser::liesGleitkommazahl("Blickpunkt-Position X");
            blickpunkt->position.loc.y = Z2Leser::liesGleitkommazahl("Blickpunkt-Position Y");
            blickpunkt->position.loc.z = Z2Leser::liesGleitkommazahl("Blickpunkt-Position Z");

            // Zwischenspeichern der Koordinaten in lokale Variablen -- wenn man die Aufrufe an
            // liesGleitkommazahl in die Konstruktorparameter packt, ist die Aufrufreihenfolge
            // undefiniert.
            koordinate_t x, y, z;
            x = Z2Leser::liesGleitkommazahl("Blickpunkt-Rotation X");
            y = Z2Leser::liesGleitkommazahl("Blickpunkt-Rotation Y");
            z = Z2Leser::liesGleitkommazahl("Blickpunkt-Rotation Z");
            blickpunkt->position.setRichtungAlsRot(Punkt3D { x, y, z });
            blickpunkt->name = mkstring(liesZeile("Blickpunkt-Name"));

            result->blickpunkte.push_back(std::move(blickpunkt));
        }

        // Streckenelemente
        this->aktElement = "Streckenelemente";
        this->liesStreckenelemente(*result);

        // Löse Element-Referenzen auf, soweit möglich.
        for (auto& referenzpunkt : result->referenzpunkte) {
            if (!referenzpunkt) {
                continue;
            }

            if (referenzpunkt->streckenelementNr >= result->streckenelemente.size()) {
                continue;
            }

            referenzpunkt->elementRichtung.streckenelement = result->streckenelemente.at(referenzpunkt->streckenelementNr).get();
        }
        return result;
    } catch (const std::exception& ex) {
        throw invalid_argument("Fehler beim Einlesen von Zeile " + to_string(this->zeilenNr) + " ("
            + this->aktElement + "): " + ex.what());
    }
}

void StrLeser::liesStreckenelemente(Strecke& strecke) {
    // Die Nachfolger-Nummern jedes Streckenelements.
    vector<vector<streckenelement_nr_t>> nachfolgerNrs;

    while (this->pos != this->buffer.end()) {
        unique_ptr<Streckenelement> element(new Streckenelement());
        StreckenelementRichtungsInfo& richtung =
            element->richtungsInfo[Streckenelement::RICHTUNG_NORM];

        element->nr = liesGanzzahl("Streckenelement-Nummer");

        liesZeile("Kilometrierung");
        liesZeile("Zählrichtung Kilometrierung");
        liesZeile("Landschaftsbezeichnung"); // # wiederholt von Vorgänger

        ereignis_nr_t ereignisNr = liesGanzzahl("Ereignis-Nummer");
        if (ereignisNr != 0) {
            richtung.ereignisse.emplace_back();
            neuesEreignis(ereignisNr, richtung.ereignisse.back());
        }

        // Element-Koordinaten
        element->p1.x = liesGleitkommazahl("Element-Koordinate X1");
        element->p1.y = liesGleitkommazahl("Element-Koordinate Y1");
        element->p1.z = liesGleitkommazahl("Element-Koordinate Z1");
        element->p2.x = liesGleitkommazahl("Element-Koordinate X2");
        element->p2.y = liesGleitkommazahl("Element-Koordinate Y2");
        element->p2.z = liesGleitkommazahl("Element-Koordinate Z2");

        liesZeile("Überhöhung");

        // Nachfolger 1-3
        for (size_t i = 0; i < 3; i++) {
            streckenelement_nr_t nachfolgerNr = liesGanzzahl("Nachfolgerelement");
            if (nachfolgerNr != 0) {
                if (nachfolgerNrs.size() <= element->nr + 1) {
                    nachfolgerNrs.resize(element->nr + 1);
                }
                nachfolgerNrs.at(element->nr).resize(i + 1);
                nachfolgerNrs.at(element->nr).at(i) = nachfolgerNr;
            }
        }

        richtung.vmax = liesGleitkommazahl("Element-vMax") / 3.6;
        if (strecke.formatMinVersion != "1.1") {
            liesZeile("Reservierter Streckenelement-Eintrag");
        }
        liesZeile("Name Bahnsteig/Betriebsstelle");

        auto flags = liesZeile("Funktionsflags");
        for (auto it = flags.first; it != flags.second; ++it) {
            switch (*it) {
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

        liesZeile("Oberleitungsspannung");

        this->aktElement = "Fahrstraßensignal";
        liesFahrstrSignal(*element, strecke);

        if (strecke.formatMinVersion != "1.1") {
            if (!this->liesRauteZeilenende()) {
                richtung.signal = liesKombiSignal();
            }
        } else {
            if (!this->liesRauteZeilenende()) {
                this->aktElement = "Vorsignal";
                liesVorsignal(*element);
            }
            if (!this->liesRauteZeilenende()) {
                this->aktElement = "Hauptsignal";
                liesHauptsignal(*element);
            }
        }

        fahrstr_register_nr_t registerNr = liesGanzzahl("Fahrstraßenregister-Nummer");
        if (registerNr != 0) {
            if (strecke.fahrstrRegister.find(registerNr) == strecke.fahrstrRegister.end()) {
                unique_ptr<FahrstrRegister> fahrstrRegister(new FahrstrRegister());
                fahrstrRegister->registerNr = registerNr;
                fahrstrRegister->manuell = registerNr <= 1000;
                strecke.fahrstrRegister[registerNr] = std::move(fahrstrRegister);
            }
            richtung.fahrstrRegister = strecke.fahrstrRegister[registerNr].get();
        }

        if (strecke.streckenelemente.size() <= element->nr + 1) {
            strecke.streckenelemente.resize(element->nr + 1);
        }
        strecke.streckenelemente.at(element->nr) = std::move(element);
    }

    // Verknüpfe Streckenelemente mit ihren Nachfolgern und Vorgängern.
    for (size_t i = 0; i < strecke.streckenelemente.size() && i < nachfolgerNrs.size(); i++) {
        Streckenelement* element = strecke.streckenelemente.at(i).get();

        if (element == nullptr) {
            continue;
        }

        for (size_t j = 0; j < nachfolgerNrs.at(i).size(); j++) {
            streckenelement_nr_t elemNr = nachfolgerNrs.at(i).at(j);
            if (elemNr == 0 || elemNr >= strecke.streckenelemente.size()) {
                continue;
            }

            Streckenelement* nachfolger = strecke.streckenelemente.at(elemNr).get();
            if (nachfolger == nullptr) {
                continue;
            }

            element->setzeNachfolger(j, Streckenelement::RICHTUNG_NORM, *nachfolger, Streckenelement::RICHTUNG_NORM);
            nachfolger->setzeVorgaenger(
                    nachfolger->nachfolgerElemente[Streckenelement::RICHTUNG_GEGEN].size(),
                    Streckenelement::RICHTUNG_NORM,
                    *element,
                    Streckenelement::RICHTUNG_NORM);
        }
    }
}

void StrLeser::liesFahrstrSignal(Streckenelement&, Strecke &strecke) {
    if (this->liesRauteZeilenende()) {
        return;
    }

    liesGleitkommazahl("Fahrstraßensignal-Standort X");
    liesGleitkommazahl("Fahrstraßensignal-Standort Y");
    liesGleitkommazahl("Fahrstraßensignal-Standort Z");
    liesGleitkommazahl("Fahrstraßensignal-Standort Rotation X");
    liesGleitkommazahl("Fahrstraßensignal-Standort Rotation Y");
    liesGleitkommazahl("Fahrstraßensignal-Standort Rotation Z");

    if (strecke.formatMinVersion != "1.1") {
        liesZeile("Fahrstraßensignal: ohne Funktion 1");
        liesZeile("Fahrstraßensignal: ohne Funktion 2");
        liesZeile("Fahrstraßensignal: ohne Funktion 3");
        liesZeile("Fahrstraßensignal: ohne Funktion 4");
        liesZeile("Fahrstraßensignal: ohne Funktion 5");
        liesZeile("Fahrstraßensignal: ohne Funktion 6");
    }

    liesZeile("Fahrstraßensignal: Landschaftsdatei");

    if (strecke.formatMinVersion != "1.1") {
        liesZeile("Fahrstraßensignal: ohne Funktion 7");
    }

    liesMehrzeiligenString("Fahrstraßensignal: Signalbilder");

    liesZeile("Fahrstraßensignal: Ereignis");
    liesZeile("Fahrstraßensignal: Angekündigte Geschwindigkeit");

    if (strecke.formatMinVersion != "1.1") {
        liesZeile("Fahrstraßensignal: Verweis auf Masterelement");
    }
}

unique_ptr<Signal> StrLeser::liesKombiSignal() {
    unique_ptr<Signal> signal(new Signal());

    liesGleitkommazahl("Kombinationssignal-Standort X1");
    liesGleitkommazahl("Kombinationssignal-Standort Y1");
    liesGleitkommazahl("Kombinationssignal-Standort Z1");
    liesGleitkommazahl("Kombinationssignal-Standort Rotation X1");
    liesGleitkommazahl("Kombinationssignal-Standort Rotation Y1");
    liesGleitkommazahl("Kombinationssignal-Standort Rotation Z1");
    liesGleitkommazahl("Kombinationssignal-Standort X2");
    liesGleitkommazahl("Kombinationssignal-Standort Y2");
    liesGleitkommazahl("Kombinationssignal-Standort Z2");
    liesGleitkommazahl("Kombinationssignal-Standort Rotation X2");
    liesGleitkommazahl("Kombinationssignal-Standort Rotation Y2");
    liesGleitkommazahl("Kombinationssignal-Standort Rotation Z2");

    liesMehrzeiligenString("Kombinationssignal-Landschaftsdateien");
    signal->betriebsstelle = mkstring(liesZeile("Kombinationssignal-Blockname"));
    signal->signalbezeichnung = mkstring(liesZeile("Kombinationssignal-Gleis"));

    size_t anzahlHsigZeilen = liesGanzzahl("Kombinationssignal Anzahl Hsig-Zeilen") + 1;
    size_t anzahlVsigSpalten = liesGanzzahl("Kombinationssignal Anzahl Vsig-Spalten") + 1;

    this->aktElement = "Kombinationssignal: Fahrziele, Vsig-Geschwindigkeiten, Matrix, Ersatzzignal";
    for (size_t i = 0; i < 5 * anzahlHsigZeilen + anzahlVsigSpalten + 1 +
            6 * anzahlHsigZeilen * anzahlVsigSpalten + 7; i++) {
        liesZeile();
    };

    liesMehrzeiligenString("Kombinationssignal: Zugeordnete Vorsignale");
    liesZeile("Kombinationssignal: Reservierter Eintrag");

    return signal;
}

void StrLeser::liesHauptsignal(Streckenelement&) {
    liesGleitkommazahl("Hauptsignal-Standort X");
    liesGleitkommazahl("Hauptsignal-Standort Y");
    liesGleitkommazahl("Hauptsignal-Standort Z");
    liesGleitkommazahl("Hauptsignal-Standort Rotation X");
    liesGleitkommazahl("Hauptsignal-Standort Rotation Y");
    liesGleitkommazahl("Hauptsignal-Standort Rotation Z");
    liesMehrzeiligenString("Hauptsignal-Landschaftsdateien");

    liesZeile("Hauptsignal: Reservierter Eintrag 1");
    liesZeile("Hauptsignal-Blockname");
    liesZeile("Hauptsignal-Gleis");

    while (!this->liesRauteZeilenende()) {
        liesZeile("Hauptsignal-Fahrstraße: Blockname");
        liesZeile("Hauptsignal-Fahrstraße: Gleis");
        liesZeile("Hauptsignal-Fahrstraße: Signalbild-Nr");
        liesZeile("Hauptsignal-Fahrstraße: vMax");
        liesZeile("Hauptsignal-Fahrstraße: Reservierter Eintrag");
    }

    liesMehrzeiligenString("Hauptsignal: Zugeordnete Vorsignale");
    liesZeile("Hauptsignal: Reservierter Eintrag 2");
}

void StrLeser::liesVorsignal(Streckenelement&) {
    liesGleitkommazahl("Vorsignal-Standort X");
    liesGleitkommazahl("Vorsignal-Standort Y");
    liesGleitkommazahl("Vorsignal-Standort Z");
    liesGleitkommazahl("Vorsignal-Standort Rotation X");
    liesGleitkommazahl("Vorsignal-Standort Rotation Y");
    liesGleitkommazahl("Vorsignal-Standort Rotation Z");
    liesZeile("Vorsignal-Landschaftsdatei");

    while (!this->liesRauteZeilenende()) {
        liesZeile("Vorsignal-Signalbild: Landschaft");
        liesGanzzahl("Vorsignal-Signalbild: Ereignis");
        liesGanzzahl("Vorsignal-Signalbild: vMax");
    }

    liesZeile("Vorsignal dunkel bei Halt am Hsig");
}

void StrLeser::neuesEreignis(ereignis_nr_t ereignisNr, Ereignis& ereignis) {
    if (ereignisNr >= 1 && ereignisNr <= 499) {
        ereignis.ereignisTyp = EreignisTyp::BedingteEntgleisung;
        ereignis.wert = float(ereignisNr) / 3.6;
    } else if (ereignisNr >= 1000 && ereignisNr <= 1500) {
        ereignis.ereignisTyp = EreignisTyp::Indusi1000Hz;
        ereignis.wert = float(ereignisNr - 1000) / 3.6;
    } else if (ereignisNr >= 2000 && ereignisNr <= 2500) {
        ereignis.ereignisTyp = EreignisTyp::Indusi2000Hz;
        ereignis.wert = float(ereignisNr - 2000) / 3.6;
    } else if (ereignisNr == 4000 || (ereignisNr >= 4004 && ereignisNr <= 4500)) {
        ereignis.ereignisTyp = EreignisTyp::GntGeschwindigkeitsErhoehung;
        ereignis.wert = float(ereignisNr - 4000) / 3.6;
    } else {
        switch (ereignisNr) {
            case 3001: ereignis.ereignisTyp = EreignisTyp::FahrstrAnfordern; break;
            case 3002: ereignis.ereignisTyp = EreignisTyp::FahrstrAufloesen; break;
            case 3003: ereignis.ereignisTyp = EreignisTyp::ZugEntfernen; break;
            case 3004: ereignis.ereignisTyp = EreignisTyp::Zwangshalt; break;
            case 3005: ereignis.ereignisTyp = EreignisTyp::LangsamfahrtEnde; break;
            case 3006: ereignis.ereignisTyp = EreignisTyp::Betriebsstelle; break;
            case 3007: ereignis.ereignisTyp = EreignisTyp::HaltepunktErwarten; break;
            case 3008: ereignis.ereignisTyp = EreignisTyp::Bahnsteigmitte; break;
            case 3009: ereignis.ereignisTyp = EreignisTyp::Bahnsteigende; break;
            case 3010: ereignis.ereignisTyp = EreignisTyp::LangsamfahrtAnfang; break;
            case 3011: ereignis.ereignisTyp = EreignisTyp::Pfeifen; break;
            case 3012: ereignis.ereignisTyp = EreignisTyp::LzbAnfang; break;
            case 3013: ereignis.ereignisTyp = EreignisTyp::LzbEnde; break;
            case 3021: ereignis.ereignisTyp = EreignisTyp::VorherKeineFahrstrEntfernung; break;
            case 3022: ereignis.ereignisTyp = EreignisTyp::Abfahrsignal; break;
            case 3023: ereignis.ereignisTyp = EreignisTyp::WeiterfahrtNachHalt; break;
            case 3024: ereignis.ereignisTyp = EreignisTyp::SignumWarnung; break;
            case 3025: ereignis.ereignisTyp = EreignisTyp::SignumHalt; break;
            case 3026:
                ereignis.ereignisTyp = EreignisTyp::VorherKeineFahrstrEntfernung;
                ereignis.wert = 1000;
                break;
            case 3027:
                ereignis.ereignisTyp = EreignisTyp::VorherKeineFahrstrEntfernung;
                ereignis.wert = 2000;
                break;
            case 3028:
                ereignis.ereignisTyp = EreignisTyp::VorherKeineFahrstrEntfernung;
                ereignis.wert = 3000;
                break;
            case 3029: ereignis.ereignisTyp = EreignisTyp::VorherKeineVsigVerknuepfung; break;
            case 3030: ereignis.ereignisTyp = EreignisTyp::OhneFunktion; break;
            case 3031: ereignis.ereignisTyp = EreignisTyp::BefehlAEinblenden; break;
            case 3032: ereignis.ereignisTyp = EreignisTyp::BefehlAEinblendenStillstand; break;
            case 3033: ereignis.ereignisTyp = EreignisTyp::BefehlBEinblenden; break;
            case 3034: ereignis.ereignisTyp = EreignisTyp::BefehlBEinblendenStillstand; break;
            case 3035: ereignis.ereignisTyp = EreignisTyp::LangsamfahrtEndeZuganfang; break;
            case 3036: ereignis.ereignisTyp = EreignisTyp::Wendepunkt; break;
            case 3037: ereignis.ereignisTyp = EreignisTyp::WendepunktAufAnderenBlocknamen; break;
            case 3038: ereignis.ereignisTyp = EreignisTyp::SignalIstZugbedient; break;
            case 3039: ereignis.ereignisTyp = EreignisTyp::ZugbedientesSignalSchalten; break;
            case 3040: ereignis.ereignisTyp = EreignisTyp::Streckensound; break;
            case 3041: ereignis.ereignisTyp = EreignisTyp::Abrupthalt; break;
            case 3042: ereignis.ereignisTyp = EreignisTyp::RegisterNichtBelegen; break;
            case 4001: ereignis.ereignisTyp = EreignisTyp::GntAnfang; break;
            case 4002: ereignis.ereignisTyp = EreignisTyp::GntEnde; break;
            case 4003: ereignis.ereignisTyp = EreignisTyp::GntIndusiUnterdrueckung; break;
            default:
                ereignis.ereignisTyp = (EreignisTyp)ereignisNr;
        }
    }
}
