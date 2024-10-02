#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include <utility>
#include <regex> // Added regex for splitting
#include <unordered_set>
#include <stack>
#include "Map.h"
using namespace std;

// Temporary 
Player::Player(){}

// Map Class Definitions
Map::Map() {}

Map::~Map() {
    for (auto& pair : Territories) {
        delete pair.second;  // Free dynamically allocated territories
    }
}

Map::Map(const Map &other) {
    Continents = other.Continents;
    for (const auto& pair : other.Territories) {
        Territories[pair.first] = new Territory(*pair.second);
    }
}

Map& Map::operator=(const Map& other) {
    if (this == &other) return *this;
    Continents = other.Continents;
    for (auto& pair : Territories) {
        delete pair.second;
    }
    Territories.clear();
    for (const auto& pair : other.Territories) {
        Territories[pair.first] = new Territory(*pair.second);
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Map& map) {
    os << "Map with " << map.Continents.size() << " continents and " << map.Territories.size() << " territories.\n";
    return os;
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

//Helper method: Check if the entire map is a connected graph
bool Map::isMapConnected() {
    if (Territories.empty()) return false;  //map is invalid if there are no territories

    std::unordered_set<Territory*> visited; //to track visited territories
    std::stack<Territory*> stack;           //to perform DFS
    
    //Start DFS from the first territory
    auto start = Territories.begin()->second;
    stack.push(start);

    //Traverse the map by visiting each adjacent territoru
    while (!stack.empty()) {
        Territory* current = stack.top();
        stack.pop();

        if (visited.find(current) == visited.end()) {   //mark territory as visited
            visited.insert(current);

            // Add all adjacent territories to the stack
            for (Territory* adjacent : current->getAdjacentTerritories()) {
                if (visited.find(adjacent) == visited.end()) {
                    stack.push(adjacent);
                }
            }
        }
    }

    //Check if all territories were visited
    return visited.size() == Territories.size();
}

//Helper method: Check if all continents are connected subgraphs
bool Map::areContinentsConnected() {
    //For each continent, perform a connected check
    for (const auto& pair : Continents) {
        std::string continentName = pair.first;
        std::unordered_set<Territory*> continentTerritories;

        //Collect all territories in this continent
        for (const auto& territoryPair : Territories) {
            if (territoryPair.second->getContinent() == continentName) {
                continentTerritories.insert(territoryPair.second);
            }
        }

        //If there are less than 2 territories, it is trivially connected
        if (continentTerritories.size() <= 1) continue;

        std::unordered_set<Territory*> visited;     //track visited territories in given continent
        std::stack<Territory*> stack;               //stack for DFS
        stack.push(*continentTerritories.begin());  //starts DFS from any territory in the continent

        //Perform DFS within the continent
        while (!stack.empty()) {
            Territory* current = stack.top();
            stack.pop();

            if (visited.find(current) == visited.end()) {   //mark territory as visited
                visited.insert(current);

                //Add all adjacent territories in the same continent to the stack
                for (Territory* adjacent : current->getAdjacentTerritories()) {
                    if (continentTerritories.find(adjacent) != continentTerritories.end() && visited.find(adjacent) == visited.end()) {
                        stack.push(adjacent);
                    }
                }
            }
        }

        //Check if all territories in this continent were visited
        if (visited.size() != continentTerritories.size()) {
            return false;
        }
    }

    return true;
}

//Helper method: Check if each territory belongs to exactly one continent
bool Map::eachTerritoryHasOneContinent() {
    //Iterate over each territory in the map
    for (const auto& pair : Territories) {
        Territory* territory = pair.second;
        std::string continent = territory->getContinent();

        //Territory is invalid if it has no assigned continent
        if (continent.empty() ) {
            return false;
        }
    }
    return true;
}

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
    return *(armyAmount);
}

void Territory::setArmyAmount(int amount) {
    *armyAmount = amount;
}

const std::vector<Territory*>& Territory::getAdjacentTerritories() const {
    return AdjTerritories;
}

void Territory::addAdjacentTerritory(Territory* adjacent) {
    AdjTerritories.push_back(adjacent);
}

std::string Territory::getName() const {
    return std::string();
}

void Territory::setName(const std::string &name) {
    *TerritoryName = name;
}

Territory::Territory()
    : Territory(new std::string("default territory"), new std::string("default continent"), {}, nullptr, new int(0)) {}

Territory::Territory(std::string* name, std::string* continent, std::vector<Territory*> territories, Player* player, int* army)
    : TerritoryName(new std::string(*name)), continent(new std::string(*continent)),
      AdjTerritories(territories), territoryOwner(player ? new Player(*player) : nullptr), armyAmount(new int(*army)) {}

Territory::~Territory() {
    delete TerritoryName;
    delete continent;
    delete armyAmount;
}

Territory::Territory(const Territory& other) {
    TerritoryName = new std::string(*other.TerritoryName);
    continent = new std::string(*other.continent);
    territoryOwner = new Player(*other.territoryOwner);
    armyAmount = new int(*other.armyAmount);
    AdjTerritories = other.AdjTerritories;
}

Territory& Territory::operator=(const Territory& other) {
    if (this == &other) return *this;
    delete TerritoryName;
    delete continent;
    delete territoryOwner;
    delete armyAmount;
    TerritoryName = new std::string(*other.TerritoryName);
    continent = new std::string(*other.continent);
    territoryOwner = other.territoryOwner ? new Player(*other.territoryOwner) : nullptr;
    armyAmount = new int(*other.armyAmount);
    AdjTerritories = other.AdjTerritories;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Territory& territory) {
    os << "Territory: " << *territory.TerritoryName << " in continent " << *territory.continent << "\n";
    return os;
}

// Continent Class Definitions
Continent::Continent(const std::string& name) : name(new std::string(name)) {}

Continent::~Continent() {
    delete name;
}

Continent::Continent(const Continent& other) {
    name = new std::string(*other.name);
    territories = other.territories;
}

Continent& Continent::operator=(const Continent& other) {
    if (this == &other) return *this;
    delete name;
    name = new std::string(*other.name);
    territories = other.territories;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Continent& continent) {
    os << "Continent: " << *continent.name << "\n";
    return os;
}

// MapLoader Class Definitions
MapLoader::MapLoader() {}

MapLoader::~MapLoader() {}

Map* MapLoader::loadMap(const std::string& path) {
    std::unordered_map<std::string, Territory*> TrackT;
    std::unordered_map<std::string, int> conts;
    std::string input;
    std::ifstream file(path);

    if (!file) {
        std::cout << "Error: invalid file" << std::endl;
        return nullptr;
    }

    while (getline(file, input)) {
        if (input.find("[Continents]") != std::string::npos) {
            while (getline(file, input)) {
                if (input.length() == 0) {
                    continue;
                }
                if (input.find("[Territories]") != std::string::npos) {
                    break;
                }
                string Cdelim = "=";
                string Cname = input.substr(0, input.find(Cdelim));
                int value = stoi(input.substr(input.find(Cdelim) + 1, input.length()));
                conts.insert({Cname, value});
            }
        }
        if (input.find("[Territories]") != std::string::npos) {
            while (getline(file, input)) {
                std::vector<std::string> FullLine; // to save the line without preceding spaces
                std::regex re(",\\s*"); // \\s*: This matches zero or more whitespace characters following the comma.
                std::sregex_token_iterator it(input.begin(), input.end(), re, -1); // -1 returns all the substrings of the input that are separated by the regex matches (not equal to regex)
                std::sregex_token_iterator end; // creates an iterator that represents the end state

                while (it != end) { // *it dereferences the iterator it to access the current token + a post increment
                    FullLine.push_back(*it++); // adds all the substrings produced by the regex match into the FullLine vector.
                }

                if (FullLine.size() > 1) {
                    Territory* newTerritory;
                    auto searchmain = TrackT.find(FullLine[0]);

                    if (searchmain == TrackT.end()) {
                        newTerritory = new Territory();
                        newTerritory->setName(FullLine[0]);
                        newTerritory->setContinent(FullLine[3]);
                    } else {
                        newTerritory = searchmain->second;
                        if (newTerritory->getContinent() == "default continent") {
                            newTerritory->setContinent(FullLine[3]);
                        }
                    }

                    for (size_t i = 4; i < FullLine.size(); i++) {
                        auto adjsearch = TrackT.find(FullLine[i]);
                        if (adjsearch == TrackT.end()) {
                            Territory* AdjT = new Territory();
                            AdjT->setName(FullLine[i]);
                            TrackT.insert({FullLine[i], AdjT});
                            newTerritory->addAdjacentTerritory(AdjT);
                        } else {
                            newTerritory->addAdjacentTerritory(adjsearch->second);
                        }
                    }
                    TrackT.insert({FullLine[0], newTerritory});
                }
            }
        }
    }

    Map* WarzoneMap = new Map();
    WarzoneMap->Territories = TrackT;
    WarzoneMap->Continents = conts;
    return WarzoneMap;
}