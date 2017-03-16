#ifndef SRC_IO_FPN_LESER_HPP
#define SRC_IO_FPN_LESER_HPP

#include <memory>

#include "lib/rapidxml-1.13/rapidxml.hpp"

#include "io/z3_leser.hpp"
#include "model/fahrplan.hpp"

#ifdef ZUSI_FILE_LIB_NOINLINE
#define _ZUSI_FILE_LIB_INLINE
#else
#define _ZUSI_FILE_LIB_INLINE inline
#endif

using rapidxml::xml_node;

class FpnLeser : public Z3Leser<Fahrplan> {

public:
    _ZUSI_FILE_LIB_INLINE std::unique_ptr<Fahrplan> parseWurzel(const xml_node<>& wurzel) override;

};

#undef _ZUSI_FILE_LIB_INLINE

#ifndef ZUSI_FILE_LIB_NOINLINE
#include "fpn_leser.cpp"
#endif

#endif
