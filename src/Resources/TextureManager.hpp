#pragma once

#include <string>
#include <unordered_map>
#include <memory>

namespace Resources {

    class Texture {
    public:
        Texture(const std::string& path);
        ~Texture();

        void* GetOffset() const { return m_offset; }
        uint32_t GetWidth() const { return m_width; }
        uint32_t GetHeight() const { return m_height; }
        uint32_t GetPitch() const { return m_pitch; }

    private:
        void* m_offset;
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_pitch;
        
        bool LoadPNG(const std::string& path);
    };

    class TextureManager {
    public:
        static TextureManager& GetInstance() {
            static TextureManager instance;
            return instance;
        }

        TextureManager(TextureManager const&) = delete;
        void operator=(TextureManager const&) = delete;

        std::shared_ptr<Texture> GetTexture(const std::string& path);
        
        // Limpia texturas que ya no tienen referencias activas (ref_count == 1)
        void GarbageCollect();

    private:
        TextureManager() {}
        std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
    };

} // namespace Resources
