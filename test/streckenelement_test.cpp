#define BOOST_TEST_MODULE StreckenelementTest
#include <cstdlib>
#include <fstream>
#include <boost/test/included/unit_test.hpp>

#include <model/streckenelement.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE(setze_nachfolger) {
    // ==1==>==2==>
    shared_ptr<Streckenelement> element1(new Streckenelement()),
        element2(new Streckenelement());

    StreckenelementUndRichtung e2norm;
    e2norm.streckenelement = weak_ptr<Streckenelement>(element2);
    e2norm.richtung = Streckenelement::RICHTUNG_NORM;

    element1->setzeNachfolger(0, Streckenelement::RICHTUNG_NORM, e2norm);
    BOOST_CHECK_EQUAL(element1->nachfolgerElemente[Streckenelement::RICHTUNG_NORM].size(), 1);
    BOOST_CHECK_EQUAL(element1->nachfolgerElemente[Streckenelement::RICHTUNG_GEGEN].size(), 0);

    StreckenelementUndRichtung e1nachnorm = element1->nachfolger(0, Streckenelement::RICHTUNG_NORM);
    BOOST_CHECK(e1nachnorm.streckenelement.lock() == element2);
    BOOST_CHECK_EQUAL(e1nachnorm.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
    StreckenelementUndRichtung e1vorgegen = element1->vorgaenger(0, Streckenelement::RICHTUNG_GEGEN);
    BOOST_CHECK(e1vorgegen.streckenelement.lock() == element2);
    BOOST_CHECK_EQUAL(e1vorgegen.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));

    StreckenelementUndRichtung e1gegen;
    e1gegen.streckenelement = weak_ptr<Streckenelement>(element1);
    e1gegen.richtung = Streckenelement::RICHTUNG_GEGEN;

    element2->setzeNachfolger(0, Streckenelement::RICHTUNG_GEGEN, e1gegen);
    BOOST_CHECK_EQUAL(element2->nachfolgerElemente[Streckenelement::RICHTUNG_NORM].size(), 0);
    BOOST_CHECK_EQUAL(element2->nachfolgerElemente[Streckenelement::RICHTUNG_GEGEN].size(), 1);

    StreckenelementUndRichtung e2nachgegen = element2->nachfolger(0, Streckenelement::RICHTUNG_GEGEN);
    BOOST_CHECK(e2nachgegen.streckenelement.lock() == element1);
    BOOST_CHECK_EQUAL(e2nachgegen.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));
    StreckenelementUndRichtung e2vornorm = element2->vorgaenger(0, Streckenelement::RICHTUNG_NORM);
    BOOST_CHECK(e2vornorm.streckenelement.lock() == element1);
    BOOST_CHECK_EQUAL(e2vornorm.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
}

BOOST_AUTO_TEST_CASE(setze_vorgaenger) {
    // ==1==>==2==>
    shared_ptr<Streckenelement> element1(new Streckenelement()),
        element2(new Streckenelement());
    element1->nr = 1;
    element2->nr = 2;

    StreckenelementUndRichtung e1norm;
    e1norm.streckenelement = weak_ptr<Streckenelement>(element1);
    e1norm.richtung = Streckenelement::RICHTUNG_NORM;

    element2->setzeVorgaenger(0, Streckenelement::RICHTUNG_NORM, e1norm);
    BOOST_CHECK_EQUAL(element2->nachfolgerElemente[Streckenelement::RICHTUNG_GEGEN].size(), 1);
    BOOST_CHECK_EQUAL(element2->nachfolgerElemente[Streckenelement::RICHTUNG_NORM].size(), 0);

    StreckenelementUndRichtung e2vornorm = element2->vorgaenger(0, Streckenelement::RICHTUNG_NORM);
    BOOST_CHECK(e2vornorm.streckenelement.lock() == element1);
    BOOST_CHECK_EQUAL(e2vornorm.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
    StreckenelementUndRichtung e2nachgegen = element2->nachfolger(0, Streckenelement::RICHTUNG_GEGEN);
    BOOST_CHECK(e2nachgegen.streckenelement.lock() == element1);
    BOOST_CHECK_EQUAL(e2nachgegen.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));

    StreckenelementUndRichtung e2gegen;
    e2gegen.streckenelement = weak_ptr<Streckenelement>(element2);
    e2gegen.richtung = Streckenelement::RICHTUNG_GEGEN;

    element1->setzeVorgaenger(0, Streckenelement::RICHTUNG_GEGEN, e2gegen);
    BOOST_CHECK_EQUAL(element1->nachfolgerElemente[Streckenelement::RICHTUNG_NORM].size(), 1);
    BOOST_CHECK_EQUAL(element1->nachfolgerElemente[Streckenelement::RICHTUNG_GEGEN].size(), 0);

    StreckenelementUndRichtung e1vorgegen = element1->vorgaenger(0, Streckenelement::RICHTUNG_GEGEN);
    BOOST_CHECK(e1vorgegen.streckenelement.lock() == element2);
    BOOST_CHECK_EQUAL(e1vorgegen.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_GEGEN));
    StreckenelementUndRichtung e1nachnorm = element1->nachfolger(0, Streckenelement::RICHTUNG_NORM);
    BOOST_CHECK(e1nachnorm.streckenelement.lock() == element2);
    BOOST_CHECK_EQUAL(e1nachnorm.richtung,
            static_cast<streckenelement_richtung_t>(Streckenelement::RICHTUNG_NORM));
}

BOOST_AUTO_TEST_CASE(gegenrichtung) {
    shared_ptr<Streckenelement> element(new Streckenelement());
    StreckenelementUndRichtung elementRichtung1(element, Streckenelement::RICHTUNG_NORM),
                               elementRichtung2(element, Streckenelement::RICHTUNG_GEGEN);

    BOOST_CHECK_EQUAL(elementRichtung1.gegenrichtung().streckenelement.lock(), element);
    BOOST_CHECK_EQUAL(elementRichtung1.gegenrichtung().richtung, Streckenelement::RICHTUNG_GEGEN);

    BOOST_CHECK_EQUAL(elementRichtung2.gegenrichtung().streckenelement.lock(), element);
    BOOST_CHECK_EQUAL(elementRichtung2.gegenrichtung().richtung, Streckenelement::RICHTUNG_NORM);
}
