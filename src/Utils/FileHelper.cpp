#include "FileHelper.hpp"
#include <fstream>
#include <sstream>

namespace Utils {

    std::string FileHelper::ReadAllText(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

} // namespace Utils
