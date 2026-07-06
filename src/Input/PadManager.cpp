#include "PadManager.hpp"
#include <iostream>

namespace Input {

    void PadManager::Initialize() {
        ioPadInit(7);
    }

    void PadManager::Update() {
        padInfo padinfo;
        padData paddata;

        ioPadGetInfo(&padinfo);
        
        m_previousState = m_currentState;

        if (padinfo.status[0]) { // Pad 0 (Player 1)
            ioPadGetData(0, &paddata);
            if (paddata.len > 0) {
                m_currentState = paddata.BTN[2] | (paddata.BTN[3] << 8);
            } else {
                m_currentState = 0;
            }
        } else {
            m_currentState = 0;
        }
    }

    uint16_t PadManager::MapButton(Button button) const {
        switch (button) {
            case Button::SELECT:   return 1 << 0;
            case Button::START:    return 1 << 3;
            case Button::UP:       return 1 << 4;
            case Button::RIGHT:    return 1 << 5;
            case Button::DOWN:     return 1 << 6;
            case Button::LEFT:     return 1 << 7;
            case Button::L2:       return 1 << 8;
            case Button::R2:       return 1 << 9;
            case Button::L1:       return 1 << 10;
            case Button::R1:       return 1 << 11;
            case Button::TRIANGLE: return 1 << 12;
            case Button::CIRCLE:   return 1 << 13;
            case Button::CROSS:    return 1 << 14;
            case Button::SQUARE:   return 1 << 15;
            default:               return 0;
        }
    }

    bool PadManager::IsPressed(Button button) const {
        return (m_currentState & MapButton(button)) != 0;
    }

    bool PadManager::IsJustPressed(Button button) const {
        uint16_t mask = MapButton(button);
        return ((m_currentState & mask) != 0) && ((m_previousState & mask) == 0);
    }

    bool PadManager::IsJustReleased(Button button) const {
        uint16_t mask = MapButton(button);
        return ((m_currentState & mask) == 0) && ((m_previousState & mask) != 0);
    }

} // namespace Input
