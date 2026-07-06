#pragma once

#include <string>
#include <tiny3d.h>

namespace Graphics {

    struct Color {
        uint8_t r, g, b, a;
        Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255) : r(_r), g(_g), b(_b), a(_a) {}
        uint32_t ToHex() const { return (r << 24) | (g << 16) | (b << 8) | a; }
    };

    class Renderer {
    public:
        static Renderer& GetInstance() {
            static Renderer instance;
            return instance;
        }

        Renderer(Renderer const&) = delete;
        void operator=(Renderer const&) = delete;

        void Initialize();
        void Clear(const Color& color);
        void Flip();

        void DrawRect(float x, float y, float w, float h, const Color& color);
        
        // Se usaría el TextureManager para obtener el rsx_texture_t u offset
        void DrawImage(float x, float y, float w, float h, void* texture_offset, const Color& color = Color(255,255,255,255));
        
        void DrawText(float x, float y, const std::string& text, float size, const Color& color);

    private:
        Renderer() {}
        
        void InitTiny3D();
        void LoadFonts(); // Freetype integration
    };

} // namespace Graphics
