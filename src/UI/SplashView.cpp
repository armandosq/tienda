#include "SplashView.hpp"
#include "UI/SceneManager.hpp"
#include "Utils/FileHelper.hpp"
#include "Database/DatabaseManager.hpp"
#include <iostream>

namespace UI {

    void SplashView::OnEnter() {
        m_timer = 0.0f;
        // Para pruebas en emulador: Cargamos el TSV desde la ruta relativa
        std::string tsv_content = Utils::FileHelper::ReadAllText("resources/games.tsv");
        if (!tsv_content.empty()) {
            Database::DatabaseManager::GetInstance().LoadFromTSV(tsv_content);
            std::cout << "TSV cargado. Juegos encontrados: " << Database::DatabaseManager::GetInstance().GetAllGames().size() << std::endl;
        } else {
            std::cout << "No se encontro resources/games.tsv o esta vacio." << std::endl;
        }
    }

    void SplashView::Update(float deltaTime) {
        m_timer += deltaTime;
        if (m_timer > 3.0f) {
            // Asumiendo que HomeView se llamará "Home"
            m_manager->SwitchTo("Home");
        }
    }

    void SplashView::Render(Graphics::Renderer& renderer) {
        // Fondo gris oscuro
        renderer.Clear(Graphics::Color(20, 20, 20));
        
        // Texto de logo (simulado)
        // renderer.DrawText(..., "Indie Store", ...);
        renderer.DrawRect(1280/2 - 100, 720/2 - 50, 200, 100, Graphics::Color(255, 100, 100)); // Placeholder logo
    }

} // namespace UI
