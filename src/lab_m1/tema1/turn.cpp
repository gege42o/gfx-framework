#include "lab_m1/tema1/turn.h"

#include <random>

#include "core/gpu/mesh.h"
#include "lab_m1/tema1/object2D.h"
#include "utils/gl_utils.h"

namespace m1
{
    Turn::Turn()
        : min_x(950.0f)
        , max_x(1050.0f)
        , min_y(330.0f)
        , max_y(345.0f)
        , player_turn(true)
    {
    }

    Mesh* Turn::CreateButton(const char* name) const
    {
        Mesh* button = tema1_object2D::CreateRectangle(
            name,
            glm::vec3(min_x, min_y, 0),
            max_x - min_x,
            max_y - min_y,
            player_turn ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0),
            true);
        return button;
    }

    bool Turn::HandleClick(float mouse_x, float mouse_y, Mesh* button)
    {
        if (!player_turn ||
            mouse_x < min_x || mouse_x > max_x ||
            mouse_y < min_y || mouse_y > max_y)
        {
            return false;
        }

        player_turn = false;
        SetButtonColor(button);
        return true;
    }

    bool Turn::IsPlayerTurn() const
    {
        return player_turn;
    }

    void Turn::PlayEnemyTurn(
        std::vector<int>& enemy_hand,
        Playground& enemy_playground,
        Card* enemy_cards,
        Mesh* button)
    {
        if (player_turn)
        {
            return;
        }

        if (enemy_hand.empty())
        {
            player_turn = true;
            SetButtonColor(button);
            return;
        }

        static std::random_device random_device;
        static std::mt19937 generator(random_device());
        std::uniform_int_distribution<size_t> distribution(0, enemy_hand.size() - 1);

        const size_t selected_index = distribution(generator);
        const int card_index = enemy_hand[selected_index];
        enemy_hand.erase(enemy_hand.begin() + selected_index);
        enemy_playground.AddCard(card_index);
        enemy_playground.ArrangeCards(enemy_cards);

        player_turn = true;
        SetButtonColor(button);
    }

    void Turn::SetButtonColor(Mesh* button) const
    {
        const glm::vec3 color = player_turn
            ? glm::vec3(0, 1, 0)
            : glm::vec3(1, 0, 0);
        button->InitFromData(
            {
                VertexFormat(glm::vec3(min_x, min_y, 0), color),
                VertexFormat(glm::vec3(max_x, min_y, 0), color),
                VertexFormat(glm::vec3(max_x, max_y, 0), color),
                VertexFormat(glm::vec3(min_x, max_y, 0), color)
            },
            { 0, 1, 2, 0, 2, 3 });
        button->SetDrawMode(GL_TRIANGLES);
    }
}
