#pragma once

template <typename T, int UniqueID = 0>
class GameValue
{
    T current;

public:
    GameValue(T init = T{}) : current(init) {}
    T get() const { return current; }
    void set(T value) { current = value; }
    operator T() const { return current; }
};