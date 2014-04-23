#include <boost/python.hpp>
#include <memory>

#include <io/str_leser.hpp>
#include <model/strecke.hpp>

using namespace boost::python;
using namespace std;

// Taken from http://stackoverflow.com/questions/20581679/boost-python-how-to-expose-stdunique-ptr

/// @brief Adapter from a member function that returns a unique_ptr to
///        a python function object that returns a raw pointer but
///        explicitly passes ownership to Python.
template <typename T,
          typename ...Args>
boost::python::object adapt_unique(unique_ptr<T> (*fn)(Args...))
{
    return boost::python::make_function(
        [fn](Args... args) { return fn(args...).release(); },
        return_value_policy<manage_new_object>(),
        boost::mpl::vector<T*, Args...>()
    );
}

BOOST_PYTHON_MODULE(zusi_file_lib_ext)
{
    class_<Strecke, boost::noncopyable>("Strecke")
        .add_property("breitengrad", &Strecke::breitengrad)
    ;

    class_<StrLeser>("StrLeser")
        .def("liesStrDatei", adapt_unique(&StrLeser::liesStrDatei))
        .staticmethod("liesStrDatei")
        .def("liesStrDateiMitDateiname", adapt_unique(&StrLeser::liesStrDateiMitDateiname))
        .staticmethod("liesStrDateiMitDateiname")
    ;
}
