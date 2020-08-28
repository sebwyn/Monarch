#pragma once

#include "events/Event.hpp"

#include <vector>
#include <iterator>
#include <memory>
#include <iostream>

namespace Monarch {

class Layer {
public:

    virtual ~Layer(){}

    virtual void init() = 0;
    virtual void destroy(){};

    virtual void update() = 0;

    virtual bool onEvent(Event& e) = 0;
};

class LayerStack {
public:
    //TODO: typecheck?
    template<typename T, typename... args>
    void pushLayer(args&&... _args){
        if(topLayer == layers.begin()){
            layers.emplace_back(new T(std::forward<args>(_args)...));
            topLayer = layers.begin();
        } else {
            topLayer = layers.emplace(topLayer, new T(std::forward<args>(_args)...));
        }
        (*topLayer)->init();
        topLayer++;
    }

    //TODO: typecheck?
    template<typename T, typename... args>
    void pushOverlay(args&&... _args){
        layers.emplace_back(new T(std::forward<args>(_args)...));
        (layers.back())->init();
    }
    
    struct Iterators {
        std::vector<std::unique_ptr<Layer>>::iterator begin, end;
        std::vector<std::unique_ptr<Layer>>::reverse_iterator rbegin, rend;
    };

    Iterators getIterators(){return {layers.begin(), layers.end(), layers.rbegin(), layers.rend()};}
private:
    std::vector<std::unique_ptr<Layer>> layers = std::vector<std::unique_ptr<Layer>>();
    std::vector<std::unique_ptr<Layer>>::iterator topLayer = layers.begin();
};

}
