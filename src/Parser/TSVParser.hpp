#pragma once

#include <string>
#include <vector>
#include "Database/Game.hpp"

namespace Parser {

    class TSVParser {
    public:
        // Carga la base de datos completa desde un string en formato TSV
        static std::vector<Database::Game> Parse(const std::string& tsv_content);
    };

} // namespace Parser
