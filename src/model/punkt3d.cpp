#include "punkt3d.hpp"

Punkt3D& Punkt3D::operator-=(const Punkt3D& rhs)
{
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
}

void Pos3D::setRichtungAlsVektoren(const Richtung3D richtungAlsVektoren) {
    this->m_richtungAlsVektoren = Richtung3D(richtungAlsVektoren);
};

void Pos3D::setRichtungAlsRot(const Punkt3D richtungAlsRot) {
    this->m_richtungAlsRot = Punkt3D(richtungAlsRot);
};
