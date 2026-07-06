#pragma once

#include "UI/View.hpp"
#include "Resources/TextureManager.hpp"
#include <vector>
#include <string>

namespace UI {

    class GameListView : public View {
    public:
        void OnEnter() override;
        void Update(float deltaTime) override;
        void Render(Graphics::Renderer& renderer) override;

    private:
        int m_selectedIndex = 0;
        int m_scrollOffset = 0;
        const int m_visibleItems = 5;
        
        std::shared_ptr<Resources::Texture> m_currentCover;
        std::string m_lastCoverPath;
    };

} // namespace UI
