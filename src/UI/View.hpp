#pragma once

#include "Graphics/Renderer.hpp"
#include <memory>

namespace UI {

    class SceneManager;

    class View {
    public:
        virtual ~View() = default;

        virtual void OnEnter() {}
        virtual void OnExit() {}

        virtual void Update(float deltaTime) = 0;
        virtual void Render(Graphics::Renderer& renderer) = 0;
        
        void SetSceneManager(SceneManager* manager) { m_manager = manager; }

    protected:
        SceneManager* m_manager = nullptr;
    };

} // namespace UI
