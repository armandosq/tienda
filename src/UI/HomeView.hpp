#pragma once

#include "UI/View.hpp"
#include <string>

namespace UI {

    class HomeView : public View {
    public:
        void OnEnter() override;
        void Update(float deltaTime) override;
        void Render(Graphics::Renderer& renderer) override;
    };

} // namespace UI
