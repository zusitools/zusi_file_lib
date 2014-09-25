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
    LangsamfahrtEnde = 3005, // = Ende Weichenbereich?
    LangsamfahrtEndeZuganfang = 3035,
    Betriebsstelle = 3006,
    HaltepunktErwarten = 3007,
    BahnsteiganfangLinks = 1000010,
    BahnsteiganfangRechts = 1000007,
    BahnsteigmitteLinks = 1000011,
    BahnsteigmitteRechts = 1000008,
    Bahnsteigmitte = 3008,
    BahnsteigendeLinks = 1000012,
    BahnsteigendeRechts = 1000009,
    Bahnsteigende = 3009,
    LangsamfahrtAnfang = 3010,
    Pfeifen = 3011,
    LzbAnfang = 3012,
    LzbEnde = 3013,
    LzbCeGeschwindigkeit = 3014,
    TrainguardAutomodus = 3050,
    TrainguardCtcModus = 3051,
    TrainguardItcModus = 3052,
    TrainguardBalise = 3053,
    Textmeldung = 3054,
    SignumWarnung = 3024,
    SignumHalt = 3025,
    GntAnfang = 4001,
    GntEnde = 4002,
    GntGeschwindigkeitsErhoehung = 4000, // + Geschwindigkeitserhoehung
    GntIndusiUnterdrueckung = 4003,
    VorherKeineFahrstrEntfernung = 3021, // + Entfernung
    VorherKeineFahrstrVerzoegerung = 13, // + Verzoegerung
    Abfahrsignal = 3022, // Zp9
    TuerschliessauftragSignal = 24,
    FahrtAufSichtBisNaechstesHsig = 17,
    WeiterfahrtNachHalt = 3023,
    VorherKeineVsigVerknuepfung = 3029,
    KeineLzbFahrstrEinrichten = 45,
    KeineZugFahrstrEinrichten = 21,
    KeineRangierFahrstrEinrichten = 22,
    BueOeffnen = 1000027,
    BueSchliessen = 27,
    Streckennummer = 39,
    HilfsHsig = 23,
    RegelgleisKennzeichnen = 39,
    GegengleisKennzeichnen = 28,
    EingleisigKennzeichnen = 40,
    Richtungsvoranzeiger = 38,
    RichtungsanzeigerZiel = 29,
    Streckensound = 3040,
    OhneFunktion = 3030,
    BefehlEinblenden = 32,
    BefehlAEinblenden = 3031,
    BefehlAEinblendenStillstand = 3032,
    BefehlBEinblenden = 3033,
    BefehlBEinblendenStillstand = 3034,
    Wendepunkt = 3036,
    WendepunktAufAnderenBlocknamen = 3037,
    Programmfunktion = 33,
    HsAusAnkuendigung = 46,
    HsAus = 41,
    HsEin = 42,
    StromabnSenken = 43,
    StromabnHeben = 44,
    HllManipulation = 47,
    RegisterInFahrstrVerknuepfen = 35,
    WeicheInFahrstrVerknuepfen = 36,
    SignalInFahrstrVerknuepfen = 37,
    VsigInFahrstrVerknuepfen = 50,
    SignalUmstellen = 48,
    WeicheUmstellen = 49,
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
