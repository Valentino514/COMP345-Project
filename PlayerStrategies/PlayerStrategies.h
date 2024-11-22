#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H

class PlayerStrategy{

    virtual void issueOrder() = 0;
    virtual void toAttack() = 0;
    virtual void toDefend() = 0;

};

class HumanPlayerStrategy: public PlayerStrategy{

};

class AggresivePlayerStrategy: public PlayerStrategy{

};


class BenevolentPlayerStrategy: public PlayerStrategy{

};

class NeutralPlayerStrategy: public PlayerStrategy{


};

class CheaterPlayerStrategy: public PlayerStrategy{

};




#endif
