#include "fpn_leser.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <clocale>

#include "lib/rapidxml-1.13/rapidxml.hpp"

void liesStrModul(xml_node<>& n, Fahrplan& fahrplan) {
    xml_node<>* datei_node = n.first_node("Datei");
    if (!datei_node) {
        return;
    }

    xml_attribute<>* dateiname_attr = datei_node->first_attribute("Dateiname");
    if (!dateiname_attr) {
        return;
    }

    fahrplan.streckenmodule.push_back(std::string(dateiname_attr->value()));
}

unique_ptr<Fahrplan> FpnLeser::liesFpnDatei(istream& datei) {
    unique_ptr<Fahrplan> fahrplan(new Fahrplan());

    // TODO: Locale auch zuruecksetzen, wenn hier irgendwo ein Fehler fliegt
    char *oldlocale = setlocale(LC_NUMERIC, nullptr);
    setlocale(LC_NUMERIC, "C");

    vector<char> buffer((istreambuf_iterator<char>(datei)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    xml_document<> dok;
    dok.parse<0>(&buffer[0]);

    xml_node<> *wurzel = dok.first_node("Zusi");
    if (wurzel == nullptr)
    {
        throw std::invalid_argument("Keine gültige Zusi-Datei (Zusi-Wurzelknoten nicht gefunden)");
    }

    fahrplan->dateiInfo = this->liesDateiInfo(*wurzel);

    xml_node<> *fpn_node = wurzel->first_node("Fahrplan");
    if (fpn_node != nullptr)
    {
        for (xml_node<> *n = fpn_node->first_node();
                n != nullptr;
                n = n->next_sibling()) {
            auto n_namesize = n->name_size();

            // Koordinaten
            if (!strncmp(n->name(), "StrModul", n_namesize)) {
                liesStrModul(*n, *fahrplan);
            }
        }
    }

    setlocale(LC_NUMERIC, oldlocale);
    return fahrplan;
}

unique_ptr<Fahrplan> FpnLeser::liesFpnDateiMitDateiname(const string dateiname) {
    ifstream datei(dateiname);
    return FpnLeser::liesFpnDatei(datei);
}
