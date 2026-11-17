#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    sunTranslateX = 0.0f;
    earthOrbitAngle = 0.0f;
    moonOrbitAngle = 0.0f;
    earthRotationAngle = 0.0f;
    moonRotationAngle = 0.0f;
    sunRotationAngle = 0.0f;

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);
}

void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Lab4::RenderScene() {

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);


    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(sunTranslateX, 0.0f, -1.5f);
    modelMatrix *= transform3D::Scale(0.6f, 0.6f, 0.6f);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    float orbitRadius = 2.5f;
    float earthX = sunTranslateX + orbitRadius * cos(earthOrbitAngle);
    float earthZ = orbitRadius * sin(earthOrbitAngle) - 1.0f;
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(earthX, 0.0f, earthZ);
    modelMatrix *= transform3D::Scale(0.3f, 0.3f, 0.3f);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
    float moonOrbitRadius = 0.8f;
    float moonX = earthX + moonOrbitRadius * cos(moonOrbitAngle);
    float moonZ = earthZ + moonOrbitRadius * sin(moonOrbitAngle);
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(moonX, 0.0f, moonZ);
    modelMatrix *= transform3D::Scale(0.15f, 0.15f, 0.15f);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    RenderScene();
    DrawCoordinateSystem();

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    RenderScene();
    DrawCoordinateSystem();
}

void Lab4::FrameEnd()
{
}


/*
 *  These are callback functions. To find out more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    const float STEP = 0.001f;

    // cub1 
    if (window->KeyHold(GLFW_KEY_A))
        translateX -= STEP;
    if (window->KeyHold(GLFW_KEY_D))
        translateX += STEP;
    if (window->KeyHold(GLFW_KEY_W))
        translateY += STEP;
    if (window->KeyHold(GLFW_KEY_S))
        translateY -= STEP;
    if (window->KeyHold(GLFW_KEY_R))
        translateZ += STEP;
    if (window->KeyHold(GLFW_KEY_F))
        translateZ -= STEP;
    // cub2 
    const float SCALE_STEP = 0.1f;
    if (window->KeyHold(GLFW_KEY_1)) {
        scaleX += SCALE_STEP;
        scaleY += SCALE_STEP;
        scaleZ += SCALE_STEP;
    }
    if (window->KeyHold(GLFW_KEY_2)) {
        scaleX -= SCALE_STEP;
        scaleY -= SCALE_STEP;
        scaleZ -= SCALE_STEP;
    }
    if (scaleX < 0.1f) scaleX = 0.1f;
    if (scaleY < 0.1f) scaleY = 0.1f;
    if (scaleZ < 0.1f) scaleZ = 0.1f;

    // cub3 
    const float ROTATION_STEP = 0.01f;
    if (window->KeyHold(GLFW_KEY_3))
        angularStepOX += ROTATION_STEP;
    if (window->KeyHold(GLFW_KEY_4))
        angularStepOX -= ROTATION_STEP;
    if (window->KeyHold(GLFW_KEY_5))
        angularStepOY += ROTATION_STEP;
    if (window->KeyHold(GLFW_KEY_6))
        angularStepOY -= ROTATION_STEP;
    if (window->KeyHold(GLFW_KEY_7))
        angularStepOZ += ROTATION_STEP;
    if (window->KeyHold(GLFW_KEY_8))
        angularStepOZ -= ROTATION_STEP;

    static float sunTime = 0.0f;
    const float SUN_SPEED = 1.5f;
    sunTime += deltaTime;
    sunTranslateX = 3.0f * sin(SUN_SPEED * sunTime);
    const float EARTH_ORBIT_SPEED = 0.5f;
    earthOrbitAngle += EARTH_ORBIT_SPEED * deltaTime;
    const float MOON_ORBIT_SPEED = 2.0f;
    moonOrbitAngle += MOON_ORBIT_SPEED * deltaTime;
}


void Lab4::OnKeyPress(int key, int mods)
{
    switch (key) {
        case GLFW_KEY_SPACE:
            switch (polygonMode) {
                case GL_POINT: polygonMode = GL_FILL; break;
                case GL_LINE:  polygonMode = GL_POINT; break;
                default:       polygonMode = GL_LINE;  break;
            }
            break;
        case GLFW_KEY_I: miniViewportArea.y += 5; break;
        case GLFW_KEY_J: miniViewportArea.x -= 5; break;
        case GLFW_KEY_K: miniViewportArea.y -= 5; break;
        case GLFW_KEY_L: miniViewportArea.x += 5; break;
        case GLFW_KEY_U:
            if (miniViewportArea.width > 30 && miniViewportArea.height > 30) {
                miniViewportArea.x += (int)(miniViewportArea.width * 0.05f);
                miniViewportArea.y += (int)(miniViewportArea.height * 0.05f);
                miniViewportArea.width = (int)(miniViewportArea.width * 0.95f);
                miniViewportArea.height = (int)(miniViewportArea.height * 0.95f);
            }
            break;
        case GLFW_KEY_O:
            if (miniViewportArea.width < (int)(window->GetResolution().x * 0.8f)) {
                miniViewportArea.width = (int)(miniViewportArea.width * 1.05f);
                miniViewportArea.height = (int)(miniViewportArea.height * 1.05f);
            }
            break;
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
