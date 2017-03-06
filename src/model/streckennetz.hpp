#ifndef SRC_MODEL_STRECKENNETZ_HPP
#define SRC_MODEL_STRECKENNETZ_HPP

#include <memory>
#include <vector>

#include <model/strecke.hpp>

#ifdef ZUSI_FILE_LIB_NOINLINE
#define _ZUSI_FILE_LIB_INLINE
#else
#define _ZUSI_FILE_LIB_INLINE inline
#endif

class Streckennetz {
private:
    // TODO: map nach Dateiname
    std::vector<std::unique_ptr<Strecke>> strecken_;

public:
    Streckennetz() : strecken_() {}
    Streckennetz(const Streckennetz& other) = delete;

    void fuegeStreckeHinzu(std::unique_ptr<Strecke>& strecke);
    void verknuepfeStrecken();

    const std::vector<std::unique_ptr<Strecke>>& strecken() const { return strecken_; }
};

#undef _ZUSI_FILE_LIB_INLINE

#ifndef ZUSI_FILE_LIB_NOINLINE
#include "streckennetz.cpp"
#endif

#endif
