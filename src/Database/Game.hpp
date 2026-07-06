#pragma once

#include <string>

namespace Database {

    struct Game {
        int id;
        std::string title;
        std::string category;
        std::string version;
        std::string size;
        std::string region;
        std::string icon;
        std::string background;
        std::string description;
        std::string pkg_url;

        // Metadatos adicionales para control interno
        bool is_downloaded = false;
        bool is_favorite = false;
    };

} // namespace Database
