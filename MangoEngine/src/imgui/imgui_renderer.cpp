#include "MangoEngine/imgui/imgui_renderer.hpp"
#include "MangoEngine/render/render.hpp"

namespace MangoEngine {
    ImGuiBackend::~ImGuiBackend() = default;
    void ImGuiBackend::begin_imgui() {}
    void ImGuiBackend::end_imgui() {}

    implement_runtime_system_start(ImGuiRenderer, imgui_renderer)
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.DisplaySize.x = static_cast<float>(render_system->get_context().get_width());
        io.DisplaySize.y = static_cast<float>(render_system->get_context().get_height());

        switch (engine_config->api) {
        case RenderAPI::eNone:
            MG_FATAL("Not impl for None yet.")
            break;
        case RenderAPI::eOpenGL:
            MG_FATAL("Not impl for OpenGL yet.")
            break;
        case RenderAPI::eVulkan:
            _instance.reset(new ImGuiRenderer(get_imgui_backend<RenderAPI::eVulkan>()));
            break;
        case RenderAPI::eDirectX:
            MG_FATAL("Not impl for DirectX yet.")
            break;
        case RenderAPI::eMetal:
            MG_FATAL("Not impl for Metal yet.")
            break;
        }

        event_system->add_event_callback<WindowResizedEvent>([&](auto &event) {
            io.DisplaySize.x = event.width;
            io.DisplaySize.y = event.height;
        });
        event_system->add_event_callback<WindowFocusdEvent>([&](auto &event) {
            io.AddFocusEvent(true);
        });
        event_system->add_event_callback<WindowLostFocusEvent>([&](auto &event) {
            io.AddFocusEvent(false);
        });
        event_system->add_event_callback<KeyPressedEvent>([&](auto &event) {
            io.AddKeyEvent(key2imgui(event.key), true);
            if (is_char(event.key) == MG_TRUE) {
                io.AddInputCharacter(static_cast<u8>(to_char(event.key)));
            }
        });
        event_system->add_event_callback<KeyReleasedEvent>([&](auto &event) {
            io.AddKeyEvent(key2imgui(event.key), false);
        });
        event_system->add_event_callback<MousePressedEvent>([&](auto &event) {
            io.AddMouseButtonEvent(static_cast<int>(event.button), true);
        });
        event_system->add_event_callback<MouseReleasedEvent>([&](auto &event) {
            io.AddMouseButtonEvent(static_cast<int>(event.button), false);
        });
        event_system->add_event_callback<MouseMovedEvent>([&](auto &event) {
            io.AddMousePosEvent(event.x, event.y);
        });
        event_system->add_event_callback<MouseScrollEvent>([&](auto &event) {
            io.AddMouseWheelEvent(0, event.delta);
        });
    implement_runtime_system_end(ImGuiRenderer, imgui_renderer)

    ImGuiRenderer::ImGuiRenderer(ImGuiBackend *backend) {
        this->backend.reset(backend);
    }

    ImGuiRenderer::~ImGuiRenderer() {
        backend.reset();
    }

    void ImGuiRenderer::begin_imgui() {
        backend->begin_imgui();
        ImGui::NewFrame();
    }

    void ImGuiRenderer::end_imgui() {
        ImGui::EndFrame();
        backend->end_imgui();
    }

    #define case_key(key_name) \
    case Key::e##key_name: \
        return ImGuiKey_##key_name;

    ImGuiKey key2imgui(Key key) {
        switch (key) {
        case_key(Tab)
        case_key(LeftArrow)
        case_key(RightArrow)
        case_key(UpArrow)
        case_key(DownArrow)
        case_key(PageUp)
        case_key(PageDown)
        case_key(Home)
        case_key(End)
        case_key(Insert)
        case_key(Delete)
        case_key(Backspace)
        case_key(Space)
        case_key(Enter)
        case_key(Escape)
        case_key(LeftCtrl)
        case_key(LeftShift)
        case_key(LeftAlt)
        case_key(LeftSuper)
        case_key(RightCtrl)
        case_key(RightShift)
        case_key(RightAlt)
        case_key(RightSuper)
        case_key(Menu)
        case_key(0)
        case_key(1)
        case_key(2)
        case_key(3)
        case_key(4)
        case_key(5)
        case_key(6)
        case_key(7)
        case_key(8)
        case_key(9)
        case_key(A)
        case_key(B)
        case_key(C)
        case_key(D)
        case_key(E)
        case_key(F)
        case_key(G)
        case_key(H)
        case_key(I)
        case_key(J)
        case_key(K)
        case_key(L)
        case_key(M)
        case_key(N)
        case_key(O)
        case_key(P)
        case_key(Q)
        case_key(R)
        case_key(S)
        case_key(T)
        case_key(U)
        case_key(V)
        case_key(W)
        case_key(X)
        case_key(Y)
        case_key(Z)
        case_key(F1)
        case_key(F2)
        case_key(F3)
        case_key(F4)
        case_key(F5)
        case_key(F6)
        case_key(F7)
        case_key(F8)
        case_key(F9)
        case_key(F10)
        case_key(F11)
        case_key(F12)
        case_key(Apostrophe)
        case_key(Comma)
        case_key(Minus)
        case Key::eDot:
            return ImGuiKey_Period;
        case_key(Slash)
        case_key(Semicolon)
        case_key(Equal)
        case_key(LeftBracket)
        case_key(RightBracket)
        case_key(Backslash)
        case_key(GraveAccent)
        case_key(CapsLock)
        case_key(ScrollLock)
        case_key(NumLock)
        case_key(PrintScreen)
        case_key(Pause)
        case Key::MAX_NUM:
            return ImGuiKey_None;
        }
    }

    #undef case_key
}
