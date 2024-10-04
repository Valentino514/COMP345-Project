#include <iostream>
#include "Map.h"

// Function declaration for testing map loading and validation
void testLoadMaps();

void testLoadMaps() {
   
    MapLoader loader;

    // Test 1: Map that fails the connected graph validation
    std::cout << "\n[TEST 1] Invalid map (not connected):" << std::endl;
    Map* invalidMap1 = loader.loadMap("Map/test_maps/invalid_map_not_connected.map");
    if (invalidMap1 != nullptr && invalidMap1->validate()) {
        std::cout << "Result: Invalid map (not connected) passed validation (ERROR)." << std::endl;
    } else {
        std::cout << "Result: Invalid map (not connected) failed validation (CORRECT)." << std::endl;
    }
    delete invalidMap1;  // Free allocated memory

    // Test 2: Map that fails the continent connected subgraph validation
    std::cout << "\n[TEST 2] Invalid map (continent not connected):" << std::endl;
    Map* invalidMap2 = loader.loadMap("Map/test_maps/invalid_continent_not_connected.map");
    if (invalidMap2 != nullptr && invalidMap2->validate()) {
        std::cout << "Result: Invalid map (continent not connected) passed validation (ERROR)." << std::endl;
    } else {
        std::cout << "Result: Invalid map (continent not connected) failed validation (CORRECT)." << std::endl;
    }
    delete invalidMap2;  // Free allocated memory

    // Test 3: Map that fails the territory belongs to multiple continents validation
    std::cout << "\n[TEST 3] Invalid map (territory in multiple continents):" << std::endl;
    Map* invalidMap3 = loader.loadMap("Map/test_maps/invalid_territory_multiple_continents.map");
    if (invalidMap3 != nullptr && invalidMap3->validate()) {
        std::cout << "Result: Invalid map (territory in multiple continents) passed validation (ERROR)." << std::endl;
    } else {
        std::cout << "Result: Invalid map (territory in multiple continents) failed validation (CORRECT)." << std::endl;
    }
    delete invalidMap3;  // Free allocated memory

    // Test 4: Valid map
    std::cout << "\n[TEST 4] Valid map:" << std::endl;
    Map* validMap = loader.loadMap("Map/test_maps/valid_map.map");
    if (validMap != nullptr && validMap->validate()) {
        std::cout << "Result: Valid map passed validation (CORRECT)." << std::endl;
    } else {
        std::cout << "Result: Valid map failed validation (ERROR)." << std::endl;
    }
    delete validMap;  // Free allocated memory

    std::cout << "\n===================================" << std::endl;
    std::cout << "   MAP VALIDATION TEST COMPLETED" << std::endl;
    std::cout << "=====================================" << std::endl;
}
