#include "Wall.h"

Wall::Wall(Game* game, sf::Vector2i pos) :
    Building(game, pos, sf::Vector2i(1, 1), Textures::getTexture("WALL_COLOR"), Textures::getTexture("WALL_OVERLAY"), 5)
{
    //ctor
}

Wall::~Wall()
{
    //dtor
}
