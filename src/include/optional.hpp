#pragma once

template<typename T>
class optional {
public:
    optional() : has_value(false) {}
    optional(T val) : has_value(true), m_value(val){}

    T value(){ return m_value; }
    T value_or(T val){
        if(has_value){
            return m_value;
        } else {
            return val;
        }
    }

    operator bool() { return has_value; }

    const T& operator=(const T& val){ has_value = true; m_value = val; return val; }

    bool has_value = false;
private:
    T m_value;
};