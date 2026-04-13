#if defined(_WIN32) && !defined(GAMELIB_SDL_DEFAULT_FONT)
#define GAMELIB_SDL_DEFAULT_FONT "C:/Windows/Fonts/msyh.ttc"
#endif

#include "../GameLib.SDL.h"

int main()
{
    GameLib game;
    if (game.Open(800, 600, "SDL Font Text Rendering Test", true) != 0) {
        return 1;
    }
    game.ShowFps(true);

    int frame = 0;

    while (!game.IsClosed()) {
        game.Clear(COLOR_BLACK);
        frame++;

        int y = 10;

#if GAMELIB_SDL_HAS_TTF
        game.DrawText(10, y, "SDL_ttf support: ON", COLOR_GREEN);
#else
        game.DrawText(10, y, "SDL_ttf support: OFF", COLOR_RED);
#endif
        y += 18;

        game.DrawText(10, y, "1. DrawTextFont - default font / fallback chain:", COLOR_WHITE);
        y += 16;
        game.DrawTextFont(20, y, "Hello, World!", COLOR_GREEN, 20);
        y += 26;
        game.DrawTextFont(20, y, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", COLOR_CYAN, 16);
        y += 22;
        game.DrawTextFont(20, y, "abcdefghijklmnopqrstuvwxyz 0123456789", COLOR_CYAN, 16);
        y += 26;

        game.DrawText(10, y, "2. Chinese text:", COLOR_WHITE);
        y += 16;
        game.DrawTextFont(20, y, "Font \xE4\xB8\xAD\xE6\x96\x87\xE6\xB8\xB2\xE6\x9F\x93\xE6\xB5\x8B\xE8\xAF\x95", COLOR_YELLOW, 24);
        y += 32;

        game.DrawText(10, y, "3. Font sizes (12, 16, 20, 24, 32):", COLOR_WHITE);
        y += 16;
        game.DrawTextFont(20, y, "Size 12", COLOR_GOLD, 12);
        int w12 = game.GetTextWidthFont("Size 12", "Microsoft YaHei", 12);
        game.DrawTextFont(20 + w12 + 10, y, "Size 16", COLOR_GOLD, 16);
        y += 20;
        game.DrawTextFont(20, y, "Size 20", COLOR_GOLD, 20);
        y += 24;
        game.DrawTextFont(20, y, "Size 24", COLOR_GOLD, 24);
        y += 30;
        game.DrawTextFont(20, y, "Size 32", COLOR_GOLD, 32);
        y += 40;

        game.DrawText(10, y, "4. Font families:", COLOR_WHITE);
        y += 16;
        game.DrawTextFont(20, y, "Arial Font", COLOR_GREEN, "Arial", 20);
        y += 26;
        game.DrawTextFont(20, y, "Times New Roman", COLOR_SKY_BLUE, "Times New Roman", 20);
        y += 26;
        game.DrawTextFont(20, y, "Courier New (monospace)", COLOR_PINK, "Courier New", 20);
        y += 26;
        game.DrawTextFont(20, y, "SimHei \xE9\xBB\x91\xE4\xBD\x93", COLOR_ORANGE, "SimHei", 20);
        y += 30;

        game.DrawText(10, y, "5. GetTextWidthFont - right-aligned / centered:", COLOR_WHITE);
        y += 16;
        const char *alignText = "Right-aligned";
        int tw = game.GetTextWidthFont(alignText, "Microsoft YaHei", 20);
        int rx = 780 - tw;
        game.DrawTextFont(rx, y, alignText, COLOR_RED, 20);
        game.DrawLine(780, y, 780, y + 20, COLOR_GRAY);
        y += 26;

        const char *centerText = "Center-aligned Text";
        int cw = game.GetTextWidthFont(centerText, "Microsoft YaHei", 20);
        int cx = (800 - cw) / 2;
        game.DrawTextFont(cx, y, centerText, COLOR_MAGENTA, 20);
        y += 30;

        game.DrawText(10, y, "6. Color variety:", COLOR_WHITE);
        y += 16;
        int xOff = 20;
        game.DrawTextFont(xOff, y, "Red ", COLOR_RED, 18);
        xOff += game.GetTextWidthFont("Red ", "Microsoft YaHei", 18);
        game.DrawTextFont(xOff, y, "Green ", COLOR_GREEN, 18);
        xOff += game.GetTextWidthFont("Green ", "Microsoft YaHei", 18);
        game.DrawTextFont(xOff, y, "Blue ", COLOR_BLUE, 18);
        xOff += game.GetTextWidthFont("Blue ", "Microsoft YaHei", 18);
        game.DrawTextFont(xOff, y, "Yellow ", COLOR_YELLOW, 18);
        xOff += game.GetTextWidthFont("Yellow ", "Microsoft YaHei", 18);
        game.DrawTextFont(xOff, y, "Cyan ", COLOR_CYAN, 18);
        y += 28;

        game.DrawText(10, y, "7. Dynamic text (frame counter):", COLOR_WHITE);
        y += 16;
        char buf[96];
        snprintf(buf, sizeof(buf), "Frame: %d", frame);
        buf[sizeof(buf) - 1] = '\0';
        game.DrawTextFont(20, y, buf, COLOR_WHITE, 20);

        game.DrawTextFont(10, 570, "Press ESC to exit", COLOR_GRAY, 14);

#if GAMELIB_SDL_HAS_TTF
        if (tw <= 0 || cw <= 0) {
            game.DrawText(430, 570, "TTF active but width measurement failed", COLOR_RED);
        }
#else
        game.DrawText(430, 570, "Build without SDL_ttf", COLOR_RED);
#endif

        if (game.IsKeyDown(KEY_ESCAPE)) {
            break;
        }

        game.Update();
        game.WaitFrame(60);
    }

    return 0;
}