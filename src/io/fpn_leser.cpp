#include "fpn_leser.hpp"

#include <cstring>

void liesStrModul(const xml_node<>& n, Fahrplan* fahrplan) {
    xml_node<>* datei_node = n.first_node("Datei");
    if (!datei_node) {
        return;
    }

    xml_attribute<>* dateiname_attr = datei_node->first_attribute("Dateiname");
    if (!dateiname_attr) {
        return;
    }

    fahrplan->streckenmodule.push_back(std::string(dateiname_attr->value()));
}

std::unique_ptr<Fahrplan> FpnLeser::parseWurzel(const xml_node<>& wurzel) {
    DateiInfo dateiInfo = this->liesDateiInfo(wurzel);
    unique_ptr<Fahrplan> fahrplan(new Fahrplan(dateiInfo.formatVersion, dateiInfo.formatMinVersion));

    xml_node<> *fpn_node = wurzel.first_node("Fahrplan");
    if (fpn_node != nullptr)
    {
        for (xml_node<> *n = fpn_node->first_node();
                n != nullptr;
                n = n->next_sibling()) {
            auto n_namesize = n->name_size();

            // Koordinaten
            if (!strncmp(n->name(), "StrModul", n_namesize)) {
                liesStrModul(*n, fahrplan.get());
            }
        }
    }

    return fahrplan;
}

