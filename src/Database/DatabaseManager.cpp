#include "DatabaseManager.hpp"
#include "Parser/TSVParser.hpp"
#include <algorithm>
#include <cctype>

namespace Database {

    void DatabaseManager::LoadFromTSV(const std::string& tsv_content) {
        m_games = Parser::TSVParser::Parse(tsv_content);
    }

    const std::vector<Game>& DatabaseManager::GetAllGames() const {
        return m_games;
    }

    std::vector<Game> DatabaseManager::SearchByName(const std::string& query) const {
        std::vector<Game> results;
        std::string lower_query = query;
        std::transform(lower_query.begin(), lower_query.end(), lower_query.begin(), ::tolower);

        for (const auto& game : m_games) {
            std::string lower_title = game.title;
            std::transform(lower_title.begin(), lower_title.end(), lower_title.begin(), ::tolower);
            
            if (lower_title.find(lower_query) != std::string::npos) {
                results.push_back(game);
            }
        }
        return results;
    }

    std::vector<Game> DatabaseManager::FilterByCategory(const std::string& category) const {
        std::vector<Game> results;
        for (const auto& game : m_games) {
            if (game.category == category) {
                results.push_back(game);
            }
        }
        return results;
    }

    std::vector<Game> DatabaseManager::GetFavorites() const {
        std::vector<Game> results;
        for (const auto& game : m_games) {
            if (game.is_favorite) {
                results.push_back(game);
            }
        }
        return results;
    }

    void DatabaseManager::SortByName() {
        std::sort(m_games.begin(), m_games.end(), [](const Game& a, const Game& b) {
            return a.title < b.title;
        });
    }

    void DatabaseManager::SortBySize() {
        // Asumiendo que el tamaño tiene formato "1.2 GB". 
        // Para ordenar correctamente, se requeriría parsear el tamaño a bytes.
        // Se implementa ordenamiento de strings por simplicidad inicialmente.
        std::sort(m_games.begin(), m_games.end(), [](const Game& a, const Game& b) {
            return a.size < b.size; 
        });
    }

    void DatabaseManager::ToggleFavorite(int game_id) {
        for (auto& game : m_games) {
            if (game.id == game_id) {
                game.is_favorite = !game.is_favorite;
                break;
            }
        }
    }

} // namespace Database
