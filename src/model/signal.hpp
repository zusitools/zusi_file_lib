#ifndef SRC_MODEL_SIGNAL_HPP
#define SRC_MODEL_SIGNAL_HPP

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

};

#endif
