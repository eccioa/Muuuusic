#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#include "console_draw3.h"
#include "kb_input2.h"
#include "font.h"

static
char getpixel(Font *ip, int x, int y);

void show_font(Font *ip, char letter, int offx, int offy, int color);
void show_font_gray(Font *ip, char letter, int offx, int offy, int color);


void putStringLarge(Font *fp, int x, int y, char *p, int color)
{
    int i;
    for(i=0; (unsigned int) i<strlen(p); i++) {
        show_font(fp,  p[i], x+i*fp->width, y, color);
    }
}
void putStringLarge_gray(Font *fp, int x, int y, char *p, int color)
{
    int i;
    for(i=0; (unsigned int) i<strlen(p); i++) {
        show_font_gray(fp,  p[i], x+i*fp->width, y, color);
    }
}
void putNumberLarge(int x, int y, int num, int color){
    char p[7];
    int k;
    for(k=0;k<7;k++)p[k] = '0';
    k = 0;
    Font *fp = read_font("font.txt");
    while(num!=0){
        p[k] = num % 10 + '0';
        num /= 10;
        k ++;
    }
    for(k=6; k >= 0; k --){
        show_font(fp, p[k], x-k*fp->width, y, color);
    }

}
void putNumberLarge_extra(int x, int y, int num, int color){
    int num_2 = num / 10;
    int numOfDigits = 1;
    while(num_2!=0){
        num_2 /= 10;
        numOfDigits++;
    }
    char p[numOfDigits];
    int k;
    for(k=0;k<numOfDigits;k++)p[k] = '0';
    k = 0;
    Font *fp = read_font("font.txt");
    while(num!=0){
        p[k] = num % 10 + '0';
        num /= 10;
        k ++;
    }
    for(k=numOfDigits-1; k >= 0; k --){
        show_font(fp, p[k], x-k*fp->width, y, color);
    }

}


Font * read_font(char *pixel_file)
{
    FILE *fid;
    int height, width, num, i, j, k;
    Font *p;

    fid = fopen(pixel_file, "r");

    if (fid==NULL) {
        return NULL;
    } else {
        fscanf(fid, "%d%d%d", &height, &width, &num);
        p = (Font*) malloc(sizeof(Font));
        p->height = height;
        p->width = width;
        p->pixel = (char*) malloc(sizeof(char)*height*width*num);
        fscanf(fid, "%*c");
        for (k=0; k<num; k++) {
            for (i=0; i<height; i++) {
                for (j=0; j<width; j++) {
                    fscanf(fid, "%c", & (p->pixel[k*width*height+i*width+j]));
                }
                fscanf(fid, "%*c");
            }
        }
        fclose(fid);

        return p;
    }
}

char getpixel(Font *ip, int x, int y)
{
    return ip->pixel[y*ip->width + x];
}



void show_font(Font *ip, char letter, int offx, int offy, int color)
{
    int x, y;
    char c;
    int shift;
    if (letter==' ') return;
    if (isdigit(letter)) {
        shift = ip->height*(26 + letter-'0');
    } else {
        shift = ip->height*(toupper(letter)-'A');
    }
    for (y=0; y<ip->height; y++) {
            for (x=0; x<ip->width; x++) {

            c = getpixel(ip, x, y+shift);
            if (c!=' ') drawChar(c,x+offx,y+offy,color,color);
            }
    }
}
void show_font_gray(Font *ip, char letter, int offx, int offy, int color)
{
    int x, y;
    char c;
    int shift;
    if (isdigit(letter)) {
        shift = ip->height*(26 + letter-'0');
    } else if (isalpha(letter)){
        shift = ip->height*(toupper(letter)-'A');
    } else {
        shift = 36*5;
    }
    for (y=0; y<ip->height; y++) {
            for (x=0; x<ip->width; x++) {

            c = getpixel(ip, x, y+shift);
            if (c!=' ') drawChar(c,x+offx,y+offy,color,color);
            else        drawChar(c,x+offx,y+offy,    7,color);
            }
    }
}

void destroy_font(Font *ip)
{
    free(ip->pixel);
    free(ip);
}
