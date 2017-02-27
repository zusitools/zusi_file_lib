#ifndef SRC_MODEL_STRECKE_HPP
#define SRC_MODEL_STRECKE_HPP

#include <memory>
#include <unordered_map>

#include <model/blickpunkt.hpp>
#include <model/fahrstr_register.hpp>
#include <model/referenzpunkt.hpp>
#include <model/streckenelement.hpp>
#include <model/utmpunkt.hpp>
#include <model/zusiobjekt.hpp>

using namespace std;

struct Strecke : public ZusiObjekt {

    // Breitengrad (nur Zusi 2).
    breitengrad_t breitengrad;

    // Rekursionstiefe Fahrstraßensuche (nur Zusi 2).
    rekursionstiefe_t rekursionstiefe;

    // Gebietsschema (nur Zusi 2).
    string gebietsschema;

    // Landschaftsdatei.

    // UTM-Punkt.
    UTMPunkt utmPunkt;

    // Haltabstand vor Signalen (nur Zusi 2).
    meter_t signalHaltabstand;

    // Die in der Strecke enthaltenen Elemente. Die Position innerhalb der
    // Liste entspricht der Nummer des Streckenelements.
    vector<unique_ptr<Streckenelement>> streckenelemente;

    // Referenzpunkte (Aufgleispunkte etc.)
    vector<unique_ptr<Referenzpunkt>> referenzpunkte;

    // Streckenfeste Blickpunkte.
    vector<unique_ptr<Blickpunkt>> blickpunkte;

    // Register.
    unordered_map<fahrstr_register_nr_t, unique_ptr<FahrstrRegister>> fahrstrRegister;

    Strecke(string formatVersion, string formatMinVersion)
        : ZusiObjekt(formatVersion, formatMinVersion),
        breitengrad(), rekursionstiefe(), gebietsschema(), utmPunkt(), signalHaltabstand(),
        streckenelemente(), referenzpunkte(), blickpunkte(), fahrstrRegister() {}
    virtual ~Strecke() {}
};

#endif
