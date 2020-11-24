#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Entity.hpp"

namespace HomeAssistant {
    union LightColor {
        uint32_t data;
        struct {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
        };
    };

    class Light : public Entity {
        public:
            Light (const char * entity);
            virtual ~Light();
            static constexpr const char * ENTITY_DOMAIN = "light";

            //Features
            static constexpr Feature SUPPORT_BRIGHTNESS = 1;
            static constexpr Feature SUPPORT_COLOR_TEMP = 2;
            static constexpr Feature SUPPORT_EFFECT = 4;
            static constexpr Feature SUPPORT_FLASH = 8;
            static constexpr Feature SUPPORT_COLOR = 16;
            static constexpr Feature SUPPORT_TRANSITION = 32;
            static constexpr Feature SUPPORT_WHITE_VALUE = 128;
            
            //Attributes
            static constexpr AttributeName ATTR_BRIGHTNESS = "brightness";
            static constexpr AttributeName ATTR_BRIGHTNESS_PCT = "brightness_pct";
            static constexpr AttributeName ATTR_RGB_COLOR = "rgb_color";
            static constexpr AttributeName ATTR_COLOR_TEMP = "color_temp";
            static constexpr AttributeName ATTR_MIN_MIREDS = "min_mireds";
            static constexpr AttributeName ATTR_MAX_MIREDS = "max_mireds";
            static constexpr AttributeName ATTR_WHITE_VALUE = "white_value";
            static constexpr AttributeName ATTR_FLASH = "flash";
            static constexpr AttributeName ATTR_TRANSITION = "transition";
            static constexpr AttributeName ATTR_EFFECT = "effect";
            static constexpr AttributeName ATTR_EFFECT_LIST = "effect_list";
            
            //attribute values
            static constexpr const char* FLASH_SHORT = "short";
            static constexpr const char* FLASH_LONG = "long";

            //Getter and setters
            inline void setBrightness(uint32_t brightness);
            inline uint32_t getBrightness();
            inline void setColorTemp(uint32_t colorTemp);
            inline uint32_t getColorTemp();
            inline void setEffect(uint32_t effect);
            inline uint32_t getEffect();
            inline void setFlash(const char * flash);
            inline const char * getFlash();
            inline void setColor(LightColor color);
            inline void setColor(uint32_t color);
            inline void setColor(uint8_t red, uint8_t green, uint8_t blue);
            inline uint32_t getColor();
            inline uint8_t getRedColor();
            inline uint8_t getGreenColor();
            inline uint8_t getBlueColor();
            inline void setTransition(uint32_t transition);
            inline uint16_t getTransition();
            inline void setWhiteValue(uint32_t whiteValue);
            inline uint32_t getWhiteValue();
            inline uint32_t getMinMireds();
            inline uint32_t getMaxMireds();
            virtual void updateAttributes(JsonObject stateAttributesObject);
            void callServiceTurnOn(HomeAssistantClient * client);
            virtual void callService(HomeAssistantClient * client, const char * service);
        protected:
            uint32_t brightness = 0;
            uint32_t colorTemp = 0;
            uint32_t minMireds = 154;
            uint32_t maxMireds = 500;
            uint32_t effect = 0;
            string flash = "";
            LightColor color = {0x00000000};
            uint32_t transition = 0;
            uint32_t whiteValue = 0;
    };


    void Light::setBrightness(uint32_t brightness) {
        this->brightness = brightness;
        if (brightness == 0 && isOn()) {
            setState(false);
        }
    }

    uint32_t Light::getBrightness() {
        return this->brightness;
    }

    void Light::setColorTemp(uint32_t colorTemp) {
        this->colorTemp = colorTemp;
    }

    uint32_t Light::getColorTemp() {
        return this->colorTemp;
    }

    void Light::setEffect(uint32_t effect) {
        this->effect = effect;
    }

    uint32_t Light::getEffect() {
        return this->effect;
    }

    void Light::setFlash(const char * flash) {
        this->flash = flash;
    }

    const char * Light::getFlash() {
        return this->flash.c_str();
    }

    void Light::setColor(LightColor color) {
        setColor(color.data);
    }

    void Light::setColor(uint32_t color) {
        this->color.data = color;
    }

    void Light::setColor(uint8_t red, uint8_t green, uint8_t blue) {
        LightColor color = {0x0};
        color.red = red;
        color.green = green;
        color.blue = blue;
        setColor(color);
    }

    uint32_t Light::getColor() {
        return this->color.data;
    }

    uint8_t Light::getRedColor() {
        return this->color.red;
    }

    uint8_t Light::getGreenColor() {
        return this->color.green;
    }

    uint8_t Light::getBlueColor() {
        return this->color.blue;
    }

    void Light::setTransition(uint32_t transition) {
        this->transition = transition;
    }

    uint16_t Light::getTransition() {
        return this->transition;
    }

    void Light::setWhiteValue(uint32_t whiteValue) {
        this->whiteValue = whiteValue;
    }

    uint32_t Light::getWhiteValue() {
        return this->whiteValue;
    }

    uint32_t Light::getMinMireds() {
        return this->minMireds;
    }

    uint32_t Light::getMaxMireds() {
        return this->maxMireds;
    }

};


#endif // __LIGHT_H__