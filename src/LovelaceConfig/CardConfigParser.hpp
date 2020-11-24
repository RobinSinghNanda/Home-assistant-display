#ifndef __CARDCONFIGPARSER_H__
#define __CARDCONFIGPARSER_H__

#include "Cards.hpp"
#include <ArduinoJson.h>

class CardConfigParser {
    public:
        static BaseCardConfig * parseCardConfig(JsonObject cardObject);
        static constexpr const char * S_TYPE = "type";
    protected:
        static AlarmPanelCardConfig * parseAlarmPanelCardConfig(JsonObject cardObject);
        static VerticalStackCardConfig * parseVerticalStackCardConfig(JsonObject cardObject);
        static HorizontalStackCardConfig * parseHorizontalStackCardConfig(JsonObject cardObject);
        static EntitiesCardConfig * parseEntitiesCardConfig(JsonObject cardObject);
        static ButtonsRowConfig * parseButtonRowConfigurtion(JsonObject entityObject);
        static BaseRowConfig * parseRowConfiguration (JsonVariant rowVariant);
        static CalendarCardConfig * parseCalandarCardConfig(JsonObject cardObject);
};

#endif // __CARDCONFIGPARSER_H__