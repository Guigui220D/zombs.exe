#pragma once

#include "Building.h"

class Wall : public Building
{
    public:
        Wall(Game* game, sf::Vector2i pos);
        virtual ~Wall();

    protected:

    private:
};
