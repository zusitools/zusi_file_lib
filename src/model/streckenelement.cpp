#include "streckenelement.hpp"

const streckenelement_richtung_t Streckenelement::RICHTUNG_NORM;
const streckenelement_richtung_t Streckenelement::RICHTUNG_GEGEN;

void Streckenelement::setzeNachfolger(const nachfolger_index_t index,
        const streckenelement_richtung_t richtung,
        const shared_ptr<Streckenelement> nachfolger) {
    if (index >= nachfolgerElemente[richtung].size()) {
        nachfolgerElemente[richtung].resize(index + 1);
    }

    nachfolgerElemente[richtung].at(index) = std::weak_ptr<Streckenelement>(nachfolger);
}

void Streckenelement::setzeNachfolger(const nachfolger_index_t index,
        const streckenelement_richtung_t richtung,
        const shared_ptr<Streckenelement> nachfolger,
        const streckenelement_richtung_t nachfolgerRichtung) {

    if (nachfolgerRichtung == Streckenelement::RICHTUNG_GEGEN) {
        this->anschluss[richtung] |= 1 << index;
    }
    this->setzeNachfolger(index, richtung, nachfolger);
}

void Streckenelement::setzeVorgaenger(const nachfolger_index_t index,
        const streckenelement_richtung_t richtung,
        const shared_ptr<Streckenelement> vorgaenger) {
    setzeNachfolger(index, Streckenelement::richtungUmkehren(richtung), vorgaenger);
}

void Streckenelement::setzeVorgaenger(const nachfolger_index_t index,
        const streckenelement_richtung_t richtung,
        const shared_ptr<Streckenelement> vorgaenger,
        const streckenelement_richtung_t vorgaengerRichtung) {

    if (vorgaengerRichtung == Streckenelement::RICHTUNG_NORM) {
        this->anschluss[Streckenelement::richtungUmkehren(richtung)] |= 1 << index;
    }
    this->setzeVorgaenger(index, richtung, vorgaenger);
}

StreckenelementUndRichtung StreckenelementUndRichtung::nachfolger(const nachfolger_index_t index) const {
    return StreckenelementUndRichtung(
        this->streckenelement->nachfolgerElemente[this->richtung].at(index).lock().get(),
        ((this->streckenelement->anschluss[this->richtung] >> index) & 1) == 0 ?
            Streckenelement::RICHTUNG_NORM : Streckenelement::RICHTUNG_GEGEN);
}

bool StreckenelementUndRichtung::hatNachfolger(const nachfolger_index_t index) const {
    return index <= this->streckenelement->nachfolgerElemente[this->richtung].size();
}

StreckenelementUndRichtung StreckenelementUndRichtung::vorgaenger(const nachfolger_index_t index) const {
    auto gegenrichtung = Streckenelement::richtungUmkehren(this->richtung);

    return StreckenelementUndRichtung(
        this->streckenelement->nachfolgerElemente[gegenrichtung].at(index).lock().get(),
        ((this->streckenelement->anschluss[gegenrichtung] >> index) & 1) == 0 ?
            Streckenelement::RICHTUNG_GEGEN : Streckenelement::RICHTUNG_NORM);
}

bool StreckenelementUndRichtung::hatVorgaenger(const nachfolger_index_t index) const {
    return index <= this->streckenelement->nachfolgerElemente[Streckenelement::richtungUmkehren(this->richtung)].size();
}

StreckenelementUndRichtung StreckenelementUndRichtung::gegenrichtung() const {
    return StreckenelementUndRichtung(this->streckenelement, Streckenelement::richtungUmkehren(this->richtung));
}

Streckenelement& StreckenelementUndRichtung::operator*() {
    return *(this->streckenelement);
}
