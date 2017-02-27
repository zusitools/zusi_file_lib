#ifndef SRC_MODEL_ZUSIOBJEKT_HPP
#define SRC_MODEL_ZUSIOBJEKT_HPP

#include <model/autorinfo.hpp>

// Ein allgemeines in Zusi verwendetes Objekt.
struct ZusiObjekt {

    // Dateiformat-Version.
    string formatVersion;

    // Dateiformat-Min.-Version
    string formatMinVersion;

    // Autoren-Info.
    vector<shared_ptr<AutorInfo>> autorInfo;

    // Beschreibung des Objekts.
    string beschreibung;

    // Objekt-ID.
    objektid_t objektId;

    // Lizenz.
    // Einsetzbar ab.
    // Einsetzbar bis.
    // Kategorie.

public:
    ZusiObjekt(std::string formatVersion, std::string formatMinVersion)
        : formatVersion(formatVersion), formatMinVersion(formatMinVersion),
          autorInfo(), beschreibung(), objektId() {}
    virtual ~ZusiObjekt() {}
};

#endif
