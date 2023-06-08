#pragma once

#include "../commons.hpp"

namespace MangoEngine {
    template <typename EventCls>
    using EventCallback = std::function<void(EventCls &)>;

    #define declare_event(category, type, ...) \
    struct type##Event { \
        __VA_ARGS__ \
        EventCategory get_category() const { \
            return EventCategory::e##category; \
        } \
        static const char *GetCategoryName() { \
            return #category; \
        } \
        const char *get_category_name() const { \
            return GetCategoryName(); \
        } \
        EventType get_type() const { \
            return EventType::e##type; \
        } \
        static const char *GetTypeName() { \
            return #type; \
        } \
        const char *get_type_name() const { \
            return GetTypeName(); \
        } \
        std::string to_string() const; \
        static ::std::vector<EventCallback<type##Event>> callbacks; \
    };

    #define implement_event(type) \
    ::std::vector<EventCallback<type##Event>> type##Event::callbacks; \
    std::string type##Event::to_string() const

    enum class EventCategory {
        eWindow,
        eKey,
        eMouse,
    };

    enum class EventType {
        eWindowResized,
        eWindowMoved,
        eWindowClose,
        eKeyPressed,
        eKeyReleased,
        eMousePressed,
        eMouseReleased,
        eMouseMoved,
        eMouseScroll,
    };

    enum class MouseButton {
        eLeft,
        eMiddle,
        eRight,
    };

    declare_event(Window, WindowResized, u32 width, height;)
    declare_event(Window, WindowMoved, u32 x, y;)
    declare_event(Window, WindowClose)
    declare_event(Key, KeyPressed, u32 keycode;)
    declare_event(Key, KeyReleased, u32 keycode;)
    declare_event(Mouse, MousePressed, MouseButton button;)
    declare_event(Mouse, MouseReleased, MouseButton button;)
    declare_event(Mouse, MouseMoved, u32 x, y;)
    declare_event(Mouse, MouseScroll, i32 delta;)

    class EventSystem {
    public:
        template<typename EventCls>
        void dispatch(EventCls event) {
            MG_TRACE("Dispatch: {}-{}", EventCls::GetCategoryName(), EventCls::GetTypeName())
            for (const auto &callback : EventCls::callbacks) {
                callback(event);
            }
        }

        template<typename EventCls>
        void add_event_callback(EventCallback<EventCls> &&callback) {
            MG_DEBUG("Add {}-{} callback", EventCls::GetCategoryName(), EventCls::GetTypeName())
            EventCls::callbacks.push_back(callback);
        }

        declare_runtime_system(EventSystem)
    };
}
