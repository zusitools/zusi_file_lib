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
    vector<shared_ptr<Ereignis>> ereignisse;

    // Die Nummer des zugeordneten Fahrstraßenregisters.
    shared_ptr<FahrstrRegister> fahrstrRegister;

    // Die zulässige Höchstgeschwindigkeit [m/s].
    geschwindigkeit_t vMax;

    // Die Kilometrierung am Elementanfang(?) [km]
    kilometrierung_t km;

    // Ist die Kilometrierung in Richtung des Elementendes aufsteigend?
    bool kmAufsteigend;

    // Fahrstraßensignal (nur Zusi 2)
    shared_ptr<FahrstrassenSignal> fahrstrSignal;

    // Kombinationssignal
    shared_ptr<KombiSignal> signal;

    // TODO: Koppelweiche
};

struct Streckenelement;

// Ein Verweis auf eine Richtung eines Streckenelements.
struct StreckenelementUndRichtung {

    // Das Streckenelement, auf das sich die Referenz bezieht.
    const Streckenelement& streckenelement;

    // Die Richtung des Streckenelements, auf das sich die Referenz bezieht.
    const streckenelement_richtung_t richtung;

    StreckenelementUndRichtung nachfolger(const nachfolger_index_t index = 0) const;

    bool hatNachfolger() const;

    StreckenelementUndRichtung vorgaenger(const nachfolger_index_t index = 0) const;

    bool hatVorgaenger() const;

    StreckenelementUndRichtung gegenrichtung() const;

    StreckenelementUndRichtung(const Streckenelement& streckenelement, const streckenelement_richtung_t richtung) :
            streckenelement(streckenelement), richtung(richtung) {
    }

    StreckenelementUndRichtung() = delete;
    StreckenelementUndRichtung(const StreckenelementUndRichtung&) = default;
    StreckenelementUndRichtung& operator=(const StreckenelementUndRichtung&) = default;
};

// Ein Streckenelement.
struct Streckenelement {

    // Konstanten für die Indizierung von richtungsspezifischen Daten, die in Arrays abgelegt sind.
    static const streckenelement_richtung_t RICHTUNG_GEGEN = 0;
    static const streckenelement_richtung_t RICHTUNG_NORM = 1;

    // Die Nummer des Streckenelements im Modul, in dem es definiert ist.
    streckenelement_nr_t nr;

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
    // Die Nachfolgerrichtung kann mittels "anschluss" berechnet werden.
    vector<weak_ptr<Streckenelement>> nachfolgerElemente[2];

    // Anschluss-Informationen für Norm- und Gegenrichtung.
    // Eine 1 in Bit Nr. i bedeutet, dass Nachfolger i in Gegenrichtung liegt.
    uint8_t anschluss[2];

    // Die Funktionsflags.
    set<StreckenelementFlag> flags;

    // ---

    inline StreckenelementUndRichtung richtung(const streckenelement_richtung_t richtung) {
        return StreckenelementUndRichtung(*this, richtung);
    }

    inline StreckenelementUndRichtung normrichtung() {
        return this->richtung(Streckenelement::RICHTUNG_NORM);
    }

    inline StreckenelementUndRichtung gegenrichtung() {
        return this->richtung(Streckenelement::RICHTUNG_GEGEN);
    }

    void setzeNachfolger(const nachfolger_index_t index, const streckenelement_richtung_t richtung,
            const shared_ptr<Streckenelement> nachfolger);
    void setzeNachfolger(const nachfolger_index_t index, const streckenelement_richtung_t richtung,
            const shared_ptr<Streckenelement> nachfolger, const streckenelement_richtung_t nachfolgerRichtung);
    void setzeVorgaenger(const nachfolger_index_t index, const streckenelement_richtung_t richtung,
            const shared_ptr<Streckenelement> vorgaenger);
    void setzeVorgaenger(const nachfolger_index_t index, const streckenelement_richtung_t richtung,
            const shared_ptr<Streckenelement> vorgaenger, const streckenelement_richtung_t vorgaengerRichtung);

    inline bool hatFktFlag(StreckenelementFlag flag) { return flags.find(flag) != flags.end(); }

    inline static streckenelement_richtung_t richtungUmkehren(
            const streckenelement_richtung_t richtung) {
        return richtung == Streckenelement::RICHTUNG_NORM ?
            Streckenelement::RICHTUNG_GEGEN : Streckenelement::RICHTUNG_NORM;
    }
};

#endif
