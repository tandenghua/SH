#ifndef __FBDRAW_H_
#define __FBDRAW_H
#include "main.h"

/*fbdraw.c*/
extern void	fbdot(COLOR_TYPE *p_vmem,int x,int y,COLOR_TYPE i);
extern void	fbliney(COLOR_TYPE *p_vmem,int x,int y1,int y2,COLOR_TYPE i);
extern void	fblinex(COLOR_TYPE *p_vmem,int y,int x1,int x2,COLOR_TYPE i);
extern void	fbline(COLOR_TYPE *p_vmem,int x1,int y1,int x2,int y2,COLOR_TYPE i);
extern void	fbrec(COLOR_TYPE *p_vmem,int x,int y,int width,int length,COLOR_TYPE i);
extern void draw_ascii(COLOR_TYPE *p_vmem,char c,int x,int y,int zoom_x,int zoom_y,COLOR_TYPE col);
extern void draw_str(COLOR_TYPE *p_vmem,char *str,int x,int y,int zoom_x,int zoom_y,COLOR_TYPE col);

extern void draw_dot_24(COLOR_TYPE *p_vmem,int x,int y,int i);
extern void draw_liney_24(COLOR_TYPE *p_vmem,int x,int y1,int y2,int i);
extern void draw_linex_24(COLOR_TYPE *p_vmem,int y,int x1,int x2,int i);
extern void draw_line_24(COLOR_TYPE *p_vmem,int x1,int y1,int x2,int y2,int i);
extern void draw_str_24(COLOR_TYPE *p_vmem,char *str,int x,int y,int zoom_x,int zoom_y,int i,int n);
extern void draw_ascii_24(COLOR_TYPE *p_vmem,unsigned char c,int x,int y,int zoom_x,int zoom_y,int i,int n);
extern void fbrec_24(COLOR_TYPE *p_vmem,int x,int y,int width,int height,int i);

extern void show_hbmpmem(COLOR_TYPE *p_vmem,COLOR_TYPE *p_bmem,int x,int y);
extern void show_vbmpmem(COLOR_TYPE *p_vmem,COLOR_TYPE *p_bmem,int x,int y);
extern void show_lbmpmem(COLOR_TYPE *p_vmem,COLOR_TYPE *p_bmem,int x,int y);
extern void show_bmpmem(COLOR_TYPE *p_vmem,COLOR_TYPE *p_bmp,int x,int y,int length,int height);

extern void draw_data1(COLOR_TYPE *p_vmem,COLOR_TYPE i,unsigned short *data);
extern void draw_data2(COLOR_TYPE *p_vmem,COLOR_TYPE i,float *data);
extern void fetch_data();

#endif
