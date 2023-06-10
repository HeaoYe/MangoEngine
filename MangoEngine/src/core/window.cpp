#include "MangoEngine/core/window.hpp"
#include "MangoEngine/core/event.hpp"
#include "MangoEngine/core/input.hpp"
#include <GLFW/glfw3.h>

namespace MangoEngine {
    implement_runtime_system(WindowSystem, window_system)

    WindowSystem::WindowSystem() {
        x = engine_config->window_x;
        y = engine_config->window_y;
        width = engine_config->window_width;
        height = engine_config->window_height;
        title = engine_config->title;

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_POSITION_X, x);
        glfwWindowHint(GLFW_POSITION_Y, y);
        native_pointer = static_cast<void *>(glfwCreateWindow(width, height, title, nullptr, nullptr));

        glfwSetFramebufferSizeCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, int width, int height){
            window_system->width = static_cast<u32>(width);
            window_system->height = static_cast<u32>(height);
            event_system->dispatch<WindowResizedEvent>({ .width = window_system->width, .height = window_system->height });
        });

        glfwSetWindowPosCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, int x, int y) {
            window_system->x = static_cast<u32>(x);
            window_system->y = static_cast<u32>(y);
            event_system->dispatch<WindowMovedEvent>({ .x = window_system->x, .y = window_system->y });
        });

        glfwSetWindowCloseCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *) {
            event_system->dispatch<WindowClosedEvent>({});
        });

        glfwSetWindowFocusCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, int focused) {
            switch (focused) {
            case GLFW_TRUE:
                event_system->dispatch<WindowFocusdEvent>({});
                break;
            case GLFW_FALSE:
                event_system->dispatch<WindowLostFocusEvent>({});
                break;
            default:
                break;
            }
        });

        glfwSetKeyCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, int keycode, int scancode, int action, int mods) {
            auto key = transform<int, Key>(keycode);
            if (key >= Key::MAX_NUM) {
                return;
            }
            switch (action) {
            case GLFW_PRESS:
                input_system->states[input_system->current_state].key_states[static_cast<u32>(key)] = MG_DOWN;
                event_system->dispatch<KeyPressedEvent>({ .key = key });
                break;
            case GLFW_RELEASE:
                input_system->states[input_system->current_state].key_states[static_cast<u32>(key)] = MG_UP;
                event_system->dispatch<KeyReleasedEvent>({ .key = key });
                break;
            default:
                break;
            }
        });

        glfwSetCharCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, unsigned int codepoint) {
            event_system->dispatch<InputCharEvent>({ .data = codepoint });
        });

        glfwSetMouseButtonCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, int button, int action, int mods) {
            auto btn = transform<int, MouseButton>(button);
            if (btn >= MouseButton::MAX_NUM)
                return;
            switch (action) {
            case GLFW_PRESS:
                input_system->states[input_system->current_state].mouse_button_states[static_cast<u32>(btn)] = MG_DOWN;
                event_system->dispatch<MousePressedEvent>({ .button = btn });
                break;
            case GLFW_RELEASE:
                input_system->states[input_system->current_state].mouse_button_states[static_cast<u32>(btn)] = MG_UP;
                event_system->dispatch<MouseReleasedEvent>({ .button = btn });
                break;
            default:
                break;
            }
        });

        glfwSetCursorPosCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, double xpos, double ypos) {
            input_system->states[input_system->current_state].mouse_x = static_cast<u32>(xpos);
            input_system->states[input_system->current_state].mouse_y = static_cast<u32>(ypos);
            event_system->dispatch<MouseMovedEvent>({ .x = static_cast<i32>(xpos), .y = static_cast<i32>(ypos) });
        });

        glfwSetScrollCallback(static_cast<GLFWwindow *>(native_pointer), [](GLFWwindow *, double xoffset, double yoffset) {
            input_system->states[input_system->current_state].mouse_scroll_x = static_cast<u32>(xoffset);
            input_system->states[input_system->current_state].mouse_scroll_y = static_cast<u32>(yoffset);
            event_system->dispatch<MouseScrollEvent>({ .delta_x = static_cast<i32>(xoffset), .delta_y = static_cast<i32>(yoffset) });
        });
    }

    WindowSystem::~WindowSystem() {
        glfwDestroyWindow(static_cast<GLFWwindow *>(native_pointer));
        native_pointer = nullptr;
        glfwTerminate();
    }

    Bool WindowSystem::pull_events() {
        glfwPollEvents();
        return glfwWindowShouldClose(static_cast<GLFWwindow *>(native_pointer)) ? MG_FALSE : MG_TRUE;
    }
}
