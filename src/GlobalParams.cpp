#include "GlobalParams.hpp"

GlobalParams globalParams;

GlobalParams::GlobalParams(/* args */)
{
}

GlobalParams::~GlobalParams()
{
}

String GlobalParams::getUpTimeString() {
    uint16_t days = 0;
    uint8_t hours = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;
    uint32_t uptime = this->uptime;
    days = uptime/(60*60*24);
    uptime -= days*60*60*24;
    hours = (uptime)/(60*60);
    uptime -= hours*60*60;
    minutes = (uptime)/60;
    uptime -= minutes*60;
    seconds = uptime;
    String uptimeString;
    if (seconds) {
        if (!hours) {
            uptimeString = String(seconds)+"s";
        }
        if (minutes){
            if (!days) {
                uptimeString = String(minutes)+"m "+uptimeString;
            }
            if (hours) {
                uptimeString = String(hours)+"h "+uptimeString;
                if (days) {
                    uptimeString = String(days)+"d "+uptimeString;
                }
            }
        }
        return uptimeString;
    } else {
        return "0";
    }
}