#include "io/z3_leser.hpp"

#include "lib/rapidxml-1.13/rapidxml.hpp"

using namespace rapidxml;

unique_ptr<DateiInfo> Z3Leser::liesDateiInfo(xml_node<>& wurzel) {
    unique_ptr<DateiInfo> result(new DateiInfo());
    xml_node<> *dateiInfoNode = wurzel.first_node("Info");
    if (dateiInfoNode != nullptr) {
        xml_attribute<> *versionAttr = dateiInfoNode->first_attribute("Version");
        if (versionAttr != nullptr) {
            result->formatVersion = versionAttr->value();
        }
        xml_attribute<> *minVersionAttr = dateiInfoNode->first_attribute("MinVersion");
        if (minVersionAttr != nullptr) {
            result->formatMinVersion = minVersionAttr->value();
        }
    }

    return std::move(result);
}
