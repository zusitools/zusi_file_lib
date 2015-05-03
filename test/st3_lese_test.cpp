#define BOOST_TEST_MODULE St3LeseTest
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "io/st3_leser.hpp"

using namespace std;

// Wert für Floating-Point-Vergleiche (in Prozent der zu vergleichenden Werte!)
#define epsilon 0.00001

BOOST_AUTO_TEST_CASE(leere_st3_datei) {
    ifstream infile("./eingabe/zusi3/LeereSt3Datei.st3");
    unique_ptr<Strecke> strecke = St3Leser().liesSt3Datei(infile);
    BOOST_CHECK(strecke);

    BOOST_CHECK(strecke->dateiInfo);

    BOOST_CHECK_EQUAL(strecke->dateiInfo->formatVersion, "A.1");
    BOOST_CHECK_EQUAL(strecke->dateiInfo->formatMinVersion, "A.1");
}

BOOST_AUTO_TEST_CASE(element_koordinaten) {
    ifstream infile("./eingabe/zusi3/ElementKoordinatenTest.st3");
    unique_ptr<Strecke> strecke = St3Leser().liesSt3Datei(infile);

    BOOST_REQUIRE_EQUAL(strecke->streckenelemente.size(), 2);

    auto& element = strecke->streckenelemente.at(1);
    BOOST_CHECK_CLOSE(element->p1.x, -10, epsilon);
    BOOST_CHECK_CLOSE(element->p1.y, -23.5, epsilon);
    BOOST_CHECK_CLOSE(element->p1.z, 5, epsilon);
    BOOST_CHECK_CLOSE(element->p2.x, 10, epsilon);
    BOOST_CHECK_CLOSE(element->p2.y, 20, epsilon);
    BOOST_CHECK_CLOSE(element->p2.z, -30, epsilon);
}
