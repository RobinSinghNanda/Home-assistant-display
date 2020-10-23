#include "VerticalStackCardConfig.hpp"

VerticalStackCardConfig::VerticalStackCardConfig() : BaseCardConfig(PAGE_TYPE_VERTICAL_STACK) {

}

VerticalStackCardConfig::VerticalStackCardConfig(const char * title, const char * icon) : BaseCardConfig(PAGE_TYPE_VERTICAL_STACK, title, icon) {

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