#include "MediaControlCardCanvas.hpp"
#include "LovelaceConfig/DefaultRowConfig.hpp"
#include "RtcTime.hpp"
#include "Fonts.hpp"

MediaControlCardCanvas::MediaControlCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, MediaControlCardConfig * cardConfig, LcdTheme * theme) :
    CardCanvas(canvas, id, hass, cardConfig, theme) {
    this->cardConfig = cardConfig;
    this->entity = hass->getEntity<MediaPlayer>(cardConfig->getEntityId());
    if (entity) {
        rowConfig = new DefaultRowConfig(cardConfig->getEntityId(),
                cardConfig->getTitle(),
                cardConfig->getRowIcon(),
                true,
                false);
        entityCanvas = new MediaPlayerEntityRowCanvas(this,
            0,
            hass,
            rowConfig,
            theme);

        //Create new canvas objects
        mediaTitleCanvas = new TextCanvas(this, 1);
        seekSliderCanvas = new SliderCanvas(this, 2);
        repeatButtonCanvas = new ImageCanvas(this, 3);
        prevButtonCanvas = new ImageCanvas(this, 4);
        playPauseButtonCanvas = new ImageCanvas(this, 5);
        nextButtonCanvas = new ImageCanvas(this, 6);
        shuffleButtonCanvas = new ImageCanvas(this, 6);
        volumeSliderCanvas = new SliderCanvas(this, 7);
        muteButtonCanvas = new ImageCanvas(this, 8);
        volumeIncButtonCanvas = new ImageCanvas(this, 9);
        volumeDecButtonCanvas = new ImageCanvas(this, 10);

        uint16_t prevY = entityCanvas->getBottomY();
        //arrage mediaNameCanvas
        mediaTitleCanvas->setY(prevY);
        mediaTitleCanvas->setHeight(entityCanvas->getHeight());
        mediaTitleCanvas->setText("");
        mediaTitleCanvas->setHAlign(ALIGN_CENTER);
        mediaTitleCanvas->setVAlign(ALIGN_MIDDLE);
        mediaTitleCanvas->setMarginLeft(10);
        mediaTitleCanvas->setMarginRight(10);
        mediaTitleCanvas->setFont(FONT_REGULAR_24);
        mediaTitleCanvas->setVisible(false);
        prevY += entityCanvas->getHeight();

        //arrange seekSliderCanvas
        seekSliderCanvas->setY(prevY);
        seekSliderCanvas->setHeight(entityCanvas->getHeight());
        seekSliderCanvas->setMin(0);
        seekSliderCanvas->setMax(100);
        seekSliderCanvas->setStep(10);
        seekSliderCanvas->setHighlightValue(true);
        prevY += entityCanvas->getHeight();
        seekSliderCanvas->setVisible(false);

        //arrange mediabuttons
        uint8_t widthBy5 = this->width/5;
        repeatButtonCanvas->setY(prevY);
        repeatButtonCanvas->setHeight(entityCanvas->getHeight());
        repeatButtonCanvas->setHAlign(ALIGN_CENTER);
        repeatButtonCanvas->setVAlign(ALIGN_MIDDLE);
        repeatButtonCanvas->setPath(ICON_REPEAT);
        repeatButtonCanvas->setWidth(widthBy5);
        repeatButtonCanvas->setVisible(false);
        prevButtonCanvas->setY(prevY);
        prevButtonCanvas->setHeight(entityCanvas->getHeight());
        prevButtonCanvas->setHAlign(ALIGN_CENTER);
        prevButtonCanvas->setVAlign(ALIGN_MIDDLE);
        prevButtonCanvas->setPath(ICON_SKIP_PREVIOUS);
        prevButtonCanvas->setWidth(widthBy5);
        prevButtonCanvas->setX(widthBy5);
        prevButtonCanvas->setVisible(false);
        playPauseButtonCanvas->setY(prevY);
        playPauseButtonCanvas->setHeight(entityCanvas->getHeight());
        playPauseButtonCanvas->setHAlign(ALIGN_CENTER);
        playPauseButtonCanvas->setVAlign(ALIGN_MIDDLE);
        playPauseButtonCanvas->setPath(ICON_PLAY_CIRCLE_48);
        playPauseButtonCanvas->setWidth(widthBy5);
        playPauseButtonCanvas->setX(2*widthBy5);
        playPauseButtonCanvas->setVisible(false);
        nextButtonCanvas->setY(prevY);
        nextButtonCanvas->setHeight(entityCanvas->getHeight());
        nextButtonCanvas->setHAlign(ALIGN_CENTER);
        nextButtonCanvas->setVAlign(ALIGN_MIDDLE);
        nextButtonCanvas->setPath(ICON_SKIP_NEXT);
        nextButtonCanvas->setWidth(widthBy5);
        nextButtonCanvas->setX(3*widthBy5);
        nextButtonCanvas->setVisible(false);
        shuffleButtonCanvas->setY(prevY);
        shuffleButtonCanvas->setHeight(entityCanvas->getHeight());
        shuffleButtonCanvas->setHAlign(ALIGN_CENTER);
        shuffleButtonCanvas->setVAlign(ALIGN_MIDDLE);
        shuffleButtonCanvas->setPath(ICON_SHUFFLE);
        shuffleButtonCanvas->setWidth(widthBy5);
        shuffleButtonCanvas->setX(4*widthBy5);
        shuffleButtonCanvas->setVisible(false);
        prevY += entityCanvas->getHeight();

        //arrnage mute button
        muteButtonCanvas->setY(prevY);
        muteButtonCanvas->setHeight(entityCanvas->getHeight());
        muteButtonCanvas->setWidth(40);
        muteButtonCanvas->setPath(ICON_VOLUME_HIGH);
        muteButtonCanvas->setHAlign(ALIGN_CENTER);
        muteButtonCanvas->setVAlign(ALIGN_MIDDLE);
        muteButtonCanvas->setVisible(false);
        //arrange volume slider
        volumeSliderCanvas->setY(prevY);
        volumeSliderCanvas->setHeight(entityCanvas->getHeight());
        volumeSliderCanvas->setMin(0);
        volumeSliderCanvas->setMax(100);
        volumeSliderCanvas->setStep(10);
        volumeDecButtonCanvas->setY(prevY);
        volumeDecButtonCanvas->setHeight(entityCanvas->getHeight());
        volumeDecButtonCanvas->setWidth(40);
        volumeDecButtonCanvas->setHAlign(ALIGN_CENTER);
        volumeDecButtonCanvas->setVAlign(ALIGN_MIDDLE);
        volumeIncButtonCanvas->setY(prevY);
        volumeIncButtonCanvas->setHeight(entityCanvas->getHeight());
        volumeIncButtonCanvas->setWidth(40);
        volumeIncButtonCanvas->setHAlign(ALIGN_CENTER);
        volumeIncButtonCanvas->setVAlign(ALIGN_MIDDLE);
        volumeIncButtonCanvas->setX(volumeDecButtonCanvas->getRightX());
        volumeDecButtonCanvas->setPath(ICON_VOLUME_MINUS);
        volumeIncButtonCanvas->setPath(ICON_VOLUME_PLUS);
        volumeSliderCanvas->setVisible(false);
        volumeIncButtonCanvas->setVisible(false);
        volumeDecButtonCanvas->setVisible(false);
        setLayout();

        seekSliderCanvas->onValueChange([this](SliderCanvas*, int16_t value, int16_t)->bool{
                entity->setMediaPosition(((float)value)/100.0);
                this->hass->callService(entity, MediaPlayer::SERVICE_MEDIA_SEEK);
                return true;
            });
        volumeSliderCanvas->onValueChange([this](SliderCanvas*, int16_t value, int16_t)->bool{
                bool prevMute = entity->isVolumeMuted();
                if (value) {
                    setMute(false);
                } else {
                    setMute(true);
                }
                if (prevMute != entity->isVolumeMuted()) {
                    this->hass->callService(entity, MediaPlayer::SERVICE_VOLUME_MUTE);
                }
                entity->setVolumeLevel(value);
                this->hass->callService(entity, MediaPlayer::SERVICE_VOLUME_SET);
                return true;
            });
        playPauseButtonCanvas->onTouch([this](Canvas*, TouchEvent event, TouchEventData)->bool{
                if (isEventLosely(event, TouchEvent::TouchActionTapped)) {
                    if (entity->isPlaying()) {
                        setPlaying(false);
                        this->hass->callService(entity, MediaPlayer::SERVICE_MEDIA_PAUSE);
                    } else {
                        setPlaying(true);
                        this->hass->callService(entity, MediaPlayer::SERVICE_MEDIA_PLAY);
                    }
                    return true;
                }
                return false;
            });
        nextButtonCanvas->onTouch([this](Canvas*, TouchEvent event, TouchEventData)->bool{
                if (isEventLosely(event, TouchEvent::TouchActionTapped)) {
                    this->hass->callService(entity, MediaPlayer::SERVICE_MEDIA_NEXT_TRACK);
                    return true;
                }
                return false;
            });
        prevButtonCanvas->onTouch([this](Canvas*, TouchEvent event, TouchEventData)->bool{
                if (isEventLosely(event, TouchEvent::TouchActionTapped)) {
                    this->hass->callService(entity, MediaPlayer::SERVICE_MEDIA_PREVIOUS_TRACK);
                    return true;
                }
                return false;
            });
        muteButtonCanvas->onTouch([this](Canvas*, TouchEvent event, TouchEventData)->bool{
                if (isEventLosely(event, TouchEvent::TouchActionTapped)) {
                    entity->setVolumeMute(!entity->isVolumeMuted());
                    setMute(entity->isVolumeMuted());
                    this->hass->callService(entity, MediaPlayer::SERVICE_VOLUME_MUTE);
                    if(!entity->isVolumeMuted() && entity->getVolumeLevel() == 0) {
                        entity->setVolumeLevel(0.5);
                        this->hass->callService(entity, MediaPlayer::SERVICE_VOLUME_SET);
                    } 
                    return true;
                }
                return false;
            });
    }
}

MediaControlCardCanvas::~MediaControlCardCanvas() {
    delete rowConfig;
}

void MediaControlCardCanvas::setLayout() {
    if (entity->isFeatureSupported(MediaPlayer::SUPPORT_VOLUME_SET)) {
        if (entity->isFeatureSupported(MediaPlayer::SUPPORT_VOLUME_MUTE)) {
            volumeSliderCanvas->setX(muteButtonCanvas->getRightX());
            volumeSliderCanvas->setMarginLeft(0);
        }
    } else if (entity->isFeatureSupported(MediaPlayer::SUPPORT_VOLUME_STEP)) {
        if (entity->isFeatureSupported(MediaPlayer::SUPPORT_VOLUME_MUTE)) {
            volumeDecButtonCanvas->setX(muteButtonCanvas->getRightX());
        }
        volumeIncButtonCanvas->setX(volumeDecButtonCanvas->getRightX());
    }
}

void MediaControlCardCanvas::setPlaying(bool playing) {
    if (playing) {
        this->playPauseButtonCanvas->setPath(ICON_PAUSE_CIRCLE_48);
    } else {
        this->playPauseButtonCanvas->setPath(ICON_PLAY_CIRCLE_48);
    }
}

void MediaControlCardCanvas::setMute(bool mute) {
    if (entity->isVolumeMuted() != mute) {
        entity->setVolumeMute(mute);
        if (mute) {
            this->muteButtonCanvas->setPath(ICON_VOLUME_OFF);
        } else {
            this->muteButtonCanvas->setPath(ICON_VOLUME_HIGH);
        }
    }
}

void MediaControlCardCanvas::update() {
    this->entityCanvas->update();
    if (entity->isPlaying()) {
        this->setPlaying(true);
    } else {
        this->setPlaying(false);
    }
    if (entity->isPlaying() || entity->isPaused()) {
        if (entity->isFeatureSupported(MediaPlayer::SUPPORT_VOLUME_SET)) {
            if (entity->isFeatureSupported(MediaPlayer::SUPPORT_VOLUME_MUTE)) {
                if (entity->isVolumeMuted()) {
                    this->volumeSliderCanvas->setValue(0);
                    this->setMute(entity->isVolumeMuted());
                } else {
                    if (entity->getVolumeLevel() == 0) {
                        this->setMute(true);
                    } else {
                        this->setMute(false);
                        this->volumeSliderCanvas->setValue(entity->getVolumeLevel()*100);
                    }
                }
            
            } else {
                this->volumeSliderCanvas->setValue(entity->getVolumeLevel()*100);
            }
        } else {
            this->setMute(entity->isVolumeMuted());
        }
        seekSliderCanvas->setVisible(true);
        this->seekSliderCanvas->setMax(entity->getMediaDuration()*100);
        this->seekSliderCanvas->setValue(entity->getMediaPosition(UtcTime())*100);
        if (entity->isFeatureSupported(MediaPlayer::SUPPORT_SEEK)) {
            this->seekSliderCanvas->setViewOnly(false);
        } else {
            this->seekSliderCanvas->setViewOnly(true);
        }
        if (entity->isFeatureSupported(MediaPlayer::SUPPORT_VOLUME_MUTE)) {
            this->muteButtonCanvas->setVisible(true);
        }
        if (entity->isFeatureSupported(MediaPlayer::SUPPORT_VOLUME_SET)) {
            this->volumeSliderCanvas->setVisible(true);
        } else if (entity->isFeatureSupported(MediaPlayer::SUPPORT_VOLUME_STEP)) {
            this->volumeIncButtonCanvas->setVisible(true);
            this->volumeDecButtonCanvas->setVisible(true);
        }
        if (entity->isFeatureSupported(MediaPlayer::SUPPORT_REPEAT_SET)) {
            this->repeatButtonCanvas->setVisible(true);
        }
        if (entity->isFeatureSupported(MediaPlayer::SUPPORT_SHUFFLE_SET)) {
            this->shuffleButtonCanvas->setVisible(true);
        }
        if (entity->isFeatureSupported(MediaPlayer::SUPPORT_PREVIOUS_TRACK)) {
            this->prevButtonCanvas->setVisible(true);
        }
        if (entity->isFeatureSupported(MediaPlayer::SUPPORT_NEXT_TRACK)) {
            this->nextButtonCanvas->setVisible(true);
        }
        if (entity->isFeatureSupported(MediaPlayer::SUPPORT_PLAY) && entity->isFeatureSupported(MediaPlayer::SUPPORT_PAUSE)) {
            this->playPauseButtonCanvas->setVisible(true);
        }
        this->mediaTitleCanvas->setVisible(true);
        this->mediaTitleCanvas->setText(entity->getMediaTitle());
        if (mediaTitleCanvas->getText().length() > 40) {
            this->mediaTitleCanvas->setFont(FONT_REGULAR_12);
        } else {
            this->mediaTitleCanvas->setFont(FONT_REGULAR_24);
        }
    } else {
        this->seekSliderCanvas->setVisible(false);
        this->muteButtonCanvas->setVisible(false);
        this->volumeSliderCanvas->setVisible(false);
        this->volumeDecButtonCanvas->setVisible(false);
        this->volumeIncButtonCanvas->setVisible(false);
        this->repeatButtonCanvas->setVisible(false);
        this->shuffleButtonCanvas->setVisible(false);
        this->prevButtonCanvas->setVisible(false);
        this->nextButtonCanvas->setVisible(false);
        this->playPauseButtonCanvas->setVisible(false);
        this->mediaTitleCanvas->setVisible(false);
    }
  this->playPauseButtonCanvas->setFgColor(theme->colorEntityIcon.get16Bit());
  setLayout();
}