#ifndef SRC_IO_FPN_LESER_HPP
#define SRC_IO_FPN_LESER_HPP

#include <io/z3_leser.hpp>
#include <model/fahrplan.hpp>

using namespace std;

class FpnLeser : public Z3Leser {

public:
    unique_ptr<Fahrplan> liesFpnDatei(istream& datei);
    unique_ptr<Fahrplan> liesFpnDateiMitDateiname(const string dateiname);

};

#endif
