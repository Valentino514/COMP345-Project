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


// Map Class Definitions

// Constructor
Map::Map() {
    Continents = new std::unordered_map<std::string, int>();
    Territories = new std::unordered_map<std::string, Territory*>();
    x = new bool(false);  // Initialize the bool pointer
}

// Destructor
Map::~Map() {
    for (auto& pair : *Territories) {
        delete pair.second; 
    }
    delete Territories; 
    delete Continents;
    delete x;
}

// Copy Constructor
Map::Map(const Map &other) {
    Continents = new std::unordered_map<std::string, int>(*other.Continents);
    Territories = new std::unordered_map<std::string, Territory*>();
    for (const auto& pair : *other.Territories) {
        (*Territories)[pair.first] = new Territory(*pair.second); // Deep copy of each territory
    }
     x = new bool(*other.x); 
}

// Assignment Operator
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


std::ostream& operator<<(std::ostream& os, const Map& map) {
    os << "Map with " << map.Continents->size() << " continents and " << map.Territories->size() << " territories.\n";
    return os;
}

// Getter for x
bool Map::getX() const {
    return *x;
}

// Setter for x
void Map::setX(bool value) {
    *x = value;
}


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

// Helper method: Check if the entire map is a connected graph
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



// Helper method: Check if each territory belongs to exactly one continent
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

// Getters and Setters for Territory Class

std::string Territory::getContinent() const {
    return *continent;
}

void Territory::setContinent(const std::string &cont) {
    *continent = cont;
}

Player* Territory::getLandOccupier() const {
    return territoryOwner;
}

int Territory::getArmyAmount() const {
    return *armyAmount;
}

void Territory::setArmyAmount(int amount) {
    *armyAmount = amount;
}

void Territory::setLandOccupier(Player* player) {
    territoryOwner = player;
}


const std::vector<Territory*>* Territory::getAdjacentTerritories() const {
    return AdjTerritories; // Return the pointer to the vector
}



void Territory::addAdjacentTerritory(Territory* adjacent) {
    AdjTerritories->push_back(adjacent); 
}

std::string Territory::getName() const {
    return *TerritoryName;
}

void Territory::setName(const std::string &name) {
    *TerritoryName = name;
}

// Default Constructor
Territory::Territory()
    : TerritoryName(new std::string("default territory")), 
      continent(new std::string("default continent")),
      AdjTerritories(new std::vector<Territory*>()),  // Allocating a new vector
      territoryOwner(nullptr), armyAmount(new int(0)) {}

// Parameterized Constructor
Territory::Territory(std::string* name, std::string* continent, std::vector<Territory*>* territories, Player* player, int* army)
    : TerritoryName(new std::string(*name)), 
      continent(new std::string(*continent)),
      AdjTerritories(new std::vector<Territory*>(*territories)),  
      territoryOwner(player ? new Player(*player) : nullptr), 
      armyAmount(new int(*army)) {}

// Destructor
Territory::~Territory() {

    
    delete TerritoryName;
    delete continent;
    delete AdjTerritories;
    delete armyAmount;
    territoryOwner = nullptr;
}

// Copy Constructor
Territory::Territory(const Territory& other) {
    TerritoryName = new std::string(*other.TerritoryName);
    continent = new std::string(*other.continent);
    AdjTerritories = new std::vector<Territory*>(*other.AdjTerritories); 
    armyAmount = new int(*other.armyAmount);
    territoryOwner = other.territoryOwner ? new Player(*other.territoryOwner) : nullptr;
}

// Assignment Operator
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
std::ostream& operator<<(std::ostream& os, const Territory& territory) {
    os << "Territory: " << *territory.TerritoryName << " in continent " << *territory.continent << "\n";
    return os;
}


// MapLoader Class Definitions
MapLoader::MapLoader() {}

// Destructor
MapLoader::~MapLoader() {}

// Helper Method to remove preceding spaces of loaded maps
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