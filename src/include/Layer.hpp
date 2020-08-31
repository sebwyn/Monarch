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
    std::shared_ptr<Layer> pushLayer(args&&... _args){
        if(topLayer == layers.begin()){
            layers.emplace_back(std::make_shared<T>(std::forward<args>(_args)...));
            topLayer = layers.begin();
        } else {
            topLayer = layers.emplace(topLayer, std::make_shared<T>(std::forward<args>(_args)...));
        }
        auto newLayer = topLayer;
        topLayer++;

        (*newLayer)->init();
        return *newLayer;
    }

    //TODO: typecheck?
    template<typename T, typename... args>
    std::shared_ptr<Layer> pushOverlay(args&&... _args){
        layers.emplace_back(std::make_shared<T>(std::forward<args>(_args)...));
        (layers.back())->init();
        return layers.back();
    }
    
    struct Iterators {
        std::vector<std::shared_ptr<Layer>>::iterator begin, end;
        std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin, rend;
    };

    Iterators getIterators(){return {layers.begin(), layers.end(), layers.rbegin(), layers.rend()};}
private:
    std::vector<std::shared_ptr<Layer>> layers = std::vector<std::shared_ptr<Layer>>();
    std::vector<std::shared_ptr<Layer>>::iterator topLayer = layers.begin();
};

}
