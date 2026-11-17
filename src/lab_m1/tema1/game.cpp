#include "lab_m1/tema1/game.h"

#include <algorithm>
#include <random>
#include <string>

#include "core/gpu/mesh.h"
#include "lab_m1/tema1/card.h"
#include "lab_m1/tema1/object2D.h"
#include "lab_m1/tema1/playground.h"

namespace m1
{
    GameObject::GameObject()
        : hp(5)
    {
    }

    void GameObject::AddHp(int amount)
    {
        hp = std::min(5, hp + std::max(0, amount));
    }

    void GameObject::LowerHp(int amount)
    {
        hp = std::max(0, hp - std::max(0, amount));
    }

    int GameObject::GetHp() const
    {
        return hp;
    }

    Game::Game()
        : player()
        , enemy()
        , player_shield(0)
        , enemy_shield(0)
        , player_war_horn(false)
        , enemy_war_horn(false)
        , result(GameResult::InProgress)
    {
    }

    GameObject& Game::GetPlayer()
    {
        return player;
    }

    GameObject& Game::GetEnemy()
    {
        return enemy;
    }

    const GameObject& Game::GetPlayer() const
    {
        return player;
    }

    const GameObject& Game::GetEnemy() const
    {
        return enemy;
    }

    void Game::DrawPlayerHP(
        float start_x,
        float start_y,
        float side,
        std::vector<Mesh*>& hp_boxes) const
    {
        const int player_hp = player.GetHp();
        constexpr float spacing = 5.0f;

        for (int i = 0; i < 5; ++i)
        {
            const glm::vec3 color = i < player_hp
                ? glm::vec3(0, 1, 0)
                : glm::vec3(1, 0, 0);
            const glm::vec3 corner(start_x + i * (side + spacing), start_y, 0);

            if (hp_boxes.size() <= static_cast<size_t>(i))
            {
                hp_boxes.push_back(
                    tema1_object2D::CreateRectangle(
                        "player_hp_" + std::to_string(i),
                        corner,
                        side,
                        side,
                        color,
                        true));
            }
            else
            {
                hp_boxes[i]->InitFromData(
                    {
                        VertexFormat(corner, color),
                        VertexFormat(corner + glm::vec3(side, 0, 0), color),
                        VertexFormat(corner + glm::vec3(side, side, 0), color),
                        VertexFormat(corner + glm::vec3(0, side, 0), color)
                    },
                    { 0, 1, 2, 0, 2, 3 });
                hp_boxes[i]->SetDrawMode(GL_TRIANGLES);
            }
        }
    }

    void Game::DrawEnemyHP(
        float start_x,
        float start_y,
        float side,
        std::vector<Mesh*>& hp_boxes) const
    {
        const int enemy_hp = enemy.GetHp();
        constexpr float spacing = 5.0f;

        for (int i = 0; i < 5; ++i)
        {
            const glm::vec3 color = i < enemy_hp
                ? glm::vec3(0, 1, 0)
                : glm::vec3(1, 0, 0);
            const glm::vec3 corner(start_x + i * (side + spacing), start_y, 0);

            if (hp_boxes.size() <= static_cast<size_t>(i))
            {
                hp_boxes.push_back(
                    tema1_object2D::CreateRectangle(
                        "enemy_hp_" + std::to_string(i),
                        corner,
                        side,
                        side,
                        color,
                        true));
            }
            else
            {
                hp_boxes[i]->InitFromData(
                    {
                        VertexFormat(corner, color),
                        VertexFormat(corner + glm::vec3(side, 0, 0), color),
                        VertexFormat(corner + glm::vec3(side, side, 0), color),
                        VertexFormat(corner + glm::vec3(0, side, 0), color)
                    },
                    { 0, 1, 2, 0, 2, 3 });
                hp_boxes[i]->SetDrawMode(GL_TRIANGLES);
            }
        }
    }

    void Game::ApplyTurnEffects(
                Playground& player_playground,
                Card* player_cards,
                std::vector<int>& player_hand,
                Playground& enemy_playground,
                Card* enemy_cards,
                std::vector<int>& enemy_hand)
            {
                bool player_war_horn_consumed = false;
                bool enemy_war_horn_consumed = false;

                struct PlayedCard
                {
                    Card* card;
                    int index;
                    bool player;
                    bool cancelled;
                };

                std::vector<PlayedCard> played_cards;
                for (int index : player_playground.GetCards())
                {
                    played_cards.push_back({ &player_cards[index], index, true, false });
                }
                for (int index : enemy_playground.GetCards())
                {
                    played_cards.push_back({ &enemy_cards[index], index, false, false });
                }

                //scorch logic
                for (PlayedCard& played : played_cards)
                {
                    if (played.card->GetType() != 8 || played.card->GetV() <= 0)
                    {
                        continue;
                    }

                    for (PlayedCard& opponent : played_cards)
                    {
                        if (opponent.player != played.player &&
                            opponent.card->GetV() >= played.card->GetV())
                        {
                            opponent.cancelled = true;
                        }
                    }
                }

                const bool clear_weather = std::any_of(
                    played_cards.begin(),
                    played_cards.end(),
                    [](const PlayedCard& played) {
                        return !played.cancelled && played.card->GetType() == 6 &&
                               played.card->GetV() > 0;
                    });

                static std::random_device random_device;
                static std::mt19937 generator(random_device());

                //spy logic
                for (PlayedCard& played : played_cards)
                {
                    if (played.cancelled || played.card->GetType() != 4 ||
                        played.card->GetV() <= 0)
                    {
                        continue;
                    }

                    std::vector<int>& source_hand = played.player ? enemy_hand : player_hand;
                    std::vector<int>& target_hand = played.player ? player_hand : enemy_hand;
                    if (!source_hand.empty())
                    {
                        std::uniform_int_distribution<size_t> distribution(0, source_hand.size() - 1);
                        const size_t selected_index = distribution(generator);
                        target_hand.push_back(source_hand[selected_index]);
                        source_hand.erase(source_hand.begin() + selected_index);
                    }
                }

                //clear_weather logic
                if (clear_weather)
                {
                    for (int index : player_playground.GetCards())
                    {
                        if (player_cards[index].GetType() == 5)
                        {
                            player_cards[index].SetV(0);
                        }
                    }
                    for (int index : enemy_playground.GetCards())
                    {
                        if (enemy_cards[index].GetType() == 5)
                        {
                            enemy_cards[index].SetV(0);
                        }
                    }
                }
                else
                {
                    //frost_weather logic
                    const std::vector<int> player_cards_in_playground = player_playground.GetCards();
                    const std::vector<int> enemy_cards_in_playground = enemy_playground.GetCards();

                    for (int index : player_cards_in_playground)
                    {
                        if (player_cards[index].GetType() == 5 && player_cards[index].GetV() > 0)
                        {
                            for (int opponent_index : enemy_cards_in_playground)
                            {
                                enemy_cards[opponent_index].SetV(
                                    enemy_cards[opponent_index].GetV() - 1);
                            }
                            player_cards[index].SetV(player_cards[index].GetV() - 1);
                        }
                    }
                    for (int index : enemy_cards_in_playground)
                    {
                        if (enemy_cards[index].GetType() == 5 && enemy_cards[index].GetV() > 0)
                        {
                            for (int opponent_index : player_cards_in_playground)
                            {
                                player_cards[opponent_index].SetV(
                                    player_cards[opponent_index].GetV() - 1);
                            }
                            enemy_cards[index].SetV(enemy_cards[index].GetV() - 1);
                        }
                    }
                }

                if (player_war_horn || enemy_war_horn)
                {
                    //war_horn logic
                    for (PlayedCard& played : played_cards)
                    {
                        if (played.cancelled || played.card->GetV() <= 0 ||
                            played.card->GetType() == 7)
                        {
                            continue;
                        }

                        if (played.player && player_war_horn && !player_war_horn_consumed)
                        {
                            played.card->SetV(played.card->GetV() * 2);
                            player_war_horn_consumed = true;
                        }
                        else if (!played.player && enemy_war_horn && !enemy_war_horn_consumed)
                        {
                            played.card->SetV(played.card->GetV() * 2);
                            enemy_war_horn_consumed = true;
                        }
                    }
                }

                //war_horn_state logic
                for (PlayedCard& played : played_cards)
                {
                    if (!played.cancelled && played.card->GetType() == 7 &&
                        played.card->GetV() > 0)
                    {
                        if (played.player)
                        {
                            player_war_horn = true;
                        }
                        else
                        {
                            enemy_war_horn = true;
                        }
                    }
                }

                //shield logic
                for (PlayedCard& played : played_cards)
                {
                    if (played.cancelled || played.card->GetType() != 3 ||
                        played.card->GetV() <= 0)
                    {
                        continue;
                    }

                    int& shield = played.player ? player_shield : enemy_shield;
                    shield += played.card->GetV();
                }

                //damage logic
                //heal logic
                for (PlayedCard& played : played_cards)
                {
                    if (played.cancelled || played.card->GetType() != 1 ||
                        played.card->GetV() <= 0)
                    {
                        continue;
                    }

                    int& shield = played.player ? enemy_shield : player_shield;
                    GameObject& target = played.player ? enemy : player;
                    const int blocked_damage = std::min(shield, played.card->GetV());
                    shield -= blocked_damage;
                    target.LowerHp(played.card->GetV() - blocked_damage);
                }

                for (PlayedCard& played : played_cards)
                {
                    if (!played.cancelled && played.card->GetType() == 2 &&
                        played.card->GetV() > 0)
                    {
                        (played.player ? player : enemy).AddHp(played.card->GetV());
                    }
                }

                for (PlayedCard& played : played_cards)
                {
                    if (played.cancelled || played.card->GetV() <= 0)
                    {
                        continue;
                    }

                    if (played.card->GetType() >= 4 &&
                        played.card->GetType() != 5)
                    {
                        played.card->SetV(played.card->GetV() - 1);
                    }
                }

                const std::vector<int> player_cards_in_playground = player_playground.GetCards();
                const std::vector<int> enemy_cards_in_playground = enemy_playground.GetCards();

                for (int index : player_cards_in_playground)
                {
                    if (player_cards[index].GetV() == 0)
                    {
                        player_playground.RemoveCard(index);
                    }
                }
                for (int index : enemy_cards_in_playground)
                {
                    if (enemy_cards[index].GetV() == 0)
                    {
                        enemy_playground.RemoveCard(index);
                    }
                }

                if (player_war_horn_consumed)
                {
                    player_war_horn = false;
                }
                if (enemy_war_horn_consumed)
                {
                    enemy_war_horn = false;
                }
            }

            void Game::EvaluateEndgame(
                const Playground& player_playground,
                const std::vector<int>& player_hand,
                const Playground& enemy_playground,
                const std::vector<int>& enemy_hand)
            {
                if (result != GameResult::InProgress)
                {
                    return;
                }

                const bool player_dead = player.GetHp() == 0;
                const bool enemy_dead = enemy.GetHp() == 0;
                if (player_dead || enemy_dead)
                {
                    if (player_dead && enemy_dead)
                    {
                        result = GameResult::Draw;
                    }
                    else if (enemy_dead)
                    {
                        result = GameResult::PlayerWon;
                    }
                    else
                    {
                        result = GameResult::EnemyWon;
                    }
                    return;
                }

                if (player_hand.empty() && enemy_hand.empty())
                {
                    if (player.GetHp() > enemy.GetHp())
                    {
                        result = GameResult::PlayerWon;
                    }
                    else if (enemy.GetHp() > player.GetHp())
                    {
                        result = GameResult::EnemyWon;
                    }
                    else
                    {
                        result = GameResult::Draw;
                    }
                }
            }

            bool Game::IsFinished() const
            {
                return result != GameResult::InProgress;
            }

            GameResult Game::GetResult() const
            {
                return result;
            }
}
