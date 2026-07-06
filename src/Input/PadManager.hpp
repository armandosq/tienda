#pragma once

#include <io/pad.h>
#include <stdint.h>

namespace Input {

    enum class Button {
        CROSS = 0,
        CIRCLE,
        SQUARE,
        TRIANGLE,
        UP,
        DOWN,
        LEFT,
        RIGHT,
        L1,
        R1,
        L2,
        R2,
        START,
        SELECT
    };

    class PadManager {
    public:
        static PadManager& GetInstance() {
            static PadManager instance;
            return instance;
        }

        PadManager(PadManager const&) = delete;
        void operator=(PadManager const&) = delete;

        void Initialize();
        void Update(); // Call once per frame

        bool IsPressed(Button button) const;
        bool IsJustPressed(Button button) const; // True solo en el primer frame en que se pulsa
        bool IsJustReleased(Button button) const;

    private:
        PadManager() : m_currentState(0), m_previousState(0) {}

        uint16_t m_currentState;
        uint16_t m_previousState;
        
        uint16_t MapButton(Button button) const;
    };

} // namespace Input
