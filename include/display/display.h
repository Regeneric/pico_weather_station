#pragma once
#include <defines.h>

#include <core/logger.h>
#include <comm/i2c.h>

enum {
    DISP_DRAW,
    DISP_SLEEP,
};

typedef struct DisplayConfig_t {
    u8 width;
    u8 height;
    u8 address;
    u8 textSize;
    vu8 status;
} DisplayConfig_t;

b8  Display_Init(I2C_Config_t* i2c, DisplayConfig_t* config);
u32 Display_WriteCommand(u8 command);
u32 Display_WriteCommandList(const u8* commands, size_t len);
u32 Display_WriteData(const u8* data, size_t len);

void GFX_ClearBuffer(void);
void GFX_Display(void);
void GFX_DrawPixel(u8 x, u8 y, u8 color);
void GFX_DrawLine(u8 x0, u8 y0, u8 x1, u8 y1, u8 color);
void GFX_DrawFastHLine(u8 x, u8 y, u8 w, u8 color);
void GFX_DrawFastVLine(u8 x, u8 y, u8 h, u8 color);
void GFX_DrawRect(u8 x, u8 y, u8 w, u8 h, u8 color);
void GFX_FillRect(u8 x, u8 y, u8 w, u8 h, u8 color);
void GFX_FillFastRect(u8 x, u8 y, u8 w, u8 h, u8 color);
void GFX_DrawFastChar(u8 x, u8 y, u8 c);
void GFX_DrawChar(u8 x, u8 y, u8 c, u8 color, u8 size);
void GFX_DrawFastString(u8 x, u8 y, const char* str);
void GFX_DrawString(u8 x, u8 y, const char* str, u8 color, u8 size);