#include "ScoreManager.h"
#include <fstream>

ScoreManager::ScoreManager(const std::string& highScoreFile)
    : score(0), highScore(0), savedHighScore(0), filePath(highScoreFile) {
    loadHighScore();
}

ScoreManager::~ScoreManager() {
    saveHighScoreIfBeaten();
}

int ScoreManager::pointsForSize(int size) {
    switch (size) {
        case 1:  return 100;
        case 2:  return 50;
        case 3:  return 20;
        default: return 10;
    }
}

void ScoreManager::onEvent(const GameEvent& event) {
    switch (event.type) {
        case EventType::GameStarted:
            score = 0;
            break;
        case EventType::AsteroidDestroyed:
            score += pointsForSize(event.value);
            if (score > highScore) {
                highScore = score;
            }
            break;
        case EventType::ShipDestroyed:
        case EventType::Victory:
            saveHighScoreIfBeaten();
            break;
        default:
            break;
    }
}

int ScoreManager::getScore() const { return score; }
int ScoreManager::getHighScore() const { return highScore; }

void ScoreManager::loadHighScore() {
    std::ifstream in(filePath);
    int value = 0;
    if (in && (in >> value) && value > 0) {
        highScore = value;
        savedHighScore = value;
    }
}

void ScoreManager::saveHighScoreIfBeaten() {
    if (highScore <= savedHighScore) {
        return;
    }
    std::ofstream out(filePath, std::ios::trunc);
    if (out) {
        out << highScore << "\n";
        savedHighScore = highScore;
    }
}
