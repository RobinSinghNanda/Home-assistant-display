#include "CardCanvas.hpp"
#include "LcdTheme.hpp"

CardCanvas::CardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, BaseCardConfig * cardConfig, LcdTheme * theme) : Canvas(canvas, id) {
    this->hass = hass;
    this->cardConfig = cardConfig;
    this->theme = theme;
}

CardCanvas::~CardCanvas() {

}

const char * CardCanvas::getCardTitle() {
    return cardConfig->getTitle();
}

const char * CardCanvas::getCardIcon() {
    return cardConfig->getIcon();
}

void CardCanvas::update() {

}