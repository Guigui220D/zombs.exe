#include "Tower1.h"

#include "Game.h"

Tower1::Tower1(Game* game, sf::Vector2i pos) :
    Building(game, pos, sf::Vector2i(2, 2), Textures::getTexture("TOWER1_COLOR"), Textures::getTexture("TOWER1_OVERLAY"), 70, 3)
{
    canon.setTexture(*Textures::getTexture("TOWER1_CANON"));
    canon.setOrigin(sf::Vector2f(12.f, 12.f));
    canon.setPosition(getCenter());
    canon.scale(2.f, 2.f);
}

Tower1::~Tower1()
{
    //dtor
}

void Tower1::update(float delta)
{
    updateLife();
    TargetInfo t = getGame()->getNearestZombie(getCenter());
    if (t.isValid() && t.squaredDistance <= squaredRange)
    {
        if (reload.getElapsedTime().asSeconds() >= reloadTime && !t.zombie->isDead())
        {
            canon.setRotation(getAngularPos(t.zombie->getPosition()));
            reload.restart();
            t.zombie->dealDamage(damage[level]);
        }
    }
}

void Tower1::drawMore(sf::RenderTarget& target) const
{
    target.draw(canon);
}
