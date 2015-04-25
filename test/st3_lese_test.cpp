#define BOOST_TEST_MODULE St3LeseTest
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "io/st3_leser.hpp"

using namespace std;

BOOST_AUTO_TEST_CASE(leere_st3_datei) {
    ifstream infile("./eingabe/zusi3/LeereSt3Datei.st3");
    unique_ptr<Strecke> strecke = St3Leser().liesSt3Datei(infile);
    BOOST_CHECK(strecke);

    BOOST_CHECK(strecke->dateiInfo);

    BOOST_CHECK_EQUAL(strecke->dateiInfo->formatVersion, "A.1");
    BOOST_CHECK_EQUAL(strecke->dateiInfo->formatMinVersion, "A.1");
}
