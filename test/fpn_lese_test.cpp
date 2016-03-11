#define BOOST_TEST_MODULE FpnLeseTest
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "io/fpn_leser.hpp"

using namespace std;

// Wert für Floating-Point-Vergleiche (in Prozent der zu vergleichenden Werte!)
#define epsilon 0.00001

BOOST_AUTO_TEST_CASE(leere_fpn_datei) {
    ifstream infile("./eingabe/zusi3/LeereFpnDatei.fpn");
    unique_ptr<Fahrplan> fahrplan = FpnLeser().liesDatei(infile);
    BOOST_REQUIRE(fahrplan);

    BOOST_CHECK(fahrplan->dateiInfo);

    BOOST_CHECK_EQUAL(fahrplan->dateiInfo->formatVersion, "A.1");
    BOOST_CHECK_EQUAL(fahrplan->dateiInfo->formatMinVersion, "A.1");
}

BOOST_AUTO_TEST_CASE(streckenmodule) {
    ifstream infile("./eingabe/zusi3/StreckenmoduleTest.fpn");
    unique_ptr<Fahrplan> fahrplan = FpnLeser().liesDatei(infile);
    BOOST_REQUIRE(fahrplan);

    BOOST_CHECK_EQUAL(fahrplan->streckenmodule.size(), 2);
    BOOST_CHECK_EQUAL(fahrplan->streckenmodule.at(0), "Routes\\Deutschland\\32U_0005_0057\\000483_005730_Paderborn\\Paderborn_1985.st3");
    BOOST_CHECK_EQUAL(fahrplan->streckenmodule.at(1), "Langeland_1985.st3");
}
