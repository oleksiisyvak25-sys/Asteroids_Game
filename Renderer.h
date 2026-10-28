#ifndef ASTEROIDS_GAME_RENDERER_H
#define ASTEROIDS_GAME_RENDERER_H

#include "GameObserver.h"
#include "GameObject.h"
#include "ScoreManager.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

class Renderer : public GameObserver {
private:
    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded;
    bool logEvents;
    std::vector<char> pressedKeys;
    std::shared_ptr<const ScoreManager> scores;
    int currentWave;

    bool loadFont();
    void drawText(const std::string& text, float x, float y, unsigned int size,
                  const sf::Color& color, bool centered);
    void drawOverlay(const std::string& title, const std::vector<std::string>& lines);

public:
    explicit Renderer(std::shared_ptr<const ScoreManager> scoreManager, bool logToConsole = true);

    bool isOpen() const;
    void pollEvents();
    std::vector<char> takePressedKeys();
    bool isKeyHeld(char key) const;

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
