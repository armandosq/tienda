#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include "UI/View.hpp"

namespace UI {

    class SceneManager {
    public:
        static SceneManager& GetInstance() {
            static SceneManager instance;
            return instance;
        }

        SceneManager(SceneManager const&) = delete;
        void operator=(SceneManager const&) = delete;

        void RegisterView(const std::string& name, std::shared_ptr<View> view);
        void SwitchTo(const std::string& name);

        void Update(float deltaTime);
        void Render(Graphics::Renderer& renderer);

    private:
        SceneManager() {}
        
        std::unordered_map<std::string, std::shared_ptr<View>> m_views;
        std::shared_ptr<View> m_currentView;
        std::shared_ptr<View> m_nextView;
        
        // Transiciones
        bool m_isTransitioning = false;
        float m_transitionAlpha = 0.0f;
    };

} // namespace UI
