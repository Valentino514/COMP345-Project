#include "Cards.h"
#include <cstdlib>  // For rand()
#include <ctime>    // For time()

// Card class implementation

// Constructor 
Card::Card(CardType* t) : type(new CardType(*t)) {}

// Destructor
Card::~Card() {
    delete type; 
}

Card Card::getRandomCard() {
    CardType randomType = static_cast<CardType>(rand() % 5); // Generate random type
    return Card(new CardType(randomType));
}

void Card::play() {
    std::cout << "Playing card: " << getCardTypeName() << std::endl;
}

Card::CardType Card::getType() const {
    return *type;
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

// Constructor
Deck::Deck() : cards(new std::vector<Card*>()) {
    initializeDeck(); 
    srand(time(0));  
}

// Destructor 
Deck::~Deck() {
    for (Card* card : *cards) {
        delete card;  
    }
    delete cards;  
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
    cards->push_back(card);  
}

// Hand class implementation

// Constructor
Hand::Hand() : handCards(new std::vector<Card*>()) {}

// Destructor
Hand::~Hand() {
    for (Card* card : *handCards) {
        delete card; 
    }
    delete handCards;  
}

// Add a card to the hand
void Hand::addCard(Card* card) {
    handCards->push_back(card);
}

// Play all cards in the hand and return them to the deck
void Hand::playAll(Deck* deck) {
    for (Card* card : *handCards) {  
        card->play();
        deck->returnCard(card);  
    }
    handCards->clear();  // Clear the hand after playing all cards
}




