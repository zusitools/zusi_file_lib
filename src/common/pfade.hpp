namespace zusi_file_lib {

    namespace pfade {

        static std::string zusi3Datenpfad = std::string();
        const std::string& getZusi3Datenpfad();

        std::string zusiPfadZuOsPfad(const std::string& zusiPfad, const std::string& osPfadUebergeordnet = std::string());

    }

}

#ifdef _WIN32
#include "pfade_win.hpp"
#else
#include "pfade_unix.hpp"
#endif
