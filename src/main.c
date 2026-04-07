#include <HIS_includes.h>


int main(void) {
    // 1. 先初始化窗口（必须最先执行）
    InitWindow(400, 200, "只是一个测试");
    SetTargetFPS(60);

    // 2. 窗口出来后，再加载中文资源
    InitChineseFont("pingfang.otf", 32);
    SetTextureFilter(GlobalChineseFont.texture, TEXTURE_FILTER_BILINEAR);

    // 3. 告诉 raygui 使用这个中文字体
    GuiSetFont(GlobalChineseFont);
    // 可选：设置 GUI 文字大小
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    bool showMessageBox = false;

    while (!WindowShouldClose()) {
        // ... 你的逻辑 ...
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        // 按钮里现在可以写中文了
        if (GuiButton((Rectangle){ 24, 24, 120, 30 }, "显示弹窗")) showMessageBox = true;

        if (showMessageBox) {
            int result = GuiMessageBox((Rectangle){ 85, 70, 250, 100 },
                "#191#提示", "这是一个中文弹窗！", "确定;取消");

            if (result >= 0) showMessageBox = false;
        }

        EndDrawing();
    }

    // 卸载资源
    UnloadFont(GlobalChineseFont);
    CloseWindow();
    return 0;
}