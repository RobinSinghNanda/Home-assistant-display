#include "ButtonsRowCanvas.hpp"
#include "LovelaceConfig/DefaultRowConfig.hpp"

ButtonsRowCanvas::ButtonsRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, ButtonsRowConfig * rowConfig, LcdTheme * theme) :
    BaseRowCanvas(canvas, id, hass, rowConfig, theme){
    this->rowConfig = rowConfig;
    this->children.clear();
    buttons  = (ImageCanvas **) malloc(sizeof(ImageCanvas *)*rowConfig->getNumButtons());
    uint16_t buttonWidth = this->getWidth()/rowConfig->getNumButtons();
    for (uint16_t i=0;i<rowConfig->getNumButtons();i++) {
        buttons[i] = new ImageCanvas(this, i);
        buttons[i]->setX(buttonWidth*i);
        buttons[i]->setWidth(buttonWidth);
        buttons[i]->setHAlign(ALIGN_CENTER);
        buttons[i]->setVAlign(ALIGN_MIDDLE);
        buttons[i]->setFgColor(BUTTONS_PRIMARY_COLOR);
    }
    for (uint8_t buttonIndex =0;buttonIndex<rowConfig->getNumButtons();buttonIndex++) {
        DefaultRowConfig * buttonConfig = (DefaultRowConfig *) rowConfig->getButton(buttonIndex);
        Entity * entity = hass->getEntity(buttonConfig->getEntityId());
        if (entity) {
            this->buttons[buttonIndex]->onTouch([entity, this](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
                if (isEventLosely(event, TouchActionTapped)) {
                    entity->setState(true);
                    this->hass->callStateService(entity);
                    return true;
                }
                return false;
            }); 
        }
    }
}

ButtonsRowCanvas::~ButtonsRowCanvas () {
    free(buttons);
}

void ButtonsRowCanvas::setIconPath(String iconPath, uint16_t index) {
    if (index<rowConfig->getNumButtons())
        this->buttons[index]->setPath(iconPath);
}

String ButtonsRowCanvas::getIconPath(uint16_t index) {
    if (index<rowConfig->getNumButtons())
        return this->buttons[index]->getPath();
    return "";
}

void ButtonsRowCanvas::setWidth(uint16_t width) {
    returnIfEqual(this->width, width);
    for (uint8_t i=0;i<rowConfig->getNumButtons();i++) {
        this->buttons[i]->setX(this->getWidth()*i/rowConfig->getNumButtons());
        this->buttons[i]->setWidth(this->width/this->rowConfig->getNumButtons());
    }
}

void ButtonsRowCanvas::setFgColor(Color16Bit fgColor) {
    for (uint8_t i=0;i<rowConfig->getNumButtons();i++) {
        this->buttons[i]->setFgColor(theme->colorEntityIcon.get16Bit());
    }
}

void ButtonsRowCanvas::update() {
    for (uint8_t buttonIndex = 0;buttonIndex<rowConfig->getNumButtons();buttonIndex++) {
        DefaultRowConfig * buttonConfig = (DefaultRowConfig *) rowConfig->getButton(buttonIndex);
        Entity * entity = hass->getEntity(buttonConfig->getEntityId());
        this->buttons[buttonIndex]->setPath(entity->getIcon());
    }
}