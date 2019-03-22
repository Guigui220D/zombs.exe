#include "Treasure.h"

Treasure::Treasure(Game* game, sf::Vector2i pos) :
    Building(game, pos, sf::Vector2i(2, 2), Textures::getTexture("TREASURE_COLOR"), Textures::getTexture("TREASURE_OVERLAY"), 150, 5)
{
    //ctor
}

Treasure::~Treasure()
{
    //dtor
}
