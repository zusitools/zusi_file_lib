#include "streckennetz.hpp"

void Streckennetz::fuegeStreckeHinzu(std::unique_ptr<Strecke>& strecke) {
    this->strecken_.push_back(std::move(strecke));
}

void Streckennetz::verknuepfeStrecken() {

}
