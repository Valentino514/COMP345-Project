#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <vector>

// Card class definition
class Card {
public:
    // Enumeration of card types
    enum CardType { Bomb, Reinforcement, Blockade, Airlift, Diplomacy };

    // Constructor to initialize the card with a type
    Card(CardType* type);

    // Copy constructor
    Card(const Card& other);

    // Assignment operator
    Card& operator=(const Card& other);

    // Destructor to clean up dynamically allocated memory
    ~Card();

    // Stream insertion operator for Card
    friend std::ostream& operator<<(std::ostream& os, const Card& card);

    // Method to play the card
    void play();

    // Get the type of the card
    CardType* getType() const;

    // Return the name of the card type as a string
    std::string getCardTypeName() const;

private:
    CardType* type;  // Holds the type of the card as a pointer
};

// Deck class definition
class Deck {
public:
    Deck();  // Constructor to initialize the deck with cards

    // Copy constructor
    Deck(const Deck& other);

    // Assignment operator
    Deck& operator=(const Deck& other);

    ~Deck();  // Destructor to clean up dynamically allocated memory
    Card* draw();  // Draw a card from the deck
    void returnCard(Card* card);  // Return a played card back to the deck

    // Stream insertion operator for Deck
    friend std::ostream& operator<<(std::ostream& os, const Deck& deck);

private:
    std::vector<Card*>* cards;  // Collection of cards in the deck (as a pointer)
    void initializeDeck();  // Fill the deck with cards
};

// Hand class definition
class Hand {
public:
    Hand();  // Constructor to initialize an empty hand

    // Copy constructor
    Hand(const Hand& other);

    // Assignment operator
    Hand& operator=(const Hand& other);

    ~Hand();  // Destructor to clean up dynamically allocated memory
    void addCard(Card* card);  // Add a card to the hand
    void playAll(Deck* deck);  // Play all cards in the hand, returning them to the deck

    // Stream insertion operator for Hand
    friend std::ostream& operator<<(std::ostream& os, const Hand& hand);

private:
    std::vector<Card*>* handCards;  // Collection of cards in the player's hand (as a pointer)
};

#endif
