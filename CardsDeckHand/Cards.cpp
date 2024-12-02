#include "Cards.h"
#include <cstdlib>  // For rand()
#include <ctime>    // For time()

/**
 * @class Card
 * @brief Represents a card in the game, each having a specific type and functionality.
 * 
 * The Card class encapsulates the concept of a card with types like Bomb, Reinforcement,
 * Blockade, Airlift, and Diplomacy. Cards can be played to perform specific actions.
 */


// Constructor
/**
 * @brief Constructs a Card object with a specified type.
 * 
 * @param t A pointer to the type of the card.
 */ 
Card::Card(CardType* t) : type(new CardType(*t)) {}

// Destructor
/**
 * @brief Destroys the Card object and releases its memory.
 */
Card::~Card() {
    delete type; 
}

// Generate a random card
/**
 * @brief Creates a Card object with a randomly selected type.
 * 
 * @return A new Card object with a random type.
 */
Card Card::getRandomCard() {
    CardType randomType = static_cast<CardType>(rand() % 5); // Generate random type
    return Card(new CardType(randomType));
}

// Play the card
/**
 * @brief Simulates playing the card, triggering its effect.
 */
void Card::play() {
    std::cout << "Playing card: " << getCardTypeName() << std::endl;
}

// Get the card type
/**
 * @brief Retrieves the type of the card.
 * 
 * @return The type of the card as a CardType enum value.
 */
Card::CardType Card::getType() const {
    return *type;
}

// Get the name of the card type
/**
 * @brief Converts the card's type to a human-readable string.
 * 
 * @return The name of the card type as a string.
 */
std::string Card::getCardTypeName() const {
    switch (*type) {
        case Bomb: return "Bomb";
        case Reinforcement: return "Reinforcement";
        case Blockade: return "Blockade";
        case Airlift: return "Airlift";
        case Diplomacy: return "Diplomacy";
        default: return "Unknown";
    }
}

/**
 * @class Deck
 * @brief Represents a collection of cards available for drawing during the game.
 * 
 * The Deck class manages a collection of cards, supports drawing cards at random,
 * and allows returning cards back into the deck.
 */

// Constructor
/**
 * @brief Constructs a Deck object and initializes it with cards of each type.
 */
Deck::Deck() : cards(new std::vector<Card*>()) {
    initializeDeck(); 
    srand(time(0));  
}

// Destructor
/**
 * @brief Destroys the Deck object and releases memory for all cards.
 */ 
Deck::~Deck() {
    for (Card* card : *cards) {
        delete card;  
    }
    delete cards;  
}

// Initialize the deck with cards
/**
 * @brief Populates the deck with a predefined number of cards for each type.
 */
void Deck::initializeDeck() {
    for (int i = 0; i < 5; ++i) {  // Add 5 cards of each type
        cards->push_back(new Card(new Card::CardType(Card::Bomb)));
        cards->push_back(new Card(new Card::CardType(Card::Blockade)));
        cards->push_back(new Card(new Card::CardType(Card::Airlift)));
        cards->push_back(new Card(new Card::CardType(Card::Diplomacy)));
    }
}

// Draw a card from the deck
/**
 * @brief Removes a random card from the deck and returns it.
 * 
 * @return A pointer to the drawn Card object, or nullptr if the deck is empty.
 */
Card* Deck::draw() {
    if (cards->empty()) {
        std::cout << "The deck is empty!" << std::endl;
        return nullptr;
    }
    int randomIndex = rand() % cards->size();  // Pick a random card
    Card* drawnCard = (*cards)[randomIndex];     // Get the card
    cards->erase(cards->begin() + randomIndex); // Remove the card from the deck
    return drawnCard;  // Return the drawn card
}

// Return a card to the deck
/**
 * @brief Adds a played card back into the deck.
 * 
 * @param card A pointer to the Card object to return to the deck.
 */
void Deck::returnCard(Card* card) {
    cards->push_back(card);  
}

/**
 * @class Hand
 * @brief Represents a player's collection of cards during the game.
 * 
 * The Hand class allows players to hold and manage cards, including playing them
 * and returning them to the deck after use.
 */


// Constructor
/**
 * @brief Constructs an empty Hand object.
 */
Hand::Hand() : handCards(new std::vector<Card*>()) {}

// Destructor
/**
 * @brief Destroys the Hand object and releases memory for all cards it contains.
 */
Hand::~Hand() {
    for (Card* card : *handCards) {
        delete card; 
    }
    delete handCards;  
}

// Add a card to the hand
/**
 * @brief Adds a card to the player's hand.
 * 
 * @param card A pointer to the Card object to be added.
 */
void Hand::addCard(Card* card) {
    handCards->push_back(card);
}

// Play all cards and return them to the deck
/**
 * @brief Plays all cards in the hand and returns them to the deck.
 * 
 * @param deck A pointer to the Deck object where cards are returned.
 */
void Hand::playAll(Deck* deck) {
    for (Card* card : *handCards) {  
        card->play();
        deck->returnCard(card);  
    }
    handCards->clear();  // Clear the hand after playing all cards
}




