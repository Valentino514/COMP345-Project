#include <iostream>
#include <algorithm>
#include "Player.h"
#include <fstream>
#include <utility>
#include <sstream>
#include <vector>
#include <limits>
#include <unordered_set>

using namespace std;


Player::Player(string* name, int* armyamount) {
    cout << "Generating player..." << *name << " with " << *armyamount << " armies\n";
    this->name = new string(*name);  
    this->armyamount = new int(*armyamount);  
    orders = new OrdersList();
    cards = new vector<Card*>;
    territories = new vector<Territory*>;
}
Player::Player(string& name) {
    cout << "Generating player... " << name << " with 0 armies (default)\n";
    this->name = new string(name);  
    this->armyamount = new int(0);  
    orders = new OrdersList();
    cards = new vector<Card*>;
    territories = new vector<Territory*>;
}

Player::Player(const Player& other) {
    name = new string(*other.name);
    armyamount = new int(*other.armyamount);  

    // Deep copy territories
    territories = new vector<Territory*>;
    for (Territory* t : *(other.territories)) {
        territories->push_back(new Territory(*t));
    }

    // Deep copy Card
    cards = new vector<Card*>;
    for (Card* c : *(other.cards)) {
        cards->push_back(new Card(*c));
    }

    
    orders = new OrdersList(*(other.orders));
}

void Player::setArmyAmount(int amount) { 
    if (armyamount) {
        *armyamount = amount;  // Update the existing armyamount
    } else {
        armyamount = new int(amount);  
    }
}

// Assignment operator 
Player& Player::operator=(const Player& other) {
    if (this == &other) return *this;  // Self-assignment check

    delete this->name;
    delete this->armyamount;
    for (Territory* t : *territories) {
        delete t;
    }
    territories->clear();
    for (Card* c : *cards) {
        delete c;
    }
    cards->clear();

    this->name = new string(*other.name);
    this->armyamount = new int(*other.armyamount);

    for (Territory* t : *(other.territories)) {
        territories->push_back(new Territory(*t));
    }

    for (Card* c : *(other.cards)) {
        cards->push_back(new Card(*c));
    }

    *orders = *(other.orders);

    return *this;
}


Player::~Player() {
   
    delete name;
    delete armyamount;


    // Delete territories only if they are owned by the Player class
    if (territories) {
        for (Territory* t : *territories) {
            delete t;
        }
        delete territories;
    }

    // Delete cards only if they are owned by the Player class
    if (cards) {
        for (Card* c : *cards) {
            delete c;
        }
        delete cards;
    }

    delete orders;
}

// Adds a player to the negotiated list if not already present
void Player::addNegotiatedPlayer(Player* player) {
    if (std::find(negotiatedPlayers.begin(), negotiatedPlayers.end(), player) == negotiatedPlayers.end()) {
        negotiatedPlayers.push_back(player);
    }
}

// Checks if this player has a truce with the specified player
bool Player::isNegotiatedWith(const Player* player) const {
    return std::find(negotiatedPlayers.begin(), negotiatedPlayers.end(), player) != negotiatedPlayers.end();
}

void Player::clearNegotiations() {
    negotiatedPlayers.clear();
}

// Return a List of Territories to Defend
std::vector<Territory*> Player::toDefend() const {

    std::vector<Territory*> defendList;
    for (Territory* territory : *territories) {
        defendList.push_back(territory);
    }
    return defendList;

}


// toAttack returns a list of adjacent territories not owned by the player
std::vector<Territory*> Player::toAttack() const {
    std::vector<Territory*> attackList;
    std::unordered_set<Territory*> addedTerritories;

    // Loop through each territory owned by the player to find adjacent ones not owned by the player
    for (Territory* territory : *territories) {
        const std::vector<Territory*>* adjacent = territory->getAdjacentTerritories();

        for (Territory* adj : *adjacent) {
            // Only add if the territory is not owned by the player and hasn't been added before
            if (adj->getLandOccupier() != this && addedTerritories.find(adj) == addedTerritories.end()) {
                attackList.push_back(adj);
                addedTerritories.insert(adj);  // Mark this territory as added
            }
        }
    }
    return attackList;
}


void Player::issueOrder(const std::vector<Player*>& playerList) {
    // Retrieve territories to defend and attack
     vector<Territory*> toDefend = this->toDefend();
    vector<Territory*> toAttack = this->toAttack();

    // Reinforcement phase
    cout<< "Player " << *name << " - Select territories to reinforce.\n" << endl;
    for (size_t i = 0; i < toDefend.size(); ++i) {
        cout << i + 1 << ". " << toDefend[i]->getName() << ": " << toDefend[i]->getArmyAmount() << " armies." << endl;
    }

    while (getArmyAmount() > 0) { // Dosent allow the player to go to the next step without finishing reinforcements
        string territoryName;
        int reinforcementAmount = 0;
        
        cout << "\nYou have " << getArmyAmount() << " armies remaining. Enter territory name for reinforcement: ";
        cin >> territoryName;
        std::cout <<"\n";

        if (territoryName == "exit" && getArmyAmount()==0) break;

        // Locate the territory to deploy reinforcements
        Territory* selectedTerritory = nullptr;
        for (Territory* t : toDefend) {
            if (t->getName() == territoryName) {
                selectedTerritory = t;
                break;
            }
        }

        if (!selectedTerritory) {
            cout << "Territory not found. Try again.\n" << endl;
            continue;
        }

        // Validates and adds Reinforcement amount for the territory chosen 
        cout << "Enter the number of armies to reinforce " << selectedTerritory->getName() << ": ";
        while (!(cin >> reinforcementAmount) || reinforcementAmount < 0 || reinforcementAmount > getArmyAmount()) {
            cout << "Invalid amount. Enter a number between 0 and " << getArmyAmount() << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Create a Deploy order and add to orders list
         Order* deployOrder = new Deploy(selectedTerritory, reinforcementAmount,this);
         orders->addOrder(deployOrder);  
        setArmyAmount(getArmyAmount() - reinforcementAmount);
        cout << "Deploy order issued to add " << reinforcementAmount << " armies to " << selectedTerritory->getName() << ".\n" << endl;
    }

    cout << "\n------------------------------------------------\n         Reinforcement phase completed\n------------------------------------------------\n\n" << endl;

  bool issuingAdvanceOrders = true;
while (issuingAdvanceOrders) {
    toDefend=this->toDefend();
    toAttack = this->toAttack();

    cout << "Territories available to move/attack from:" << endl;
    cout << "------------------------------------------------\n" << endl;
    for (size_t i = 0; i < toDefend.size(); ++i) {
        cout << i + 1 << ". " << toDefend[i]->getName() << ": " << toDefend[i]->getArmyAmount() << " armies available." << endl;
    }

    // Gather source territory
    Territory* source = nullptr;
    while (true) {
        string sourceName;
        cout << "\nEnter the name of the territory to move troops from: ";
        cin >> sourceName;

        //Validate maybe will comment out
        for (Territory* t : toDefend) {
            if (t->getName() == sourceName) {
                source = t;
                break;
            }
        }
        if (source) break; // Exit Loop if player owns territory
        cout << "Source territory not found. Try again." << endl;
    }

    
    cout << "\nChoose a destination territory:" << endl;
    cout << "-----------------------------------------------" << endl;

    // Display player's own territories (for defensive moves)
    cout << "Your territories (to reinforce):" << endl;
    for (Territory* t : toDefend) {
        cout << "- " << t->getName() << ": " << t->getArmyAmount() << " armies." << endl;
    }

    // Display enemy territories (for attack)
    cout << "\nEnemy territories (to attack):" << endl;
    for (Territory* t : toAttack) {
        for(Territory* s: *source->getAdjacentTerritories()){
            if (t->getName()==s->getName())
            {
               cout << "- " << t->getName() << ": " << t->getArmyAmount() << " armies." << endl;
            }
            
        }
    }

    // Get destination territory
    Territory* destination = nullptr;
    while (true) {
        string destinationName;
        cout << "\nEnter the name of the destination territory: ";
        cin >> destinationName;

        // Locate the destination in either toDefend or toAttack            (maybe comment out later )
        for (Territory* t : toDefend) {
            if (t->getName() == destinationName) {
                destination = t;
                break;
            }
        }
        if (!destination) { // If not found in toDefend, check in toAttack
            for (Territory* t : toAttack) {
                if (t->getName() == destinationName) {
                    destination = t;
                    break;
                }
            }
        }
        if (destination) break; // Exit loop if a valid destination is found
        cout << "Invalid destination. Try again." << endl;
    }

    // Choose army amount to move or attack
    int numArmies = 0;
    while (true) {
        cout << "Enter number of armies to move: ";
        if (cin >> numArmies && numArmies > 0) break;
        cout << "Invalid number. Enter a positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    
    Order* order = new Advance(this, source, destination, numArmies);  
    orders->addOrder(order); // Add the order to the player's order list

    cout << "Advance order issued to move " << numArmies << " armies from " << source->getName()
         << " to " << destination->getName() << "." << endl;

    // Ask if the player wants to issue another Advance order
     string decision;
    cout << "Would you like to issue another Advance order? (Y/N): ";
    cin >> decision;
    issuingAdvanceOrders = (decision == "Y" || decision == "y");
}

cout << "Orders Issuing phase for Advance orders completed.\n------------------------------------------------" << endl;


 while (!cards->empty()) {
        cout << *name << " - Cards in hand:" << endl;
        for (size_t i = 0; i < cards->size(); ++i) {
            cout << i + 1 << ". " << cards->at(i)->getCardTypeName() << endl;
        }
        cout << "Would you like to play a card? (Y/N): ";
        char playCardDecision;
        cin >> playCardDecision;
        if (playCardDecision != 'Y' && playCardDecision != 'y') {
            break;
        }

        int cardChoice = 0;
        cout << "Select a card to play (enter the number): ";
        while (!(cin >> cardChoice) || cardChoice < 1 || cardChoice > cards->size()) {
            cout << "Invalid choice. Please select a valid card number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
       Card* selectedCard = cards->at(cardChoice - 1);

        Order* cardOrder = nullptr;
        if (selectedCard->getCardTypeName() == "Bomb") {
            Territory* target = selectTargetFromAttackList();
            cardOrder = new Bomb(target,this); // Bomb order targets enemy territory

            cout << "Bomb order created targeting " << target->getName() << "." << endl;
        } else if (selectedCard->getCardTypeName() == "Airlift") {
            Territory* source = selectSourceTerritory();
            Territory* destination = selectDestinationTerritory();
            int armyAmount = selectArmyAmount(source);
            cardOrder = new Airlift(source, destination, armyAmount,this); // Airlift within own territories

            cout << "Airlift order created to move " << armyAmount << " armies from " << source->getName()
                 << " to " << destination->getName() << "." << endl;
        } else if (selectedCard->getCardTypeName() == "Blockade") {
            Territory* target = selectTargetFromDefendList();
            cardOrder = new Blockade(this, target); // Blockade on own territory

            cout << "Blockade order created on " << target->getName() << "." << endl;
        } else if (selectedCard->getCardTypeName() == "Diplomacy") {
            //Player* targetPlayer = selectPlayerToNegotiate(*playerList);
            Player* targetPlayer = selectPlayerToNegotiate(playerList);

            cardOrder = new Negociate(this, targetPlayer); // Negotiate with enemy player
            cout << "Negotiate order created with " << *(targetPlayer->getName()) << "." << endl;
         }// else if (selectedCard->getType() == "Reinforcement") {
        //     int reinforcementArmies = 5;
        //     setArmyAmount(getArmyAmount() + reinforcementArmies);
        //     cout << "Reinforcement card played. " << reinforcementArmies << " armies added to your reinforcement pool." << endl;
        //     removeCard(selectedCard);
        //     continue;
        // }

        if (cardOrder) {
            orders->addOrder(cardOrder);
        }
        removeCard(selectedCard);
    }

    cout << "Orders Issuing phase completed for player " << *name << ".\n------------------------------------------------" << endl;
}






//Overloaded stream insertion 
ostream& operator<<(ostream& os, const Player& player) {
    os << "Player with name " << *(player.name)
       << " has " << player.territories->size() << " territories, "
       << player.cards->size() << " cards, and "
       << *(player.armyamount) << " armies.";  
    return os;
}


void Player::addTerritory(Territory* territory) {
    territories->push_back(territory);  // Add territory to the player's list
}


void Player::printOwnedTerritories() const {
    std::cout << "Player " << *name << " owns the following territories:\n";
    for (auto t : *territories) {
        std::cout << t->getName() << std::endl;  
    }
}

void Player::addCard(Card* card) {
    cards->push_back(card);  // Adds a card to the player's hand
}


Territory* Player::selectTargetFromAttackList() {
    vector<Territory*> attackList = toAttack();
    cout << "Enemy territories to target:" << endl;
    for (size_t i = 0; i < attackList.size(); ++i) {
        cout << i + 1 << ". " << attackList[i]->getName() << " (Armies: " << attackList[i]->getArmyAmount() << ")" << endl;
    }
    int choice = 0;
    cout << "Select a territory to target (enter the number): ";
    while (!(cin >> choice) || choice < 1 || choice > attackList.size()) {
        cout << "Invalid choice. Please select a valid territory number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return attackList[choice - 1];
}

Territory* Player::selectTargetFromDefendList() {
    vector<Territory*> defendList = toDefend();
    cout << "Your territories to target:" << endl;
    for (size_t i = 0; i < defendList.size(); ++i) {
        cout << i + 1 << ". " << defendList[i]->getName() << " (Armies: " << defendList[i]->getArmyAmount() << ")" << endl;
    }
    int choice = 0;
    cout << "Select a territory to target (enter the number): ";
    while (!(cin >> choice) || choice < 1 || choice > defendList.size()) {
        cout << "Invalid choice. Please select a valid territory number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return defendList[choice - 1];
}

Player* Player::selectPlayerToNegotiate(const std::vector<Player*>& playerList) {
    std::cout << "Players to negotiate with:" << std::endl;
    int index = 1;
    for (Player* p : playerList) {
        if (p != this) { // Exclude self
            std::cout << index << ". " << *(p->getName()) << std::endl;
            index++;
        }
    }

    int choice = 0;
    std::cout << "Select a player to negotiate with (enter the number): ";
    while (!(std::cin >> choice) || choice < 1 || choice >= index) {
        std::cout << "Invalid choice. Please select a valid player number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return playerList[choice - 1];
}


Territory* Player::selectSourceTerritory() {
    vector<Territory*> defendList = toDefend();
    cout << "Your territories (to move armies from):" << endl;
    for (size_t i = 0; i < defendList.size(); ++i) {
        cout << i + 1 << ". " << defendList[i]->getName() << " (Armies: " << defendList[i]->getArmyAmount() << ")" << endl;
    }
    int choice = 0;
    cout << "Select a source territory (enter the number): ";
    while (!(cin >> choice) || choice < 1 || choice > defendList.size()) {
        cout << "Invalid choice. Please select a valid territory number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return defendList[choice - 1];
}

Territory* Player::selectDestinationTerritory() {
   
    vector<Territory*> defendList = toDefend();

    cout << "Your territories to move armies to:" << endl;
    for (size_t i = 0; i < defendList.size(); ++i) {
        cout << i + 1 << ". " << defendList[i]->getName() << " (Armies: " << defendList[i]->getArmyAmount() << ")" << endl;
    }

    int choice = 0;
    cout << "Select a destination territory (enter the number): ";
    while (!(cin >> choice) || choice < 1 || choice > defendList.size()) {
        cout << "Invalid choice. Please select a valid territory number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return defendList[choice - 1];
}

void Player::removeTerritory(Territory* territory) {
    auto it = find(territories->begin(), territories->end(), territory); // Find the territory in the list
    if (it != territories->end()) { // If found
        territories->erase(it); // Remove the territory
    }
}

int Player::selectArmyAmount(Territory* sourceTerritory) {
    int maxAmount = sourceTerritory->getArmyAmount();
    int amount = 0;
    cout << "Enter the number of armies to move (available: " << maxAmount << "): ";
    while (!(cin >> amount) || amount < 1 || amount > maxAmount) {
        cout << "Invalid amount. Enter a number between 1 and " << maxAmount << ": ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return amount;
}

void Player::removeCard(Card* card) {
    auto it = find(cards->begin(), cards->end(), card); // Find the card in the hand
    if (it != cards->end()) { // If found
        delete *it; // Delete the card to free memory
        cards->erase(it); // Remove the card from the hand
    }
}


//checks if player has a specific card
bool Player::hasCard(Card::CardType type) const {
    for (const auto& card : *cards) {
        if (card->getType() == type) {
            return true; // Found the card
        }
    }
    return false; // Card not found
}