#include "MangoEngine/render/camera/camera.hpp"
#include "MangoEngine/render/camera/orthographic_camera.hpp"
#include "MangoEngine/render/render.hpp"

namespace MangoEngine {
    implement_runtime_system(CameraSystem, camera_system)

    CameraSystem::CameraSystem() = default;
    CameraSystem::~CameraSystem() = default;

    void Camera::bind() {
        void *ptr = render_system->get_descriptor_set().lock()->get_uniform_buffer_pointer(0, 0);
        memcpy(ptr, datas, sizeof(glm::mat4) * 2);
    }

    Camera &CameraSystem::create_orthographic_camera(glm::vec3 pos, glm::vec2 size, f32 depth) {
        return *cameras.emplace_back(new OrthographicCamera(pos, size, depth));
    }
}
