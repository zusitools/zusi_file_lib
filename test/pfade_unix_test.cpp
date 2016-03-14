#define BOOST_TEST_MODULE PfadeTestUnix
#include <boost/test/included/unit_test.hpp>

#include "common/pfade.hpp"

// ZUSI3_DATAPATH muss zum Testen auf /mnt/zusi/Zusi3/Daten (mit oder ohne Slash am Ende) gesetzt werden

BOOST_AUTO_TEST_CASE(zusiPfadZuOsPfadAbsolut) {
    BOOST_CHECK_EQUAL(zusi_file_lib::pfade::zusiPfadZuOsPfad(std::string("RollingStock\\ICE.ls3")),
                "/mnt/zusi/Zusi3/Daten/RollingStock/ICE.ls3");
    BOOST_CHECK_EQUAL(zusi_file_lib::pfade::zusiPfadZuOsPfad(std::string("\\RollingStock\\ICE.ls3")),
                "/mnt/zusi/Zusi3/Daten/RollingStock/ICE.ls3");
    BOOST_CHECK_EQUAL(zusi_file_lib::pfade::zusiPfadZuOsPfad(std::string("\\ICE.ls3")),
                "/mnt/zusi/Zusi3/Daten/ICE.ls3");
    BOOST_CHECK_EQUAL(zusi_file_lib::pfade::zusiPfadZuOsPfad(std::string("ICE.ls3")),
                "/mnt/zusi/Zusi3/Daten/ICE.ls3");
    BOOST_CHECK_EQUAL(zusi_file_lib::pfade::zusiPfadZuOsPfad(std::string("")),
                "/mnt/zusi/Zusi3/Daten/");
}

BOOST_AUTO_TEST_CASE(zusiPfadZuOsPfadRelativ) {
    BOOST_CHECK_EQUAL(zusi_file_lib::pfade::zusiPfadZuOsPfad(std::string("RollingStock\\ICE.ls3"),
                std::string("/mnt/zusi/Zusi3/Daten/RollingStock/Deutschland/ICE/ICE.fzg")),
                "/mnt/zusi/Zusi3/Daten/RollingStock/ICE.ls3");
    BOOST_CHECK_EQUAL(zusi_file_lib::pfade::zusiPfadZuOsPfad(std::string("\\RollingStock\\ICE.ls3"),
                std::string("/mnt/zusi/Zusi3/Daten/RollingStock/Deutschland/ICE/ICE.fzg")),
                "/mnt/zusi/Zusi3/Daten/RollingStock/ICE.ls3");
    BOOST_CHECK_EQUAL(zusi_file_lib::pfade::zusiPfadZuOsPfad(std::string("\\ICE.ls3"),
                std::string("/mnt/zusi/Zusi3/Daten/RollingStock/Deutschland/ICE/ICE.fzg")),
                "/mnt/zusi/Zusi3/Daten/ICE.ls3");
    BOOST_CHECK_EQUAL(zusi_file_lib::pfade::zusiPfadZuOsPfad(std::string("ICE.ls3"),
                std::string("/mnt/zusi/Zusi3/Daten/RollingStock/Deutschland/ICE/ICE.fzg")),
                "/mnt/zusi/Zusi3/Daten/RollingStock/Deutschland/ICE/ICE.ls3");
    BOOST_CHECK_EQUAL(zusi_file_lib::pfade::zusiPfadZuOsPfad(std::string(""),
                std::string("/mnt/zusi/Zusi3/Daten/RollingStock/Deutschland/ICE/ICE.fzg")),
                "/mnt/zusi/Zusi3/Daten/RollingStock/Deutschland/ICE/ICE.fzg");
}

