void testVorgaengerNachfolger(unique_ptr<Strecke>& strecke, streckenelement_nr_t nr,
        vector<streckenelement_nr_t> nrsVorgaenger, vector<streckenelement_richtung_t> richtungenVorgaenger,
        vector<streckenelement_nr_t> nrsNachfolger, vector<streckenelement_richtung_t> richtungenNachfolger) {
    auto& element = strecke->streckenelemente.at(nr);

    BOOST_CHECK_EQUAL(element->nr, nr);

    BOOST_REQUIRE_EQUAL(
        element->nachfolgerElemente[Streckenelement::RICHTUNG_GEGEN].size(),
        nrsVorgaenger.size());
    BOOST_REQUIRE_EQUAL(
        element->nachfolgerElemente[Streckenelement::RICHTUNG_NORM].size(),
        nrsNachfolger.size());

    BOOST_REQUIRE_EQUAL(nrsVorgaenger.size(), richtungenVorgaenger.size());
    BOOST_REQUIRE_EQUAL(nrsNachfolger.size(), richtungenNachfolger.size());

    // TODO: Bei Zusi 2 hat die Liste der Vorgänger keine definierte Ordnung.
    for (size_t i = 0; i < nrsVorgaenger.size(); i++) {
        BOOST_MESSAGE("Element " << nr << ", Vorgaenger " << i);
        auto vorgaenger = element->normrichtung().vorgaenger(i);
        BOOST_CHECK_EQUAL(vorgaenger.richtung, richtungenVorgaenger.at(i));
        BOOST_CHECK_EQUAL(&(vorgaenger.streckenelement), strecke->streckenelemente.at(nrsVorgaenger.at(i)).get());
    }

    for (size_t i = 0; i < nrsNachfolger.size(); i++) {
        BOOST_MESSAGE("Element " << nr << ", Nachfolger " << i);
        auto nachfolger = element->normrichtung().nachfolger(i);
        BOOST_CHECK_EQUAL(nachfolger.richtung, richtungenNachfolger.at(i));
        BOOST_CHECK_EQUAL(&(nachfolger.streckenelement), strecke->streckenelemente.at(nrsNachfolger.at(i)).get());
    }
}

void testEreignis(unique_ptr<Strecke>& strecke, streckenelement_nr_t nr,
        EreignisTyp ereignisTyp, ereignis_wert_t wert = 0, string beschreibung = "") {
    auto& richtungsInfo =
        strecke->streckenelemente.at(nr)->richtungsInfo[Streckenelement::RICHTUNG_NORM];
    BOOST_CHECK_EQUAL(richtungsInfo.ereignisse.size(), 1);
    BOOST_CHECK_EQUAL(richtungsInfo.ereignisse.at(0)->ereignisTyp, ereignisTyp);
    BOOST_CHECK_CLOSE(richtungsInfo.ereignisse.at(0)->wert, wert, epsilon);
    BOOST_CHECK_EQUAL(richtungsInfo.ereignisse.at(0)->beschreibung, beschreibung);
}

template<typename T>
void testSetEqual(set<T>& actual, set<T> expected) {
    BOOST_CHECK_EQUAL_COLLECTIONS(actual.begin(), actual.end(), expected.begin(), expected.end());
}
