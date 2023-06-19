#pragma once

#include "../commons.hpp"
#include "MangoEngine/core/logger.hpp"
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

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
    struct ComponmentHandle;

    template<typename ComponentType>
    struct ComponmentPool {
        friend ComponmentHandle<ComponentType>;
        u32 add_componment(u32 entity_id, ComponentType &&componment) {
            return componments.emplace(entity_id, std::forward<ComponentType>(componment));
        }
    private:
        template<typename ComponmentType>
        struct SpecialComponment {
            SpecialComponment(u32 entity_id, ComponentType &&componment) : entity_id(entity_id), inner(std::forward<ComponentType>(componment)) {}
            u32 entity_id;
            ComponmentType inner;
        };

        IDPool<SpecialComponment<ComponentType>> componments;
    };

    template <typename ComponmentType>
    using ComponmentHandler = std::function<void(u32, ComponmentType &)>;

    template<typename ComponentType>
    struct ComponmentHandle {
        ComponmentHandle(ComponmentPool<ComponentType> *pool) : pool(pool) {}
        void for_each(const ComponmentHandler<ComponentType> &handler) {
            std::for_each(pool->componments.begin(), pool->componments.end(), [&](auto &componment) {
                handler(componment.entity_id, componment.inner);
            });
        }
    private:
        ComponmentPool<ComponentType> *pool;
    };

    template<typename ComponentType>
    using System = std::function<void (ComponmentHandle<ComponentType> &)>;

    class World {
        template<typename ComponentType>
        friend struct SystemHandler;
    private:
        template<typename ComponentType>
        void create_componment(u32 entity_id, std::vector<std::pair<u32, u32>> &ids, ComponentType &&componment) {
            u32 cls_id = get_componment_cls_id<ComponentType>();
            auto *pool = reinterpret_cast<ComponmentPool<ComponentType> *>(componment_pools[cls_id]);
            u32 id = pool->add_componment(entity_id, std::forward<ComponentType>(componment));
            ids.push_back(std::make_pair(cls_id, id));
        }

        template<typename ComponentType, typename ...Others>
        void create_componment(u32 entity_id, std::vector<std::pair<u32, u32>> &ids, ComponentType &&componment, Others && ...others) {
            create_componment<ComponentType>(entity_id, ids, std::forward<ComponentType>(componment));
            create_componment<Others...>(entity_id, ids, std::forward<Others>(others)...);
        }

        std::vector<void *> componment_pools;
        // entity_id to componment id(s)
        IDPool<std::vector<std::pair<u32, u32>>> entities;
        // componment_id to entity_id
        // IDPool<u32> componments;

    public:
        template<typename ...ComponentType>
        void create_entity(ComponentType && ...componments) {
            u32 id = entities.push(std::move(std::vector<std::pair<u32, u32>>(sizeof...(componments))));
            create_componment<ComponentType...>(id, entities.at(id), std::forward<ComponentType>(componments)...);
        }

    private:
        template<typename ComponentType>
        u32 get_componment_cls_id() {
            static u32 i = [=]() constexpr {
                componment_pools.push_back(reinterpret_cast<void *>(new ComponmentPool<ComponentType>()));
                return _current_componment_cls_id++;
            }();
            return i;
        }
        u32 _current_componment_cls_id = 0;

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
                ComponmentHandle<ComponentType> handle { reinterpret_cast<ComponmentPool<ComponentType> *>(world.componment_pools[world.get_componment_cls_id<ComponentType>()]) };
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
    };
}
