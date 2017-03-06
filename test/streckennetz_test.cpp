#define BOOST_TEST_MODULE StreckennetzTest 
#include <cstdlib>
#include <fstream>
#include <boost/test/included/unit_test.hpp>

// #include <model/streckenelement.hpp>
#include <model/streckennetz.hpp>
#include <io/st3_leser.hpp>

BOOST_AUTO_TEST_CASE(lade_strecken) {
    std::unique_ptr<Streckennetz> streckennetz = std::make_unique<Streckennetz>();

    // Zusi-3-Datenpfad muss ./eingabe/zusi3 sein
    std::ifstream infile1("./eingabe/zusi3/Routes/ModulTest1.st3");
    std::unique_ptr<Strecke> str1 = St3Leser().liesDatei(infile1);
    std::ifstream infile2("./eingabe/zusi3/Routes/ModulTest2.st3");
    std::unique_ptr<Strecke> str2 = St3Leser().liesDatei(infile2);

    // nur laden, nicht verknuepfen
    streckennetz->fuegeStreckeHinzu(str1);
    streckennetz->fuegeStreckeHinzu(str2);

    auto& modul1_element1 = streckennetz->strecken()[0]->streckenelemente[1];
    auto& modul1_element2 = streckennetz->strecken()[0]->streckenelemente[2];
    auto& modul2_element3 = streckennetz->strecken()[1]->streckenelemente[3];

    BOOST_CHECK_EQUAL(modul1_element1->normrichtung().anzahlNachfolger(), 2);
    BOOST_CHECK(modul1_element1->normrichtung().nachfolger(0).streckenelement == nullptr);
    BOOST_CHECK_EQUAL(modul1_element1->normrichtung().nachfolger(1).streckenelement, &*modul1_element2);
    BOOST_CHECK_EQUAL(modul1_element1->normrichtung().nachfolger(1).richtung, static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));

    BOOST_CHECK_EQUAL(modul1_element2->gegenrichtung().anzahlNachfolger(), 1);
    BOOST_CHECK_EQUAL(modul1_element2->gegenrichtung().nachfolger(0).streckenelement, &*modul1_element1);
    BOOST_CHECK_EQUAL(modul1_element2->gegenrichtung().nachfolger(0).richtung, static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));

    BOOST_CHECK_EQUAL(modul2_element3->gegenrichtung().anzahlNachfolger(), 1);
    BOOST_CHECK(modul2_element3->gegenrichtung().nachfolger(0).streckenelement == nullptr);

    streckennetz->verknuepfeStrecken();

}
