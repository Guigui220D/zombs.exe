#pragma once

#include "Building.h"

class Tower1 : public Building
{
    public:
        Tower1(Game* game, sf::Vector2i pos);
        virtual ~Tower1();

        virtual void update(float delta) override;
        virtual void drawMore(sf::RenderTarget& target) const override;

    private:
        float squaredRange = (32.f * 8.f) * (32.f * 8.f);
        float reloadTime = .5f;
        int damage[MAX_UPGRADE + 1] = { 0, 3, 5, 7, 9, 11 };

        sf::Clock reload;

        sf::Sprite canon;
};
