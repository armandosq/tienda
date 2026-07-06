#include "SceneManager.hpp"
#include <iostream>

namespace UI {

    void SceneManager::RegisterView(const std::string& name, std::shared_ptr<View> view) {
        view->SetSceneManager(this);
        m_views[name] = view;
    }

    void SceneManager::SwitchTo(const std::string& name) {
        auto it = m_views.find(name);
        if (it != m_views.end()) {
            if (m_currentView == nullptr) {
                m_currentView = it->second;
                m_currentView->OnEnter();
            } else {
                m_nextView = it->second;
                m_isTransitioning = true;
                m_transitionAlpha = 0.0f; // Fade out
            }
        }
    }

    void SceneManager::Update(float deltaTime) {
        if (m_isTransitioning) {
            m_transitionAlpha += deltaTime * 2.0f; // Velocidad de transición
            if (m_transitionAlpha >= 1.0f) {
                m_transitionAlpha = 0.0f;
                m_isTransitioning = false;
                
                m_currentView->OnExit();
                m_currentView = m_nextView;
                m_currentView->OnEnter();
                m_nextView = nullptr;
            }
        } else if (m_currentView) {
            m_currentView->Update(deltaTime);
        }
    }

    void SceneManager::Render(Graphics::Renderer& renderer) {
        if (m_currentView) {
            m_currentView->Render(renderer);
        }

        if (m_isTransitioning) {
            // Draw un rectangulo negro con alfa sobre toda la pantalla
            uint8_t alpha = static_cast<uint8_t>(m_transitionAlpha * 255.0f);
            renderer.DrawRect(0, 0, 1920, 1080, Graphics::Color(0, 0, 0, alpha));
        }
    }

} // namespace UI
