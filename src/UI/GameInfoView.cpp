#include "GameInfoView.hpp"
#include "UI/SceneManager.hpp"
#include "Input/PadManager.hpp"
#include "Database/DatabaseManager.hpp"
#include "Downloader/DownloadManager.hpp"

namespace UI {

    // En un sistema real usaríamos el SceneManager para pasar estados (ej. ID del juego),
    // pero por ahora podemos usar una variable estática o el juego actualmente seleccionado
    // en una clase estática de "Estado de UI" si fuera necesario.
    // Para simplificar, asumiremos que GameListView deja seleccionado un ID y GameInfoView lo toma.
    
    // Solo para esta demo rápida, guardamos un extern simulado:
    extern int g_selectedGameId;
    int g_selectedGameId = 1; // Asumiremos el 1 por defecto

    void GameInfoView::OnEnter() {
        // En base al g_selectedGameId, cargar background
        // auto bgPath = "resources/" + game.background;
        // m_background = Resources::TextureManager::GetInstance().GetTexture(bgPath);
        
        m_activeDownload = nullptr;
        m_isDownloading = false;
    }

    void GameInfoView::Update(float deltaTime) {
        auto& pad = Input::PadManager::GetInstance();
        
        if (pad.IsJustPressed(Input::Button::CIRCLE)) {
            m_manager->SwitchTo("GameList");
        }
        else if (pad.IsJustPressed(Input::Button::CROSS) && !m_isDownloading) {
            // Simular obtener datos del juego
            std::string url = "http://miservidor.com/pkg/game.pkg"; 
            std::string dest = "/dev_hdd0/packages/game.pkg";
            
            m_activeDownload = Downloader::DownloadManager::GetInstance().AddDownload(url, dest, "Juego Indie");
            m_isDownloading = true;
        }
    }

    void GameInfoView::Render(Graphics::Renderer& renderer) {
        renderer.Clear(Graphics::Color(15, 15, 20));
        
        // Si hay background, lo dibujamos a pantalla completa con alfa
        if (m_background && m_background->GetOffset()) {
            renderer.DrawImage(0, 0, 1280, 720, m_background->GetOffset(), Graphics::Color(255, 255, 255, 128)); // Mitad transparente
        }
        
        // UI Layout
        renderer.DrawRect(100, 100, 500, 500, Graphics::Color(30, 30, 40, 200));
        
        // Detalles textuales
        // renderer.DrawText(120, 120, "Título del Juego", 40, Graphics::Color(255,255,255));
        // renderer.DrawText(120, 200, "Descripción detallada...", 20, Graphics::Color(200,200,200));
        
        // Botón
        if (!m_isDownloading) {
            renderer.DrawRect(120, 400, 200, 50, Graphics::Color(50, 150, 255));
            // renderer.DrawText(140, 415, "[X] Descargar", 20, Graphics::Color(255,255,255));
        } else {
            // Barra de progreso
            float progress = 0.0f;
            if (m_activeDownload) {
                progress = m_activeDownload->GetProgress();
                
                // std::string speedStr = std::to_string(m_activeDownload->GetSpeed() / 1024 / 1024) + " MB/s";
                // renderer.DrawText(120, 380, speedStr, 20, Graphics::Color(255,255,255));
            }
            
            // Fondo barra
            renderer.DrawRect(120, 400, 400, 20, Graphics::Color(60, 60, 60));
            // Progreso
            renderer.DrawRect(120, 400, 400 * progress, 20, Graphics::Color(50, 255, 100));
        }
    }

} // namespace UI
