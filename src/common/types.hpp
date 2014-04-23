#ifndef SRC_COMMON_TYPES_HPP
#define SRC_COMMON_TYPES_HPP

#include <cstdint>

typedef float autoraufwand_t;
typedef int_fast32_t autorid_t;
typedef int_fast32_t ereignis_nr_t;
typedef float ereignis_wert_t;
typedef int_fast32_t fahrstr_register_nr_t;
typedef float geschwindigkeit_t;
typedef float helligkeit_t;
typedef float kilometrierung_t;
typedef float koordinate_t;
typedef float kruemmung_t;
typedef int_fast32_t objektid_t;
typedef float meter_t;
typedef float ueberhoehung_t;
typedef float spannung_t;
typedef int_fast32_t streckenelement_nr_t;
typedef bool streckenelement_richtung_t;

// Ein Punkt im dreidimensionalen Raum.
struct Punkt3D {
    koordinate_t x;
    koordinate_t y;
    koordinate_t z;
};

// Eine Position inkl. Blickrichtung im dreidimensionalen Raum.
struct Pos3D {
    Punkt3D pos;
    Punkt3D lookat;
    Punkt3D up;
};

#endif
