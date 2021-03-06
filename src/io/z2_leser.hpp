#ifndef SRC_IO_Z2_LESER_HPP
#define SRC_IO_Z2_LESER_HPP

#include <cstdint>
#include <istream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#ifdef ZUSI_FILE_LIB_NOINLINE
#define _ZUSI_FILE_LIB_INLINE
#else
#define _ZUSI_FILE_LIB_INLINE inline
#endif

// Basisklasse für Dateileser, die das Zusi-2-Datenformat lesen.
// Nicht reentrant!
class Z2Parser {

protected:
    _ZUSI_FILE_LIB_INLINE bool liesZeilenende();
    _ZUSI_FILE_LIB_INLINE bool liesRauteZeilenende();

    // Liest eine Zeile aus einem Stream und entfernt Zeilenende-Zeichen.
    _ZUSI_FILE_LIB_INLINE std::pair<std::vector<char>::const_iterator, std::vector<char>::const_iterator> liesZeile();
    _ZUSI_FILE_LIB_INLINE std::pair<std::vector<char>::const_iterator, std::vector<char>::const_iterator> liesZeile(const char* aktElement);

    // Liest eine Zeile mit einem Integer-Wert aus einem Stream.
    // Wirft std::invalid_argument, wenn keine Konversion durchgeführt werden konnte.
    _ZUSI_FILE_LIB_INLINE int_fast32_t liesGanzzahl();
    _ZUSI_FILE_LIB_INLINE int_fast32_t liesGanzzahl(const char* aktElement);
    
    // Liest einen String mit einem Integer-Wert in eine Ganzzahl.
    _ZUSI_FILE_LIB_INLINE int_fast32_t konvertiereInGanzzahl(std::string zeile);
    _ZUSI_FILE_LIB_INLINE int_fast32_t konvertiereInGanzzahl(const char* aktElement, std::string zeile);

    // Liest eine Zeile mit einem Gleitkomma-Wert (Dezimalkomma) aus einem Stream.
    // Wirft std::invalid_argument, wenn keine Konversion durchgeführt werden konnte.
    _ZUSI_FILE_LIB_INLINE double liesGleitkommazahl();
    _ZUSI_FILE_LIB_INLINE double liesGleitkommazahl(const char* aktElement);

    // Konvertiert einen String mit Dezimalkomma in eine Gleitkommazahl.
    _ZUSI_FILE_LIB_INLINE double konvertiereInGleitkommazahl(std::string zeile);
    _ZUSI_FILE_LIB_INLINE double konvertiereInGleitkommazahl(const char* aktElement, std::string zeile);

    // Liest einen durch "#" abgeschlossenen mehrzeiligen String aus der Datei. Das '#' und das
    // letzte Zeilenende werden dabei nicht in den String aufgenommen. Zeilenenden werden auf '\n'
    // normiert.
    _ZUSI_FILE_LIB_INLINE std::string liesMehrzeiligenString();
    _ZUSI_FILE_LIB_INLINE std::string liesMehrzeiligenString(const char* aktElement);

    // Die einzulesende Datei
    std::vector<char> buffer;

    // Die Position innerhalb der Datei
    std::vector<char>::const_iterator pos;

    // Die aktuelle Zeilennummer innerhalb der Datei.
    size_t zeilenNr;

    // Beschreibung des aktuell gelesenen Elements (zum Debuggen).
    const char* aktElement;
};

template<typename R>
class Z2Leser : public Z2Parser {
    virtual std::unique_ptr<R> parse() = 0; // Hier wird das dateispezifische Parsen implementiert.

public:
    std::unique_ptr<R> liesDatei(std::istream& stream) {
        stream.seekg(0, std::ios::end);
        size_t size = stream.tellg();
        stream.seekg(0);

        this->buffer = std::vector<char>(size);
        stream.read(&this->buffer.front(), size);
        this->pos = this->buffer.begin();
        return this->parse();
    }

    std::unique_ptr<R> liesDateiMitDateiname(const std::string& dateiname) {
        try {
            std::ifstream datei(dateiname, std::ios::binary);
            if (datei.fail()) {
                throw std::runtime_error("Kann Datei nicht öffnen");
            }
            return this->liesDatei(datei);
        } catch (const std::exception& e) {
            using namespace std::literals::string_literals;
            throw std::runtime_error("Fehler beim Öffnen von '"s + dateiname + "': " + e.what());
        }
    }
};

#undef _ZUSI_FILE_LIB_INLINE

#ifndef ZUSI_FILE_LIB_NOINLINE
#include "z2_leser.cpp"
#endif

#endif
