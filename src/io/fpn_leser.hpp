#ifndef SRC_IO_FPN_LESER_HPP
#define SRC_IO_FPN_LESER_HPP

#include <io/z3_leser.hpp>
#include <model/fahrplan.hpp>

using namespace std;

class FpnLeser : public Z3Leser<Fahrplan> {

public:
    virtual unique_ptr<Fahrplan> parseWurzel(xml_node<>& wurzel) override;

};

#endif
