#ifndef __LOG_HPP__
#define __LOG_HPP__

namespace RoadGL
{
    void Log (const char *tag, const char *message);

    // Simple logging functions, log only 1 variable
    void SimpleLog (const char *message);
    void SimpleLog (float message);
};

#endif // __LOG_HPP__
