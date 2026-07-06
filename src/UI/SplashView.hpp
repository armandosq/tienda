#pragma once

#include "UI/View.hpp"
#include <string>

namespace UI {

    class SplashView : public View {
    public:
        void OnEnter() override;
        void Update(float deltaTime) override;
        void Render(Graphics::Renderer& renderer) override;

    private:
        float m_timer = 0.0f;
    };

} // namespace UI
