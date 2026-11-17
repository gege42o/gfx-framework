#include <algorithm>

#include "lab_m1/tema1/playground.h"

namespace m1
{
    Playground::Playground(
        float min_x,
        float max_x,
        float min_y,
        float max_y,
        float first_card_x,
        float card_y,
        float card_spacing)
        : min_x(min_x)
        , max_x(max_x)
        , min_y(min_y)
        , max_y(max_y)
        , first_card_x(first_card_x)
        , card_y(card_y)
        , card_spacing(card_spacing)
    {
    }

    bool Playground::Contains(float x, float y) const
    {
        return x >= min_x && x <= max_x && y >= min_y && y <= max_y;
    }

    void Playground::Clear()
    {
        cards.clear();
        locked_cards.clear();
    }

    void Playground::AddCard(int card_index)
    {
        if (std::find(cards.begin(), cards.end(), card_index) == cards.end())
        {
            cards.push_back(card_index);
        }
    }

    void Playground::RemoveCard(int card_index)
    {
        cards.erase(std::remove(cards.begin(), cards.end(), card_index), cards.end());
    }

    bool Playground::HasCard(int card_index) const
    {
        return std::find(cards.begin(), cards.end(), card_index) != cards.end();
    }

    void Playground::LockCards()
    {
        locked_cards.insert(cards.begin(), cards.end());
    }

    bool Playground::IsCardLocked(int card_index) const
    {
        return locked_cards.find(card_index) != locked_cards.end();
    }

    const std::vector<int>& Playground::GetCards() const
    {
        return cards;
    }

    std::vector<int>& Playground::GetCards()
    {
        return cards;
    }

    void Playground::ArrangeCards(Card* card_list) const
    {
        for (size_t i = 0; i < cards.size(); ++i)
        {
            card_list[cards[i]].SetCorner(
                glm::vec3(first_card_x + static_cast<float>(i) * card_spacing, card_y, 0));
        }
    }
}
