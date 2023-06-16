#include "MangoEngine/render/camera/camera.hpp"
#include "MangoEngine/render/camera/orthographic_camera.hpp"
#include "MangoEngine/render/render.hpp"

namespace MangoEngine {
    implement_runtime_system(CameraSystem, camera_system)

    CameraSystem::CameraSystem() = default;
    CameraSystem::~CameraSystem() = default;

    std::shared_ptr<OrthographicCamera> CameraSystem::create_orthographic_camera(glm::vec3 pos, glm::vec2 size, f32 depth) {
        auto camera = std::make_shared<OrthographicCamera>(pos, size, depth);
        cameras.push_back(std::reinterpret_pointer_cast<Camera>(camera));
        return std::move(camera);
    }
}
