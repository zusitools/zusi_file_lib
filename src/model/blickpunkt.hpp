#ifndef SRC_MODEL_BLICKPUNKT_HPP
#define SRC_MODEL_BLICKPUNKT_HPP

#include <string>

#include <model/punkt3d.hpp>

using namespace std;

// Streckenfester Blickpunkt.
struct Blickpunkt {

    // Bezeichnung.
    string name;

    // Position im 3D-Raum.
    Pos3D position;

};

#endif
