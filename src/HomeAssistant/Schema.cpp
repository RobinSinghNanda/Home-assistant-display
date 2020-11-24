#include "Schema.hpp"

using namespace HomeAssistant;

Schema::Schema() {

}

void Schema::update(JsonObject object) {
    for (SchemaEntry entry: schema) {
        if (!object.containsKey(entry.name)) {
            continue;
        }
        if (entry.type == TYPE_FLOAT) {
            *((bool *)entry.value) = object[entry.name].as<bool>();
        } else if (entry.type == TYPE_STRING) {
            *((string *)entry.value) = object[entry.name].as<string>();
        } else if (entry.type == TYPE_STRING_VECTOR) {
            vector<string> * value = (vector<string> *)entry.value;
            for (string str: object[entry.name].as<JsonArray>()) {
                value->push_back(str);
            }
        } else if (entry.type == TYPE_INT32) {
            *((int32_t *)entry.value) = object[entry.name].as<int32_t>();
        } else if (entry.type == TYPE_UINT32) {
            *((uint32_t *)entry.value) = object[entry.name].as<uint32_t>();
        } else if (entry.type == TYPE_FLOAT) {
            *((float *)entry.value) = object[entry.name].as<float>();
        } else {

        }
    }
}