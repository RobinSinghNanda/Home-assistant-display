#include "ButtonsRowsConfig.hpp"
#include "Arduino.h"

ButtonsRowConfig::ButtonsRowConfig() : BaseRowConfig(ENTITES_ROW_TYPE_BUTTONS) {
}

void ButtonsRowConfig::addButton(BaseRowConfig * rowConfig) {
    if (buttons.size() <5) {
    buttons.insert(buttons.end() , rowConfig);
    } else {
    Serial.println("Max 5 buttons allowed per card");
    }
}

uint16_t ButtonsRowConfig::getNumButtons() {
    return buttons.size();
} 

BaseRowConfig * ButtonsRowConfig::getButton(uint16_t index) {
    return this->buttons[index];
}