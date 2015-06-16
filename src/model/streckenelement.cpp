#include "streckenelement.hpp"

const streckenelement_richtung_t Streckenelement::RICHTUNG_NORM;
const streckenelement_richtung_t Streckenelement::RICHTUNG_GEGEN;

StreckenelementUndRichtung Streckenelement::nachfolger (
        const nachfolger_index_t index, const streckenelement_richtung_t richtung) const {
    return nachfolgerElemente[richtung].at(index);
}

StreckenelementUndRichtung Streckenelement::vorgaenger(
        const nachfolger_index_t index, const streckenelement_richtung_t richtung) const {
    StreckenelementUndRichtung result =
        nachfolger(index, Streckenelement::gegenrichtung(richtung));
    result.richtung = Streckenelement::gegenrichtung(result.richtung);
    return result;
}

void Streckenelement::setzeNachfolger(const nachfolger_index_t index,
        const streckenelement_richtung_t richtung,
        const StreckenelementUndRichtung& nachfolger) {
    if (index >= nachfolgerElemente[richtung].size()) {
        nachfolgerElemente[richtung].resize(index + 1);
    }

    nachfolgerElemente[richtung].at(index) = nachfolger;
}

void Streckenelement::setzeVorgaenger(const nachfolger_index_t index,
        const streckenelement_richtung_t richtung,
        const StreckenelementUndRichtung& vorgaenger) {
    StreckenelementUndRichtung neueRichtung(
            vorgaenger.streckenelement,
            Streckenelement::gegenrichtung(vorgaenger.richtung));
    setzeNachfolger(index, Streckenelement::gegenrichtung(richtung), neueRichtung);
}
