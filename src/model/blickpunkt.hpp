#ifndef SRC_MODEL_BLICKPUNKT_HPP
#define SRC_MODEL_BLICKPUNKT_HPP

#include <string>

#include <common/types.hpp>

using namespace std;

// Streckenfester Blickpunkt.
struct Blickpunkt {

    // Bezeichnung.
    string bezeichnung;

    // Position im 3D-Raum.
    Pos3D position;

};

#endif
