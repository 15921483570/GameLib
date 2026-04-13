#include "../GameLib.h"

int main()
{
    GameLib game;
    game.Open(640, 480, "My Game", true);

    int x = 320, y = 240;

    while (!game.IsClosed()) {
        if (game.IsKeyDown(KEY_LEFT))  x -= 3;
        if (game.IsKeyDown(KEY_RIGHT)) x += 3;
        if (game.IsKeyDown(KEY_UP))    y -= 3;
        if (game.IsKeyDown(KEY_DOWN))  y += 3;

        game.Clear(COLOR_BLACK);
        game.FillCircle(x, y, 15, COLOR_CYAN);
        game.DrawText(10, 10, "Up/Down/Left/Right to move!", COLOR_WHITE);
        game.Update();
        game.WaitFrame(60);
    }
    return 0;
}
