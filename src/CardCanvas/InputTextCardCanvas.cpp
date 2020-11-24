#include "InputTextCardCanvas.hpp"
#include "Fonts.hpp"

InputTextCardCanvas::InputTextCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, InputTextCardConfig * cardConfig, LcdTheme * theme) :
    CardCanvas(canvas, id, hass, cardConfig, theme) {
    this->entity = hass->getEntity<InputText>(cardConfig->getEntityId());
    if (entity) {
        this->cardConfig = cardConfig;
        entityRowCanvas = new InputTextRowCanvas(this,
            0,
            hass,
            new DefaultRowConfig(cardConfig->getEntityId(),
                cardConfig->getTitle(),
                cardConfig->getRowIcon(),
                0,
                0),
            theme
            );
        textCanvas = (TextCanvas *) entityRowCanvas->get(2);
        textCanvas->setBorderColor(theme->colorSwitchSecondary);
        keyboardCanvas = new KeyboardCanvas(this, 2, textCanvas);
        keyboardCanvas->setY(y+40);
        keyboardCanvas->onStateChange([this](KeyboardCanvas*, const char * text)->bool{
                entity->setValue(text);
                this->hass->callService(entity, InputText::SERVICE_SET_VALUE);
                return true;
            });
    }
}

InputTextCardCanvas::~InputTextCardCanvas() {
}

void InputTextCardCanvas::update() {
    entityRowCanvas->update();
    keyboardCanvas->loop();
    if (entity) {
        keyboardCanvas->setPasswordMode(entity->isMode(InputText::MODE_PASSWORD));
        if (savedText.compare(entity->getValue()) != 0) {
            savedText = entity->getValue();
            keyboardCanvas->setText(savedText.c_str());
        }
    }
    keyboardCanvas->resetLayout();
}