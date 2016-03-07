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

unsigned int liesUint(xml_node<> &node, const char* attrName) {
    xml_attribute<> *attr = node.first_attribute(attrName);
    if (attr != nullptr) {
        // TODO: Fehlerbehandlung
        return strtoul(attr->value(), nullptr, 10);
    } else {
        return 0;
    }
}

void setzeVorgaengerNachfolger(Strecke &strecke) {
    for (auto& streckenelement : strecke.streckenelemente) {
        if (!streckenelement) continue;
        for (auto& aufloeseInfo : streckenelement->nachfolgerElementeUnaufgeloest) {
            auto& nachfolger = strecke.streckenelemente.at(aufloeseInfo.nr.nr_se);
            if (nachfolger) {
                streckenelement->setzeNachfolger(aufloeseInfo.nachfolger_index, aufloeseInfo.richtung, *nachfolger);
            }
        }
    }
}

void liesRichtungsInfo(xml_node<>& ri_node, Streckenelement& element, const streckenelement_richtung_t richtung) {
    auto& richtungsInfo = element.richtungsInfo[richtung];

    for (xml_node<> *n = ri_node.first_node();
            n != nullptr;
            n = n->next_sibling()) {
        auto n_namesize = n->name_size();

        if (!strncmp(n->name(), "Signal", n_namesize)) {
            richtungsInfo.signal.reset(new Signal());

            for (xml_attribute<> *attr = n->first_attribute();
                    attr != nullptr;
                    attr = attr->next_attribute()) {
                auto attr_namesize = attr->name_size();

                if (!strncmp(attr->name(), "NameBetriebsstelle", attr_namesize)) {
                    richtungsInfo.signal->betriebsstelle = std::string(attr->value());

                } else if (!strncmp(attr->name(), "Stellwerk", attr_namesize)) {
                    richtungsInfo.signal->stellwerk = std::string(attr->value());

                } else if (!strncmp(attr->name(), "Signalname", attr_namesize)) {
                    richtungsInfo.signal->signalbezeichnung = std::string(attr->value());
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

            size_t nachnorm_idx = 0;
            size_t nachgegen_idx = 0;

            for (xml_node<> *n = elem_node->first_node();
                    n != nullptr;
                    n = n->next_sibling()) {
                auto n_namesize = n->name_size();

                // Koordinaten
                if (!strncmp(n->name(), "g", n_namesize)) {
                    liesXYZ(*n, &element->p1.x, &element->p1.y, &element->p1.z);
                } else if (!strncmp(n->name(), "b", n_namesize)) {
                    liesXYZ(*n, &element->p2.x, &element->p2.y, &element->p2.z);

                // Nachfolger
                } else if (!strncmp(n->name(), "NachNorm", n_namesize)) {
                    unsigned int nachfolgerNr = liesUint(*n, "Nr");
                    bool aufgeloest = false;
                    if (nachfolgerNr < strecke->streckenelemente.size()) {
                        auto& nachfolger = strecke->streckenelemente[nachfolgerNr];
                        if (nachfolger) {
                            element->setzeNachfolger(nachnorm_idx, Streckenelement::RICHTUNG_NORM, *nachfolger.get());
                            aufgeloest = true;
                        }
                    }

                    if (!aufgeloest) {
                        element->nachfolgerElementeUnaufgeloest.push_back(
                                StreckenelementAufloeseInfo(Streckenelement::RICHTUNG_NORM, nachgegen_idx, nachfolgerNr));
                    }
                    nachnorm_idx++;

                } else if (!strncmp(n->name(), "NachGegen", n_namesize)) {
                    unsigned int nachfolgerNr = liesUint(*n, "Nr");
                    bool aufgeloest = false;
                    if (nachfolgerNr < strecke->streckenelemente.size()) {
                        auto& nachfolger = strecke->streckenelemente[nachfolgerNr];
                        if (nachfolger) {
                            element->setzeNachfolger(nachnorm_idx, Streckenelement::RICHTUNG_GEGEN, *nachfolger.get());
                            aufgeloest = true;
                        }
                    }

                    if (!aufgeloest) {
                        element->nachfolgerElementeUnaufgeloest.push_back(
                                StreckenelementAufloeseInfo(Streckenelement::RICHTUNG_GEGEN, nachgegen_idx, nachfolgerNr));
                    }
                    nachgegen_idx++;

                } else if (!strncmp(n->name(), "NachNormModul", n_namesize)) {
                    // TODO
                    nachnorm_idx++;

                } else if (!strncmp(n->name(), "NachGegenModul", n_namesize)) {
                    // TODO
                    nachgegen_idx++;

                // Richtungsinfo
                } else if (!strncmp(n->name(), "InfoNormRichtung", n_namesize)) {
                    liesRichtungsInfo(*n, *element, Streckenelement::RICHTUNG_NORM);

                } else if (!strncmp(n->name(), "InfoGegenRichtung", n_namesize)) {
                    liesRichtungsInfo(*n, *element, Streckenelement::RICHTUNG_GEGEN);
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
