#include "../GameLib.SDL.h"

static const int TS = 16;
static const int CHAR_W = 32;
static const int CHAR_FOOT = 44;

enum { FG_GRASS = 0, FG_DIRT = 1, FG_BRICK = 2, FG_STONE = 3 };
enum { BG_SKY = 0, BG_CLOUD_L = 1, BG_CLOUD_R = 2, BG_HILL_TOP = 3, BG_HILL = 4 };

static void spriteFill(GameLib &g, int id, int x0, int y0, int w, int h, uint32_t c)
{
    for (int y = y0; y < y0 + h; y++)
        for (int x = x0; x < x0 + w; x++)
            g.SetSpritePixel(id, x, y, c);
}

static int makeBgTileset(GameLib &game)
{
    int id = game.CreateSprite(5 * TS, TS);
    if (id < 0) return -1;

    uint32_t sky = COLOR_RGB(135, 206, 235);
    int ox = 0;

    spriteFill(game, id, BG_SKY * TS, 0, TS, TS, sky);

    ox = BG_CLOUD_L * TS;
    spriteFill(game, id, ox, 0, TS, TS, sky);
    spriteFill(game, id, ox + 4, 6, 12, 6, COLOR_WHITE);
    spriteFill(game, id, ox + 6, 3, 8, 5, COLOR_WHITE);

    ox = BG_CLOUD_R * TS;
    spriteFill(game, id, ox, 0, TS, TS, sky);
    spriteFill(game, id, ox, 6, 12, 6, COLOR_WHITE);
    spriteFill(game, id, ox + 2, 3, 8, 5, COLOR_WHITE);

    ox = BG_HILL_TOP * TS;
    spriteFill(game, id, ox, 0, TS, TS, sky);
    for (int y = 0; y < TS; y++) {
        int hw = 1 + y * 7 / 15;
        for (int x = 8 - hw; x < 8 + hw; x++) {
            if (x >= 0 && x < TS) {
                game.SetSpritePixel(id, ox + x, y, COLOR_RGB(70, 130, 70));
            }
        }
    }

    spriteFill(game, id, BG_HILL * TS, 0, TS, TS, COLOR_RGB(70, 130, 70));

    return id;
}

static int loadSpriteWithFallbacks(GameLib &game, const char *pathA, const char *pathB)
{
    int id = game.LoadSprite(pathA);
    if (id >= 0) return id;
    if (pathB && pathB[0]) return game.LoadSprite(pathB);
    return -1;
}

int main()
{
    GameLib game;
    const int SW = 640;
    const int SH = 480;
    if (game.Open(SW, SH, "SDL Demo 5 - Tilemap Two-Layer Scrolling", true) != 0) {
        return 1;
    }
    game.ShowFps(true);

    int fgTS = loadSpriteWithFallbacks(game, "../assets/tileset.png", "assets/tileset.png");
    int bgTS = makeBgTileset(game);
    int charSpr = loadSpriteWithFallbacks(game, "../assets/character.png", "assets/character.png");
    int treeSpr = loadSpriteWithFallbacks(game, "../assets/tree.png", "assets/tree.png");

    if (fgTS < 0 || bgTS < 0 || charSpr < 0 || treeSpr < 0) {
        while (!game.IsClosed()) {
            if (game.IsKeyPressed(KEY_ESCAPE)) break;

            game.Clear(COLOR_DARK_RED);
            game.DrawTextScale(96, 100, "SDL DEMO 5", COLOR_WHITE, 2);
            game.DrawText(64, 154, "Required sprite assets could not be loaded.", COLOR_WHITE);
            game.DrawText(64, 176, "This demo tries both ../assets/*.png and assets/*.png.", COLOR_LIGHT_GRAY);
            game.DrawPrintf(64, 210, COLOR_YELLOW, "tileset.png: %s", fgTS >= 0 ? "OK" : "Missing");
            game.DrawPrintf(64, 226, COLOR_YELLOW, "character.png: %s", charSpr >= 0 ? "OK" : "Missing");
            game.DrawPrintf(64, 242, COLOR_YELLOW, "tree.png: %s", treeSpr >= 0 ? "OK" : "Missing");
            game.DrawText(64, 286, "Press ESC to exit.", COLOR_WHITE);
            game.Update();
            game.WaitFrame(60);
        }

        game.FreeSprite(fgTS);
        game.FreeSprite(bgTS);
        game.FreeSprite(charSpr);
        game.FreeSprite(treeSpr);
        return 1;
    }

    const int FG_C = 80;
    const int FG_R = 30;
    const int BG_C = 60;
    const int BG_R = 30;
    int fgMap = game.CreateTilemap(FG_C, FG_R, TS, fgTS);
    int bgMap = game.CreateTilemap(BG_C, BG_R, TS, bgTS);
    if (fgMap < 0 || bgMap < 0) {
        game.FreeTilemap(fgMap);
        game.FreeTilemap(bgMap);
        game.FreeSprite(fgTS);
        game.FreeSprite(bgTS);
        game.FreeSprite(charSpr);
        game.FreeSprite(treeSpr);
        return 1;
    }

    int fgCols = game.GetTilemapCols(fgMap);
    int fgRows = game.GetTilemapRows(fgMap);
    int fgTileSize = game.GetTileSize(fgMap);
    int bgRows = game.GetTilemapRows(bgMap);
    int fgWorldWidth = fgCols * fgTileSize;
    int fgWorldHeight = fgRows * fgTileSize;

    game.ClearTilemap(bgMap, BG_SKY);
    int cloudPos[][2] = { {5, 4}, {18, 3}, {33, 5}, {48, 2}, {55, 6} };
    for (int i = 0; i < 5; i++) {
        game.SetTile(bgMap, cloudPos[i][0], cloudPos[i][1], BG_CLOUD_L);
        game.SetTile(bgMap, cloudPos[i][0] + 1, cloudPos[i][1], BG_CLOUD_R);
    }

    int hills[][3] = { {8, 13, 24}, {22, 30, 22}, {40, 47, 25} };
    for (int i = 0; i < 3; i++) {
        int hillCols = hills[i][1] - hills[i][0];
        game.FillTileRect(bgMap, hills[i][0], hills[i][2], hillCols, 1, BG_HILL_TOP);
        game.FillTileRect(bgMap, hills[i][0], hills[i][2] + 1, hillCols,
                          bgRows - (hills[i][2] + 1), BG_HILL);
    }

    game.ClearTilemap(fgMap);
    game.FillTileRect(fgMap, 0, fgRows - 3, fgCols, 1, FG_GRASS);
    game.FillTileRect(fgMap, 0, fgRows - 2, fgCols, 2, FG_DIRT);
    game.FillTileRect(fgMap, 20, fgRows - 3, 3, 3, -1);
    game.FillTileRect(fgMap, 48, fgRows - 3, 3, 3, -1);
    game.FillTileRect(fgMap, 12, 23, 6, 1, FG_BRICK);
    game.FillTileRect(fgMap, 30, 20, 6, 1, FG_BRICK);
    game.FillTileRect(fgMap, 42, 24, 5, 1, FG_BRICK);
    for (int step = 0; step < 5; step++) {
        game.FillTileRect(fgMap, 60, fgRows - 4 - step, step + 1, 1, FG_STONE);
    }
    game.FillTileRect(fgMap, 66, 17, 7, 1, FG_STONE);

    int treeX[] = { 5 * TS, 25 * TS, 38 * TS, 55 * TS, 70 * TS };
    int treeCount = 5;
    int treeY = (FG_R - 3) * TS - 61;

    float playerX = 48.0f;
    float playerY = (float)((fgRows - 3) * fgTileSize - CHAR_FOOT);
    float speed = 180.0f;
    int facing = 1;

    while (!game.IsClosed()) {
        float dt = game.GetDeltaTime();
        if (dt > 0.05f) dt = 0.05f;

        if (game.IsKeyDown(KEY_RIGHT) || game.IsKeyDown(KEY_D)) {
            playerX += speed * dt;
            facing = 1;
        }
        if (game.IsKeyDown(KEY_LEFT) || game.IsKeyDown(KEY_A)) {
            playerX -= speed * dt;
            facing = -1;
        }
        if (playerX < 0) playerX = 0;
        if (playerX > fgWorldWidth - CHAR_W) playerX = (float)(fgWorldWidth - CHAR_W);

        float cameraX = playerX - SW / 2 + CHAR_W / 2;
        float maxCam = (float)(fgWorldWidth - SW);
        if (cameraX < 0) cameraX = 0;
        if (cameraX > maxCam) cameraX = maxCam;

        int footX = (int)playerX + CHAR_W / 2;
        int footY = (int)playerY + CHAR_FOOT;
        int footCol = game.WorldToTileCol(fgMap, footX);
        int footRow = game.WorldToTileRow(fgMap, footY);
        int footTile = game.GetTileAtPixel(fgMap, footX, footY);

        game.Clear(COLOR_RGB(135, 206, 235));
        game.DrawTilemap(bgMap, -(int)(cameraX * 0.5f), 0);

        int camX = (int)cameraX;
        game.DrawTilemap(fgMap, -camX, 0);

        for (int i = 0; i < treeCount; i++) {
            game.DrawSpriteEx(treeSpr, treeX[i] - camX, treeY, SPRITE_COLORKEY);
        }

        int px = (int)playerX - camX;
        int py = (int)playerY;
        int pflags = SPRITE_COLORKEY | ((facing < 0) ? SPRITE_FLIP_H : 0);
        game.DrawSpriteEx(charSpr, px, py, pflags);

        game.FillRect(0, 0, 316, 52, COLOR_RGB(15, 15, 25));
        game.DrawText(8, 6, "<- -> / A D Move    ESC Quit", COLOR_WHITE);
        game.DrawPrintf(8, 20, COLOR_LIGHT_GRAY,
                        "Camera: %d   World: %dx%d", camX, fgWorldWidth, fgWorldHeight);
        game.DrawPrintf(8, 34, COLOR_LIGHT_GRAY,
                        "Foot tile: (%d,%d) = %d", footCol, footRow, footTile);

        if (game.IsKeyPressed(KEY_ESCAPE)) break;

        game.Update();
        game.WaitFrame(60);
    }

    game.FreeTilemap(fgMap);
    game.FreeTilemap(bgMap);
    game.FreeSprite(fgTS);
    game.FreeSprite(bgTS);
    game.FreeSprite(charSpr);
    game.FreeSprite(treeSpr);
    return 0;
}