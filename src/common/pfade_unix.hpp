#include <algorithm>

namespace zusi_file_lib {

    namespace pfade {

        const std::string& getZusi3Datenpfad() {
            if (zusi3Datenpfad.empty()) {
                zusi3Datenpfad = std::string(getenv("ZUSI3_DATAPATH"));
            }
            return zusi3Datenpfad;
        }

        std::string zusiPfadZuOsPfad(const std::string& zusiPfad, const std::string& osPfadUebergeordnet) {
            std::string result;
            if (zusiPfad.find('\\') == zusiPfad.npos && !osPfadUebergeordnet.empty()) {
                // Relativ zu uebergeordnetem Pfad
                if (zusiPfad.empty()) {
                    return osPfadUebergeordnet;
                }
                result = osPfadUebergeordnet.substr(0, osPfadUebergeordnet.rfind('/'));
            } else {
                // Relativ zum Zusi-Datenverzeichnis
                result = std::string(getZusi3Datenpfad());
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
