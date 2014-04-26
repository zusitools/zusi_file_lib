#ifndef SRC_MODEL_FAHRSTR_REGISTER_HPP
#define SRC_MODEL_FAHRSTR_REGISTER_HPP

#include <common/types.hpp>

using namespace std;

struct FahrstrRegister {

    // Die Nummer des Registers.
    fahrstr_register_nr_t registerNr;

    // Ob das Register manuell gesetzt wurde (und nicht automatisch vom Streckeneditor).
    bool manuell;

};

#endif
