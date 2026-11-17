#include "lab_m1/tema1/symbol.h"

#include <cmath>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/gl_utils.h"

namespace m1
{
    Symbol::Symbol()
        : corner(0.0f)
        , type(0)
    {
    }

    Symbol::Symbol(const glm::vec3& corner, int type)
        : corner(corner)
        , type(type)
    {
    }

    Mesh* Symbol::CreateSymbol(const std::string& name) const
    {
        constexpr float card_width = 50.0f;
        constexpr float card_height = 75.0f;
        constexpr float symbol_size = 30.0f;
        constexpr float heal_cross_thick = 6.0f;
        constexpr int circle_segments = 32;

        Mesh* symbol = new Mesh(name);
        const glm::vec3 center = corner +
            glm::vec3(card_width / 2.0f, card_height / 2.0f, 1.0f);
        const glm::vec3 color(0.65f, 0.65f, 0.65f);
        const float half_size = symbol_size / 2.0f;
        const float half_thick = heal_cross_thick / 2.0f;

        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        switch (type)
        {
            case 1:
                vertices =
                {
                    VertexFormat(center + glm::vec3(-half_size, -half_size / 2, 0), color),
                    VertexFormat(center + glm::vec3(0, half_size, 0), color),
                    VertexFormat(center + glm::vec3(half_size, -half_size / 2, 0), color)
                };
                indices = { 0, 1, 2 };
                break;

            case 2:
                vertices =
                {
                    VertexFormat(center + glm::vec3(-half_thick, -half_size, 0), color),
                    VertexFormat(center + glm::vec3(half_thick, -half_size, 0), color),
                    VertexFormat(center + glm::vec3(half_thick, half_size, 0), color),
                    VertexFormat(center + glm::vec3(-half_thick, half_size, 0), color),
                    VertexFormat(center + glm::vec3(-half_size, -half_thick, 0), color),
                    VertexFormat(center + glm::vec3(half_size, -half_thick, 0), color),
                    VertexFormat(center + glm::vec3(half_size, half_thick, 0), color),
                    VertexFormat(center + glm::vec3(-half_size, half_thick, 0), color)
                };
                indices = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7 };
                break;

            case 3:
                vertices =
                {
                    VertexFormat(center + glm::vec3(-half_size, 0, 0), color),
                    VertexFormat(center + glm::vec3(-half_size, half_size, 0), color),
                    VertexFormat(center + glm::vec3(half_size, half_size, 0), color),
                    VertexFormat(center + glm::vec3(half_size, 0, 0), color),
                    VertexFormat(center + glm::vec3(0, -half_size, 0), color)
                };
                indices = { 0, 1, 2, 0, 2, 3, 0, 3, 4 };
                break;

            case 4:
                vertices =
                {
                    VertexFormat(center + glm::vec3(-half_size, 0, 0), color),
                    VertexFormat(center + glm::vec3(0, half_size, 0), color),
                    VertexFormat(center + glm::vec3(half_size, 0, 0), color),
                    VertexFormat(center + glm::vec3(0, -half_size, 0), color)
                };
                indices = { 0, 1, 2, 0, 2, 3 };
                break;

            case 5:
                vertices =
                {
                    VertexFormat(center + glm::vec3(-half_size, -half_size, 0), color),
                    VertexFormat(center + glm::vec3(-half_size, half_size, 0), color),
                    VertexFormat(center + glm::vec3(0, half_size * 2, 0), color),
                    VertexFormat(center + glm::vec3(half_size, half_size, 0), color),
                    VertexFormat(center + glm::vec3(half_size, -half_size, 0), color),
                    VertexFormat(center + glm::vec3(0, -half_size * 2, 0), color)
                };
                indices = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5 };
                break;

            case 6:
            {
                const float radius = symbol_size / 2.0f;
                vertices.emplace_back(center, color);
                for (int i = 0; i < circle_segments; ++i)
                {
                    const float angle = 2.0f * static_cast<float>(M_PI) * i / circle_segments;
                    vertices.emplace_back(
                        center + glm::vec3(std::cos(angle) * radius, std::sin(angle) * radius, 0),
                        color);
                }
                for (int i = 0; i < circle_segments; ++i)
                {
                    const unsigned int next = (i + 1) % circle_segments;
                    indices.insert(indices.end(), { 0, static_cast<unsigned int>(i + 1), next + 1 });
                }
                break;
            }

            case 7:
                vertices =
                {
                    VertexFormat(center + glm::vec3(-half_size, 0, 0), color),
                    VertexFormat(center + glm::vec3(-half_size / 2, half_size / 2, 0), color),
                    VertexFormat(center + glm::vec3(-half_size / 2, -half_size / 2, 0), color),
                    VertexFormat(center + glm::vec3(half_size / 2, half_size / 2, 0), color),
                    VertexFormat(center + glm::vec3(half_size / 2, -half_size / 2, 0), color),
                    VertexFormat(center + glm::vec3(half_size * 1.5f, half_size * 1.5f, 0), color)
                };
                indices = { 0, 1, 2, 2, 1, 3, 2, 3, 4, 4, 3, 5 };
                break;

            case 8:
                vertices =
                {
                    VertexFormat(center + glm::vec3(0, -half_size, 0), color),
                    VertexFormat(center + glm::vec3(-half_size, -half_size / 2, 0), color),
                    VertexFormat(center + glm::vec3(-half_size, 0, 0), color),
                    VertexFormat(center + glm::vec3(-half_size / 2, half_size / 2, 0), color),
                    VertexFormat(center + glm::vec3(-half_size / 2, 0, 0), color),
                    VertexFormat(center, color),
                    VertexFormat(center + glm::vec3(0, half_size * 1.5f, 0), color),
                    VertexFormat(center + glm::vec3(half_size / 2, 0, 0), color),
                    VertexFormat(center + glm::vec3(half_size / 2, half_size, 0), color),
                    VertexFormat(center + glm::vec3(half_size, 0, 0), color),
                    VertexFormat(center + glm::vec3(half_size, -half_size / 2, 0), color)
                };
                indices = { 0, 1, 2, 0, 2, 9, 0, 9, 10, 2, 3, 5, 4, 6, 7, 5, 8, 9 };
                break;

            default:
                break;
        }

        symbol->SetDrawMode(GL_TRIANGLES);
        symbol->InitFromData(vertices, indices);
        return symbol;
    }

    int Symbol::GetType() const
    {
        return type;
    }

    const glm::vec3& Symbol::GetCorner() const
    {
        return corner;
    }
}
