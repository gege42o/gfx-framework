#include "lab_m1/tema1/card.h"

#include <algorithm>
#include <cstdio>

#include "core/gpu/mesh.h"
#include "utils/gl_utils.h"

namespace m1
{
    namespace
    {
        constexpr float card_width = 50.0f;
        constexpr float card_height = 75.0f;
        constexpr float border_width = 5.0f;

        glm::vec3 GetCardColor(int type)
        {
            switch (type)
            {
                case 1: return glm::vec3(1, 0, 0);
                case 2: return glm::vec3(0, 1, 0);
                case 3: return glm::vec3(0, 0, 1);
                case 5: return glm::vec3(1, 1, 1);
                case 6: return glm::vec3(1, 1, 0);
                case 7: return glm::vec3(1, 0.5f, 0);
                case 8: return glm::vec3(1, 0, 1);
                default: return glm::vec3(0, 0, 0);
            }
        }
    }

    Card::Card()
        : min_x(0.0f)
        , max_x(0.0f)
        , min_y(0.0f)
        , max_y(0.0f)
        , type(0)
        , card_number(0)
        , corner(0.0f)
        , initial_corner(0.0f)
        , V(0)
    {
    }

    Card::Card(const glm::vec3& corner, int type, int card_number, int V)
        : min_x(corner.x)
        , max_x(corner.x + card_width)
        , min_y(corner.y)
        , max_y(corner.y + card_height)
        , type(type)
        , card_number(card_number)
        , corner(corner)
        , initial_corner(corner)
        , V(V)
    {
    }

    void Card::ArrangeCardsInHand(Card* cards, const std::vector<int>& hand_cards)
    {
        constexpr float first_x = 500.0f;
        constexpr float y = 50.0f;
        constexpr float spacing = 60.0f;

        for (size_t i = 0; i < hand_cards.size(); ++i)
        {
            cards[hand_cards[i]].SetCorner(
                glm::vec3(first_x + static_cast<float>(i) * spacing, y, 0));
        }
    }

    Mesh* Card::CreateCard(const std::string& name, bool fill) const
    {
        const glm::vec3 color = GetCardColor(type);
        const glm::vec3 border_color(1, 1, 1);
        const std::vector<VertexFormat> vertices =
        {
            VertexFormat(corner, border_color),
            VertexFormat(corner + glm::vec3(card_width, 0, 0), border_color),
            VertexFormat(corner + glm::vec3(card_width, card_height, 0), border_color),
            VertexFormat(corner + glm::vec3(0, card_height, 0), border_color),
            VertexFormat(corner + glm::vec3(border_width, border_width, 0), color),
            VertexFormat(corner + glm::vec3(card_width - border_width, border_width, 0), color),
            VertexFormat(corner + glm::vec3(card_width - border_width, card_height - border_width, 0), color),
            VertexFormat(corner + glm::vec3(border_width, card_height - border_width, 0), color)
        };

        Mesh* card = new Mesh(name);
        std::vector<unsigned int> indices;

        if (!fill)
        {
            indices = { 0, 1, 2, 3 };
            card->SetDrawMode(GL_LINE_LOOP);
        }
        else
        {
            indices =
            {
                0, 1, 5, 0, 5, 4,
                1, 2, 6, 1, 6, 5,
                2, 3, 7, 2, 7, 6,
                3, 0, 4, 3, 4, 7,
                4, 5, 6, 4, 6, 7
            };
            card->SetDrawMode(GL_TRIANGLES);
        }

        card->InitFromData(vertices, indices);
        return card;
    }

    Mesh* Card::CreateCardBack(const std::string& name) const
    {
        const glm::vec3 border_color(1, 1, 1);
        const glm::vec3 back_color(0.1f, 0.2f, 0.55f);
        const std::vector<VertexFormat> vertices =
        {
            VertexFormat(corner, border_color),
            VertexFormat(corner + glm::vec3(card_width, 0, 0), border_color),
            VertexFormat(corner + glm::vec3(card_width, card_height, 0), border_color),
            VertexFormat(corner + glm::vec3(0, card_height, 0), border_color),
            VertexFormat(corner + glm::vec3(border_width, border_width, 0), back_color),
            VertexFormat(corner + glm::vec3(card_width - border_width, border_width, 0), back_color),
            VertexFormat(corner + glm::vec3(card_width - border_width, card_height - border_width, 0), back_color),
            VertexFormat(corner + glm::vec3(border_width, card_height - border_width, 0), back_color)
        };

        Mesh* card_back = new Mesh(name);
        card_back->SetDrawMode(GL_TRIANGLES);
        card_back->InitFromData(
            vertices,
            {
                0, 1, 5, 0, 5, 4,
                1, 2, 6, 1, 6, 5,
                2, 3, 7, 2, 7, 6,
                3, 0, 4, 3, 4, 7,
                4, 5, 6, 4, 6, 7
            });
        return card_back;
    }

    Mesh* Card::CreateValueOutline(const std::string& name) const
    {
        constexpr float center_offset = 13.0f;
        constexpr float radius = 9.0f;
        const glm::vec3 center = initial_corner +
            glm::vec3(center_offset, center_offset, 2.0f);
        const glm::vec3 color(0, 0, 0);

        Mesh* outline = new Mesh(name);
        outline->SetDrawMode(GL_LINE_LOOP);
        outline->InitFromData(
            {
                VertexFormat(center + glm::vec3(0, radius, 0), color),
                VertexFormat(center + glm::vec3(radius, 0, 0), color),
                VertexFormat(center + glm::vec3(0, -radius, 0), color),
                VertexFormat(center + glm::vec3(-radius, 0, 0), color)
            },
            { 0, 1, 2, 3 });
        return outline;
    }

    Mesh* Card::CreateValueSegments(const std::string& name) const
    {
        Mesh* segments = new Mesh(name);
        UpdateValueSegments(segments);
        return segments;
    }

    void Card::UpdateValueSegments(Mesh* segments) const
    {
        constexpr float center_offset = 13.0f;
        constexpr float radius = 8.0f;
        const glm::vec3 center = initial_corner +
            glm::vec3(center_offset, center_offset, 2.0f);
        const glm::vec3 active_color(0, 1, 0);
        const glm::vec3 inactive_color(1, 1, 1);
        const glm::vec3 points[] =
        {
            center + glm::vec3(0, radius, 0),
            center + glm::vec3(radius, 0, 0),
            center + glm::vec3(0, -radius, 0),
            center + glm::vec3(-radius, 0, 0)
        };

        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;
        for (int i = 0; i < 4; ++i)
        {
            const glm::vec3 color = i < V ? active_color : inactive_color;
            vertices.emplace_back(center, color);
            vertices.emplace_back(points[i], color);
            vertices.emplace_back(points[(i + 1) % 4], color);
            const unsigned int base = static_cast<unsigned int>(i * 3);
            indices.insert(indices.end(), { base, base + 1, base + 2 });
        }

        segments->SetDrawMode(GL_TRIANGLES);
        segments->InitFromData(vertices, indices);
    }

    void Card::PrintCardInfo() const
    {
        std::printf(
            "card info: min_x:%.1f, max_x:%.1f, min_y:%.1f, max_y:%.1f, card_number:%d\n",
            min_x,
            max_x,
            min_y,
            max_y,
            card_number);
    }

    void Card::SetCorner(const glm::vec3& new_corner)
    {
        corner = new_corner;
        min_x = corner.x;
        max_x = corner.x + card_width;
        min_y = corner.y;
        max_y = corner.y + card_height;
    }

    glm::vec3 Card::GetRenderOffset() const
    {
        return corner - initial_corner;
    }

    float Card::GetMinX() const
    {
        return min_x;
    }

    float Card::GetMaxX() const
    {
        return max_x;
    }

    float Card::GetMinY() const
    {
        return min_y;
    }

    float Card::GetMaxY() const
    {
        return max_y;
    }

    int Card::GetType() const
    {
        return type;
    }

    int Card::GetCardNumber() const
    {
        return card_number;
    }

    const glm::vec3& Card::GetCorner() const
    {
        return corner;
    }

    int Card::GetV() const
    {
        return V;
    }

    void Card::SetV(int value)
    {
        V = std::max(0, value);
    }
}
