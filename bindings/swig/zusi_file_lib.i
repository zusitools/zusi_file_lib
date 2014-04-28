%module zusi_file_lib

%include "typemaps.i"
%include "std_shared_ptr.i"
%include "std_string.i"
%include "std_vector.i"

%{
#include <memory>
%}

%shared_ptr(ZusiObjekt)
%shared_ptr(Strecke)
%shared_ptr(Streckenelement)

%{
#include <io/str_leser.hpp>
%}

%include <common/types.hpp>
%include <model/zusiobjekt.hpp>
%include <model/strecke.hpp>
%include <model/streckenelement.hpp>
%include <io/z2_leser.hpp>
%include <io/str_leser.hpp>

%template(StreckenelementPtrVector) std::vector<std::shared_ptr<Streckenelement>>;
