#ifndef SRC_MODEL_STRECKENELEMENT_HPP
#define SRC_MODEL_STRECKENELEMENT_HPP

using namespace std;

#include <memory>
#include <set>
#include <string>
#include <vector>

#include <common/types.hpp>
#include <model/ereignis.hpp>
#include <model/punkt3d.hpp>
#include <model/fahrstr_register.hpp>
#include <model/signal.hpp>

// Funktions-Flags eines Streckenelements.
enum StreckenelementFlag {

    // Tunnel. Zusi 2 und Zusi 3.
    Tunnel = 0,

    // Weichenbausatz. Zusi 2 und Zusi 3. Entspricht bei Zusi 2 der Funktion
    // "Weiche"/"Weichensound".
    Weichenbausatz = 1,

    // Steinbrücke. Nur Zusi 2.
    Steinbruecke = 2,

    // Stahlbrücke. Nur Zusi 2.
    Stahlbruecke = 3,

    // Keine Gleisfunktion. Zusi 2 und Zusi 3.
    KeineGleisfunktion = 4,

    // Kein Schienenbau. Nur Zusi 2.
    KeinSchienenbau = 5,

    // Kein Weichenbau. Nur Zusi 2.
    KeinWeichenbau = 6,

    // Keine Schulter rechts. Nur Zusi 3.
    KeineSchulterRechts = 7,

    // Keine Schulter links. Nur Zusi 3.
    KeineSchulterLinks = 8,
};

// Info über eine Richtung eines Streckenelements.
struct StreckenelementRichtungsInfo {

    // Durch das Befahren ausgelöste Ereignisse.
    vector<unique_ptr<Ereignis>> ereignisse;

    // Die Nummer des zugeordneten Fahrstraßenregisters.
    shared_ptr<FahrstrRegister> fahrstrRegister;

    // Die zulässige Höchstgeschwindigkeit [m/s].
    geschwindigkeit_t vMax;

    // Die Kilometrierung am Elementanfang(?) [km]
    kilometrierung_t km;

    // Ist die Kilometrierung in Richtung des Elementendes aufsteigend?
    bool kmAufsteigend;

    // Fahrstraßensignal
    unique_ptr<FahrstrassenSignal> fahrstrSignal;

    // Kombinationssignal
    unique_ptr<KombiSignal> signal;

    // TODO: Koppelweiche
};

struct StreckenelementUndRichtung;

// Ein Streckenelement.
struct Streckenelement {

    // Konstanten für die Indizierung von richtungsspezifischen Daten, die in Arrays abgelegt sind.
    static const streckenelement_richtung_t RICHTUNG_NORM = 0;
    static const streckenelement_richtung_t RICHTUNG_GEGEN = 1;

    // Die Nummer des Streckenelements in der fertig zusammengesetzten Strecke.
    streckenelement_nr_t nr;

    // Die Nummer des Streckenelements im Modul, in dem es definiert ist.
    streckenelement_nr_t nrInModul;

    // Die Position von Element-Ende 1 (Normrichtung: 1-->2).
    Punkt3D p1;

    // Die Position von Element-Ende 2 (Normrichtung: 1-->2).
    Punkt3D p2;

    // Die Überhöhung des Elements [rad].
    ueberhoehung_t ueberhoehung;

    // Die Krümmung des Elements [1/km].
    kruemmung_t kruemmung;

    // Die Spannung der Oberleitung [kV]. 0 = nicht elektrifiziert.
    spannung_t volt;

    // Die Fahrdrahthöhe in Elementmitte [m].
    meter_t drahthoehe;

    // Der Name des Oberbaus.
    string oberbauName;

    // Der Name des Fahrleitungstyps.
    string fahrleitungTyp;

    // Die Zwangshelligkeit des Streckenelements [0..1].
    helligkeit_t zwangshelligkeit;

    // Die Trassierungsgeschwindigkeit des Streckenelements [m/s].
    geschwindigkeit_t vTrassierung;

    // Informationen zu Norm- und Gegenrichtung.
    StreckenelementRichtungsInfo richtungsInfo[2];

    // Geordnete Liste von Nachfolgern in jeder Richtung.
    vector<StreckenelementUndRichtung> nachfolgerElemente[2];

    // Die Funktionsflags.
    set<StreckenelementFlag> flags;

    // ---

    StreckenelementUndRichtung nachfolger(
            const nachfolger_index_t index = 0,
            const streckenelement_richtung_t richtung = Streckenelement::RICHTUNG_NORM);
    StreckenelementUndRichtung vorgaenger(
            const nachfolger_index_t index = 0,
            const streckenelement_richtung_t richtung = Streckenelement::RICHTUNG_NORM);

    void setzeNachfolger(const nachfolger_index_t index, const streckenelement_richtung_t richtung,
            const StreckenelementUndRichtung& nachfolger);
    void setzeVorgaenger(const nachfolger_index_t index, const streckenelement_richtung_t richtung,
            const StreckenelementUndRichtung& vorgaenger);

    inline static const streckenelement_richtung_t gegenrichtung(
            const streckenelement_richtung_t richtung) {
        return richtung == Streckenelement::RICHTUNG_NORM ?
            Streckenelement::RICHTUNG_GEGEN : Streckenelement::RICHTUNG_NORM;
    }
};

// Ein Verweis auf eine Richtung eines Streckenelements.
struct StreckenelementUndRichtung {

    // Das Streckenelement, auf das sich die Referenz bezieht.
    weak_ptr<Streckenelement> streckenelement;

    // Die Richtung des Streckenelements, auf das sich die Referenz bezieht.
    streckenelement_richtung_t richtung;

};

#endif
