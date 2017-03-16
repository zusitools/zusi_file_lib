#ifndef SRC_MODEL_ZUSIOBJEKT_HPP
#define SRC_MODEL_ZUSIOBJEKT_HPP

#include <string>
#include <vector>
#include <memory>

#include <model/autorinfo.hpp>

// Ein allgemeines in Zusi verwendetes Objekt.
struct ZusiObjekt {

    // Dateiformat-Version.
    std::string formatVersion;

    // Dateiformat-Min.-Version
    std::string formatMinVersion;

    // Autoren-Info.
    std::vector<std::shared_ptr<AutorInfo>> autorInfo;

    // Beschreibung des Objekts.
    std::string beschreibung;

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
