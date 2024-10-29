#include "CommandProcessing.h"
#include "Map.h"
#include <iostream>

void testCommandProcessor(CommandProcessor* processor) {
    // Process commands until the user issues a "quit" command
    while (true) {
        processor->processInput();
        Command* cmd = processor->getCommand();

        if (cmd != nullptr) {
            std::cout << *cmd << std::endl;
            if (cmd->getCommand() == "quit") {
                break;
            }
        } else {
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " -console OR -file <filename>" << std::endl;
        return 1;
    }

    Map* gameMap = new Map();  // Create an instance of Map
    CommandProcessor* processor = nullptr;

    if (std::string(argv[1]) == "-console") {
        processor = new CommandProcessor(gameMap);
    } 
    else if (std::string(argv[1]) == "-file" && argc == 3) {
        processor = new FileCommandProcessorAdapter(gameMap, argv[2]);
    } 
    else {
        std::cerr << "Invalid arguments. Usage: " << argv[0] << " -console OR -file <filename>" << std::endl;
        delete gameMap;
        return 1;
    }

    std::cout << "Testing CommandProcessor:" << std::endl;
    testCommandProcessor(processor);

    delete processor;
    delete gameMap;
    return 0;
}
