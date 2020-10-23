#include "HorizontalStackCardConfig.hpp"

HorizontalStackCardConfig::HorizontalStackCardConfig() : BaseCardConfig(PAGE_TYPE_HORIZONTAL_STACK) {

}

HorizontalStackCardConfig::HorizontalStackCardConfig(const char * title, const char * icon) : BaseCardConfig(PAGE_TYPE_HORIZONTAL_STACK, title, icon) {

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