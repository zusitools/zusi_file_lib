#ifndef SRC_MODEL_ZUSIOBJEKT_HPP
#define SRC_MODEL_ZUSIOBJEKT_HPP

#include <model/dateiinfo.hpp>

// Ein allgemeines in Zusi verwendetes Objekt.
struct ZusiObjekt {

    // Allgemeine Informationen über die Datei.
    shared_ptr<DateiInfo> dateiInfo;

    // Objekt-ID.
    objektid_t objektId;

    // Lizenz.
    // Einsetzbar ab.
    // Einsetzbar bis.
    // Kategorie.

    // Kurze Beschreibung.
    string beschreibung;

};

#endif
