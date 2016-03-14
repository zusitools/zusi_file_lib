#include <algorithm>

namespace zusi_file_lib {

    namespace pfade {

        using namespace std;

        string zusiPfadZuOsPfad(const string zusiPfad, const string osPfadUebergeordnet = std::string()) {
            string result;
            if (zusiPfad.find('\\') == zusiPfad.npos && !osPfadUebergeordnet.empty()) {
                // Relativ zu uebergeordnetem Pfad
                if (zusiPfad.empty()) {
                    return osPfadUebergeordnet;
                }
                result = osPfadUebergeordnet.substr(0, osPfadUebergeordnet.rfind('/'));
            } else {
                // Relativ zum Zusi-Datenverzeichnis
                const char* datapath = getenv("ZUSI3_DATAPATH");
                if (datapath == nullptr) {
                    return "";
                }
                result = string(datapath);
            }

            if (result.back() == '/' && zusiPfad.front() == '\\') {
                result.pop_back();
            } else if (result.back() != '/' && zusiPfad.front() != '\\') {
                result.push_back('/');
            }

            result += zusiPfad;
            std::replace(result.begin(), result.end(), '\\', '/');
            return result;
        }

    }

}
