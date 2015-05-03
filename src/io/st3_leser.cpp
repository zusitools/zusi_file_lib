#include "st3_leser.hpp"

#include <iostream>

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

    xml_node<> *str_node = wurzel->first_node("Strecke");
    if (str_node != nullptr)
    {
        xml_node<> *elem_node = str_node->first_node("StrElement");
        while (elem_node != nullptr)
        {
            shared_ptr<Streckenelement> element(new Streckenelement());

            xml_attribute<> *nrAttr = elem_node->first_attribute("Nr");
            if (nrAttr != nullptr) {
                // TODO: Fehlerbehandlung
                element->nr = stoi(nrAttr->value());
            } else {
                element->nr = 0;
            }

            if (strecke->streckenelemente.size() <= element->nr + 1)
            {
                strecke->streckenelemente.resize(element->nr + 1);
            }
            strecke->streckenelemente.at(element->nr) = element;

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

            elem_node = elem_node->next_sibling("StrElement");
        }
    }

    return strecke;
}
