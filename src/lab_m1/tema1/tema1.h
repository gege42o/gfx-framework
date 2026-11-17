#pragma once

#include <vector>

#include "components/simple_scene.h"
#include "lab_m1/tema1/card.h"
#include "lab_m1/tema1/game.h"
#include "lab_m1/tema1/playground.h"
#include "lab_m1/tema1/symbol.h"
#include "lab_m1/tema1/turn.h"


namespace m1
{
    class tema1 : public gfxc::SimpleScene
    {
     public:
        tema1();
        ~tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        float mouse_transX, mouse_transY;
        glm::ivec2 sceneResolution;
        Card cards[5];
        Card enemy_cards[5];
        std::vector<int> hand_cards;
        std::vector<int> enemy_hand_cards;
        Playground playground;
        Playground enemy_playground;
        Turn turn;
        Game game;
        std::vector<Mesh*> player_hp_boxes;
        std::vector<Mesh*> enemy_hp_boxes;
        Mesh* player_win_mesh;
        Mesh* enemy_win_mesh;
        Mesh* draw_mesh;
        int dragged_card;
        bool dragging_card;
        bool player_card_played;
        glm::vec2 drag_offset;

        // TODO(student): If you need any other class variables, define them here.

    };
} // namespace m1
