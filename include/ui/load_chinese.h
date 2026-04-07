#ifndef LOAD_CHINESE_H
#define LOAD_CHINESE_H

#include <HIS_core.h>
#include <HIS_ui.h>

// 全局变量声明：增加 static 确保它只在包含它的文件内可见
// 注意：如果多个 .c 文件都要用，建议看下方的“方案 2”
static Font GlobalChineseFont;

// 增加 static inline，解决 IDE 提示的 inline 说明符问题，并防止重复定义报错
static inline void InitChineseFont(const char* fontPath, int fontSize) {
    int codepointsCount = (0x9FA5 - 0x4E00) + 256;
    int *codepoints = (int *)malloc(codepointsCount * sizeof(int));

    for (int i = 0; i < 256; i++) codepoints[i] = i;
    for (int i = 256; i < codepointsCount; i++) {
        codepoints[i] = 0x4E00 + (i - 256);
    }

    GlobalChineseFont = LoadFontEx(fontPath, fontSize, codepoints, codepointsCount);
    free(codepoints);
}

static inline void DrawTextCN(const char* text, float x, float y, float size, Color color) {
    // 只要包含了 raylib.h，这里的 (Vector2){x, y} 就会恢复正常
    DrawTextEx(GlobalChineseFont, text, (Vector2){x, y}, size, 2, color);
}

#endif