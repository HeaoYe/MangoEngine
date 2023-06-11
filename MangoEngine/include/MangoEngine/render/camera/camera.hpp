#pragma once

#include "../../commons.hpp"

namespace MangoEngine {
    class Camera {
    public:
        virtual ~Camera() = default;
        virtual void update() = 0;
        virtual void bind();
        glm::mat4 &view() { return datas[0]; }
        glm::mat4 &project() { return datas[1]; }
    public:
        glm::vec3 pos;
        f32 zoom;
    protected:
        glm::mat4 datas[2];
    };

    class CameraSystem {
    public:
        Camera &create_orthographic_camera(glm::vec3 pos, glm::vec2 size, f32 depth);
    private:
        std::vector<std::unique_ptr<Camera>> cameras;

    declare_runtime_system(CameraSystem)
    };
    declare_runtime_system_alias(CameraSystem, camera_system)
};
