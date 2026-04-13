## API 速查

### 窗口

| 函数                         | 说明                         |
| ---------------------------- | ---------------------------- |
| `Open(w, h, title, center)`  | 创建窗口，center=true 时居中 |
| `IsClosed()`                 | 窗口是否关闭                 |
| `Update()`                   | 刷新画面、处理输入           |
| `WaitFrame(fps)`             | 帧率控制                     |
| `GetWidth()` / `GetHeight()` | 窗口尺寸                     |
| `GetDeltaTime()`             | 帧间隔（秒）                 |
| `GetFPS()`                   | 当前帧率                     |
| `GetTime()`                  | 运行总时间（秒）             |
| `SetTitle(title)`            | 修改窗口标题                 |

### 绘图

| 函数                              | 说明       |
| --------------------------------- | ---------- |
| `Clear(color)`                    | 清屏       |
| `SetPixel(x, y, color)`           | 画点       |
| `GetPixel(x, y)`                  | 读点       |
| `DrawLine(x1, y1, x2, y2, color)` | 画线       |
| `DrawRect(x, y, w, h, color)`     | 矩形边框   |
| `FillRect(x, y, w, h, color)`     | 填充矩形   |
| `DrawCircle(cx, cy, r, color)`    | 圆形边框   |
| `FillCircle(cx, cy, r, color)`    | 填充圆     |
| `DrawTriangle(...)`               | 三角形边框 |
| `FillTriangle(...)`               | 填充三角形 |

### 文字

| 函数                                      | 说明       |
| ----------------------------------------- | ---------- |
| `DrawText(x, y, text, color)`             | 绘制文字   |
| `DrawNumber(x, y, number, color)`         | 绘制整数   |
| `DrawTextScale(x, y, text, color, scale)` | 放大文字   |
| `DrawPrintf(x, y, color, fmt, ...)`       | 格式化输出 |
| `DrawTextFont(x, y, text, color, size)`   | 用默认字体绘制文字 |
| `DrawTextFont(x, y, text, color, font, size)` | 用指定字体绘制文字 |
| `GetTextWidthFont(...)` / `GetTextHeightFont(...)` | 测量字体文字尺寸 |

### 精灵

| 函数                                         | 说明                            |
| -------------------------------------------- | ------------------------------- |
| `CreateSprite(w, h)`                         | 创建空白精灵，返回 ID           |
| `LoadSprite(filename)`                       | 加载图片精灵（PNG/JPG/BMP/GIF） |
| `LoadSpriteBMP(filename)`                    | 从 BMP 加载精灵（8/24/32-bit）  |
| `FreeSprite(id)`                             | 释放精灵                        |
| `DrawSprite(id, x, y)`                       | 绘制精灵                        |
| `DrawSpriteEx(id, x, y, flags)`              | 带翻转/透明/Alpha混合绘制       |
| `DrawSpriteRegion(id, x, y, sx, sy, sw, sh)` | 绘制精灵子区域                  |
| `SetSpritePixel(id, x, y, color)`            | 修改精灵像素                    |
| `GetSpritePixel(id, x, y)`                   | 读取精灵像素                    |

精灵标志：`SPRITE_FLIP_H`（水平翻转）、`SPRITE_FLIP_V`（垂直翻转）、`SPRITE_COLORKEY`（品红色透明）、`SPRITE_ALPHA`（Alpha 混合）

### 输入

| 函数                          | 说明                       |
| ----------------------------- | -------------------------- |
| `IsKeyDown(key)`              | 按键是否按住               |
| `IsKeyPressed(key)`           | 按键是否刚按下（单次触发） |
| `GetMouseX()` / `GetMouseY()` | 鼠标位置                   |
| `IsMouseDown(button)`         | 鼠标按键是否按下           |

### 声音

| 函数                        | 说明                     |
| --------------------------- | ------------------------ |
| `PlayWAV(filename, loop)`   | 播放音效                 |
| `StopWAV()`                 | 停止音效                 |
| `PlayMusic(filename, loop)` | 播放背景音乐（MP3/MIDI） |
| `StopMusic()`               | 停止背景音乐             |
| `PlayBeep(freq, duration)`  | 蜂鸣器                   |

### 工具

| 函数                                      | 说明         |
| ----------------------------------------- | ------------ |
| `Random(min, max)`                        | 随机数       |
| `RectOverlap(...)`                        | 矩形碰撞检测 |
| `CircleOverlap(...)`                      | 圆形碰撞检测 |
| `PointInRect(px, py, x, y, w, h)`         | 点在矩形内   |
| `Distance(x1, y1, x2, y2)`                | 两点距离     |
| `DrawGrid(x, y, rows, cols, size, color)` | 画网格       |
| `FillCell(gx, gy, row, col, size, color)` | 填充网格单元 |

### Tilemap

| 函数                                             | 说明                            |
| ------------------------------------------------ | ------------------------------- |
| `CreateTilemap(cols, rows, tileSize, tilesetId)` | 创建瓦片地图，返回 ID           |
| `FreeTilemap(mapId)`                             | 释放地图                        |
| `SetTile(mapId, col, row, tileId)`               | 设置瓦片（-1=空）               |
| `GetTile(mapId, col, row)`                       | 读取瓦片                        |
| `DrawTilemap(mapId, x, y, flags)`                | 绘制地图（支持 ColorKey/Alpha） |

tileset 是一张普通精灵（`LoadSprite` / `CreateSprite`），按 `tileSize` 自动切分。`DrawTilemap` 只绘制屏幕可见瓦片，传 `(-cameraX, -cameraY)` 即可实现卷轴。

### 颜色常量

```
COLOR_BLACK    COLOR_WHITE     COLOR_RED       COLOR_GREEN     COLOR_BLUE
COLOR_YELLOW   COLOR_CYAN      COLOR_MAGENTA   COLOR_ORANGE    COLOR_PINK
COLOR_PURPLE   COLOR_GRAY      COLOR_DARK_GRAY COLOR_LIGHT_GRAY
COLOR_DARK_RED COLOR_DARK_GREEN COLOR_DARK_BLUE COLOR_SKY_BLUE
COLOR_BROWN    COLOR_GOLD      COLOR_TRANSPARENT
```

自定义颜色：`COLOR_RGB(r, g, b)` 或 `COLOR_ARGB(a, r, g, b)`