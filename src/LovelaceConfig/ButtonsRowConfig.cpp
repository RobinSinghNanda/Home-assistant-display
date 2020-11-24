#include "ButtonsRowConfig.hpp"
#include "Log.hpp"

ButtonsRowConfig::ButtonsRowConfig() : BaseRowConfig(TYPE) {
}

void ButtonsRowConfig::addButton(BaseRowConfig * rowConfig) {
    if (buttons.size() < MAX_BUTTONS) {
        buttons.insert(buttons.end() , rowConfig);
    } else {
        Log::log(LOG_LEVEL_INFO, D_LOG_LCD "Max %d buttons allowed per card\n", MAX_BUTTONS);
    }
}

uint16_t ButtonsRowConfig::getNumButtons() {
    return buttons.size();
} 

BaseRowConfig * ButtonsRowConfig::getButton(uint16_t index) {
    return this->buttons[index];
}