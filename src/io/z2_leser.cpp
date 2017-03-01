#ifdef DEBUG
#include <iostream>
#endif
#include <stdexcept>

#include <boost/spirit/include/qi_real.hpp>
#include <boost/spirit/include/qi_int.hpp>

#include <io/z2_leser.hpp>

using namespace std;
namespace qi = boost::spirit::qi;

template <typename T>
struct decimal_comma_real_policies : qi::real_policies<T>
{
    template <typename Iterator> static bool parse_dot(Iterator& first, Iterator const& last)
    {
        if (first == last || *first != ',')
            return false;
        ++first;
        return true;
    }
};

string Z2Leser::liesZeile(istream& datei) {
    string result;
    getline(datei, result);
#ifdef DEBUG
    cout << this->zeilenNr << ":" << aktElement << ":" << result << endl;
#endif
    this->zeilenNr++;
    auto substrLaenge = result.size();
    while (substrLaenge > 0 &&
            (result.at(substrLaenge - 1) == '\r' || result.at(substrLaenge - 1) == '\n')) {
        substrLaenge--;
    }
    return result.substr(0, substrLaenge);
}

string Z2Leser::liesZeile(const string& aktElement, istream& datei) {
    this->aktElement = aktElement;
    return liesZeile(datei);
}

int_fast32_t Z2Leser::liesGanzzahl(istream &datei) {
    return Z2Leser::konvertiereInGanzzahl(Z2Leser::liesZeile(datei));
}

int_fast32_t Z2Leser::liesGanzzahl(const string& aktElement, istream& datei) {
    this->aktElement = aktElement;
    return liesGanzzahl(datei);
}

int_fast32_t Z2Leser::konvertiereInGanzzahl(string zeile) {
    auto begin = zeile.begin(), end = zeile.end();
    int_fast32_t result;
    if (!qi::parse(begin, end, qi::int_, result) || (begin != end)) {
        throw invalid_argument("'" + zeile + "' kann nicht in eine Ganzzahl konvertiert werden.");
    }
    return result;
}

int_fast32_t Z2Leser::konvertiereInGanzzahl(const string& aktElement, string zeile) {
    this->aktElement = aktElement;
    return konvertiereInGanzzahl(zeile);
}

double Z2Leser::liesGleitkommazahl(istream& datei) {
    return Z2Leser::konvertiereInGleitkommazahl(Z2Leser::liesZeile(datei));
}

double Z2Leser::liesGleitkommazahl(const string& aktElement, istream& datei) {
    this->aktElement = aktElement;
    return liesGleitkommazahl(datei);
}

double Z2Leser::konvertiereInGleitkommazahl(string zeile) {
    auto begin = zeile.begin(), end = zeile.end();
    double result;
    qi::real_parser<double, decimal_comma_real_policies<double> > parser;
    if (!qi::parse(begin, end, parser, result) || (begin != end)) {
        throw invalid_argument("'" + zeile + "' kann nicht in eine Gleitkommazahl konvertiert werden.");
    }
    return result;
}

double Z2Leser::konvertiereInGleitkommazahl(const string& aktElement, string zeile) {
    this->aktElement = aktElement;
    return konvertiereInGleitkommazahl(zeile);
}

string Z2Leser::liesMehrzeiligenString(istream& datei) {
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

string Z2Leser::liesMehrzeiligenString(const string& aktElement, istream& datei) {
    this->aktElement = aktElement;
    return liesMehrzeiligenString(datei);
}
