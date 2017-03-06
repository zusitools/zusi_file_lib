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
// Nicht reentrant!
class Z2Parser {

protected:
    _ZUSI_FILE_LIB_INLINE bool liesZeilenende();
    _ZUSI_FILE_LIB_INLINE bool liesRauteZeilenende();

    // Liest eine Zeile aus einem Stream und entfernt Zeilenende-Zeichen.
    _ZUSI_FILE_LIB_INLINE string liesZeile();
    _ZUSI_FILE_LIB_INLINE string liesZeile(const string& aktElement);

    // Liest eine Zeile mit einem Integer-Wert aus einem Stream.
    // Wirft std::invalid_argument, wenn keine Konversion durchgeführt werden konnte.
    _ZUSI_FILE_LIB_INLINE int_fast32_t liesGanzzahl();
    _ZUSI_FILE_LIB_INLINE int_fast32_t liesGanzzahl(const string& aktElement);
    
    // Liest einen String mit einem Integer-Wert in eine Ganzzahl.
    _ZUSI_FILE_LIB_INLINE int_fast32_t konvertiereInGanzzahl(string zeile);
    _ZUSI_FILE_LIB_INLINE int_fast32_t konvertiereInGanzzahl(const string& aktElement, string zeile);

    // Liest eine Zeile mit einem Gleitkomma-Wert (Dezimalkomma) aus einem Stream.
    // Wirft std::invalid_argument, wenn keine Konversion durchgeführt werden konnte.
    _ZUSI_FILE_LIB_INLINE double liesGleitkommazahl();
    _ZUSI_FILE_LIB_INLINE double liesGleitkommazahl(const string& aktElement);

    // Konvertiert einen String mit Dezimalkomma in eine Gleitkommazahl.
    _ZUSI_FILE_LIB_INLINE double konvertiereInGleitkommazahl(string zeile);
    _ZUSI_FILE_LIB_INLINE double konvertiereInGleitkommazahl(const string& aktElement, string zeile);

    // Liest einen durch "#" abgeschlossenen mehrzeiligen String aus der Datei. Das '#' und das
    // letzte Zeilenende werden dabei nicht in den String aufgenommen. Zeilenenden werden auf '\n'
    // normiert.
    _ZUSI_FILE_LIB_INLINE string liesMehrzeiligenString();
    _ZUSI_FILE_LIB_INLINE string liesMehrzeiligenString(const string& aktElement);

    // Die einzulesende Datei
    std::vector<char> buffer;

    // Die Position innerhalb der Datei
    std::vector<char>::const_iterator pos;

    // Die aktuelle Zeilennummer innerhalb der Datei.
    size_t zeilenNr;

    // Beschreibung des aktuell gelesenen Elements.
    string aktElement;
};

template<typename R>
class Z2Leser : public Z2Parser {
    virtual unique_ptr<R> parse() = 0; // Hier wird das dateispezifische Parsen implementiert.

public:
    unique_ptr<R> liesDatei(istream& stream) {
        stream.seekg(0, ios::end);
        size_t size = stream.tellg();
        stream.seekg(0);

        this->buffer = vector<char>(size);
        stream.read(&this->buffer.front(), size);
        this->pos = this->buffer.begin();
        return this->parse();
    }

    unique_ptr<R> liesDateiMitDateiname(const string dateiname) {
        ifstream datei(dateiname, std::ios::binary);
        return this->liesDatei(datei);
    }
};

#undef _ZUSI_FILE_LIB_INLINE

#ifndef ZUSI_FILE_LIB_NOINLINE
#include "z2_leser.cpp"
#endif

#endif
