#ifndef SRC_MODEL_EREIGNIS_HPP
#define SRC_MODEL_EREIGNIS_HPP

#include <common/types.hpp>

using namespace std;

// Ein Ereignis, das einer Richtung eines Streckenelements zugeordnet ist.
struct Ereignis {

    // Die Nummer des Ereignisses.
    ereignis_nr_t ereignisNr;

    // Der Parameter "Wert" des Ereignisses.
    ereignis_wert_t wert;

    // Der Parameter "Beschreibung" des Ereignisses.
    string beschreibung;
};

#endif
