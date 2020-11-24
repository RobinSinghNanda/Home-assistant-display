#include "Person.hpp"
using namespace HomeAssistant;

Person::Person(const char * entity) :Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_ACCOUNT;
}

Person::~Person() {

}