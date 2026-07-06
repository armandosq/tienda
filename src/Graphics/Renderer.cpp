#include "Renderer.hpp"
#include <iostream>
#include <rsx/rsx.h>

namespace Graphics {

    void Renderer::Initialize() {
        InitTiny3D();
        LoadFonts();
    }

    void Renderer::InitTiny3D() {
        tiny3d_Init(1024 * 1024);
        
        // Configuración de pantalla
        videoState state;
        videoGetState(0, 0, &state);
        
        // Set matrices for 2D UI
        // tiny3d_Project2D();
    }

    void Renderer::LoadFonts() {
        // Inicializar freetype para RSX
    }

    void Renderer::Clear(const Color& color) {
        tiny3d_Clear(color.ToHex(), TINY3D_CLEAR_ALL);
        tiny3d_AlphaTest(1, 0, TINY3D_ALPHA_FUNC_GEQUAL);
        tiny3d_BlendFunc(1, TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA, TINY3D_BLEND_FUNC_SRC_ALPHA_ONE_MINUS_SRC_ALPHA,
                         TINY3D_BLEND_RGB_FUNC_ADD, TINY3D_BLEND_ALPHA_FUNC_ADD);
    }

    void Renderer::Flip() {
        tiny3d_Flip();
    }

    void Renderer::DrawRect(float x, float y, float w, float h, const Color& color) {
        tiny3d_SetPolygon(TINY3D_QUADS);
        tiny3d_VertexPos(x, y, 65535.0f);
        tiny3d_VertexColor(color.ToHex());
        
        tiny3d_VertexPos(x + w, y, 65535.0f);
        tiny3d_VertexColor(color.ToHex());
        
        tiny3d_VertexPos(x + w, y + h, 65535.0f);
        tiny3d_VertexColor(color.ToHex());
        
        tiny3d_VertexPos(x, y + h, 65535.0f);
        tiny3d_VertexColor(color.ToHex());
        tiny3d_End();
    }

    void Renderer::DrawImage(float x, float y, float w, float h, void* texture_offset, const Color& color) {
        if (!texture_offset) return;

        // Suponiendo que el texture manager ya nos dio un offset válido en VRAM
        // En un caso real habría que tener el width y pitch original de la textura
        // Aquí pasamos valores genéricos para que tiny3d lo tome como textura activa.
        // Lo ideal es que Texture tenga un método para bindear.
        uint32_t tex_offset;
        rsxAddressToOffset(texture_offset, &tex_offset);
        tiny3d_SetTexture(0, tex_offset, w, h, w * 4, TINY3D_TEX_FORMAT_A8R8G8B8, TEXTURE_LINEAR);

        tiny3d_SetPolygon(TINY3D_QUADS);
        
        tiny3d_VertexPos(x, y, 65535.0f);
        tiny3d_VertexColor(color.ToHex());
        tiny3d_VertexTexture(0.0f, 0.0f);
        
        tiny3d_VertexPos(x + w, y, 65535.0f);
        tiny3d_VertexColor(color.ToHex());
        tiny3d_VertexTexture(1.0f, 0.0f);
        
        tiny3d_VertexPos(x + w, y + h, 65535.0f);
        tiny3d_VertexColor(color.ToHex());
        tiny3d_VertexTexture(1.0f, 1.0f);
        
        tiny3d_VertexPos(x, y + h, 65535.0f);
        tiny3d_VertexColor(color.ToHex());
        tiny3d_VertexTexture(0.0f, 1.0f);
        
        tiny3d_End();
    }

    void Renderer::DrawText(float x, float y, const std::string& text, float size, const Color& color) {
        // Renderizado de fuente freetype con tiny3d
    }

} // namespace Graphics
