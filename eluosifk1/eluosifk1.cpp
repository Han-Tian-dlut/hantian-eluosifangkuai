#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <string>

// 游戏区域大小
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int BLOCK_SIZE = 30; // 每个方块的像素大小
// 方块颜色
COLORREF colors[8] = {
    RGB(0, 0, 0),       // 黑色，背景
    RGB(0, 255, 255),    // 青色，I型方块
    RGB(0, 0, 255),      // 蓝色，J型方块
    RGB(255, 165, 0),    // 橙色，L型方块
    RGB(255, 255, 0),    // 黄色，O型方块
    RGB(0, 255, 0),      // 绿色，S型方块ss
    RGB(128, 0, 128),    // 紫色，T型方块
    RGB(255, 0, 0)       // 红色，Z型方块
};

// 方块形状定义
int shapes[7][4][4][4] = {
    // I型方块
    {
        {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
        {{0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0}},
        {{0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}}
    },
    // J型方块
    {
        {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0}}
    },
    // L型方块
    {
        {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0}},
        {{1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}}
    },
    // O型方块
    {
        {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}
    },
    // S型方块
    {
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0}},
        {{1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}
    },
    // T型方块
    {
        {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}
    },
    // Z型方块
    {
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0}},
        {{0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}}
    }
};

// 游戏板
int board[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };

// 当前方块
int currentPiece[4][4];
int currentX, currentY;
int currentType, currentRotation;
COLORREF currentColor;

// 下一个方块
int nextPiece[4][4];
int nextType, nextRotation;
COLORREF nextColor;

// 游戏状态
int score = 0;
int level = 1;
int linesCleared = 0;
bool gameOver = false;
bool paused = false;

// 初始化游戏
void initGame() {
    // 清空游戏板
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            board[y][x] = 0;
        }
    }

    // 重置游戏状态
    score = 0;
    level = 1;
    linesCleared = 0;
    gameOver = false;
    paused = false;

    // 初始化随机数种子
    srand((unsigned)time(NULL));

    // 生成第一个方块和下一个方块
    currentType = rand() % 7;
    currentRotation = 0;
    currentColor = colors[currentType + 1];
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            currentPiece[y][x] = shapes[currentType][currentRotation][y][x];
        }
    }
    currentX = BOARD_WIDTH / 2 - 2;
    currentY = 0;

    nextType = rand() % 7;
    nextRotation = 0;
    nextColor = colors[nextType + 1];
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            nextPiece[y][x] = shapes[nextType][nextRotation][y][x];
        }
    }
}

// 检查碰撞
bool checkCollision(int piece[4][4], int x, int y) {
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (piece[py][px] > 0) {
                int newX = x + px;
                int newY = y + py;

                // 检查是否超出边界
                if (newX < 0 || newX >= BOARD_WIDTH || newY >= BOARD_HEIGHT) {
                    return true;
                }

                // 检查是否与已有方块重叠
                if (newY >= 0 && board[newY][newX] > 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

// 旋转方块
void rotatePiece() {
    int newRotation = (currentRotation + 1) % 4;
    int newPiece[4][4];

    // 获取旋转后的方块形状
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            newPiece[y][x] = shapes[currentType][newRotation][y][x];
        }
    }

    // 检查旋转后是否会碰撞
    if (!checkCollision(newPiece, currentX, currentY)) {
        currentRotation = newRotation;
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                currentPiece[y][x] = newPiece[y][x];
            }
        }
    }
}

// 将当前方块固定到游戏板上
void lockPiece() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (currentPiece[y][x] > 0 && currentY + y >= 0) {
                board[currentY + y][currentX + x] = currentType + 1;
            }
        }
    }

    // 检查是否有完整的行可以消除
    int linesToClear = 0;
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        bool lineComplete = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board[y][x] == 0) {
                lineComplete = false;
                break;
            }
        }

        if (lineComplete) {
            linesToClear++;
            // 将上面的行下移
            for (int ny = y; ny > 0; ny--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    board[ny][x] = board[ny - 1][x];
                }
            }
            // 清空最顶行
            for (int x = 0; x < BOARD_WIDTH; x++) {
                board[0][x] = 0;
            }
            y++; // 重新检查当前行，因为上面的行已经下移
        }
    }

    // 更新分数
    if (linesToClear > 0) {
        switch (linesToClear) {
        case 1: score += 100 * level; break;
        case 2: score += 300 * level; break;
        case 3: score += 500 * level; break;
        case 4: score += 800 * level; break;
        }
        linesCleared += linesToClear;
        level = linesCleared / 10 + 1;
    }

    // 生成新方块
    currentType = nextType;
    currentRotation = nextRotation;
    currentColor = nextColor;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            currentPiece[y][x] = nextPiece[y][x];
        }
    }
    currentX = BOARD_WIDTH / 2 - 2;
    currentY = 0;

    // 生成下一个方块
    nextType = rand() % 7;
    nextRotation = 0;
    nextColor = colors[nextType + 1];
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            nextPiece[y][x] = shapes[nextType][nextRotation][y][x];
        }
    }

    // 检查游戏是否结束
    if (checkCollision(currentPiece, currentX, currentY)) {
        gameOver = true;
    }
}

// 处理键盘输入
void handleInput() {
    ExMessage msg;
    while (peekmessage(&msg, EX_KEY)) {
        if (gameOver) {
            if (msg.message == WM_KEYDOWN && (msg.vkcode == 'R' || msg.vkcode == 'r')) {
                initGame();
                continue;
            }
            if (msg.message == WM_KEYDOWN)
            {
                exit(0);
            }
            continue;
        }

        if (paused) {
            if (msg.message == WM_KEYDOWN && (msg.vkcode == 'P' || msg.vkcode == 'p')) {
                paused = false;
            }
            continue;
        }

        if (msg.message == WM_KEYDOWN) {
            switch (msg.vkcode) {
            case 'A':
            case 'a':
            case VK_LEFT: // 左箭头
                if (!checkCollision(currentPiece, currentX - 1, currentY)) {
                    currentX--;
                }
                break;
            case 'D':
            case 'd':
            case VK_RIGHT: // 右箭头
                if (!checkCollision(currentPiece, currentX + 1, currentY)) {
                    currentX++;
                }
                break;
            case 'S':
            case 's':
            case VK_DOWN: // 下箭头
                if (!checkCollision(currentPiece, currentX, currentY + 1)) {
                    currentY++;
                }
                break;
            case 'W':
            case 'w':
            case VK_UP: // 上箭头
                rotatePiece();
                break;
            case VK_SPACE: // 空格键，直接落到底部
                while (!checkCollision(currentPiece, currentX, currentY + 1)) {
                    currentY++;
                }
                lockPiece();
                break;
            case 'P':
            case 'p':
                paused = true;
                break;
            }
        }
    }
}

// 绘制文本信息
void drawTextInfo(IMAGE* pBuffer) {
    // 设置文字样式
    settextstyle(20, 0, _T("宋体"));
    settextcolor(WHITE);

    // 信息区域位置
    int infoX = BOARD_WIDTH * BLOCK_SIZE + 20;
    int infoY = 20;

    // 绘制分数
    std::string scoreStr = "分数: " + std::to_string(score);
    outtextxy(infoX, infoY, scoreStr.c_str());

    // 绘制等级
    std::string levelStr = "等级: " + std::to_string(level);
    outtextxy(infoX, infoY + 30, levelStr.c_str());

    // 绘制消行数
    std::string linesStr = "消行: " + std::to_string(linesCleared);
    outtextxy(infoX, infoY + 60, linesStr.c_str());

    // 绘制下一个方块标题
    outtextxy(infoX, infoY + 100, _T("下一个:"));

    // 绘制操作说明
    outtextxy(infoX, infoY + 280, _T("操作说明:"));
    outtextxy(infoX, infoY + 310, _T("左: A/←"));
    outtextxy(infoX, infoY + 340, _T("右: D/→"));
    outtextxy(infoX, infoY + 370, _T("下: S/↓"));
    outtextxy(infoX, infoY + 400, _T("旋转: W/↑"));
    outtextxy(infoX, infoY + 430, _T("直接下落: 空格"));
    outtextxy(infoX, infoY + 460, _T("暂停: P"));

    // 如果游戏结束，显示游戏结束信息
    if (gameOver) {
        settextcolor(RED);
        settextstyle(30, 0, _T("宋体"));
        outtextxy(BOARD_WIDTH * BLOCK_SIZE / 2 - 100, BOARD_HEIGHT * BLOCK_SIZE / 2 - 30, _T("游戏结束!"));
        settextstyle(20, 0, _T("宋体"));
        outtextxy(BOARD_WIDTH * BLOCK_SIZE / 2 - 80, BOARD_HEIGHT * BLOCK_SIZE / 2 + 20, _T("按R键重新开始"));
    }

    // 如果游戏暂停，显示暂停信息
    if (paused) {
        settextcolor(YELLOW);
        settextstyle(30, 0, _T("宋体"));
        outtextxy(BOARD_WIDTH * BLOCK_SIZE / 2 - 60, BOARD_HEIGHT * BLOCK_SIZE / 2 - 30, _T("游戏暂停"));
    }
}

// 绘制游戏界面到缓冲区
void drawGameToBuffer(IMAGE* pBuffer) {
    // 设置缓冲区为当前绘图目标
    SetWorkingImage(pBuffer);

    // 清空缓冲区
    cleardevice();

    // 绘制游戏区域边框
    setlinecolor(WHITE);
    rectangle(0, 0, BOARD_WIDTH * BLOCK_SIZE, BOARD_HEIGHT * BLOCK_SIZE);

    // 绘制游戏板上的方块
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board[y][x] > 0) {
                setfillcolor(colors[board[y][x]]);
                fillrectangle(x * BLOCK_SIZE, y * BLOCK_SIZE,
                    (x + 1) * BLOCK_SIZE, (y + 1) * BLOCK_SIZE);
            }
        }
    }

    // 绘制当前方块
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (currentPiece[y][x] > 0) {
                setfillcolor(currentColor);
                fillrectangle((currentX + x) * BLOCK_SIZE, (currentY + y) * BLOCK_SIZE,
                    (currentX + x + 1) * BLOCK_SIZE, (currentY + y + 1) * BLOCK_SIZE);
            }
        }
    }

    // 绘制下一个方块预览区域
    int nextX = BOARD_WIDTH * BLOCK_SIZE + 20;
    int nextY = 150;

    setlinecolor(WHITE);
    rectangle(nextX, nextY, nextX + 4 * BLOCK_SIZE, nextY + 4 * BLOCK_SIZE);

    // 绘制下一个方块
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (nextPiece[y][x] > 0) {
                setfillcolor(nextColor);
                fillrectangle(nextX + x * BLOCK_SIZE, nextY + y * BLOCK_SIZE,
                    nextX + (x + 1) * BLOCK_SIZE, nextY + (y + 1) * BLOCK_SIZE);
            }
        }
    }

    // 绘制文本信息
    drawTextInfo(pBuffer);

    // 恢复绘图目标到默认窗口
    SetWorkingImage(NULL);
}

// 游戏主循环
void gameLoop(IMAGE* pBuffer) {
    static int counter = 0;
    int speed = 20 - level; // 速度随等级提高而加快
    if (speed < 1) speed = 1;

    if (!gameOver && !paused) {
        counter++;
        if (counter >= speed) {
            counter = 0;
            if (!checkCollision(currentPiece, currentX, currentY + 1)) {
                currentY++;
            }
            else {
                lockPiece();
            }
        }
    }

    handleInput();

    // 绘制到缓冲区
    drawGameToBuffer(pBuffer);

    // 将缓冲区内容输出到屏幕
    putimage(0, 0, pBuffer);
}

int main() {
    // 初始化图形窗口
    initgraph(BOARD_WIDTH * BLOCK_SIZE + 200, BOARD_HEIGHT * BLOCK_SIZE);
    setbkcolor(BLACK);
    cleardevice();

    // 创建双缓冲图像
    IMAGE buffer(BOARD_WIDTH * BLOCK_SIZE + 200, BOARD_HEIGHT * BLOCK_SIZE);

    // 初始化游戏
    initGame();

    // 游戏主循环
    while (true) {
        gameLoop(&buffer);
        Sleep(50); // 控制游戏速度
    }

    closegraph();
    return 0;
}