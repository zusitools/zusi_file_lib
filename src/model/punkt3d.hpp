#ifndef SRC_MODEL_PUNKT3D_HPP
#define SRC_MODEL_PUNKT3D_HPP

#include <type_traits>

#include <common/types.hpp>

#ifdef ZUSI_FILE_LIB_NOINLINE
#define _ZUSI_FILE_LIB_INLINE
#else
#define _ZUSI_FILE_LIB_INLINE inline
#endif

// Ein Punkt oder eine Rotation im dreidimensionalen Raum.
struct Punkt3D {
    koordinate_t x;
    koordinate_t y;
    koordinate_t z;

    _ZUSI_FILE_LIB_INLINE Punkt3D& operator-=(const Punkt3D& rhs);
    friend Punkt3D operator-(Punkt3D lhs, const Punkt3D& rhs) {
      lhs -= rhs;
      return lhs;
    }
};

static_assert(std::is_pod<Punkt3D>::value, "Punkt3D sollte ein POD-Datentyp sein");

// Eine Blickrichtung als zwei Vektoren.
struct Richtung3D {
    Punkt3D lookat;
    Punkt3D up;
};

// Eine Position inkl. Blickrichtung im dreidimensionalen Raum.
struct Pos3D {
    Punkt3D loc;

    inline const Richtung3D &richtungAlsVektoren() { return this->m_richtungAlsVektoren; }
    inline const Punkt3D &richtungAlsRot() { return this->m_richtungAlsRot; }

    _ZUSI_FILE_LIB_INLINE void setRichtungAlsVektoren(const Richtung3D richtungAlsVektoren);
    _ZUSI_FILE_LIB_INLINE void setRichtungAlsRot(const Punkt3D richtungAlsRot);

private:
    Richtung3D m_richtungAlsVektoren;
    Punkt3D m_richtungAlsRot;
};

#undef _ZUSI_FILE_LIB_INLINE

#ifndef ZUSI_FILE_LIB_NOINLINE
#include "punkt3d.cpp"
#endif

#endif
