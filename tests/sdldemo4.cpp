#include "../GameLib.SDL.h"

int main()
{
    GameLib game;
    if (game.Open(760, 560, "SDL Sound Demo", true) != 0) {
        return 1;
    }
    game.ShowFps(true);

    const char *wavExplosion = "../assets/sound/explosion.wav";
    const char *wavCoin = "../assets/sound/coin.wav";
    const char *wavJump = "../assets/sound/jump.wav";
    const char *musicLoop = "../assets/sound/victory.wav";

    bool lastWavOk = true;
    bool lastMusicOk = true;
    const char *lastEffect = "none";

    int notes[8] = {262, 294, 330, 349, 392, 440, 494, 523};
    const char *noteNames[8] = {"C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5"};
    int activeNote = -1;

    while (!game.IsClosed()) {
        if (game.IsKeyPressed(KEY_ESCAPE)) {
            break;
        }

        game.Clear(COLOR_DARK_BLUE);
        game.DrawTextScale(212, 18, "SDL SOUND DEMO", COLOR_GOLD, 2);

#if GAMELIB_SDL_HAS_MIXER
        game.DrawText(24, 58, "SDL_mixer support: ON", COLOR_GREEN);
#else
        game.DrawText(24, 58, "SDL_mixer support: OFF", COLOR_RED);
#endif
        game.DrawPrintf(24, 72, COLOR_LIGHT_GRAY, "Music state: %s", game.IsMusicPlaying() ? "Playing" : "Stopped");

        game.DrawText(24, 104, "1. Beep piano (1~8):", COLOR_WHITE);
        game.DrawText(24, 118, "PlayBeep is blocking by design, same as the teaching-oriented Win32 line.", COLOR_LIGHT_GRAY);

        activeNote = -1;
        for (int i = 0; i < 8; i++) {
            int bx = 24 + i * 86;
            int by = 144;
            bool pressed = game.IsKeyPressed(KEY_1 + i);
            if (pressed) activeNote = i;

            uint32_t fill = (activeNote == i) ? COLOR_YELLOW : COLOR_WHITE;
            game.FillRect(bx, by, 68, 110, fill);
            game.DrawRect(bx, by, 68, 110, COLOR_BLACK);
            game.DrawText(bx + 28, by + 74, noteNames[i], COLOR_BLACK);
            game.DrawPrintf(bx + 22, by + 90, COLOR_BLACK, "%d", i + 1);

            if (pressed) {
                game.PlayBeep(notes[i], 150);
            }
        }

        game.DrawText(24, 280, "2. WAV sound effects:", COLOR_WHITE);
        game.DrawText(24, 294, "SPACE explosion   C coin   J jump   S stop current WAV channel", COLOR_LIGHT_GRAY);

        if (game.IsKeyPressed(KEY_SPACE)) {
            lastEffect = "explosion.wav";
            lastWavOk = game.PlayWAV(wavExplosion);
        }
        if (game.IsKeyPressed(KEY_C)) {
            lastEffect = "coin.wav";
            lastWavOk = game.PlayWAV(wavCoin);
        }
        if (game.IsKeyPressed(KEY_J)) {
            lastEffect = "jump.wav";
            lastWavOk = game.PlayWAV(wavJump);
        }
        if (game.IsKeyPressed(KEY_S)) {
            game.StopWAV();
        }

        game.FillRect(24, 316, 210, 30, COLOR_GREEN);
        game.DrawText(38, 325, "SPACE - Explosion", COLOR_BLACK);
        game.FillRect(248, 316, 120, 30, COLOR_GOLD);
        game.DrawText(284, 325, "C - Coin", COLOR_BLACK);
        game.FillRect(382, 316, 120, 30, COLOR_CYAN);
        game.DrawText(418, 325, "J - Jump", COLOR_BLACK);
        game.FillRect(516, 316, 150, 30, COLOR_RED);
        game.DrawText(548, 325, "S - Stop WAV", COLOR_BLACK);

        game.DrawPrintf(24, 358, COLOR_LIGHT_GRAY, "Last WAV request: %s (%s)", lastEffect, lastWavOk ? "OK" : "Failed");
        game.DrawText(24, 372, "Assets used: ../assets/sound/explosion.wav, coin.wav, jump.wav", COLOR_GRAY);

        game.DrawText(24, 404, "3. Looping music path:", COLOR_WHITE);
        game.DrawText(24, 418, "M starts PlayMusic on victory.wav in loop mode, N stops it.", COLOR_LIGHT_GRAY);
        game.DrawText(24, 432, "The repo currently ships WAV effects, so this demo uses a looping WAV as the music source.", COLOR_GRAY);

        if (game.IsKeyPressed(KEY_M)) {
            lastMusicOk = game.PlayMusic(musicLoop, true);
        }
        if (game.IsKeyPressed(KEY_N)) {
            game.StopMusic();
        }

        game.FillRect(24, 456, 170, 32, COLOR_GREEN);
        game.DrawText(68, 466, "M - Play Music", COLOR_BLACK);
        game.FillRect(210, 456, 170, 32, COLOR_RED);
        game.DrawText(260, 466, "N - Stop Music", COLOR_BLACK);

        game.DrawPrintf(24, 504, COLOR_LIGHT_GRAY, "Last music request: %s", lastMusicOk ? "OK" : "Failed");
        game.DrawText(24, 520, "ESC to exit", COLOR_DARK_GRAY);
        if (!lastMusicOk) {
            game.DrawText(170, 520, "If M fails, verify SDL_mixer build/runtime codecs for Mix_LoadMUS.", COLOR_ORANGE);
        }

        game.Update();
        game.WaitFrame(60);
    }

    return 0;
}