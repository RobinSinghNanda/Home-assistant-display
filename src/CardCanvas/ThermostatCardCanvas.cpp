#include "ThermostatCardCanvas.hpp"

ThermostatCardCanvas::ThermostatCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, ThermostatCardConfig * cardConfig, LcdTheme * theme) :
    CardCanvas(canvas, id, hass, cardConfig, theme) {
    this->entity = hass->getEntity<Climate>(cardConfig->getEntityId());
    this->cardConfig = cardConfig;
    if (entity) {
        iconCanvas  = new IconCanvas(this, 0);
        nameCanvas = new TextCanvas(this, 1);
        selectCanvas = new TextSelectCanvas(this, 2);
        iconCanvas->setWidth(40);
        iconCanvas->setHeight(40);
        iconCanvas->setVAlign(ALIGN_MIDDLE);
        iconCanvas->setHAlign(ALIGN_CENTER);
        iconCanvas->setFgColor(theme->colorEntityIcon);
        
        selectCanvas->setWidth(150);
        selectCanvas->setHeight(40);
        selectCanvas->setHAlign(ALIGN_RIGHT);
        selectCanvas->setVAlign(ALIGN_MIDDLE);
        selectCanvas->hAlignRight();
        selectCanvas->setFont(FONT_REGULAR_24);

        nameCanvas->setWidth(80);
        nameCanvas->setHeight(40);
        nameCanvas->setVAlign(ALIGN_MIDDLE);
        nameCanvas->setHAlign(ALIGN_LEFT);
        nameCanvas->setFont(FONT_REGULAR_24);
        nameCanvas->hAlignBetween(iconCanvas, selectCanvas);


        temperatureIconCanvas = new IconCanvas(this, 1);
        temperatureSliderCanvas = new SliderCanvas(this, 2);
        temperatureTextCanvas = new TextCanvas(this, 3);
        temperatureIconCanvas->setWidth(40);
        temperatureIconCanvas->setHeight(40);
        temperatureIconCanvas->setVAlign(ALIGN_MIDDLE);
        temperatureIconCanvas->setHAlign(ALIGN_CENTER);
        temperatureIconCanvas->setPath(ICON_THERMOMETER);
        temperatureIconCanvas->vAlignBottom(iconCanvas);
        temperatureIconCanvas->setFgColor(theme->colorEntityIcon);
        temperatureSliderCanvas->setHMargin(0);
        temperatureSliderCanvas->vAlignBottom(iconCanvas);
        temperatureSliderCanvas->setHeight(40);
        temperatureSliderCanvas->setMin(0);
        temperatureSliderCanvas->setMax(100);
        temperatureSliderCanvas->setStep(10);
        temperatureSliderCanvas->setHighlightValue(true);
        temperatureTextCanvas->vAlignBottom(iconCanvas);
        temperatureTextCanvas->setVAlign(ALIGN_MIDDLE);
        temperatureTextCanvas->setHAlign(ALIGN_RIGHT);
        temperatureTextCanvas->setPaddingRight(10);
        temperatureTextCanvas->setFont(FONT_REGULAR_24);
        temperatureTextCanvas->setWidth(80);
        temperatureTextCanvas->hAlignRight();
        temperatureTextCanvas->setText("°C");
        temperatureTextCanvas->setHeight(40);
        temperatureSliderCanvas->hAlignBetween(temperatureIconCanvas, temperatureTextCanvas);

        //preset mode 
        presetModeIconCanvas = new IconCanvas(this, 4);
        presetModeTextCanvas = new TextCanvas(this, 5);
        presetModeSelectCanvas = new TextSelectCanvas(this, 6);
        presetModeIconCanvas->setWidth(40);
        presetModeIconCanvas->setHeight(40);
        presetModeIconCanvas->vAlignBottom(temperatureIconCanvas);
        presetModeIconCanvas->setVAlign(ALIGN_MIDDLE);
        presetModeIconCanvas->setHAlign(ALIGN_CENTER);
        presetModeIconCanvas->setPath(ICON_CACHED);
        presetModeIconCanvas->setFgColor(theme->colorEntityIcon);

        presetModeTextCanvas->setWidth(80);
        presetModeTextCanvas->setHeight(40);
        presetModeTextCanvas->setVAlign(ALIGN_MIDDLE);
        presetModeTextCanvas->setHAlign(ALIGN_LEFT);
        presetModeTextCanvas->setFont(FONT_REGULAR_24);
        presetModeTextCanvas->setText("Preset mode");
        presetModeTextCanvas->resize();
        presetModeTextCanvas->vAlignBottom(temperatureIconCanvas);
        presetModeTextCanvas->hAlignRight(presetModeIconCanvas);

        presetModeSelectCanvas->setWidth(100);
        presetModeSelectCanvas->setHeight(40);
        presetModeSelectCanvas->setHAlign(ALIGN_RIGHT);
        presetModeSelectCanvas->setVAlign(ALIGN_MIDDLE);
        presetModeSelectCanvas->vAlignBottom(temperatureIconCanvas);
        presetModeSelectCanvas->setFont(FONT_REGULAR_24);
        presetModeSelectCanvas->setWidth(this->getRightX() - presetModeTextCanvas->getRightX());
        presetModeSelectCanvas->setX(presetModeTextCanvas->getRightX());
        presetModeSelectCanvas->resetLayout();
        
        //Fan modes
        fanModeIconCanvas = new IconCanvas(this, 4);
        fanModeTextCanvas = new TextCanvas(this, 5);
        fanModeSelectCanvas = new TextSelectCanvas(this, 6);
        fanModeIconCanvas->setWidth(40);
        fanModeIconCanvas->setHeight(40);
        fanModeIconCanvas->vAlignBottom(presetModeIconCanvas);
        fanModeIconCanvas->setVAlign(ALIGN_MIDDLE);
        fanModeIconCanvas->setHAlign(ALIGN_CENTER);
        fanModeIconCanvas->setPath(ICON_FAN);
        fanModeIconCanvas->setFgColor(theme->colorEntityIcon);

        fanModeTextCanvas->setWidth(80);
        fanModeTextCanvas->setHeight(40);
        fanModeTextCanvas->setVAlign(ALIGN_MIDDLE);
        fanModeTextCanvas->setHAlign(ALIGN_LEFT);
        fanModeTextCanvas->setFont(FONT_REGULAR_24);
        fanModeTextCanvas->setText("Fan mode");
        fanModeTextCanvas->vAlignBottom(presetModeIconCanvas);
        fanModeTextCanvas->hAlignRight(fanModeIconCanvas);
        fanModeTextCanvas->resize();
        
        fanModeSelectCanvas->setWidth(100);
        fanModeSelectCanvas->setHeight(40);
        fanModeSelectCanvas->setHAlign(ALIGN_RIGHT);
        fanModeSelectCanvas->setVAlign(ALIGN_MIDDLE);
        fanModeSelectCanvas->vAlignBottom(presetModeIconCanvas);
        fanModeSelectCanvas->setFont(FONT_REGULAR_24);
        fanModeSelectCanvas->setWidth(this->getRightX() - fanModeTextCanvas->getRightX());
        fanModeSelectCanvas->setX(fanModeTextCanvas->getRightX());
        fanModeSelectCanvas->resetLayout();

        //Swing modes
        swingModeIconCanvas = new IconCanvas(this, 4);
        swingModeTextCanvas = new TextCanvas(this, 5);
        swingModeSelectCanvas = new TextSelectCanvas(this, 6);
        swingModeIconCanvas->setWidth(40);
        swingModeIconCanvas->setHeight(40);
        swingModeIconCanvas->vAlignBottom(fanModeIconCanvas);
        swingModeIconCanvas->setVAlign(ALIGN_MIDDLE);
        swingModeIconCanvas->setHAlign(ALIGN_CENTER);
        swingModeIconCanvas->setPath(ICON_WEATHER_WINDY);
        swingModeIconCanvas->setFgColor(theme->colorEntityIcon);

        swingModeTextCanvas->setWidth(80);
        swingModeTextCanvas->setHeight(40);
        swingModeTextCanvas->setVAlign(ALIGN_MIDDLE);
        swingModeTextCanvas->setHAlign(ALIGN_LEFT);
        swingModeTextCanvas->setFont(FONT_REGULAR_24);
        swingModeTextCanvas->setText("Swing mode");
        swingModeTextCanvas->vAlignBottom(fanModeIconCanvas);
        swingModeTextCanvas->hAlignRight(swingModeIconCanvas);
        swingModeTextCanvas->resize();
        
        swingModeSelectCanvas->setWidth(100);
        swingModeSelectCanvas->setHeight(40);
        swingModeSelectCanvas->setHAlign(ALIGN_RIGHT);
        swingModeSelectCanvas->setVAlign(ALIGN_MIDDLE);
        swingModeSelectCanvas->vAlignBottom(fanModeIconCanvas);
        swingModeSelectCanvas->setFont(FONT_REGULAR_24);
        swingModeSelectCanvas->setWidth(this->getRightX() - swingModeTextCanvas->getRightX());
        swingModeSelectCanvas->setX(swingModeTextCanvas->getRightX());
        swingModeSelectCanvas->resetLayout();

        selectCanvas->onStateChange([this](SelectCanvas*, SelectCanvasStateChangeEvent event, const char * &value)->bool {       
                uint8_t index = entity->getHvacModeIndex();
                if (event == CHANGE_NEXT) {    
                    index++;
                    if (index == entity->getNumHvacModes()) {
                        index = 0;
                    }
                } else {
                    if (index == 0) {
                        index = entity->getNumHvacModes() - 1;
                    } else {
                        index--;
                    }
                }
                entity->setHvacMode(index);
                entity->setState(entity->getHvacMode());
                value = entity->getHvacMode();
                this->hass->callService(entity, Climate::SERVICE_SET_HVAC_MODE);
                return true;
            });

        temperatureSliderCanvas->onValueChange([this](SliderCanvas*, int32_t value, int32_t)->void{
                float temperature = value * entity->getTemperatureStep();
                entity->setTemperature(temperature);
                this->hass->callService(entity, Climate::SERVICE_SET_TEMPERATURE);
            });
        
        presetModeSelectCanvas->onStateChange([this](SelectCanvas*, SelectCanvasStateChangeEvent event, const char * &value)->bool {       
                uint8_t index = entity->getPresetModeIndex();
                if (event == CHANGE_NEXT) {    
                    index++;
                    if (index == entity->getNumPresetModes()) {
                        index = 0;
                    }
                } else {
                    if (index == 0) {
                        index = entity->getNumPresetModes() - 1;
                    } else {
                        index--;
                    }
                }
                entity->setPresetMode(index);
                this->hass->callService(entity, Climate::SERVICE_SET_PRESET_MODE);
                return true;
            });

        fanModeSelectCanvas->onStateChange([this](SelectCanvas*, SelectCanvasStateChangeEvent event, const char * &value)->bool {       
                uint8_t index = entity->getFanModeIndex();
                if (event == CHANGE_NEXT) {    
                    index++;
                    if (index == entity->getNumFanModes()) {
                        index = 0;
                    }
                } else {
                    if (index == 0) {
                        index = entity->getNumFanModes() - 1;
                    } else {
                        index--;
                    }
                }
                entity->setFanMode(index);
                this->hass->callService(entity, Climate::SERVICE_SET_FAN_MODE);
                return true;
            });
        
        swingModeSelectCanvas->onStateChange([this](SelectCanvas*, SelectCanvasStateChangeEvent event, const char * &value)->bool {       
                uint8_t index = entity->getSwingModeIndex();
                if (event == CHANGE_NEXT) {    
                    index++;
                    if (index == entity->getNumSwingModes()) {
                        index = 0;
                    }
                } else {
                    if (index == 0) {
                        index = entity->getNumSwingModes() - 1;
                    } else {
                        index--;
                    }
                }
                entity->setSwingMode(index);
                this->hass->callService(entity, Climate::SERVICE_SET_SWING_MODE);
                return true;
            });
    }
}

ThermostatCardCanvas::~ThermostatCardCanvas() {
}

void ThermostatCardCanvas::update() {
    CardCanvas::update();
    if (entity) {
        if (cardConfig->getName()[0] != '\0') {
            nameCanvas->setText(cardConfig->getName());
        } else {
            nameCanvas->setText(entity->getFriendlyName());
        }
        if (cardConfig->getRowIcon()[0] != '\0') {
            iconCanvas->setPath(cardConfig->getRowIcon());
        } else {
            iconCanvas->setPath(entity->getIcon());
        }
        selectCanvas->setValue(entity->getHvacMode());
        char str[15];
        snprintf(str, sizeof(str)-1, "%0.1f°", entity->getTemperature());
        temperatureTextCanvas->setText(str);
        temperatureSliderCanvas->setMin(entity->getMinTemperature()/entity->getTemperatureStep());
        temperatureSliderCanvas->setMax(entity->getMaxTemperature()/entity->getTemperatureStep());
        temperatureSliderCanvas->setStep(1);
        temperatureSliderCanvas->setValue(entity->getTemperature()/entity->getTemperatureStep());

        fanModeSelectCanvas->setValue(entity->getFanMode());
        presetModeSelectCanvas->setValue(entity->getPresetMode());
        swingModeSelectCanvas->setValue(entity->getSwingMode());
        resetLayout();
    }
}

void ThermostatCardCanvas::resetLayout() {
    if (entity) {
        Canvas * rowCanvas = iconCanvas;
        if (entity->isAvailable()) {
            selectCanvas->setVisible(true);
        } else {
            selectCanvas->setVisible(false);
        }

        if (entity->isAvailable() && !entity->isOff() && entity->isFeatureSupported(Climate::SUPPORT_TARGET_TEMPERATURE)) {
            temperatureIconCanvas->vAlignBottom(rowCanvas);
            temperatureSliderCanvas->vAlignBottom(rowCanvas);
            temperatureTextCanvas->vAlignBottom(rowCanvas);
            temperatureIconCanvas->setVisible(true);
            temperatureSliderCanvas->setVisible(true);
            temperatureTextCanvas->setVisible(true);
            rowCanvas = temperatureIconCanvas;
        }
        if (entity->isAvailable() && !entity->isOff() && entity->isFeatureSupported(Climate::SUPPORT_PRESET_MODE)) {
            presetModeIconCanvas->vAlignBottom(rowCanvas);
            presetModeTextCanvas->vAlignBottom(rowCanvas);
            presetModeSelectCanvas->vAlignBottom(rowCanvas);
            presetModeIconCanvas->setVisible(true);
            presetModeTextCanvas->setVisible(true);
            presetModeSelectCanvas->setVisible(true);
            presetModeSelectCanvas->resetLayout();
            rowCanvas = presetModeIconCanvas;
        }
        if (entity->isAvailable() && !entity->isOff() && entity->isFeatureSupported(Climate::SUPPORT_FAN_MODE)) {
            fanModeIconCanvas->vAlignBottom(rowCanvas);
            fanModeTextCanvas->vAlignBottom(rowCanvas);
            fanModeSelectCanvas->vAlignBottom(rowCanvas);
            fanModeIconCanvas->setVisible(true);
            fanModeTextCanvas->setVisible(true);
            fanModeSelectCanvas->setVisible(true);
            fanModeSelectCanvas->resetLayout();
            rowCanvas = fanModeIconCanvas;
        }
        if (entity->isAvailable() && !entity->isOff() && entity->isFeatureSupported(Climate::SUPPORT_SWING_MODE)) {
            swingModeIconCanvas->vAlignBottom(rowCanvas);
            swingModeTextCanvas->vAlignBottom(rowCanvas);
            swingModeSelectCanvas->vAlignBottom(rowCanvas);
            swingModeIconCanvas->setVisible(true);
            swingModeTextCanvas->setVisible(true);
            swingModeSelectCanvas->setVisible(true);
            swingModeSelectCanvas->resetLayout();
            rowCanvas = swingModeIconCanvas;
        }
        if (!entity->isAvailable() || entity->isOff() || !entity->isFeatureSupported(Climate::SUPPORT_TARGET_TEMPERATURE)) {
            temperatureIconCanvas->vAlignBottom(rowCanvas);
            temperatureSliderCanvas->vAlignBottom(rowCanvas);
            temperatureTextCanvas->vAlignBottom(rowCanvas);
            temperatureIconCanvas->setVisible(false);
            temperatureSliderCanvas->setVisible(false);
            temperatureTextCanvas->setVisible(false);
            rowCanvas = temperatureIconCanvas;
        }
        if (!entity->isAvailable() || entity->isOff() || !entity->isFeatureSupported(Climate::SUPPORT_PRESET_MODE)) {
            presetModeIconCanvas->vAlignBottom(rowCanvas);
            presetModeTextCanvas->vAlignBottom(rowCanvas);
            presetModeSelectCanvas->vAlignBottom(rowCanvas);
            presetModeIconCanvas->setVisible(false);
            presetModeTextCanvas->setVisible(false);
            presetModeSelectCanvas->setVisible(false);
            presetModeSelectCanvas->resetLayout();
            rowCanvas = presetModeIconCanvas;
        }
        if (!entity->isAvailable() || entity->isOff() || !entity->isFeatureSupported(Climate::SUPPORT_FAN_MODE)) {
            fanModeIconCanvas->vAlignBottom(rowCanvas);
            fanModeTextCanvas->vAlignBottom(rowCanvas);
            fanModeSelectCanvas->vAlignBottom(rowCanvas);
            fanModeIconCanvas->setVisible(false);
            fanModeTextCanvas->setVisible(false);
            fanModeSelectCanvas->setVisible(false);
            fanModeSelectCanvas->resetLayout();
            rowCanvas = fanModeIconCanvas;
        }
        if (!entity->isAvailable() || entity->isOff() || !entity->isFeatureSupported(Climate::SUPPORT_SWING_MODE)) {
            swingModeIconCanvas->vAlignBottom(rowCanvas);
            swingModeTextCanvas->vAlignBottom(rowCanvas);
            swingModeSelectCanvas->vAlignBottom(rowCanvas);
            swingModeIconCanvas->setVisible(false);
            swingModeTextCanvas->setVisible(false);
            swingModeSelectCanvas->setVisible(false);
            swingModeSelectCanvas->resetLayout();
            rowCanvas = swingModeIconCanvas;
        }
    }
}