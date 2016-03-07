#ifndef SRC_IO_STR_LESER_HPP
#define SRC_IO_STR_LESER_HPP

#include <istream>

#include <io/z2_leser.hpp>
#include <model/strecke.hpp>

#ifdef SWIG
#define unique_ptr shared_ptr
#endif

using namespace std;

class StrLeser : public Z2Leser {

public:
    unique_ptr<Strecke> liesStrDatei(istream& datei);
    unique_ptr<Strecke> liesStrDateiMitDateiname(const string dateiname);

private:
    void liesStreckenelemente(istream& datei, Strecke& strecke);
    void liesFahrstrSignal(istream& datei, Streckenelement& element, Strecke& strecke);
    unique_ptr<Signal> liesKombiSignal(istream& datei);
    void liesHauptsignal(istream& datei, Streckenelement& element);
    void liesVorsignal(istream& datei, Streckenelement& element);
    unique_ptr<Ereignis> neuesEreignis(ereignis_nr_t ereignisNr);
};

#endif
