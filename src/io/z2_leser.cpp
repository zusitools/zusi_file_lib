#include <stdexcept>

#include <io/z2_leser.hpp>

using namespace std;

string Z2Leser::liesZeile(istream &datei) {
    string result;
    getline(datei, result);
    this->zeilenNr++;
    auto substrLaenge = result.size();
    while (substrLaenge > 0 &&
            (result.at(substrLaenge - 1) == '\r' || result.at(substrLaenge - 1) == '\n')) {
        substrLaenge--;
    }
    return result.substr(0, substrLaenge);
}

int_fast32_t Z2Leser::liesGanzzahl(istream &datei) {
    return Z2Leser::konvertiereInGanzzahl(Z2Leser::liesZeile(datei));
}

int_fast32_t Z2Leser::konvertiereInGanzzahl(string zeile) {
    if (zeile.find_first_not_of("-0123456789") != string::npos) {
        throw invalid_argument("'" + zeile + "' kann nicht in eine Ganzzahl konvertiert werden.");
    } else {
        try {
            return stoi(zeile);
        } catch (invalid_argument) {
            throw invalid_argument(
                "'" + zeile + "' kann nicht in eine Ganzzahl konvertiert werden.");
        }
    }
}

double Z2Leser::liesGleitkommazahl(istream &datei) {
    return Z2Leser::konvertiereInGleitkommazahl(Z2Leser::liesZeile(datei));
}

double Z2Leser::konvertiereInGleitkommazahl(string zeile) {
    if (zeile.find_first_not_of("-,0123456789e") != string::npos) {
        throw invalid_argument(zeile + " kann nicht in eine Gleitkommazahl konvertiert werden.");
    } else {
        // Ersetze (erstes) , durch .
        size_t kommaPos = zeile.find(',');
        if (kommaPos != string::npos) {
            zeile[kommaPos] = '.';
        }
        if (zeile.find(',') != string::npos) {
            throw invalid_argument(
                zeile + " kann nicht in eine Gleitkommazahl konvertiert werden.");
        }
        return stof(zeile);
    }
}

string Z2Leser::liesMehrzeiligenString(istream &datei) {
    string zeile;
    string result;
    bool ersteZeile = true;

    do {
      zeile = Z2Leser::liesZeile(datei);
      if (zeile.compare("#") == 0) {
        break;
      }
      if (!ersteZeile) {
        result.append("\n");
      }
      result.append(zeile);
      ersteZeile = false;
    } while (!datei.eof());

    return result;
}
