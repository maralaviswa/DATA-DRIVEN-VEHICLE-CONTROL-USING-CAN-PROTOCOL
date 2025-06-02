#ifndef LCD_H
#define LCD_H


#include"type.h"
void init_LCD(void);
void cmd_LCD(u32);
void data_LCD(u32);
void write_LCD(u32);
void num_LCD(u32);
void U32LCD(u32);
void str_LCD(s8 *);
void S32LCD(s32 num);
void F32LCD(f32 fNum,u32 nDP);
void BuildCGRAM(u8 *p,u8 num);

#endif


