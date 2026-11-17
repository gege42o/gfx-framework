#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"
#include "core/gpu/mesh.h"
#include <random>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


tema1::tema1()
    : playground(355.0f, 935.0f, 135.0f, 330.0f, 500.0f, 195.0f, 60.0f)
    , enemy_playground(355.0f, 935.0f, 345.0f, 540.0f, 500.0f, 420.0f, 60.0f)
    , turn()
    , dragged_card(-1)
    , dragging_card(false)
    , drag_offset(0.0f)
{
}


tema1::~tema1()
{
}

int getRandomIndex(int min_index, int max_index) {
    static std::random_device rd;   
    static std::mt19937 gen(rd());  
    
    std::uniform_int_distribution<int> distrib(min_index, max_index);
    
    return distrib(gen);
}

void tema1::Init()
{
    sceneResolution = window->GetResolution();
    glm::ivec2 resolution = sceneResolution;
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    int cx = (corner[0]+(squareSide)/2);
    int cy = (corner[1]+(squareSide)/2);
    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;
    hand_cards.clear();
    enemy_hand_cards.clear();
    game = Game();
    playground.Clear();
    enemy_playground.Clear();
    dragged_card = -1;
    dragging_card = false;
    player_card_played = false;
    int startx=500;
    for (int i = 1; i <= 5; i++) {
        std::string card_name = "card" + std::to_string(i);
        std::string symbol_name = "symbol" + std::to_string(i);
        std::string back_name = "enemy_card_back" + std::to_string(i);
        std::string enemy_card_name = "enemy_card" + std::to_string(i);
        std::string enemy_symbol_name = "enemy_symbol" + std::to_string(i);
        std::string value_outline_name = "value_outline" + std::to_string(i);
        std::string value_segments_name = "value_segments" + std::to_string(i);
        std::string enemy_value_outline_name = "enemy_value_outline" + std::to_string(i);
        std::string enemy_value_segments_name = "enemy_value_segments" + std::to_string(i);

        int type= getRandomIndex(1,8);
        int type_enemy = getRandomIndex(1,8);
        cards[i - 1] = Card(
            glm::vec3(startx, 50, 0),
            type,
            i, //card_number
            getRandomIndex(1, 4) //V
        );

        AddMeshToList(cards[i - 1].CreateCard(card_name));

        Symbol symbol(cards[i - 1].GetCorner(), cards[i - 1].GetType());
        AddMeshToList(symbol.CreateSymbol(symbol_name));
        AddMeshToList(cards[i - 1].CreateValueOutline(value_outline_name));
        AddMeshToList(cards[i - 1].CreateValueSegments(value_segments_name));

        //enemy cards gen

        enemy_cards[i - 1] = Card(
            glm::vec3(startx, 550, 0),
            type_enemy,
            i, //card_number
            getRandomIndex(1, 4) //V
        );
        AddMeshToList(enemy_cards[i - 1].CreateCardBack(back_name));
        AddMeshToList(enemy_cards[i - 1].CreateCard(enemy_card_name));
        Symbol enemy_symbol(enemy_cards[i - 1].GetCorner(), enemy_cards[i - 1].GetType());
        AddMeshToList(enemy_symbol.CreateSymbol(enemy_symbol_name));
        AddMeshToList(enemy_cards[i - 1].CreateValueOutline(enemy_value_outline_name));
        AddMeshToList(enemy_cards[i - 1].CreateValueSegments(enemy_value_segments_name));
        //DEBUG
        printf("created card number:%d\n", cards[i - 1].GetCardNumber());
        cards[i - 1].PrintCardInfo();
        hand_cards.push_back(i - 1);

        printf("created enemy_card number:%d\n", cards[i - 1].GetCardNumber());
        enemy_cards[i - 1].PrintCardInfo();
        enemy_hand_cards.push_back(i - 1);
        
        startx+=60;
    }

    Mesh* playground = tema1_object2D::CreateRectangle("playground", glm::vec3(200,0,0),900,650,glm::vec3(0, 0, 1));
    AddMeshToList(playground);
    Mesh* playground_enemy = tema1_object2D::CreateRectangle("playground_enemy", glm::vec3(355,345,0),580,195,glm::vec3(1, 0, 0));
    AddMeshToList(playground_enemy);
    Mesh* playground_player = tema1_object2D::CreateRectangle("playground_player", glm::vec3(355,135,0),580,195,glm::vec3(0, 1, 0));
    AddMeshToList(playground_player);
    Mesh* deck_area = tema1_object2D::CreateRectangle("deck_area", glm::vec3(470, 45, 0), 350, 85, glm::vec3(1, 1, 0), false);
    AddMeshToList(deck_area);
    Mesh* turn_button = turn.CreateButton("turn_button");
    AddMeshToList(turn_button);
    game.DrawPlayerHP(300.0f, 85.0f, 20.0f, player_hp_boxes);
    for (Mesh* hp_box : player_hp_boxes)
    {
        AddMeshToList(hp_box);
    }
    game.DrawEnemyHP(300.0f, 550.0f, 20.0f, enemy_hp_boxes);
    for (Mesh* hp_box : enemy_hp_boxes)
    {
        AddMeshToList(hp_box);
    }

    player_win_mesh = tema1_object2D::CreateRectangle(
        "player_win", glm::vec3(resolution.x / 2.0f - 120.0f,
        resolution.y / 2.0f - 120.0f, 0), 240.0f, 240.0f,
        glm::vec3(0, 1, 0), true);
    AddMeshToList(player_win_mesh);

    enemy_win_mesh = new Mesh("enemy_win");
    enemy_win_mesh->InitFromData(
        {
            VertexFormat(glm::vec3(resolution.x / 2.0f, resolution.y / 2.0f + 140.0f, 0),
                glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(resolution.x / 2.0f - 140.0f, resolution.y / 2.0f - 100.0f, 0),
                glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(resolution.x / 2.0f + 140.0f, resolution.y / 2.0f - 100.0f, 0),
                glm::vec3(1, 0, 0))
        },
        { 0, 1, 2 });
    enemy_win_mesh->SetDrawMode(GL_TRIANGLES);
    AddMeshToList(enemy_win_mesh);

    draw_mesh = new Mesh("draw");
    const glm::vec3 draw_center(resolution.x / 2.0f, resolution.y / 2.0f, 0);
    constexpr int draw_segments = 64;
    constexpr float draw_radius = 120.0f;
    constexpr float pi = 3.14159265358979323846f;
    std::vector<VertexFormat> draw_vertices;
    std::vector<unsigned int> draw_indices;
    draw_vertices.emplace_back(draw_center, glm::vec3(1, 1, 0));
    for (int i = 0; i <= draw_segments; ++i)
    {
        const float angle = 2.0f * pi * i / draw_segments;
        draw_vertices.emplace_back(
            draw_center + glm::vec3(
                draw_radius * std::cos(angle),
                draw_radius * std::sin(angle),
                0),
            glm::vec3(1, 1, 0));
    }
    for (int i = 1; i <= draw_segments; ++i)
    {
        draw_indices.push_back(0);
        draw_indices.push_back(i);
        draw_indices.push_back(i + 1);
    }
    draw_mesh->InitFromData(draw_vertices, draw_indices);
    draw_mesh->SetDrawMode(GL_TRIANGLES);
    AddMeshToList(draw_mesh);

}


void tema1::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    RenderMesh2D(meshes["playground"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["playground_enemy"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["playground_player"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["deck_area"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["turn_button"], shaders["VertexColor"], glm::mat3(1));
    game.DrawPlayerHP(300.0f, 85.0f, 20.0f, player_hp_boxes);
    for (Mesh* hp_box : player_hp_boxes)
    {
        RenderMesh2D(hp_box, shaders["VertexColor"], glm::mat3(1));
    }
    game.DrawEnemyHP(300.0f, 550.0f, 20.0f, enemy_hp_boxes);
    for (Mesh* hp_box : enemy_hp_boxes)
    {
        RenderMesh2D(hp_box, shaders["VertexColor"], glm::mat3(1));
    }

    for (int i = 1; i <= 5; i++)
    {
        std::string card_name = "card" + std::to_string(i);
        std::string symbol_name = "symbol" + std::to_string(i);
        std::string value_outline_name = "value_outline" + std::to_string(i);
        std::string value_segments_name = "value_segments" + std::to_string(i);
        if (std::find(hand_cards.begin(), hand_cards.end(), i - 1) != hand_cards.end() ||
            playground.HasCard(i - 1))
        {
            cards[i - 1].UpdateValueSegments(meshes[value_segments_name]);
            const glm::vec3 offset = cards[i - 1].GetRenderOffset();
            modelMatrix = transform2D::Translate(offset.x, offset.y);
            RenderMesh2D(meshes[card_name], shaders["VertexColor"], modelMatrix);
            RenderMesh2D(meshes[symbol_name], shaders["VertexColor"], modelMatrix);
            RenderMesh2D(meshes[value_segments_name], shaders["VertexColor"], modelMatrix);
            RenderMesh2D(meshes[value_outline_name], shaders["VertexColor"], modelMatrix);
        }
        std::string enemy_back_name = "enemy_card_back" + std::to_string(i);
        std::string enemy_card_name = "enemy_card" + std::to_string(i);
        std::string enemy_symbol_name = "enemy_symbol" + std::to_string(i);
        std::string enemy_value_outline_name = "enemy_value_outline" + std::to_string(i);
        std::string enemy_value_segments_name = "enemy_value_segments" + std::to_string(i);
        if (std::find(enemy_hand_cards.begin(), enemy_hand_cards.end(), i - 1) !=
                enemy_hand_cards.end() ||
            enemy_playground.HasCard(i - 1))
        {
            const glm::vec3 enemy_offset = enemy_cards[i - 1].GetRenderOffset();
            modelMatrix = transform2D::Translate(enemy_offset.x, enemy_offset.y);
            if (enemy_playground.HasCard(i - 1))
            {
                enemy_cards[i - 1].UpdateValueSegments(meshes[enemy_value_segments_name]);
                RenderMesh2D(meshes[enemy_card_name], shaders["VertexColor"], modelMatrix);
                RenderMesh2D(meshes[enemy_symbol_name], shaders["VertexColor"], modelMatrix);
                RenderMesh2D(meshes[enemy_value_segments_name], shaders["VertexColor"], modelMatrix);
                RenderMesh2D(meshes[enemy_value_outline_name], shaders["VertexColor"], modelMatrix);
            }
            else
            {
                RenderMesh2D(meshes[enemy_back_name], shaders["VertexColor"], modelMatrix);
            }
        }
    }

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    if (game.GetResult() == GameResult::PlayerWon)
    {
        RenderMesh2D(player_win_mesh, shaders["VertexColor"], glm::mat3(1));
    }
    else if (game.GetResult() == GameResult::EnemyWon)
    {
        RenderMesh2D(enemy_win_mesh, shaders["VertexColor"], glm::mat3(1));
    }
    else if (game.GetResult() == GameResult::Draw)
    {
        RenderMesh2D(draw_mesh, shaders["VertexColor"], glm::mat3(1));
    }

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    
}


void tema1::Update(float deltaTimeSeconds)
{
    /*
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(150, 250);
    //int cx = (150+250)/2;
    int cx = 50;
    int cy=50;
    //int cy = (150+250)/2;
    
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Rotate(angularStep);
    modelMatrix *= transform2D::Translate(-cx, -cy);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented.
    // Remember, the last matrix in the chain will take effect first!

    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(400, 250);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Scale(cos(angularStep)*3,sin(angularStep)*3);
    modelMatrix *= transform2D::Translate(-cx, -cy);

    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(650, 250);
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Translate(-cos(angularStep)*30,sin(angularStep)*30);
    modelMatrix *= transform2D::Translate(-cx, -cy);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!
    angularStep += deltaTimeSeconds;
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    
*/
    modelMatrix = glm::mat3(1);

    
}


void tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (game.IsFinished())
    {
        return;
    }

    if (!dragging_card)
    {
        return;
    }

    const glm::ivec2 windowResolution = window->GetResolution(true);
    const float renderMouseX = mouseX * sceneResolution.x / float(windowResolution.x);
    const float renderMouseY = sceneResolution.y -
        mouseY * sceneResolution.y / float(windowResolution.y);

    cards[dragged_card].SetCorner(
        glm::vec3(renderMouseX - drag_offset.x, renderMouseY - drag_offset.y, 0));
}


void tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (game.IsFinished())
    {
        return;
    }

    if (!IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) || dragging_card)
    {
        return;
    }

    const glm::ivec2 windowResolution = window->GetResolution(true);

    const float renderMouseX =
        mouseX * sceneResolution.x /
        float((windowResolution.x));

    const float renderMouseY =
        sceneResolution.y -
        mouseY * sceneResolution.y /
        float((windowResolution.y));

    for (auto it = hand_cards.rbegin(); it != hand_cards.rend(); ++it)
    {
        if (player_card_played)
        {
            break;
        }

        Card& card = cards[*it];
        if (card.GetMinX() <= renderMouseX && renderMouseX <= card.GetMaxX() &&
            card.GetMinY() <= renderMouseY && renderMouseY <= card.GetMaxY())
        {
            dragged_card = *it;
            dragging_card = true;
            drag_offset = glm::vec2(
                renderMouseX - card.GetMinX(),
                renderMouseY - card.GetMinY());
            return;
        }
    }

    const std::vector<int>& playground_cards = playground.GetCards();
    for (auto it = playground_cards.rbegin(); it != playground_cards.rend(); ++it)
    {
        Card& card = cards[*it];
        if (!playground.IsCardLocked(*it) &&
            card.GetMinX() <= renderMouseX && renderMouseX <= card.GetMaxX() &&
            card.GetMinY() <= renderMouseY && renderMouseY <= card.GetMaxY())
        {
            dragged_card = *it;
            dragging_card = true;
            drag_offset = glm::vec2(
                renderMouseX - card.GetMinX(),
                renderMouseY - card.GetMinY());
            return;
        }
    }
}


void tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (game.IsFinished())
    {
        return;
    }

    if (!IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT))
    {
        return;
    }

    const glm::ivec2 windowResolution = window->GetResolution(true);
    const float renderMouseX = mouseX * sceneResolution.x / float(windowResolution.x);
    const float renderMouseY = sceneResolution.y -
        mouseY * sceneResolution.y / float(windowResolution.y);

    if (!dragging_card &&
        turn.HandleClick(renderMouseX, renderMouseY, meshes["turn_button"]))
    {
        turn.PlayEnemyTurn(
            enemy_hand_cards,
            enemy_playground,
            enemy_cards,
            meshes["turn_button"]);
        game.ApplyTurnEffects(
            playground,
            cards,
            hand_cards,
            enemy_playground,
            enemy_cards,
            enemy_hand_cards);
        game.EvaluateEndgame(
            playground,
            hand_cards,
            enemy_playground,
            enemy_hand_cards);
        playground.ArrangeCards(cards);
        enemy_playground.ArrangeCards(enemy_cards);
        playground.LockCards();
        enemy_playground.LockCards();
        Card::ArrangeCardsInHand(cards, hand_cards);
        player_card_played = false;
        return;
    }

    if (!dragging_card)
    {
        return;
    }

    hand_cards.erase(
        std::remove(hand_cards.begin(), hand_cards.end(), dragged_card),
        hand_cards.end());
    playground.RemoveCard(dragged_card);
    playground.ArrangeCards(cards);

    if (playground.Contains(renderMouseX, renderMouseY))
    {
        playground.AddCard(dragged_card);
        player_card_played = true;
        playground.ArrangeCards(cards);
        Card::ArrangeCardsInHand(cards, hand_cards);
    }
    else
    {
        hand_cards.push_back(dragged_card);
        Card::ArrangeCardsInHand(cards, hand_cards);
    }

    dragged_card = -1;
    dragging_card = false;
}


void tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema1::OnWindowResize(int width, int height)
{
}
