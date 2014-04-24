#ifndef SRC_IO_STR_LESER_HPP
#define SRC_IO_STR_LESER_HPP

#include <istream>

#include <io/z2_leser.hpp>
#include <model/strecke.hpp>

using namespace std;

class StrLeser : public Z2Leser {

public:
    unique_ptr<Strecke> liesStrDatei(istream& datei);
    unique_ptr<Strecke> liesStrDateiMitDateiname(const string dateiname);

private:
    void liesStreckenelemente(istream& datei, unique_ptr<Strecke>& strecke);
};

#endif
