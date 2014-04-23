#include <io/z2_leser.hpp>

string Z2Leser::liesZeile(istream &datei) {
    string result;
    getline(datei, result);
    auto substrLaenge = result.size();
    while (substrLaenge > 0 &&
            (result.at(substrLaenge - 1) == '\r' || result.at(substrLaenge - 1) == '\n')) {
        substrLaenge--;
    }
    return result.substr(0, substrLaenge);
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
