#ifndef __BUTTONSROWSCONFIG_H__
#define __BUTTONSROWSCONFIG_H__

#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include "BaseRowConfig.hpp"
#include "CardTypes.hpp"

class ButtonsRowConfig : public BaseRowConfig {
  public:
    ButtonsRowConfig();
    static constexpr const char * TYPE = "custom:paper-buttons-row";
    static constexpr int MAX_BUTTONS = 5;

    void addButton(BaseRowConfig * rowConfig);
    uint16_t getNumButtons();
    BaseRowConfig * getButton(uint16_t index);
  protected:
    std::vector<BaseRowConfig*> buttons;
};

#endif // __BUTTONSROWSCONFIG_H__