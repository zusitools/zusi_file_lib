#include <algorithm>
#include <windows.h>
#include <winerror.h>

namespace zusi_file_lib {

    namespace pfade {

        using namespace std;

        string getZusi3Datenpfad() {
            static string zusi3Datenpfad = string();

            if (zusi3Datenpfad.empty()) {
                HKEY key;
                char buffer[MAX_PATH];
                DWORD len = MAX_PATH;
                if (SUCCEEDED(RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Zusi3", 0, KEY_READ, &key)) ||
                        SUCCEEDED(RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Wow6432Node\\Zusi3", 0, KEY_READ, &key)) ||
                        SUCCEEDED(RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Zusi3", 0, KEY_READ, &key)) ||
                        SUCCEEDED(RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Wow6432Node\\Zusi3", 0, KEY_READ, &key))) {
                    // TODO: use RegQueryValueEx and return wstring?
                    if (SUCCEEDED(RegGetValueA(key, nullptr, "DatenVerzeichnis", RRF_RT_REG_SZ, nullptr, (LPBYTE)buffer, &len))) {
                        zusi3Datenpfad = string(buffer, len - 1); // subtract terminating null character
                        RegCloseKey(key);
                    }
                }
            }

            return zusi3Datenpfad;
        }

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
                string datenpfad = getZusi3Datenpfad();
                if (datenpfad.empty()) {
                    return "";
                }
                result = string(datenpfad);
            }

            if (result.back() == '\\' && zusiPfad.front() == '\\') {
                result.pop_back();
            } else if (result.back() != '\\' && zusiPfad.front() != '\\') {
                result.push_back('\\');
            }

            return result + zusiPfad;
        }

    }

}
