#pragma once

#include "../core/SpscQueue.h"
#include "Command.h"
#include <iostream>
#include <sstream>
#include <string>

namespace renderer {
class ConsoleInterface {
  public:
    static constexpr size_t QSIZE = 1024;
    using Queue                   = SpscQueue<Command, QSIZE>;

    explicit ConsoleInterface(Queue& queue);
    void run();

  private:
    Queue& m_queue;
    void showMenu();
    int readOption();
    std::string readLine(const std::string& prompt);
    Vec3 readVec3(const std::string& prompt);
    float readFloat(const std::string& prompt, float min, float max);
    Command readLoadObject();
    Command readAddLight();
    void waitForEnter();
};
} // namespace renderer
