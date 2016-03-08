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

    // Direkter Zugriff ueber set
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

    // Zugriff ueber hatFktFlag-Methode
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(1)->hatFktFlag(StreckenelementFlag::Tunnel), false);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(1)->hatFktFlag(StreckenelementFlag::KeineGleisfunktion), false);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(1)->hatFktFlag(StreckenelementFlag::Weichenbausatz), false);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(1)->hatFktFlag(StreckenelementFlag::KeineSchulterRechts), false);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(1)->hatFktFlag(StreckenelementFlag::KeineSchulterLinks), false);

    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(2)->hatFktFlag(StreckenelementFlag::Tunnel), true);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(3)->hatFktFlag(StreckenelementFlag::KeineGleisfunktion), true);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(4)->hatFktFlag(StreckenelementFlag::Weichenbausatz), true);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(5)->hatFktFlag(StreckenelementFlag::KeineSchulterRechts), true);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(6)->hatFktFlag(StreckenelementFlag::KeineSchulterLinks), true);

    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(7)->hatFktFlag(StreckenelementFlag::Tunnel), true);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(7)->hatFktFlag(StreckenelementFlag::Weichenbausatz), true);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(7)->hatFktFlag(StreckenelementFlag::KeineSchulterLinks), true);
}

BOOST_AUTO_TEST_CASE(signale) {
    ifstream infile("./eingabe/zusi3/SignalTest.st3");
    unique_ptr<Strecke> strecke = St3Leser().liesSt3Datei(infile);

    BOOST_REQUIRE_EQUAL(strecke->streckenelemente.size(), 5);

    auto& signal2 = strecke->streckenelemente.at(2)->richtungsInfo[Streckenelement::RICHTUNG_NORM].signal;
    BOOST_CHECK(signal2);
    BOOST_CHECK_EQUAL(signal2->betriebsstelle, "Anfang");
    BOOST_CHECK_EQUAL(signal2->stellwerk, "Anfang Af");
    BOOST_CHECK_EQUAL(signal2->signalbezeichnung, "a");
    BOOST_CHECK_EQUAL(signal2->signaltyp, SignalTyp::Vorsignal);

    auto& signal3norm = strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_NORM].signal;
    BOOST_CHECK(signal3norm);
    BOOST_CHECK_EQUAL(signal3norm->betriebsstelle, "Anfang");
    BOOST_CHECK_EQUAL(signal3norm->stellwerk, "Anfang Af");
    BOOST_CHECK_EQUAL(signal3norm->signalbezeichnung, "A");
    BOOST_CHECK_EQUAL(signal3norm->signaltyp, SignalTyp::Einfahrsignal);

    auto& signal3gegen = strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_GEGEN].signal;
    BOOST_CHECK(signal3gegen);
    BOOST_CHECK_EQUAL(signal3gegen->betriebsstelle, "Aheim");
    BOOST_CHECK_EQUAL(signal3gegen->stellwerk, "Aheim Af");
    BOOST_CHECK_EQUAL(signal3gegen->signalbezeichnung, "H-Tafel");
    BOOST_CHECK_EQUAL(signal3gegen->signaltyp, SignalTyp::Tafel);
}
