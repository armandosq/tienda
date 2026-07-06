#include "TextureManager.hpp"
#include <png.h>
#include <iostream>

namespace Resources {

    Texture::Texture(const std::string& path) : m_offset(nullptr), m_width(0), m_height(0), m_pitch(0) {
        LoadPNG(path);
    }

    Texture::~Texture() {
        if (m_offset) {
            rsxFree(m_offset);
            m_offset = nullptr;
        }
    }

    bool Texture::LoadPNG(const std::string& path) {
        FILE *fp = fopen(path.c_str(), "rb");
        if (!fp) {
            std::cerr << "No se pudo abrir PNG: " << path << std::endl;
            return false;
        }

        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) { fclose(fp); return false; }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) { png_destroy_read_struct(&png_ptr, NULL, NULL); fclose(fp); return false; }

        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return false;
        }

        png_init_io(png_ptr, fp);
        png_read_info(png_ptr, info_ptr);

        m_width = png_get_image_width(png_ptr, info_ptr);
        m_height = png_get_image_height(png_ptr, info_ptr);
        png_byte color_type = png_get_color_type(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

        // Convertir formato para que la PS3 lo entienda como RGBA 8-bit
        if (bit_depth == 16) png_set_strip_16(png_ptr);
        if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
        
        if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE) {
            png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER); // Agregar alpha
        }
        if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
            png_set_gray_to_rgb(png_ptr); // Convertir gris a RGB
        }

        png_read_update_info(png_ptr, info_ptr);

        m_pitch = m_width * 4; // RGBA 32-bit
        
        // Asignar memoria de video (RSX) alineada
        m_offset = rsxMemalign(128, m_pitch * m_height);
        if (!m_offset) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return false;
        }

        // Leer pixeles directamente a memoria asignada temporal o rsx
        png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * m_height);
        for (uint32_t y = 0; y < m_height; y++) {
            row_pointers[y] = ((png_bytep)m_offset) + (y * m_pitch);
        }

        png_read_image(png_ptr, row_pointers);
        free(row_pointers);

        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);

        return true;
    }

    std::shared_ptr<Texture> TextureManager::GetTexture(const std::string& path) {
        auto it = m_textures.find(path);
        if (it != m_textures.end()) {
            return it->second;
        }

        auto tex = std::make_shared<Texture>(path);
        m_textures[path] = tex;
        return tex;
    }

    void TextureManager::GarbageCollect() {
        for (auto it = m_textures.begin(); it != m_textures.end(); ) {
            // Si solo el TextureManager tiene la referencia, la borramos
            if (it->second.use_count() == 1) {
                it = m_textures.erase(it);
            } else {
                ++it;
            }
        }
    }

} // namespace Resources
