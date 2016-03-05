#include "st3_leser.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <clocale>

#include "lib/rapidxml-1.13/rapidxml.hpp"

void liesFloatAttr(xml_node<> &node, const char *name, float *wert) {
    if (wert != nullptr)
    {
        xml_attribute<> *attr = node.first_attribute(name);
        if (attr != nullptr)
        {
            *wert = atof(attr->value());
        }
    }
}

void liesXYZ(xml_node<> &node, float *x, float *y, float *z) {
    liesFloatAttr(node, "X", x);
    liesFloatAttr(node, "Y", y);
    liesFloatAttr(node, "Z", z);
}

int liesInt(xml_node<> &node, const char* attrName) {
    xml_attribute<> *attr = node.first_attribute(attrName);
    if (attr != nullptr) {
        // TODO: Fehlerbehandlung
        return stoi(attr->value());
    } else {
        return 0;
    }
}

void setzeVorgaengerNachfolger(Strecke &strecke) {
    for (auto &streckenelement : strecke.streckenelemente) {
        if (!streckenelement) continue;
        for (auto &richtung : {Streckenelement::RICHTUNG_NORM, Streckenelement::RICHTUNG_GEGEN}) {
            streckenelement->nachfolgerElemente[richtung].resize(
                    streckenelement->nachfolgerElementeUnaufgeloest[richtung].size());
            for (size_t i = 0; i < streckenelement->nachfolgerElementeUnaufgeloest[richtung].size(); i++) {
                auto &pair = streckenelement->nachfolgerElementeUnaufgeloest[richtung][i];
                if (pair.first == nullptr) {
                    Streckenelement* nachfolger = strecke.streckenelemente.at(pair.second).get();
                    if (nachfolger != nullptr) {
                        streckenelement->setzeNachfolger(i, richtung, *nachfolger);
                    }
                }
            }
        }
    }
}

unique_ptr<Strecke> St3Leser::liesSt3Datei(istream& datei) {
    unique_ptr<Strecke> strecke(new Strecke());

    // TODO: Locale auch zuruecksetzen, wenn hier irgendwo ein Fehler fliegt
    char *oldlocale = setlocale(LC_NUMERIC, nullptr);
    setlocale(LC_NUMERIC, "C");

    vector<char> buffer((istreambuf_iterator<char>(datei)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    xml_document<> dok;
    dok.parse<0>(&buffer[0]);

    xml_node<> *wurzel = dok.first_node("Zusi");
    if (wurzel != nullptr)
    {
        strecke->dateiInfo = this->liesDateiInfo(*wurzel);
    }

    xml_node<> *str_node = wurzel->first_node("Strecke");
    if (str_node != nullptr)
    {
        xml_node<> *utm_node = str_node->first_node("UTM");
        if (utm_node != nullptr)
        {
            strecke->utmPunkt.we = liesInt(*utm_node, "UTM_WE");
            strecke->utmPunkt.ns = liesInt(*utm_node, "UTM_NS");
        }

        xml_node<> *elem_node = str_node->first_node("StrElement");
        while (elem_node != nullptr)
        {
            unique_ptr<Streckenelement> element(new Streckenelement());
            element->nr = liesInt(*elem_node, "Nr");

            // Koordinaten
            xml_node<> *g_node = elem_node->first_node("g");
            if (g_node != nullptr)
            {
                liesXYZ(*g_node, &element->p1.x, &element->p1.y, &element->p1.z);
            }
            xml_node<> *b_node = elem_node->first_node("b");
            if (b_node != nullptr)
            {
                liesXYZ(*b_node, &element->p2.x, &element->p2.y, &element->p2.z);
            }

            // Nachfolger
            for (xml_node<> *nachfolger_node = elem_node->first_node();
                    nachfolger_node != nullptr;
                    nachfolger_node = nachfolger_node->next_sibling()) {

                if (!strncmp(nachfolger_node->name(), "NachNorm", nachfolger_node->name_size())) {
                    int nachfolgerNr = liesInt(*nachfolger_node, "Nr");
                    element->nachfolgerElementeUnaufgeloest[Streckenelement::RICHTUNG_NORM].push_back(
                            std::pair<std::string*, streckenelement_nr_t>(nullptr, nachfolgerNr));

                } else if (!strncmp(nachfolger_node->name(), "NachGegen", nachfolger_node->name_size())) {
                    int nachfolgerNr = liesInt(*nachfolger_node, "Nr");
                    element->nachfolgerElementeUnaufgeloest[Streckenelement::RICHTUNG_GEGEN].push_back(
                            std::pair<std::string*, streckenelement_nr_t>(nullptr, nachfolgerNr));

                } else if (!strncmp(nachfolger_node->name(), "NachNormModul", nachfolger_node->name_size())) {
                    // TODO
                    element->nachfolgerElementeUnaufgeloest[Streckenelement::RICHTUNG_NORM].push_back(
                            std::pair<std::string*, streckenelement_nr_t>());

                } else if (!strncmp(nachfolger_node->name(), "NachGegenModul", nachfolger_node->name_size())) {
                    // TODO
                    element->nachfolgerElementeUnaufgeloest[Streckenelement::RICHTUNG_GEGEN].push_back(
                            std::pair<std::string*, streckenelement_nr_t>());
                }
            }

            // Anschluss (ohne Nachfolger in anderen Modulen)
            auto anschluss = liesInt(*elem_node, "Anschluss");
            element->anschluss[Streckenelement::RICHTUNG_NORM] = anschluss & 0xFF;
            element->anschluss[Streckenelement::RICHTUNG_GEGEN] = (anschluss >> 8) & 0xFF;

            // Funktions-Flags
            auto fkt_flags = liesInt(*elem_node, "Fkt");
            if (fkt_flags & 1) element->flags.insert(StreckenelementFlag::Tunnel);
            if (fkt_flags & 2) element->flags.insert(StreckenelementFlag::KeineGleisfunktion);
            if (fkt_flags & 4) element->flags.insert(StreckenelementFlag::Weichenbausatz);
            if (fkt_flags & 8) element->flags.insert(StreckenelementFlag::KeineSchulterRechts);
            if (fkt_flags & 16) element->flags.insert(StreckenelementFlag::KeineSchulterLinks);

            elem_node = elem_node->next_sibling("StrElement");

            if (strecke->streckenelemente.size() <= element->nr + 1)
            {
                strecke->streckenelemente.resize(element->nr + 1);
            }
            strecke->streckenelemente.at(element->nr) = std::move(element);
        }

        // Verknuepfungen zu Vorgaengern und Nachfolgern herstellen
        setzeVorgaengerNachfolger(*strecke);
    }

    setlocale(LC_NUMERIC, oldlocale);
    return strecke;
}

unique_ptr<Strecke> St3Leser::liesSt3DateiMitDateiname(const string dateiname) {
    ifstream datei(dateiname);
    return St3Leser::liesSt3Datei(datei);
}
