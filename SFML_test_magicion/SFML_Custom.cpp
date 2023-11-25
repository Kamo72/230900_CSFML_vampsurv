#include "SFML_Custom.h"
#include "SFML_Data.h"
using namespace SFML_Data;


namespace SFML_Custom 
{

    bool LoadTexture(string imagePath, string textureName)
    {
        Texture texture;
        if (!texture.loadFromFile(imagePath)) { return false;/*¹ö±×*/ }
        textureDictionary.Add(textureName, texture);
        return true;
    }

    Sprite MakeSprite(string textureName)
    {
        Sprite sprite(textureDictionary.Get(textureName));
        return sprite;
    }

    //Sprite GetSprite(string name) { return spriteDictionary.Get(name); }

    void DrawSprite(Sprite sprite, float xPos, float yPos, float rotation = 0,
        float xScl = 1.f, float yScl = 1.f, Color color = { 0,0,0 }, float alpha = 255)
    {
        sprite.setPosition(xPos, yPos);
        sprite.setRotation(rotation);
        sprite.setScale(xScl, yScl);
        Color tempColor = { color.r,color.g,color.b, alpha };
        sprite.setColor(tempColor);

        (*nowWindow).draw(sprite);
    }
}
