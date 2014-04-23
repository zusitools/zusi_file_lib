#ifndef SRC_IO_Z2_LESER_HPP
#define SRC_IO_Z2_LESER_HPP

#include <cstdint>
#include <istream>
#include <string>

using namespace std;

// Basisklasse für Dateileser, die das Zusi-2-Datenformat lesen.
class Z2Leser {

protected:
    // Liest eine Zeile aus einem Stream und entfernt Zeilenende-Zeichen.
    static string liesZeile(istream &datei);

    // Liest eine Zeile mit einem Integer-Wert aus einem Stream.
    // Wirft std::invalid_argument, wenn keine Konversion durchgeführt werden konnte.
    static int_fast32_t liesGanzzahl(istream &atei);

    // Liest einen durch "#" abgeschlossenen mehrzeiligen String aus der Datei. Das '#' und das
    // letzte Zeilenende werden dabei nicht in den String aufgenommen. Zeilenenden werden auf '\n'
    // normiert.
    static string liesMehrzeiligenString(istream &datei);

};

#endif
