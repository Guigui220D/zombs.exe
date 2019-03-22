#pragma once

#include "Building.h"

class Sniper : public Building
{
    public:
        Sniper(Game* game, sf::Vector2i pos);
        virtual ~Sniper();

        virtual void update(float delta) override;
        virtual void drawMore(sf::RenderTarget& target) const override;

    private:
        float reloadTime[MAX_UPGRADE + 1] = { 0.f, 1.6f, 1.4f, 1.2f, 1.1f, 1.f };
        int damage = 20;

        sf::Clock reload;

        sf::Sprite canon;
};
