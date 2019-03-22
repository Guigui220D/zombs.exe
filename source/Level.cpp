#include "Level.h"

#include <assert.h>

#include "Building.h"

Level::Level(sf::Vector2i size) :
    size(size)
{
    assert(size.x > 0 && size.y > 0);

    map = new BuildingPtr[size.x * size.y];

    for (auto i = 0; i < size.x * size.y; i++)
        map[i] = nullptr;
}

Level::~Level()
{
    delete[] map;
}

Level::BuildingPtr Level::get(sf::Vector2i pos) const
{
    if (pos.y < 0 || pos.y < 0 || pos.x >= size.x || pos.y >= size.y)
        return nullptr;
    return map[pos.x + pos.y * size.x];
}

bool Level::testSpaceAvailability(sf::Vector2i pos, sf::Vector2i size) const
{
    assert(size.y >= 0 && size.y >= 0);

    int rx, ry;
    for (int x = 0; x < size.x; x++)
    for (int y = 0; y < size.y; y++)
    {
        rx = pos.x + x;
        ry = pos.y + y;

        if (rx < 0 || ry < 0 || rx >= Level::size.x || ry >= Level::size.y)
            return false;
        if (get(sf::Vector2i(rx, ry)))
            return false;
    }
    return true;
}

bool Level::placeBuilding(BuildingPtr building)
{
    sf::Vector2i bpos = building->getPos();
    sf::Vector2i bsize = building->getSize();
    if (testSpaceAvailability(bpos, bsize))
    {
        int rx, ry;
        for (int x = 0; x < bsize.x; x++)
        for (int y = 0; y < bsize.x; y++)
        {
            rx = bpos.x + x;
            ry = bpos.y + y;
            map[rx + ry * size.x] = building;
        }
        return true;
    }
    else
        return false;
}

void Level::removeBuilding(BuildingPtr building)
{
    sf::Vector2i bpos = building->getPos();
    sf::Vector2i bsize = building->getSize();
    int rx, ry;
    for (int x = 0; x < bsize.x; x++)
    for (int y = 0; y < bsize.x; y++)
    {
        rx = bpos.x + x;
        ry = bpos.y + y;
        map[rx + ry * size.x] = nullptr;
    }
}
