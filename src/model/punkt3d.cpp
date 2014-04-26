#include "punkt3d.hpp"

void Pos3D::setRichtungAlsVektoren(const Richtung3D richtungAlsVektoren) {
    this->m_richtungAlsVektoren = Richtung3D(richtungAlsVektoren);
};

void Pos3D::setRichtungAlsRot(const Punkt3D richtungAlsRot) {
    this->m_richtungAlsRot = Punkt3D(richtungAlsRot);
};
