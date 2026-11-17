#pragma once

#include <vector>

#include "lab_m1/tema1/card.h"
#include "lab_m1/tema1/playground.h"
#include "utils/glm_utils.h"

class Mesh;

namespace m1
{
    class Turn
    {
    public:
        Turn();

        Mesh* CreateButton(const char* name) const;
        bool HandleClick(float mouse_x, float mouse_y, Mesh* button);
        void PlayEnemyTurn(
            std::vector<int>& enemy_hand,
            Playground& enemy_playground,
            Card* enemy_cards,
            Mesh* button);
        bool IsPlayerTurn() const;

    private:
        void SetButtonColor(Mesh* button) const;

        float min_x;
        float max_x;
        float min_y;
        float max_y;
        bool player_turn;
    };
}
