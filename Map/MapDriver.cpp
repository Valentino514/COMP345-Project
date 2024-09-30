#include <iostream>
#include "Map.h"


void testLoadMaps();

int main() {
    testLoadMaps();
    return 0;
}

void testLoadMaps() {
    std::cout << "Testing map validation..." << std::endl;

    MapLoader loader;

    // Test map that fails the connected graph validation
    std::cout << "Testing invalid map (not connected):" << std::endl;
    Map* invalidMap1 = loader.loadMap("test_maps/invalid_map_not_connected.map");
    if (invalidMap1->validate()) {
        std::cout << "Invalid map (not connected) passed validation (ERROR)." << std::endl;
    } else {
        std::cout << "Invalid map (not connected) failed validation (CORRECT)." << std::endl;
    }
    delete invalidMap1;

    // Test map that fails the continent connected subgraph validation
    std::cout << "Testing invalid map (continent not connected):" << std::endl;
    Map* invalidMap2 = loader.loadMap("test_maps/invalid_continent_not_connected.map");
    if (invalidMap2->validate()) {
        std::cout << "Invalid map (continent not connected) passed validation (ERROR)." << std::endl;
    } else {
        std::cout << "Invalid map (continent not connected) failed validation (CORRECT)." << std::endl;
    }
    delete invalidMap2;

    // Test map that fails the territory belongs to one continent validation
    std::cout << "Testing invalid map (territory in multiple continents):" << std::endl;
    Map* invalidMap3 = loader.loadMap("test_maps/invalid_territory_multiple_continents.map");
    if (invalidMap3->validate()) {
        std::cout << "Invalid map (territory in multiple continents) passed validation (ERROR)." << std::endl;
    } else {
        std::cout << "Invalid map (territory in multiple continents) failed validation (CORRECT)." << std::endl;
    }
    delete invalidMap3;

    // Test valid map
    std::cout << "Testing valid map:" << std::endl;
    Map* validMap = loader.loadMap("test_maps/valid_map.map");
    if (validMap->validate()) {
        std::cout << "Valid map passed validation (CORRECT)." << std::endl;
    } else {
        std::cout << "Valid map failed validation (ERROR)." << std::endl;
    }
    delete validMap;

}
