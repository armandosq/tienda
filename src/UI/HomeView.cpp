#include "HomeView.hpp"
#include "UI/SceneManager.hpp"
#include "Input/PadManager.hpp"
#include "Database/DatabaseManager.hpp"

namespace UI {

    void HomeView::OnEnter() {
        // Cargar lista de juegos desde base de datos
    }

    void HomeView::Update(float deltaTime) {
        auto& pad = Input::PadManager::GetInstance();
        
        if (pad.IsJustPressed(Input::Button::CROSS)) {
            // Ejemplo de cambio a la lista
            // m_manager->SwitchTo("GameList");
        }
    }

    void HomeView::Render(Graphics::Renderer& renderer) {
        renderer.Clear(Graphics::Color(10, 10, 15));
        
        // Header
        renderer.DrawRect(0, 0, 1280, 80, Graphics::Color(30, 30, 40));
        
        // Banner central
        renderer.DrawRect(100, 120, 1080, 250, Graphics::Color(40, 50, 70));
    }

} // namespace UI
