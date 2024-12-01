#include "../GameEngine/GameEngine.h"
#include "../CommandProcessing/CommandProcessing.h"
#include <iostream>
#include <vector>
#include <string>

void testTournament() {
    // Initialize the game engine and command processor
    GameEngine gameEngine;
    CommandProcessor commandProcessor;

    // Prompt the user for the tournament command
    std::cout << "Enter the tournament command in the following format:\ntournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns> ";

    // Read the user's input
    std::string tournamentCommand;
    std::cout << "Enter command: ";
    std::getline(std::cin, tournamentCommand);

    // Validate the command and parse parameters
    TournamentParams params = commandProcessor.parseTournamentCommand(tournamentCommand);
    if (params.maps.empty() || params.strategies.empty() || params.games <= 0 || params.maxTurns <= 0) {
        std::cout << "Invalid tournament parameters. Please check the command syntax.\n";
        return;
    }

    // Display the parsed parameters for confirmation
    std::cout << "\nTournament Parameters Parsed Successfully:\n";
    std::cout << "Maps: ";
    for (const std::string& map : params.maps) {
        std::cout << map << " ";
    }
    std::cout << "\nStrategies: ";
    for (const std::string& strategy : params.strategies) {
        std::cout << strategy << " ";
    }
    std::cout << "\nNumber of Games per Map: " << params.games << std::endl;
    std::cout << "Maximum Number of Turns: " << params.maxTurns << std::endl;

    // Add players to the game engine's player list based on strategies
    gameEngine.addPlayersToGameEngine(params.strategies);

    // Execute the tournament
    std::cout << "\nStarting the tournament...\n" << std::endl;
    gameEngine.executeTournament(params);

    // Indicate the end of the tournament
    std::cout << "\nTournament completed. Results displayed above.\n" << std::endl;
}
