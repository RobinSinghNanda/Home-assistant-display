#include "CardConfigParser.hpp"
#include "Cards.hpp"

BaseCardConfig * CardConfigParser::parseCardConfig(JsonObject cardObject) {
  if (cardObject[S_TYPE]) {
    if (cardObject[S_TYPE] == EntitiesCardConfig::TYPE) {
      return parseEntitiesCardConfig(cardObject);
    } else if (cardObject[S_TYPE] == EntityCardConfig::TYPE) {
      return new EntityCardConfig(cardObject["entity"] | "",
          cardObject["name"]|"",
          cardObject["attribute"]|"",
          cardObject["unit"]|"",
          cardObject["title"]|"",
          cardObject["icon"]|"");
    } else if (cardObject[S_TYPE] == LightCardConfig::TYPE) {
      return new LightCardConfig(cardObject["entity"] | "",
          cardObject["title"]|"",
          cardObject["icon"]|"",
          cardObject["state_color"]|false
        );
    } else if (cardObject[S_TYPE] == FanCardConfig::TYPE) {
      return new FanCardConfig(cardObject["entity"] | "",
          cardObject["title"]|"",
          cardObject["icon"]|"",
          cardObject["state_color"]|false
        );
    } else if (cardObject[S_TYPE] == SwitchCardConfig::TYPE) {
      return new SwitchCardConfig(cardObject["entity"] | "",
          cardObject["title"]|"",
          cardObject["icon"]|"",
          cardObject["state_color"]|false
        );
    } else if (cardObject[S_TYPE] == MediaControlCardConfig::TYPE) {
      return new MediaControlCardConfig(cardObject["entity"] | "",
          cardObject["title"]|"",
          cardObject["icon"]|"");
    } else if (cardObject[S_TYPE] == HorizontalStackCardConfig::TYPE) {
      return parseHorizontalStackCardConfig(cardObject);
    } else if (cardObject[S_TYPE] == VerticalStackCardConfig::TYPE) {
      return parseVerticalStackCardConfig(cardObject);
    } else if (cardObject[S_TYPE] == AlarmPanelCardConfig::TYPE) {
      return parseAlarmPanelCardConfig(cardObject);
    } else if (cardObject[S_TYPE] == CalendarCardConfig::TYPE) {
      return parseCalandarCardConfig(cardObject);
    } else if (cardObject[S_TYPE] == WeatherForecastCardConfig::TYPE) {
      return new WeatherForecastCardConfig(cardObject["entity"]|"", cardObject["name"]|"",
              cardObject["show_forecase"]|true, 
              cardObject["secondary_info_attribute"]|"",
              cardObject["title"]|"",
              cardObject["icon"]|"");
    } else if (cardObject[S_TYPE] == ThermostatCardConfig::TYPE) {
      return new ThermostatCardConfig(cardObject["entity"]|"", cardObject["name"]|"",
              cardObject["title"]|"",
              cardObject["icon"]|"");
    } else if (cardObject[S_TYPE] == HumidifierCardConfig::TYPE) {
      return new HumidifierCardConfig(cardObject["entity"]|"", cardObject["name"]|"",
              cardObject["title"]|"",
              cardObject["icon"]|"");
    } else {
      return nullptr;
    }
  } else {
    return NULL;
  }
  return nullptr;
}

BaseRowConfig * CardConfigParser::parseRowConfiguration (JsonVariant rowVariant) {
   if (rowVariant.is<String>()) {
      return new DefaultRowConfig(rowVariant.as<String>().c_str());
  } else if (rowVariant.is<JsonObject>()){
    JsonObject entityObject = rowVariant.as<JsonObject>();
    if (entityObject.containsKey("type")) {
      if (entityObject["type"] == ButtonsRowConfig::TYPE) {
        return parseButtonRowConfigurtion(entityObject);
      }
    } else {
      if (entityObject.containsKey("entity")) {
        return new DefaultRowConfig(entityObject["entity"].as<String>().c_str(),
            entityObject["name"] | "",
            entityObject["icon"] | "",
            entityObject["state_color"] | 2,
            entityObject["hide_if_unavailable"] | 0);
      } else {
        return new DefaultRowConfig("");
      }
    }
  }
  return NULL;
}

ButtonsRowConfig * CardConfigParser::parseButtonRowConfigurtion(JsonObject entityObject) {
  if (entityObject.containsKey("buttons")) {
    if (entityObject["buttons"].is<JsonArray>()) {
      ButtonsRowConfig * buttonsRowConfig = new ButtonsRowConfig();
      JsonArray buttonsArray = entityObject["buttons"].as<JsonArray>();
      for (int buttonIndex = 0;buttonIndex<buttonsArray.size();buttonIndex++) {
        buttonsRowConfig->addButton(parseRowConfiguration(buttonsArray[buttonIndex]));
      }
      return buttonsRowConfig;
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
  return NULL;
}

EntitiesCardConfig * CardConfigParser::parseEntitiesCardConfig(JsonObject cardObject) {
  EntitiesCardConfig * entitiesCard = new EntitiesCardConfig(cardObject["title"] | "",
        cardObject["icon"] | "");
  if ((cardObject["entities"].is<JsonArray>())) {
    JsonArray entitesArray = (cardObject["entities"]).as<JsonArray>();
    for (uint8_t entityIndex=0;entityIndex<entitesArray.size();entityIndex++) {
      entitiesCard->addEntity(parseRowConfiguration(entitesArray[entityIndex]));
    }
    return entitiesCard;
  }
  return entitiesCard;
}

HorizontalStackCardConfig * CardConfigParser::parseHorizontalStackCardConfig(JsonObject cardObject) {
  if (cardObject.containsKey("cards")) {
    if (cardObject["cards"].is<JsonArray>()) {
      HorizontalStackCardConfig * horizontalStackCardConfig = new HorizontalStackCardConfig(cardObject["title"]|"", cardObject["icon"]|"");
      for (JsonVariant cardConfig:cardObject["cards"].as<JsonArray>()) {
        horizontalStackCardConfig->addCard(parseCardConfig(cardConfig));
      }
      return horizontalStackCardConfig;
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

VerticalStackCardConfig * CardConfigParser::parseVerticalStackCardConfig(JsonObject cardObject) {
  if (cardObject.containsKey("cards")) {
    if (cardObject["cards"].is<JsonArray>()) {
      VerticalStackCardConfig * verticalStackCardConfig = new VerticalStackCardConfig(cardObject["title"]|"", cardObject["icon"]|"");
      for (JsonVariant cardConfig:cardObject["cards"].as<JsonArray>()) {
        verticalStackCardConfig->addCard(parseCardConfig(cardConfig));
      }
      return verticalStackCardConfig;
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

AlarmPanelCardConfig * CardConfigParser::parseAlarmPanelCardConfig(JsonObject cardObject) {
  if (cardObject.containsKey("entity")) {
    std::list<string> * states = nullptr;
    uint8_t numStates = 0;
    if (cardObject.containsKey("states")) {
      if (cardObject["states"].is<JsonArray>()) {
        JsonArray statesArr = cardObject["states"].as<JsonArray>();
        numStates = statesArr.size();
        states = new std::list<string>();
        for(uint8_t stateIndex =0;stateIndex<numStates;stateIndex++) {
          states->push_back(statesArr[stateIndex]);
        } 
      } else {
        return NULL;
      }
    }
    AlarmPanelCardConfig * alarmPanelCardConfig = new AlarmPanelCardConfig(cardObject["entity"], cardObject["name"]|"", states);
    if (states != nullptr) {
      delete states;
    }
    return alarmPanelCardConfig;
  } else {
    return NULL;
  }
}

CalendarCardConfig * CardConfigParser::parseCalandarCardConfig(JsonObject cardObject) {
  if (cardObject.containsKey("entities")) {
    if (cardObject["entities"].is<JsonArray>()) {
      JsonArray entitiesArray = cardObject["entities"];
      if (entitiesArray.size() != 0) {
        std::vector<string> entities;
        for (auto entityEle : entitiesArray) {
          if (entityEle.is<string>()) {
            entities.push_back(entityEle);
          }
        }
        if (entities.size() != 0) {
          return new CalendarCardConfig(entities, cardObject["intial_view"]|"", cardObject["title"]|"", cardObject["icon"]|"");
        }
      }
    }
  }
  return nullptr;
}