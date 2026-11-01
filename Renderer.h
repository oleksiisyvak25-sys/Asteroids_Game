#ifndef ASTEROIDS_GAME_RENDERER_H
#define ASTEROIDS_GAME_RENDERER_H

#include "GameObserver.h"
#include "GameObject.h"
#include "ScoreManager.h"
#include "Spaceship.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <string>
#include <vector>

class Renderer : public GameObserver {
private:
    struct Particle {
        Vector2 position;
        Vector2 velocity;
        float life;
        float maxLife;
        float size;
        sf::Color color;
    };

    struct Ring {
        Vector2 position;
        float maxRadius;
        float age;
        float duration;
        sf::Color color;
        bool filled;
    };

    struct Star {
        float x;
        float y;
        float size;
        float phase;
        float speed;
    };

    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded;
    bool logEvents;
    std::vector<char> pressedKeys;
    std::shared_ptr<const ScoreManager> scores;
    int currentWave;

    std::vector<Particle> particles;
    std::vector<Ring> rings;
    std::vector<Star> stars;
    float time;
    bool thrusting;
    std::mt19937 effectsRng;

    bool loadFont();
    float randomRange(float lo, float hi);
    void spawnBurst(const Vector2& position, int count, float speedMin, float speedMax,
                    const sf::Color& colorA, const sf::Color& colorB,
                    float sizeMin, float sizeMax, float lifeMin, float lifeMax);
    void spawnRing(const Vector2& position, float maxRadius, float duration,
                   const sf::Color& color, bool filled);
    void clearEffects();

    void drawBackground();
    void drawEffects();
    void drawHud(const Spaceship* ship, int bulletCount);
    void drawText(const std::string& text, float x, float y, unsigned int size,
                  const sf::Color& color, bool centered);
    void drawOverlay(const std::string& title, const std::vector<std::string>& lines);

public:
    explicit Renderer(std::shared_ptr<const ScoreManager> scoreManager, bool logToConsole = true);

    bool isOpen() const;
    void pollEvents();
    std::vector<char> takePressedKeys();
    bool isKeyHeld(char key) const;

    void update(float dt);
    void setShipThrusting(bool value);
    void beginFrame();
    void endFrame();

    void render(const std::vector<std::shared_ptr<GameObject>>& objects);
    void showMenu();
    void showPaused();
    void showGameOver();
    void showVictory();

    void onEvent(const GameEvent& event) override;
};

#endif //ASTEROIDS_GAME_RENDERER_H
