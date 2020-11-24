#ifndef __LCDTHEME_H__
#define __LCDTHEME_H__

#include "Canvas/Color.hpp"

struct LcdTheme {
    Color colorBodyBackground;
    Color colorBodyText;
    Color colorHeaderBackground;
    Color colorHeaderText;
    Color colorHeaderIcon;
    Color colorPageIndicator;
    Color colorPageIndicatorSelected;
    Color colorScreenSaverBackground;
    Color colorScreenSaverText;
    Color colorScreenSaverHeaderIcon;
    Color colorEntityIcon;
    Color colorEntityIconUnavailable;
    Color colorEntityIconEnabled;
    Color colorSwitchSecondary;
    Color colorSwitchSurface;
    Color colorSwitchOnSurface;
};

#endif // __LCDTHEME_H__