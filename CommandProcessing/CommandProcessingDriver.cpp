#include "CommandProcessing.h"
#include <iostream>
#include <fstream>

void testCommandProcessor() {
    // Test 1: Console-based Command Processor
    std::cout << "Testing CommandProcessor with console input..." << std::endl;
    CommandProcessor cp;
    cp.processInput();  // Reads a command from the console and validates it
    std::cout << cp << std::endl;  // Outputs the command and effect to check if it was processed correctly

    // Test 2: File-based Command Processor Adapter
    std::cout << "\nTesting FileCommandProcessorAdapter with file input..." << std::endl;

    // Create a test command file
    std::ofstream testFile("test_commands.txt");
    if (!testFile) {
        std::cerr << "Error: Could not create test_commands.txt." << std::endl;
        return;
    }
    testFile << "loadmap\n";
    testFile << "validatemap\n";
    testFile << "addplayer\n";
    testFile << "gamestart\n";
    testFile << "replay\n";
    testFile.close();

    // Initialize FileCommandProcessorAdapter with the test file
    FileCommandProcessorAdapter fcp(nullptr, "test_commands.txt");

    // Verify file opened correctly within FileCommandProcessorAdapter
    if (!fcp.commandFile.is_open()) {
        std::cerr << "Error: Unable to open file test_commands.txt in FileCommandProcessorAdapter." << std::endl;
        return;
    }

    // Read and process each command from the file until EOF
    while (true) {
        std::string* result = fcp.readCommand();  // Read next command from the file
        std::cout<< *result<<endl;

        std::cout << fcp << std::endl;  // Outputs the command and effect to verify processing
    }
}

