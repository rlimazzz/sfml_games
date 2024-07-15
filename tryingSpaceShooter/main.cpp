#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <bits/stdc++.h>
#include <math.h>
#include <cstdlib>
#include <stdlib.h>

class Game 
{
  public:
    Game() : mWindow(sf::VideoMode(1366,780), "SFML APPLICATION"), mPlayer() , mTexture(), shootSound(), shootBuffer(), mFont() {
      if(!shootBuffer.loadFromFile("./sounds/laser.wav")) {
        printf("Error with sound laser!\n");
      }
      if(!mTexture.loadFromFile("./sprites/mCharacter.png")) {
        printf("Error with sprite\n");
      }

      shootSound.setBuffer(shootBuffer);
      shootSound.setVolume(50.f);
      shootSound.setPlayingOffset(sf::seconds(5.f));
      shootSound.play();
      mPlayer.setTexture(mTexture);
      mPlayer.setScale(16, 16);
    }

    void run() 
    {
      sf::Clock clock;
      sf::Time timePerFrame = sf::seconds(1.f / 60.f);
      sf::Time timeSinceLastUpdate = sf::Time::Zero;
      while(mWindow.isOpen()) {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        
        while(timeSinceLastUpdate > timePerFrame) {
          timeSinceLastUpdate -= timePerFrame;
          processEvents();  
          update(timePerFrame);
        }
        render();
      }
    }
  
  private:

    void processEvents()
    {
      sf::Event event;

      while(mWindow.pollEvent(event))
      {

        switch(event.type) 
        { 
          case sf::Event::KeyPressed : 
            handlePlayerInput(event.key.code, true);
            break;
          
          case sf::Event::KeyReleased : 
            handlePlayerInput(event.key.code, false);
            break;

          case sf::Event::Closed : 
            mWindow.close();
            break;
        }
        
      }
    }

    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
      if(key == sf::Keyboard::W) mIsMovingUp = isPressed;
      else if(key == sf::Keyboard::S) mIsMovingDown = isPressed;
      else if(key == sf::Keyboard::A) mIsMovingLeft = isPressed;
      else if(key == sf::Keyboard::D) 
      {
        mIsMovingRight = isPressed;
      }
      else if(key == sf::Keyboard::Enter) mIsShooting = isPressed;
    }


    void update(sf::Time deltaTime) 
    {
      sf::Vector2f movement(0.f, 0.f);

      if(mIsMovingUp) 
      {
        movement.y -= movementVariation;
        mIsMovingUp = false;
      }
      else if(mIsMovingDown) 
      {
        movement.y += movementVariation;
        mIsMovingDown = false;
      }
      else if(mIsMovingLeft) 
      {
        movement.x -= movementVariation;
        mIsMovingLeft = false;
      }
      else if(mIsMovingRight) 
      {
        movement.x += movementVariation;
        mIsMovingRight = false;
      }
      else if(mIsShooting) {
        shootSound.play();
      }

      mPlayer.move(movement * deltaTime.asSeconds());
    }

    void render()
    {
      mWindow.clear();
      mWindow.draw(mPlayer);
      mWindow.display();
    }
  
  private: 
    sf::RenderWindow mWindow;
    sf::Texture mTexture;
    sf::Sprite mPlayer;
    sf::SoundBuffer shootBuffer;
    sf::Sound shootSound;
    sf::Font mFont;

    bool mIsMovingRight, mIsMovingLeft, mIsMovingDown, mIsMovingUp, mIsShooting;
    double movementVariation = 5000.f;

};

int main()
{
  Game game;
  game.run();

  return 0;
}
