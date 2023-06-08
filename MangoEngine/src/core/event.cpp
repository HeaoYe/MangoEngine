#include "MangoEngine/core/event.hpp"

namespace MangoEngine {
    implement_event(WindowResized) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "width: " << width << ", height: " << height;
        return ss.str();
    }

    implement_event(WindowMoved) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "x: " << x << ", y: " << y;
        return ss.str();
    }

    implement_event(WindowClose) {
        std::stringstream ss;
        ss << get_type_name();
        return ss.str();
    }

    implement_event(KeyPressed) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "key: " << key;
        return ss.str();
    }

    implement_event(KeyReleased) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "key: " << key;
        return ss.str();
    }

    implement_event(MousePressed) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "button: " << (u32)button;
        return ss.str();
    }

    implement_event(MouseReleased) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "button: " << (u32)button;
        return ss.str();
    }

    implement_event(MouseMoved) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "x: " << x << ", y: " << y;
        return ss.str();
    }

    implement_event(MouseScroll) {
        std::stringstream ss;
        ss << get_type_name() << ": " << "delta: " << delta;
        return ss.str();
    }

    implement_runtime_system_start(EventSystem)
        _instance.reset(new EventSystem);
    implement_runtime_system_end(EventSystem, event_system)

    EventSystem::EventSystem() = default;

    EventSystem::~EventSystem() = default;
}
