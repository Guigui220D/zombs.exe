#include "Resources.h"

#include <assert.h>

std::unordered_map<std::string, std::unique_ptr<sf::Texture>> Textures::textures;

bool Textures::loadTextures()
{
    bool success =
    loadTexture("gold-pile-color", "TREASURE_COLOR") &
    loadTexture("gold-pile-overlay", "TREASURE_OVERLAY") &
    loadTexture("wall-color", "WALL_COLOR") &
    loadTexture("wall-overlay", "WALL_OVERLAY") &
    loadTexture("tower-1-canon", "TOWER1_CANON") &
    loadTexture("tower-1-color", "TOWER1_COLOR") &
    loadTexture("tower-1-overlay", "TOWER1_OVERLAY") &
    loadTexture("tower-2-canon", "TOWER2_CANON") &
    loadTexture("tower-2-color", "TOWER2_COLOR") &
    loadTexture("tower-2-overlay", "TOWER2_OVERLAY");
    return success;
}

sf::Texture* Textures::getTexture(std::string name)
{
    auto i = textures.find(name);
    assert(i != textures.end());
    return (*i).second.get();
}

bool Textures::loadTexture(std::string filename, std::string name)
{
    if (textures.find(name) != textures.end())
    {
        std::cerr << "Could not load texture with name \"" << name << "\", name already exists." << std::endl;
        return false;
    }
    sf::Texture* tex = new sf::Texture();
    bool success = tex->loadFromFile("res/png/" + filename + ".png");
    if (success)
    {
        textures.emplace(std::pair<std::string, std::unique_ptr<sf::Texture>>(name, std::unique_ptr<sf::Texture>(tex)));
    }
    else
        std::cerr << "Could not load file \"res/png/" << filename << ".png\"." << std::endl;
    return success;
}
