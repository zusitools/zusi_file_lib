#include <algorithm>
#include <windows.h>
#include <winerror.h>

namespace zusi_file_lib {

    namespace pfade {

        const std::string& getZusi3Datenpfad() {
            if (zusi3Datenpfad.empty()) {
                HKEY key;
                char buffer[MAX_PATH];
                DWORD len = MAX_PATH;
                if (SUCCEEDED(RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Zusi3", 0, KEY_READ | KEY_WOW64_32KEY, &key)) &&
				        SUCCEEDED(RegGetValueA(key, nullptr, "DatenVerzeichnis", RRF_RT_REG_SZ, nullptr, (LPBYTE)buffer, &len))) {
                    zusi3Datenpfad = string(buffer, len - 1); // subtract terminating null character
                    RegCloseKey(key);
                }
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
                result = getZusi3Datenpfad();
                if (result.empty()) {
                    return "";
                }
            }

            if (result.back() == '\\' && zusiPfad.front() == '\\') {
                result.pop_back();
            } else if (result.back() != '\\' && zusiPfad.front() != '\\') {
                result.push_back('\\');
            }

            result += zusiPfad;
            return result;
        }

    }

}
