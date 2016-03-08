#ifndef SRC_MODEL_SIGNAL_HPP
#define SRC_MODEL_SIGNAL_HPP

// Nur Zusi 3
enum SignalTyp {
    Unbestimmt = 0,
    Tafel = 1,
    Weiche = 2,
    Gleissperre = 3,
    Bahnuebergang = 4,
    Rangiersignal = 5,
    Vorsignal = 6,
    Einfahrsignal = 7,
    Zwischensignal = 8,
    Ausfahrsignal = 9,
    Blocksignal = 10,
    Deckungssignal = 11,
    LZB_Block = 12,
    Hilfshauptsignal = 13,
    Sonstiges = 14,
};

// Nur Zusi 2
struct FahrstrassenSignal {

};

struct Signal {

    // Name der Betriebsstelle, zu der das Signal gehört.
    string betriebsstelle;

    // Name des Stellwerks, von dem aus das Signal gesteuert wird.
    string stellwerk;

    // Signalbezeichnung oder Gleis.
    string signalbezeichnung;

    // Typ des Signals (nur Zusi 3).
    SignalTyp signaltyp;

};

#endif
