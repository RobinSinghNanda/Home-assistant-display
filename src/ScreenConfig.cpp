#include "ScreenConfig.hpp"

void ScreenConfig::addCard(BaseCardConfig * baseCardConfig) {
    if (cards.size() <5) {
        cards.insert(cards.end() , baseCardConfig);
    } else {
        Serial.println("Max 5 cards are allowed");
    }
}
BaseCardConfig * ScreenConfig::getCard(uint16_t index) {
    return cards[index];
}
uint16_t ScreenConfig::getNumCards() {
    return cards.size();
}
void ScreenConfig::clearCards () {
    for (auto card:cards) {
    delete card;
    }
    this->cards.clear();
}