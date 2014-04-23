#define BOOST_TEST_MODULE StrLeseTest 
#include <fstream>
#include <boost/test/included/unit_test.hpp>

#include <io/str_leser.hpp>

using namespace std;

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

    // Streckenelemente.
    BOOST_CHECK_EQUAL(strecke->streckenelemente.size(), 0);
}

BOOST_AUTO_TEST_CASE(leere_str_datei_kein_haltabstand) {
    ifstream infile("./eingabe/zusi2/LeereStrecke_KeinHaltabstand.str");
    unique_ptr<Strecke> strecke = StrLeser::liesStrDatei(infile);

    BOOST_CHECK_EQUAL(strecke->signalHaltabstand, 0);
}
