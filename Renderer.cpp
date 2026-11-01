#include "Renderer.h"
#include "Asteroid.h"
#include "Config.h"
#include <SFML/Config.hpp>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#if SFML_VERSION_MAJOR >= 3
#include <optional>
#endif

namespace {
    const sf::Color BACKGROUND(6, 6, 16);
    const sf::Color SHIP_COLOR(225, 235, 255);
    const sf::Color SHIP_OUTLINE(110, 190, 255);
    const sf::Color SHIELD_COLOR(80, 200, 255);
    const sf::Color PROJECTILE_COLOR(255, 235, 110);
    const sf::Color BONUS_COLOR(80, 230, 120);
    const sf::Color TEXT_COLOR(230, 230, 230);

    const int STAR_COUNT = 140;
    const std::size_t MAX_PARTICLES = 200;
    const bool PARTICLES_ENABLED = true;

    sf::Color withAlpha(const sf::Color& color, float alpha) {
        const float clamped = std::max(0.0f, std::min(255.0f, alpha));
        return sf::Color(color.r, color.g, color.b, static_cast<std::uint8_t>(clamped));
    }

    sf::Color scaled(const sf::Color& color, float k) {
        return sf::Color(static_cast<std::uint8_t>(std::min(255.0f, color.r * k)),
                         static_cast<std::uint8_t>(std::min(255.0f, color.g * k)),
                         static_cast<std::uint8_t>(std::min(255.0f, color.b * k)));
    }

    sf::Color asteroidColor(AsteroidType kind) {
        switch (kind) {
            case AsteroidType::Explosive: return sf::Color(225, 90, 55);
            case AsteroidType::Gravity:   return sf::Color(155, 105, 235);
            default:                      return sf::Color(150, 150, 165);
        }
    }

    bool isDown(sf::Keyboard::Key key) {
        return sf::Keyboard::isKeyPressed(key);
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
        if (radius < 0.5f) {
            return;
        }
        const float wanted = std::max(12.0f, std::min(100.0f, radius * 1.5f));
        sf::CircleShape shape(radius, static_cast<std::size_t>(wanted));
        shape.setOrigin(sf::Vector2f(radius, radius));
        shape.setPosition(sf::Vector2f(center.x, center.y));
        shape.setFillColor(fill);
        shape.setOutlineColor(outline);
        shape.setOutlineThickness(outlineThickness);
        target.draw(shape);
    }

    void drawTriangle(sf::RenderTarget& target, const Vector2& a, const Vector2& b, const Vector2& c,
                      const sf::Color& fill, const sf::Color& outline = sf::Color::Transparent,
                      float outlineThickness = 0.0f) {
        sf::ConvexShape tri;
        tri.setPointCount(3);
        tri.setPoint(0, sf::Vector2f(a.x, a.y));
        tri.setPoint(1, sf::Vector2f(b.x, b.y));
        tri.setPoint(2, sf::Vector2f(c.x, c.y));
        tri.setFillColor(fill);
        tri.setOutlineColor(outline);
        tri.setOutlineThickness(outlineThickness);
        target.draw(tri);
    }

    void drawGravityField(sf::RenderTarget& target, const Asteroid& asteroid, float time) {
        const float pulse = 0.5f + 0.5f * std::sin(time * 2.0f);
        const sf::Color base = asteroidColor(asteroid.getKind());
        drawCircle(target, asteroid.getPosition(), Config::GRAVITY_RANGE,
                   withAlpha(base, 10.0f), withAlpha(base, 25.0f + 30.0f * pulse), 1.5f);
    }

    void drawAsteroid(sf::RenderTarget& target, const Asteroid& asteroid, float time) {
        const Vector2& p = asteroid.getPosition();
        const float r = asteroid.getRadius();
        const sf::Color base = asteroidColor(asteroid.getKind());

        sf::Color outline = scaled(base, 1.35f);
        float thickness = 2.0f;
        if (asteroid.getKind() == AsteroidType::Explosive) {
            const float pulse = 0.5f + 0.5f * std::sin(time * 7.0f);
            outline = sf::Color(255, static_cast<std::uint8_t>(120 + 110 * pulse), 70);
            thickness = 2.0f + 2.0f * pulse;
        }

        drawCircle(target, p, r, scaled(base, 0.55f), outline, thickness);
        drawCircle(target, p + Vector2(-r * 0.3f, -r * 0.3f), r * 0.35f,
                   withAlpha(sf::Color(255, 255, 255), 28.0f));
    }

    void drawBonus(sf::RenderTarget& target, const GameObject& bonus, float time) {
        const float pulse = 0.5f + 0.5f * std::sin(time * 5.0f);
        const float r = bonus.getRadius();
        drawCircle(target, bonus.getPosition(), r * 2.0f, withAlpha(BONUS_COLOR, 20.0f + 25.0f * pulse));
        drawCircle(target, bonus.getPosition(), r * (1.0f + 0.15f * pulse),
                   withAlpha(BONUS_COLOR, 210.0f), sf::Color(200, 255, 220), 2.0f);
        drawCircle(target, bonus.getPosition(), r * 0.35f, sf::Color(255, 255, 255));
    }

    void drawProjectile(sf::RenderTarget& target, const GameObject& projectile) {
        const float r = projectile.getRadius();
        drawCircle(target, projectile.getPosition(), r * 2.6f, withAlpha(PROJECTILE_COLOR, 55.0f));
        drawCircle(target, projectile.getPosition(), r, PROJECTILE_COLOR);
    }

    void drawFlame(sf::RenderTarget& target, const Spaceship& ship, float time) {
        const Vector2& p = ship.getPosition();
        const float r = ship.getRadius();
        const float a = ship.getAngle();
        const Vector2 dir = Vector2::fromAngle(a);

        const Vector2 b1 = p + Vector2::fromAngle(a + 140.0f) * r;
        const Vector2 b2 = p + Vector2::fromAngle(a - 140.0f) * r;
        const Vector2 mid = (b1 + b2) * 0.5f;
        const Vector2 half = (b1 - b2) * 0.5f;

        const float flicker = 0.5f + 0.5f * std::sin(time * 55.0f) * std::sin(time * 37.0f);
        const float length = 9.0f + 9.0f * flicker;

        drawTriangle(target, mid + half * 0.7f, mid - half * 0.7f, mid - dir * length,
                     sf::Color(255, 140, 40));
        drawTriangle(target, mid + half * 0.35f, mid - half * 0.35f, mid - dir * (length * 0.6f),
                     sf::Color(255, 230, 120));
    }

    void drawShip(sf::RenderTarget& target, const Spaceship& ship, bool thrusting, float time) {
        const Vector2& p = ship.getPosition();
        const float r = ship.getRadius();
        const float a = ship.getAngle();

        if (thrusting) {
            drawFlame(target, ship, time);
        }

        drawTriangle(target,
                     p + Vector2::fromAngle(a) * r,
                     p + Vector2::fromAngle(a + 140.0f) * r,
                     p + Vector2::fromAngle(a - 140.0f) * r,
                     SHIP_COLOR, SHIP_OUTLINE, 1.5f);

        if (ship.isShielded()) {
            float visible = 1.0f;
            if (ship.getShieldTime() < 1.0f) {
                visible = (std::sin(time * 25.0f) > 0.0f) ? 1.0f : 0.35f;
            }
            drawCircle(target, p, r + 7.0f, withAlpha(SHIELD_COLOR, 30.0f * visible),
                       withAlpha(SHIELD_COLOR, 230.0f * visible), 2.0f);
        }
    }
}

Renderer::Renderer(std::shared_ptr<const ScoreManager> scoreManager, bool logToConsole)
    : fontLoaded(false),
      logEvents(logToConsole),
      scores(std::move(scoreManager)),
      currentWave(1),
      time(0.0f),
      thrusting(false),
      effectsRng(std::random_device{}()) {
    const unsigned int w = static_cast<unsigned int>(Config::WORLD_WIDTH);
    const unsigned int h = static_cast<unsigned int>(Config::WORLD_HEIGHT);
#if SFML_VERSION_MAJOR >= 3
    window.create(sf::VideoMode({w, h}), "Asteroids", sf::Style::Titlebar | sf::Style::Close);
#else
    window.create(sf::VideoMode(w, h), "Asteroids", sf::Style::Titlebar | sf::Style::Close);
#endif
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    stars.reserve(STAR_COUNT);
    for (int i = 0; i < STAR_COUNT; ++i) {
        Star star;
        star.x = randomRange(0.0f, Config::WORLD_WIDTH);
        star.y = randomRange(0.0f, Config::WORLD_HEIGHT);
        star.size = randomRange(0.7f, 1.9f);
        star.phase = randomRange(0.0f, 6.28f);
        star.speed = randomRange(0.5f, 2.5f);
        stars.push_back(star);
    }

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

float Renderer::randomRange(float lo, float hi) {
    std::uniform_real_distribution<float> dist(lo, hi);
    return dist(effectsRng);
}


bool Renderer::isOpen() const { return window.isOpen(); }

void Renderer::pollEvents() {
#if SFML_VERSION_MAJOR >= 3
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } else if (const sf::Event::KeyPressed* key = event->getIf<sf::Event::KeyPressed>()) {
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
    switch (key) {
        case 'w': return isDown(K::W) || isDown(K::Up);
        case 'a': return isDown(K::A) || isDown(K::Left);
        case 'd': return isDown(K::D) || isDown(K::Right);
        case ' ': return isDown(K::Space) || isDown(K::F);
        default:  return false;
    }
}

void Renderer::update(float dt) {
    time += dt;

    for (Particle& p : particles) {
        p.life -= dt;
        p.position += p.velocity * dt;
        p.velocity = p.velocity * std::max(0.0f, 1.0f - 2.0f * dt);
    }
    particles.erase(std::remove_if(particles.begin(), particles.end(),
                                   [](const Particle& p) { return p.life <= 0.0f; }),
                    particles.end());

    for (Ring& r : rings) {
        r.age += dt;
    }
    rings.erase(std::remove_if(rings.begin(), rings.end(),
                               [](const Ring& r) { return r.age >= r.duration; }),
                rings.end());
}

void Renderer::setShipThrusting(bool value) {
    thrusting = value;
}

void Renderer::spawnBurst(const Vector2& position, int count, float speedMin, float speedMax,
                          const sf::Color& colorA, const sf::Color& colorB,
                          float sizeMin, float sizeMax, float lifeMin, float lifeMax) {
    if (!PARTICLES_ENABLED) {
        return;
    }
    for (int i = 0; i < count && particles.size() < MAX_PARTICLES; ++i) {
        Particle p;
        p.position = position;
        p.velocity = Vector2::fromAngle(randomRange(0.0f, 360.0f)) * randomRange(speedMin, speedMax);
        p.maxLife = randomRange(lifeMin, lifeMax);
        p.life = p.maxLife;
        p.size = randomRange(sizeMin, sizeMax);
        p.color = (randomRange(0.0f, 1.0f) < 0.5f) ? colorA : colorB;
        particles.push_back(p);
    }
}

void Renderer::spawnRing(const Vector2& position, float maxRadius, float duration,
                         const sf::Color& color, bool filled) {
    Ring ring;
    ring.position = position;
    ring.maxRadius = maxRadius;
    ring.age = 0.0f;
    ring.duration = duration;
    ring.color = color;
    ring.filled = filled;
    rings.push_back(ring);
}

void Renderer::clearEffects() {
    particles.clear();
    rings.clear();
}

void Renderer::beginFrame() { window.clear(BACKGROUND); }
void Renderer::endFrame()   { window.display(); }

void Renderer::drawBackground() {
    sf::RectangleShape dot;
    for (const Star& star : stars) {
        const float twinkle = 0.5f + 0.5f * std::sin(time * star.speed + star.phase);
        dot.setSize(sf::Vector2f(star.size, star.size));
        dot.setPosition(sf::Vector2f(star.x, star.y));
        dot.setFillColor(withAlpha(sf::Color(200, 210, 255), 70.0f + 150.0f * twinkle));
        window.draw(dot);
    }
}

void Renderer::drawEffects() {
    for (const Ring& ring : rings) {
        const float t = ring.age / ring.duration;
        const float eased = 1.0f - (1.0f - t) * (1.0f - t);
        const float fade = 1.0f - t;
        drawCircle(window, ring.position, ring.maxRadius * eased,
                   ring.filled ? withAlpha(ring.color, 45.0f * fade) : sf::Color::Transparent,
                   withAlpha(ring.color, 255.0f * fade), 1.0f + 3.0f * fade);
    }

    sf::RectangleShape dot;
    for (const Particle& p : particles) {
        const float lifeFraction = p.life / p.maxLife;
        const float side = 2.0f * p.size * (0.4f + 0.6f * lifeFraction);
        dot.setSize(sf::Vector2f(side, side));
        dot.setOrigin(sf::Vector2f(side / 2.0f, side / 2.0f));
        dot.setPosition(sf::Vector2f(p.position.x, p.position.y));
        dot.setFillColor(withAlpha(p.color, 255.0f * lifeFraction));
        window.draw(dot);
    }
}

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
    for (const std::string& line : lines) {
        drawText(line, cx, y, 20, TEXT_COLOR, true);
        y += 32.0f;
    }
}

void Renderer::drawHud(const Spaceship* ship, int bulletCount) {
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
    line2 << "   bullets: " << bulletCount << "   fx: " << particles.size();
    drawText(line1.str(), 10.0f, 8.0f, 16, TEXT_COLOR, false);
    drawText(line2.str(), 10.0f, 30.0f, 16, TEXT_COLOR, false);
    if (!fontLoaded) {
        window.setTitle("Asteroids | " + line1.str() + " | " + line2.str());
    }

    if (!ship) {
        return;
    }

    for (int i = 0; i < Config::SHIP_START_HEALTH; ++i) {
        const Vector2 center(Config::WORLD_WIDTH - 22.0f - 22.0f * static_cast<float>(i), 20.0f);
        if (i < ship->getHealth()) {
            drawCircle(window, center, 7.0f, sf::Color(230, 70, 85), sf::Color(255, 170, 180), 1.5f);
        } else {
            drawCircle(window, center, 7.0f, sf::Color::Transparent, sf::Color(110, 60, 70), 1.5f);
        }
    }

    if (ship->isShielded()) {
        const float fraction = std::min(1.0f, ship->getShieldTime() / Config::SHIELD_BONUS_DURATION);
        const float barWidth = 100.0f;
        const float x = Config::WORLD_WIDTH - 20.0f - barWidth;

        sf::RectangleShape back(sf::Vector2f(barWidth, 6.0f));
        back.setPosition(sf::Vector2f(x, 38.0f));
        back.setFillColor(sf::Color(40, 50, 70));
        window.draw(back);

        sf::RectangleShape fill(sf::Vector2f(barWidth * fraction, 6.0f));
        fill.setPosition(sf::Vector2f(x, 38.0f));
        fill.setFillColor(SHIELD_COLOR);
        window.draw(fill);
    }
}

void Renderer::render(const std::vector<std::shared_ptr<GameObject>>& objects) {
    const bool thrustNow = thrusting;
    thrusting = false;

    drawBackground();

    const Spaceship* ship = nullptr;
    for (const std::shared_ptr<GameObject>& obj : objects) {
        if (obj->getType() == ObjectType::Ship) {
            ship = static_cast<const Spaceship*>(obj.get());
        }
    }

    for (const std::shared_ptr<GameObject>& obj : objects) {
        if (!obj->isActive() || obj->getType() != ObjectType::Asteroid) continue;
        const Asteroid* a = static_cast<const Asteroid*>(obj.get());
        if (a->getKind() == AsteroidType::Gravity) {
            drawGravityField(window, *a, time);
        }
    }
    for (const std::shared_ptr<GameObject>& obj : objects) {
        if (!obj->isActive() || obj->getType() != ObjectType::Asteroid) continue;
        drawAsteroid(window, *static_cast<const Asteroid*>(obj.get()), time);
    }
    for (const std::shared_ptr<GameObject>& obj : objects) {
        if (obj->isActive() && obj->getType() == ObjectType::Bonus) {
            drawBonus(window, *obj, time);
        }
    }
    int bulletCount = 0;
    for (const std::shared_ptr<GameObject>& obj : objects) {
        if (obj->isActive() && obj->getType() == ObjectType::Projectile) {
            drawProjectile(window, *obj);
            ++bulletCount;
        }
    }
    drawEffects();
    if (ship && ship->getHealth() > 0) {
        drawShip(window, *ship, thrustNow, time);
    }
    drawHud(ship, bulletCount);
}

void Renderer::showMenu() {
    drawBackground();
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
    switch (event.type) {
        case EventType::GameStarted:
            clearEffects();
            break;
        case EventType::WaveStarted:
            currentWave = event.value;
            break;
        case EventType::AsteroidDestroyed:
            spawnBurst(event.position, 3 + 2 * event.value, 30.0f, 140.0f,
                       sf::Color(170, 170, 185), sf::Color(105, 105, 120),
                       1.0f, 2.2f, 0.25f, 0.6f);
            break;
        case EventType::Explosion:
            spawnRing(event.position, static_cast<float>(event.value), 0.45f, sf::Color(255, 140, 50), true);
            spawnBurst(event.position, 12, 100.0f, 300.0f,
                       sf::Color(255, 120, 40), sf::Color(255, 60, 30),
                       1.2f, 2.6f, 0.25f, 0.55f);
            break;
        case EventType::ShieldCollected:
            spawnRing(event.position, 60.0f, 0.4f, SHIELD_COLOR, false);
            break;
        case EventType::ShipHit:
            spawnBurst(event.position, 10, 60.0f, 200.0f,
                       sf::Color(255, 255, 255), sf::Color(255, 120, 120),
                       1.2f, 2.5f, 0.25f, 0.5f);
            break;
        case EventType::ShipDestroyed:
            spawnRing(event.position, 120.0f, 0.8f, sf::Color(255, 255, 255), true);
            spawnBurst(event.position, 60, 50.0f, 380.0f,
                       sf::Color(255, 255, 255), SHIELD_COLOR,
                       1.5f, 4.0f, 0.6f, 1.5f);
            break;
        default:
            break;
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