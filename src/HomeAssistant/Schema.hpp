#ifndef __SCHEMA_H__
#define __SCHEMA_H__

#include <stdint.h>
#include <string>
#include <vector>
#include <ArduinoJson.h>
using namespace std;

namespace HomeAssistant {
    enum Type {
        TYPE_STRING,
        TYPE_STRING_VECTOR,
        TYPE_INT32,
        TYPE_UINT32,
        TYPE_FLOAT,
        TYPE_BOOL,
        TYPE_COLOR
    };
    typedef const char * Name;
    struct SchemaEntry {
        Type type; 
        Name name;
        void * value;
    };
    class Schema {
        public:
            Schema();
            inline void add(const char * name, float &value);
            inline void add(const char * name, uint32_t &value);
            inline void add(const char * name, int32_t &value);
            inline void add(const char * name, string &value);
            inline void add(const char * name, bool &value);
            inline void add(const char * name, vector<string> &value);
            void update(JsonObject object);
        protected:
            vector<SchemaEntry> schema;
    };

    void Schema::add(const char * name, float &value) {
        schema.push_back({TYPE_FLOAT, name, (void *)&value});
    }

    void Schema::add(const char * name, uint32_t &value) {
        schema.push_back({TYPE_UINT32, name, (void *)&value});
    }

    void Schema::add(const char * name, int32_t &value) {
        schema.push_back({TYPE_INT32, name, (void *)&value});
    }

    void Schema::add(const char * name, string &value) {
        schema.push_back({TYPE_STRING, name, (void *)&value});
    }

    void Schema::add(const char * name, bool &value) {
        schema.push_back({TYPE_BOOL, name, (void *)&value});
    }

    void Schema::add(const char * name, vector<string> &value) {
        schema.push_back({TYPE_STRING_VECTOR, name, (void *)&value});
    }

};

#endif // __SCHEMA_H__