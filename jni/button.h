#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string.h>

class Button{

public:

    //CONSTRUCTOR
    

Button(){
    text.setString("");
    clicked = false;
    is_clicked = false;
    clickEffect = true;

    if(!font.loadFromFile("defaultFont.otf")){ std::cerr << "Can't find the font file" << std::endl; }
    else setFont(font); setCharacterSize(30); setTextColor(sf::Color::White);

    if(!texture.loadFromFile("defaultButton.png")) std::cerr << "Default texture not loaded" << std::endl;
    else sprite.setTexture(texture);

    if(!pressed_texture.loadFromFile("defaultPressedButton.png")) std::cerr << "presedButton text. not loaded" << std::endl;

    setPosition(0,0);
}

Button(std::string myText, std::string texturePath, float sizeX, float sizeY){
    text.setString(myText);
    clicked = false;
    is_clicked = false;
    clickEffect = true;

    if(!font.loadFromFile("defaultFont.otf")){ std::cerr << "Can't find the font file" << std::endl; }
    else setFont(font); setCharacterSize(30); setTextColor(sf::Color::White);

    if(!texture.loadFromFile(texturePath)) std::cerr << "Failed on load texture from " << texturePath << std::endl;
    else sprite.setTexture(texture);

    disableClickEffect();

    setPosition(0,0);
}

Button(std::string myText, std::string texturePath, std::string pressedTexturePath, float sizeX, float sizeY){
    text.setString(myText);
    clicked = false;
    is_clicked = false;
    clickEffect = true;

    if(!font.loadFromFile("defaultFont.otf")){ std::cerr << "Can't find the font file" << std::endl; }
    else setFont(font); setCharacterSize(30); setTextColor(sf::Color::White);

    if(!texture.loadFromFile(texturePath)) std::cerr << "Failed on load texture from " << texturePath << std::endl;
    else sprite.setTexture(texture);

    if(!pressed_texture.loadFromFile(pressedTexturePath))  std::cerr << "Failed on load texture from " << pressedTexturePath << std::endl;
    else sprite.setTexture(pressed_texture);    

    setPosition(0,0);
}

void setTextColor(sf::Color c){text.setColor(c); }
sf::Color getTextColor(){ return text.getColor(); }

void setCharacterSize(uint u){ text.setCharacterSize(u); }
uint getCharacterSize(){ return text.getCharacterSize(); }

void setFont(sf::Font f){ font = f; text.setFont(font); }


void setPosition(sf::Vector2f position){
    sprite.setPosition(position);
    text.setPosition(position.x +
                     sprite.getGlobalBounds().width/2 -
                     text.getGlobalBounds().width/2,
        position.y + sprite.getGlobalBounds().height/2 - (text.getGlobalBounds().height/2));
}

void setPosition(float x, float y){
    setPosition(sf::Vector2f(x, y));
}

sf::Vector2f getPosition(){
    return sprite.getPosition();
}

sf::Vector2f getSize(){
    return sf::Vector2f(sprite.getGlobalBounds().width,sprite.getGlobalBounds().height);
}


void setSize(sf::Vector2f size){
    sprite.scale(size.x/sprite.getGlobalBounds().width, size.y/sprite.getGlobalBounds().height);
    this->setTextResizeText(text.getString());
}

void setSize(float x, float y){ setSize(sf::Vector2f(x,y)); }

bool isClicked(){ return is_clicked; }

bool hasBeenClicked(){
    bool r = clicked;
    if(clicked) clicked = false;
    return r;
}

float timeSinceLastClick(){ return clock.getElapsedTime().asSeconds(); }

void setText(std::string s = "Click"){
    text.setString(s);
    if(s.size() != 0){
        float actualCharSize, desiredCharSize;
        float actualTextSize, desiredTextSize;
        actualCharSize = text.getCharacterSize();
        actualTextSize = text.getGlobalBounds().width+2;
        desiredTextSize = sprite.getGlobalBounds().width;
        desiredCharSize = actualCharSize*desiredTextSize/actualTextSize;
        text.setCharacterSize(desiredCharSize);
    }
}

std::string getText(){ return text.getString();}


void setTextResizeButton(std::string s = "Click"){
    text.setString(s);
    if(s.size() != 0){
        float actualTextSize, factor;
        actualTextSize = text.getGlobalBounds().width;
        factor = (actualTextSize)/sprite.getGlobalBounds().width;
        sprite.scale(factor, 1);      
    }
}

void setTextResizeText(std::string s = "Click"){
    text.setString(s);
    if(s.size() != 0){
        float actualTextSize, factor;
        actualTextSize = text.getGlobalBounds().width;
        factor = sprite.getGlobalBounds().width/(actualTextSize);
        text.scale(factor, factor);      
    }
}

void draw(sf::RenderWindow& w){
    sf::Vector2f position = getPosition();
    text.setPosition(position.x + sprite.getGlobalBounds().width/2 - text.getGlobalBounds().width/2, position.y + sprite.getGlobalBounds().height/2 - text.getGlobalBounds().height/2);
    w.draw(sprite);
    w.draw(text);
}

void setTexture(std::string name){
    float sizeX = getSize().x;
    float sizeY = getSize().y;
    
    if(!texture.loadFromFile(name)) std::cerr << "Default texture not loaded on setTexture" << std::endl;
    else sprite.setTexture(texture, true); 
    
    setSize(sizeX, sizeY);
    
}

void setPressedTexture(std::string name){
    if(!pressed_texture.loadFromFile(name)) std::cerr << "presedButton text. not loaded on Setter" << std::endl; }

void enableClickEffect(){ clickEffect = true; }
void disableClickEffect(){ clickEffect = false; }
bool clickEffectActivated(){ return clickEffect; }

void handleEvent(sf::Event e){
    if(e.type == sf::Event::TouchBegan){
        if (sf::Touch::isDown(0)) {
        sf::Vector2i position = sf::Touch::getPosition(0);
        sf::Vector2f tclick(position.x, position.y);
            if(tclick.x > sprite.getPosition().x && tclick.x < sprite.getPosition().x+sprite.getGlobalBounds().width){
                if(tclick.y > sprite.getPosition().y && tclick.y < sprite.getPosition().y+sprite.getGlobalBounds().height){
                    clicked = true;
                    is_clicked = true;
                    clock.restart();
                    if(clickEffect && sprite.getTexture() == &texture) sprite.setTexture(pressed_texture);
                }
            }
        }
    }
    if(e.type == sf::Event::TouchEnded){
        is_clicked = false;
        if(clickEffect && sprite.getTexture() != &texture) sprite.setTexture(texture);
    }
}

    
private:

    bool clicked;
    bool is_clicked;
    bool clickEffect;

    sf::Font font;
    sf::Text text;
    sf::Clock clock;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Texture pressed_texture;
    float time_since_last_click;

};

#endif // BUTTON_H
