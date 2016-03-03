#include "st3_leser.hpp"

#include <iostream>
#include <fstream>

#include "lib/rapidxml-1.13/rapidxml.hpp"

void liesFloatAttr(xml_node<> &node, const char *name, float *wert) {
    if (wert != nullptr)
    {
        xml_attribute<> *attr = node.first_attribute(name);
        if (attr != nullptr)
        {
            *wert = stof(attr->value());
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

void setzeVorgaengerNachfolger(unordered_map<int, vector<int> > &nachfolger, streckenelement_richtung_t richtung,
        Strecke &strecke) {
    for (auto &it : nachfolger) {
        streckenelement_nr_t nr = it.first;
        if (nr >= 0 && nr < strecke.streckenelemente.size()) {
            auto streckenelement = strecke.streckenelemente.at(nr);
            if (!streckenelement) continue;
            nachfolger_index_t index = 0;
            for (auto nachfolgerNr : it.second) {
                auto nachfolger = strecke.streckenelemente.at(nachfolgerNr);
                if (!nachfolger) continue;
                streckenelement->setzeNachfolger(index++, richtung, nachfolger);
            }
        }
    }
}

unique_ptr<Strecke> St3Leser::liesSt3Datei(istream& datei) {
    unique_ptr<Strecke> strecke(new Strecke());

    vector<char> buffer((istreambuf_iterator<char>(datei)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    xml_document<> dok;
    dok.parse<0>(&buffer[0]);

    xml_node<> *wurzel = dok.first_node("Zusi");
    if (wurzel != nullptr)
    {
        strecke->dateiInfo = this->liesDateiInfo(*wurzel);
    }

    unordered_map<int, vector<int> > nachNorm;
    unordered_map<int, vector<int> > nachGegen;

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
            shared_ptr<Streckenelement> element(new Streckenelement());
            element->nr = liesInt(*elem_node, "Nr");

            if (strecke->streckenelemente.size() <= element->nr + 1)
            {
                strecke->streckenelemente.resize(element->nr + 1);
            }
            strecke->streckenelemente.at(element->nr) = element;

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
            for (xml_node<> *nachnorm_node = elem_node->first_node("NachNorm");
                    nachnorm_node != nullptr;
                    nachnorm_node = nachnorm_node->next_sibling("NachNorm")) {
                int nachfolgerNr = liesInt(*nachnorm_node, "Nr");
                if (nachfolgerNr != 0) {
                    nachNorm[element->nr].push_back(nachfolgerNr);
                }
            }
            for (xml_node<> *nachgegen_node = elem_node->first_node("NachGegen");
                    nachgegen_node != nullptr;
                    nachgegen_node = nachgegen_node->next_sibling("NachGegen")) {
                int nachfolgerNr = liesInt(*nachgegen_node, "Nr");
                if (nachfolgerNr != 0) {
                    nachGegen[element->nr].push_back(nachfolgerNr);
                }
            }

            // Anschluss
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
        }

        // Verknuepfungen zu Vorgaengern und Nachfolgern herstellen
        setzeVorgaengerNachfolger(nachNorm, Streckenelement::RICHTUNG_NORM, *strecke);
        setzeVorgaengerNachfolger(nachGegen, Streckenelement::RICHTUNG_GEGEN, *strecke);
    }

    return strecke;
}

unique_ptr<Strecke> St3Leser::liesSt3DateiMitDateiname(const string dateiname) {
    ifstream datei(dateiname);
    return St3Leser::liesSt3Datei(datei);
}
