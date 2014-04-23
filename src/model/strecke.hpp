#ifndef SRC_MODEL_STRECKE_HPP
#define SRC_MODEL_STRECKE_HPP

using namespace std;

#include <model/streckenelement.hpp>
#include <model/zusiobjekt.hpp>

struct Strecke : public ZusiObjekt {

    // Breitengrad (nur Zusi 2).

    // Rekursionstiefe (nur Zusi 2).

    // Gebietsschema (nur Zusi 2).
    string gebietsschema;

    // Landschaft.

    // UTM-Info.

    // Haltabstand vor Signalen (nur Zusi 2).

    // Die in der Strecke enthaltenen Elemente. Die Position innerhalb der
    // Liste entspricht der Nummer des Streckenelements.
    vector<Streckenelement> streckenelemente;

    // Referenzpunkte (Aufgleispunkte etc.)

    // Streckenfeste Blickpunkte.

};

#endif
