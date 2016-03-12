#ifndef SRC_IO_Z2_LESER_HPP
#define SRC_IO_Z2_LESER_HPP

#include <cstdint>
#include <istream>
#include <string>

#ifdef ZUSI_FILE_LIB_NOINLINE
#define _ZUSI_FILE_LIB_INLINE
#else
#define _ZUSI_FILE_LIB_INLINE inline
#endif

using namespace std;

// Basisklasse für Dateileser, die das Zusi-2-Datenformat lesen.
class Z2Leser {

protected:
    // Liest eine Zeile aus einem Stream und entfernt Zeilenende-Zeichen.
    _ZUSI_FILE_LIB_INLINE string liesZeile(istream& datei);
    _ZUSI_FILE_LIB_INLINE string liesZeile(const string& aktElement, istream& datei);

    // Liest eine Zeile mit einem Integer-Wert aus einem Stream.
    // Wirft std::invalid_argument, wenn keine Konversion durchgeführt werden konnte.
    _ZUSI_FILE_LIB_INLINE int_fast32_t liesGanzzahl(istream& datei);
    _ZUSI_FILE_LIB_INLINE int_fast32_t liesGanzzahl(const string& aktElement, istream &datei);
    
    // Liest einen String mit einem Integer-Wert in eine Ganzzahl.
    _ZUSI_FILE_LIB_INLINE int_fast32_t konvertiereInGanzzahl(string zeile);
    _ZUSI_FILE_LIB_INLINE int_fast32_t konvertiereInGanzzahl(const string& aktElement, string zeile);

    // Liest eine Zeile mit einem Gleitkomma-Wert (Dezimalkomma) aus einem Stream.
    // Wirft std::invalid_argument, wenn keine Konversion durchgeführt werden konnte.
    _ZUSI_FILE_LIB_INLINE double liesGleitkommazahl(istream& datei);
    _ZUSI_FILE_LIB_INLINE double liesGleitkommazahl(const string& aktElement, istream& datei);

    // Konvertiert einen String mit Dezimalkomma in eine Gleitkommazahl.
    _ZUSI_FILE_LIB_INLINE double konvertiereInGleitkommazahl(string zeile);
    _ZUSI_FILE_LIB_INLINE double konvertiereInGleitkommazahl(const string& aktElement, string zeile);

    // Liest einen durch "#" abgeschlossenen mehrzeiligen String aus der Datei. Das '#' und das
    // letzte Zeilenende werden dabei nicht in den String aufgenommen. Zeilenenden werden auf '\n'
    // normiert.
    _ZUSI_FILE_LIB_INLINE string liesMehrzeiligenString(istream& datei);
    _ZUSI_FILE_LIB_INLINE string liesMehrzeiligenString(const string& aktElement, istream& datei);

    // Die aktuelle Zeilennummer innerhalb der Datei. TODO: Threadsicherheit?
    size_t zeilenNr;

    // Beschreibung des aktuell gelesenen Elements.
    string aktElement;

};

#undef _ZUSI_FILE_LIB_INLINE

#ifndef ZUSI_FILE_LIB_NOINLINE
#include "z2_leser.cpp"
#endif

#endif
