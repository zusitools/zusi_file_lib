#ifndef SRC_IO_STR_LESER_HPP
#define SRC_IO_STR_LESER_HPP

#include <memory>
#include <vector>

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

class StrLeser : public Z2Leser<Strecke> {

protected:
    _ZUSI_FILE_LIB_INLINE virtual unique_ptr<Strecke> parse();

private:
    _ZUSI_FILE_LIB_INLINE void liesStreckenelemente(Strecke& strecke);
    _ZUSI_FILE_LIB_INLINE void liesFahrstrSignal(Streckenelement& element, Strecke& strecke);
    _ZUSI_FILE_LIB_INLINE unique_ptr<Signal> liesKombiSignal();
    _ZUSI_FILE_LIB_INLINE void liesHauptsignal(Streckenelement& element);
    _ZUSI_FILE_LIB_INLINE void liesVorsignal(Streckenelement& element);
    _ZUSI_FILE_LIB_INLINE void neuesEreignis(ereignis_nr_t ereignisNr, Ereignis& ereignis);
};

#undef _ZUSI_FILE_LIB_INLINE

#ifndef ZUSI_FILE_LIB_NOINLINE
#include "str_leser.cpp"
#endif

#endif
