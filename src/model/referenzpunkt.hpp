#ifndef SRC_MODEL_REFERENZPUNKT_HPP
#define SRC_MODEL_REFERENZPUNKT_HPP

#include <string>

#include <common/types.hpp>
#include <model/streckenelement.hpp>

using namespace std;

// Ein Referenzpunkt einer Strecke.
struct Referenzpunkt {

    // Konstanten für den Referenzpunkt-Typ.
    enum Typ {
        Aufgleispunkt = 0,
        Modulgrenze = 1,
        Register = 2,
        Weiche = 3,
        Signal = 4,
        Aufloesepunkt = 5,
        Signalhaltfall = 6
    };

    // Referenz-Nummer in der fertig zusammengesetzten Strecke.
    referenz_nr_t referenzNr;

    // Referenz-Nummer innerhalb des Moduls, in dem die Referenz deklariert ist.
    referenz_nr_t referenzNrInModul;

    // Referenz-Typ.
    referenz_typ_t referenzTyp;

    // Streckenelement, auf das sich die Referenz bezieht.
    StreckenelementReferenz element;

    // Beschreibung.
    string beschreibung;

};

#endif
