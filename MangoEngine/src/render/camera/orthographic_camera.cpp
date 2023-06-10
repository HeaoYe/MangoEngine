#include "MangoEngine/render/camera/orthographic_camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace MangoEngine {
    OrthographicCamera::OrthographicCamera(glm::vec3 pos, glm::vec2 size, f32 depth) {
        this->pos = pos;
        this->size = size;
        this->depth = depth;
        this->zoom = 1.0f;
        update();
    }

    void OrthographicCamera::update() {
        view() = glm::scale(glm::mat4(1), { zoom, zoom, 1.0f / depth }) * glm::lookAt(pos, { pos.x, pos.y, pos.z - 1 }, { 0, 1, 0 });
        project() = glm::ortho(-size.x * 0.5f, size.x * 0.5f, size.y * 0.5f, -size.y * 0.5f);
    }
}
