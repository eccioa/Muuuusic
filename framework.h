#include "font.h"
#ifndef FRAMEWORK_
#define FRAMEWORK_
struct t_menu{
    int x; // X �V�j�A�ﶵ�V���k
    int y; // Y �V�j�A�ﶵ�V���U
    int fgcolor; //�e���C��
    int bgcolor; //�I���C��
    int cursor;  //��Ц�m
    int num_options; // �ﶵ�Ӽ�
    int state[15];
    char text[15][30]; // �C���ﶵ����r�ԭz
    char text_2[15][30];
    char text_3[15][30];
    Font *large_font;  //����C���ﶵ���r��
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

