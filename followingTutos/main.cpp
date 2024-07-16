#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <math.h>
#include <cstdlib>

class Bullet 
{
    public : 
        sf::Sprite shape;
        sf::Vector2f posBullet;

        Bullet(sf::Texture *texture, sf::Vector2f pos) 
        {
            this->shape.setTexture(*texture);
            this->shape.setScale(0.05f, 0.05f);
            this->shape.setPosition(pos.x + 97, pos.y);

        }

        ~Bullet() {}
};

class Player 
{
    public :
        sf::Sprite shape;
        sf::Texture *texture;
        int HP;
        int HPMax;
        int score;

        std::vector<Bullet> bullets;

        Player(sf::Texture *texture) 
        {
            this->HPMax = 10;
            this->HP = this->HPMax;
            this->score = 0;

            this->texture = texture;
            this->shape.setTexture(*texture);
        }

        ~Player() {}
};

class Enemy
{
    public: 
        int HP;
        int HPMax;

        sf::Texture *texture;
        sf::Sprite shape;

        Enemy(sf::Texture *texture, sf::Vector2u windowSize)
        {
            this->HPMax = 1;
            this->HP = this->HPMax;

            this->shape.setTexture(*texture);
            this->shape.setScale(7.f, 7.f);
            this->shape.setPosition(rand() %windowSize.x - this->shape.getGlobalBounds().width +100, -100);
        }   

        ~Enemy(){}
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1366, 720), "Savio Shooters");
    int moveVariation = 10, shootTimer = 20, enemySpawnTimer = 0;

    srand(time(NULL));
    window.setFramerateLimit(60);

    //Setting up textures, fonts, sounds, etc
    sf::Font font;
    font.loadFromFile("./fonts/airstrike.ttf");

    sf::Texture spaceshipTexture;
    spaceshipTexture.loadFromFile("./sprites/space.png");

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("./sprites/bullet.png");

    sf::Texture enemyTexture;
    enemyTexture.loadFromFile("./sprites/cat.png");

    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("./images/space.jpg");

    //Texts
    sf::Text hpText;
    hpText.setFont(font);
    hpText.setCharacterSize(20);
    hpText.setFillColor(sf::Color::White);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::White);

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Red);

    sf::Text restartText;
    restartText.setFont(font);
    restartText.setCharacterSize(40);
    restartText.setFillColor(sf::Color::Red);

    //Main player initializer
    Player mPlayer(&spaceshipTexture);
    mPlayer.shape.setPosition(300.f, 300.f);

    //Enemies initializer
    std::vector<Enemy> enemies;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        if(mPlayer.HP <= 0)
        {
            gameOverText.setPosition(window.getSize().x / 3, window.getSize().y / 3);
            restartText.setPosition(window.getSize().x / 3, window.getSize().y / 3 + 50);
            restartText.setString("RESTART THE GAME?");
            gameOverText.setString("GAME OVER");
        }else {
            //Timers 
            if(shootTimer < 20) 
            {
                shootTimer++;
            }
            if(enemySpawnTimer < 15)
            {
                enemySpawnTimer++;
            }
            if(enemySpawnTimer >= 15) 
            {
                enemies.push_back(Enemy(&enemyTexture, window.getSize()));  
                enemySpawnTimer = 0;
            }

            //Player Controls
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && shootTimer >= 20)
            {
                mPlayer.bullets.push_back(Bullet(&bulletTexture, mPlayer.shape.getPosition()));
                shootTimer = 0;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                mPlayer.shape.move(0.f, -moveVariation);
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) 
            {
                mPlayer.shape.move(0.f, moveVariation);
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
            {
                mPlayer.shape.move(-moveVariation, 0.f);
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
            {
                mPlayer.shape.move(moveVariation, 0.f);
            }

            //Hp text configuration
            hpText.setPosition(mPlayer.shape.getPosition().x, mPlayer.shape.getPosition().y - hpText.getGlobalBounds().height);
            hpText.setString(std::to_string(mPlayer.HP) + "/" + std::to_string(mPlayer.HPMax));

            //Score text configuration
            scoreText.setPosition(0, window.getSize().y - 70);
            scoreText.setString("SCORE : " + std::to_string(mPlayer.score));

            //Game over message


            //Colision with window
            if(mPlayer.shape.getPosition().x <= 0) 
            {
                mPlayer.shape.setPosition(0.f, mPlayer.shape.getPosition().y);
            }
            if(mPlayer.shape.getPosition().x >= window.getSize().x - mPlayer.shape.getGlobalBounds().width) 
            {
                mPlayer.shape.setPosition(window.getSize().x - mPlayer.shape.getGlobalBounds().width, mPlayer.shape.getPosition().y);
            }
            if(mPlayer.shape.getPosition().y <= 0) 
            {
                mPlayer.shape.setPosition(mPlayer.shape.getPosition().x, 0.f);
            }
            if(mPlayer.shape.getPosition().y >= window.getSize().y - mPlayer.shape.getGlobalBounds().height)
            {
                mPlayer.shape.setPosition(mPlayer.shape.getPosition().x, window.getSize().y - mPlayer.shape.getGlobalBounds().height);
            }

            //background image configuration
            sf::Sprite bgSprite;
            bgSprite.setTexture(backgroundTexture);
            bgSprite.setScale(0.8f,0.8f);
            

            window.clear();
            window.draw(bgSprite);

            window.draw(mPlayer.shape);

            //Bullets
            for(size_t i = 0;i < mPlayer.bullets.size();i++) 
            {
                mPlayer.bullets[i].shape.move(0.f, -20.f);
                if(mPlayer.bullets[i].shape.getPosition().x > window.getSize().x) 
                {
                    mPlayer.bullets.erase(mPlayer.bullets.begin() + i);
                    break;
                }
                else if(mPlayer.bullets[i].shape.getPosition().y > window.getSize().y) 
                {
                    mPlayer.bullets.erase(mPlayer.bullets.begin() + i);
                    break;
                }
                for(size_t j = 0;j < enemies.size();j++) 
                {
                    if(mPlayer.bullets[i].shape.getGlobalBounds().intersects(enemies[j].shape.getGlobalBounds()))
                    {
                        if(enemies[j].HP <= 0) 
                        {
                            enemies.erase(enemies.begin() + j);
                            mPlayer.score += 5;

                        }else 
                        {
                            enemies[j].HP -= 10;
                        }
                        mPlayer.bullets.erase(mPlayer.bullets.begin() + i);
                        break;
                    }
                }
            }
            //Shooting bullets animation
            for(size_t i = 0;i < mPlayer.bullets.size();i++) 
            {
                window.draw(mPlayer.bullets[i].shape);
            }

            //Enemies animation
            for(size_t i = 0;i < enemies.size();i++) 
            {
                enemies[i].shape.move(0.f, +2.f);
        
                if(enemies[i].shape.getPosition().y >= window.getSize().y) 
                {
                    enemies.erase(enemies.begin() + i);
                    break;
                }
                if(enemies[i].shape.getGlobalBounds().intersects(mPlayer.shape.getGlobalBounds()))
                {
                    enemies.erase(enemies.begin() + i);
                    mPlayer.HP--;
                    break;
                }
            }
            //Spawning enemies
            for(size_t i = 0;i < enemies.size();i++) 
            {
                window.draw(enemies[i].shape);
            }
        }

        //UI
        window.draw(hpText);
        window.draw(restartText);
        window.draw(scoreText);
        window.draw(gameOverText);

        window.display();
    }

    return 0;
}