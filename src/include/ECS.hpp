#pragma once

#include <memory>
#include <array>
#include <utility>
#include <algorithm>
#include <vector>
#include <bitset>
#include <iostream>

#include "events/Event.hpp"

namespace Monarch {
    class Component;
    class Entity;
    class Manager;
}

const int max_component_types = 32;

inline int componentTypeID(){
    static int id = 0;
    return id++;
}

template<typename T>
inline int getComponentTypeID(){
    static int typeID = componentTypeID();
    //std::cout << "TypeID : " << typeID << std::endl;
    return typeID;
}

namespace Monarch {

    class Component{
    protected:
        Entity* m_entity;
    public:
        virtual void init() {};
        virtual void update() {};
        virtual void draw() {};
        virtual bool onEvent(Event& e){return false;};
        
        virtual ~Component() {std::cout << "component deleted" << std::endl;};

        friend Entity;
    };

    class Entity {
        public:
            virtual void update(){
                for(auto& component : components){
                    component->update();
                }
            }

            virtual void draw(){
                for (auto& component : components){
                    component->draw();
                }
            }

            virtual bool onEvent(Event& e){
                bool handled = false;
                for (auto& component : components){
                    bool componentHandled = component->onEvent(e);
                    if(componentHandled) handled = true;
                }
                return handled;
            }

            void destroy() {alive = false;}

            bool destroyed() const {return !alive;}

            template<typename T>
            bool hasComponent() const {
                return typeList[getComponentTypeID<T>()];
            }

            template<typename T>
            T& getComponent() const{
                if (!hasComponent<T>()){
                    std::cout << "COULDN'T GET COMPONENT: "
                        << __PRETTY_FUNCTION__ << std::endl;
                    exit(-1);
                }
                Component* component = componentLookup[getComponentTypeID<T>()];
                return *static_cast<T*>(component);
            } 

            template<typename T, typename... args>
            T& addComponent(args&&... _args){
                //Here we are making a unique_ptr directly in the data_structure
                //this unique_ptr constructor could lead to memory leaks
                //TODO: use make_unique in more modern c++
                components.emplace_back(new T(std::forward<args>(_args)...));
                T* component = static_cast<T*>(components.back().get());

                component->m_entity = this;
                component->init();

                unsigned int typeID = getComponentTypeID<T>();
                typeList[typeID] = true;
                componentLookup[typeID] = component;

                return *component;
            }
        private:
            bool alive = true;

            std::vector<std::shared_ptr<Component>> components;
            std::array<Component*,max_component_types> componentLookup;
            std::bitset<max_component_types> typeList;
    };

    //TODO: work on recycling entities (and consider making it more virtual)
    //also consider being able to query for entities
    class Manager {
    private:
        std::vector<std::shared_ptr<Entity>> entities;
    public:

        Entity& addEntity(){
            entities.emplace_back(new Entity());
            return *(entities.back().get());
        }

        template<typename T, typename... args>        
        T& addEntity(args&&... _args){
            //TODO: make_unique
            entities.emplace_back(new T(std::forward<args>(_args)...));
            return *((T*)entities.back().get());
        }

        void cleanUp(){
            entities.erase(std::remove_if(std::begin(entities), std::end(entities), 
                [](std::shared_ptr<Entity> &entity){
                    return entity->destroyed();
                }), std::end(entities));
        }

        void update(){
            for (auto& entity : entities){
                entity->update();
            }
        }

        void draw(){
            for (auto& entity : entities){
                entity->draw();
            }
        }

        bool onEvent(Event& e){
            for(auto& entity : entities){
                if(entity->onEvent(e)){
                    return true;
                }   
            }
            return false;
        }
    };
}
