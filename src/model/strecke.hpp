#ifndef SRC_MODEL_STRECKE_HPP
#define SRC_MODEL_STRECKE_HPP

#include <memory>
#include <unordered_map>
#include <vector>

#include <model/blickpunkt.hpp>
#include <model/fahrstr_register.hpp>
#include <model/referenzpunkt.hpp>
#include <model/streckenelement.hpp>
#include <model/utmpunkt.hpp>
#include <model/zusiobjekt.hpp>

// Ein Verweis auf ein (noch nicht geladenes) Streckenelement.
// Die Nummer ist entweder eine Streckenelementnummer (wenn der Verweis im aktuellen Modul liegt)
// oder eine Referenzpunkt-Nummer (wenn der Verweis in einem anderen Modul liegt).
struct StreckenelementAufloeseInfo {
    StreckenelementUndRichtung element_richtung;
    nachfolger_index_t nachfolger_index;

    bool anderes_modul;
    std::string modul;

    union {
        streckenelement_nr_t nr_se;
        referenz_nr_t nr_ref;
    } ref;

    StreckenelementAufloeseInfo(StreckenelementUndRichtung element_richtung,
            nachfolger_index_t nachfolger_index,
            streckenelement_nr_t nr_se)
                : element_richtung(element_richtung), nachfolger_index(nachfolger_index), anderes_modul(false), modul() {
        ref.nr_se = nr_se;
    }

    StreckenelementAufloeseInfo(StreckenelementUndRichtung element_richtung,
            nachfolger_index_t nachfolger_index,
            const std::string& modul,
            referenz_nr_t nr_ref)
                : element_richtung(element_richtung), nachfolger_index(nachfolger_index), anderes_modul(true), modul(modul) {
        ref.nr_ref = nr_ref;
    }
};

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
    std::vector<std::unique_ptr<Streckenelement>> streckenelemente;

    // Informationen zu unaufgeloesten Elementverknuepfungen.
    std::vector<StreckenelementAufloeseInfo> aufloeseInfo;

    // Referenzpunkte (Aufgleispunkte etc.)
    std::vector<std::unique_ptr<Referenzpunkt>> referenzpunkte;

    // Streckenfeste Blickpunkte.
    std::vector<std::unique_ptr<Blickpunkt>> blickpunkte;

    // Register.
    unordered_map<fahrstr_register_nr_t, unique_ptr<FahrstrRegister>> fahrstrRegister;

    Strecke(string formatVersion, string formatMinVersion)
        : ZusiObjekt(formatVersion, formatMinVersion),
        breitengrad(), rekursionstiefe(), gebietsschema(), utmPunkt(), signalHaltabstand(),
        streckenelemente(), aufloeseInfo(), referenzpunkte(), blickpunkte(), fahrstrRegister() {}
    virtual ~Strecke() {}
};

#endif
