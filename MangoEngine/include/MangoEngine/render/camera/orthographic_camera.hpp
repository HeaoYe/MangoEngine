#pragma once

#include "camera.hpp"

namespace MangoEngine {
    class OrthographicCamera : public Camera {
    public:
        OrthographicCamera(glm::vec3 pos, glm::vec2 extent, f32 depth);
        virtual void update() override;
        void set_extent(glm::vec2 extent);
        void set_extent(f32 width, f32 height) { set_extent({ width, height }); }
        glm::vec2 get_extent() const { return extent; }

    private:
        glm::vec2 extent;
        f32 depth;
    };
}
