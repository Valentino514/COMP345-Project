#ifndef ILOGGABLE_H
#define ILOGGABLE_H

#include <string>

// Interface
class ILoggable {
public:
    virtual ~ILoggable() = default;
    virtual std::string stringToLog() const = 0;
};

#endif
