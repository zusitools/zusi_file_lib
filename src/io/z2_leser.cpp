#ifdef DEBUG
#include <iostream>
#endif
#include <stdexcept>

#include <boost/spirit/include/qi_real.hpp>
#include <boost/spirit/include/qi_int.hpp>
#include <boost/spirit/include/qi_eol.hpp>

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

bool Z2Parser::liesZeilenende() {
    bool found = false;
    if (this->pos != this->buffer.end() && *this->pos == '\r') {
        found = true;
        ++(this->pos);
    }
    if (this->pos != this->buffer.end() && *this->pos == '\n') {
        found = true;
        ++(this->pos);
    }
    if (found) {
        this->zeilenNr++;
    }
    return found;
}

bool Z2Parser::liesRauteZeilenende() {
    if (this->pos != this->buffer.end() && *this->pos == '#') {
        ++(this->pos);
        if (!this->liesZeilenende()) {
            throw invalid_argument("Zusaetzliche Zeichen am Ende der Zeile.");
        }
        return true;
    }
    return false;
}

std::string Z2Parser::liesZeile() {
    std::vector<char>::const_iterator end = this->pos;
    while (end != this->buffer.end() && *end != '\r' && *end != '\n') {
        ++end;
    }
    std::string result(this->pos, end);
    this->pos = end;
    this->liesZeilenende();
    return result;
}

string Z2Parser::liesZeile(const string& aktElement) {
    this->aktElement = aktElement;
    std::string result = this->liesZeile();
#ifdef DEBUG
    cout << this->zeilenNr << ":" << this->aktElement << ":" << result << endl;
#endif
    return result;
}

int_fast32_t Z2Parser::liesGanzzahl() {
    auto it = this->pos;
    double result;
    if (!qi::parse(it, this->buffer.cend(), qi::int_, result)) {
        throw invalid_argument("Zeile kann nicht in eine Ganzzahl konvertiert werden. Gelesener Teil: '" + std::string(this->pos, it) + "'");
    }
    this->pos = it;
    if (!this->liesZeilenende()) {
        throw invalid_argument("Zusaetzliche Zeichen am Ende der Zeile.");
    }
    return result;
}

int_fast32_t Z2Parser::liesGanzzahl(const string& aktElement) {
    this->aktElement = aktElement;
    int_fast32_t result = this->liesGanzzahl();
#ifdef DEBUG
    cout << this->zeilenNr-1 << ":" << this->aktElement << ":" << result << endl;
#endif
    return result;
}

double Z2Parser::liesGleitkommazahl() {
    auto it = this->pos;
    double result;
    qi::real_parser<double, decimal_comma_real_policies<double> > parser;
    if (!qi::parse(it, this->buffer.cend(), parser, result)) {
        throw invalid_argument("Zeile kann nicht in eine Gleitkommazahl konvertiert werden. Gelesener Teil: '" + std::string(this->pos, it) + "'");
    }
    this->pos = it;
    if (!this->liesZeilenende()) {
        throw invalid_argument("Zusaetzliche Zeichen am Ende der Zeile.");
    }
    return result;
}

double Z2Parser::liesGleitkommazahl(const string& aktElement) {
    this->aktElement = aktElement;
    double result = this->liesGleitkommazahl();
#ifdef DEBUG
    cout << this->zeilenNr << ":" << this->aktElement << ":" << result << endl;
#endif
    return result;
}

string Z2Parser::liesMehrzeiligenString() {
    string zeile;
    string result;
    bool ersteZeile = true;

    while (!this->liesRauteZeilenende()) {
        zeile = this->liesZeile(this->aktElement);
        if (ersteZeile) {
            ersteZeile = false;
        } else {
            result.append("\n");
        }
        result.append(zeile);
    }

    return result;
}

string Z2Parser::liesMehrzeiligenString(const string& aktElement) {
    this->aktElement = aktElement;
    return this->liesMehrzeiligenString();
}
