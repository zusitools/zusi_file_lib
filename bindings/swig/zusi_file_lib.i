%module zusi_file_lib

%include "typemaps.i"
%include "stdint.i"
%include "std_shared_ptr.i"
%include "std_string.i"
%include "std_vector.i"

%apply float { autoraufwand_t };
%apply uint_fast32_t { autorid_t };
%apply uint_fast8_t { breitengrad_t };
%apply uint_fast32_t { ereignis_nr_t };
%apply float { ereignis_wert_t };
%apply uint_fast32_t { fahrstr_register_nr_t };
%apply float { geschwindigkeit_t };
%apply float { helligkeit_t };
%apply float { kilometrierung_t };
%apply float { koordinate_t };
%apply float { kruemmung_t };
%apply uint_fast32_t { objektid_t };
%apply float { meter_t };
%apply uint_fast8_t { nachfolger_index_t} ;
%apply float { ueberhoehung_t} ;
%apply uint_fast32_t { referenz_nr_t} ;
%apply uint_fast8_t { referenz_typ_t} ;
%apply uint_fast32_t { rekursionstiefe_t} ;
%apply float { spannung_t} ;
%apply uint_fast32_t { streckenelement_nr_t} ;
%apply bool { streckenelement_richtung_t };

%array_class(StreckenelementRichtungsInfo, StreckenelementRichtungsInfoArray);

%{
#include <memory>
%}

%shared_ptr(ZusiObjekt)
%shared_ptr(Strecke)
%shared_ptr(Streckenelement)

%{
#include <io/str_leser.hpp>
%}

%inline %{
#include <common/types.hpp>
%}

%include <model/zusiobjekt.hpp>
%include <model/strecke.hpp>
%include <model/streckenelement.hpp>
%include <io/z2_leser.hpp>
%include <io/str_leser.hpp>

%template(StreckenelementPtrVector) std::vector<std::shared_ptr<Streckenelement>>;
