#ifndef SRC_MODEL_REFERENZPUNKT_HPP
#define SRC_MODEL_REFERENZPUNKT_HPP

#include <string>

#include <common/types.hpp>
#include <model/streckenelement.hpp>

// Ein Referenzpunkt einer Strecke. Die Referenz kann aufgelöst sein (der
// Streckenelement-Zeiger hat einen Wert) oder nicht (z.B. wenn das Element nicht existiert).
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

    // Referenz-Nummer innerhalb des Moduls, in dem die Referenz deklariert ist.
    referenz_nr_t referenzNr;

    // Referenz-Typ.
    referenz_typ_t referenzTyp;

    // Das Streckenelement und die Richtung, auf das sich die Referenz bezieht.
    StreckenelementUndRichtung elementRichtung;

    // Die Nummer des Streckenelements, auf das sich die Referenz bezieht.
    streckenelement_nr_t streckenelementNr;

    // Beschreibung.
    std::string beschreibung;

};

#endif
