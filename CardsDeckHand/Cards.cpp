#include "Cards.h"
#include <cstdlib>  // For rand()
#include <ctime>    // For time()

// Card class implementation

Card::Card(CardType* t) : type(new CardType(*t)) {}

// Copy constructor
Card::Card(const Card& other) : type(new CardType(*other.type)) {}

// Assignment operator
Card& Card::operator=(const Card& other) {
    if (this != &other) {
        delete type;  // Free existing memory
        type = new CardType(*other.type);  // Deep copy
    }
    return *this;
}

// Destructor
Card::~Card() {
    delete type;  // Free the dynamically allocated card type
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Card& card) {
    os << card.getCardTypeName();
    return os;
}

// Play method
void Card::play() {
    std::cout << "Playing card: " << *this << std::endl;
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

Deck::Deck() : cards(new std::vector<Card*>()) {
    initializeDeck();
    srand(time(0));  // Seed for random card drawing
}

// Copy constructor
Deck::Deck(const Deck& other) : cards(new std::vector<Card*>()) {
    for (Card* card : *other.cards) {
        cards->push_back(new Card(*card));  // Deep copy each card
    }
}

// Assignment operator
Deck& Deck::operator=(const Deck& other) {
    if (this != &other) {
        for (Card* card : *cards) {
            delete card;  // Free existing cards
        }
        cards->clear();  // Clear the vector

        for (Card* card : *other.cards) {
            cards->push_back(new Card(*card));  // Deep copy each card
        }
    }
    return *this;
}

// Destructor
Deck::~Deck() {
    for (Card* card : *cards) {
        delete card;
    }
    delete cards;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Deck& deck) {
    for (Card* card : *deck.cards) {
        os << *card << " ";
    }
    return os;
}

// Initialize the deck with several cards of each type
void Deck::initializeDeck() {
    for (int i = 0; i < 5; ++i) {
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
    int randomIndex = rand() % cards->size();
    Card* drawnCard = (*cards)[randomIndex];
    cards->erase(cards->begin() + randomIndex);
    return drawnCard;
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

Hand::Hand() : handCards(new std::vector<Card*>()) {}

// Copy constructor
Hand::Hand(const Hand& other) : handCards(new std::vector<Card*>()) {
    for (Card* card : *other.handCards) {
        handCards->push_back(new Card(*card));  // Deep copy each card
    }
}

// Assignment operator
Hand& Hand::operator=(const Hand& other) {
    if (this != &other) {
        for (Card* card : *handCards) {
            delete card;
        }
        handCards->clear();

        for (Card* card : *other.handCards) {
            handCards->push_back(new Card(*card));  // Deep copy each card
        }
    }
    return *this;
}

// Destructor
Hand::~Hand() {
    for (Card* card : *handCards) {
        delete card;
    }
    delete handCards;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Hand& hand) {
    for (Card* card : *hand.handCards) {
        os << *card << " ";
    }
    return os;
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
    handCards->clear();
}
