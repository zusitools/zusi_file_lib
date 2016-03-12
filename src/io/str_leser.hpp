#ifndef SRC_IO_STR_LESER_HPP
#define SRC_IO_STR_LESER_HPP

#include <istream>

#include <io/z2_leser.hpp>
#include <model/strecke.hpp>

#ifdef SWIG
#define unique_ptr shared_ptr
#endif

#ifdef ZUSI_FILE_LIB_NOINLINE
#define _ZUSI_FILE_LIB_INLINE
#else
#define _ZUSI_FILE_LIB_INLINE inline
#endif

using namespace std;

class StrLeser : public Z2Leser {

public:
    _ZUSI_FILE_LIB_INLINE unique_ptr<Strecke> liesStrDatei(istream& datei);
    _ZUSI_FILE_LIB_INLINE unique_ptr<Strecke> liesStrDateiMitDateiname(const string dateiname);

private:
    _ZUSI_FILE_LIB_INLINE void liesStreckenelemente(istream& datei, Strecke& strecke);
    _ZUSI_FILE_LIB_INLINE void liesFahrstrSignal(istream& datei, Streckenelement& element, Strecke& strecke);
    _ZUSI_FILE_LIB_INLINE unique_ptr<Signal> liesKombiSignal(istream& datei);
    _ZUSI_FILE_LIB_INLINE void liesHauptsignal(istream& datei, Streckenelement& element);
    _ZUSI_FILE_LIB_INLINE void liesVorsignal(istream& datei, Streckenelement& element);
    _ZUSI_FILE_LIB_INLINE void neuesEreignis(ereignis_nr_t ereignisNr, Ereignis& ereignis);
};

#undef _ZUSI_FILE_LIB_INLINE

#ifndef ZUSI_FILE_LIB_NOINLINE
#include "str_leser.cpp"
#endif

#endif
