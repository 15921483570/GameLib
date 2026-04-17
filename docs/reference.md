# GameLib API Reference

本文档提供 GameLib.h 所有公开 API 的详细参考，按功能模块组织。

---

## 1. 窗口与主循环

### Open

```cpp
int Open(int width, int height, const char *title, bool center = false, bool resizable = false)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `width` | `int` | Framebuffer 逻辑宽度（像素），范围 1~16384 |
| `height` | `int` | Framebuffer 逻辑高度（像素），范围 1~16384 |
| `title` | `const char *` | 窗口标题，支持 UTF-8 |
| `center` | `bool` | 是否居中显示，默认 `false` |
| `resizable` | `bool` | 是否允许用户拖拽缩放和最大化，默认 `false` |

**返回值**
| 值 | 说明 |
|-----|------|
| `0` | 成功 |
| `-1` | 窗口类注册失败 |
| `-2` | 创建 DC 失败 |
| `-3` | 创建 DIB Section 失败 |
| `-4` | SelectObject 失败 |
| `-5` | UTF-8 转换失败 |
| `-6` | 创建窗口失败 |
| `-7` | 尺寸超限 |

**说明**
创建窗口并初始化帧缓冲、输入、时间系统。`width/height` 决定固定 framebuffer 逻辑尺寸，打开后不会因窗口缩放而改变。若之前已 `Open()` 过，会先清理旧状态，支持 restart-safe 重开。

---

### IsClosed

```cpp
bool IsClosed() const
```

**参数**
无

**返回值**
| 值 | 说明 |
|-----|------|
| `true` | 窗口已关闭 |
| `false` | 窗口仍在运行 |

**说明**
判断窗口是否已关闭（WM_CLOSE 或 WM_DESTROY 触发）。

---

### Update

```cpp
void Update()
```

**参数**
无

**返回值**
无

**说明**
刷新画面并处理输入。执行步骤：保存上一帧按键状态、派发 Windows 消息、同步客户区尺寸和输入状态、提交帧缓冲到窗口、更新 deltaTime 和 FPS。

---

### WaitFrame

```cpp
void WaitFrame(int fps)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `fps` | `int` | 目标帧率，`<= 0` 时默认按 60 处理 |

**返回值**
无

**说明**
帧率控制，基于绝对帧边界做节拍。使用高精度计时器维护帧起点，避免把本帧工作耗时重复算进等待时间。

---

### GetDeltaTime

```cpp
double GetDeltaTime() const
```

**参数**
无

**返回值**
从上一帧到当前帧的时间间隔（秒），类型 `double`。

**说明**
获取帧间隔，用于物理计算、动画速度控制等。

---

### GetFPS

```cpp
double GetFPS() const
```

**参数**
无

**返回值**
当前帧率（每秒更新一次），类型 `double`。

**说明**
获取当前帧率，每秒统计一次，避免帧间波动。

---

### GetTime

```cpp
double GetTime() const
```

**参数**
无

**返回值**
从 `Open()` 开始到现在的总时间（秒），类型 `double`。

**说明**
获取运行总时间。在 `Open()` 前调用安全返回 `0.0`。

---

### GetWidth / GetHeight

```cpp
int GetWidth() const
int GetHeight() const
```

**参数**
无

**返回值**
Framebuffer 逻辑宽度/高度（像素），类型 `int`。

**说明**
获取 framebuffer 尺寸，与 `Open()` 时传入的参数一致。

---

### WinResize

```cpp
void WinResize(int width, int height)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `width` | `int` | 窗口客户区宽度（像素） |
| `height` | `int` | 窗口客户区高度（像素） |

**返回值**
无

**说明**
设置窗口客户区尺寸，不改变 framebuffer 尺寸。对不可缩放窗口同样有效；若当前是最大化的可缩放窗口，会先还原再设置尺寸。

---

### SetMaximized

```cpp
void SetMaximized(bool maximized)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `maximized` | `bool` | `true` 最大化，`false` 还原 |

**返回值**
无

**说明**
仅在 `Open(..., ..., ..., ..., true)` 创建的可缩放窗口上有效。

---

### SetTitle

```cpp
void SetTitle(const char *title)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `title` | `const char *` | 新窗口标题，支持 UTF-8 |

**返回值**
无

**说明**
修改窗口标题。

---

### ShowFps

```cpp
void ShowFps(bool show)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `show` | `bool` | 是否在标题栏显示 FPS |

**返回值**
无

**说明**
设置是否在窗口标题栏显示实时 FPS。`show=true` 时标题栏显示为 `"原标题 (FPS: 58.8)"` 格式，每秒更新一次。

---

### ShowMouse

```cpp
void ShowMouse(bool show)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `show` | `bool` | 是否显示鼠标光标 |

**返回值**
无

**说明**
控制窗口客户区内的鼠标光标显示/隐藏。不使用全局 `ShowCursor` 引用计数，避免不同窗口/库同时操作时弄乱系统全局状态。

---

### ShowMessage

```cpp
int ShowMessage(const char *text, const char *title = NULL, int buttons = MESSAGEBOX_OK)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `text` | `const char *` | 消息文本，支持 UTF-8 |
| `title` | `const char *` | 消息框标题，`NULL` 或空时使用窗口标题，支持 UTF-8 |
| `buttons` | `int` | 按钮类型：`MESSAGEBOX_OK` 或 `MESSAGEBOX_YESNO` |

**返回值**
| 值 | 说明 |
|-----|------|
| `MESSAGEBOX_RESULT_OK` | 用户点击 OK |
| `MESSAGEBOX_RESULT_YES` | 用户点击 Yes |
| `MESSAGEBOX_RESULT_NO` | 用户点击 No |

**说明**
弹出消息框。文本和标题按 UTF-8 解释。

---

## 2. 绘图

### Clear

```cpp
void Clear(uint32_t color = COLOR_BLACK)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `color` | `uint32_t` | 填充颜色，ARGB 格式，默认黑色 |

**返回值**
无

**说明**
用指定颜色填充当前裁剪矩形覆盖到的帧缓冲区域。不做 Alpha 混合。

---

### SetPixel

```cpp
void SetPixel(int x, int y, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 像素 X 坐标 |
| `y` | `int` | 像素 Y 坐标 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |

**返回值**
无

**说明**
设置指定像素颜色（带裁剪和边界检查）。当 `color` 的 Alpha 小于 255 时，按 source-over 规则与当前帧缓冲做混合。

---

### GetPixel

```cpp
uint32_t GetPixel(int x, int y) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 像素 X 坐标 |
| `y` | `int` | 像素 Y 坐标 |

**返回值**
指定像素的 ARGB 颜色值，越界返回 `0`。

**说明**
获取指定像素颜色。

---

### SetClip

```cpp
void SetClip(int x, int y, int w, int h)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 裁剪矩形左上角 X |
| `y` | `int` | 裁剪矩形左上角 Y |
| `w` | `int` | 裁剪矩形宽度 |
| `h` | `int` | 裁剪矩形高度 |

**返回值**
无

**说明**
设置裁剪矩形。传入矩形会自动与屏幕 `[0, width) × [0, height)` 求交。`w <= 0 || h <= 0` 或求交后为空时，进入"无可见区域"状态，后续所有绘制函数不生效。

---

### ClearClip

```cpp
void ClearClip()
```

**参数**
无

**返回值**
无

**说明**
清除当前裁剪，恢复整屏可见。

---

### GetClip

```cpp
void GetClip(int *x, int *y, int *w, int *h) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int *` | 输出裁剪矩形左上角 X |
| `y` | `int *` | 输出裁剪矩形左上角 Y |
| `w` | `int *` | 输出裁剪矩形宽度 |
| `h` | `int *` | 输出裁剪矩形高度 |

**返回值**
无

**说明**
读取当前有效裁剪矩形，返回值已与屏幕求交。

---

### GetClipX / GetClipY / GetClipW / GetClipH

```cpp
int GetClipX() const
int GetClipY() const
int GetClipW() const
int GetClipH() const
```

**参数**
无

**返回值**
当前有效裁剪矩形的各个分量，类型 `int`。

**说明**
读取当前有效裁剪矩形的左上角坐标和尺寸。

---

### Screenshot

```cpp
void Screenshot(const char *filename)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | 输出文件路径，支持 UTF-8 |

**返回值**
无

**说明**
将当前 framebuffer 保存为 24-bit BMP 文件。从 ARGB32 提取 R/G/B 三通道，按 BGR 顺序写入。

---

### DrawLine

```cpp
void DrawLine(int x1, int y1, int x2, int y2, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x1` | `int` | 起点 X 坐标 |
| `y1` | `int` | 起点 Y 坐标 |
| `x2` | `int` | 终点 X 坐标 |
| `y2` | `int` | 终点 Y 坐标 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |

**返回值**
无

**说明**
绘制直线。先与当前裁剪矩形做线段裁剪，再进入 Bresenham 算法逐点绘制。支持 Alpha 混合。

---

### DrawRect

```cpp
void DrawRect(int x, int y, int w, int h, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 矩形左上角 X |
| `y` | `int` | 矩形左上角 Y |
| `w` | `int` | 矩形宽度 |
| `h` | `int` | 矩形高度 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |

**返回值**
无

**说明**
绘制矩形边框。`w <= 0 || h <= 0` 时直接返回。支持 Alpha 混合。

---

### FillRect

```cpp
void FillRect(int x, int y, int w, int h, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 矩形左上角 X |
| `y` | `int` | 矩形左上角 Y |
| `w` | `int` | 矩形宽度 |
| `h` | `int` | 矩形高度 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |

**返回值**
无

**说明**
填充矩形。带裁剪。支持 Alpha 混合。

---

### DrawCircle

```cpp
void DrawCircle(int cx, int cy, int r, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `cx` | `int` | 圆心 X 坐标 |
| `cy` | `int` | 圆心 Y 坐标 |
| `r` | `int` | 半径 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |

**返回值**
无

**说明**
绘制圆形边框。使用中点圆算法，按唯一对称点输出轮廓，避免半透明颜色在边界点重复混合。支持 Alpha 混合。

---

### FillCircle

```cpp
void FillCircle(int cx, int cy, int r, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `cx` | `int` | 圆心 X 坐标 |
| `cy` | `int` | 圆心 Y 坐标 |
| `r` | `int` | 半径 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |

**返回值**
无

**说明**
填充圆。复用椭圆扫描线填充路径，每行写一条水平线，避免半透明颜色重复覆盖。支持 Alpha 混合。

---

### DrawEllipse

```cpp
void DrawEllipse(int cx, int cy, int rx, int ry, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `cx` | `int` | 椭圆中心 X 坐标 |
| `cy` | `int` | 椭圆中心 Y 坐标 |
| `rx` | `int` | 横向半径 |
| `ry` | `int` | 纵向半径 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |

**返回值**
无

**说明**
绘制椭圆边框。使用 midpoint ellipse 分区迭代，按唯一对称点输出轮廓。退化情况自动回退为点或直线。支持 Alpha 混合。

---

### FillEllipse

```cpp
void FillEllipse(int cx, int cy, int rx, int ry, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `cx` | `int` | 椭圆中心 X 坐标 |
| `cy` | `int` | 椭圆中心 Y 坐标 |
| `rx` | `int` | 横向半径 |
| `ry` | `int` | 纵向半径 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |

**返回值**
无

**说明**
按扫描线方式填充椭圆。每行通过 `_DrawHLine` 写入。支持退化为点或直线。支持 Alpha 混合。

---

### DrawTriangle

```cpp
void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x1` | `int` | 第一个顶点 X |
| `y1` | `int` | 第一个顶点 Y |
| `x2` | `int` | 第二个顶点 X |
| `y2` | `int` | 第二个顶点 Y |
| `x3` | `int` | 第三个顶点 X |
| `y3` | `int` | 第三个顶点 Y |
| `color` | `uint32_t` | 颜色，ARGB 格式 |

**返回值**
无

**说明**
绘制三角形边框（3 条 DrawLine）。支持 Alpha 混合。

---

### FillTriangle

```cpp
void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x1` | `int` | 第一个顶点 X |
| `y1` | `int` | 第一个顶点 Y |
| `x2` | `int` | 第二个顶点 X |
| `y2` | `int` | 第二个顶点 Y |
| `x3` | `int` | 第三个顶点 X |
| `y3` | `int` | 第三个顶点 Y |
| `color` | `uint32_t` | 颜色，ARGB 格式 |

**返回值**
无

**说明**
扫描线填充三角形。先按 Y 排序三个顶点，然后逐行扫描。处理退化情况。边插值使用 `int64_t` 防溢出。支持 Alpha 混合。

---

## 3. 文字（内置 8x8 字体）

### DrawText

```cpp
void DrawText(int x, int y, const char *text, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 文字左上角 X 坐标 |
| `y` | `int` | 文字左上角 Y 坐标 |
| `text` | `const char *` | 文字内容，ASCII 32~126 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |

**返回值**
无

**说明**
使用内嵌 8x8 位图字体绘制文字。支持 `\n` 换行（行间距 10 像素）。每个字符宽度 8 像素。

---

### DrawNumber

```cpp
void DrawNumber(int x, int y, int number, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 文字左上角 X 坐标 |
| `y` | `int` | 文字左上角 Y 坐标 |
| `number` | `int` | 整数值 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |

**返回值**
无

**说明**
将整数转为字符串后调用 DrawText。

---

### DrawTextScale

```cpp
void DrawTextScale(int x, int y, const char *text, uint32_t color, int scale)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 文字左上角 X 坐标 |
| `y` | `int` | 文字左上角 Y 坐标 |
| `text` | `const char *` | 文字内容，ASCII 32~126 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |
| `scale` | `int` | 放大倍数，每个字体像素变为 scale × scale 矩形 |

**返回值**
无

**说明**
放大版文字绘制。支持 `\n` 换行（行间距 `(8 + 2) * scale`）。

---

### DrawPrintf

```cpp
void DrawPrintf(int x, int y, uint32_t color, const char *fmt, ...)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 文字左上角 X 坐标 |
| `y` | `int` | 文字左上角 Y 坐标 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |
| `fmt` | `const char *` | 格式字符串，支持 `%d`, `%s`, `%f` 等 |
| `...` | 可变参数 | 格式参数 |

**返回值**
无

**说明**
格式化输出，类似 `printf`。内部使用 `vsnprintf`（1024 字节缓冲），格式化后调用 `DrawText` 绘制。

---

### DrawPrintfScale

```cpp
void DrawPrintfScale(int x, int y, uint32_t color, int scale, const char *fmt, ...)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 文字左上角 X 坐标 |
| `y` | `int` | 文字左上角 Y 坐标 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |
| `scale` | `int` | 放大倍数 |
| `fmt` | `const char *` | 格式字符串，支持 `%d`, `%s`, `%f` 等 |
| `...` | 可变参数 | 格式参数 |

**返回值**
无

**说明**
放大版格式化输出。格式化后调用 `DrawTextScale` 绘制，适合放大显示分数、标题等。

---

## 4. 字体文字渲染

### DrawTextFont

```cpp
void DrawTextFont(int x, int y, const char *text, uint32_t color, const char *fontName, int fontSize)
void DrawTextFont(int x, int y, const char *text, uint32_t color, int fontSize)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 文字左上角 X 坐标 |
| `y` | `int` | 文字左上角 Y 坐标 |
| `text` | `const char *` | 文字内容，支持 UTF-8 和 `\n` 多行 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |
| `fontName` | `const char *` | 字体名称，如 "Microsoft YaHei"、"Arial"（可选） |
| `fontSize` | `int` | 字体大小（像素） |

**返回值**
无

**说明**
使用可缩放字体渲染文字。Windows 版内部用 GDI 实现。支持 UTF-8 编码和多行输出。不传 `fontName` 时使用默认字体 `GAMELIB_DEFAULT_FONT_NAME`（"Microsoft YaHei"）。当 alpha 为 0 时不绘制；当 `0 < alpha < 255` 时按调用方 alpha 混合。

---

### DrawPrintfFont

```cpp
void DrawPrintfFont(int x, int y, uint32_t color, const char *fontName, int fontSize, const char *fmt, ...)
void DrawPrintfFont(int x, int y, uint32_t color, int fontSize, const char *fmt, ...)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 文字左上角 X 坐标 |
| `y` | `int` | 文字左上角 Y 坐标 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |
| `fontName` | `const char *` | 字体名称（可选） |
| `fontSize` | `int` | 字体大小（像素） |
| `fmt` | `const char *` | 格式字符串 |
| `...` | 可变参数 | 格式参数 |

**返回值**
无

**说明**
字体版格式化输出。内部用 `vsnprintf` 组装文本后调用 `DrawTextFont`。适合分数、FPS、调试变量等输出。

---

### GetTextWidthFont

```cpp
int GetTextWidthFont(const char *text, const char *fontName, int fontSize)
int GetTextWidthFont(const char *text, int fontSize)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `text` | `const char *` | 文字内容 |
| `fontName` | `const char *` | 字体名称（可选） |
| `fontSize` | `int` | 字体大小（像素） |

**返回值**
文字宽度（像素），类型 `int`。

**说明**
获取文字在指定字体下的宽度，用于计算文字对齐、布局等。

---

### GetTextHeightFont

```cpp
int GetTextHeightFont(const char *text, const char *fontName, int fontSize)
int GetTextHeightFont(const char *text, int fontSize)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `text` | `const char *` | 文字内容 |
| `fontName` | `const char *` | 字体名称（可选） |
| `fontSize` | `int` | 字体大小（像素） |

**返回值**
文字高度（像素），类型 `int`。

**说明**
获取文字在指定字体下的高度。支持多行文本高度计算。

---

## 5. 精灵

### CreateSprite

```cpp
int CreateSprite(int width, int height)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `width` | `int` | 精灵宽度，范围 1~16384 |
| `height` | `int` | 精灵高度，范围 1~16384 |

**返回值**
精灵 ID，失败返回 `-1`。

**说明**
创建空白精灵，像素初始化为全 0（透明黑）。

---

### LoadSprite

```cpp
int LoadSprite(const char *filename)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | 图片文件路径，支持 PNG/JPG/BMP/GIF/TIFF，UTF-8 |

**返回值**
精灵 ID，失败返回 `-1`。

**说明**
通用图片加载。首次调用时懒加载 GDI+。始终请求 32bppARGB 格式。若 GDI+ 初始化失败且文件是 BMP，自动回退到 `LoadSpriteBMP`。

---

### LoadSpriteBMP

```cpp
int LoadSpriteBMP(const char *filename)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | BMP 文件路径，支持 8/24/32-bit，UTF-8 |

**返回值**
精灵 ID，失败返回 `-1`。

**说明**
从 BMP 文件加载精灵。支持 8-bit 调色板、24-bit、32-bit BMP。8-bit 调色板自动转换为 32-bit ARGB。

---

### FreeSprite

```cpp
void FreeSprite(int id)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `id` | `int` | 精灵 ID |

**返回值**
无

**说明**
释放精灵内存，标记槽位为未使用。

---

### DrawSprite

```cpp
void DrawSprite(int id, int x, int y)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `id` | `int` | 精灵 ID |
| `x` | `int` | 绘制位置 X |
| `y` | `int` | 绘制位置 Y |

**返回值**
无

**说明**
绘制精灵到帧缓冲。默认走不透明快路径，不检查透明孔洞。如需透明，改用 `DrawSpriteEx` 并传入 `SPRITE_COLORKEY` 或 `SPRITE_ALPHA`。

---

### DrawSpriteEx

```cpp
void DrawSpriteEx(int id, int x, int y, int flags)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `id` | `int` | 精灵 ID |
| `x` | `int` | 绘制位置 X |
| `y` | `int` | 绘制位置 Y |
| `flags` | `int` | 绘制标志，可组合 |

**返回值**
无

**说明**
带标志的精灵绘制。标志：
- `SPRITE_FLIP_H` (1)：水平翻转
- `SPRITE_FLIP_V` (2)：垂直翻转
- `SPRITE_COLORKEY` (4)：跳过与该精灵 Color Key 匹配的像素
- `SPRITE_ALPHA` (8)：逐像素 Alpha 混合

---

### DrawSpriteRegion

```cpp
void DrawSpriteRegion(int id, int x, int y, int sx, int sy, int sw, int sh)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `id` | `int` | 精灵 ID |
| `x` | `int` | 绘制位置 X |
| `y` | `int` | 绘制位置 Y |
| `sx` | `int` | 源区域左上角 X |
| `sy` | `int` | 源区域左上角 Y |
| `sw` | `int` | 源区域宽度 |
| `sh` | `int` | 源区域高度 |

**返回值**
无

**说明**
绘制精灵的子区域（sprite sheet 切图）。

---

### DrawSpriteRegionEx

```cpp
void DrawSpriteRegionEx(int id, int x, int y, int sx, int sy, int sw, int sh, int flags = 0)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `id` | `int` | 精灵 ID |
| `x` | `int` | 绘制位置 X |
| `y` | `int` | 绘制位置 Y |
| `sx` | `int` | 源区域左上角 X |
| `sy` | `int` | 源区域左上角 Y |
| `sw` | `int` | 源区域宽度 |
| `sh` | `int` | 源区域高度 |
| `flags` | `int` | 绘制标志，默认 0 |

**返回值**
无

**说明**
带 flags 绘制精灵子区域。翻转、ColorKey、Alpha 语义与 `DrawSpriteEx` 一致。

---

### DrawSpriteScaled

```cpp
void DrawSpriteScaled(int id, int x, int y, int w, int h, int flags = 0)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `id` | `int` | 精灵 ID |
| `x` | `int` | 绘制位置 X |
| `y` | `int` | 绘制位置 Y |
| `w` | `int` | 目标宽度 |
| `h` | `int` | 目标高度 |
| `flags` | `int` | 绘制标志，默认 0 |

**返回值**
无

**说明**
将整张精灵按目标尺寸缩放绘制。使用最近邻采样，适合像素风和教学场景。

---

### DrawSpriteRotated

```cpp
void DrawSpriteRotated(int id, int cx, int cy, double angleDeg, int flags = 0)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `id` | `int` | 精灵 ID |
| `cx` | `int` | 旋转中心 X |
| `cy` | `int` | 旋转中心 Y |
| `angleDeg` | `double` | 旋转角度，> 0 顺时针 |
| `flags` | `int` | 绘制标志，默认 0 |

**返回值**
无

**说明**
将整张精灵绕自身中心旋转后绘制，中心点落在 `(cx, cy)`。使用最近邻旋转采样。

---

### DrawSpriteFrame

```cpp
void DrawSpriteFrame(int id, int x, int y, int frameW, int frameH, int frameIndex, int flags = 0)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `id` | `int` | 精灵 ID（sprite sheet） |
| `x` | `int` | 绘制位置 X |
| `y` | `int` | 绘制位置 Y |
| `frameW` | `int` | 每帧宽度 |
| `frameH` | `int` | 每帧高度 |
| `frameIndex` | `int` | 帧号（从左到右、从上到下编号） |
| `flags` | `int` | 绘制标志，默认 0 |

**返回值**
无

**说明**
按帧号绘制 sprite sheet 中的帧。每行帧数由 `spriteWidth / frameW` 自动推导。

---

### DrawSpriteFrameScaled

```cpp
void DrawSpriteFrameScaled(int id, int x, int y, int frameW, int frameH, int frameIndex, int w, int h, int flags = 0)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `id` | `int` | 精灵 ID |
| `x` | `int` | 绘制位置 X |
| `y` | `int` | 绘制位置 Y |
| `frameW` | `int` | 每帧宽度 |
| `frameH` | `int` | 每帧高度 |
| `frameIndex` | `int` | 帧号 |
| `w` | `int` | 目标宽度 |
| `h` | `int` | 目标高度 |
| `flags` | `int` | 绘制标志，默认 0 |

**返回值**
无

**说明**
先按帧号选取 sprite sheet 子区域，再按目标尺寸缩放绘制。适合角色动画、头像预览等。

---

### DrawSpriteFrameRotated

```cpp
void DrawSpriteFrameRotated(int id, int cx, int cy, int frameW, int frameH, int frameIndex, double angleDeg, int flags = 0)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `id` | `int` | 精灵 ID |
| `cx` | `int` | 旋转中心 X |
| `cy` | `int` | 旋转中心 Y |
| `frameW` | `int` | 每帧宽度 |
| `frameH` | `int` | 每帧高度 |
| `frameIndex` | `int` | 帧号 |
| `angleDeg` | `double` | 旋转角度 |
| `flags` | `int` | 绘制标志，默认 0 |

**返回值**
无

**说明**
先按帧号选取 sprite sheet 子区域，再绕该帧中心旋转绘制。

---

### SetSpritePixel

```cpp
void SetSpritePixel(int id, int x, int y, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `id` | `int` | 精灵 ID |
| `x` | `int` | 像素 X 坐标 |
| `y` | `int` | 像素 Y 坐标 |
| `color` | `uint32_t` | 颜色，ARGB 格式 |

**返回值**
无

**说明**
修改精灵指定像素。

---

### GetSpritePixel

```cpp
uint32_t GetSpritePixel(int id, int x, int y) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `id` | `int` | 精灵 ID |
| `x` | `int` | 像素 X 坐标 |
| `y` | `int` | 像素 Y 坐标 |

**返回值**
像素的 ARGB 颜色值。

**说明**
读取精灵指定像素。

---

### GetSpriteWidth / GetSpriteHeight

```cpp
int GetSpriteWidth(int id) const
int GetSpriteHeight(int id) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `id` | `int` | 精灵 ID |

**返回值**
精灵宽度/高度（像素），无效 ID 返回 `0`。

**说明**
获取精灵尺寸。

---

### SetSpriteColorKey / GetSpriteColorKey

```cpp
void SetSpriteColorKey(int id, uint32_t color)
uint32_t GetSpriteColorKey(int id) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `id` | `int` | 精灵 ID |
| `color` | `uint32_t` | Color Key 颜色（设置时） |

**返回值**
设置函数无返回值。读取函数返回 Color Key 颜色，默认 `COLORKEY_DEFAULT`（品红 `0xFFFF00FF`）。

**说明**
设置或读取该精灵自己的 Color Key。配合 `SPRITE_COLORKEY` 使用。

---

## 6. 输入

### IsKeyDown

```cpp
bool IsKeyDown(int key) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `key` | `int` | 按键码，如 `KEY_UP`, `KEY_A` 等 |

**返回值**
按键是否正在按下（持续按住也返回 `true`）。

**说明**
检测按键状态。按键码使用 Windows Virtual Key Code。

---

### IsKeyPressed

```cpp
bool IsKeyPressed(int key) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `key` | `int` | 按键码 |

**返回值**
按键是否刚按下（当前帧按下且上一帧未按下）。

**说明**
边沿检测，适合单次触发逻辑。

---

### IsKeyReleased

```cpp
bool IsKeyReleased(int key) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `key` | `int` | 按键码 |

**返回值**
按键是否刚松开（当前帧未按下且上一帧按下）。

**说明**
边沿检测。

---

### GetMouseX / GetMouseY

```cpp
int GetMouseX() const
int GetMouseY() const
```

**参数**
无

**返回值**
鼠标逻辑位置（按当前窗口缩放反算到 framebuffer 坐标），类型 `int`。

**说明**
返回 framebuffer 坐标系的鼠标位置。当窗口与 framebuffer 同尺寸时，等价于普通客户区像素坐标。

---

### IsMouseDown

```cpp
bool IsMouseDown(int button) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `button` | `int` | 鼠标按键：`MOUSE_LEFT`(0)、`MOUSE_RIGHT`(1)、`MOUSE_MIDDLE`(2) |

**返回值**
鼠标按键是否按下。

**说明**
检测鼠标按键状态。

---

### IsMousePressed

```cpp
bool IsMousePressed(int button) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `button` | `int` | 鼠标按键 |

**返回值**
鼠标按键是否刚按下（当前帧按下且上一帧未按下）。

**说明**
边沿检测。

---

### IsMouseReleased

```cpp
bool IsMouseReleased(int button) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `button` | `int` | 鼠标按键 |

**返回值**
鼠标按键是否刚松开（当前帧未按下且上一帧按下）。

**说明**
边沿检测。

---

### GetMouseWheelDelta

```cpp
int GetMouseWheelDelta() const
```

**参数**
无

**返回值**
自上次 `Update()` 以来累计的滚轮增量，Windows 标准滚轮一格通常为 `120` 或 `-120`。

**说明**
读取不会清零；在下一次 `Update()` 开始时刷新为 0。

---

### IsActive

```cpp
bool IsActive() const
```

**参数**
无

**返回值**
窗口当前是否处于激活状态。

**说明**
适合在游戏失焦时暂停输入或显示暂停提示。

---

## 7. 声音

### PlayBeep

```cpp
void PlayBeep(int frequency, int duration)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `frequency` | `int` | 频率（Hz） |
| `duration` | `int` | 持续时间（毫秒） |

**返回值**
无

**说明**
Windows Beep，阻塞式蜂鸣。

---

### PlayWAV

```cpp
bool PlayWAV(const char *filename, bool loop = false)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | WAV 文件路径，支持 UTF-8 |
| `loop` | `bool` | 是否循环播放，默认 `false` |

**返回值**
成功返回 `true`，失败返回 `false`。

**说明**
使用 `PlaySoundW` 播放 WAV 文件（异步）。与 `PlayMusic` 独立通道，可同时播放。

---

### StopWAV

```cpp
void StopWAV()
```

**参数**
无

**返回值**
无

**说明**
停止当前 WAV 播放。

---

### PlayMusic

```cpp
bool PlayMusic(const char *filename, bool loop = true)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | 音乐文件路径，支持 MP3/MIDI/WAV，UTF-8 |
| `loop` | `bool` | 是否循环播放，默认 `true` |

**返回值**
成功返回 `true`，失败返回 `false`。

**说明**
使用 MCI 播放背景音乐。按扩展名选择设备类型：`.mp3` → `mpegvideo`，`.mid/.midi` → `sequencer`，`.wav` → `waveaudio`。MIDI 使用 notify 回调重播而非 repeat 命令。同一时刻只能播放一首背景音乐。拒绝包含引号和换行的文件名（防止 MCI 命令注入）。

---

### StopMusic

```cpp
void StopMusic()
```

**参数**
无

**返回值**
无

**说明**
停止当前 MCI 背景音乐并释放资源。

---

### IsMusicPlaying

```cpp
bool IsMusicPlaying() const
```

**参数**
无

**返回值**
当前音乐是否处于播放状态。

**说明**
适合 UI 状态同步。

---

## 8. Tilemap

### CreateTilemap

```cpp
int CreateTilemap(int cols, int rows, int tileSize, int tilesetId)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `cols` | `int` | 地图列数，不超过 4096 |
| `rows` | `int` | 地图行数，不超过 4096 |
| `tileSize` | `int` | 瓦片边长（像素） |
| `tilesetId` | `int` | tileset 精灵 ID |

**返回值**
地图 ID，失败返回 `-1`。

**说明**
创建瓦片地图。tileset 精灵按 `tileSize` 切分为瓦片，编号从 0 开始，从左到右、从上到下排列。所有格子初始化为 `-1`（空）。

---

### SaveTilemap

```cpp
bool SaveTilemap(const char *filename, int mapId) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | 输出文件路径，`.glm` 格式，UTF-8 |
| `mapId` | `int` | 地图 ID |

**返回值**
成功返回 `true`。

**说明**
保存地图为纯文本 `.glm` 文件。第一行 `GLM1`，第二行 `tileSize rows cols`，后续每行瓦片数据。不保存 tileset 信息。

---

### LoadTilemap

```cpp
int LoadTilemap(const char *filename, int tilesetId)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | `.glm` 文件路径，UTF-8 |
| `tilesetId` | `int` | tileset 精灵 ID |

**返回值**
新地图 ID，失败返回 `-1`。

**说明**
从 `.glm` 文件创建地图。文件第一行必须是 `GLM1`。数据行不足时补 `-1`，超出时忽略。不从文件读取 tileset，调用者必须显式提供。

---

### FreeTilemap

```cpp
void FreeTilemap(int mapId)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `mapId` | `int` | 地图 ID |

**返回值**
无

**说明**
释放地图的 tiles 数组，不释放 tileset 精灵。

---

### SetTile

```cpp
void SetTile(int mapId, int col, int row, int tileId)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `mapId` | `int` | 地图 ID |
| `col` | `int` | 瓦片列号 |
| `row` | `int` | 瓦片行号 |
| `tileId` | `int` | 瓦片编号，`-1` 表示空，`< -1` 忽略 |

**返回值**
无

**说明**
设置瓦片。超出 tileset 范围的非负 `tileId` 会原样写入，绘制时自动跳过。

---

### GetTile

```cpp
int GetTile(int mapId, int col, int row) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `mapId` | `int` | 地图 ID |
| `col` | `int` | 瓦片列号 |
| `row` | `int` | 瓦片行号 |

**返回值**
瓦片编号，越界返回 `-1`。

**说明**
读取瓦片。

---

### GetTilemapCols / GetTilemapRows

```cpp
int GetTilemapCols(int mapId) const
int GetTilemapRows(int mapId) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `mapId` | `int` | 地图 ID |

**返回值**
地图列数/行数，无效 ID 返回 `0`。

**说明**
获取地图网格尺寸。

---

### GetTileSize

```cpp
int GetTileSize(int mapId) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `mapId` | `int` | 地图 ID |

**返回值**
瓦片边长（像素），无效 ID 返回 `0`。

**说明**
获取地图瓦片尺寸。

---

### WorldToTileCol / WorldToTileRow

```cpp
int WorldToTileCol(int mapId, int x) const
int WorldToTileRow(int mapId, int y) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `mapId` | `int` | 地图 ID |
| `x` | `int` | 像素 X 坐标 |
| `y` | `int` | 像素 Y 坐标 |

**返回值**
瓦片列号/行号，使用向下取整。

**说明**
像素坐标转瓦片坐标。负坐标也能得到符合直觉的结果。

---

### GetTileAtPixel

```cpp
int GetTileAtPixel(int mapId, int x, int y) const
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `mapId` | `int` | 地图 ID |
| `x` | `int` | 像素 X 坐标 |
| `y` | `int` | 像素 Y 坐标 |

**返回值**
瓦片编号，越界或空返回 `-1`。

**说明**
按像素位置读取瓦片，适合脚下地面检测、像素级碰撞。

---

### FillTileRect

```cpp
void FillTileRect(int mapId, int col, int row, int cols, int rows, int tileId)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `mapId` | `int` | 地图 ID |
| `col` | `int` | 起始列号 |
| `row` | `int` | 起始行号 |
| `cols` | `int` | 填充列数 |
| `rows` | `int` | 填充行数 |
| `tileId` | `int` | 瓦片编号 |

**返回值**
无

**说明**
批量填充矩形瓦片区域。自动裁剪到地图边界。`tileId < -1` 时忽略。

---

### ClearTilemap

```cpp
void ClearTilemap(int mapId, int tileId = -1)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `mapId` | `int` | 地图 ID |
| `tileId` | `int` | 填充瓦片编号，默认 `-1` 表示清空 |

**返回值**
无

**说明**
将整张地图填充为同一个瓦片编号。

---

### DrawTilemap

```cpp
void DrawTilemap(int mapId, int x, int y, int flags = 0)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `mapId` | `int` | 地图 ID |
| `x` | `int` | 地图左上角屏幕位置（卷轴时传 `-cameraX`） |
| `y` | `int` | 地图左上角屏幕位置（卷轴时传 `-cameraY`） |
| `flags` | `int` | 绘制标志，默认 0 |

**返回值**
无

**说明**
绘制瓦片地图。`flags` 与 `DrawSpriteEx` 一致：`0` 不透明快路径，`SPRITE_COLORKEY` 透明色，`SPRITE_ALPHA` Alpha 混合。只绘制裁剪矩形内可见瓦片，自动跳过超出 tileset 范围的格子。

---

## 9. 场景管理

### SetScene

```cpp
void SetScene(int scene)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `scene` | `int` | 场景编号 |

**返回值**
无

**说明**
设置下一帧要切换的场景。不会立即生效，而是在下一次 `Update()` 时处理。可通过 `SetScene(GetScene())` 重启当前场景。

---

### GetScene

```cpp
int GetScene() const
```

**参数**
无

**返回值**
当前场景编号。

**说明**
初始场景为 `0`。

---

### IsSceneChanged

```cpp
bool IsSceneChanged() const
```

**参数**
无

**返回值**
本帧是否刚切换到新场景。新场景第一帧返回 `true`，之后返回 `false`。

**说明**
初始帧也返回 `true`，方便首帧初始化。

---

### GetPreviousScene

```cpp
int GetPreviousScene() const
```

**参数**
无

**返回值**
切换前的场景编号，首帧或未切换时返回 `0`。

**说明**
获取切换前的场景。

---

## 10. UI 控件

### Button

```cpp
bool Button(int x, int y, int w, int h, const char *text, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 按钮左上角 X |
| `y` | `int` | 按钮左上角 Y |
| `w` | `int` | 按钮宽度 |
| `h` | `int` | 按钮高度 |
| `text` | `const char *` | 按钮文字，使用内置 8x8 字体 |
| `color` | `uint32_t` | 按钮基色 |

**返回值**
在按钮内按下并在按钮内松开左键时返回 `true`。

**说明**
立即模式按钮。视觉状态分 `normal`、`hover`、`pressed` 三种。`color` 作为基色，悬停与按下效果由库自动计算。

---

### Checkbox

```cpp
bool Checkbox(int x, int y, const char *text, bool *checked)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 复选框左上角 X |
| `y` | `int` | 复选框左上角 Y |
| `text` | `const char *` | 标签文字 |
| `checked` | `bool *` | 勾选状态指针（必须非空） |

**返回值**
状态变化时返回 `true`（翻转 `checked`）。

**说明**
立即模式复选框。点击区域覆盖 16x16 方框和文字标签。状态分 `checked`、`checked-hover`、`unchecked`、`unchecked-hover` 四种。

---

### RadioBox

```cpp
bool RadioBox(int x, int y, const char *text, int *value, int index)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 单选框左上角 X |
| `y` | `int` | 单选框左上角 Y |
| `text` | `const char *` | 标签文字 |
| `value` | `int *` | 组共享值指针（必须非空） |
| `index` | `int` | 该项编号 |

**返回值**
选中该项时返回 `true`（设置 `*value = index`）。

**说明**
立即模式单选框。同一组共享 `value` 指针实现互斥。点击区域覆盖 16x16 圆形和文字标签。选中时显示中心圆点。

---

### ToggleButton

```cpp
bool ToggleButton(int x, int y, int w, int h, const char *text, bool *toggled, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 按钮左上角 X |
| `y` | `int` | 按钮左上角 Y |
| `w` | `int` | 按钮宽度 |
| `h` | `int` | 按钮高度 |
| `text` | `const char *` | 按钮文字 |
| `toggled` | `bool *` | 开关状态指针（必须非空） |
| `color` | `uint32_t` | 按钮基色 |

**返回值**
状态变化时返回 `true`（翻转 `toggled`）。

**说明**
立即模式开关按钮。`toggled == true` 时持续显示凹陷外观。视觉分 `normal`、`hover`、`toggled`、`toggled-hover` 四种。

---

## 11. 存档读写

所有存档函数为 `static`，可通过 `GameLib::SaveInt(...)` 直接调用，不需要实例。

### SaveInt

```cpp
static bool SaveInt(const char *filename, const char *key, int value)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | 存档文件路径，UTF-8 |
| `key` | `const char *` | 键名（不能为空，不能含 `=`、`\r`、`\n`） |
| `value` | `int` | 整数值 |

**返回值**
成功返回 `true`。

**说明**
将整数写入存档。文件不存在则创建，key 已存在则覆盖。

---

### SaveFloat

```cpp
static bool SaveFloat(const char *filename, const char *key, float value)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | 存档文件路径，UTF-8 |
| `key` | `const char *` | 键名 |
| `value` | `float` | 浮点数值 |

**返回值**
成功返回 `true`。

**说明**
将浮点数写入存档，以 `%g` 格式存储。

---

### SaveString

```cpp
static bool SaveString(const char *filename, const char *key, const char *value)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | 存档文件路径，UTF-8 |
| `key` | `const char *` | 键名 |
| `value` | `const char *` | 字符串值 |

**返回值**
成功返回 `true`。

**说明**
将字符串写入存档。`\` 转义为 `\\`，`\n` 转义为 `\n`，读取时还原。

---

### LoadInt

```cpp
static int LoadInt(const char *filename, const char *key, int defaultValue = 0)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | 存档文件路径，UTF-8 |
| `key` | `const char *` | 键名 |
| `defaultValue` | `int` | 默认值 |

**返回值**
整数值，文件/key 不存在或解析失败返回 `defaultValue`。

**说明**
从存档读取整数。

---

### LoadFloat

```cpp
static float LoadFloat(const char *filename, const char *key, float defaultValue = 0.0f)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | 存档文件路径，UTF-8 |
| `key` | `const char *` | 键名 |
| `defaultValue` | `float` | 默认值 |

**返回值**
浮点数值，不存在返回 `defaultValue`。

**说明**
从存档读取浮点数。

---

### LoadString

```cpp
static const char *LoadString(const char *filename, const char *key, const char *defaultValue = "")
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | 存档文件路径，UTF-8 |
| `key` | `const char *` | 键名 |
| `defaultValue` | `const char *` | 默认值 |

**返回值**
字符串指针，指向内部静态缓冲区（最大 1023 字符），下次调用前有效。

**说明**
从存档读取字符串。不存在返回 `defaultValue`。

---

### HasSaveKey

```cpp
static bool HasSaveKey(const char *filename, const char *key)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | 存档文件路径，UTF-8 |
| `key` | `const char *` | 键名 |

**返回值**
存在返回 `true`。

**说明**
判断存档中是否存在指定 key。

---

### DeleteSaveKey

```cpp
static bool DeleteSaveKey(const char *filename, const char *key)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | 存档文件路径，UTF-8 |
| `key` | `const char *` | 键名 |

**返回值**
成功返回 `true`。

**说明**
从存档删除指定 key。删除后只剩头部则删除整个文件。

---

### DeleteSave

```cpp
static bool DeleteSave(const char *filename)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `filename` | `const char *` | 存档文件路径，UTF-8 |

**返回值**
成功返回 `true`。

**说明**
删除整个存档文件。

---

## 12. 工具函数

所有工具函数为 `static`。

### Random

```cpp
static int Random(int minVal, int maxVal)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `minVal` | `int` | 最小值 |
| `maxVal` | `int` | 最大值 |

**返回值**
`[minVal, maxVal]` 范围内的随机整数。

**说明**
内部使用 `rand()`，构造函数自动 `srand(time(NULL))`。

---

### RectOverlap

```cpp
static bool RectOverlap(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x1` | `int` | 第一个矩形左上角 X |
| `y1` | `int` | 第一个矩形左上角 Y |
| `w1` | `int` | 第一个矩形宽度 |
| `h1` | `int` | 第一个矩形高度 |
| `x2` | `int` | 第二个矩形左上角 X |
| `y2` | `int` | 第二个矩形左上角 Y |
| `w2` | `int` | 第二个矩形宽度 |
| `h2` | `int` | 第二个矩形高度 |

**返回值**
两个矩形是否重叠。

**说明**
AABB 矩形碰撞检测。

---

### CircleOverlap

```cpp
static bool CircleOverlap(int cx1, int cy1, int r1, int cx2, int cy2, int r2)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `cx1` | `int` | 第一个圆心 X |
| `cy1` | `int` | 第一个圆心 Y |
| `r1` | `int` | 第一个半径 |
| `cx2` | `int` | 第二个圆心 X |
| `cy2` | `int` | 第二个圆心 Y |
| `r2` | `int` | 第二个半径 |

**返回值**
两个圆是否重叠。

**说明**
圆形碰撞检测，使用距离平方比较，无浮点开方，`int64_t` 防溢出。

---

### PointInRect

```cpp
static bool PointInRect(int px, int py, int x, int y, int w, int h)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `px` | `int` | 点 X 坐标 |
| `py` | `int` | 点 Y 坐标 |
| `x` | `int` | 矩形左上角 X |
| `y` | `int` | 矩形左上角 Y |
| `w` | `int` | 矩形宽度 |
| `h` | `int` | 矩形高度 |

**返回值**
点是否在矩形内。

**说明**
点矩形碰撞检测。

---

### Distance

```cpp
static float Distance(int x1, int y1, int x2, int y2)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x1` | `int` | 第一个点 X |
| `y1` | `int` | 第一个点 Y |
| `x2` | `int` | 第二个点 X |
| `y2` | `int` | 第二个点 Y |

**返回值**
两点距离（浮点）。

**说明**
使用 `sqrtf` 计算。

---

### DrawGrid

```cpp
void DrawGrid(int x, int y, int rows, int cols, int cellSize, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `x` | `int` | 网格左上角 X |
| `y` | `int` | 网格左上角 Y |
| `rows` | `int` | 行数 |
| `cols` | `int` | 列数 |
| `cellSize` | `int` | 单元格边长 |
| `color` | `uint32_t` | 网格线颜色 |

**返回值**
无

**说明**
绘制网格线。

---

### FillCell

```cpp
void FillCell(int gridX, int gridY, int row, int col, int cellSize, uint32_t color)
```

**参数**
| 参数 | 类型 | 说明 |
|------|------|------|
| `gridX` | `int` | 网格左上角 X |
| `gridY` | `int` | 网格左上角 Y |
| `row` | `int` | 单元格行号 |
| `col` | `int` | 单元格列号 |
| `cellSize` | `int` | 单元格边长 |
| `color` | `uint32_t` | 填充颜色 |

**返回值**
无

**说明**
填充网格单元格，留 1 像素内边距避免覆盖网格线。

---

## 13. 常量参考

### 颜色常量

```cpp
COLOR_BLACK       0xFF000000  // 黑色
COLOR_WHITE       0xFFFFFFFF  // 白色
COLOR_RED         0xFFFF0000  // 红色
COLOR_GREEN       0xFF00FF00  // 绿色
COLOR_BLUE        0xFF0000FF  // 蓝色
COLOR_YELLOW      0xFFFFFF00  // 黄色
COLOR_CYAN        0xFF00FFFF  // 青色
COLOR_MAGENTA     0xFFFF00FF  // 品红
COLOR_ORANGE      0xFFFF8800  // 橙色
COLOR_PINK        0xFFFF88CC  // 粉色
COLOR_PURPLE      0xFF8800FF  // 紫色
COLOR_GRAY        0xFF888888  // 灰色
COLOR_DARK_GRAY   0xFF444444  // 深灰
COLOR_LIGHT_GRAY  0xFFCCCCCC  // 浅灰
COLOR_DARK_RED    0xFF880000  // 深红
COLOR_DARK_GREEN  0xFF008800  // 深绿
COLOR_DARK_BLUE   0xFF000088  // 深蓝
COLOR_SKY_BLUE    0xFF87CEEB  // 天蓝
COLOR_BROWN       0xFF8B4513  // 棕色
COLOR_GOLD        0xFFFFD700  // 金色
COLOR_TRANSPARENT 0x00000000  // 透明
```

### 颜色宏

```cpp
COLOR_RGB(r, g, b)       // 构造不透明颜色
COLOR_ARGB(a, r, g, b)   // 构造 ARGB 颜色
COLOR_GET_A(c)           // 提取 Alpha
COLOR_GET_R(c)           // 提取 Red
COLOR_GET_G(c)           // 提取 Green
COLOR_GET_B(c)           // 提取 Blue
```

### 键盘常量

```cpp
KEY_LEFT    KEY_RIGHT   KEY_UP      KEY_DOWN
KEY_SPACE   KEY_ENTER   KEY_ESCAPE  KEY_TAB
KEY_SHIFT   KEY_CONTROL KEY_BACK
KEY_A ~ KEY_Z
KEY_0 ~ KEY_9
KEY_F1 ~ KEY_F12
```

### 鼠标按键常量

```cpp
MOUSE_LEFT    0  // 左键
MOUSE_RIGHT   1  // 右键
MOUSE_MIDDLE  2  // 中键
```

### 精灵标志

```cpp
SPRITE_FLIP_H     1  // 水平翻转
SPRITE_FLIP_V     2  // 垂直翻转
SPRITE_COLORKEY   4  // 透明色模式
SPRITE_ALPHA      8  // Alpha 混合
COLORKEY_DEFAULT  0xFFFF00FF  // 默认透明色（品红）
```

### 消息框常量

```cpp
MESSAGEBOX_OK           0  // OK 按钮
MESSAGEBOX_YESNO        1  // Yes/No 按钮
MESSAGEBOX_RESULT_OK    1  // 点击 OK
MESSAGEBOX_RESULT_YES   2  // 点击 Yes
MESSAGEBOX_RESULT_NO    3  // 点击 No
```