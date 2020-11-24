#include "AlarmPanelCardCanvas.hpp"
#include "Icons.hpp"
#include "Fonts.hpp"

AlarmPanelCardCanvas::AlarmPanelCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, AlarmPanelCardConfig * cardConfig, LcdTheme * theme):
    CardCanvas(canvas, id, hass, cardConfig, theme) {
    this->cardConfig = cardConfig;
    this->entity = hass->getEntity<AlarmControlPanel>(cardConfig->getEntityId());
    if (entity) {
        numPadCanvas = new NumberPadCanvas(this, 0);
        numPadCanvas->setRightButtonText("C");
        numPadCanvas->setHAlign(ALIGN_CENTER);
        numPadCanvas->setVAlign(ALIGN_MIDDLE);
        numPadCanvas->setX(this->x + this->getDrawableWidth()/2);
        numPadCanvas->setEnabledColor(theme->colorSwitchSecondary);
        numPadCanvas->setDisabledColor(Color32Bit(0x7F7F7F));
        numPadCanvas->setPressedColor(Color32Bit(0x7F7F7F));
        numPadCanvas->setFont(FONT_REGULAR_24);
        numPadCanvas->resetLayout();
        numPadCanvas->setDisabled(true);
        
        stateCanvas = new ImageCanvas(this, 11);
        stateCanvas->setPath(ICON_BELL);
        stateCanvas->setHAlign(ALIGN_CENTER);
        stateCanvas->setVAlign(ALIGN_MIDDLE);
        stateCanvas->setHeight(40);
        stateCanvas->setWidth(24);
        stateCanvas->setX(this->getDrawX());
        stateCanvas->setY(this->getDrawY());
        stateCanvas->setBorderBottom(1);
        stateCanvas->setBorderColor(theme->colorBodyText);

        codeCanvas = new TextCanvas(this, 12);
        codeCanvas->setText("Code");
        codeCanvas->setFgColor(Color32Bit(0x7F7F7F));
        codeCanvas->setFont(FONT_REGULAR_24);
        codeCanvas->setWidth(this->getDrawableWidth()/2 + 10 - 2 * stateCanvas->getWidth());
        codeCanvas->setHeight(40);
        codeCanvas->setHAlign(ALIGN_CENTER);
        codeCanvas->setVAlign(ALIGN_MIDDLE);
        codeCanvas->setX(stateCanvas->getRightX());
        codeCanvas->setY(this->getDrawY());
        codeCanvas->setBorderBottom(1);
        codeCanvas->setBorderColor(theme->colorBodyText);

        passwordVisibilityToggleButton = new ImageCanvas(this, 11);
        passwordVisibilityToggleButton->setPath(ICON_EYE);
        passwordVisibilityToggleButton->setHAlign(ALIGN_CENTER);
        passwordVisibilityToggleButton->setVAlign(ALIGN_MIDDLE);
        passwordVisibilityToggleButton->setHeight(40);
        passwordVisibilityToggleButton->setWidth(stateCanvas->getWidth());
        passwordVisibilityToggleButton->setX(codeCanvas->getRightX());
        passwordVisibilityToggleButton->setY(this->getDrawY());
        passwordVisibilityToggleButton->setBorderBottom(1);
        passwordVisibilityToggleButton->setBorderColor(theme->colorBodyText);
        
        for (uint8_t i=0;i<MAX_STATE_BUTTONS;i++) {
            stateChangeButtonCanvas[i] = new TextCanvas(this, 13+i);
            stateChangeButtonCanvas[i]->setText("");
            stateChangeButtonCanvas[i]->setFont(FONT_REGULAR_24);
            stateChangeButtonCanvas[i]->setWidth(this->getDrawableWidth()/2 + 10);
            stateChangeButtonCanvas[i]->setHPadding(10, 10);
            stateChangeButtonCanvas[i]->setBorder(1);
            stateChangeButtonCanvas[i]->setBorderColor(theme->colorBodyText);
            stateChangeButtonCanvas[i]->setHeight(40);
            stateChangeButtonCanvas[i]->setHAlign(ALIGN_CENTER);
            stateChangeButtonCanvas[i]->setVAlign(ALIGN_MIDDLE);
            stateChangeButtonCanvas[i]->setX(this->getDrawX());
            stateChangeButtonCanvas[i]->setY(this->getDrawY()+50*i+50);
            stateChangeButtonCanvas[i]->setVisible(false);
            stateChangeButtonCanvas[i]->setFgColor(theme->colorSwitchSecondary);
            stateChangeButtonCanvas[i]->onTouch([this, i](Canvas*, TouchEvent event, TouchEventData)->bool{
                    if (isEventLosely(event, TouchEvent::TouchActionPressed)) {
                        if (i<numStateButtons) {
                            stateChangeButtonCanvas[i]->setBgColor(Color32Bit(0x7F7F7F));
                        }
                    } else if (isEventLosely(event, TouchEvent::TouchActionTapped)) {
                        stateChangeButtonCanvas[i]->setBgColor(bgColor);
                        if (i<numStateButtons) {
                            if (entity->isState(AlarmControlPanel::STATE_ALARM_DISARMED) ||
                                    entity->isState(AlarmControlPanel::STATE_ALARM_DISARMING) ) {
                                uint8_t buttonIndex = 0;
                                if (entity->isFeatureSupported(AlarmControlPanel::SUPPORT_ALARM_ARM_HOME)) {
                                    if (buttonIndex == i) {
                                        entity->setCode(code, codeIndex);
                                        codeIndex = 0;
                                        setCodeString();
                                        this->hass->callService(entity, AlarmControlPanel::SERVICE_ALARM_ARM_HOME);
                                    }
                                    buttonIndex++;
                                }
                                if (entity->isFeatureSupported(AlarmControlPanel::SUPPORT_ALARM_ARM_AWAY)) {
                                    if (buttonIndex == i) {
                                        entity->setCode(code, codeIndex);
                                        codeIndex = 0;
                                        setCodeString();
                                        this->hass->callService(entity, AlarmControlPanel::SERVICE_ALARM_ARM_AWAY);
                                    }
                                    buttonIndex++;
                                }
                                if (entity->isFeatureSupported(AlarmControlPanel::SUPPORT_ALARM_ARM_NIGHT)) {
                                    if (buttonIndex == i) {
                                        entity->setCode(code, codeIndex);
                                        codeIndex = 0;
                                        setCodeString();
                                        this->hass->callService(entity, AlarmControlPanel::SERVICE_ALARM_ARM_NIGHT);
                                    }
                                    buttonIndex++;
                                }
                            } else if (entity->isState(AlarmControlPanel::STATE_ALARM_ARMED_HOME) ||
                                        entity->isState(AlarmControlPanel::STATE_ALARM_ARMED_NIGHT) ||
                                        entity->isState(AlarmControlPanel::STATE_ALARM_ARMED_AWAY) ||
                                        entity->isState(AlarmControlPanel::STATE_ALARM_ARMING)) {
                                if (entity->isCodeArmRequired()) {
                                    entity->setCode(code, codeIndex);
                                    codeIndex = 0;
                                    setCodeString();
                                    this->hass->callService(entity, AlarmControlPanel::SERVICE_ALARM_DISARM);
                                } else {
                                    entity->resetCode();
                                    codeIndex = 0;
                                    setCodeString();
                                    this->hass->callService(entity, AlarmControlPanel::SERVICE_ALARM_DISARM);
                                }
                            }
                            return true;
                        } else {
                            return false;
                        }
                    }
                    return false;
                });
        }
        numPadCanvas->onNumberPress([this](NumberPadCanvas*, uint8_t number)->bool{
                if (number <= 9) {
                    if (codeIndex < MAX_CODE_SIZE) {
                        code[codeIndex] = number;
                        codeIndex++;
                    } else {
                        numPadCanvas->setNumbersDisable(true);
                    }
                } else if (number == 11) {
                    codeIndex = 0;
                    numPadCanvas->setNumbersDisable(false);
                }
                setCodeString();
                return true;
            });
        passwordVisibilityToggleButton->onTouch([this](Canvas*, TouchEvent event, TouchEventData)->bool{
                if (isEvent(event, TouchEvent::TouchActionTapped)) {
                    this->passwordVisible = !passwordVisible;
                    this->passwordVisibilityToggleButton->setPath(passwordVisible?ICON_EYE_OFF:ICON_EYE);
                    setCodeString();
                    return true;
                }
                return false;
            });
    }
}

AlarmPanelCardCanvas::~AlarmPanelCardCanvas() {

}

void AlarmPanelCardCanvas::setCodeString() {
    if (codeIndex == 0) {
        codeCanvas->setText("Code");
        codeCanvas->setFgColor(Color32Bit(0x7F7F7F));
    } else {
        char codeText[MAX_CODE_SIZE+1] = "";
        for (uint8_t i=0;i<codeIndex;i++) {
            snprintf(codeText, sizeof(codeText), "%s%c", codeText, (passwordVisible)?(this->code[i]+'0'):'*');
        }
        codeCanvas->setText(codeText);
        codeCanvas->setFgColor(theme->colorBodyText);
    }
}

const char * AlarmPanelCardCanvas::getCardTitle() {
    return this->cardConfig->getTitle();
}

const char * AlarmPanelCardCanvas::getCardIcon() {
    return this->cardConfig->getIcon();
}

void AlarmPanelCardCanvas::update() {
    if (entity) {
        if (entity->isUnavailable()) {
            stateCanvas->setPath(entity->getIcon());
            numPadCanvas->setDisabled(true);
        } else if (entity->isState(AlarmControlPanel::STATE_ALARM_DISARMED) ||
                    entity->isState(AlarmControlPanel::STATE_ALARM_DISARMING)) {
            uint8_t buttonIndex = 0;
            if (entity->isFeatureSupported(AlarmControlPanel::SUPPORT_ALARM_ARM_HOME)) {
                stateChangeButtonCanvas[buttonIndex]->setText("ARM HOME");
                stateChangeButtonCanvas[buttonIndex]->setVisible(true);
                buttonIndex++;
            }
            if (entity->isFeatureSupported(AlarmControlPanel::SUPPORT_ALARM_ARM_AWAY)) {
                stateChangeButtonCanvas[buttonIndex]->setText("ARM AWAY");
                stateChangeButtonCanvas[buttonIndex]->setVisible(true);
                buttonIndex++;
            }
            if (entity->isFeatureSupported(AlarmControlPanel::SUPPORT_ALARM_ARM_NIGHT)) {
                stateChangeButtonCanvas[buttonIndex]->setText("ARM NIGHT");
                stateChangeButtonCanvas[buttonIndex]->setVisible(true);
                buttonIndex++;
            }
            if (!entity->isCodeArmRequired()) {
                codeIndex = 0;
                setCodeString();
                numPadCanvas->setDisabled(true);
            } else {
                numPadCanvas->setDisabled(false);
            }
            if (buttonIndex != numStateButtons) {
                this->invalidate();
                numStateButtons = buttonIndex;
            }
            for (uint8_t i=buttonIndex;i<MAX_STATE_BUTTONS;i++) {
                stateChangeButtonCanvas[i]->setVisible(false);
            }
            if (entity->isState(AlarmControlPanel::STATE_ALARM_DISARMING)) {
                uint32_t time = millis();
                if (time - prevTime > 1000) {
                    stateCanvas->setPath(iconState?"":entity->getIcon());
                    prevTime = time;
                    iconState = !iconState;
                }
            } else {
                stateCanvas->setPath(entity->getIcon());
            }
        } else if (entity->isState(AlarmControlPanel::STATE_ALARM_ARMED_HOME) ||
                    entity->isState(AlarmControlPanel::STATE_ALARM_ARMED_NIGHT) ||
                    entity->isState(AlarmControlPanel::STATE_ALARM_ARMED_AWAY) ||
                    (entity->isState(AlarmControlPanel::STATE_ALARM_ARMING))) {
            numPadCanvas->setDisabled(false);
            uint8_t buttonIndex = 0;
            stateChangeButtonCanvas[buttonIndex]->setText("DISARM");
            stateChangeButtonCanvas[buttonIndex]->setVisible(true);
            buttonIndex++;
            if (buttonIndex != numStateButtons) {
                this->invalidate();
                numStateButtons = buttonIndex;
            }
            for (uint8_t i=buttonIndex;i<MAX_STATE_BUTTONS;i++) {
                stateChangeButtonCanvas[i]->setVisible(false);
            }
            if (entity->isState(AlarmControlPanel::STATE_ALARM_ARMING)) {
                uint32_t time = millis();
                if (time - prevTime > 1000) {
                    stateCanvas->setPath(iconState?"":entity->getIcon());
                    prevTime = time;
                    iconState = !iconState;
                }
            } else {
                stateCanvas->setPath(entity->getIcon());
            }
        } else {
            numPadCanvas->setDisabled(true);
            stateCanvas->setPath(iconState?"":entity->getIcon());
            for (uint8_t i=0;i<MAX_STATE_BUTTONS;i++) {
                stateChangeButtonCanvas[i]->setVisible(false);
            }
        }
    }
}