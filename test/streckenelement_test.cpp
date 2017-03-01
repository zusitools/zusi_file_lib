#define BOOST_TEST_MODULE StreckenelementTest
#include <cstdlib>
#include <fstream>
#include <boost/test/included/unit_test.hpp>

#include <model/streckenelement.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE(setze_nachfolger) {
    // ==1==>==2==>
    unique_ptr<Streckenelement> element1(new Streckenelement()),
        element2(new Streckenelement());

    element1->setzeNachfolger(0, Streckenelement::RICHTUNG_NORM, *element2, Streckenelement::RICHTUNG_NORM);
    BOOST_CHECK_EQUAL(element1->nachfolgerElemente[Streckenelement::RICHTUNG_NORM].size(), 1);
    BOOST_CHECK_EQUAL(element1->nachfolgerElemente[Streckenelement::RICHTUNG_GEGEN].size(), 0);

    StreckenelementUndRichtung e1nachnorm = element1->normrichtung().nachfolger(0);
    BOOST_CHECK_EQUAL(&*e1nachnorm, element2.get());
    BOOST_CHECK_EQUAL(e1nachnorm.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
    StreckenelementUndRichtung e1vorgegen = element1->gegenrichtung().vorgaenger(0);
    BOOST_CHECK_EQUAL(&*e1vorgegen, element2.get());
    BOOST_CHECK_EQUAL(e1vorgegen.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));

    element2->setzeNachfolger(0, Streckenelement::RICHTUNG_GEGEN, *element1, Streckenelement::RICHTUNG_GEGEN);
    BOOST_CHECK_EQUAL(element2->nachfolgerElemente[Streckenelement::RICHTUNG_NORM].size(), 0);
    BOOST_CHECK_EQUAL(element2->nachfolgerElemente[Streckenelement::RICHTUNG_GEGEN].size(), 1);

    StreckenelementUndRichtung e2nachgegen = element2->gegenrichtung().nachfolger(0);
    BOOST_CHECK_EQUAL(&*e2nachgegen, element1.get());
    BOOST_CHECK_EQUAL(e2nachgegen.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));
    StreckenelementUndRichtung e2vornorm = element2->normrichtung().vorgaenger(0);
    BOOST_CHECK_EQUAL(&*e2vornorm, element1.get());
    BOOST_CHECK_EQUAL(e2vornorm.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
}

BOOST_AUTO_TEST_CASE(setze_vorgaenger) {
    // ==1==>==2==>
    unique_ptr<Streckenelement> element1(new Streckenelement()),
        element2(new Streckenelement());

    element2->setzeVorgaenger(0, Streckenelement::RICHTUNG_NORM, *element1, Streckenelement::RICHTUNG_NORM);
    BOOST_CHECK_EQUAL(element2->nachfolgerElemente[Streckenelement::RICHTUNG_GEGEN].size(), 1);
    BOOST_CHECK_EQUAL(element2->nachfolgerElemente[Streckenelement::RICHTUNG_NORM].size(), 0);

    StreckenelementUndRichtung e2vornorm = element2->normrichtung().vorgaenger(0);
    BOOST_CHECK_EQUAL(&*e2vornorm, element1.get());
    BOOST_CHECK_EQUAL(e2vornorm.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
    StreckenelementUndRichtung e2nachgegen = element2->gegenrichtung().nachfolger(0);
    BOOST_CHECK_EQUAL(&*e2nachgegen, element1.get());
    BOOST_CHECK_EQUAL(e2nachgegen.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));

    element1->setzeVorgaenger(0, Streckenelement::RICHTUNG_GEGEN, *element2, Streckenelement::RICHTUNG_GEGEN);
    BOOST_CHECK_EQUAL(element1->nachfolgerElemente[Streckenelement::RICHTUNG_NORM].size(), 1);
    BOOST_CHECK_EQUAL(element1->nachfolgerElemente[Streckenelement::RICHTUNG_GEGEN].size(), 0);

    StreckenelementUndRichtung e1vorgegen = element1->gegenrichtung().vorgaenger(0);
    BOOST_CHECK_EQUAL(&*e1vorgegen, element2.get());
    BOOST_CHECK_EQUAL(e1vorgegen.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));
    StreckenelementUndRichtung e1nachnorm = element1->normrichtung().nachfolger(0);
    BOOST_CHECK_EQUAL(&*e1nachnorm, element2.get());
    BOOST_CHECK_EQUAL(e1nachnorm.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
}

BOOST_AUTO_TEST_CASE(richtungen) {
    unique_ptr<Streckenelement> element(new Streckenelement());
    auto elementRichtungen = element->richtungen();
    BOOST_CHECK_EQUAL(elementRichtungen[Streckenelement::RICHTUNG_NORM].streckenelement, element.get());
    BOOST_CHECK_EQUAL(elementRichtungen[Streckenelement::RICHTUNG_GEGEN].streckenelement, element.get());
    BOOST_CHECK_EQUAL(elementRichtungen[Streckenelement::RICHTUNG_NORM].richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
    BOOST_CHECK_EQUAL(elementRichtungen[Streckenelement::RICHTUNG_GEGEN].richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));
}

BOOST_AUTO_TEST_CASE(gegenrichtung) {
    unique_ptr<Streckenelement> element(new Streckenelement());
    StreckenelementUndRichtung elementRichtung1 { element.get(), Streckenelement::RICHTUNG_NORM },
                               elementRichtung2 { element.get(), Streckenelement::RICHTUNG_GEGEN };

    BOOST_CHECK_EQUAL(&*elementRichtung1.gegenrichtung(), element.get());
    BOOST_CHECK_EQUAL(elementRichtung1.gegenrichtung().richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));

    BOOST_CHECK_EQUAL(&*elementRichtung2.gegenrichtung(), element.get());
    BOOST_CHECK_EQUAL(elementRichtung2.gegenrichtung().richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
}

BOOST_AUTO_TEST_CASE(endpunkt) {
    unique_ptr<Streckenelement> element(new Streckenelement());
    element->p1 = { -1.0f, 2.0f, 0.0f };
    element->p2 = { 100.0f, -200.0f, 0.0f };

    StreckenelementUndRichtung elementRichtung1 { element.get(), Streckenelement::RICHTUNG_NORM },
                               elementRichtung2 { element.get(), Streckenelement::RICHTUNG_GEGEN };

    BOOST_CHECK_EQUAL(&elementRichtung1.endpunkt(), &element->p2);
    BOOST_CHECK_EQUAL(&elementRichtung2.endpunkt(), &element->p1);
}
