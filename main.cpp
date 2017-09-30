#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <chrono>


#define PI 3.14159265
#define title "topdown_shooter"


#include "Player_class.h"
#include "Enemy_class.h"





int main(){
    short int width  = 800;
    short int height = 600;
    short int ai_count = 3;
    Enemy enemys[3];

    auto time_start   =   std::chrono::steady_clock::now();
    auto time_end     =   std::chrono::steady_clock::now();
    auto time_diff_ms =   (time_end - time_start).count();


    sf::RenderWindow window(sf::VideoMode(width, height), title);
    window.setFramerateLimit(60);

    Player P(window, width, height);






    for (unsigned int i = 0; i < ai_count; i++){

            enemys[i].setTarget(P.player_sprite);
            enemys[i].init(window, width, height, width / 2, 10 * i);
            P.add_enemy(enemys[i]);
    }



	// Start the game loop
    while (window.isOpen())
    {
        time_start = std::chrono::steady_clock::now();

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        P.time_update(time_diff_ms);
        for(unsigned int i = 0; i < ai_count; i++){
            enemys[i].time_update(time_diff_ms);
        }
        // Clear screen
        window.clear();


        P.input();

        // Update cords
        P.update_cord();

        // Draw all the sprites
        P.event_loop();


        P.enemy_attack();

        for(unsigned int i = 0; i < ai_count; i++){
            enemys[i].move_enemy();
            enemys[i].event_loop();
        }



        // Update the window
        window.display();

        time_end     = std::chrono::steady_clock::now();
        time_diff_ms = (time_end  - time_start).count();
    }
    return EXIT_SUCCESS;
}
