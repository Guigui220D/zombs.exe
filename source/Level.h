#pragma once

#include <SFML/System.hpp>

class Building;

class Level
{
    public:
        typedef Building* BuildingPtr;

        Level(sf::Vector2i size);
        virtual ~Level();

        inline sf::Vector2i getSize() const { return size; };

        BuildingPtr get(sf::Vector2i pos) const;

        bool testSpaceAvailability(sf::Vector2i pos, sf::Vector2i size) const;

        bool placeBuilding(BuildingPtr building);
        void removeBuilding(BuildingPtr building);

    private:
        const sf::Vector2i size;
        BuildingPtr* map;
};
