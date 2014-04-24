#define BOOST_TEST_MODULE StrLeseTest 
#include <cmath>
#include <fstream>
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <io/str_leser.hpp>

using namespace std;

// Wert für Floating-Point-Vergleiche (in Prozent der zu vergleichenden Werte!)
#define epsilon 0.00001

BOOST_AUTO_TEST_CASE(leere_str_datei) {
    ifstream infile("./eingabe/zusi2/LeereStrecke.str");
    unique_ptr<Strecke> strecke = StrLeser::liesStrDatei(infile);
    BOOST_CHECK(strecke.get() != NULL);

    // Dateiinformationen.
    BOOST_CHECK(strecke->dateiInfo.get() != NULL);

    BOOST_CHECK_EQUAL(strecke->dateiInfo->formatVersion, "2.3");
    BOOST_CHECK_EQUAL(strecke->dateiInfo->formatMinVersion, "2.3");

    // Dateibeschreibung.
    BOOST_CHECK_EQUAL(
      strecke->dateiInfo->beschreibung, "Freier Text, 1. Zeile\nFreier Text, 2. Zeile");

    // Dateiautor.
    BOOST_CHECK_EQUAL(strecke->dateiInfo->autorInfo.size(), 1);
    BOOST_CHECK(strecke->dateiInfo->autorInfo.at(0).get() != NULL);
    BOOST_CHECK_EQUAL(strecke->dateiInfo->autorInfo.at(0)->name, "TestAutor");

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
    unique_ptr<Strecke> strecke = StrLeser::liesStrDatei(infile);

    BOOST_CHECK_EQUAL(strecke->signalHaltabstand, 0);
}

BOOST_AUTO_TEST_CASE(leere_str_datei_blickpunkte) {
    ifstream infile("./eingabe/zusi2/LeereStrecke_Blickpunkte.str");
    unique_ptr<Strecke> strecke = StrLeser::liesStrDatei(infile);

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
