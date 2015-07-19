#ifndef LEVELMENU_H
#define LEVELMENU_H

#include "button.h"
#include "menu2.hpp"
#include <sstream> 
#include <string>   
#include <fstream>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class LevelMenu {

private:
    bool open;
    int actualLvl;
    Button forward;
    Button backward;
    Button centralImage;
    TextMenu text;
    int maxScore;
    int bestTime;
    
public:
   

    LevelMenu() {
        open = true;
        actualLvl = 0;
        forward.setPosition(0,0);
        forward.setSize(100,100);
        forward.setTextResizeText(" > ");
        backward.setPosition(0,0);
        backward.setSize(100,100);
        backward.setTextResizeText(" < ");
        centralImage.setText("");
    }
    
    ~LevelMenu(){}
    
    int display(sf::RenderWindow* window){
        
        forward.setSize(100,100);
        forward.setPosition(window->getSize().x - backward.getSize().x -10, window->getSize().y/2);
        
        backward.setSize(100,100);
        backward.setPosition(0 +10, window->getSize().y/2);
        
        centralImage.setSize(window->getSize().x*3/4, window->getSize().y*3/4);
        centralImage.setPosition(window->getSize().x/2 - centralImage.getSize().x/2, window->getSize().y/2 - centralImage.getSize().y/2);
        centralImage.disableClickEffect();
            
        open = true;
        bool clicked = false;
        bool reloadImage = true;
        while(open){
            
            sf::Event event;
            while (window->pollEvent(event)) {
                forward.handleEvent(event);
                backward.handleEvent(event);
                centralImage.handleEvent(event);
                switch (event.type) {
                case sf::Event::Closed:
                    window->close();
                    exit(0);
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) { window->close(); exit(0);}
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        clicked = true;
                    }
                default:
                    break;
                }
            }
            
            if (forward.hasBeenClicked()) {
                ++actualLvl;

                std::stringstream str; str << "res/board" << actualLvl << ".png";       
                std::string filePath = str.str();

                sf::Image img;
                if (img.loadFromFile(filePath)) {
                    int actual = img.getPixel(0,0).r;
                    if(actual < 2) {
                        reloadImage = true;
                        maxScore = img.getPixel(0,0).g;
                        bestTime = img.getPixel(0,0).b;
                    }
                    else { 
                        reloadImage = false;
                        --actualLvl;
                    }
                }
                else { 
                    reloadImage = false;
                    --actualLvl;
                }

            }
            if (backward.hasBeenClicked()) {
                if(actualLvl > 0) {
                    --actualLvl;
                    
                    std::stringstream str; str << "res/board" << actualLvl << ".png";       
                    std::string filePath = str.str();

                    sf::Image img;
                    if(img.loadFromFile(filePath)){
                    
                        int actual = img.getPixel(0,0).r;
                        if(actual < 2) {
                            reloadImage = true;
                            maxScore = img.getPixel(0,0).g;
                            bestTime = img.getPixel(0,0).b;
                        }
                        else { 
                            reloadImage = false;
                            ++actualLvl;
                        }
                    }                        
                }
            }
            if (centralImage.hasBeenClicked()) return actualLvl;
            
            if(reloadImage){
                std::stringstream st;
                st << "board" << actualLvl;       
                std::string board = st.str();
                centralImage.setTexture("res/"+board+".png");
                reloadImage = false;
            }
            
            window->clear();
            forward.draw(*window);
            backward.draw(*window);
            std::stringstream conversor;
            conversor << maxScore;
            text.drawTextPos(window, "Max Score: " + conversor.str(), sf::Color::White, 0, 0);
            std::stringstream conversor2;
            conversor2 << bestTime;
            text.drawTextPos(window, "Best Time: " + conversor2.str(), sf::Color::White, 0, 50);
            centralImage.draw(*window);
            window->display();       
        }
    }
};

#endif // LEVELMENU_H