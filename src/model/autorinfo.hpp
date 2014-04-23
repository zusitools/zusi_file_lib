#ifndef SRC_MODEL_AUTORINFO_HPP
#define SRC_MODEL_AUTORINFO_HPP

#include <string>
#include <common/types.hpp>

using namespace std;

struct AutorInfo {

    // Name des Autors.
    string name;

    // E-Mail-Adresse des Autors.
    string eMail;
    
    // Autor-ID.
    autorid_t autorId;

    // Autor-Aufwand.
    autoraufwand_t aufwand;
};

#endif
