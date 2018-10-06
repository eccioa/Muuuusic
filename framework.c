#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>


#include "console_draw3.h"
#include "kb_input2.h"
#include "font.h"
#include "framework.h"
#include "image.h"

/*----------------------------------------------------------*/


int IsOnItem(Menu *m, int index)
{
    return (m->state[index] & 1); /* 用位元運算來判斷目前選擇的項目 */
}
int IsItemSelected(Menu *m, int index)
{
    return (m->state[index] & 2);
}
void showMenu(Menu *m)
{
    int i;
    int fg;
    char *str;

    for (i=0; i< m->num_options; i++) {

        if ( !IsOnItem(m, i) ) {
            fg = m->fgcolor;
        } else {
            fg = m->bgcolor;
        }

        str = m->text[i];

        if (i == 2)
            putStringLarge(m->large_font, (m->x)*(m->large_font->width+2)+1,
                (m->y+i)*(m->large_font->height+3), str, fg);
        else
            putStringLarge(m->large_font, (m->x)*(m->large_font->width+2)-3,
                (m->y+i)*(m->large_font->height+3), str, fg);
    }
}
void showSongmenu(Menu *s)
{
    int i, k;
    int fg;
    char *str;
    Image *p01 = read_image("images\\p01.pixel", "images\\p01.color");

    for (i=0; i<5; i++)
    {
        if (s->cursor-2+i < 0 || s->cursor-2+i >= s->num_options)
            continue;

        if (i == 2)k = 0;
        else if (i == 1 || i == 3)k = 1;
        else k = 2;
        if (IsItemSelected(s, s->cursor - 2 + i))k -= 4;
        show_image_no_space(p01, 330 + 4*k, 5 + 24*i);

        str = s->text[s->cursor - 2 + i];
        putStringLarge_gray(s->large_font, 328 + 4*k+8,
                        8 + 24*i, str, 0);
        str = s->text_2[s->cursor - 2 + i];
        putStringLarge_gray(s->large_font, 328 + 4*k+8,
                        14 + 24*i, str, 0);
        str = s->text_3[s->cursor - 2 + i];
        putStringLarge_gray(s->large_font, 328 + 4*k+8,
                        20 + 24*i, str, 0);

    }
}

void showStopmenu(Menu *p)
{
    int i, k;
    int fg;
    char *str;
    Image *p02 = read_image("images\\p02.pixel", "images\\p02.color");

    show_image_gray(p02, 203, 50);
    for (i=0;i<3;i++){
        if (!IsOnItem(p, i)){
            fg = p->fgcolor;
        }else{
            fg = p->bgcolor;
        }

        str = p->text[i];

        if(i == 0){
            putStringLarge_gray(p->large_font, 209, 55, str, fg);
        }else{
            putStringLarge_gray(p->large_font, 213, 55 + i*8, str, fg);
        }

    }
}

void scrollMenu(Menu *m, int diff)
{
    m->state[m->cursor] = m->state[m->cursor] & (~1);  /* 把目前游標所在的選項狀態清除 */
    m->cursor = (m->cursor + diff + m->num_options) % m->num_options; /* 把遊標移到下一個選項 */
    m->state[m->cursor] = m->state[m->cursor] | 1; /* 選擇目前游標所在的選項 */
}

int radioMenu(Menu *m)
{
    int i;
    if (IsItemSelected(m, m->cursor))return 0;

    for (i=0; i<m->num_options; i++) {
        m->state[i] = m->state[i] & (~2); /* 清掉全部的選項 */
    }
    m->state[m->cursor] = m->state[m->cursor] | 2; /* 設定目前游標 */
}
void toggleMenu(Menu *m)
{
    m->state[m->cursor] = m->state[m->cursor] ^ 2;  /* 利用位元運算 產生 toggle 的效果 */
}




