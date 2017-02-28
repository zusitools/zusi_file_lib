#define BOOST_TEST_MODULE St3LeseTest
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "io/st3_leser.hpp"
#include "model/streckenelement.hpp"

using namespace std;

// Wert für Floating-Point-Vergleiche (in Prozent der zu vergleichenden Werte!)
#define epsilon 0.00001

#include "testhelpers.hpp"

BOOST_AUTO_TEST_CASE(leere_st3_datei) {
    ifstream infile("./eingabe/zusi3/LeereSt3Datei.st3");
    unique_ptr<Strecke> strecke = St3Leser().liesDatei(infile);
    BOOST_CHECK(strecke);

    BOOST_CHECK_EQUAL(strecke->formatVersion, "A.1");
    BOOST_CHECK_EQUAL(strecke->formatMinVersion, "A.1");
}

BOOST_AUTO_TEST_CASE(element_koordinaten) {
    ifstream infile("./eingabe/zusi3/ElementKoordinatenTest.st3");
    unique_ptr<Strecke> strecke = St3Leser().liesDatei(infile);

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
    unique_ptr<Strecke> strecke = St3Leser().liesDatei(infile);

    // Es werden die Vorgaenger verglichen, nicht die Nachfolger in Gegenrichtung
    // (wichtig fuer die Richtungsangaben).
    testVorgaengerNachfolger(strecke, 1, {4}, {Streckenelement::RICHTUNG_GEGEN}, {2}, {Streckenelement::RICHTUNG_GEGEN});
    testVorgaengerNachfolger(strecke, 2, {3}, {Streckenelement::RICHTUNG_NORM},  {1}, {Streckenelement::RICHTUNG_GEGEN});
    testVorgaengerNachfolger(strecke, 3, {},  {},                                {2}, {Streckenelement::RICHTUNG_NORM});
    testVorgaengerNachfolger(strecke, 4, {1}, {Streckenelement::RICHTUNG_GEGEN}, {},  {});
}

BOOST_AUTO_TEST_CASE(utm_punkt) {
    ifstream infile("./eingabe/zusi3/UTMPunktTest.st3");
    unique_ptr<Strecke> strecke = St3Leser().liesDatei(infile);

    BOOST_CHECK_EQUAL(strecke->utmPunkt.we, 42);
    BOOST_CHECK_EQUAL(strecke->utmPunkt.ns, -90);
}

BOOST_AUTO_TEST_CASE(element_flags) {
    ifstream infile("./eingabe/zusi3/ElementFlagsTest.st3");
    unique_ptr<Strecke> strecke = St3Leser().liesDatei(infile);

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
    unique_ptr<Strecke> strecke = St3Leser().liesDatei(infile);

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

BOOST_AUTO_TEST_CASE(ereignisse) {
    ifstream infile("./eingabe/zusi3/EreignisTest.st3");
    unique_ptr<Strecke> strecke = St3Leser().liesDatei(infile);

    BOOST_REQUIRE_EQUAL(strecke->streckenelemente.size(), 5);

    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(1)->richtungsInfo[Streckenelement::RICHTUNG_NORM].ereignisse.size(), 0);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(1)->richtungsInfo[Streckenelement::RICHTUNG_GEGEN].ereignisse.size(), 0);

    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(2)->richtungsInfo[Streckenelement::RICHTUNG_NORM].ereignisse.size(), 1);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(2)->richtungsInfo[Streckenelement::RICHTUNG_NORM].ereignisse.at(0).ereignisTyp, EreignisTyp::BahnsteiganfangRechts);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(2)->richtungsInfo[Streckenelement::RICHTUNG_NORM].ereignisse.at(0).wert, -123.45f);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(2)->richtungsInfo[Streckenelement::RICHTUNG_NORM].ereignisse.at(0).beschreibung, "Test");
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(2)->richtungsInfo[Streckenelement::RICHTUNG_GEGEN].ereignisse.size(), 0);

    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_NORM].ereignisse.size(), 2);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_NORM].ereignisse.at(0).ereignisTyp, EreignisTyp::FahrstrAufloesen);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_NORM].ereignisse.at(0).wert, 0.0f);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_NORM].ereignisse.at(0).beschreibung, "");
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_NORM].ereignisse.at(1).ereignisTyp, EreignisTyp::FahrstrAnfordern);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_NORM].ereignisse.at(1).wert, 0.0f);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_NORM].ereignisse.at(1).beschreibung, "Test 2");

    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_GEGEN].ereignisse.size(), 1);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_GEGEN].ereignisse.at(0).ereignisTyp, EreignisTyp::KeineZugFahrstrEinrichten);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_GEGEN].ereignisse.at(0).wert, 0.0f);
    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_GEGEN].ereignisse.at(0).beschreibung, "");
}

BOOST_AUTO_TEST_CASE(referenzpunkte) {
    ifstream infile("./eingabe/zusi3/ReferenzpunkteTest.st3");
    unique_ptr<Strecke> strecke = St3Leser().liesDatei(infile);

    BOOST_REQUIRE_EQUAL(strecke->streckenelemente.size(), 5+1);
    BOOST_REQUIRE_EQUAL(strecke->referenzpunkte.size(), 15+1);

    BOOST_CHECK(!strecke->referenzpunkte.at(0));

    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(1)->referenzNr, 1);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(1)->referenzTyp, Referenzpunkt::Typ::Aufgleispunkt);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(1)->streckenelementNr, 1);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(1)->richtung, static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(1)->streckenelement, strecke->streckenelemente.at(1).get());
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(1)->beschreibung, "Aufgleispunkt");

    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(2)->referenzNr, 2);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(2)->referenzTyp, Referenzpunkt::Typ::Modulgrenze);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(2)->streckenelementNr, 1);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(2)->richtung, static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(2)->streckenelement, strecke->streckenelemente.at(1).get());
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(2)->beschreibung, "Modulgrenze");

    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(3)->referenzNr, 3);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(3)->referenzTyp, Referenzpunkt::Typ::Register);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(3)->streckenelementNr, 2);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(3)->richtung, static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(3)->streckenelement, strecke->streckenelemente.at(2).get());
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(3)->beschreibung, "Nr. 456");

    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(4)->referenzNr, 4);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(4)->referenzTyp, Referenzpunkt::Typ::Weiche);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(4)->streckenelementNr, 3);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(4)->richtung, static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(4)->streckenelement, strecke->streckenelemente.at(3).get());
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(4)->beschreibung, "123");

    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(5)->referenzNr, 5);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(5)->referenzTyp, Referenzpunkt::Typ::Aufloesepunkt);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(5)->streckenelementNr, 5);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(5)->richtung, static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(5)->streckenelement, strecke->streckenelemente.at(5).get());
    BOOST_CHECK(strecke->referenzpunkte.at(5)->beschreibung.empty());

    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(6)->referenzNr, 6);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(6)->referenzTyp, Referenzpunkt::Typ::Signal);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(6)->streckenelementNr, 42);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(6)->richtung, static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
    BOOST_CHECK(strecke->referenzpunkte.at(6)->streckenelement == nullptr);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(6)->beschreibung, "Bla bla");

    BOOST_CHECK(!strecke->referenzpunkte.at(7));

    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(15)->referenzNr, 15);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(15)->referenzTyp, Referenzpunkt::Typ::Signalhaltfall);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(15)->streckenelementNr, 5);
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(15)->richtung, static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(15)->streckenelement, strecke->streckenelemente.at(5).get());
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.at(15)->beschreibung, "Signalhaltfall");
}

BOOST_AUTO_TEST_CASE(vmax) {
    ifstream infile("./eingabe/zusi3/VmaxTest.st3");
    unique_ptr<Strecke> strecke = St3Leser().liesDatei(infile);

    BOOST_REQUIRE_EQUAL(strecke->streckenelemente.size(), 4+1);

    BOOST_CHECK_CLOSE(strecke->streckenelemente.at(1)->richtungsInfo[Streckenelement::RICHTUNG_NORM].vmax, 0.0, epsilon);
    BOOST_CHECK_CLOSE(strecke->streckenelemente.at(1)->richtungsInfo[Streckenelement::RICHTUNG_GEGEN].vmax, 0.0, epsilon);

    BOOST_CHECK_CLOSE(strecke->streckenelemente.at(2)->richtungsInfo[Streckenelement::RICHTUNG_NORM].vmax, 27.7778, epsilon);
    BOOST_CHECK_CLOSE(strecke->streckenelemente.at(2)->richtungsInfo[Streckenelement::RICHTUNG_GEGEN].vmax, 0.0, epsilon);

    BOOST_CHECK_CLOSE(strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_NORM].vmax, -1.0, epsilon);
    BOOST_CHECK_CLOSE(strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_GEGEN].vmax, 27.7778, epsilon);
}
