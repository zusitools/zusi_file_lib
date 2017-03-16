#ifndef SRC_IO_ST3_LESER_HPP
#define SRC_IO_ST3_LESER_HPP

#include "lib/rapidxml-1.13/rapidxml.hpp"

#include "io/z3_leser.hpp"
#include "model/strecke.hpp"

#ifdef ZUSI_FILE_LIB_NOINLINE
#define _ZUSI_FILE_LIB_INLINE
#else
#define _ZUSI_FILE_LIB_INLINE inline
#endif

using rapidxml::xml_node;

class St3Leser : public Z3Leser<Strecke> {

protected:
    _ZUSI_FILE_LIB_INLINE std::unique_ptr<Strecke> parseWurzel(const xml_node<>& wurzel) override;

};

#undef _ZUSI_FILE_LIB_INLINE

#ifndef ZUSI_FILE_LIB_NOINLINE
#include "st3_leser.cpp"
#endif

#endif
