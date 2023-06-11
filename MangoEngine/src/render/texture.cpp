#include "MangoEngine/render/texture.hpp"
#include "MangoEngine/render/render.hpp"

namespace MangoEngine {
    std::shared_ptr<Texture> Texture::load_from_file(const char *filename) {
        auto texture = MangoRHI::get_context().get_resource_factory_reference().create_texture(filename);
        return std::move(std::shared_ptr<Texture>(new Texture(texture.release())));
    }

    std::shared_ptr<Texture> Texture::generate() {
        auto texture = MangoRHI::get_context().get_resource_factory_reference().create_empty_texture();
        return std::move(std::shared_ptr<Texture>(new Texture(texture.release())));
    }

    static u32 _current_uuid = 0;

    Texture::Texture(MangoRHI::Texture *raw_texture) : uuid(_current_uuid) {
        this->texture.reset(raw_texture);
        this->texture->destroy_before(&MangoRHI::get_context());
        _current_uuid ++;
    }
}
