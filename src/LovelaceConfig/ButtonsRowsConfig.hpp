#ifndef __BUTTONSROWSCONFIG_H__
#define __BUTTONSROWSCONFIG_H__

#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include "BaseRowConfig.hpp"
#include "CardDefines.hpp"

class ButtonsRowConfig : public BaseRowConfig {
  public:
    ButtonsRowConfig();
    void addButton(BaseRowConfig * rowConfig);
    uint16_t getNumButtons();
    BaseRowConfig * getButton(uint16_t index);
  protected:
    std::vector<BaseRowConfig*> buttons;
};

#endif // __BUTTONSROWSCONFIG_H__