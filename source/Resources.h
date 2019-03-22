#pragma once

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <memory>
#include <iostream>

class Textures
{
    public:
        static bool loadTextures();

        static sf::Texture* getTexture(std::string name);

    private:
        static bool loadTexture(std::string filename, std::string name);

        static std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
};
