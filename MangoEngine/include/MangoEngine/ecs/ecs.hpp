#pragma once

#include "../commons.hpp"

namespace MangoEngine {
    template <typename T>
    struct IDPool {
        T &at(u32 id) {
            return datas[id].value();
        }

        const T& at(u32 id) const {
            return datas[id];
        }

        u32 push(T &&t) {
            u32 id;
            if (available_indices.empty()) {
                id = datas.size();
                datas.push_back(std::forward<T>(t));
            } else {
                id = available_indices.back();
                datas[id] = std::forward<T>(t);
                available_indices.pop_back();
            }
            return id;
        }

        template<typename ...Args>
        u32 emplace(Args && ...args) {
            u32 id;
            if (available_indices.empty()) {
                id = datas.size();
                datas.emplace_back().emplace(std::forward<Args>(args)...);
            } else {
                id = available_indices.back();
                datas[id].emplace(std::forward<Args>(args)...);
                available_indices.pop_back();
            }
            return id;
        }

        void pop(u32 id) {
            datas[id].reset();
            available_indices.push_back(id);
        }

        struct iterator {
            iterator(typename std::vector<std::optional<T>>::iterator iter, typename std::vector<std::optional<T>>::iterator end) : iter(iter), end(end) {}

            void operator++() {
                iter ++;
                while (iter != end && !iter->has_value()) {
                    iter ++;
                }
            }

            bool operator!=(const iterator &other) {
                return iter != other.iter;
            }

            T &operator*() {
                return iter->value();
            }

            T *operator->() {
                return &iter->value();
            }
        private:
            typename std::vector<std::optional<T>>::iterator iter, end;
        };

        iterator begin() {
            return iterator(datas.begin(), datas.end());
        }

        iterator end() {
            return iterator(datas.end(), datas.end());
        }

    private:
        std::vector<std::optional<T>> datas;
        std::vector<u32> available_indices;
    };

    template<typename ComponentType>
    struct ComponentHandle;

    struct EntityQuery;

    template<typename ComponentType>
    struct ComponentPool {
        friend ComponentHandle<ComponentType>;
        friend EntityQuery;
        u32 add_component(u32 entity_id, ComponentType &&component) {
            return components.emplace(entity_id, std::forward<ComponentType>(component));
        }
    private:
        template<typename ComponentType_>
        struct SpecialComponent {
            SpecialComponent(u32 entity_id, ComponentType_ &&component) : entity_id(entity_id), inner(std::forward<ComponentType_>(component)) {}
            u32 entity_id;
            ComponentType_ inner;
        };

        IDPool<SpecialComponent<ComponentType>> components;
    };

    template <typename ComponentType>
    using ComponentHandler = std::function<void(u32, ComponentType &)>;

    template<typename ComponentType>
    struct ComponentHandle {
        ComponentHandle(ComponentPool<ComponentType> *pool) : pool(pool) {}
        void for_each(const ComponentHandler<ComponentType> &handler) {
            std::for_each(pool->components.begin(), pool->components.end(), [&](auto &component) {
                handler(component.entity_id, component.inner);
            });
        }
    private:
        ComponentPool<ComponentType> *pool;
    };

    template<typename EventType>
    using EventHandler = std::function<void(u32, const EntityQuery &, EventType &)>;

    template<typename ComponentType>
    using System = std::function<void (ComponentHandle<ComponentType> &)>;

    class World {
        template<typename ComponentType>
        friend struct SystemHandler;
        friend EntityQuery;
    private:
        template<typename ComponentType>
        void create_component(u32 entity_id, std::vector<std::pair<u32, u32>> &ids, ComponentType &&component) {
            u32 cls_id = get_component_cls_id<std::decay_t<ComponentType>>();
            auto *pool = reinterpret_cast<ComponentPool<ComponentType> *>(component_pools[cls_id]);
            u32 id = pool->add_component(entity_id, std::forward<ComponentType>(component));
            ids.push_back(std::make_pair(cls_id, id));
        }

        template<typename ComponentType, typename ...Others>
        void create_component(u32 entity_id, std::vector<std::pair<u32, u32>> &ids, ComponentType &&component, Others && ...others) {
            create_component<ComponentType>(entity_id, ids, std::forward<ComponentType>(component));
            create_component<Others...>(entity_id, ids, std::forward<Others>(others)...);
        }

        struct EntityManager {
            u32 push(std::vector<std::pair<u32, u32>> &&components) {
                handlers.push(std::move(std::vector<std::pair<u32, void *>>(0)));
                return entities.push(std::forward<std::vector<std::pair<u32, u32>>>(components));
            }

            std::vector<std::pair<u32, u32>> &get_components(u32 id) {
                return entities.at(id);
            }

            std::vector<std::pair<u32, void *>> &get_handlers(u32 id) {
                return handlers.at(id);
            }
        private:
            IDPool<std::vector<std::pair<u32, u32>>> entities;
            IDPool<std::vector<std::pair<u32, void *>>> handlers;
        } entity_manager;

        std::vector<void *> component_pools;

    public:
        template<typename ...ComponentType>
        u32 create_entity(ComponentType && ...components) {
            u32 id = entity_manager.push(std::move(std::vector<std::pair<u32, u32>>(sizeof...(components))));
            create_component<ComponentType...>(id, entity_manager.get_components(id), std::forward<ComponentType>(components)...);
            return id;
        }

    private:
        template<typename ComponentType>
        u32 get_component_cls_id() {
            static u32 i = [=]() constexpr {
                component_pools.push_back(reinterpret_cast<void *>(new ComponentPool<ComponentType>()));
                return _current_component_cls_id++;
            }();
            return i;
        }
        u32 _current_component_cls_id = 0;

    private:
        struct SystemHandler {
            friend World;
            virtual void handle(World &world) = 0;
            virtual void disable() { enabled = MG_FALSE; }
            virtual void enable() { enabled = MG_TRUE; }
        protected:
            Bool enabled = MG_TRUE;
        };

        template<typename ComponentType>
        struct SpecialSystemHandler : public SystemHandler {
            SpecialSystemHandler(System<ComponentType> &&system) : system(std::forward<System<ComponentType>>(system)) {}
            void handle(World &world) override {
                ComponentHandle<ComponentType> handle { reinterpret_cast<ComponentPool<ComponentType> *>(world.component_pools[world.get_component_cls_id<std::decay_t<ComponentType>>()]) };
                system(handle);
            }
        private:
            System<ComponentType> system;
        };

        IDPool<SystemHandler *> systems;

    public:
        template<typename ComponentType>
        u32 add_system(System<ComponentType> &&system) {
            return systems.push(new SpecialSystemHandler<ComponentType> { std::forward<System<ComponentType>>(system) });
        }

        void remove_system(u32 id) {
            systems.pop(id);
        }

        void disable_system(u32 id) {
            systems.at(id)->disable();
        }

        void enable_system(u32 id) {
            systems.at(id)->enable();
        }

        void tick() {
            std::for_each(systems.begin(), systems.end(), [&](auto &system) {
                if (system->enabled == MG_TRUE) {
                    system->handle(*this);
                }
            });
        }

    public:
        template<typename EventType>
        void attach_event_handler(u32 entity_id, EventHandler<EventType> &&handler) {
            u32 event_type_id = get_event_type_id<std::decay_t<EventType>>();
            auto &handlers = entity_manager.get_handlers(entity_id);
            handlers.push_back(std::make_pair(event_type_id, static_cast<void *>(new EventHandlerWrapper<EventType> { std::forward<EventHandler<EventType>>(handler) })));
        }

        template<typename EventType>
        void send_event(u32 src_entity, u32 dst_entity, EventType &&event) {
            u32 event_type_id = get_event_type_id<std::decay_t<EventType>>();
            auto &handlers = entity_manager.get_handlers(dst_entity);
            for (auto [_event_type_id, handler] : handlers) {
                if (_event_type_id == event_type_id) {
                    reinterpret_cast<EventHandlerWrapper<EventType> *>(handler)->handler(
                        src_entity,
                        { *this, dst_entity },
                        event
                    );
                }
            }
        }

    private:
        template<typename EventType>
        struct EventHandlerWrapper {
            EventHandler<EventType> handler;
        };
        template<typename EventType>
        u32 get_event_type_id() {
            static u32 i = _current_event_type_id++;
            return i;
        }
        u32 _current_event_type_id = 0;
    };

    struct EntityQuery {
        EntityQuery(World &world, u32 entity_id) : world(world), entity_id(entity_id), components(world.entity_manager.get_components(entity_id)) {}
        template<typename ComponentType>
        u32 get_component_count() const {
            u32 count = 0;
            u32 cls_id = world.get_component_cls_id<std::decay_t<ComponentType>>();
            for (auto [component_cls_id, component_id] : components) {
                if (cls_id == component_cls_id) {
                    count ++;
                }
            }
            return count;
        }
        template<typename ComponentType>
        ComponentType &get_component(u32 index = 0) const {
            u32 cls_id = world.get_component_cls_id<std::decay_t<ComponentType>>();
            u32 count = 0;
            for (auto [component_cls_id, component_id] : components) {
                if (cls_id == component_cls_id) {
                    if (count == index) {
                        return reinterpret_cast<ComponentPool<ComponentType> *>(world.component_pools[cls_id])->components.at(component_id).inner;
                    } else {
                        count ++;
                    }
                }
            }
            MG_ERROR("Component [{}, {}] not found", cls_id, index)
            ComponentType *null = nullptr;
            return *null;
        }
    private:
        World &world;
        u32 entity_id;
        std::vector<std::pair<u32, u32>> &components;
    };
}
