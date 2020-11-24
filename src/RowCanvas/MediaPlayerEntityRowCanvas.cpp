#include "MediaPlayerEntityRowCanvas.hpp"
#include "LovelaceConfig/MediaControlCardConfig.hpp"
using namespace std::placeholders;
MediaPlayerEntityRowCanvas::MediaPlayerEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme) :
    BaseEntityRowCanvas(canvas, id, hass, rowConfig, theme) {
    rowConfig = this->rowConfig;
    entity = hass->getEntity<MediaPlayer>(rowConfig->getEntityId());
    if (entity) {
        stateCanvas = new IconCanvas(this, 2);
        stateCanvas->setWidth(34);
        stateCanvas->setPaddingRight(10);
        stateCanvas->hAlignRight();
        stateCanvas->setVAlign(ALIGN_MIDDLE);
        stateCanvas->setHAlign(ALIGN_RIGHT);
        stateCanvas->setPath(ICON_POWER);
        nameCanvas->hAlignBetween(iconCanvas, stateCanvas);
        auto touchCallback = [this](Canvas*, TouchEvent event, TouchEventData)->bool{
            if (isEvent(event, TouchActionTapped)) {
                return entityCallback(new MediaControlCardConfig(entity->getEntityId(), this->rowConfig->getName(),
                    ICON_CHEVRON_LEFT, this->rowConfig->getIcon()));
            }
            return false;
        };
        this->nameCanvas->onTouch(touchCallback);
        this->iconCanvas->onTouch(touchCallback);
        stateCanvas->onTouch([this](Canvas *, TouchEvent event, TouchEventData)->bool{
                if (isEvent(event, TouchActionTapped)) {
                    if (entity->isFeatureSupported(MediaPlayer::SUPPORT_TURN_ON) && entity->isOff()) {
                        this->hass->callService(entity, MediaPlayer::SERVICE_TURN_ON);
                        return true;
                    } else if (entity->isFeatureSupported(MediaPlayer::SUPPORT_TURN_OFF) && !entity->isOff()) {
                        this->hass->callService(entity, MediaPlayer::SERVICE_TURN_OFF);
                        return true;
                    }
                }
                return false;
            });
    }
}

void MediaPlayerEntityRowCanvas::update() {
    BaseEntityRowCanvas::update();
    if (entity) {
        if (entity->isUnavailable()) {
            stateCanvas->setFgColor(theme->colorEntityIconUnavailable);
        } else {
            stateCanvas->setFgColor(theme->colorBodyText);
        }
    }
}