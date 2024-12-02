#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include <utility>
#include <regex> 
#include <unordered_set>
#include <stack>
#include "Map.h"
using namespace std;


/**
 * @class Map
 * @brief Represents the game map with continents, territories, and validation logic.
 * 
 * The Map class manages the relationships between continents and territories, ensuring
 * connectivity and proper structure. It provides validation methods to check the map's integrity.
 */


// Constructor
/**
 * @brief Constructs an empty Map object, initializing its member variables.
 */
Map::Map() {
    Continents = new std::unordered_map<std::string, int>();
    Territories = new std::unordered_map<std::string, Territory*>();
    x = new bool(false);  // Initialize the bool pointer
}

// Destructor
/**
 * @brief Cleans up dynamically allocated memory for continents, territories, and flags.
 */
Map::~Map() {
    for (auto& pair : *Territories) {
        delete pair.second; 
    }
    delete Territories; 
    delete Continents;
    delete x;
}

// Copy Constructor
/**
 * @brief Constructs a deep copy of another Map object.
 * 
 * @param other The Map object to copy.
 */
Map::Map(const Map &other) {
    Continents = new std::unordered_map<std::string, int>(*other.Continents);
    Territories = new std::unordered_map<std::string, Territory*>();
    for (const auto& pair : *other.Territories) {
        (*Territories)[pair.first] = new Territory(*pair.second); // Deep copy of each territory
    }
     x = new bool(*other.x); 
}

// Assignment Operator
/**
 * @brief Assigns the state of another Map object to this one.
 * 
 * @param other The Map object to copy.
 * @return A reference to the updated Map object.
 */
Map& Map::operator=(const Map& other) {
    if (this == &other) return *this;
    
    // Free existing resources
    for (auto& pair : *Territories) {
        delete pair.second;
    }
    delete Territories;
    delete Continents;
    delete x;

    // Deep copy from the other map
    Continents = new std::unordered_map<std::string, int>(*other.Continents);
    Territories = new std::unordered_map<std::string, Territory*>();
    for (const auto& pair : *other.Territories) {
        (*Territories)[pair.first] = new Territory(*pair.second);
    }
     x = new bool(*other.x); 
    return *this;
}

// Stream Insertion Operator
/**
 * @brief Outputs a summary of the Map's structure to the console.
 * 
 * @param os The output stream.
 * @param map The Map object to summarize.
 * @return The modified output stream.
 */
std::ostream& operator<<(std::ostream& os, const Map& map) {
    os << "Map with " << map.Continents->size() << " continents and " << map.Territories->size() << " territories.\n";
    return os;
}

// Getter for x
/**
 * @brief Retrieves the value of the `x` flag.
 * 
 * @return The boolean value of `x`.
 */
bool Map::getX() const {
    return *x;
}

// Setter for x
/**
 * @brief Sets the value of the `x` flag.
 * 
 * @param value The new boolean value for `x`.
 */
void Map::setX(bool value) {
    *x = value;
}

// Validate the map structure
/**
 * @brief Validates the map's connectivity and structure.
 * 
 * @details Ensures the map is a connected graph, each continent is a connected subgraph,
 * and each territory belongs to exactly one continent.
 * 
 * @return true if the map is valid; false otherwise.
 */
bool Map::validate() {
    //Check if map is a connected graph
    if (!isMapConnected()) {
        std::cout << "Error: The map is not a connected graph." << std::endl;
        return false;
    }

    //Check each continent is a connected subgraph
    if (!areContinentsConnected()) {
        std::cout << "Error: Some continents are not connected subgraphs." << std::endl;
        return false;
    }

    //Check each territory belongs to exactly one continent
    if (!eachTerritoryHasOneContinent()) {
        std::cout << "Error: Some territories belong to multiple or no continents." << std::endl;
        return false;
    }

    std::cout << "Map is valid." << std::endl;
    return true;
}

// Check if the entire map is connected
/**
 * @brief Verifies that all territories in the map are part of a single connected graph.
 * 
 * @return true if the map is connected; false otherwise.
 */
bool Map::isMapConnected() {
    if (Territories->empty()) return false;  // Map is invalid if there are no territories

    std::unordered_set<Territory*> visited; // To track visited territories
    std::stack<Territory*> stack;           // To perform DFS
    
    // Start DFS from the first territory
    auto start = Territories->begin()->second;
    stack.push(start);

    // Traverse the map by visiting each adjacent territory
    while (!stack.empty()) {
        Territory* current = stack.top();
        stack.pop();

        if (visited.find(current) == visited.end()) {   // Mark territory as visited
            visited.insert(current);

            // Add all adjacent territories to the stack
            for (Territory* adjacent : *(current->getAdjacentTerritories())) {  
                if (visited.find(adjacent) == visited.end()) {
                    stack.push(adjacent);
                }
            }
        }
    }

    // Check if all territories were visited
    return visited.size() == Territories->size();
}

// Check if all continents are connected subgraphs
/**
 * @brief Verifies that each continent forms a connected subgraph.
 * 
 * @return true if all continents are connected; false otherwise.
 */
bool Map::areContinentsConnected() {
    // For each continent, perform a connected check
    for (const auto& pair : *Continents) {  
        std::string continentName = pair.first;
        std::unordered_set<Territory*> continentTerritories;

        // Collect all territories in this continent
        for (const auto& territoryPair : *Territories) {  
            if (territoryPair.second->getContinent() == continentName) {
                continentTerritories.insert(territoryPair.second);
            }
        }

        // If there are less than 2 territories, it is trivially connected
        if (continentTerritories.size() <= 1) continue;

        std::unordered_set<Territory*> visited;     // Track visited territories in the continent
        std::stack<Territory*> stack;               // Stack for DFS
        Territory* startTerritory = *continentTerritories.begin();  // Start DFS from any territory in the continent
        stack.push(startTerritory);


        // Perform DFS within the continent
        while (!stack.empty()) {
            Territory* current = stack.top();
            stack.pop();

            if (visited.find(current) == visited.end()) {   // Mark territory as visited
                visited.insert(current);

                // Check adjacent territories
                for (Territory* adjacent : *(current->getAdjacentTerritories())) {

                    // New Condition: Check bidirectional adjacency
                    const std::vector<Territory*>* adjToCurrent = adjacent->getAdjacentTerritories();
                    if (std::find(adjToCurrent->begin(), adjToCurrent->end(), current) == adjToCurrent->end()) {
                        return false;  // Return false if bidirectional adjacency fails
                    }

                    // Add adjacent territories in the same continent to the stack
                    if (continentTerritories.find(adjacent) != continentTerritories.end() && visited.find(adjacent) == visited.end()) {
                        stack.push(adjacent);
                    }
                }
            }
        }

        // Check if all territories in this continent were visited
        if (visited.size() != continentTerritories.size()) {
            std::cout << "Error: Continent " << continentName << " is not fully connected." << std::endl;
            return false;
        }
    }

    std::cout << "All continents are fully connected!" << std::endl;
    return true;
}


// Check if each territory belongs to exactly one continent
/**
 * @brief Ensures each territory is assigned to exactly one continent.
 * 
 * @return true if all territories belong to one continent; false otherwise.
 */
bool Map::eachTerritoryHasOneContinent() {
    // Iterate over each territory in the map
    for (const auto& pair : *Territories) {  
        Territory* territory = pair.second;
        std::string continent = territory->getContinent();

        // Territory is invalid if it has no assigned continent
        if (continent.empty()) {
            return false;
        }
        if (*x)
        {
            return false;
        }
        

    }
    return true;
}

/**
 * @class Territory
 * @brief Represents a territory in the game map with attributes such as name, continent, adjacent territories, and ownership.
 * 
 * The Territory class manages the core properties and relationships of a territory. 
 * It includes details about its owning player, army count, and adjacency to other territories.
 */


// Getter for continent
/**
 * @brief Retrieves the continent associated with the territory.
 * 
 * @return A string representing the continent name.
 */
std::string Territory::getContinent() const {
    return *continent;
}

// Setter for continent
/**
 * @brief Sets the continent name for the territory.
 * 
 * @param cont The name of the continent.
 */
void Territory::setContinent(const std::string &cont) {
    *continent = cont;
}

// Getter for the occupying player
/**
 * @brief Retrieves the player currently occupying the territory.
 * 
 * @return A pointer to the Player object owning the territory.
 */
Player* Territory::getLandOccupier() const {
    return territoryOwner;
}

// Getter for the army count
/**
 * @brief Retrieves the number of armies stationed in the territory.
 * 
 * @return The number of armies as an integer.
 */
int Territory::getArmyAmount() const {
    return *armyAmount;
}

// Setter for the army count
/**
 * @brief Sets the number of armies stationed in the territory.
 * 
 * @param amount The new army count. Values less than or equal to 0 are set to 0.
 */
void Territory::setArmyAmount(int amount) {
    if(amount <=0){
        *armyAmount = 0;
    }
    else{
        *armyAmount = amount;
    }
}

// Setter for the occupying player
/**
 * @brief Assigns a player as the owner of the territory.
 * 
 * @param player A pointer to the Player object.
 */
void Territory::setLandOccupier(Player* player) {
    territoryOwner = player;
}

// Getter for adjacent territories
/**
 * @brief Retrieves the list of adjacent territories.
 * 
 * @return A pointer to the vector containing pointers to adjacent Territory objects.
 */
const std::vector<Territory*>* Territory::getAdjacentTerritories() const {
    return AdjTerritories; // Return the pointer to the vector
}

// Add an adjacent territory
/**
 * @brief Adds a territory to the list of adjacent territories.
 * 
 * @param adjacent A pointer to the Territory object to add.
 */
void Territory::addAdjacentTerritory(Territory* adjacent) {
    AdjTerritories->push_back(adjacent); 
}

// Getter for the territory name
/**
 * @brief Retrieves the name of the territory.
 * 
 * @return A string representing the territory name.
 */
std::string Territory::getName() const {
    return *TerritoryName;
}

// Setter for the territory name
/**
 * @brief Sets the name of the territory.
 * 
 * @param name The new name of the territory.
 */
void Territory::setName(const std::string &name) {
    *TerritoryName = name;
}

// Static member for default territory count
int* Territory::defaultTerritoryCount = new int(0);

// Default Constructor
/**
 * @brief Constructs a Territory object with default values.
 * 
 * @details Automatically generates a unique default name and assigns a default continent name.
 */
Territory::Territory()
    : TerritoryName(new std::string("default territory " + to_string(++(*defaultTerritoryCount)))), 
      continent(new std::string("default continent")),
      AdjTerritories(new std::vector<Territory*>()),  // Allocating a new vector
      territoryOwner(nullptr), armyAmount(new int(0)) {
      }

// Parameterized Constructor
/**
 * @brief Constructs a Territory object with specified attributes.
 * 
 * @param name A pointer to the territory's name.
 * @param continent A pointer to the name of the continent the territory belongs to.
 * @param territories A pointer to a vector of adjacent territories.
 * @param player A pointer to the owning Player object (optional).
 * @param army A pointer to the initial army count.
 */
Territory::Territory(std::string* name, std::string* continent, std::vector<Territory*>* territories, Player* player, int* army)
    : TerritoryName(new std::string(*name)), 
      continent(new std::string(*continent)),
      AdjTerritories(new std::vector<Territory*>(*territories)),  
      territoryOwner(player ? new Player(*player) : nullptr), 
      armyAmount(new int(*army)) {}

// Destructor
/**
 * @brief Cleans up dynamically allocated memory for the Territory object.
 */
Territory::~Territory() {

    
    delete TerritoryName;
    delete continent;
    delete AdjTerritories;
    delete armyAmount;
    territoryOwner = nullptr;
    delete defaultTerritoryCount;
    defaultTerritoryCount = nullptr;
}

// Copy Constructor
/**
 * @brief Constructs a deep copy of another Territory object.
 * 
 * @param other The Territory object to copy.
 */
Territory::Territory(const Territory& other) {
    TerritoryName = new std::string(*other.TerritoryName);
    continent = new std::string(*other.continent);
    AdjTerritories = new std::vector<Territory*>(*other.AdjTerritories); 
    armyAmount = new int(*other.armyAmount);
    territoryOwner = other.territoryOwner ? new Player(*other.territoryOwner) : nullptr;
}

// Assignment Operator
/**
 * @brief Assigns the state of another Territory object to this one.
 * 
 * @param other The Territory object to copy.
 * @return A reference to the updated Territory object.
 */
Territory& Territory::operator=(const Territory& other) {
    if (this == &other) return *this;
    delete TerritoryName;
    delete continent;
    delete AdjTerritories;
    delete armyAmount;
    if (territoryOwner) delete territoryOwner;

    TerritoryName = new std::string(*other.TerritoryName);
    continent = new std::string(*other.continent);
    AdjTerritories = new std::vector<Territory*>(*other.AdjTerritories); 
    armyAmount = new int(*other.armyAmount);
    territoryOwner = other.territoryOwner ? new Player(*other.territoryOwner) : nullptr;
    return *this;
}

// Stream Operator
/**
 * @brief Outputs the details of a Territory object to an output stream.
 * 
 * @param os The output stream.
 * @param territory The Territory object to output.
 * @return The modified output stream.
 */
std::ostream& operator<<(std::ostream& os, const Territory& territory) {
    os << "Territory: " << *territory.TerritoryName << " in continent " << *territory.continent << "\n";
    return os;
}

/**
 * @class MapLoader
 * @brief Handles the loading and parsing of map files to construct Map objects.
 * 
 * The MapLoader class reads a file containing information about continents and territories,
 * processes the data, and creates a corresponding Map object. It ensures proper formatting
 * and structure during the loading process.
 */


// Default Constructor
/**
 * @brief Constructs a MapLoader object.
 * 
 * @details Initializes the MapLoader with no specific state. It serves as a utility class
 * for loading maps from file paths.
 */
MapLoader::MapLoader() {}

// Destructor
/**
 * @brief Cleans up resources allocated by the MapLoader.
 * 
 * @details Since the MapLoader does not allocate persistent memory, the destructor is
 * trivial. It ensures any derived or future extensions clean up properly.
 */
MapLoader::~MapLoader() {}

// Helper Function: Split a line into parts
/**
 * @brief Splits a line into components based on a delimiter pattern.
 * 
 * @param line The input string to split.
 * @param re A regex pattern used for splitting the string.
 * @return A vector of substrings resulting from the split operation.
 * 
 * @details This function is used to process lines in the map file, separating
 * values such as territory names, continents, and adjacency lists.
 */
std::vector<std::string> splitLine(const std::string& line, const std::regex& re) {
    std::vector<std::string> result;
    std::sregex_token_iterator it(line.begin(), line.end(), re, -1);
    std::sregex_token_iterator end;

    result.reserve(std::distance(it, end));  
    while (it != end) {
        result.push_back(*it++);
    }
    return result;
}

// Load a Map from a file
/**
 * @brief Loads a map from the specified file path and constructs a Map object.
 * 
 * @param path The file path to the map file.
 * @return A pointer to the constructed Map object, or nullptr if the file is invalid.
 * 
 * @details This method processes the file line-by-line, extracting data about continents
 * and territories. It constructs `Territory` objects, assigns them to continents, and
 * links adjacent territories to create a complete map structure.
 */
Map* MapLoader::loadMap(const std::string& path) {

    // Temporary containers to hold territories and continents

    // store the territory name and its object, and used to track the territories created 
    std::unordered_map<std::string, Territory*> TrackT; 

    // Continent map to store the continent name and its value
    std::unordered_map<std::string, int> conts;
    std::string input;
    std::ifstream file(path);
    bool y=false;

    // Check if the file was opened successfully
    if (!file) {
        std::cout << "Error: invalid file" << std::endl;
        return nullptr;
    }

    // Constructing the regex 
    std::regex re(",\\s*");  

    // Reading the file line by line
    while (getline(file, input)) {
        // Process the continents section
        if (input.find("[Continents]") != std::string::npos) {
            while (getline(file, input)) {
                if (input.empty()) continue;

                //breaks out the loop when we finish reading continents 
                if (input.find("[Territories]") != std::string::npos) break;

                std::string Cdelim = "=";
                std::string Cname = input.substr(0, input.find(Cdelim));
                int value = stoi(input.substr(input.find(Cdelim) + 1));

                // Insert continent name and its value into the map
                conts.insert({Cname, value});
            }
        }

        // Process the territories section
        if (input.find("[Territories]") != std::string::npos) {
            while (getline(file, input)) {
                // Use the splitLine helper function to split the input line
                std::vector<std::string> FullLine = splitLine(input, re);

                // If the line has more than one element (valid territory entry)
                if (FullLine.size() > 1) {
                    Territory* newTerritory;

                    // If its not in tracker it will be equal to TrackT.end
                    auto searchmain = TrackT.find(FullLine[0]);

                    // If the territory does not already exist, create it
                    if (searchmain == TrackT.end()) {
                        newTerritory = new Territory();
                        newTerritory->setName(FullLine[0]);
                        newTerritory->setContinent(FullLine[3]);

                    // if the territory was adjacent to a territory found before (object was already created)
                    } else {
                        newTerritory = searchmain->second; // accessing the obj
                        if (newTerritory->getContinent() == "default continent") {
                            newTerritory->setContinent(FullLine[3]);
                        }
                        else {
                            y=true;
                        }
                    }

                    // Process the adjacent territories
                    for (size_t i = 4; i < FullLine.size(); i++) {
                        auto adjsearch = TrackT.find(FullLine[i]);
                        if (adjsearch == TrackT.end()) {
                            // Create new adjacent territory if it doesn't exist
                            Territory* AdjT = new Territory();
                            AdjT->setName(FullLine[i]);
                            TrackT.insert({FullLine[i], AdjT});
                            newTerritory->addAdjacentTerritory(AdjT);
                        } else {
                            // Add existing territory as adjacent
                            newTerritory->addAdjacentTerritory(adjsearch->second);
                        }
                    }
                    TrackT.insert({FullLine[0], newTerritory});
                }
            }
        }
    }

    // Create a new Map object and assign territories and continents
    Map* WarzoneMap = new Map();
    WarzoneMap->Territories = new std::unordered_map<std::string, Territory*>(TrackT);
    WarzoneMap->Continents = new std::unordered_map<std::string, int>(conts);
    WarzoneMap->setX(y);

    return WarzoneMap;
}