#define BOOST_TEST_MODULE StrLeseTest 
#include <cmath>
#include <fstream>
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <iostream>

#include <io/str_leser.hpp>

#include "testhelpers.hpp"
using testhelpers::epsilon;

using namespace std;

BOOST_AUTO_TEST_CASE(leere_str_datei) {
    ifstream infile("./eingabe/zusi2/LeereStrecke.str");
    unique_ptr<Strecke> strecke = StrLeser().liesDatei(infile);
    BOOST_CHECK(strecke);

    // Dateiinformationen.
    BOOST_CHECK_EQUAL(strecke->formatVersion, "2.3");
    BOOST_CHECK_EQUAL(strecke->formatMinVersion, "2.3");

    // Dateibeschreibung.
    BOOST_CHECK_EQUAL(
      strecke->beschreibung, "Freier Text, 1. Zeile\nFreier Text, 2. Zeile");

    // Dateiautor.
    BOOST_CHECK_EQUAL(strecke->autorInfo.size(), 1);
    BOOST_CHECK(strecke->autorInfo.at(0));
    BOOST_CHECK_EQUAL(strecke->autorInfo.at(0)->name, "TestAutor");

    // Gebietsschema.
    BOOST_CHECK_EQUAL(strecke->gebietsschema, "DBAGWest2000");

    // Breitengrad.
    BOOST_CHECK_EQUAL(strecke->breitengrad, 42);

    // Rekursionstiefe.
    BOOST_CHECK_EQUAL(strecke->rekursionstiefe, 15);

    // Signalhaltabstand.
    BOOST_CHECK_EQUAL(strecke->signalHaltabstand, 23);

    // Referenzpunkte.
    BOOST_CHECK_EQUAL(strecke->referenzpunkte.size(), 0);

    // Streckenelemente.
    BOOST_CHECK_EQUAL(strecke->streckenelemente.size(), 0);
}

BOOST_AUTO_TEST_CASE(leere_str_datei_kein_haltabstand) {
    ifstream infile("./eingabe/zusi2/LeereStrecke_KeinHaltabstand.str");
    unique_ptr<Strecke> strecke = StrLeser().liesDatei(infile);

    BOOST_CHECK_EQUAL(strecke->signalHaltabstand, 0);
}

BOOST_AUTO_TEST_CASE(leere_str_datei_blickpunkte) {
    ifstream infile("./eingabe/zusi2/LeereStrecke_Blickpunkte.str");
    unique_ptr<Strecke> strecke = StrLeser().liesDatei(infile);

    BOOST_CHECK_EQUAL(strecke->blickpunkte.size(), 2);

    auto &blickpunkt1 = strecke->blickpunkte.at(0);
    auto &blickpunkt2 = strecke->blickpunkte.at(1);

    BOOST_CHECK_CLOSE(blickpunkt1->position.loc.x, 10, epsilon);
    BOOST_CHECK_CLOSE(blickpunkt1->position.loc.y, 20, epsilon);
    BOOST_CHECK_CLOSE(blickpunkt1->position.loc.z, 30.3, epsilon);

    BOOST_CHECK_CLOSE(blickpunkt1->position.richtungAlsRot().x, 1.571, epsilon);
    BOOST_CHECK_CLOSE(blickpunkt1->position.richtungAlsRot().y, 0, epsilon);
    BOOST_CHECK_CLOSE(blickpunkt1->position.richtungAlsRot().z, 0, epsilon);

    BOOST_CHECK_EQUAL(blickpunkt1->name, "Standort 1");

    BOOST_CHECK_CLOSE(blickpunkt2->position.loc.x, 2, epsilon);
    BOOST_CHECK_CLOSE(blickpunkt2->position.loc.y, -3, epsilon);
    BOOST_CHECK_CLOSE(blickpunkt2->position.loc.z, 0, epsilon);

    BOOST_CHECK_CLOSE(blickpunkt2->position.richtungAlsRot().x, 0, epsilon);
    BOOST_CHECK_CLOSE(blickpunkt2->position.richtungAlsRot().y, 1.571, epsilon);
    BOOST_CHECK_CLOSE(blickpunkt2->position.richtungAlsRot().z, -1.571, epsilon);

    BOOST_CHECK_EQUAL(blickpunkt2->name, "#");
}

BOOST_AUTO_TEST_CASE(nachfolger_vorgaenger) {
    ifstream infile("./eingabe/zusi2/NachfolgerVorgaengerTest.str");
    unique_ptr<Strecke> strecke = StrLeser().liesDatei(infile);

    BOOST_REQUIRE_EQUAL(strecke->streckenelemente.size(), 6);

    BOOST_CHECK(!strecke->streckenelemente.at(0));
    testVorgaengerNachfolger(strecke, 1, {}, {}, {2, 3}, {Streckenelement::RICHTUNG_NORM, Streckenelement::RICHTUNG_NORM});
    testVorgaengerNachfolger(strecke, 2, {1}, {Streckenelement::RICHTUNG_NORM}, {5}, {Streckenelement::RICHTUNG_NORM});
    testVorgaengerNachfolger(strecke, 3, {1}, {Streckenelement::RICHTUNG_NORM}, {}, {});
    testVorgaengerNachfolger(strecke, 4, {}, {}, {5}, {Streckenelement::RICHTUNG_NORM});
    testVorgaengerNachfolger(strecke, 5, {2, 4}, {Streckenelement::RICHTUNG_NORM, Streckenelement::RICHTUNG_NORM}, {}, {});
}

BOOST_AUTO_TEST_CASE(aufgleispunkte) {
    ifstream infile("./eingabe/zusi2/AufgleispunkteTest.str");
    unique_ptr<Strecke> strecke = StrLeser().liesDatei(infile);

    BOOST_REQUIRE_EQUAL(strecke->referenzpunkte.size(), 3);

    auto& referenzpunkt1 = strecke->referenzpunkte.at(1);
    auto& referenzpunkt2 = strecke->referenzpunkte.at(2);

    BOOST_CHECK_EQUAL(referenzpunkt1->referenzNr, 1);
    BOOST_CHECK_EQUAL(referenzpunkt1->referenzTyp, Referenzpunkt::Typ::Aufgleispunkt);
    BOOST_CHECK_EQUAL(referenzpunkt1->elementRichtung.streckenelement, strecke->streckenelemente.at(4).get());
    BOOST_CHECK_EQUAL(referenzpunkt1->elementRichtung.richtung,
        static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
    BOOST_CHECK_EQUAL(referenzpunkt1->streckenelementNr, 4);
    BOOST_CHECK_EQUAL(referenzpunkt1->beschreibung, "#");
    
    BOOST_CHECK_EQUAL(referenzpunkt2->referenzNr, 2);
    BOOST_CHECK_EQUAL(referenzpunkt2->referenzTyp, Referenzpunkt::Typ::Aufgleispunkt);
    BOOST_CHECK_EQUAL(referenzpunkt2->elementRichtung.streckenelement, strecke->streckenelemente.at(2).get());
    BOOST_CHECK_EQUAL(referenzpunkt2->elementRichtung.richtung,
        static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
    BOOST_CHECK_EQUAL(referenzpunkt2->streckenelementNr, 2);
    BOOST_CHECK_EQUAL(referenzpunkt2->beschreibung, "Aufgleispunkt 1");
}

BOOST_AUTO_TEST_CASE(element_koordinaten) {
    ifstream infile("./eingabe/zusi2/ElementKoordinatenTest.str");
    unique_ptr<Strecke> strecke = StrLeser().liesDatei(infile);

    BOOST_REQUIRE_EQUAL(strecke->streckenelemente.size(), 2);

    auto& element = strecke->streckenelemente.at(1);
    BOOST_CHECK_CLOSE(element->p1.x, -10, epsilon);
    BOOST_CHECK_CLOSE(element->p1.y, -23.5, epsilon);
    BOOST_CHECK_CLOSE(element->p1.z, 5, epsilon);
    BOOST_CHECK_CLOSE(element->p2.x, 10, epsilon);
    BOOST_CHECK_CLOSE(element->p2.y, 20, epsilon);
    BOOST_CHECK_CLOSE(element->p2.z, -30, epsilon);
}

BOOST_AUTO_TEST_CASE(element_flags) {
    ifstream infile("./eingabe/zusi2/ElementFlagsTest.str");
    unique_ptr<Strecke> strecke = StrLeser().liesDatei(infile);

    BOOST_REQUIRE_EQUAL(strecke->streckenelemente.size(), 11);

    // Flag "Elektrifiziert" wird nicht in den Element-Flags gespeichert.
    testSetEqual(strecke->streckenelemente.at(1)->flags,
        set<StreckenelementFlag>({}));
    testSetEqual(strecke->streckenelemente.at(2)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::Tunnel}));
    testSetEqual(strecke->streckenelemente.at(3)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::Weichenbausatz}));
    testSetEqual(strecke->streckenelemente.at(4)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::Stahlbruecke}));
    testSetEqual(strecke->streckenelemente.at(5)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::Steinbruecke}));
    testSetEqual(strecke->streckenelemente.at(6)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::KeineGleisfunktion}));
    testSetEqual(strecke->streckenelemente.at(7)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::KeinSchienenbau}));
    testSetEqual(strecke->streckenelemente.at(8)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::KeinWeichenbau}));
    testSetEqual(strecke->streckenelemente.at(9)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::Tunnel, StreckenelementFlag::Weichenbausatz,
            StreckenelementFlag::Steinbruecke, StreckenelementFlag::Stahlbruecke,
            StreckenelementFlag::KeineGleisfunktion, StreckenelementFlag::KeinSchienenbau,
            StreckenelementFlag::KeinWeichenbau}));
    testSetEqual(strecke->streckenelemente.at(10)->flags,
        set<StreckenelementFlag>({StreckenelementFlag::Weichenbausatz,
            StreckenelementFlag::Steinbruecke, StreckenelementFlag::KeinSchienenbau}));
}

BOOST_AUTO_TEST_CASE(ereignisse) {
    ifstream infile("./eingabe/zusi2/EreignisTest.str");
    unique_ptr<Strecke> strecke = StrLeser().liesDatei(infile);

    BOOST_REQUIRE(strecke->streckenelemente.size() > 15);

    BOOST_CHECK_EQUAL(strecke->streckenelemente.at(1)->
        richtungsInfo[Streckenelement::RICHTUNG_NORM].ereignisse.size(), 0);
    testEreignis(strecke, 2, EreignisTyp::Indusi500Hz);
    testEreignis(strecke, 3, EreignisTyp::Indusi1000Hz);
    testEreignis(strecke, 4, EreignisTyp::Indusi2000Hz);
    testEreignis(strecke, 5, EreignisTyp::Indusi1000Hz, 125.0/3.6);
    testEreignis(strecke, 6, EreignisTyp::Indusi2000Hz, 499.0/3.6);
    testEreignis(strecke, 7, EreignisTyp::VorherKeineFahrstrEntfernung, 1000);
    testEreignis(strecke, 8, EreignisTyp::VorherKeineFahrstrEntfernung, 2000);
    testEreignis(strecke, 9, EreignisTyp::VorherKeineFahrstrEntfernung, 3000);
    testEreignis(strecke, 10, EreignisTyp::OhneFunktion);
    testEreignis(strecke, 11, EreignisTyp::Bahnsteigmitte);
    testEreignis(strecke, 12, EreignisTyp::Bahnsteigende);
    testEreignis(strecke, 13, EreignisTyp::Betriebsstelle);
    testEreignis(strecke, 14, EreignisTyp::RegisterNichtBelegen);
    testEreignis(strecke, 15, EreignisTyp::GntGeschwindigkeitsErhoehung, 500.0/3.6);
}

BOOST_AUTO_TEST_CASE(signalnamen) {
    ifstream infile("./eingabe/zusi2/SignalTest.str");
    unique_ptr<Strecke> strecke = StrLeser().liesDatei(infile);

    BOOST_REQUIRE(strecke->streckenelemente.size() > 6);

    auto& element1 = strecke->streckenelemente.at(1)->richtungsInfo[Streckenelement::RICHTUNG_NORM];
    BOOST_CHECK(!element1.signal);

    auto& element2 = strecke->streckenelemente.at(2)->richtungsInfo[Streckenelement::RICHTUNG_NORM];
    BOOST_REQUIRE(element2.signal);
    BOOST_CHECK_EQUAL(element2.signal->betriebsstelle, "Block A");
    BOOST_CHECK_EQUAL(element2.signal->signalbezeichnung, "0");

    auto& element4 = strecke->streckenelemente.at(4)->richtungsInfo[Streckenelement::RICHTUNG_NORM];
    BOOST_REQUIRE(element4.signal);
    BOOST_CHECK_EQUAL(element4.signal->betriebsstelle, "Block B|");
    BOOST_CHECK_EQUAL(element4.signal->signalbezeichnung, "1");

    auto& element6 = strecke->streckenelemente.at(6)->richtungsInfo[Streckenelement::RICHTUNG_NORM];
    BOOST_REQUIRE(element6.signal);
    BOOST_CHECK_EQUAL(element6.signal->betriebsstelle, "Block C...E50||");
    BOOST_CHECK_EQUAL(element6.signal->signalbezeichnung, "Foobar");
}


BOOST_AUTO_TEST_CASE(fahrstr_register) {
    ifstream infile("./eingabe/zusi2/RegisterTest.str");
    unique_ptr<Strecke> strecke = StrLeser().liesDatei(infile);

    BOOST_REQUIRE(strecke->streckenelemente.size() > 4);
    BOOST_REQUIRE(strecke->fahrstrRegister.size() == 2);

    BOOST_REQUIRE(strecke->fahrstrRegister[42]);
    BOOST_CHECK_EQUAL(strecke->fahrstrRegister[42]->registerNr, 42);
    BOOST_CHECK_EQUAL(strecke->fahrstrRegister[42]->manuell, true);

    BOOST_REQUIRE(strecke->fahrstrRegister[1024]);
    BOOST_CHECK_EQUAL(strecke->fahrstrRegister[1024]->registerNr, 1024);
    BOOST_CHECK_EQUAL(strecke->fahrstrRegister[1024]->manuell, false);

    auto& element1 = strecke->streckenelemente.at(1)->richtungsInfo[Streckenelement::RICHTUNG_NORM];
    BOOST_CHECK(!element1.fahrstrRegister);
    auto& element2 = strecke->streckenelemente.at(2)->richtungsInfo[Streckenelement::RICHTUNG_NORM];
    BOOST_CHECK_EQUAL(element2.fahrstrRegister, strecke->fahrstrRegister[42].get());
    auto& element3 = strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_NORM];
    BOOST_CHECK_EQUAL(element3.fahrstrRegister, strecke->fahrstrRegister[1024].get());
    auto& element4 = strecke->streckenelemente.at(4)->richtungsInfo[Streckenelement::RICHTUNG_NORM];
    BOOST_CHECK_EQUAL(element4.fahrstrRegister, strecke->fahrstrRegister[42].get());
}

BOOST_AUTO_TEST_CASE(vmax) {
    ifstream infile("./eingabe/zusi2/VmaxTest.str");
    unique_ptr<Strecke> strecke = StrLeser().liesDatei(infile);

    BOOST_REQUIRE(strecke->streckenelemente.size() > 3);
    BOOST_CHECK_CLOSE(strecke->streckenelemente.at(1)->richtungsInfo[Streckenelement::RICHTUNG_NORM].vmax, 100.0 / 3.6, epsilon);
    BOOST_CHECK_CLOSE(strecke->streckenelemente.at(2)->richtungsInfo[Streckenelement::RICHTUNG_NORM].vmax, 0.0 / 3.6, epsilon);
    BOOST_CHECK_CLOSE(strecke->streckenelemente.at(3)->richtungsInfo[Streckenelement::RICHTUNG_NORM].vmax, -1.0 / 3.6, epsilon);
}
