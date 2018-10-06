/* 0: 黑     1: 暗藍   2: 暗綠   3: 暗青
   4: 暗紅   5: 暗紫   6: 暗黃   7: 淺灰
   8: 暗灰   9: 亮藍  10: 亮綠  11: 亮青
  12: 亮紅  13: 亮紫  14: 亮黃  15: 白*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>

#include "audio.h"
#include "console_draw3.h"
#include "kb_input2.h"
#include "font.h"
#include "image.h"
#include "framework.h"
#define NUM_KEYS 8
#define REFRESH_RATE 20
#define OFFSET_X  5
#define OFFSET_Y  5
#define DELAY 6

int timer(void);
void setMainmenu(Menu *m);
void setSongmenu(Menu *s);
void inGame();
void songChoosing();
void gameStart();
void loadRhythm(int num);
void ptNote();
int refresh(int count);
void judge(int i);


int perfect, great, miss;
int maxcombo, combo, score;
int last_time, cur_time, interval;
int restart;
int effect0[4];
int effect1[4];
int notes[20][4];
char musicFileName[30];
int rhythm[1000000] = {0};
Menu songmenu;

typedef struct{
    char name[30];
    char author[20];
    int numOfNotes;
    int lenOfSong;
    int delay;
    int ms;
    int n;
    int level;
}song;


song songs[]={
/*   name                        author         numofn  len     delay   ms    n      level*/
    {"Confession Balloon",      "JAY CHOU",     389,    5083,   50,    41,   38     ,"lv 5"},
    {"Flower Dance",            "DJ OKAWARI",   1086,   7000,    0,    37,   49     ,"lv 8"},
    {"We dont talk anymore",    "CHARLIE PUTH", 576,    5900,    5,    37,   50     ,"lv 6"},
    {"Chiruchiru",              "REOL",         591,    6700,    0,    29,   68     ,"lv 6"},
    {"Revival",                 "TQ",           533,    5500,    0,    27,   40     ,"lv 9"}
};

int main(void)
{

    system("mode con cols=480 lines=132");
    system("chcp 1200");

    srand(time(NULL));
    int IsEnding = 0;
    int k;
    int key_down[NUM_KEYS] = {0};
    int key_val[NUM_KEYS] = {VK_UP, VK_DOWN, VK_ESCAPE, VK_RETURN, VK_SPACE};
    int count = 0;
    int i;
    initializeKeyInput();

    /*----------------------載入音效檔----------------------*/
    char soundFileName[50];
    Audio sounds[9];
    for(i=0;i<4;i++){
        sprintf(soundFileName, "sounds\\sound%d.wav", i);
        openAudioFile(soundFileName, &sounds[i]);
    }

    /*---------------------開場動畫準備---------------------*/
    Image *background = read_image("title0.pixel", "title0.color");
    clearScreen();
    show_image(background,0,14);
    //show_image_extra(p01, 322,61);
    saveScreen();

    Font *F = read_font("font.txt");
    char showEnter[30] = "PRESS ENTER TO CONTINUE";
    putStringLarge(F, 149, 73, showEnter, 7);

    drawCmdWindow();

    setSongmenu(&songmenu);
    playMusic(songmenu.cursor);


    /*----------------按下ENTER進入主選單-------------------*/
    cur_time = last_time = timer();
    while(!IsEnding){
        cur_time = timer();
        if(cur_time - last_time > 1000){
            count ++;
            last_time = cur_time;
            clearScreen();
            restoreScreen();
            if(count%2 == 0)
                putStringLarge(F, 148, 73, showEnter, 7);
            drawCmdWindow();
        }
        for (k=0; k<NUM_KEYS; k++) {
            if(KEY_DOWN(key_val[k]) && !key_down[k]) {
                key_down[k] = 1;
                if(key_val[k] == VK_RETURN)IsEnding = 1;
            }
            if(KEY_UP(key_val[k]) && key_down[k]) key_down[k] = 0;
        }
    }


    /*-----------------------過場動畫-----------------------*/
    playAudio(&sounds[1]);

    cur_time = last_time = timer();
    count = 0;
    while(count < 14){
        cur_time = timer();
        if(cur_time - last_time > 50){
            count ++;
            last_time = cur_time;
            show_image(background,0,14 - count);
            drawCmdWindow();
        }
    }


    /*------------------------主選單------------------------*/
    IsEnding = 0;
    Menu mainmenu;
    setMainmenu(&mainmenu);


    show_image(background,0,0);
    showMenu(&mainmenu);
    drawCmdWindow();
    cur_time = last_time = timer();


//    Audio M;
//    char str[30];
//    sprintf(str, "musics\\music%d.wav", rand()%5);
//    openAudioFile(str, &M);
//    playAudio(&M);

    while(!IsEnding){
        cur_time = timer();

        if(cur_time - last_time > 100){
            last_time = cur_time;
            clearScreen();
            show_image(background,0,0);
            showMenu(&mainmenu);
            drawCmdWindow();
        }
        for (k=0; k<NUM_KEYS; k++) {
            if(KEY_DOWN(key_val[k]) && !key_down[k]) {
                key_down[k] = 1;
                switch (key_val[k]) {
                    case VK_UP:
                        scrollMenu(&mainmenu, -1);
                        playAudio(&sounds[2]);
                        break;
                    case VK_DOWN:
                        scrollMenu(&mainmenu, +1);
                        playAudio(&sounds[2]);
                        break;
                    case VK_RETURN:
                        if(mainmenu.cursor==0){
                            Sleep(300);
                            songChoosing();
                            Sleep(300);
                        }
                        else if(mainmenu.cursor==1){
                            about();
                            Sleep(300);
                        }
                        else if(mainmenu.cursor==2)IsEnding = 1;
                        break;
                    case VK_ESCAPE:
                        IsEnding = 1;
                        break;
                }
            }
            if(KEY_UP(key_val[k]) && key_down[k])key_down[k] = 0;
        }
    }
    return 0;
}
int timer(void)
{
    return (clock()/(0.001*CLOCKS_PER_SEC));
}
void setMainmenu(Menu *m)
{
    int i;
    m->large_font = read_font("font.txt");
    m->x = 22;
    m->y = 8;
    m->fgcolor = 7;
    m->bgcolor = 0;
    m->num_options = 3;
    m->cursor = 0;
    for (i=0; i<m->num_options; i++) {
        m->state[i] = 0;
    }
    m->state[m->cursor] = m->state[m->cursor] | 1;  /* 目前選擇的項目 */

    strcpy(m->text[0], " START");
    //strcpy(m->text[1], "OPTIONS");
    strcpy(m->text[1], " ABOUT");
    strcpy(m->text[2], " EXIT");
}
void setSongmenu(Menu *s)
{
    int i;
    s->large_font = read_font("font.txt");
    s->fgcolor = 15;
    s->num_options = 5;
    s->cursor = rand()%(s->num_options);
    for (i=0; i<s->num_options; i++) {
        s->state[i] = 0;
        strcpy(s->text[i], songs[i].name);
        strcpy(s->text_2[i], songs[i].author);
        strcpy(s->text_3[i], songs[i].level);
    }
    s->state[s->cursor] = s->state[s->cursor] | 1;
}

void setStopmenu(Menu *s)
{
    int i;
    s->large_font = read_font("font.txt");
    s->fgcolor = 8;
    s->bgcolor = 0;
    s->num_options = 3;
    s->cursor = 0;
    strcpy(s->text[0], " RESUME");
    strcpy(s->text[1], "RESTART");
    strcpy(s->text[2], " SONGS");
    for (i=0; i<s->num_options; i++) {
        s->state[i] = 0;
    }

    s->state[s->cursor] = s->state[s->cursor] | 1;
}
void about(){
    clearScreen();
    char w1[] = "    muuuusic";
    char w2[] = "    version 2";
    char w3[] = "    producer";
    char w4[] = "Nerien 105062123";
    char w5[] = "Kronos 105062139";
    putStringLarge(songmenu.large_font, 180, 30, w1, 0);
    putStringLarge(songmenu.large_font, 176, 40, w2, 0);
    putStringLarge(songmenu.large_font, 180, 55, w3, 0);
    putStringLarge(songmenu.large_font, 180, 65, w4, 0);
    putStringLarge(songmenu.large_font, 180, 75, w5, 0);
    drawCmdWindow();
    while(1){
        if (KEY_DOWN(VK_ESCAPE))break;
    }
}

void songChoosing()
{
    int IsEnding = 0;
    int UP = 0, DOWN = 0;
    int k, i;

    radioMenu(&songmenu);
    Image *ima[20];

    /*----------------載入歌曲圖片----------------*/
    char ch[30];
    char ch_2[30];
    for (i=0; i<5; i++){
        sprintf(ch, "images\\ima%d.pixel", i);
        sprintf(ch_2, "images\\ima%d.color", i);
        ima[i] = read_image(ch, ch_2);
    }
    /*--------------------------------------------*/
    clearScreen();
    cur_time = last_time = timer();
    int key_down[NUM_KEYS] = {0};
    int music_pre = songmenu.cursor, song_changing = 0;
    int key_val[NUM_KEYS] = {VK_UP, VK_DOWN, VK_ESCAPE, VK_RETURN, VK_SPACE};
    while(!IsEnding){
        cur_time = timer();

        if (cur_time - last_time > 120){
            clearScreen();
            if (UP){
                scrollMenu(&songmenu, -1);
                //playAudio(&sounds[2]);
            }else if(DOWN){
                scrollMenu(&songmenu, -1);
                //playAudio(&sounds[2]);
            }
            last_time = cur_time;
            for (i=0; i<songmenu.num_options; i++){
                if (songmenu.state[i] & 2){
                    if (song_changing){
                        playMusic(songmenu.cursor);
                        song_changing = 0;
                    }
                    show_image(ima[i], 0, 3);
                }
            }

            showSongmenu(&songmenu);
            drawCmdWindow();
        }
        for (k=0; k<NUM_KEYS; k++) {
            if(KEY_DOWN(key_val[k]) && !key_down[k]) {
                key_down[k] = 1;
                switch (key_val[k]) {
                    case VK_UP:
                        scrollMenu(&songmenu, -1);
                        break;
                    case VK_DOWN:
                        scrollMenu(&songmenu, +1);
                        break;
                    case VK_RETURN:
                        if(radioMenu(&songmenu))
                            song_changing = 1;
                        else{
                            do{
                                gameStart();
                            }while(restart);
                            Sleep(250);
                        }
                        clearScreen();
                        break;
                    case VK_ESCAPE:
                        IsEnding = 1;
                        break;
                }
            }
            if(KEY_UP(key_val[k]) && key_down[k]) {
                key_down[k] = 0;
                switch (key_val[k]) {
                    case VK_UP:
                        UP = 0;
                        break;
                    case VK_DOWN:
                        DOWN = 0;
                        break;
                }
            }
        }
    }
}

void gameStart(){
    mciSendString("close mymusic", NULL, 0, NULL);
    int i, k;

    restart = 0;
    perfect = 0;
    great = 0;
    miss = 0;
    combo = 0;
    maxcombo = 0;
    score = 0;
    memset(effect0, 0, sizeof(effect0));
    memset(effect1, 0, sizeof(effect1));
    memset(notes, 0, sizeof(int));
    Font *ff = read_font("font.txt");
    Menu stopmenu;
    setStopmenu(&stopmenu);

    clearScreen();
    int IsEnding = 0, count = 0;

    int key_down[NUM_KEYS] = {0};
    int key_val[NUM_KEYS] = {VK_D, VK_F, VK_J, VK_K, VK_ESCAPE, VK_RETURN, VK_UP, VK_DOWN};
    int keys[8] = {0};

    char ch[30];
    char ch_2[30];
    sprintf(ch, "images\\ima%d.pixel", songmenu.cursor);
    sprintf(ch_2, "images\\ima%d.color", songmenu.cursor);

    Image *ef0[2];
    ef0[0] = read_image("images\\ef00.pixel", "images\\ef00.color");
    ef0[1] = read_image("images\\ef01.pixel", "images\\ef01.color");
    Image *ef1 = read_image("images\\ef00.pixel", "images\\ef10.color");

    Image *ima = read_image(ch, ch_2);
    Image *itr0 = read_image("images\\itr0.pixel",  "images\\itr0.color");
    Image *itr1 = read_image("images\\itr11.pixel", "images\\itr2.color");
    Image *itr2 = read_image("images\\itr11.pixel", "images\\itr1.color");
    Image *itr3 = read_image("images\\itr3.pixel",  "images\\itr3.color");
    Image *itr4 = read_image("images\\itr4.pixel",  "images\\itr4.color");


    //show_image(ima, 0, 3);
    show_image_gray(itr0, 163,9);

    for (i=0; i<4; i++)
        show_image_gray(itr1, 174 + i*36,112);
    for (i=0;i<20;i++)notes[i][2] = 0;
//
//    putStringLarge(songmenu.large_font, 479-strlen(songmenu.text[songmenu.cursor])*8, 115, songmenu.text[songmenu.cursor], 0);
//    putStringLarge(songmenu.large_font, 479-strlen(songmenu.text_2[songmenu.cursor])*8, 121, songmenu.text_2[songmenu.cursor], 0);
    putStringLarge(songmenu.large_font, 2, 3, songmenu.text[songmenu.cursor], 0);
    putStringLarge(songmenu.large_font, 2, 9, songmenu.text_2[songmenu.cursor], 0);


    loadRhythm(songmenu.cursor);


    saveScreen();
    drawCmdWindow();

    cur_time = last_time = timer();

    while(!IsEnding  &&  count < songs[songmenu.cursor].lenOfSong){
        last_time = timer();
        interval = 0;


        if (rhythm[count]){
            for (i=0; notes[i][2]!=0; i++);
            notes[i][0] = 36 * (rand()%4) + 182;                             //X座標
            notes[i][1] = 12;                                                //Y座標
            notes[i][2] = 1;                                                 //是否在畫面當中
            notes[i][3] = 0;                                                 //是否被判定過
        }


        for (k=0; k<NUM_KEYS; k++) {
            if(KEY_DOWN(key_val[k]) && !key_down[k]) {
                key_down[k] = 1;
                switch (key_val[k]) {
                    case VK_D:
                        judge(0);
                        keys[0] = 1;
                        break;
                    case VK_F:
                        judge(1);
                        keys[1] = 1;
                        break;
                    case VK_J:
                        judge(2);
                        keys[2] = 1;
                        break;
                    case VK_K:
                        judge(3);
                        keys[3] = 1;
                        break;
                    case VK_ESCAPE:
                        cur_time = timer();
                        int pause = 1;
                        int choice_2 = 0;
                        mciSendString("stop mymusic", NULL, 0, NULL);
                        while(pause){
                            Sleep(50);
                            showStopmenu(&stopmenu);
                            for (k=0; k<NUM_KEYS; k++) {
                                if(KEY_DOWN(key_val[k]) && !key_down[k]) {
                                    key_down[k] = 1;
                                    switch (key_val[k]) {
                                        case VK_RETURN:
                                            if(stopmenu.cursor == 1){
                                                restart = 1;
                                                IsEnding = 1;
                                            }else if(stopmenu.cursor == 2){
                                                IsEnding = 1;
                                            }
                                            pause = 0;
                                            break;
                                        case VK_ESCAPE:
                                            pause = 0;
                                            break;
                                        case VK_UP:
                                            scrollMenu(&stopmenu, -1);
                                            break;
                                        case VK_DOWN:
                                            scrollMenu(&stopmenu, +1);
                                            break;
                                    }
                                }
                                if(KEY_UP(key_val[k]) && key_down[k])key_down[k] = 0;
                            }

                            drawCmdWindow();

                        }Sleep(50);
                        mciSendString("play mymusic", NULL, 0, NULL);
                        interval = timer() - cur_time;
                        break;
                }
            }
            if(KEY_UP(key_val[k]) && key_down[k]){
                key_down[k] = 0;
                switch (key_val[k]) {
                    case VK_D:
                        keys[0] = 0;
                        break;
                    case VK_F:
                        keys[1] = 0;
                        break;
                    case VK_J:
                        keys[2] = 0;
                        break;
                    case VK_K:
                        keys[3] = 0;
                        break;
                }
            }
        }

        restoreScreen();

        char bb[] = "█";
        for(i=0;i<(count*480/songs[songmenu.cursor].lenOfSong);i+=2){
            drawString(bb, i, 0, 7, 7);
        }


        ptNote();

        for(i=0;i<4;i++){
            if(keys[i])
            show_image_gray(itr2, 174 + i*36,112);
            if(effect0[i]){
                switch (effect0[i]){
                case 1:
                    show_image_gray(ef0[0], 174 + i*36,112);
                    effect0[i]++;
                    break;
                case 2:
                    show_image_gray(ef0[1], 170 + i*36,110);
                    effect0[i]++;
                    break;
                case 3:
                    show_image_gray(ef0[1], 170 + i*36,110);
                    effect0[i]++;
                    break;
                case 4:
                    effect0[i] = 0;
                    break;
                }
            }
            if(effect1[i]){
                if(effect1[i]<4){
                    show_image_gray(ef1, 174 + i*36,112);
                    effect1[i] ++;
                }
                else
                    effect1[i] = 0;
            }
        }

        char ccc[100]="combo";
        putNumberLarge      (460, 3,    score, 0);
        putNumberLarge_extra(460, 15, perfect, 0);
        putNumberLarge_extra(460, 25,   great, 0);
        putNumberLarge_extra(460, 35,    miss, 0);
        if(combo){
            if(combo > maxcombo)maxcombo = combo;
            putNumberLarge_extra(250, 3, combo, 0);
            putStringLarge(ff, 260, 3, ccc, 0);
        }

        drawCmdWindow();

        if (count == songs[songmenu.cursor].delay){
            playMusic(songmenu.cursor);
        }
        count ++;

        refresh(count);
    }

    //結算畫面
    clearScreen();
    show_image(ima, 0, 3);

    show_image_no_space(itr3, 315, 15);

    show_image_no_space(itr4, 273, 15);
    show_image_no_space(itr4, 191, 15);
    show_image_no_space(itr4, 109, 15);
    show_image_no_space(itr4, 232, 36);
    show_image_no_space(itr4, 150, 36);

    int nnn;
    Image *yee;
    if(score>900000)        yee = read_image("images\\s.pixel", "images\\s.color");
    else if(score>800000)   yee = read_image("images\\a.pixel", "images\\a.color");
    else if(score>700000)   yee = read_image("images\\b.pixel", "images\\b.color");
    else                    yee = read_image("images\\f.pixel", "images\\f.color");

    show_image_no_space(yee, 295, 22);
    putNumberLarge_extra(234, 27,   perfect, 0);
    putNumberLarge_extra(152, 27,      miss, 0);
    putNumberLarge_extra(275, 48,  maxcombo, 0);
    putNumberLarge_extra(193, 48,     great, 0);

    char c[] = "C";
    char p[] = "P";
    char g[] = "G";
    char m[] = "M";
    putStringLarge(songmenu.large_font,263,57, c,0);
    putStringLarge(songmenu.large_font,222,36, p,0);
    putStringLarge(songmenu.large_font,181,57, g,0);
    putStringLarge(songmenu.large_font,140,36, m,0);

    putNumberLarge(412, 50,  score, 0);

    if(!IsEnding)drawCmdWindow();
    while(!IsEnding){

        for (k=0; k<NUM_KEYS; k++) {
            if(KEY_DOWN(key_val[k]) && !key_down[k]) {
                key_down[k] = 1;
                switch (key_val[k]) {
                    case VK_ESCAPE:
                        IsEnding = 1;
                        break;
                }
            }
            if(KEY_UP(key_val[k]) && key_down[k])key_down[k] = 0;
        }
    }


}

void loadRhythm(int num){
    FILE *fin;
    int i;
    char f[30];
    sprintf(f, "rhythm\\rhythm%d.txt", num);
    fin = fopen(f, "r");
    if (fin == NULL) {
        perror(f);
        exit(EXIT_FAILURE);
    }
    i = DELAY;
    while(fscanf(fin, "%1d", &rhythm[i])!=EOF)i+=4;
    fclose(fin);
}

void ptNote(){
    int i;
    Image *nt = read_image("images\\nt0.pixel", "images\\nt0.color");
    Image *nt2 = read_image("images\\nt1.pixel", "images\\nt0.color");
    for (i=0; i<20; i++){
        if (notes[i][2] == 1){

            if (notes[i][1]  < 124 && notes[i][1] > 2 && notes[i][3] == 0){
                /*if (notes[i][1] == 120){
                    PlaySound("sounds\\sound4.wav", NULL, SND_FILENAME | SND_ASYNC);
                }*/
                if (notes[i][1]  < 118 && notes[i][1] > 112)
                show_image_gray(nt2, notes[i][0], notes[i][1]);
                else
                show_image_gray(nt, notes[i][0], notes[i][1]);
            }else if(notes[i][1] > 126){
                if (notes[i][3] == 0){
                    miss++;
                    combo = 0;
                }
                notes[i][2] = 0;
                notes[i][1] = 0;
            }
            notes[i][1]+=3;
        }

    }

}

int refresh(int count){
    if (count%96 < songs[songmenu.cursor].n){
        do{
            cur_time = timer();
        }while(cur_time - last_time - interval < songs[songmenu.cursor].ms);
    }else
        do{
            cur_time = timer();
        }while(cur_time - last_time - interval < songs[songmenu.cursor].ms + 1);

}

void judge(int k){
    int x = 36*k + 182;
    int i;
    int n = songs[songmenu.cursor].numOfNotes;
    for (i=0; i<20; i++){
        if (notes[i][0] == x && notes[i][1] >=41 && notes[i][3] == 0){
            if (notes[i][1] <= 119 && notes[i][1] >= 113){
                score += 900000/n + 1 + 100000*combo/(n*(n + 1)/2);
                if (score > 1000000)
                    score = 1000000;
                perfect++;
                combo++;
                effect0[k] = 1;
                notes[i][3] = 1;
                return;
            }
        }
    }
    for (i=0; i<20; i++){
        if (notes[i][0] == x && notes[i][1] >=39 && notes[i][3] == 0){
            if(notes[i][1] <= 128 && notes[i][1] >= 104){
                score += 630000/n + 1 + 100000*combo/(n*(n + 1)/2);

                great++;
                combo++;
                effect1[k] = 1;
                notes[i][3] = 1;
                return;
            }
        }
    }
    for (i=0; i<20; i++){
        if (notes[i][0] == x && notes[i][1] >=98 && notes[i][3] == 0){

            miss++;
            combo = 0;
            notes[i][3] = 1;
            return;
        }
    }
}

void playMusic(int num){
    char musicFileName[50];
    mciSendString("close mymusic", NULL, 0, NULL);
    sprintf(musicFileName, "open musics\\music%d.mp3 alias mymusic", num);
    mciSendString(musicFileName, NULL, 0, NULL);
    mciSendString("play mymusic", NULL, 0, NULL);
}
