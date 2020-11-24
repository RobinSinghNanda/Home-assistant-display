#ifndef __AIRQUALITY_H__
#define __AIRQUALITY_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class AirQuality : public Entity {
        public:
            AirQuality(const char * entity);
            virtual ~AirQuality();
            static constexpr const char * ENTITY_DOMAIN = "air_quality";

            //Attributes
            static constexpr AttributeName ATTR_AQI = "air_quality_index";
            static constexpr AttributeName ATTR_ATTRIBUTION = "attribution";
            static constexpr AttributeName ATTR_CO2 = "carbon_dioxide";
            static constexpr AttributeName ATTR_CO = "carbon_monoxide";
            static constexpr AttributeName ATTR_N2O = "nitrogen_oxide";
            static constexpr AttributeName ATTR_NO = "nitrogen_monoxide";
            static constexpr AttributeName ATTR_NO2 = "nitrogen_dioxide";
            static constexpr AttributeName ATTR_OZONE = "ozone";
            static constexpr AttributeName ATTR_PM_0_1 = "particulate_matter_0_1";
            static constexpr AttributeName ATTR_PM_10 = "particulate_matter_10";
            static constexpr AttributeName ATTR_PM_2_5 = "particulate_matter_2_5";
            static constexpr AttributeName ATTR_SO2 = "sulphur_dioxide";
    };
};

#endif // __AIRQUALITY_H__