#ifndef PLAYER_CLASS_H_INCLUDED
#define PLAYER_CLASS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "Enemy_class.h"


#define PI 3.14159265
#define title "topdown_shooter"



class  Player{
/*
    TODO:
        GUI:
            Healthbar
            Ammostatus
            quotes
            DOOM like style

        GUN:
            fix weapon
            ammo
            bullet
            bullet hit
            animations

        MOVEMENT:
            Out of bounds
            walk into object
            speed
            animations



    When stamina is low the gun should be less accurate!


*/


public:
    bool pause = false;
    bool gameover = false;
    bool isShift = false;
    // Variables
    int stamina;
    int health = 100;


    // Red flash
    sf::RectangleShape R;

    //Player sprites
    sf::Sprite player_sprite;

    bool draw_lines = true;
    // Memorable quotes
    std::string quotes[10] = {
        "Kick gum, eat a**",
        "attac, n defen",
        "get down",
        "i k*ll u",
        "im gonna f*cking f*ck your f*cking a** up you f*cking f*cktard",
        "if it p*ops I can kill it",
        "let off some steam",
        "hasta la vista p*opt*rd",
        "you shoot like a vegan",
        "imma kick u in the b*ll"
    };

    // Ammo
    short int clip_amount = 10;
    short int clip_max_amount = 12;
    short int clips[10] = {
            clip_max_amount,
            clip_max_amount,
            clip_max_amount,
            clip_max_amount,
            clip_max_amount,
            clip_max_amount,
            clip_max_amount,
            clip_max_amount,
            clip_max_amount,
            clip_max_amount};
    short int equipped_clip = 0;

    // Timers
    std::chrono::_V2::steady_clock::time_point shoot_timer  = std::chrono::steady_clock::now();
    std::chrono::_V2::steady_clock::time_point reload_timer = std::chrono::steady_clock::now();
    std::chrono::_V2::steady_clock::time_point flash_timer  = std::chrono::steady_clock::now();


    // Bullet holes
    sf::RectangleShape Red;
    std::vector <sf::Vector2f> bullet_holes;

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
                printf("Error loading file: %s", file_path);
                return EXIT_FAILURE;
            }

            S_ptr->setTexture(texture_cache[texture_cache_index]);
            texture_cache_index++;

            return EXIT_SUCCESS;
        }
    }
    sf::Font font;

    sf::Text draw_text(std::string text, int size_, int x, int y, int r, int g, int b){

        sf::Text txt;
        txt.setFont(font);
        txt.setString(text);
        txt.setCharacterSize(size_);
        txt.setPosition(x, y);
        txt.setColor(sf::Color(r,g,b));

        return txt;
    }
    void event_loop(){

        if(health <= 0 && !pause && !gameover){
            std::cout << "GAME OVER" << std::endl;
            pause = true;
            gameover = true;
            for (unsigned int i = 0; i < Enemy_list.size(); i++){
                Enemy_list.at(i)->pause = true;
            }
        }
        if (clip_amount - equipped_clip == 0)
            clips[equipped_clip] = 0;



        app->draw(snow);
        // Draw lines


        // Time to display shot trace
        if ((std::chrono::steady_clock::now() - flash_timer).count() < 10000000){
            for(unsigned int item = 0; item < VA.size(); item++){
                    app->draw(VA[item]);
            }



        }if (((std::chrono::steady_clock::now() - flash_timer).count() < 20000000) &&
             ((std::chrono::steady_clock::now() - flash_timer).count() > 5000000)){

             app->draw(Red);


        }



        for (unsigned int i = 0; i < bullet_holes.size(); i++){
            R.setPosition(bullet_holes.at(i));
            R.setSize(sf::Vector2f(3, 3));
            R.setFillColor(sf::Color(00,00,00));
            app->draw(R);
        }
        if (clips[equipped_clip] == 0){
            app->draw(draw_text("Reload!", 24, (width / 2) + 50, 2, 00, 255, 00));
        }
        app->draw(draw_text("AMMO=" + std::to_string(clips[equipped_clip]), 24, 300, 2, 00, 255, 00));
        app->draw(draw_text("CLIPS=" + std::to_string(clip_amount - equipped_clip), 24, 100, 2, 00, 255, 00));

        if (health > 50)
            app->draw(draw_text(std::to_string(health), 24, 2, 2, 00, 255, 00));
        else
            app->draw(draw_text(std::to_string(health), 36, 2, 2, 255, 00, 00));



        //Player
        app->draw(player_sprite);
        if (pause && gameover)
            app->draw(draw_text("--> YOU <-- suck, GAME OVER!",50, 50, height / 2, 255, 00, 00));

    }

    int width, height;

    Player(sf::RenderWindow &W, int Window_X, int Window_Y){
        printf("Initiating player class...");
        srand(time(NULL));
        app = &W;

        width  = Window_X;
        height = Window_Y;

        // Load font
        if (!font.loadFromFile("fonts/Capture_it.ttf")){
            std::cout << "FAILED TO LOAD FONT" << std::endl;
        }

        Red.setFillColor(sf::Color(255, 00, 00, 60));
        Red.setSize(sf::Vector2f(width, height));



        // Load player sprite
        load_sprite(player_sprite, player_filepath);
        player_bounds_x = player_sprite.getTexture()->getSize().x;
        player_bounds_y = player_sprite.getTexture()->getSize().y;
        player_sprite.setOrigin(player_bounds_x / 2,
                                player_bounds_y / 2);
        player_bounds_x = 200;
        player_bounds_y = 200;
        // Player spawn
        playerX = width / 2;
        playerY = height - 50;

        load_sprite(snow, snow_filepath);
        //snow.setPosition(sf::Vector2f(background_pos[0], background_pos[1]));
        snow.setOrigin(snow.getTexture()->getSize().x / 2,
                       snow.getTexture()->getSize().y / 2);

        load_sprite(grass_sprite, grass_filepath);
        grass_sprite.setScale(sf::Vector2f(3, 3));

        if (draw_lines){
            sf::VertexArray player_line(sf::Lines, 2);
            sf::VertexArray rotation_line(sf::Lines, 2);
            VA.push_back(player_line);
            VA.push_back(rotation_line);
        }
        printf("\tDone!\n\n");
    }
    void shoot_f(bool steady = false){

        int DeltaX, DeltaY, dist;

        int hit_size = 20; // px

        int bullet_off = 120;
        if (steady)
            bullet_off = bullet_off / 2;



        if(clips[equipped_clip] > 0 && equipped_clip != 10){
            clips[equipped_clip]--;

            int x = sf::Mouse::getPosition(*app).x - (bullet_off / 2) + rand() % bullet_off;
            int y = sf::Mouse::getPosition(*app).y - (bullet_off / 2) + rand() % bullet_off;

            last_shotX = x;
            last_shotY = y;
            bullet_holes.push_back(sf::Vector2f(x, y));

            flash_timer = std::chrono::steady_clock::now();


            for (unsigned int i = 0; i < Enemy_list.size(); i++){
                DeltaY = abs(Enemy_list.at(i)->enemyY - y);
                DeltaX = abs(Enemy_list.at(i)->enemyX - x);

                dist = std::sqrt(DeltaY * DeltaY + DeltaX * DeltaX);

                if (dist <= hit_size)
                    Enemy_list.at(i)->dead = true;
            }
        }
    }

    void reload(){
        if (equipped_clip < clip_amount ){
            clips[equipped_clip] = 0;
            equipped_clip++;
        }
    }

    // Get keyboard input
    void input(){
        if (!pause){
            isShift = false;
            int sped = 1;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                isShift = true;
            sped = isShift ? 2 : 1;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                update_player_pos(0, sped * -1 * player_speed * time_df);

            }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                update_player_pos(0, sped * player_speed * time_df);

            }if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                update_player_pos(sped * -1 * player_speed * time_df, 0);

            }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                update_player_pos(sped * player_speed * time_df, 0);

            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
                if ( (std::chrono::steady_clock::now() - shoot_timer).count() > 200000000){
                    shoot_f(true);
                    shoot_timer = std::chrono::steady_clock::now();
                }
            }else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                if ( (std::chrono::steady_clock::now() - shoot_timer).count() > 200000000){
                    shoot_f();
                    shoot_timer = std::chrono::steady_clock::now();
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
                if ( (std::chrono::steady_clock::now() - reload_timer).count() > 400000000){
                    reload();
                    reload_timer = std::chrono::steady_clock::now();
                }
            }
            if (pause && gameover && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
                app->close();
            }


            move_player();

        }
    }

    // Get mouse position
    void update_cord(){
        if (!pause){
            sf::Vector2i pos = sf::Mouse::getPosition(*app);
            mouseX = pos.x;
            mouseY = pos.y;
            set_line();
        }
    }
    void time_update(int time_diffrence){
        time_df = time_diffrence;
    }

    void update_enemy_pos(int x, int y){


        for (unsigned int i = 0; i < Enemy_list.size(); i++){
            Enemy_list.at(i)->update_enemy(x, y);

        }
        for (unsigned int i = 0; i < bullet_holes.size(); i++){
            bullet_holes.at(i).x += x;
            bullet_holes.at(i).y += y;
        }

    }
    void enemy_attack(){
        if (!pause){
            int DeltaX, DeltaY, dist;
            for (unsigned int i = 0; i < Enemy_list.size(); i++){
                if (!Enemy_list.at(i)->dead){
                    DeltaY = abs(Enemy_list.at(i)->enemyY - playerY);
                    DeltaX = abs(Enemy_list.at(i)->enemyX - playerX);

                    dist = std::sqrt(DeltaY * DeltaY + DeltaX * DeltaX);

                    if (dist < enemy_attack_dist)
                        health += enemy_attack_dmg;
                }
            }
        }
    }

    void add_enemy(Enemy &instance){
        Enemy *I = &instance;
        Enemy_list.push_back(I);
    }
private:

    int enemy_attack_dmg = -5;
    int enemy_attack_dist = 20;
    std::vector<Enemy*> Enemy_list;

    int time_df;

    // Store the textures
    sf::Texture texture_cache[256];
    short int texture_cache_index = 0;

    sf::RenderWindow *app;

    // Move the player
    void update_player_pos(int x, int y){
        playerX += x;
        playerY += y;
        move_player();
    }

    // Absolute value
    int abs(double n){
        if (n >= 0){
            return n;
        }return -n;
    }

    void get_player_rotation(){
        float R = player_sprite.getRotation();


        double DeltaY;
        double DeltaX;
        double K = (R * PI) / 180;

        DeltaY = abs(playerY - mouseY);
        DeltaX = abs(playerX - mouseX);



        // Y = kx + m
        float Y = DeltaY*cos(K) - DeltaX*sin(K);
        float X = DeltaY*sin(K) + DeltaX*cos(K);

        VA.at(1)[0].position = sf::Vector2f(playerX, playerY);
        VA.at(1)[1].position = sf::Vector2f(X, Y);

    }
    // Set the rotation and location of the player

    void update_background(int x, int y){
        background_pos[0] += x;
        background_pos[1] += y;
        update_enemy_pos(x, y);
        move_background();

    }
    void move_background(){
        snow.setPosition(sf::Vector2f(round(background_pos[0]), round(background_pos[1])));
    }
    void move_player(){
        check_player();
        player_sprite.setPosition(sf::Vector2f(round(playerX), round(playerY)));

        double DeltaY;
        double DeltaX;

        DeltaY = playerY - mouseY;
        DeltaX = playerX - mouseX;



        double R = atan(DeltaY/DeltaX) * (180 / PI);

        if(DeltaX < 0)
            R += 180;
        R += 180;

        //printf("%f\n",R);


        //grass_sprite.setRotation(0);
        player_sprite.setRotation(R);
        //get_player_rotation();
    }


    //out of bounds
    unsigned int player_bounds_x;
    unsigned int player_bounds_y;

    void check_player(){
        int x = background_pos[0];
        int y = background_pos[1];

        int sizex = snow.getTexture()->getSize().x / 2;
        int sizey = snow.getTexture()->getSize().y / 2;

        // player moves right
        if (playerX + player_bounds_x > width){
            playerX = width - player_bounds_x;

            if (x - width > -sizex){
                update_background(-1 * player_speed * time_df, 0);
            }else{
                background_pos[0] = -sizex + width;
            }
        }
        // player moves left
        else if (playerX < player_bounds_x){
            playerX = player_bounds_x;
            if (x < sizex){
                update_background(player_speed * time_df, 0);
            }else{
                background_pos[0] = sizex;
            }
        }
        // player moves down
        if (playerY + player_bounds_y > height){
            playerY = height - player_bounds_y;
            if (y - height > -sizey){
                update_background(0, -1 * player_speed * time_df);
            }else{
                background_pos[1] = -sizey + height;
            }
        }
        // Player moves up
        else if(playerY < player_bounds_y){
            playerY = player_bounds_y;
            if (y < sizey){
                update_background(0, player_speed * time_df);
            }else{
                background_pos[1] = sizey;
            }
        }
    }

    // Sight line between player and mouse
    void set_line(){

        VA.at(0)[0].position = sf::Vector2f(playerX, playerY);
        VA.at(0)[1].position = sf::Vector2f(last_shotX, last_shotY);
        VA.at(0)[0].color = sf::Color(255, 255, 00);
        VA.at(0)[1].color = sf::Color(255, 00, 00);
    }


    // Position variables
    short int playerX, playerY = 0;
    short int mouseX, mouseY = 0;
    short int last_shotX, last_shotY = 0;
    // Player sprite

    std::string player_filepath = "images/player_CAMO.png";

    // Store the line
    std::vector <sf::VertexArray> VA;

    // Background grass sprite

    sf::Sprite snow;
    std::string snow_filepath = "images/dark_snow.png";
    double background_pos[2] = {0, 0};
    std::string grass_filepath = "images/grass.bmp";
    sf::Sprite grass_sprite;

    // player Speed
    float  player_speed = 0.0000004;
};

#endif // PLAYER_CLASS_H_INCLUDED
