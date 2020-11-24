#include "ScreenConfig.hpp"
#include "Log.hpp"
void ScreenConfig::addCard(BaseCardConfig * baseCardConfig) {
    if (cards.size() < MAX_NUM_CARDS) {
        cards.insert(cards.end() , baseCardConfig);
    } else {
        Log::log(LOG_LEVEL_INFO, D_LOG_APPLICATION "Max %d cards are allowed. Discarding the rest", MAX_NUM_CARDS);
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