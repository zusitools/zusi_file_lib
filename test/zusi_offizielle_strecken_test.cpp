#define BOOST_TEST_MODULE ZusiOffizielleStreckenTest 
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <boost/test/included/unit_test.hpp>

#include <io/str_leser.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE(lies_strecken) {
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

    char *zusiStreckenPfad = getenv("ZUSI_STRECKEN_PFAD");
    BOOST_REQUIRE(zusiStreckenPfad != nullptr);

    for (const string& dateiname : strDateiNamen) {
        BOOST_TEST_CHECKPOINT(dateiname);
        ifstream infile(zusiStreckenPfad + dateiname);
        auto leseStart = chrono::high_resolution_clock::now();
        unique_ptr<Strecke> strecke = StrLeser().liesStrDatei(infile);
        auto leseEnde = chrono::high_resolution_clock::now();
        auto ms = chrono::duration_cast<chrono::milliseconds>(leseEnde - leseStart).count();
        BOOST_MESSAGE(dateiname + ": " + to_string(ms) + " ms");
    }
}
