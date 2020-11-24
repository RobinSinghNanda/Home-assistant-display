#include "ToggleEntityRowCanvas.hpp"
using namespace std::placeholders;
ToggleEntityRowCanvas::ToggleEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme) :
    BaseEntityRowCanvas(canvas, id, hass, rowConfig, theme){
    rowConfig = this->rowConfig;
    stateCanvas = new SwitchCanvas(this, 2);
    stateCanvas->setWidth(SWITCH_ICON_WIDTH + SWICTH_ENTITY_MARGIN_RIGHT + SWICTH_ENTITY_MARGIN_RIGHT);
    stateCanvas->hAlignRight();
    stateCanvas->setVAlign(ALIGN_MIDDLE);
    stateCanvas->setHAlign(ALIGN_CENTER);
    nameCanvas->hAlignBetween(iconCanvas, stateCanvas);
    stateCanvas->setSecondaryColor(theme->colorSwitchSecondary.get16Bit());
    stateCanvas->setSurfaceColor(theme->colorSwitchSurface.get16Bit());
    stateCanvas->setOnSurfaceColor(theme->colorSwitchOnSurface.get16Bit());
    this->stateCanvas->onStateChange([this](SwitchCanvas* canvas, bool state)->bool{
        entity->setState(state);
        this->hass->callStateService(entity);
        return true;
    });
}

void ToggleEntityRowCanvas::update() {
    BaseEntityRowCanvas::update();
    if (entity) {
        if (entity->isUnavailable()) {
            stateCanvas->setDisabled(true);
        } else {
            stateCanvas->setState(!entity->isOff());
        }
    }
}