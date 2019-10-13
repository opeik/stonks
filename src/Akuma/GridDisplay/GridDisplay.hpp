#pragma once

#include <glm/vec2.hpp>

#include "Akuma/Pathing/Grid.hpp"
#include "Akuma/Pathing/Pathfinding.hpp"
#include "Akuma/ProceduralGen/BSP/BSP.hpp"

using BSP::BSPTree;
using Pathing::Grid;
using Pathing::Node;

namespace GridDisplay {

    auto drawRectangle(float width, float height, bool wireframe) -> void;
    auto drawSquare(float size, bool wireframe) -> void;
    auto displayGrid(Grid &grid) -> void;
    auto displayPath(std::vector<Node *> path, Grid &grid) -> void;
    auto setGridSquare(glm::ivec2 bottomLeft, glm::ivec2 topRight, Grid &grid,
                       bool walkable) -> void;
    auto loadBSP(Pathing::Grid &grid, BSP::BSPTree &tree) -> void;

};
