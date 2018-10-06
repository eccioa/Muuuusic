#include "font.h"
#ifndef FRAMEWORK_
#define FRAMEWORK_
struct t_menu{
    int x; // X 越大，選項越偏右
    int y; // Y 越大，選項越偏下
    int fgcolor; //前景顏色
    int bgcolor; //背景顏色
    int cursor;  //游標位置
    int num_options; // 選項個數
    int state[15];
    char text[15][30]; // 遊戲選項的文字敘述
    char text_2[15][30];
    char text_3[15][30];
    Font *large_font;  //控制遊戲選項的字體
};
typedef struct t_menu Menu;

extern void showMenu(Menu *m);


extern int IsOnItem(Menu *m, int index);
extern int IsItemSelected(Menu *m, int index);
extern void showMenu(Menu *m);
extern void showSongmenu(Menu *s);
extern void showStopmenu(Menu *p);
extern void scrollMenu(Menu *m, int diff);
extern int radioMenu(Menu *m);
extern void toggleMenu(Menu *m);


#endif

