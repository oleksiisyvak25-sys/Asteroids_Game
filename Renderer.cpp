#include "Renderer.h"
#include "Asteroid.h"
#include "Spaceship.h"
#include "Config.h"
#include <SFML/Config.hpp>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#if SFML_VERSION_MAJOR >= 3
#include <optional>
#endif

namespace {
    const sf::Color BACKGROUND(8, 8, 20);
    const sf::Color SHIP_COLOR(240, 240, 240);
    const sf::Color SHIELD_COLOR(80, 200, 255);
    const sf::Color PROJECTILE_COLOR(255, 230, 90);
    const sf::Color BONUS_COLOR(80, 230, 120);
    const sf::Color TEXT_COLOR(230, 230, 230);

    sf::Color asteroidColor(AsteroidType kind) {
        switch (kind) {
            case AsteroidType::Explosive: return sf::Color(220, 80, 50);
            case AsteroidType::Gravity:   return sf::Color(150, 100, 230);
            default:                      return sf::Color(150, 150, 160);
        }
    }

    char keyToChar(sf::Keyboard::Key key) {
        switch (key) {
            case sf::Keyboard::Key::P: return 'p';
            case sf::Keyboard::Key::Q: return 'q';
            case sf::Keyboard::Key::S: return 's';
            case sf::Keyboard::Key::R: return 'r';
            case sf::Keyboard::Key::M: return 'm';
            case sf::Keyboard::Key::C: return 'c';
            default:                   return '\0';
        }
    }

    void drawCircle(sf::RenderTarget& target, const Vector2& center, float radius,
                    const sf::Color& fill, const sf::Color& outline = sf::Color::Transparent,
                    float outlineThickness = 0.0f) {
        const std::size_t points = static_cast<std::size_t>(std::max(12.0f, radius * 1.5f));
        sf::CircleShape shape(radius, points);
        shape.setOrigin(sf::Vector2f(radius, radius));
        shape.setPosition(sf::Vector2f(center.x, center.y));
        shape.setFillColor(fill);
        shape.setOutlineColor(outline);
        shape.setOutlineThickness(outlineThickness);
        target.draw(shape);
    }

    void drawShip(sf::RenderTarget& target, const Spaceship& ship) {
        const Vector2& p = ship.getPosition();
        const float r = ship.getRadius();
        const float a = ship.getAngle();

        const Vector2 pts[3] = {
            p + Vector2::fromAngle(a) * r,
            p + Vector2::fromAngle(a + 140.0f) * r,
            p + Vector2::fromAngle(a - 140.0f) * r
        };

        sf::ConvexShape tri;
        tri.setPointCount(3);
        for (std::size_t i = 0; i < 3; ++i) {
            tri.setPoint(i, sf::Vector2f(pts[i].x, pts[i].y));
        }
        tri.setFillColor(SHIP_COLOR);
        target.draw(tri);

        if (ship.isShielded()) {
            drawCircle(target, p, r + 6.0f, sf::Color::Transparent, SHIELD_COLOR, 2.0f);
        }
    }
}

Renderer::Renderer(std::shared_ptr<const ScoreManager> scoreManager, bool logToConsole)
    : fontLoaded(false), logEvents(logToConsole), scores(std::move(scoreManager)), currentWave(1) {
    const unsigned int w = static_cast<unsigned int>(Config::WORLD_WIDTH);
    const unsigned int h = static_cast<unsigned int>(Config::WORLD_HEIGHT);
#if SFML_VERSION_MAJOR >= 3
    window.create(sf::VideoMode({w, h}), "Asteroids", sf::Style::Titlebar | sf::Style::Close);
#else
    window.create(sf::VideoMode(w, h), "Asteroids", sf::Style::Titlebar | sf::Style::Close);
#endif
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    fontLoaded = loadFont();
    if (!fontLoaded) {
        std::cout << "[Renderer] Font not found, HUD is shown in the window title. "
                     "Put any font.ttf next to the executable.\n";
    }
}

bool Renderer::loadFont() {
    static const char* candidates[] = {
        "font.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf",
        "/usr/share/fonts/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/usr/share/fonts/liberation/LiberationSans-Regular.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/Library/Fonts/Arial.ttf"
    };
    for (const char* path : candidates) {
#if SFML_VERSION_MAJOR >= 3
        if (font.openFromFile(path)) return true;
#else
        if (font.loadFromFile(path)) return true;
#endif
    }
    return false;
}

bool Renderer::isOpen() const { return window.isOpen(); }

void Renderer::pollEvents() {
#if SFML_VERSION_MAJOR >= 3
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            const char c = keyToChar(key->code);
            if (c != '\0') pressedKeys.push_back(c);
        }
    }
#else
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            const char c = keyToChar(event.key.code);
            if (c != '\0') pressedKeys.push_back(c);
        }
    }
#endif
}

std::vector<char> Renderer::takePressedKeys() {
    std::vector<char> keys;
    keys.swap(pressedKeys);
    return keys;
}

bool Renderer::isKeyHeld(char key) const {
    if (!window.hasFocus()) {
        return false;
    }
    using K = sf::Keyboard::Key;
    auto down = [](K k) { return sf::Keyboard::isKeyPressed(k); };
    switch (key) {
        case 'w': return down(K::W) || down(K::Up);
        case 'a': return down(K::A) || down(K::Left);
        case 'd': return down(K::D) || down(K::Right);
        case ' ': return down(K::Space) || down(K::F);
        default:  return false;
    }
}

void Renderer::beginFrame() { window.clear(BACKGROUND); }
void Renderer::endFrame()   { window.display(); }

void Renderer::drawText(const std::string& text, float x, float y, unsigned int size,
                        const sf::Color& color, bool centered) {
    if (!fontLoaded) {
        return;
    }
#if SFML_VERSION_MAJOR >= 3
    sf::Text t(font, text, size);
#else
    sf::Text t(text, font, size);
#endif
    t.setFillColor(color);
    if (centered) {
        const sf::FloatRect b = t.getLocalBounds();
#if SFML_VERSION_MAJOR >= 3
        t.setOrigin(sf::Vector2f(b.position.x + b.size.x / 2.0f, 0.0f));
#else
        t.setOrigin(sf::Vector2f(b.left + b.width / 2.0f, 0.0f));
#endif
    }
    t.setPosition(sf::Vector2f(x, y));
    window.draw(t);
}

void Renderer::drawOverlay(const std::string& title, const std::vector<std::string>& lines) {
    sf::RectangleShape dim(sf::Vector2f(Config::WORLD_WIDTH, Config::WORLD_HEIGHT));
    dim.setFillColor(sf::Color(0, 0, 0, 170));
    window.draw(dim);

    const float cx = Config::WORLD_WIDTH / 2.0f;
    drawText(title, cx, 170.0f, 48, TEXT_COLOR, true);
    float y = 260.0f;
    for (const auto& line : lines) {
        drawText(line, cx, y, 20, TEXT_COLOR, true);
        y += 32.0f;
    }
}

void Renderer::render(const std::vector<std::shared_ptr<GameObject>>& objects) {
    const Spaceship* ship = nullptr;

    for (const auto& obj : objects) {
        if (!obj->isActive()) continue;
        if (obj->getType() == ObjectType::Asteroid) {
            const Asteroid* a = static_cast<const Asteroid*>(obj.get());
            drawCircle(window, a->getPosition(), a->getRadius(), asteroidColor(a->getKind()));
        }
    }
    for (const auto& obj : objects) {
        if (obj->isActive() && obj->getType() == ObjectType::Bonus) {
            drawCircle(window, obj->getPosition(), obj->getRadius(), BONUS_COLOR);
        }
    }
    for (const auto& obj : objects) {
        if (obj->isActive() && obj->getType() == ObjectType::Projectile) {
            drawCircle(window, obj->getPosition(), obj->getRadius(), PROJECTILE_COLOR);
        }
    }
    for (const auto& obj : objects) {
        if (obj->getType() == ObjectType::Ship) {
            ship = static_cast<const Spaceship*>(obj.get());
        }
    }
    if (ship) {
        drawShip(window, *ship);
    }

    std::ostringstream line1;
    line1 << "Score: " << scores->getScore() << "   Best: " << scores->getHighScore()
          << "   Wave: " << currentWave;
    std::ostringstream line2;
    if (ship) {
        line2 << "HP: " << ship->getHealth() << "/" << Config::SHIP_START_HEALTH
              << "   speed: " << static_cast<int>(std::lround(ship->getVelocity().length())) << " px/s";
        if (ship->isShielded()) {
            line2 << "   SHIELD: " << std::fixed << std::setprecision(1) << ship->getShieldTime() << "s";
        }
    }
    drawText(line1.str(), 10.0f, 8.0f, 16, TEXT_COLOR, false);
    drawText(line2.str(), 10.0f, 30.0f, 16, TEXT_COLOR, false);
    if (!fontLoaded) {
        window.setTitle("Asteroids | " + line1.str() + " | " + line2.str());
    }
}

void Renderer::showMenu() {
    drawOverlay("ASTEROIDS", {
        "Best score: " + std::to_string(scores->getHighScore()),
        "",
        "[S] start game      [Q] quit",
        "",
        "W / Up - thrust     A,D / Left,Right - rotate",
        "Space or F - shoot  P - pause"
    });
}

void Renderer::showPaused() {
    drawOverlay("PAUSED", {"[R] or [P] resume      [M] main menu      [Q] quit"});
}

void Renderer::showGameOver() {
    drawOverlay("GAME OVER", {
        "Final score: " + std::to_string(scores->getScore()) +
            "     Best: " + std::to_string(scores->getHighScore()),
        "",
        "[M] main menu      [Q] quit"
    });
}

void Renderer::showVictory() {
    drawOverlay("VICTORY!", {
        "Score: " + std::to_string(scores->getScore()) +
            "     Best: " + std::to_string(scores->getHighScore()),
        "",
        "[C] continue in endless mode",
        "[M] main menu      [Q] quit"
    });
}

void Renderer::onEvent(const GameEvent& event) {
    if (event.type == EventType::WaveStarted) {
        currentWave = event.value;
    }
    if (!logEvents) {
        return;
    }
    switch (event.type) {
        case EventType::GameStarted:      std::cout << "=== New game started ===\n"; break;
        case EventType::WaveStarted:      std::cout << ">>> WAVE " << event.value << " incoming!\n"; break;
        case EventType::WaveCleared:      std::cout << ">>> Wave " << event.value << " cleared!\n"; break;
        case EventType::AsteroidDestroyed:
            std::cout << "[BOOM] Asteroid (size " << event.value << ") destroyed, +"
                      << ScoreManager::pointsForSize(event.value) << " points\n";
            break;
        case EventType::Explosion:        std::cout << "[EXPLOSION] Blast radius " << event.value << " px!\n"; break;
        case EventType::BonusDropped:     std::cout << "[BONUS] A shield bonus dropped\n"; break;
        case EventType::ShieldCollected:  std::cout << "[SHIELD] Collected! Invulnerable for " << event.value << " s\n"; break;
        case EventType::ShipHit:          std::cout << "[HIT] Ship damaged! HP left: " << event.value << "\n"; break;
        case EventType::ShipDestroyed:    std::cout << "[DESTROYED] The ship was destroyed!\n"; break;
        case EventType::Victory:          std::cout << ">>> Final wave destroyed!\n"; break;
        case EventType::ProjectileFired:  break;
    }
}
