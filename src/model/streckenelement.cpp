#include "streckenelement.hpp"

void Streckenelement::setzeNachfolger(nachfolger_index_t index,
        streckenelement_richtung_t richtung,
        Streckenelement& nachfolger) {
    if (index >= nachfolgerElemente[richtung].size()) {
        nachfolgerElemente[richtung].resize(index + 1);
    }

    nachfolgerElemente[richtung].at(index) = &nachfolger;
}

void Streckenelement::setzeNachfolger(nachfolger_index_t index,
        streckenelement_richtung_t richtung,
        Streckenelement& nachfolger,
        streckenelement_richtung_t nachfolgerRichtung) {

    if (nachfolgerRichtung == Streckenelement::RICHTUNG_GEGEN) {
        this->anschluss[richtung] |= 1 << index;
    }
    this->setzeNachfolger(index, richtung, nachfolger);
}

void Streckenelement::setzeVorgaenger(nachfolger_index_t index,
        streckenelement_richtung_t richtung,
        Streckenelement& vorgaenger) {
    setzeNachfolger(index, Streckenelement::richtungUmkehren(richtung), vorgaenger);
}

void Streckenelement::setzeVorgaenger(nachfolger_index_t index,
        streckenelement_richtung_t richtung,
        Streckenelement& vorgaenger,
        streckenelement_richtung_t vorgaengerRichtung) {

    if (vorgaengerRichtung == Streckenelement::RICHTUNG_NORM) {
        this->anschluss[Streckenelement::richtungUmkehren(richtung)] |= 1 << index;
    }
    this->setzeVorgaenger(index, richtung, vorgaenger);
}

std::vector<Streckenelement*>& StreckenelementUndRichtung::nachfolgerElemente() const {
    return this->streckenelement->nachfolgerElemente[this->richtung];
}

uint8_t StreckenelementUndRichtung::anschluss() const {
    return this->streckenelement->anschluss[this->richtung];
}

StreckenelementUndRichtung StreckenelementUndRichtung::nachfolger(const nachfolger_index_t index) const {
    return StreckenelementUndRichtung {
        this->streckenelement->nachfolgerElemente[this->richtung].at(index),
        ((this->streckenelement->anschluss[this->richtung] >> index) & 1) == 0 ?
            Streckenelement::RICHTUNG_NORM : Streckenelement::RICHTUNG_GEGEN };
}

bool StreckenelementUndRichtung::hatNachfolger(const nachfolger_index_t index) const {
    auto& nachfolger = this->streckenelement->nachfolgerElemente[this->richtung];
    return index < nachfolger.size() && nachfolger[index] != nullptr;
}

size_t StreckenelementUndRichtung::anzahlNachfolger() const {
    return this->streckenelement->nachfolgerElemente[this->richtung].size();
}

StreckenelementUndRichtung StreckenelementUndRichtung::vorgaenger(const nachfolger_index_t index) const {
    auto gegenrichtung = Streckenelement::richtungUmkehren(this->richtung);

    return StreckenelementUndRichtung {
        this->streckenelement->nachfolgerElemente[gegenrichtung].at(index),
        ((this->streckenelement->anschluss[gegenrichtung] >> index) & 1) == 0 ?
            Streckenelement::RICHTUNG_GEGEN : Streckenelement::RICHTUNG_NORM };
}

bool StreckenelementUndRichtung::hatVorgaenger(const nachfolger_index_t index) const {
    auto& nachfolger = this->streckenelement->nachfolgerElemente[Streckenelement::richtungUmkehren(this->richtung)];
    return index < nachfolger.size() && nachfolger[index] != nullptr;
}

size_t StreckenelementUndRichtung::anzahlVorgaenger() const {
    return this->streckenelement->nachfolgerElemente[Streckenelement::richtungUmkehren(this->richtung)].size();
}

StreckenelementUndRichtung StreckenelementUndRichtung::gegenrichtung() const {
    return StreckenelementUndRichtung { this->streckenelement, Streckenelement::richtungUmkehren(this->richtung) };
}

StreckenelementRichtungsInfo& StreckenelementUndRichtung::richtungsInfo() const {
    return this->streckenelement->richtungsInfo[this->richtung];
}

Punkt3D& StreckenelementUndRichtung::endpunkt() const {
    return this->richtung == Streckenelement::RICHTUNG_NORM ? this->streckenelement->p2 : this->streckenelement->p1;
}

Streckenelement& StreckenelementUndRichtung::operator*() const {
    return *(this->streckenelement);
}
