#include "HorizontalStackCardConfig.hpp"

HorizontalStackCardConfig::HorizontalStackCardConfig() :
    HorizontalStackCardConfig("", "") {
}

HorizontalStackCardConfig::HorizontalStackCardConfig(const char * title, const char * icon) :
    BaseCardConfig(TYPE, title, icon) {
}


void HorizontalStackCardConfig::addCard(BaseCardConfig * card) {
    this->cards.insert(this->cards.end(), card);
}

uint16_t HorizontalStackCardConfig::getNumCards() {
    return this->cards.size();
}

BaseCardConfig * HorizontalStackCardConfig::getCard(uint16_t index) {
    return this->cards[index];
}