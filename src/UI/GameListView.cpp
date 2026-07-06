#include "GameListView.hpp"
#include "UI/SceneManager.hpp"
#include "Input/PadManager.hpp"
#include "Database/DatabaseManager.hpp"
#include <iostream>

namespace UI {

    void GameListView::OnEnter() {
        m_selectedIndex = 0;
        m_scrollOffset = 0;
        m_lastCoverPath = "";
    }

    void GameListView::Update(float deltaTime) {
        auto& pad = Input::PadManager::GetInstance();
        const auto& games = Database::DatabaseManager::GetInstance().GetAllGames();
        int totalGames = games.size();

        if (totalGames == 0) return;

        if (pad.IsJustPressed(Input::Button::DOWN)) {
            m_selectedIndex++;
            if (m_selectedIndex >= totalGames) m_selectedIndex = totalGames - 1;
            
            if (m_selectedIndex >= m_scrollOffset + m_visibleItems) {
                m_scrollOffset++;
            }
        }
        else if (pad.IsJustPressed(Input::Button::UP)) {
            m_selectedIndex--;
            if (m_selectedIndex < 0) m_selectedIndex = 0;
            
            if (m_selectedIndex < m_scrollOffset) {
                m_scrollOffset--;
            }
        }
        else if (pad.IsJustPressed(Input::Button::CROSS)) {
            // Abrir InfoView
            m_manager->SwitchTo("GameInfo");
        }
        else if (pad.IsJustPressed(Input::Button::CIRCLE)) {
            m_manager->SwitchTo("Home");
        }

        // Cargar portada si cambió la selección
        const auto& selectedGame = games[m_selectedIndex];
        if (selectedGame.icon != m_lastCoverPath) {
            m_lastCoverPath = selectedGame.icon;
            std::string fullPath = "resources/" + selectedGame.icon; // Ruta absoluta o relativa al EBOOT
            m_currentCover = Resources::TextureManager::GetInstance().GetTexture(fullPath);
            Resources::TextureManager::GetInstance().GarbageCollect(); // Liberar VRAM
        }
    }

    void GameListView::Render(Graphics::Renderer& renderer) {
        renderer.Clear(Graphics::Color(20, 20, 25));
        
        const auto& games = Database::DatabaseManager::GetInstance().GetAllGames();
        if (games.empty()) {
            // renderer.DrawText(100, 100, "Cargando o Sin Juegos...", 20, Graphics::Color(255,255,255));
            return;
        }

        // Render Sidebar / Carátula Grande
        renderer.DrawRect(50, 50, 400, 620, Graphics::Color(30, 30, 40));
        if (m_currentCover && m_currentCover->GetOffset() != nullptr) {
            renderer.DrawImage(100, 100, 300, 300, m_currentCover->GetOffset(), Graphics::Color(255, 255, 255));
        }
        
        // Información de texto para la portada
        const auto& selGame = games[m_selectedIndex];
        // renderer.DrawText(100, 450, selGame.title, 30, Graphics::Color(255,255,255));
        // renderer.DrawText(100, 500, "Versión: " + selGame.version, 20, Graphics::Color(200,200,200));
        // renderer.DrawText(100, 530, "Tamaño: " + selGame.size, 20, Graphics::Color(200,200,200));

        // Render Lista
        int startY = 100;
        int itemHeight = 80;
        int listX = 500;

        for (int i = 0; i < m_visibleItems; ++i) {
            int gameIndex = m_scrollOffset + i;
            if (gameIndex >= (int)games.size()) break;

            const auto& game = games[gameIndex];
            
            // Fondo del item
            if (gameIndex == m_selectedIndex) {
                renderer.DrawRect(listX, startY + (i * itemHeight), 700, itemHeight - 10, Graphics::Color(80, 150, 255, 200)); // Resaltado azul
            } else {
                renderer.DrawRect(listX, startY + (i * itemHeight), 700, itemHeight - 10, Graphics::Color(40, 40, 50));
            }

            // Texto del item
            // renderer.DrawText(listX + 20, startY + (i * itemHeight) + 20, game.title, 24, Graphics::Color(255,255,255));
            // renderer.DrawText(listX + 500, startY + (i * itemHeight) + 20, game.category, 20, Graphics::Color(150,150,150));
        }
    }

} // namespace UI
