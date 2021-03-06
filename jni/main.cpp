//GRAVITY
#include <map>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "button.h"
#include "menu2.hpp"
#include "Portada.hpp"
#include "LevelMenu.hpp"

namespace sf{
    bool operator< (const sf::Color& c1, const sf::Color& c2){
        if(c1.r < c2.r) return true; else if(c1.r > c2.r) return false;
        else if(c1.g < c2.g) return true; else if(c1.g > c2.g) return false;
        else if(c1.b < c2.b) return true; else if(c1.b > c2.b) return false;
        else if(c1.a < c2.a) return true; else if(c1.a > c2.a) return false;
        else return false;
    }
}

sf::Color getColisionColor(float posx, float posy, sf::Image& img, sf::Sprite& bSprite){
    return img.getPixel( posx/bSprite.getScale().x, posy/bSprite.getScale().y);
}

int main(){
    
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Gravity");
    sf::Vector2f screenSize = sf::Vector2f(window.getSize().x,window.getSize().y);

    sf::RectangleShape r(sf::Vector2f(screenSize.x/10, screenSize.y/10));
    r.setPosition(1,1); r.setFillColor(sf::Color::White);
    
    Portada p;
    Button button;
    
    sf::Image bimg;
    sf::Clock timer;
    sf::Texture bTex;
    sf::Sprite bSprite;
    sf::Vector2f v = sf::Vector2f(0,0);

    sf::Text text; sf::Font font; 
    if(! font.loadFromFile("res/font.ttf")) std::cerr << "fail on font load" << std::endl;
    text.setFont(font); text.setPosition(0,0); text.setString("penguin <3");
    text.setColor(sf::Color(255,255,255));
    
    std::string path;
    std::map<sf::Color, sf::Time> colorsColiding;
    
    int pantalla = 0;
    float deltatime = 0;
    float boardTime = 0;
    bool reboot = false;
    bool needshiet = true;
    float g = (int)screenSize.y*2 ;
    float ground = screenSize.y-4; //  float ground = screenSize.y*6/7;
    
    
    //GAME LOOP
    while(window.isOpen()){
        if(needshiet){
            v = sf::Vector2f(0,0);
            colorsColiding.clear();
            r = sf::RectangleShape(sf::Vector2f(screenSize.x/10, screenSize.y/10));
            r.setPosition(0,0); r.setOrigin(0,0);
            r.setScale(1, 1); r.setRotation(0);
            r.setFillColor(sf::Color::White);
            std::stringstream s;
            s << "board" << pantalla;       
            std::string board = s.str();
            path = "res/"+board+".png";
            if(!bimg.loadFromFile(path)) std::cerr << "I CAN'T LOAD BOARD image" << std::endl;
            if(!bTex.loadFromFile(path)) std::cerr << "I CAN'T LOAD BOARD texture" << std::endl;
            bSprite.setTexture(bTex, true);
            bSprite.setScale(screenSize.x/bSprite.getLocalBounds().width , 
                            screenSize.y/bSprite.getLocalBounds().height);
            
            needshiet = false;
            deltatime = 0;
            boardTime = 0;
            timer.restart();
            
            if(pantalla < 2){
                button.setSize((window.getSize().x/10), (window.getSize().x/20));
                button.setPosition(window.getSize().x-button.getSize().x - 10,window.getSize().y-button.getSize().y - 10);
                button.setTextResizeText(" Levels ");
                button.setTexture("res/button.png");
            } 
            
        }
        
        deltatime = timer.restart().asSeconds();
        boardTime += deltatime;
        //effect.update(deltatime);
        
        sf::Event event;
        if(window.hasFocus()) 
        while(window.pollEvent(event)) {
            button.handleEvent(event);
            if (event.type == sf::Event::Closed) window.close(); 
            if (event.type == sf::Event::Resized) {window.setSize(sf::Vector2u(event.size.width, event.size.height)); needshiet = true;}
            if (pantalla < 2 && event.type == sf::Event::MouseButtonPressed){
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if(!button.isClicked()) r.setPosition(event.mouseButton.x*screenSize.x/window.getSize().x, event.mouseButton.y*screenSize.y/window.getSize().y);
                }
            }
            if (pantalla < 2 && sf::Touch::isDown(0)) {
                sf::Vector2i position = sf::Touch::getPosition(0, window);
                 if(!button.isClicked()) r.setPosition(position.x*screenSize.x/window.getSize().x, position.y*screenSize.y/window.getSize().y);
            }
        }
        
        if(pantalla < 2 && button.hasBeenClicked()){
            // arrenca el menú
            LevelMenu levelMenu;
            pantalla = levelMenu.display(&window);
            needshiet = true;
            timer.restart();
        }
        
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        
        if (sf::Touch::isDown(0)) {
            sf::Vector2i position = sf::Touch::getPosition(0);
            float quart = window.getSize().x/4;
            if(position.x > 3*window.getSize().x/4){
                v.x = window.getSize().x/20;
                }
            else if(position.x < window.getSize().x/4){
                v.x = (int)window.getSize().x/20 * -1;
                }
            if(r.getPosition().y > 0){
                v.y = (int)window.getSize().y/2 * -1;
            }
        }

        if(pantalla < 2) r.move(v*3.0f * deltatime);
        else r.move(v * deltatime);
        
        if(r.getPosition().y < 0) {v.y += g*deltatime;}
        if( (r.getPosition().y + r.getSize().y) < ground || v.y < 0) v.y += g *deltatime;
        else {
            r.setPosition(r.getPosition().x, ground - r.getSize().y);
            v.y = 0;
        }
        if(r.getPosition().x < 1) r.setPosition(1, r.getPosition().y);
        if(r.getPosition().x + r.getSize().x+3 > window.getSize().x) r.setPosition(window.getSize().x-r.getSize().x-3, r.getPosition().y);
    
        
        sf::VertexArray pj(sf::Quads, 4);
        sf::FloatRect fr = r.getGlobalBounds();
        pj[0].position = sf::Vector2<float>(r.getPosition().x, r.getPosition().y);
        pj[1].position = sf::Vector2<float>(r.getPosition().x + fr.width-2, r.getPosition().y);
        pj[3].position = sf::Vector2<float>(r.getPosition().x, r.getPosition().y + fr.height-2);
        pj[2].position = sf::Vector2<float>(r.getPosition().x + fr.width-2, r.getPosition().y + fr.height-2);

        for(int i = 0; i < 4; ++i) pj[i].color = sf::Color::Black;
        
        if(r.getPosition().y >= 0 && r.getPosition().x+r.getSize().x < screenSize.x-1 && r.getPosition().x > 1 && r.getPosition().y+r.getSize().y < screenSize.y) {

            sf::Color color = getColisionColor(r.getPosition().x, r.getPosition().y, bimg, bSprite);
            if(color != sf::Color::Black) colorsColiding[color]  += sf::seconds(deltatime);
            sf::Color color2 = getColisionColor(r.getPosition().x + fr.width, r.getPosition().y, bimg, bSprite);
            if(color2 != sf::Color::Black) colorsColiding[color2] += sf::seconds(deltatime);
            pj[0].color = color; pj[1].color = color2; 
            sf::Color color3 = getColisionColor(r.getPosition().x, r.getPosition().y + fr.height , bimg, bSprite);
            if(color3 != sf::Color::Black) colorsColiding[color3] += sf::seconds(deltatime);
            sf::Color color4 = getColisionColor(r.getPosition().x + fr.width, r.getPosition().y + fr.height, bimg, bSprite);
            if(color4 != sf::Color::Black) colorsColiding[color4] += sf::seconds(deltatime);
            pj[3].color = color3; pj[2].color = color4;   
        }
        else if(r.getPosition().y+fr.height >= 0){
            sf::Color color3 = getColisionColor(r.getPosition().x, r.getPosition().y + fr.height , bimg, bSprite);
            if(color3 != sf::Color::Black) colorsColiding[color3] += sf::seconds(deltatime);
            sf::Color color4 = getColisionColor(r.getPosition().x + fr.width, r.getPosition().y + fr.height, bimg, bSprite);
            if(color4 != sf::Color::Black) colorsColiding[color4] += sf::seconds(deltatime);
            pj[3].color = color3; pj[2].color = color4;     
        }
                
        r.setOutlineColor(sf::Color::White);
        r.setOutlineThickness(1);
        
        std::map<std::string, int> colorTimers;
        colorTimers["Red:"] = colorTimers["Yellow:"] = colorTimers["Green:"] = colorTimers["Blue:"] = 0; 
        
        for (std::map<sf::Color, sf::Time>::iterator it=colorsColiding.begin(); it!=colorsColiding.end(); ++it){
            std::string col = "wat:";
            sf::Color aux = (it->first);
            if(aux.r >= aux.g and aux.r >= aux.b) {
                if(aux.g < 100) col = "Red:";
                else col = "Yellow:";
            }
            else if(aux.g >= aux.r and aux.g >= aux.b) col = "Green:";
            else if(aux.b >= aux.g and aux.b >= aux.r) col = "Blue:";
            if((int)(it->second).asSeconds() > 0) 
                colorTimers[col] += (int)(it->second).asSeconds();
        }
        
        std::stringstream ss;
        for (std::map<std::string, int>::iterator it=colorTimers.begin(); it!=colorTimers.end(); ++it){
            if(it->second > 0) 
                ss << " " << it->first << "" << it->second;     
        }
        std::string str = ss.str();
        text.setString(str);
        
        sf::Text textBg = text;
        textBg.setScale(1.1,1.1);
        textBg.setColor(sf::Color(100,100,100));
        
        int max = 0;
        int qtty = 0;
        int min = 99999999;
        if(colorsColiding[sf::Color::White] != sf::seconds(0.0) || reboot){
            std::ostringstream oss;
            //COMPUTING MIN AND MAX NUMBER OF THE COLORS
            for (std::map<std::string, int>::iterator it=colorTimers.begin(); it!=colorTimers.end(); ++it){
                int num = (int)(it->second);
                if(num > 0) {
                    if(num > max) max = num; if(num < min) min = num;
                    ++qtty;
                }
            }
            //IF THE NEW LEVEL HAVE TO BE SETTED
            if((max - min <= 3 && qtty >= 4) || reboot || pantalla < 2) {
                oss << max;
                std::string strn = oss.str();
                if(!reboot) str = "Well Done! score = " + strn; //text.setString("YouWonTheGame!   punctuation = " + strn);
                else str = " Nice try! ";                                   //text.setString(" Nice try!");
                if(window.hasFocus()) window.clear();
                if(window.hasFocus()) window.draw(bSprite);
                //if(window.hasFocus()) window.draw(r);
                //if(window.hasFocus()) window.display();
                
                sf::Clock c; 
                int maxT = 3;
                float t = 0.0;
                float deltaAux = 0.0;
                r.setOrigin(r.getSize().x/2, r.getSize().y/2);
                r.move(r.getSize().x/2, r.getSize().y/2);
                
                //MAKE THE ANIMATION TO CHANGE TO THE NEXT LVL
                while(t < maxT){ 
                    if(window.hasFocus())
                    while(window.pollEvent(event)) {
                        //button.handleEvent(event);
                        if (event.type == sf::Event::Closed) { window.close(); exit(0);}
                        if (event.type == sf::Event::Resized) {window.setSize(sf::Vector2u(event.size.width, event.size.height)); needshiet = true;}
                        if (event.type == sf::Event::KeyPressed){ if (event.key.code == sf::Keyboard::Escape) { window.close(); exit(0); } }
                    }
                    r.setOutlineThickness(0);
                    deltaAux = c.restart().asSeconds();
                    t += deltaAux;
                    r.setRotation(360/maxT*t);
                    r.setScale(1-t/maxT,1-t/maxT);
                    r.setFillColor(sf::Color(83*t, 83*t, 83*t));
                    if(window.hasFocus()) window.draw(bSprite);
                    if(pantalla >= 2){ //IN THE CASE OF COVER AND INSTRUCTIONS
                        for(int i = 0; i < str.size(); ++i) {
                            text.setString(str[i]);
                            textBg.setString(str[i]);
                            text.setPosition(text.getCharacterSize()*i, 0);
                            textBg.setPosition(text.getCharacterSize()*i, 0);
                            if(window.hasFocus()) window.draw(textBg, sf::BlendAlpha);
                            if(window.hasFocus()) window.draw(text, sf::BlendAlpha);
                        }
                    }
                    if(window.hasFocus())window.draw(r);
                    if(window.hasFocus()) window.display();
                    timer.restart();
                }
                //IF IT IS A NEW LEVEL (NOT REBOOT)
                if(!reboot) {
                    if(pantalla == 1) {
                        p.display(&window, "res/credits.png");    
                        //textMenu.displayText(&window, "Start");
    //                        textMenu.display_recieve_text(&window);
                    } else if (pantalla == 0) {
                        //do nothing
                    } else {
                        std::stringstream ss;
                        ss << pantalla;
                        std::string str = ss.str();
                        //textMenu.displayText(&window, "Next Level -> "+str);

                    }
                    
                    
                    
                    int bestTime = bimg.getPixel(0,0).b;
                    int bestScore = bimg.getPixel(0,0).g;

                        
                    int actual = max;
                    int score = bestScore;
                        if(actual < score) bestScore = actual;
                        else if(actual > score) bestScore = score;
                        else if(actual < score) bestScore = actual;
                        else bestScore = score;
                        
                        //getline (myfile,actual);
                        actual = boardTime;
                    int bTime = bestTime;
                        if(actual < bTime) bestTime = actual;
                        else if(actual > bTime) bestTime = bTime;
                        else if(actual < bTime) bestTime = actual;
                        else bestTime = bTime;
                            
                    
                    bimg.setPixel(0,0,sf::Color(250,bestTime,bestScore));
                    if(!bimg.saveToFile(path)){
                        //NO PUTO FUNCIONA
                        //p.display(&window, "res/credits.png"); 
                    }
                    ++pantalla;
                }
                needshiet = true;
                reboot = false;
            }
            else reboot = true;
            
        }
        else{
            if(window.hasFocus()) window.clear();
            if(window.hasFocus()) window.draw(bSprite);
            if(window.hasFocus()) window.draw(r);
            if(window.hasFocus()) window.draw(pj, sf::BlendAlpha);
            if(pantalla < 2) button.draw(window);
            else for(int i = 0; i < str.size(); ++i) {
                text.setString(str[i]);
                textBg.setString(str[i]);
                text.setPosition(text.getCharacterSize()/1.5*i, 0);
                textBg.setPosition(text.getCharacterSize()/1.5*i, 0);
                if(window.hasFocus()) window.draw(textBg, sf::BlendAlpha);
                if(window.hasFocus()) window.draw(text, sf::BlendAlpha);
            }
            //if(window.hasFocus()) window.draw(bSprite);
            sf::VertexArray pj_top(sf::Triangles, 3);
            sf::VertexArray pj_bot(sf::Triangles, 3);
            pj_top[0] = pj[0]; pj_top[1] = pj[1]; pj_top[2] = pj[3];
            pj_bot[0] = pj[1]; pj_bot[1] = pj[2]; pj_bot[2] = pj[3];
            if(window.hasFocus()) window.draw(pj_top, sf::BlendAlpha);
            if(window.hasFocus()) window.draw(pj_bot, sf::BlendAlpha);
            if(window.hasFocus()) window.display();
        }
    }
}

