#include "GLDisplay.hpp"

#include <iostream>

#include <SDL2/SDL.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

using View::GLDisplay;
using namespace GridDisplay;
using namespace Pathing;

GLDisplay::GLDisplay() {

    auto &engine = Stonk::Engine::get();

    path = Pathfinding::findPath(testGrid, testGrid.getStartNode(),
                                 testGrid.getEndNode(), 1);

    SDL_GL_GetDrawableSize(engine.window.get(), &width, &height);
    GLDisplay::ratio = static_cast<double>(width) / static_cast<double>(height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(60, GLDisplay::ratio, 1, 50000);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.f, 0.f, 0.f, 1.f);

    gluLookAt(camera.position.x, camera.position.y, camera.position.z,
              camera.look.x, camera.look.z, camera.look.z, camera.up.x,
              camera.up.y, camera.up.z);

    glm::vec2 size = {testGrid.gridSizeX - 1, testGrid.gridSizeY - 1};
    BSPTree tree   = BSPTree(size, 3);
    loadBSP(testGrid, tree);

    /*std::vector<std::vector<Node *>> paths;
    Grid blankGrid  = Grid(40, 40);

    if (!roomList.empty()) {
        auto startPoint = roomList[0]->getCentrePoint();
        for (auto n : roomList) {
            auto endPoint  = n->getCentrePoint();
            auto startNode = blankGrid.getNode(startPoint);
            auto endNode   = blankGrid.getNode(endPoint);
            paths.push_back(Pathfinding::findPath(blankGrid, *startNode, *endNode));
        }
    }

     for (auto n : paths) {
         for (auto i : n) {
             if (n.size() > 0) {
                 glm::vec2 pos = {i->x, i->y};
                 testGrid.getNode(pos)->setWalkable(1);
             }
         }
     }*/
    path = Pathfinding::findPath(testGrid, testGrid.getStartNode(),
                                 testGrid.getEndNode(), 1);
}

auto GLDisplay::display() -> void {
    auto &engine = Stonk::Engine::get();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glPushMatrix();
    glTranslatef(gridTranslation.x, gridTranslation.y, gridTranslation.z);
    displayGrid(testGrid);
    displayPath(path, testGrid);
    glPopMatrix();

    glDisable(GL_DEPTH_TEST);
    SDL_GL_SwapWindow(engine.window.get());
}

auto View::GLDisplay::handleKeyPress(SDL_Event &event) -> void {

    switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_W: {
            testGrid.selected[1] =
                ((testGrid.selected[1] + 1) % testGrid.gridSizeY);

        } break;
        case SDL_SCANCODE_S: {
            testGrid.selected[1] =
                ((testGrid.selected[1] - 1) % testGrid.gridSizeY);

            testGrid.selected[1] = testGrid.gridSizeY - 1;

        } break;
        case SDL_SCANCODE_A: {
            testGrid.selected[0] =
                ((testGrid.selected[0] - 1) % testGrid.gridSizeX);

            testGrid.selected[0] = testGrid.gridSizeX - 1;

        } break;
        case SDL_SCANCODE_D: {
            testGrid.selected[0] =
                ((testGrid.selected[0] + 1) % testGrid.gridSizeX);
        } break;
        case SDL_SCANCODE_SPACE: {
            testGrid.getSelectedNode().toggleWalkable();
            path = Pathfinding::findPath(testGrid, testGrid.getStartNode(),
                                         testGrid.getEndNode(), 1);
        } break;
        case SDL_SCANCODE_UP: {
            gridTranslation.y -= 1;
        } break;
        case SDL_SCANCODE_DOWN: {
            gridTranslation.y += 1;
        } break;
        case SDL_SCANCODE_LEFT: {
            gridTranslation.x += 1;
        } break;
        case SDL_SCANCODE_RIGHT: {
            gridTranslation.x -= 1;
        } break;
        case SDL_SCANCODE_Z: {
            testGrid.pathStart[0] = testGrid.selected[0];
            testGrid.pathStart[1] = testGrid.selected[1];
            path = Pathfinding::findPath(testGrid, testGrid.getStartNode(),
                                         testGrid.getEndNode(), 1);
        } break;
        case SDL_SCANCODE_X: {
            testGrid.pathEnd[0] = testGrid.selected[0];
            testGrid.pathEnd[1] = testGrid.selected[1];
            path = Pathfinding::findPath(testGrid, testGrid.getStartNode(),
                                         testGrid.getEndNode(), 1);
        } break;
        case SDL_SCANCODE_B: {
            glm::vec2 size = {testGrid.gridSizeX - 1, testGrid.gridSizeY - 1};
            BSPTree tree   = BSPTree(size, 3);
            loadBSP(testGrid, tree);
            auto roomList  = tree.getRooms();
            auto startNode = *roomList.begin();
            auto endNode   = *--roomList.end();

            testGrid.pathStart[0] =
                testGrid.getNode(startNode->getCentrePoint())->x;
            testGrid.pathStart[1] =
                testGrid.getNode(startNode->getCentrePoint())->y;
            testGrid.pathEnd[0] = testGrid.getNode(endNode->getCentrePoint())->x;
            testGrid.pathEnd[1] = testGrid.getNode(endNode->getCentrePoint())->y;
            path = Pathfinding::findPath(testGrid, testGrid.getStartNode(),
                                         testGrid.getEndNode(), 1);

        } break;

        default: break;
    }
}

auto View::GLDisplay::handleMouseWheel(SDL_Event &event) -> void {
    int amountScrolledY = event.wheel.y; // Amount scrolled up or down
    if (gridTranslation.z < -1 || amountScrolledY < 0) {
        gridTranslation.z += amountScrolledY;
    }
}

// auto GLDisplay::update(double dt) -> void {
//    // dt == delta time
//}

auto GLDisplay::get() -> GLDisplay & {
    static auto instance = GLDisplay{};

    return instance;
}

auto View::GLDisplay::updateCamera() -> void {
    gluLookAt(camera.position.x, camera.position.y, camera.position.z,
              camera.look.x, camera.look.y, camera.look.z, camera.up.x,
              camera.up.y, camera.up.z);
}
