#include "Shaders.h"
#include <iostream>

Shaders::Shaders(sf::RenderWindow* window):
    GameWindow(window)
{
    // Load shaders
    if (!bloomX.loadFromFile("Shaders/BloomX.frag", sf::Shader::Fragment))
    {
        std::cout << "SHADERS: Failed to load BloomX.frag" << "\n";
    }
    else
    {
        std::cout << "SHADERS: BloomX.frag - SUCCESS" << "\n";
    }
    if (!bloomY.loadFromFile("Shaders/BloomY.frag", sf::Shader::Fragment))
    {
        std::cout << "SHADERS: Failed to load BloomY.frag" << "\n";
    }
    else
    {
        std::cout << "SHADERS: BloomY.frag - SUCCESS" << "\n";
    }
    if (!add.loadFromFile("Shaders/add.frag", sf::Shader::Fragment))
    {
        std::cout << "SHADERS: Failed to load add.frag" << "\n";
    }
    else
    {
        std::cout << "SHADERS: add.frag - SUCCESS" << "\n";
    }

    // setUniforms
    float sigma = 5.0f;
    float glowMultiplier = 1.0f;
    float width = GameWindow->getSize().x;
    float height = GameWindow->getSize().y;

    bloomX.setUniform("sigma", sigma);
    bloomX.setUniform("glowMultiplier", glowMultiplier);
    bloomX.setUniform("width", width);

    bloomY.setUniform("sigma", sigma);
    bloomY.setUniform("glowMultiplier", glowMultiplier);
    bloomY.setUniform("height", height);
}

void Shaders::applyBloom(std::shared_ptr<sf::RenderTexture> xRenderTexture)
{ 
    // Create temp textures
    sf::RenderTexture tempTexture;
    sf::RenderTexture yRenderTexture;
    sf::Sprite sprite(xRenderTexture->getTexture());
    sf::Texture sourceTexture(xRenderTexture->getTexture());
    if (!tempTexture.create(GameWindow->getSize().x, GameWindow->getSize().y)) 
    {
        std::cout << "SHADERS: failed to create tempTexture" << "\n";
    }
    if (!yRenderTexture.create(GameWindow->getSize().x, GameWindow->getSize().y))
    {
        std::cout << "SHADERS: failed to create yRenderTexture" << "\n";
    }
    
    bloomX.setUniform("sourceTexture", sourceTexture);
    bloomY.setUniform("sourceTexture", sourceTexture);
    
    // X bloom
    xRenderTexture->draw(sprite, &bloomX);
    xRenderTexture->display();
    // Y bloom
    yRenderTexture.draw(sprite, &bloomY);
    yRenderTexture.display();
   
    //Add X + Y
    add.setUniform("source", xRenderTexture->getTexture());
    add.setUniform("bloom", yRenderTexture.getTexture());    
    tempTexture.draw(sprite, &add);
    
    //Add to original frame
    add.setUniform("bloom", tempTexture.getTexture());
    add.setUniform("source", sourceTexture);
    tempTexture.draw(sprite, &add);
    sf::Sprite result(tempTexture.getTexture());
    GameWindow->draw(result);
    xRenderTexture->clear();
    xRenderTexture->setActive(false);   
}
