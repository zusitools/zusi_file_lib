#ifndef SRC_MODEL_EREIGNIS_HPP
#define SRC_MODEL_EREIGNIS_HPP

#include <common/types.hpp>

using namespace std;

enum EreignisTyp {
    BedingteEntgleisung = 1, // + Geschwindigkeitswert
    Indusi500Hz = 500,
    Indusi1000Hz = 1000, // + Geschwindigkeitswert
    Indusi2000Hz = 2000, // + Geschwindigkeitswert
    FahrstrAnfordern = 3001,
    FahrstrAufloesen = 3002,
    ZugEntfernen = 3003,
    Zwangshalt = 3004,
    Abrupthalt = 3041,
    LangsamfahrtEnde = 3005, 
    LangsamfahrtEndeZuganfang = 3035,
    Betriebsstelle = 3006,
    HaltepunktErwarten = 3007,
    Bahnsteigmitte = 3008,
    Bahnsteigende = 3009,
    LangsamfahrtAnfang = 3010,
    Pfeifen = 3011,
    LzbAnfang = 3012,
    LzbEnde = 3013,
    SignumWarnung = 3024,
    SignumHalt = 3025,
    GntAnfang = 4001,
    GntEnde = 4002,
    GntGeschwindigkeitsErhoehung = 4000, // + Geschwindigkeitserhoehung
    GntIndusiUnterdrueckung = 4003,
    VorherKeineFahrstrEntfernung = 3021, // + Entfernung
    VorherKeineFahrstrVerzoegerung = 13, // + Verzoegerung
    Abfahrsignal = 3022, // Zp9
    WeiterfahrtNachHalt = 3023,
    VorherKeineVsigVerknuepfung = 3029,
    Streckensound = 3040,
    OhneFunktion = 3030,
    BefehlAEinblenden = 3031,
    BefehlAEinblendenStillstand = 3032,
    BefehlBEinblenden = 3033,
    BefehlBEinblendenStillstand = 3034,
    Wendepunkt = 3036,
    WendepunktAufAnderenBlocknamen = 3037,
    SignalIstZugbedient = 3038,
    ZugbedientesSignalSchalten = 3039,
    RegisterNichtBelegen = 3042,
};

// Ein Ereignis, das einer Richtung eines Streckenelements zugeordnet ist.
struct Ereignis {

    // Der Typ des Ereignisses.
    EreignisTyp ereignisTyp;

    // Der Parameter "Wert" des Ereignisses.
    ereignis_wert_t wert;

    // Der Parameter "Beschreibung" des Ereignisses.
    string beschreibung;
};


#endif
