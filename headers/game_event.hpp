
#pragma once
#include <string>

class GameEvent
{
public:
    virtual ~GameEvent() = default;

    virtual void trigger() = 0;

    virtual std::string getEventName() const = 0;
    virtual bool isActive() const = 0;
    virtual void setActive(bool active) = 0;
};