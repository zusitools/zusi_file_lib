#ifndef SRC_MODEL_DATEIINFO_HPP
#define SRC_MODEL_DATEIINFO_HPP

#include <memory>
#include <string>
#include <vector>

#include <model/autorinfo.hpp>

using namespace std;

// Informationen über eine Zusi-Datei.
struct DateiInfo {

    // Autoren-Info.
    vector<unique_ptr<AutorInfo>> autorInfo;

    // Beschreibung des Objekts.
    string beschreibung;

    // Version.
    // MinVersion.

};

#endif
