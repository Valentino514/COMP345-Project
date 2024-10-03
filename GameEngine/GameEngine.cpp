#include <iostream>
#include <array>
#include "GameEngine.h"

using namespace std;

GameEngine::GameEngine(){}

void GameEngine::navigate() {
    string command;

    while (true) {
        if (currentIndex==7)
        {
            
        }
        
        cout << "current state: " << map[currentIndex] << endl;

        // Check if currentIndex is at the end of the map
        if (currentIndex + 1 >= sizeof(map) / sizeof(map[0])) {
            cout << "No more states to navigate to. Type 'exit' to end." << endl;
            break;
        }

        // Prompt user for the next move
        cout << "Type '" << map[currentIndex + 1] << "' to move to the next state, or type 'exit' to end: ";
        cin >> ws;  // Clear leading whitespaces
        getline(cin, command);  // Get full command including spaces

        // If the user types 'exit'
        if (command == "exit") {
            cout << "Exiting..." << endl;
            return;
        }

        // Move to the next state
        if (command == map[currentIndex + 1]) {
            currentIndex++;
            // Handle looping states: 1, 3, 5, 6 (execute orders)
            if (currentIndex == 1 || currentIndex == 3 || currentIndex == 5 || currentIndex == 6) {
                while (true) {
                    cout << "current state: " << map[currentIndex] << endl;

                    // If in 'execute orders' (index 6), allow looping indefinitely
                    if (currentIndex == 6) {
                        cout << "You are in the 'execute orders' phase. Type '" << map[currentIndex]
                             << "' to loop, or type '" << map[currentIndex + 1]
                             << "' to move to the next state: ";
                    } else {
                        cout << "Type the state again to loop, or type '"
                             << map[currentIndex + 1] << "' to move to the next state: ";
                    }
                    
                    cin >> ws;  // Clear leading whitespaces
                    getline(cin, command);  // Get full command including spaces

                    if (command == map[currentIndex]) {
                        // User chooses to loop, so continue the loop
                        continue;
                    } else if (command == map[currentIndex + 1]) {
                        // Move to the next state
                        currentIndex++;
                        if(currentIndex ==6){
                            continue;
                        }
                        else{
                        break;  // Break the inner loop and continue navigating
                        }
                    } else if (command == "exit") {
                        cout << "Exiting..." << endl;
                        return;
                    } else {
                        // Invalid input within loop states
                        cout << "Invalid input, try again." << endl;
                    }
                }
            }
        } else {
            // Invalid input in non-loop states
            cout << "Invalid input, try again." << endl;
        }
    }
}
