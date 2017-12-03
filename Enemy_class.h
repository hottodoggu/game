#ifndef ENEMY_CLASS_H_INCLUDED
#define ENEMY_CLASS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#define PI 3.14159265
#define title "topdown_shooter"





class Enemy{
public:
    bool pause = false;
    

    int health = 100;
    
    int load_sprite(sf::Sprite &S ,std::string file_path){
        /*
            Load texture into a 256 objects big cache
            Then load the texture into the provided sprite.
        */
        if (!texture_cache_index < 256){
            sf::Sprite *S_ptr = &S;
            sf::Texture texture;
            texture_cache[texture_cache_index] = texture;

            if(!texture_cache[texture_cache_index].loadFromFile(file_path)){
                printf("# Error loading file: %s", file_path);
                return EXIT_FAILURE;
            }

            S_ptr->setTexture(texture_cache[texture_cache_index]);
            texture_cache_index++;

            return EXIT_SUCCESS;
        }
    }

    void event_loop(){
	if(health <= 0){
	  pause = true;
	  
	}


        if(!pause){
            //Player


            app->draw(enemy_sprite);


            // Draw lines

            for(unsigned int item = 0; item < VA.size(); item++){
                app->draw(VA[item]);
            }
        }
    }


    void get_player_rotation(){

        if (!pause){
            float R = enemy_sprite.getRotation();


            double DeltaY;
            double DeltaX;
            double K = (R * PI) / 180;

            DeltaY = abs(enemyY - targetY);
            DeltaX = abs(enemyX - targetX);



            // Y = kx + m
            float Y = DeltaY*cos(K) - DeltaX*sin(K);
            float X = DeltaY*sin(K) + DeltaX*cos(K);

            VA.at(1)[0].position = sf::Vector2f(enemyX, enemyY);
            VA.at(1)[1].position = sf::Vector2f(X, Y);
        }
    }



    void move2target(){
        if (!pause){
            double DeltaX = abs(enemyX - targetX);
            double DeltaY = abs(enemyY - targetY);
            double dist = std::sqrt(DeltaX * DeltaX + DeltaY * DeltaY);
            int Sboost = 1;
            double noise = 1 / rand() % 10 + 1;
            if (rand() % 2 == 2){
                noise = 1.5f;
            }

            int far = rand() % 50 + 1 < 50 ? -200 : 200;

            if (dist < 250){
                Sboost = 10;
                noise = 1;

                if (enemyX > targetX + 1 * noise ){
                    update_enemy(noise * Sboost * -enemy_speed * time_df, 0);
                }else{
                    update_enemy(noise * Sboost * enemy_speed * time_df, 0);
                }if (enemyY > targetY + 1 * noise){
                    update_enemy(0, noise * Sboost * -enemy_speed * time_df);
                }else{
                    update_enemy(0, noise * Sboost * enemy_speed * time_df);
                }


            }else{
                if (enemyX > targetX + far){
                    update_enemy(noise * Sboost * -enemy_speed * time_df, 0);
                }else{
                    update_enemy(noise * Sboost * enemy_speed * time_df, 0);
                }if (enemyY > targetY + far){
                    update_enemy(0, noise * Sboost * -enemy_speed * time_df);
                }else{
                    update_enemy(0, noise * Sboost * enemy_speed * time_df);
                }


            }



        }


    }

    // Set the rotation and location of the player

    void update_enemy(int x, int y){
        enemyX += x;
        enemyY += y;
    }

    void move_enemy(){
        if (!pause){
            enemy_sprite.setPosition(sf::Vector2f(round(enemyX), round(enemyY)));

            double DeltaY;
            double DeltaX;

            targetX = target->getPosition().x;
            targetY = target->getPosition().y;
            DeltaY = enemyY - targetY;
            DeltaX = enemyX - targetX;

            double R = atan(DeltaY/DeltaX) * (180 / PI);

            if(DeltaX < 0)
                R += 180;
            R += 180;

            //printf("%f\n",R);


            //grass_sprite.setRotation(0);
            enemy_sprite.setRotation(R);
            //get_player_rotation();
            move2target();
        }
    }



    int width, height;

    void init(sf::RenderWindow &W, int Window_X, int Window_Y, int SpawnX, int SpawnY){
        app = &W;
        srand(time(NULL));

        width  = Window_X;
        height = Window_Y;

        enemyX = SpawnX;
        enemyY = SpawnY;


        printf("# Enemy spawned!\n");

        //Load enemy textures
        load_sprite(enemy_sprite, enemy_filepath);

        enemy_sprite.setOrigin(enemy_sprite.getTexture()->getSize().x / 2,
                               enemy_sprite.getTexture()->getSize().y / 2);

        enemy_sprite.setRotation(90);
        enemy_sprite.setPosition(sf::Vector2f(round(enemyX), round(enemyY)));

        printf("");
    }

    void setTarget(sf::Sprite &t){
        target = &t;
    }
    void time_update(int time_diffrence){
        time_df = time_diffrence;
    }
    short int enemyX, enemyY = 0;
private:
    // Store the textures
    sf::Texture texture_cache[256];
    short int texture_cache_index = 0;

    int time_df;

    sf::Sprite *target;

    sf::RenderWindow *app;

    // Position variables

    short int targetX, targetY = 0;

    // Player sprite
    sf::Sprite enemy_sprite;
    std::string enemy_filepath = "images/monster.bmp";

    // Store the line
    std::vector <sf::VertexArray> VA;

    // player Speed
    float  enemy_speed = 0.0000001;

};

#endif // ENEMY_CLASS_H_INCLUDED
