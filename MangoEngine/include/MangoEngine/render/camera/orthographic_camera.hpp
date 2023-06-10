#pragma once

#include "camera.hpp"

namespace MangoEngine {
    class OrthographicCamera : public Camera {
    public:
        OrthographicCamera(glm::vec3 pos, glm::vec2 size, f32 depth);
        virtual void update() override;

    private:
        glm::vec2 size;
        f32 depth;
    };
}
