#pragma once

#include "Building.h"

class Treasure : public Building
{
    public:
        Treasure(Game* game, sf::Vector2i pos);
        virtual ~Treasure();

    protected:

    private:
};
