#pragma once

#include <vector>
#include <string>
#include <functional>
#include "Database/Game.hpp"

namespace Database {

    class DatabaseManager {
    public:
        // Singleton pattern
        static DatabaseManager& GetInstance() {
            static DatabaseManager instance;
            return instance;
        }

        DatabaseManager(DatabaseManager const&) = delete;
        void operator=(DatabaseManager const&) = delete;

        // Carga la base de datos completa utilizando el contenido TSV
        void LoadFromTSV(const std::string& tsv_content);

        // Obtener todos los juegos
        const std::vector<Game>& GetAllGames() const;

        // Filtrado y búsqueda
        std::vector<Game> SearchByName(const std::string& query) const;
        std::vector<Game> FilterByCategory(const std::string& category) const;
        std::vector<Game> GetFavorites() const;

        // Ordenamiento
        void SortByName();
        void SortBySize();

        // Modificadores de estado (ej. marcar favorito)
        void ToggleFavorite(int game_id);

    private:
        DatabaseManager() {}
        std::vector<Game> m_games;
    };

} // namespace Database
