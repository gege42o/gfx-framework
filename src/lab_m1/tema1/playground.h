#pragma once

#include <set>
#include <vector>

#include "lab_m1/tema1/card.h"

namespace m1
{
    class Playground
    {
    public:
        Playground(
            float min_x,
            float max_x,
            float min_y,
            float max_y,
            float first_card_x,
            float card_y,
            float card_spacing);

        bool Contains(float x, float y) const;
        void Clear();
        void AddCard(int card_index);
        void RemoveCard(int card_index);
        bool HasCard(int card_index) const;
        void LockCards();
        bool IsCardLocked(int card_index) const;
        const std::vector<int>& GetCards() const;
        std::vector<int>& GetCards();
        void ArrangeCards(Card* cards) const;

    private:
        float min_x;
        float max_x;
        float min_y;
        float max_y;
        float first_card_x;
        float card_y;
        float card_spacing;
        std::vector<int> cards;
        std::set<int> locked_cards;
    };
}
