#include "VerticalStackCardConfig.hpp"

VerticalStackCardConfig::VerticalStackCardConfig() :
    VerticalStackCardConfig("", "") {

}

VerticalStackCardConfig::VerticalStackCardConfig(const char * title, const char * icon) :
    BaseCardConfig(TYPE, title, icon) {
}


void VerticalStackCardConfig::addCard(BaseCardConfig * card) {
    this->cards.insert(this->cards.end(), card);
}

uint16_t VerticalStackCardConfig::getNumCards() {
    return this->cards.size();
}

BaseCardConfig * VerticalStackCardConfig::getCard(uint16_t index) {
    return this->cards[index];
}