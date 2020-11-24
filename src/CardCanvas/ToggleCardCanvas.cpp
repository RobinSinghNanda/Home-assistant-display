#include "ToggleCardCanvas.hpp"
#include "LovelaceConfig/DefaultRowConfig.hpp"

ToggleCardCanvas::ToggleCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, BaseEntityCardConfig * cardConfig, LcdTheme * theme) :
    CardCanvas(canvas, id, hass, cardConfig, theme) {
    this->cardConfig = cardConfig;
    entity = hass->getEntity(cardConfig->getEntityId());
    rowConfig = new DefaultRowConfig(entity->getEntityId(),
                    cardConfig->getTitle(),
                    cardConfig->getRowIcon(),
                    cardConfig->getStateColor(),
                    false);
    if (entity) {
        entityCanvas = new ToggleEntityRowCanvas(this, 
            0,
            hass,
            rowConfig,
            theme);
    }
}

ToggleCardCanvas::~ToggleCardCanvas() {
    delete rowConfig;
}

const char * ToggleCardCanvas::getCardTitle() {
    if (cardConfig->getTitle()[0] == '\0') {
        return entity->getFriendlyName();
    } else {
        return cardConfig->getTitle();
    }
}

const char * ToggleCardCanvas::getCardIcon() {
    if (cardConfig->getIcon()[0] == '\0') {
        return entity->getIcon();
    } else {
        return cardConfig->getIcon();
    }
}

void ToggleCardCanvas::update() {
    if (entity) {
        this->entityCanvas->update();
    }
}