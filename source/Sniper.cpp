#include "Sniper.h"

#include "Game.h"

Sniper::Sniper(Game* game, sf::Vector2i pos) :
    Building(game, pos, sf::Vector2i(2, 2), Textures::getTexture("TOWER2_COLOR"), Textures::getTexture("TOWER2_OVERLAY"), 100, 3)
{
    canon.setTexture(*Textures::getTexture("TOWER2_CANON"));
    canon.setOrigin(sf::Vector2f(12.f, 12.f));
    canon.setPosition(getCenter());
    canon.scale(3.f, 3.f);
}

Sniper::~Sniper()
{
    //dtor
}

void Sniper::update(float delta)
{
    updateLife();
    TargetInfo t = getGame()->getNearestZombie();
    if (t.isValid())
    {
        if (reload.getElapsedTime().asSeconds() >= reloadTime[level])
        {
            canon.setRotation(getAngularPos(t.zombie->getPosition()));
            reload.restart();
            t.zombie->dealDamage(damage);
        }
    }
}

void Sniper::drawMore(sf::RenderTarget& target) const
{
    target.draw(canon);
}
