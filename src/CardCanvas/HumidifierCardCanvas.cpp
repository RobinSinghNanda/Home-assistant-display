#include "HumidifierCardCanvas.hpp"

HumidifierCardCanvas::HumidifierCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, HumidifierCardConfig * cardConfig, LcdTheme * theme) :
    CardCanvas(canvas, id, hass, cardConfig, theme) {
    this->entity = hass->getEntity<Humidifier>(cardConfig->getEntityId());
    this->cardConfig = cardConfig;
    if (entity) {
        entityConfig = new DefaultRowConfig(cardConfig->getEntityId(),
                cardConfig->getName(), 
                cardConfig->getRowIcon(),
                cardConfig->getStateColor(),
                false);
        entityCanvas = new ToggleEntityRowCanvas(this, 0, hass, entityConfig, theme);

        //Fan modes
        modeIconCanvas = new IconCanvas(this, 1);
        modeTextCanvas = new TextCanvas(this, 2);
        modeSelectCanvas = new TextSelectCanvas(this, 3);
        modeIconCanvas->setWidth(40);
        modeIconCanvas->setHeight(40);
        modeIconCanvas->vAlignBottom(entityCanvas);
        modeIconCanvas->setVAlign(ALIGN_MIDDLE);
        modeIconCanvas->setHAlign(ALIGN_CENTER);
        modeIconCanvas->setPath(ICON_FAN);
        modeIconCanvas->setFgColor(theme->colorEntityIcon);

        modeTextCanvas->setWidth(80);
        modeTextCanvas->setHeight(40);
        modeTextCanvas->setVAlign(ALIGN_MIDDLE);
        modeTextCanvas->setHAlign(ALIGN_LEFT);
        modeTextCanvas->setFont(FONT_REGULAR_24);
        modeTextCanvas->setText("Fan mode");
        modeTextCanvas->vAlignBottom(entityCanvas);
        modeTextCanvas->hAlignRight(modeIconCanvas);
        modeTextCanvas->resize();
        
        modeSelectCanvas->setWidth(100);
        modeSelectCanvas->setHeight(40);
        modeSelectCanvas->setHAlign(ALIGN_RIGHT);
        modeSelectCanvas->setVAlign(ALIGN_MIDDLE);
        modeSelectCanvas->vAlignBottom(entityCanvas);
        modeSelectCanvas->setFont(FONT_REGULAR_24);
        modeSelectCanvas->setWidth(this->getRightX() - modeTextCanvas->getRightX());
        modeSelectCanvas->setX(modeTextCanvas->getRightX());
        modeSelectCanvas->resetLayout();        

        humidityIconCanvas = new IconCanvas(this, 1);
        humiditySliderCanvas = new SliderCanvas(this, 2);
        humidityTextCanvas = new TextCanvas(this, 3);
        humidityIconCanvas->setWidth(40);
        humidityIconCanvas->setHeight(40);
        humidityIconCanvas->setVAlign(ALIGN_MIDDLE);
        humidityIconCanvas->setHAlign(ALIGN_CENTER);
        humidityIconCanvas->setPath(ICON_THERMOMETER);
        humidityIconCanvas->vAlignBottom(modeIconCanvas);
        humidityIconCanvas->setFgColor(theme->colorEntityIcon);
        humiditySliderCanvas->setHMargin(0);
        humiditySliderCanvas->vAlignBottom(modeIconCanvas);
        humiditySliderCanvas->setHeight(40);
        humiditySliderCanvas->setMin(0);
        humiditySliderCanvas->setMax(100);
        humiditySliderCanvas->setStep(10);
        humiditySliderCanvas->setHighlightValue(true);
        humidityTextCanvas->vAlignBottom(modeIconCanvas);
        humidityTextCanvas->setVAlign(ALIGN_MIDDLE);
        humidityTextCanvas->setHAlign(ALIGN_RIGHT);
        humidityTextCanvas->setPaddingRight(10);
        humidityTextCanvas->setFont(FONT_REGULAR_24);
        humidityTextCanvas->setWidth(80);
        humidityTextCanvas->hAlignRight();
        humidityTextCanvas->setText("°C");
        humidityTextCanvas->setHeight(40);
        humiditySliderCanvas->hAlignBetween(humidityIconCanvas, humidityTextCanvas);

        modeSelectCanvas->onStateChange([this](SelectCanvas*, SelectCanvasStateChangeEvent event, const char * &value)->bool {       
                uint8_t index = entity->getModeIndex();
                if (event == CHANGE_NEXT) {    
                    index++;
                    if (index == entity->getNumModes()) {
                        index = 0;
                    }
                } else {
                    if (index == 0) {
                        index = entity->getNumModes() - 1;
                    } else {
                        index--;
                    }
                }
                entity->setMode(index);
                entity->setState(entity->getMode());
                this->hass->callService(entity, Humidifier::SERVICE_SET_MODE);
                return true;
            });

        humiditySliderCanvas->onValueChange([this](SliderCanvas*, int32_t value, int32_t)->void{
                float humidity = value;
                entity->setHumidity(humidity);
                this->hass->callService(entity, Humidifier::SERVICE_SET_HUMIDITY);
            });
    }
}

HumidifierCardCanvas::~HumidifierCardCanvas() {
    if (entityConfig) {
        delete entityConfig;
    }
}

void HumidifierCardCanvas::update() {
    CardCanvas::update();
    entityCanvas->update();
    if (entity) {
        modeSelectCanvas->setValue(entity->getMode());
        char str[15];
        snprintf(str, sizeof(str)-1, "%0.1f", entity->getHumidity());
        humidityTextCanvas->setText(str);
        humiditySliderCanvas->setMin(entity->getMinHumidity());
        humiditySliderCanvas->setMax(entity->getMaxHumidity());
        humiditySliderCanvas->setStep(1);
        humiditySliderCanvas->setValue(entity->getHumidity());

        resetLayout();
    }
}

void HumidifierCardCanvas::resetLayout() {
    if (entity) {
        Canvas * rowCanvas = entityCanvas;
        if (entity->isAvailable() && !entity->isOff() && entity->isFeatureSupported(Humidifier::SUPPORT_MODES)) {
            modeIconCanvas->vAlignBottom(rowCanvas);
            modeTextCanvas->vAlignBottom(rowCanvas);
            modeSelectCanvas->vAlignBottom(rowCanvas);
            modeIconCanvas->setVisible(true);
            modeTextCanvas->setVisible(true);
            modeSelectCanvas->setVisible(true);
            modeSelectCanvas->resetLayout();
            rowCanvas = modeIconCanvas;
        }

        humidityIconCanvas->vAlignBottom(rowCanvas);
        humidityTextCanvas->vAlignBottom(rowCanvas);
        humiditySliderCanvas->vAlignBottom(rowCanvas);
        humidityIconCanvas->setVisible(true);
        humiditySliderCanvas->setVisible(true);
        humidityTextCanvas->setVisible(true);
        
        if (!entity->isAvailable() || entity->isOff() || !entity->isFeatureSupported(Humidifier::SUPPORT_MODES)) {
            modeIconCanvas->vAlignBottom(rowCanvas);
            modeSelectCanvas->vAlignBottom(rowCanvas);
            modeTextCanvas->vAlignBottom(rowCanvas);
            modeIconCanvas->setVisible(false);
            modeSelectCanvas->setVisible(false);
            modeTextCanvas->setVisible(false);
        }
    }
}