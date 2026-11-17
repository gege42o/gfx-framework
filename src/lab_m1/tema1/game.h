#pragma once

#include <vector>

class Mesh;

namespace m1
{
    enum class GameResult
    {
        InProgress,
        PlayerWon,
        EnemyWon,
        Draw
    };

    class Card;
    class Playground;

    class GameObject
    {
    public:
        GameObject();

        void AddHp(int amount);
        void LowerHp(int amount);
        int GetHp() const;

    private:
        int hp;
    };

    class Game
    {
    public:
        Game();

        GameObject& GetPlayer();
        GameObject& GetEnemy();
        const GameObject& GetPlayer() const;
        const GameObject& GetEnemy() const;
        void DrawPlayerHP(
            float start_x,
            float start_y,
            float side,
            std::vector<Mesh*>& hp_boxes) const;
        void DrawEnemyHP(
            float start_x,
            float start_y,
            float side,
            std::vector<Mesh*>& hp_boxes) const;
        void ApplyTurnEffects(
            Playground& player_playground,
            Card* player_cards,
            std::vector<int>& player_hand,
            Playground& enemy_playground,
            Card* enemy_cards,
            std::vector<int>& enemy_hand);
        void EvaluateEndgame(
            const Playground& player_playground,
            const std::vector<int>& player_hand,
            const Playground& enemy_playground,
            const std::vector<int>& enemy_hand);
        bool IsFinished() const;
        GameResult GetResult() const;

    private:
        GameObject player;
        GameObject enemy;
        int player_shield;
        int enemy_shield;
        bool player_war_horn;
        bool enemy_war_horn;
        GameResult result;
    };
}
