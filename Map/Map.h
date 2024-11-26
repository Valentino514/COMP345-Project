#ifndef MAP_H_
#define MAP_H_


#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <fstream>
#include "../Player/Player.h"

// Forward declarations
class Territory;
class Continent;
class Player;



class Map {
    private:
    //Helper methods for validate()
    bool isMapConnected();                      //Check if map is a connected graph
    bool areContinentsConnected();              //Check if each continent is a connected subgraph
    bool eachTerritoryHasOneContinent();        //Check if each territory belongs to exactly one continent
    bool *x;
    
    

    public:
       /*  // Data members
        std::unordered_map<std::string, int> Continents; // continent name and value
        std::unordered_map<std::string, Territory*> Territories; // territory name and pointer to Territory object */

    std::unordered_map<std::string, int>* Continents; // continent name and value
    std::unordered_map<std::string, Territory*>* Territories; // territory name and pointer to Territory object



        // Constructor
        Map();

        // Destructor
        ~Map();

        // Copy Constructor
        Map(const Map& other);

        // Assignment Operator
        Map& operator=(const Map& other);

        // Stream Insertion
        friend std::ostream& operator<<(std::ostream& os, const Map& map);

        // Validation Method
        bool validate();

          // Getter for x
    bool getX() const;

    // Setter for x
    void setX(bool value);
    };
    

    class Territory {
private:

    std::string* TerritoryName;
    std::string* continent; 
    std::vector<Territory*>* AdjTerritories; 
    Player* territoryOwner; 
    int* armyAmount; 

public:
    // Default Constructor
    Territory();

    // Parameterized Constructor
    Territory(std::string* name, std::string* continent, std::vector<Territory*>* territories, Player* player, int* army);

    // Destructor
    ~Territory();

    // Copy Constructor
    Territory(const Territory& other);

    // Assignment Operator
    Territory& operator=(const Territory& other);

    // Getter and Setter methods
    std::string getName() const;
    void setName(const std::string& name);
    const std::vector<Territory*>* getAdjacentTerritories() const;
    void setAdjacentTerritories(const std::vector<Territory*>* territories);
    std::string getContinent() const;
    void setContinent(const std::string& cont);
    Player* getLandOccupier() const;
    void setLandOccupier(Player* player);
    int getArmyAmount() const;
    void setArmyAmount(int amount);

    // Method to add an adjacent territory
    void addAdjacentTerritory(Territory* adjacent);

    friend std::ostream& operator<<(std::ostream& os, const Territory& territory);

};

class Continent {
private:
    std::string* name;
    std::vector<Territory*>* territories;

public:
    // Constructor
    Continent(const std::string& name);

    // Destructor
    ~Continent();

    // Copy Constructor
    Continent(const Continent& other);

    // Assignment Operator
    Continent& operator=(const Continent& other);

    // Getter methods
    std::string getName() const;
    std::vector<Territory*>* getTerritories() const;

    // Add Method
    void addTerritory(Territory* territory);
};


class MapLoader {
public:
    // Constructor
    MapLoader();

    // Destructor
    ~MapLoader();

    // Map loading function
    Map* loadMap(const std::string& path);
};


#endif