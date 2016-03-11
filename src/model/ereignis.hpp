#ifndef SRC_MODEL_EREIGNIS_HPP
#define SRC_MODEL_EREIGNIS_HPP

#include <common/types.hpp>

using namespace std;

enum EreignisTyp {
    // Zusi-3-spezifische oder gemeinsam genutzte Ereignisse.
    // Der numerische Wert der Ereignistypen entspricht ihrem Wert in Zusi 3.
    Signalgeschwindigkeit = 1,
    EndeWeichenbereich = 1000002,
    Signalhaltfall = 3,
    FahrstrAufloesen = 4,
    FahrstrAnfordern = 5,
    Buchfahrplaneintrag = 6,
    BahnsteiganfangRechts = 1000007,
    BahnsteigmitteRechts = 1000008,
    BahnsteigendeRechts = 1000009,
    BahnsteiganfangLinks = 1000010,
    BahnsteigmitteLinks = 1000011,
    BahnsteigendeLinks = 1000012,
    VorherKeineFahrstrVerzoegerung = 13,
    Zwangshalt = 14,
    Abrupthalt = 15,
    ZugEntfernen = 16,
    FahrtAufSichtBisNaechstesHsig = 17,
    WeiterfahrtNachHalt = 18,
    Beschreibung = 19,
    VorherKeineVsigVerknuepfung = 20,
    KeineLzbFahrstrEinrichten = 45,
    KeineZugFahrstrEinrichten = 21,
    KeineRangierFahrstrEinrichten = 22,
    HilfsHsig = 23,
    TuerschliessauftragSignal = 24,
    Abfahrsignal = 25,
    BedingteEntgleisung = 52, // + Geschwindigkeitswert
    BueOeffnen = 1000027,
    BueSchliessen = 27,
    GegengleisKennzeichnen = 28,
    RegelgleisKennzeichnen = 39,
    EingleisigKennzeichnen = 40,
    Richtungsvoranzeiger = 38,
    RichtungsanzeigerZiel = 29,
    Pfeifen = 30,
    Streckensound = 31,
    BefehlEinblenden = 32,
    HsAusAnkuendigung = 46,
    HllManipulation = 47,
    HsAus = 41,
    HsEin = 42,
    StromabnSenken = 43,
    StromabnHeben = 44,
    RegisterInFahrstrVerknuepfen = 35,
    WeicheInFahrstrVerknuepfen = 36,
    SignalInFahrstrVerknuepfen = 37,
    VsigInFahrstrVerknuepfen = 50,
    SignalUmstellen = 48,
    WeicheUmstellen = 49,
    Indusi500Hz = 500,
    Indusi1000Hz = 1000, // + Geschwindigkeitswert
    Indusi2000Hz = 2000, // + Geschwindigkeitswert
    LzbAnfang = 1003001,
    LzbEnde = 3002,
    LzbCeGeschwindigkeit = 303,
    GntGeschwindigkeitsErhoehung = 4000, // + Geschwindigkeitserhoehung
    GntAnfang = 4001,
    GntEnde = 4002,
    GntIndusiUnterdrueckung = 4003,

    // Zusi-2-spezifische Ereignisse
    LangsamfahrtEnde = 3005, // = Ende Weichenbereich?
    LangsamfahrtEndeZuganfang = 3035,
    Betriebsstelle = 3006,
    HaltepunktErwarten = 3007,
    Bahnsteigmitte = 3008,
    Bahnsteigende = 3009,
    LangsamfahrtAnfang = 3010,
    TrainguardAutomodus = 3050,
    TrainguardCtcModus = 3051,
    TrainguardItcModus = 3052,
    TrainguardBalise = 3053,
    Textmeldung = 3054,
    SignumWarnung = 3024,
    SignumHalt = 3025,
    VorherKeineFahrstrEntfernung = 3021, // + Entfernung
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
