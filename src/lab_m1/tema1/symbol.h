#pragma once

#include <string>

#include "utils/glm_utils.h"

class Mesh;

namespace m1
{
    class Symbol
    {
    public:
        Symbol();
        Symbol(const glm::vec3& corner, int type);

        Mesh* CreateSymbol(const std::string& name) const;

        int GetType() const;
        const glm::vec3& GetCorner() const;

    private:
        glm::vec3 corner;
        int type;
    };
}
