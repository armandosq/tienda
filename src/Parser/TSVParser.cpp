#include "TSVParser.hpp"
#include <sstream>
#include <iostream>

namespace Parser {

    std::vector<Database::Game> TSVParser::Parse(const std::string& tsv_content) {
        std::vector<Database::Game> games;
        std::stringstream ss(tsv_content);
        std::string line;

        // Skip the header line
        std::getline(ss, line);

        while (std::getline(ss, line)) {
            if (line.empty()) continue;

            std::stringstream line_ss(line);
            std::string cell;
            Database::Game game;

            int column = 0;
            while (std::getline(line_ss, cell, '\t')) {
                switch (column) {
                    case 0: game.id = std::stoi(cell); break;
                    case 1: game.title = cell; break;
                    case 2: game.category = cell; break;
                    case 3: game.version = cell; break;
                    case 4: game.size = cell; break;
                    case 5: game.region = cell; break;
                    case 6: game.icon = cell; break;
                    case 7: game.background = cell; break;
                    case 8: game.description = cell; break;
                    case 9: game.pkg_url = cell; break;
                }
                column++;
            }

            // Only add if we parsed all columns (basic validation)
            if (column >= 10) {
                games.push_back(game);
            }
        }

        return games;
    }

} // namespace Parser
