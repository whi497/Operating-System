#include "io.h"
#include "vga.h"

#define VGA_BASE 0xB8000 // vga 显存起始地址
#define VGA_SCREEN_WIDTH 80 // vga 屏幕宽度（可容纳字符数）
#define VGA_SCREEN_HEIGHT 25 // vga 屏幕高度

#define CURSOR_LINE_REG 0xE // 行号寄存器
#define CURSOR_COL_REG 0xF // 列号寄存器
#define CURSOR_INDEX_PORT 0x3D4 // 光标行列索引端口号
#define CURSOR_DATA_PORT 0x3D5 // 光标数据端口号

/* ========= 以下函数仅供参考，可以根据自己的需求进行修改，甚至删除 ========= */

/* 将光标设定到特定位置
 * 提示：使用 outb */
void set_cursor_pos(unsigned short int pos) {
    /* todo//done */
    unsigned char line = 0;
    unsigned char column = 0;
    line = pos>>8;
    column = pos;
    outb(CURSOR_INDEX_PORT,CURSOR_LINE_REG);//写数据
    outb(CURSOR_DATA_PORT,line);
    outb(CURSOR_INDEX_PORT,CURSOR_COL_REG);
    outb(CURSOR_DATA_PORT,column);
}

/* 获取光标当前所在位置
 * 提示：使用 inb */ 
unsigned short int get_cursor_pos(void) {
    /* todo//done */
    unsigned char line = 0;
    unsigned char column = 0;
    outb(CURSOR_INDEX_PORT,CURSOR_LINE_REG);//读数据
    line=inb(CURSOR_DATA_PORT);
    outb(CURSOR_INDEX_PORT,CURSOR_COL_REG);
    column=inb(CURSOR_DATA_PORT);
    int pos = 0;
    pos = (pos | line)<<8;
    pos = pos | column;
    return pos; 
}

/* 滚屏，vga 屏幕满时使用。丢弃第一行内容，将剩余行整体向上滚动一行
 * 提示：使用指针修改显存 */
void scroll_screen(void) {
    /* todo//done */
    unsigned short int* p = VGA_BASE;
    unsigned short int* q = VGA_BASE+2*VGA_SCREEN_WIDTH;
    for (int i = 0; i < (VGA_SCREEN_WIDTH)*(VGA_SCREEN_HEIGHT-1); i++) {
        *p=*q;
        p++;q++;
    }
    q-=80;
    for(int i=0; i < VGA_SCREEN_WIDTH; i++){//清空最后一行
        *q=0x0F00;
        q++;
    }
    set_cursor_pos(80*24);
}

/* 向 vga 的特定光标位置 pos 输出一个字符
 * 提示：使用指针修改显存 */
void put_char2pos(unsigned char c, int color, unsigned short int pos) {
    /* todo//done */
    unsigned short int* p=pos*2+VGA_BASE;
    unsigned short int data=0;
    if(c=='\n') {
        if(pos/80==24) scroll_screen();//光标在最底端换行
        else set_cursor_pos((pos/80+1)*80);//换行
    }
    else{
        data = data | (color<<8);
        data = data | c;
        *p=data;
    }
}
/* ========= 以下函数接口禁止修改 ========= */

/* 清除屏幕上所有字符，并将光标位置重置到顶格
 * 提示：使用指针修改显存 */
void clear_screen(void) {
    /* todo//done */
    unsigned short int* p = VGA_BASE;
    for (int i = 0; i < VGA_SCREEN_WIDTH * VGA_SCREEN_HEIGHT; i++){
        *p=0x0F00;//黑底白标
        p++;
    }
    set_cursor_pos(0);
}

/* 向 vga 的当前光标位置输出一个字符串，并移动光标位置到串末尾字符的下一位
 * 如果超出了屏幕范围，则需要滚屏
 * 需要能够处理转义字符 \n */
void append2screen(char *str, int color) { //换行和滚屏由put_char2pos()内部处理
    /* todo//done */
    for (int i = 0;str[i]!='\0';i++){
        int pos = get_cursor_pos();
        put_char2pos(str[i],color,pos);
        if(str[i] != '\n'){
            if(pos==80*25-1) scroll_screen();//已写入最后一个位置
            else set_cursor_pos(pos+1);
        }
    }
}