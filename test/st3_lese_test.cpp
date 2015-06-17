#define BOOST_TEST_MODULE St3LeseTest
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "io/st3_leser.hpp"

using namespace std;

// Wert für Floating-Point-Vergleiche (in Prozent der zu vergleichenden Werte!)
#define epsilon 0.00001

#include "testhelpers.hpp"

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

BOOST_AUTO_TEST_CASE(vorgaenger_nachfolger) {
    ifstream infile("./eingabe/zusi3/VorgaengerNachfolgerTest.st3");
    unique_ptr<Strecke> strecke = St3Leser().liesSt3Datei(infile);

    // Es werden die Vorgaenger verglichen, nicht die Nachfolger in Gegenrichtung
    // (wichtig fuer die Richtungsangaben).
    testVorgaengerNachfolger(strecke, 1, {4}, {Streckenelement::RICHTUNG_GEGEN}, {2}, {Streckenelement::RICHTUNG_GEGEN});
    testVorgaengerNachfolger(strecke, 2, {3}, {Streckenelement::RICHTUNG_NORM},  {1}, {Streckenelement::RICHTUNG_GEGEN});
    testVorgaengerNachfolger(strecke, 3, {},  {},                                {2}, {Streckenelement::RICHTUNG_NORM});
    testVorgaengerNachfolger(strecke, 4, {1}, {Streckenelement::RICHTUNG_GEGEN}, {},  {});
}

BOOST_AUTO_TEST_CASE(utm_punkt) {
    ifstream infile("./eingabe/zusi3/UTMPunktTest.st3");
    unique_ptr<Strecke> strecke = St3Leser().liesSt3Datei(infile);

    BOOST_CHECK_EQUAL(strecke->utmPunkt.we, 42);
    BOOST_CHECK_EQUAL(strecke->utmPunkt.ns, -90);
}

BOOST_AUTO_TEST_CASE(element_flags) {
    ifstream infile("./eingabe/zusi3/ElementFlagsTest.st3");
    unique_ptr<Strecke> strecke = St3Leser().liesSt3Datei(infile);

    BOOST_REQUIRE_EQUAL(strecke->streckenelemente.size(), 8);

    testSetEqual(strecke->streckenelemente.at(1)->flags,
        set<StreckenelementFlag>({}));
    testSetEqual(strecke->streckenelemente.at(2)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::Tunnel}));
    testSetEqual(strecke->streckenelemente.at(3)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::KeineGleisfunktion}));
    testSetEqual(strecke->streckenelemente.at(4)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::Weichenbausatz}));
    testSetEqual(strecke->streckenelemente.at(5)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::KeineSchulterRechts}));
    testSetEqual(strecke->streckenelemente.at(6)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::KeineSchulterLinks}));
    testSetEqual(strecke->streckenelemente.at(7)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::Tunnel,
            StreckenelementFlag::Weichenbausatz, StreckenelementFlag::KeineSchulterLinks}));
}
