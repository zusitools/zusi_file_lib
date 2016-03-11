#ifndef SRC_IO_ST3_LESER_HPP
#define SRC_IO_ST3_LESER_HPP

#include <io/z3_leser.hpp>
#include <model/strecke.hpp>

using namespace std;

class St3Leser : public Z3Leser<Strecke> {

protected:
    virtual unique_ptr<Strecke> parseWurzel(xml_node<>& wurzel) override;

};

#endif
