#ifndef SRC_IO_Z3_LESER_HPP
#define SRC_IO_Z3_LESER_HPP

#include <memory>

#include "model/dateiinfo.hpp"
#include "lib/rapidxml-1.13/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

// Basisklasse für Dateileser, die das Zusi-3-Datenformat lesen.
class Z3Leser {

protected:
    unique_ptr<DateiInfo> liesDateiInfo(xml_node<>& wurzel);
};

#endif
