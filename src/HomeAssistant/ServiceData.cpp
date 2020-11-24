#include "ServiceData.hpp"

ServiceData _serviceData;
ServiceData * serviceData = &_serviceData;


ServiceData::ServiceData() {
    this->seperator[0] = '\0';
    this->seperator[1] = '\0';
}

ServiceData::operator char*() const {
    return (char *)buffer;
}