#define BOOST_TEST_MODULE StrLeseTest 
#include <fstream>
#include <boost/test/included/unit_test.hpp>

#include <io/str_leser.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE(leere_str_datei)
{
    ifstream infile("./eingabe/zusi2/LeereStrecke.str");
    unique_ptr<Strecke> strecke = StrLeser::liesStrDatei(infile);
    BOOST_CHECK(strecke.get() != NULL);

    // Dateiinformationen.
    BOOST_CHECK(strecke->dateiInfo.get() != NULL);

    // Dateibeschreibung.
    BOOST_CHECK_EQUAL(
      strecke->dateiInfo->beschreibung, "Freier Text, 1. Zeile\nFreier Text, 2. Zeile");

    // Dateiautor.
    BOOST_CHECK_EQUAL(strecke->dateiInfo->autorInfo.size(), 1);
    BOOST_CHECK(strecke->dateiInfo->autorInfo.at(0).get() != NULL);
    BOOST_CHECK_EQUAL(strecke->dateiInfo->autorInfo.at(0)->name, "TestAutor");

    // Gebietsschema
    BOOST_CHECK_EQUAL(strecke->gebietsschema, "DBAGWest2000");

    // Breitengrad 42
    // RekTiefe 15
    // Signalhaltabstand 23

    // Streckenelemente.
    BOOST_CHECK_EQUAL(strecke->streckenelemente.size(), 0);
}
