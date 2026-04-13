#include "../GameLib.h"

int main()
{
    GameLib game;
    game.Open(800, 600, "Starfield", true);

    // Éú³É 200 ¿ÅÐÇÐÇ
    int sx[200], sy[200], speed[200];
    uint32_t colors[] = {COLOR_WHITE, COLOR_LIGHT_GRAY, COLOR_YELLOW, COLOR_CYAN};
    for (int i = 0; i < 200; i++) {
        sx[i] = GameLib::Random(0, 799);
        sy[i] = GameLib::Random(0, 599);
        speed[i] = GameLib::Random(1, 5);
    }

    while (!game.IsClosed()) {
        game.Clear(COLOR_BLACK);

        for (int i = 0; i < 200; i++) {
            sx[i] -= speed[i];
            if (sx[i] < 0) {
                sx[i] = 800;
                sy[i] = GameLib::Random(0, 599);
            }
            game.SetPixel(sx[i], sy[i], colors[speed[i] % 4]);
        }

        game.DrawText(250, 290, "Press ESC to exit", COLOR_GRAY);
        if (game.IsKeyPressed(KEY_ESCAPE)) break;

        game.Update();
        game.WaitFrame(60);
    }
    return 0;
}
