#include "ConsoleInterface.h"

#include <limits>

namespace renderer {

ConsoleInterface::ConsoleInterface(Queue& queue)
    : m_queue(queue) {
}

void ConsoleInterface::waitForEnter() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ConsoleInterface::run() {
    bool done = false;
    while (!done) {
        showMenu();
        int opt = readOption();
        Command cmd{};
        switch (opt) {
        case 1:
            cmd = readLoadObject();
            break;
        case 2:
            cmd = readAddLight();
            break;
        case 3:
            cmd.type = CmdType::Exit;
            done     = true;
            break;
        default:
            std::cerr << "Unknown option." << std::endl;
            continue;
        }
        if (!m_queue.enqueue(cmd)) {
            std::cerr << "Command queue full, skipping." << std::endl;
        }
        waitForEnter();
    }
}

void ConsoleInterface::showMenu() {
    std::cout << "\nOperations:\n"
              << " 1. Load object\n"
              << " 2. Add light\n"
              << " 3. Exit\n"
              << "Enter option: ";
}

int ConsoleInterface::readOption() {
    int o;
    if (!(std::cin >> o)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return o;
}

std::string ConsoleInterface::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    if (s.empty()) {
        std::getline(std::cin, s);
    }
    return s;
}

Vec3 ConsoleInterface::readVec3(const std::string& prompt) {
    Vec3 v;
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        std::istringstream ss(line);
        if (ss >> v.x >> v.y >> v.z) {
            break;
        }
        std::cerr << "Invalid vector format." << std::endl;
    }
    return v;
}

float ConsoleInterface::readFloat(const std::string& prompt, float min, float max) {
    float x;
    while (true) {
        try {
            x = std::stof(readLine(prompt));
        } catch (...) {
            std::cerr << "Invalid number." << std::endl;
            continue;
        }
        if (x < min || x > max) {
            std::cerr << "Value must be in [" << min << "," << max << "]."
                      << std::endl;
        } else {
            break;
        }
    }
    return x;
}

Command ConsoleInterface::readLoadObject() {
    Command c;
    c.type  = CmdType::LoadObject;
    c.path  = readLine("Enter path to obj file: ");
    auto yn = readLine("Do you want to apply transformations? (y/n): ");
    if (!yn.empty() && (yn[0] == 'y' || yn[0] == 'Y')) {
        c.applyTransform = true;
        c.rotAxis        = readVec3("Enter rotation axis in format x y z: ");
        c.rotAngle       = std::stof(readLine("Enter rotation angle: "));
        c.translation    = readVec3("Enter translation in format x y z: ");
    }
    return c;
}

Command ConsoleInterface::readAddLight() {
    Command c;
    c.type         = CmdType::AddLight;
    c.lightDir     = readVec3("Enter direction vector of light in format x y z: ");
    c.lightColor.x = readFloat("Enter red intensity of light (0 to 1): ", 0.f, 1.f);
    c.lightColor.y =
        readFloat("Enter green intensity of light (0 to 1): ", 0.f, 1.f);
    c.lightColor.z = readFloat("Enter blue intensity of light (0 to 1): ", 0.f, 1.f);
    return c;
}

} // namespace renderer
