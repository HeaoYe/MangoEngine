#pragma once

#include "../commons.hpp"

namespace MangoEngine {
    class Texture {
    public:
        static std::shared_ptr<Texture> load_from_file(const char *filename);
        static std::shared_ptr<Texture> generate();

        MangoRHI::Texture &get_texture_reference() { return *texture; }
        const u32 uuid;
    private:
        Texture(MangoRHI::Texture *raw_texture);
        std::unique_ptr<MangoRHI::Texture> texture;
    };
}
