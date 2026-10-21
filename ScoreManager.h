#ifndef ASTEROIDS_GAME_SCOREMANAGER_H
#define ASTEROIDS_GAME_SCOREMANAGER_H

#include "GameObserver.h"
#include <string>

class ScoreManager : public GameObserver {
private:
    int score;
    int highScore;
    int savedHighScore;
    std::string filePath;

    void loadHighScore();
    void saveHighScoreIfBeaten();

public:
    explicit ScoreManager(const std::string& highScoreFile = "highscore.txt");
    ~ScoreManager() override;

    void onEvent(const GameEvent& event) override;

    int getScore() const;
    int getHighScore() const;

    static int pointsForSize(int size);
};

#endif //ASTEROIDS_GAME_SCOREMANAGER_H
