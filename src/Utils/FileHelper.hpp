#pragma once

#include <string>

namespace Utils {

    class FileHelper {
    public:
        // Lee el contenido completo de un archivo de texto y lo retorna como string.
        static std::string ReadAllText(const std::string& path);
    };

} // namespace Utils
