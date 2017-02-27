#ifndef SRC_MODEL_FAHRPLAN_HPP
#define SRC_MODEL_FAHRPLAN_HPP

#include <memory>

#include <model/zusiobjekt.hpp>

using namespace std;

struct Fahrplan : public ZusiObjekt {
    // TODO: p/phi unterstuetzen, wenn dereinst notwendig
    vector<string> streckenmodule;

    Fahrplan(string formatVersion, string formatMinVersion)
        : ZusiObjekt(formatVersion, formatMinVersion), streckenmodule() {}
    virtual ~Fahrplan() {}
};

#endif
