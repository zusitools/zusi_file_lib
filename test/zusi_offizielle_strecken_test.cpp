#define BOOST_TEST_MODULE ZusiOffizielleStreckenTest 
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <boost/test/included/unit_test.hpp>

#include <io/str_leser.hpp>
#include <io/st3_leser.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE(lies_z2_strecken) {
    vector<string> strDateiNamen({
        "Arnstadt_Ilmenau/Arnstadt_Ilmenau.str",
        "Bln_Anhalter_Bhf_Jueterbog/Strecke.str",
        "Braunschweig-Harzburg/BraunschweigHarzburg.str",
        "ChemnZwickau/Chemnitz-ReichenbacV31.str",
        "DemoStrecken/Ma_Signale_Demostrecke/MaDemoStrecke.str",
        "DemoStrecken/SignalDemo/SignalDemo_HpLicht.str",
        "DemoStrecken/Sk_Signale_Demostrecke/SkDemostrecke.str",
        "DemoStrecken/Uebungsstrecke/UebungsStrecke.str",
        "DemoStrecken/Uebungsstrecke/UebungsStrecke_Schritt1.str",
        "DemoStrecken/Uebungsstrecke/UebungsStrecke_Schritt2.str",
        "DemoStrecken/Uebungsstrecke/UebungsStrecke_Schritt3.str",
        "DemoStrecken/Uebungsstrecke/UebungsStrecke_Schritt4.str",
        "DemoStrecken/Uebungsstrecke/UebungsStrecke_Schritt5.str",
        "DessauRosslau/Version_2_5/MedewitzBitterfeld.str",
        "DessauRosslau/Version_2_5/MedewitzBitterfeld_patched.str",
        "DessauRosslau/Version_DR1/DessauBitterfeld_DR.str",
        "DessauRosslau/Version_Sp1/GueterglueckWittenberg.str",
        "Eifel/Eifelstrecke.str",
        "Fantasiestrecken/Bummelbahn/Bummelbahn2.str",
        "Fantasiestrecken/Wuestenstrecke/Strecke_mit_Geiern.str",
        "Fantasiestrecken/Wuestenstrecke/Strecke_ohne_Geier.str",
        "Fantasiestrecken/F(R)S/F(R)S V2_3_0_24.str",
        "Fantasiestrecken/F(R)S/F(R)S V2_3_0_24_Arbeit.str",
        "Fantasiestrecken/F(R)S/F(R)S V2_3_0_24_Light.str",
        "Fantasiestrecken/Miechow-Wittenhagen/Miechow-Wittenhagen.str",
        "Fantasiestrecken/Nebenstrecke/Strecke.str",
        "Fantasiestrecken/Nebenstrecke_mit_Strab/Strecke.str",
        "Fantasiestrecken/Untersmatt-Hochleiten/Stand_1970/Neuenburg-Untersmatt-Hochleiten.str",
        "Fantasiestrecken/Untersmatt-Hochleiten/Stand_2002/Neuenburg-Untersmatt-Hochleiten2002.str",
        "Fantasiestrecken/Fiktive_SFS_Benni/ICE_BW-SFS110_V1.str",
        "Fantasiestrecken/kuk-Netz/KuK_Netz.str",
        "Fantasiestrecken/NordAmerika/Branchline/Branchline_14.str",
        "Fantasiestrecken/NordAmerika/PassRoute_V23/PassRoute_V23_12.str",
        "Fantasiestrecken/Neustadt-Nonnendorf/NN_ohneLandschaft.str",
        "Fantasiestrecken/Neustadt-Nonnendorf/StreckennetzNeustadt-Nonnendorf_2_0_29.str",
        "Fantasiestrecken/Neustadt-Nonnendorf/StreckennetzNeustadt-Nonnendorf_2_0_29_Light.str",
        "Fantasiestrecken/FiktiveHauptbahn/FiktiveHauptbahn.str",
        "Fantasiestrecken/Am_See/Am_See_V73_1.str",
        "Freilassing-Berchtesgaden/Freilassing-Berchtesgaden.str",
        "Freising-Unterzolling/Freising-Unterzolling.str",
        "EVAG/Stadtbahn.str",
        "EVAG/Tram.str",
        "KBS650_Main-Neckar-Bahn/KBS650_Main_Neckar_Bahn_1_1.str",
        "KBS850_Lichtenfels-Hof/Stand_1981/KBS850_1981.str",
        "KBS850_Lichtenfels-Hof/Stand_2001/KBS850_2001.str",
        "KBS850_Lichtenfels-Hof/Stand_2004/KBS850_2004.str",
        "KBS850_Lichtenfels-Hof/Stand_1995/KBS850_1995.str",
        "Kochelseebahn/Kochelseebahn_LA.str",
        "Main-Weser-Bahn GI-FB/Streckenversion_V6/GI-FFM_V6_5_03.str",
        "Main-Weser-Bahn GI-FB/Streckenversion_V8/GI-FFM_V8_29.str",
        "Miesbach_Bayrischzell/Miesbach_Bayrischzell_Winter.str",
        "Muenchen-Ingolstadt/Stand2001/Muenchen-Ingolstadt.str",
        "Muenchen-Ingolstadt/Stand2002/Muenchen-Ingolstadt_2002.str",
        "Muenchen-Petershausen (Stammstrecke)/Muenchen-Petershausen.str",
        "Muenchen_S4_S8West/S4/Leienfelsstrasse-Geltendorf.str",
        "Muenchen_S4_S8West/S4/Leienfelsstrasse-Geltendorf_GNT.str",
        "Muenchen_S4_S8West/S8West/Pasing-Nannhofen.str",
        "Muenchen_S4_S8West/S8West/Pasing-Nannhofen_fuer_Fahrplan_Umleitung.str",
        "schmalspur/Bottwartalbahn/bottwartalbahn_v1_2_beta.str",
        "Netz_Muenchen_Sued/netz_m_sued.str",
        "Netz_Suedniedersachsen/Altenbeken-Northeim/1984_Altenbeken-Northeim.str",
        "Netz_Suedniedersachsen/Bahnhoefe/Verliehausen/1980_Verliehausen.str",
        "Netz_Suedniedersachsen/KBS350/Goettingen-Kreiensen.str",
        "Netz_Suedniedersachsen/KBS351/SFS Hildesheim-Goettingen.str",
        "Netz_Suedniedersachsen/Braunschweig-Kreiensen/Braunschweig-Kreiensen.str",
        "ObereRuhrtalbahn/Version_09/Obere_Ruhrtalbahn.str",
        "ObereRuhrtalbahn/Version_09/Obere_Ruhrtalbahn_1975.str",
        "Oesterreich/Klettertour/Klettertour_OeBB_modern.str",
        "Oesterreich/Klettertour/Klettertour_OeBB_modern_Streckensperre.str",
        "Oesterreich/Klettertour/Klettertour_OeBB_nostalgisch.str",
        "Oesterreich/Semmeringbahn/Semmeringbahn_OeBB.str",
        "UsedomerBaederbahn/Usedomer Baederbahn.str",
        "Wannseebahn/1970/Wannseebahn_1970.str",
        "Wannseebahn/2001/Wannseebahn_2001.str",
        "Wolfsburg-Harz/Wolfsburg-BraunschweigV_1_0.str",
        "Zuggruppe_B_N/Strecke.str",
        "Wutachtalbahn/Version_2005/ZollhausWeizen_2005.str",
        "Wutachtalbahn/Version_2002/ZollhausWeizen_2002.str",
        "Wutachtalbahn/Version_Tempo80/ZollhausWeizen_Tempo80.str",
        "Schweiz/Insubria/Insubria_1_4_1.str",
        "Netz_Nordbaden/Elsenztalbahn/Elsenztalbahn_V1_2.str",
        "Netz_Nordbaden/Elsenztalbahn/Elsenztalbahn_V1_12_Debug.str",
        "Netz_Duesseldorf/Gesamtstrecke_NE-Hafen/Gesamtstrecke_Neuss-Hafen.str",
        "Netz_Duesseldorf/Duesseldorf_Neuss/Duesseldorf_Koeln_MGladbach_Krefeld.str",
        "Netz_Duesseldorf/Duesseldorf-Leverkusen/Duesseldorf-Leverkusen.str",
        "Limburg-Westerburg/Limburg-Westerburg.str",
        "LinkeRheinstrecke/LinkeRheinstrecke1987_V2_62.str",
    });

    char *zusiStreckenPfad = getenv("ZUSI2_STRECKEN_PFAD");
    BOOST_REQUIRE(zusiStreckenPfad != nullptr);

    for (const string& dateiname : strDateiNamen) {
        BOOST_TEST_CHECKPOINT(dateiname);
        ifstream infile(zusiStreckenPfad + dateiname);
        auto leseStart = chrono::high_resolution_clock::now();
        unique_ptr<Strecke> strecke = StrLeser().liesStrDatei(infile);
        auto leseEnde = chrono::high_resolution_clock::now();
        auto ms = chrono::duration_cast<chrono::milliseconds>(leseEnde - leseStart).count();
        BOOST_TEST_MESSAGE(dateiname + ": " + to_string(ms) + " ms");
    }
}

BOOST_AUTO_TEST_CASE(lies_z3_strecken) {
    vector<string> strDateiNamen({
        "Deutschland/32T_0005_0053/000463_005293_Grimmelshofen/Grimmelshofen_2008.st3",
        "Deutschland/32T_0005_0053/000465_005296_Fuetzen/fuetzen2005.st3",
        "Deutschland/32T_0005_0053/000468_005299_Zollhaus/Zollhaus2008.st3",
        "Deutschland/32T_0005_0053/000474_005303_Aulfingen/Aulfingen2005.st3",
        "Deutschland/32U_0004_0057/000442_005692_Freienohl/Freienohl_1985.st3",
        "Deutschland/32U_0004_0057/000444_005689_Wennemen/Wennemen_1973.st3",
        "Deutschland/32U_0004_0057/000449_005689_Meschede/Meschede_1980.st3",
        "Deutschland/32U_0005_0057/000455_005689_Eversberg/Eversberg_1985.st3",
        "Deutschland/32U_0005_0057/000460_005690_Bestwig/Bestwig_1985.st3",
        "Deutschland/32U_0005_0057/000461_005691_Nuttlar/Nuttlar_1980.st3",
        "Deutschland/32U_0005_0057/000464_005690_Olsberg/Olsberg_1973.st3",
        "Deutschland/32U_0005_0057/000466_005689_Elleringhausen/Elleringhausen_1980.st3",
        "Deutschland/32U_0005_0057/000471_005689_Brilon_Wald/Brilon_Wald_1985.st3",
        "Deutschland/32U_0005_0057/000475_005692_Hoppecke/Hoppecke_1980.st3",
        "Deutschland/32U_0005_0057/000480_005694_Messinghausen/Messinghausen_1980.st3",
        "Deutschland/32U_0005_0057/000483_005730_Paderborn/Paderborn_1985.st3",
        "Deutschland/32U_0005_0057/000486_005697_Bredelar/Bredelar_1978.st3",
        "Deutschland/32U_0005_0057/000487_005731_Benhausen/Benhausen_1985.st3",
        "Deutschland/32U_0005_0057/000490_005701_Marsberg/Marsberg_1978.st3",
        "Deutschland/32U_0005_0057/000495_005705_Westheim/Westheim_1985.st3",
        "Deutschland/32U_0005_0057/000496_005734_Altenbeken/Altenbeken_1985.st3",
        "Deutschland/32U_0005_0057/000497_005731_Buke/Buke_1985.st3",
        "Deutschland/32U_0005_0057/000498_005723_Neuenheerse/Neuenheerse_1985.st3",
        "Deutschland/32U_0005_0057/000499_005737_Langeland/Langeland_1985.st3",
        "Deutschland/32U_0005_0057/000500_005707_Wrexen/Wrexen_1985.st3",
        "Deutschland/32U_0005_0057/000500_005719_Willebadessen/Willebadessen_1985.st3",
        "Deutschland/32U_0005_0057/000500_005739_Himmighausen/Himmighausen_1985.st3",
        "Deutschland/32U_0005_0057/000503_005732_Driburg/Driburg_1985.st3",
        "Deutschland/32U_0005_0057/000504_005709_Scherfede/Scherfede_1974.st3",
        "Deutschland/32U_0005_0057/000505_005712_Bonenburg/Bonenburg_1985.st3",
        "Deutschland/32U_0005_0057/000509_005707_Noerde/Noerde_1985_Dummy.st3",
        "Deutschland/32U_0005_0057/000509_005707_Warburg/Warburg_1985.st3",
        "Deutschland/32U_0005_0057/000517_005705_Liebenau/Liebenau_1980.st3",
        "Deutschland/32U_0005_0057/000523_005708_Lamerden/Lamerden_1985.st3",
        "Deutschland/32U_0005_0057/000527_005706_Hofgeismar/Hofgeismar_1985.st3",
        "Deutschland/32U_0005_0057/000527_005709_Huemme/Huemme_1980.st3",
        "Deutschland/32U_0005_0057/000530_005701_Grebenstein/Grebenstein_1985.st3",
        "Deutschland/32U_0005_0057/000531_005684_Kassel_Wilhelmshoehe/Kassel_Wilhelmshoehe_1988.st3",
        "Deutschland/32U_0005_0057/000531_005690_Obervellmar/Obervellmar_1985.st3",
        "Deutschland/32U_0005_0057/000531_005696_Immenhausen/Immenhausen_1985.st3",
        "Deutschland/32U_0005_0057/000532_005686_Kassel_Bahndreieck/Kassel_Bahndreieck_1990.st3",
        "Deutschland/32U_0005_0057/000532_005693_Moenchehof/Moenchehof_1980.st3",
        "Deutschland/32U_0005_0057/000533_005686_Kassel_Hbf/Kassel_Hbf_1988.st3",
        "Deutschland/32U_0005_0057/000533_005687_Harleshausen/Harleshausen_1980.st3",
        "Deutschland/32U_0005_0057/000533_005687_Kassel_Rbf/Kassel_Rbf_1988.st3",
        "Deutschland/32U_0005_0057/000534_005686_Kassel_Unterstadt/Kassel_Unterstadt_GF_Dummy.st3",
        "Deutschland/32U_0005_0057/000534_005689_Niedervellmar/Niedervellmar_2000.st3",
        "Deutschland/32U_0005_0057/000537_005690_Ihringshausen/Ihringshausen_2000.st3",
        "Deutschland/32U_0005_0057/000541_005691_Lutterberg/Lutterberg_2000.st3",
        "Deutschland/32U_0005_0057/000548_005695_Lippoldshausen/Lippoldshausen_2000.st3",
        "Deutschland/32U_0005_0057/000474_005726_Salzkotten/Salzkotten_1994.st3",
        "Deutschland/32U_0005_0060/000486_006019_StPeterOrding/StPeterOrding_2006.st3",
        "Deutschland/32U_0005_0060/000500_006026_Hoern-Toenning/Hoern-Toenning_2008.st3",
        "Deutschland/32U_0005_0060/000504_006026_Friedrichstadt/Friedrichstadt_2006.st3",
        "Deutschland/32U_0005_0060/000504_006031_AbzwHoern/AbzwHoern_2006.st3",
        "Deutschland/32U_0005_0060/000504_006037_Husum/Husum_2006.st3",
        "Deutschland/32U_0006_0057/000558_005703_Juehnde/Juehnde_2000.st3",
        "Deutschland/32U_0006_0057/000562_005708_Siekweg/Siekweg_2000.st3",
        "Deutschland/32U_0006_0057/000563_005706_Rosdorf/Rosdorf_1998.st3",
        "Deutschland/32U_0006_0057/000564_005711_Goettingen_Pbf/Goettingen_Pbf_2000.st3",
        "Deutschland/32U_0006_0057/000564_005713_Goettingen_Gbf/Goettingen_Gbf_2000.st3",
        "_Docu/Bremsteststrecke/Bremsteststrecke.st3",
        "_Docu/Kap3_Uebungsstrecke/000442_005680_Eslohe/Eslohe_1980.st3",
        "_Docu/Kap3_Uebungsstrecke/000443_005681_Wenholthausen/Wenholthausen_1980.st3",
        "_Docu/Kap3_Uebungsstrecke/000443_005681_Wenholthausen/Wenholthausen_1980_Grundplatte_Huellkurve.st3",
        "_Docu/Radienteststrecke/Radienteststrecke.st3",
    });

    char *zusiStreckenPfad = getenv("ZUSI3_STRECKEN_PFAD");
    BOOST_REQUIRE(zusiStreckenPfad != nullptr);

    auto globalLeseStart = chrono::high_resolution_clock::now();
    for (const string& dateiname : strDateiNamen) {
        BOOST_TEST_CHECKPOINT(dateiname);
        ifstream infile(zusiStreckenPfad + dateiname);
        auto leseStart = chrono::high_resolution_clock::now();
        unique_ptr<Strecke> strecke = St3Leser().liesDatei(infile);
        auto leseEnde = chrono::high_resolution_clock::now();
        auto ms = chrono::duration_cast<chrono::milliseconds>(leseEnde - leseStart).count();
        BOOST_TEST_MESSAGE(dateiname + ": " + to_string(ms) + " ms");
    }
    auto globalLeseEnde = chrono::high_resolution_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(globalLeseEnde - globalLeseStart).count();
    BOOST_TEST_MESSAGE("Gesamt: " + to_string(ms) + " ms");
}
