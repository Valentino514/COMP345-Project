#include "Cards.h"
#include <cstdlib>  // For rand()
#include <ctime>    // For time()

// Card class implementation

// Constructor to initialize a card with a specific type
Card::Card(CardType* t) : type(t) {}

// Destructor to free the allocated memory
Card::~Card() {
    delete type;  // Delete the dynamically allocated card type
}

// Play method: Outputs the card type when the card is played
void Card::play() {
    std::cout << "Playing card: " << getCardTypeName() << std::endl;
}

// Get the type of the card
Card::CardType* Card::getType() const {
    return type;
}

// Return the name of the card type as a string
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

// Deck class implementation

// Constructor: Initialize the deck with cards
Deck::Deck() : cards(new std::vector<Card*>()) {
    initializeDeck();  // Fill the deck with cards
    srand(time(0));  // Seed for random card drawing
}

// Destructor to clean up dynamically allocated memory
Deck::~Deck() {
    for (Card* card : *cards) {
        delete card;  // Free memory for each card
    }
    delete cards;  // Free the vector memory
}

// Initialize the deck with several cards of each type
void Deck::initializeDeck() {
    for (int i = 0; i < 5; ++i) {  // Add 5 cards of each type
        cards->push_back(new Card(new Card::CardType(Card::Bomb)));
        cards->push_back(new Card(new Card::CardType(Card::Reinforcement)));
        cards->push_back(new Card(new Card::CardType(Card::Blockade)));
        cards->push_back(new Card(new Card::CardType(Card::Airlift)));
        cards->push_back(new Card(new Card::CardType(Card::Diplomacy)));
    }
}

// Draw a card from the deck at random
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

// Return a played card to the deck
void Deck::returnCard(Card* card) {
    if (card != nullptr) {
        cards->push_back(card);
    } else {
        std::cout << "Cannot return a null card to the deck." << std::endl;
    }
}


// Hand class implementation

// Constructor: Initialize an empty hand
Hand::Hand() : handCards(new std::vector<Card*>()) {}

// Destructor to clean up dynamically allocated memory
Hand::~Hand() {
    for (Card* card : *handCards) {
        delete card;  // Free memory for each card
    }
    delete handCards;  // Free the vector memory
}

// Add a card to the hand
void Hand::addCard(Card* card) {
    handCards->push_back(card);
}

// Play all cards in the hand and return them to the deck
void Hand::playAll(Deck* deck) {
    for (Card* card : *handCards) {  // Play each card in the hand
        card->play();
        deck->returnCard(card);  // Return the card to the deck after playing
    }
    handCards->clear();  // Clear the hand after playing all cards
}
