#include "EntitesCardCanvas.hpp"
#include "RowCanvas/RowCanvases.hpp"
#include "HomeAssistant/Components/Components.hpp"
#include "Log.hpp"
using namespace HomeAssistant;

EntitesCardCanvas::EntitesCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, EntitiesCardConfig * cardConfig, LcdTheme * theme) :
    CardCanvas(canvas, id, hass, cardConfig, theme) {
  this->cardConfig = cardConfig;
  this->entityCallback = [](BaseCardConfig*)->bool{return false;};
  BaseRowCanvas * rowCanvas = nullptr;
  BaseRowCanvas * prevRowCanvas = nullptr;
  for (uint8_t i=0;i<cardConfig->getNumEntites();i++) {
    BaseRowConfig * rowConfig = cardConfig->getEntityRow(i);
    if (rowConfig == nullptr) {
      continue;
    }
    if (rowConfig->isType(DefaultRowConfig::TYPE)) {
        DefaultRowConfig * defaultRowConfig = (DefaultRowConfig *)rowConfig;
        Entity * entity = hass->getEntity(defaultRowConfig->getEntityId());
        if (entity) {
          if (entity->is<Fan>()) {
            rowCanvas = new FanEntityRowCanvas(this, i, hass, defaultRowConfig, theme);
          } else if (entity->is<Light>()) {
            rowCanvas = new LightEntityRowCanvas(this, i, hass, defaultRowConfig, theme);
          } else if (entity->is<Switch>()) {
            rowCanvas = new SwitchEntityRowCanvas(this, i, hass, defaultRowConfig, theme);
          } else if (entity->is<Climate>()) {
            rowCanvas = new ClimateEntityRowCanvas(this, i, hass, defaultRowConfig, theme);
          } else if (entity->is<InputNumber>()) {
            rowCanvas = new InputNumberRowCanvas(this, i, hass, defaultRowConfig, theme);
          } else if (entity->is<InputSelect>()) {
            rowCanvas = new InputSelectRowCanvas(this, i, hass, defaultRowConfig, theme);
          } else if (entity->is<InputText>()) {
            rowCanvas = new InputTextRowCanvas(this, i, hass, defaultRowConfig, theme);
          } else if (entity->is<Scene>()) {
            rowCanvas = new SceneEntityRowCanvas(this, i, hass, defaultRowConfig, theme);
          } else if (entity->is<Script>()) {
            rowCanvas = new ScriptEntityRowCanvas(this, i, hass, defaultRowConfig, theme);
          } else if (entity->is<MediaPlayer>()) {
            rowCanvas = new MediaPlayerEntityRowCanvas(this, i, hass, defaultRowConfig, theme);
          } else if (entity->is<Humidifier>()) {
            rowCanvas = new ToggleEntityRowCanvas(this, i, hass, defaultRowConfig, theme);
          } else {
            rowCanvas = new SensorEntityRowCanvas(this, i, hass, defaultRowConfig, theme);
          }
        } else {
          continue;
        }
        if (rowCanvas != nullptr) {
          BaseEntityRowCanvas * baseEntityRowCanvas = (BaseEntityRowCanvas *)rowCanvas;
          baseEntityRowCanvas->setCallback([this](BaseCardConfig * cardConfig)->bool{
              return entityCallback(cardConfig);
            });
        }
    } else if (rowConfig->isType(ButtonsRowConfig::TYPE)) {
      ButtonsRowConfig * buttonRowConfig = (ButtonsRowConfig *)rowConfig;
      rowCanvas = new ButtonsRowCanvas(this, i, hass, buttonRowConfig, theme);
    } else {
      Log::log(LOG_LEVEL_ERROR, D_LOG_LCD "Unknown type row %s", rowConfig->getType());
      continue;
    }
    if (rowCanvas != nullptr) {
      if (prevRowCanvas != nullptr) {
        rowCanvas->setY(prevRowCanvas->getBottomY());
      }
      prevRowCanvas = rowCanvas;
    }
  }
}

EntitesCardCanvas::~EntitesCardCanvas() {

}

void EntitesCardCanvas::setCallback(EntitesCardCanvasCallback callback) {
  this->entityCallback = callback;
}

void EntitesCardCanvas::update() {
  for (uint8_t i=0;i<cardConfig->getNumEntites();i++) {
    BaseRowCanvas * baseRowCanvas = (BaseRowCanvas *) get(i);
    if (baseRowCanvas != nullptr) {
      baseRowCanvas->update();
    }
  }
}