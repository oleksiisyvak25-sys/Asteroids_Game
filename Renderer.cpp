#include "Renderer.h"
#include "Asteroid.h"
#include "Spaceship.h"
#include "Config.h"
#include <algorithm>
#include <cmath>
#include <iomanip>

namespace {
    const int COLS = 80;
    const int ROWS = 30;
    const float CELL_W = Config::WORLD_WIDTH / COLS;
    const float CELL_H = Config::WORLD_HEIGHT / ROWS;

    int clampInt(int v, int lo, int hi) {
        return std::max(lo, std::min(v, hi));
    }

    char asteroidGlyph(AsteroidType kind, int size) {
        static const char normal[]    = {'.', 'o', 'O'};
        static const char explosive[] = {'+', 'x', 'X'};
        static const char gravity[]   = {':', 'g', 'G'};
        const int idx = clampInt(size, 1, 3) - 1;
        switch (kind) {
            case AsteroidType::Explosive: return explosive[idx];
            case AsteroidType::Gravity:   return gravity[idx];
            default:                      return normal[idx];
        }
    }

    char shipGlyph(float angleDeg) {
        static const char glyphs[8] = {'>', '\\', 'v', '/', '<', '\\', '^', '/'};
        float a = std::fmod(angleDeg, 360.0f);
        if (a < 0.0f) a += 360.0f;
        const int idx = static_cast<int>(std::floor(a / 45.0f + 0.5f)) % 8;
        return glyphs[idx];
    }

    void plotCell(std::vector<std::string>& grid, const Vector2& p, char ch) {
        const int col = clampInt(static_cast<int>(p.x / CELL_W), 0, COLS - 1);
        const int row = clampInt(static_cast<int>(p.y / CELL_H), 0, ROWS - 1);
        grid[row][col] = ch;
    }

    void fillDisc(std::vector<std::string>& grid, const Vector2& c, float r, char ch) {
        const int c0 = clampInt(static_cast<int>((c.x - r) / CELL_W), 0, COLS - 1);
        const int c1 = clampInt(static_cast<int>((c.x + r) / CELL_W), 0, COLS - 1);
        const int r0 = clampInt(static_cast<int>((c.y - r) / CELL_H), 0, ROWS - 1);
        const int r1 = clampInt(static_cast<int>((c.y + r) / CELL_H), 0, ROWS - 1);

        for (int row = r0; row <= r1; ++row) {
            for (int col = c0; col <= c1; ++col) {
                const Vector2 center((col + 0.5f) * CELL_W, (row + 0.5f) * CELL_H);
                if (center.distanceTo(c) <= r) {
                    grid[row][col] = ch;
                }
            }
        }
        plotCell(grid, c, ch);
    }
}

Renderer::Renderer(std::shared_ptr<const ScoreManager> scoreManager, std::ostream& output)
    : out(output), scores(std::move(scoreManager)), currentWave(1) {}


void Renderer::render(const std::vector<std::shared_ptr<GameObject>>& objects) {
    std::vector<std::string> grid(ROWS, std::string(COLS, ' '));
    const Spaceship* ship = nullptr;

    for (const auto& obj : objects) {
        if (obj->isActive() && obj->getType() == ObjectType::Asteroid) {
            const Asteroid* a = static_cast<const Asteroid*>(obj.get());
            fillDisc(grid, a->getPosition(), a->getRadius(), asteroidGlyph(a->getKind(), a->getSize()));
        }
    }
    for (const auto& obj : objects) {
        if (obj->isActive() && obj->getType() == ObjectType::Bonus) {
            plotCell(grid, obj->getPosition(), '$');
        }
    }
    for (const auto& obj : objects) {
        if (obj->isActive() && obj->getType() == ObjectType::Projectile) {
            plotCell(grid, obj->getPosition(), '*');
        }
    }
    for (const auto& obj : objects) {
        if (obj->getType() == ObjectType::Ship) {
            ship = static_cast<const Spaceship*>(obj.get());
        }
    }
    if (ship) {
        const Vector2& p = ship->getPosition();
        if (ship->isShielded()) {
            const int col = clampInt(static_cast<int>(p.x / CELL_W), 0, COLS - 1);
            const int row = clampInt(static_cast<int>(p.y / CELL_H), 0, ROWS - 1);
            if (col > 0) grid[row][col - 1] = '(';
            if (col < COLS - 1) grid[row][col + 1] = ')';
        }
        plotCell(grid, p, shipGlyph(ship->getAngle()));
    }

    const std::string border = "+" + std::string(COLS, '-') + "+";
    out << "\n" << border << "\n";
    for (const auto& line : grid) {
        out << "|" << line << "|\n";
    }
    out << border << "\n";

    out << "Score: " << scores->getScore()
        << "   Best: " << scores->getHighScore()
        << "   Wave: " << currentWave;
    if (ship) {
        out << "   HP: " << ship->getHealth() << "/" << Config::SHIP_START_HEALTH;
        if (ship->isShielded()) {
            out << "   SHIELD: " << std::fixed << std::setprecision(1) << ship->getShieldTime() << "s";
        }
        out << "\nShip angle: " << static_cast<int>(std::lround(ship->getAngle()))
            << " deg   speed: " << static_cast<int>(std::lround(ship->getVelocity().length())) << " px/s";
    }
    out << "\nLegend: O/o/. normal  X/x/+ explosive  G/g/: gravity  $ shield bonus  * shot  ^ > v < ship\n";
}


void Renderer::onEvent(const GameEvent& event) {
    switch (event.type) {
        case EventType::GameStarted:
            out << "\n=== New game started ===\n";
            break;
        case EventType::WaveStarted:
            currentWave = event.value;
            out << ">>> WAVE " << event.value << " incoming!\n";
            break;
        case EventType::WaveCleared:
            out << ">>> Wave " << event.value << " cleared!\n";
            break;
        case EventType::AsteroidDestroyed:
            out << "[BOOM] Asteroid (size " << event.value << ") destroyed, +"
                << ScoreManager::pointsForSize(event.value) << " points\n";
            break;
        case EventType::Explosion:
            out << "[EXPLOSION] Blast radius " << event.value << " px!\n";
            break;
        case EventType::BonusDropped:
            out << "[BONUS] A shield bonus ($) dropped\n";
            break;
        case EventType::ShieldCollected:
            out << "[SHIELD] Collected! Invulnerable for " << event.value << " s\n";
            break;
        case EventType::ShipHit:
            out << "[HIT] Ship damaged! HP left: " << event.value << "\n";
            break;
        case EventType::ShipDestroyed:
            out << "[DESTROYED] The ship was destroyed!\n";
            break;
        case EventType::Victory:
            out << ">>> All asteroids of the final wave are destroyed!\n";
            break;
        case EventType::ProjectileFired:
            break;
    }
}


void Renderer::showMenu() {
    out << "\n==========================================\n"
        << "                ASTEROIDS\n"
        << "==========================================\n"
        << " Best score: " << scores->getHighScore() << "\n\n"
        << " [S] start game     [Q] quit\n\n"
        << " In game: W thrust | A/D rotate | Space or F shoot | P pause | Q quit\n"
        << " One input line = one 0.25 s turn (you can type several keys, e.g. \"wwdf\").\n"
        << " An empty line = wait one turn.\n";
}

void Renderer::showPaused() {
    out << "\n--- PAUSED ---   [R] resume   [M] main menu   [Q] quit\n";
}

void Renderer::showGameOver() {
    out << "\n==========================================\n"
        << "                GAME OVER\n"
        << "==========================================\n"
        << " Final score: " << scores->getScore() << "   Best: " << scores->getHighScore() << "\n"
        << " [M] main menu     [Q] quit\n";
}

void Renderer::showVictory() {
    out << "\n==========================================\n"
        << "                 VICTORY!\n"
        << "==========================================\n"
        << " Score: " << scores->getScore() << "   Best: " << scores->getHighScore() << "\n"
        << " [C] continue in endless mode (beat your high score)\n"
        << " [M] main menu     [Q] quit\n";
}

void Renderer::showPrompt(const std::string& text) {
    out << text << std::flush;
}
