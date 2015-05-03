#ifndef SRC_IO_ST3_LESER_HPP
#define SRC_IO_ST3_LESER_HPP

#include <io/z3_leser.hpp>
#include <model/strecke.hpp>

using namespace std;

class St3Leser : public Z3Leser {

public:
    unique_ptr<Strecke> liesSt3Datei(istream& datei);
    unique_ptr<Strecke> liesSt3DateiMitDateiname(const string dateiname);

};

#endif
