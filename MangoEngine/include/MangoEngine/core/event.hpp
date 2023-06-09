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
        eInput,
        eMouse,
    };

    enum class EventType {
        eWindowResized,
        eWindowMoved,
        eWindowClosed,
        eWindowFocusd,
        eWindowLostFocus,
        eKeyPressed,
        eKeyReleased,
        eInputChar,
        eMousePressed,
        eMouseReleased,
        eMouseMoved,
        eMouseScroll,
    };

    enum class MouseButton {
        eLeft,
        eMiddle,
        eRight,
        MAX_NUM
    };

    enum class Key {
        eTab,
        eLeftArrow,
        eRightArrow,
        eUpArrow,
        eDownArrow,
        ePageUp,
        ePageDown,
        eHome,
        eEnd,
        eInsert,
        eDelete,
        eBackspace,
        eSpace,
        eEnter,
        eEscape,
        eLeftCtrl, eLeftShift, eLeftAlt, eLeftSuper,
        eRightCtrl, eRightShift, eRightAlt, eRightSuper,
        eMenu,
        e0, e1, e2, e3, e4, e5, e6, e7, e8, e9,
        eA, eB, eC, eD, eE, eF, eG, eH, eI, eJ,
        eK, eL, eM, eN, eO, eP, eQ, eR, eS, eT,
        eU, eV, eW, eX, eY, eZ,
        eF1, eF2, eF3, eF4, eF5, eF6,
        eF7, eF8, eF9, eF10,eF11, eF12,
        eApostrophe,    // '
        eComma,         // ,
        eMinus,         // -
        eDot,           // .
        eSlash,         // /
        eSemicolon,     // ;
        eEqual,         // =
        eLeftBracket,   // [
        eRightBracket,  // ]
        eBackslash,     // '\'
        eGraveAccent,   // `
        eCapsLock,
        eScrollLock,
        eNumLock,
        ePrintScreen,
        ePause,
        MAX_NUM
    };
    std::string to_string(Key key);

    declare_event(Window, WindowResized, u32 width, height;)
    declare_event(Window, WindowMoved, u32 x, y;)
    declare_event(Window, WindowClosed)
    declare_event(Window, WindowFocusd)
    declare_event(Window, WindowLostFocus)
    declare_event(Key, KeyPressed, Key key;)
    declare_event(Key, KeyReleased, Key key;)
    declare_event(Input, InputChar, u32 data;)
    declare_event(Mouse, MousePressed, MouseButton button;)
    declare_event(Mouse, MouseReleased, MouseButton button;)
    declare_event(Mouse, MouseMoved, u32 x, y;)
    declare_event(Mouse, MouseScroll, i32 delta_x, delta_y;)

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
    declare_runtime_system_alias(EventSystem, event_system);
}
