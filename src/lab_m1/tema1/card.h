#pragma once

#include <string>
#include <vector>

#include "utils/glm_utils.h"

class Mesh;

namespace m1
{
    class Card
    {
    public:
        Card();
        Card(const glm::vec3& corner, int type, int card_number, int V);

        static void ArrangeCardsInHand(Card* cards, const std::vector<int>& hand_cards);
        Mesh* CreateCard(const std::string& name, bool fill = true) const;
        Mesh* CreateCardBack(const std::string& name) const;
        Mesh* CreateValueOutline(const std::string& name) const;
        Mesh* CreateValueSegments(const std::string& name) const;
        void UpdateValueSegments(Mesh* segments) const;
        void PrintCardInfo() const;
        void SetCorner(const glm::vec3& new_corner);
        glm::vec3 GetRenderOffset() const;
        float GetMinX() const;
        float GetMaxX() const;
        float GetMinY() const;
        float GetMaxY() const;
        int GetType() const;
        int GetCardNumber() const;
        const glm::vec3& GetCorner() const;
        int GetV() const;
        void SetV(int value);

    private:
        float min_x;
        float max_x;
        float min_y;
        float max_y;
        int type;
        int card_number;
        glm::vec3 corner;
        glm::vec3 initial_corner;
        int V;
    };
}
