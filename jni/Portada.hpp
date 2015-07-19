#ifndef PORTADA_H
#define PORTADA_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Portada {

private:
    sf::Sprite s;
    sf::Texture t;
    bool open;
    
public:

    Portada() {
        open = true;
    }
    
    ~Portada(){}
    
//     void display(sf::RenderWindow* window){
//         open = true;
//         while(open){
//             
//             sf::Event event;
//         while (window->pollEvent(event)) {
//             if( event.type == sf::Event::KeyPressed ||
//                 event.type == sf::Event::MouseButtonPressed||
//                 event.type == sf::Event::TouchEnded           )  open = false;
//         }
// 
//             t.loadFromFile("res/Portada.png");
//             s.setTexture(t);
//             s.scale(window->getSize().y/s.getGlobalBounds().height,window->getSize().y/s.getGlobalBounds().height);
//             s.setPosition(window->getSize().x/2 - s.getGlobalBounds().width/2, 0);
//             window->draw(s);
// 
//             window->display();
//                 
//         }
//         
//     }
    
    void display(sf::RenderWindow* window, std::string pathImage){
        open = true;
            t.loadFromFile(pathImage);
            s = sf::Sprite();
            s.setTexture(t);
            s.scale(window->getSize().y/s.getGlobalBounds().height,window->getSize().y/s.getGlobalBounds().height);
            s.setPosition(window->getSize().x/2 - s.getGlobalBounds().width/2, 0);
        while(open){
            
            sf::Event event;
                    while (window->pollEvent(event)) {
            if( event.type == sf::Event::KeyPressed ||
                event.type == sf::Event::MouseButtonPressed||
                event.type == sf::Event::TouchEnded           )  open = false;
        }

            window->clear();
            window->draw(s);

            window->display();
                
        }
        
    }

    
};

#endif // PORTADA_H