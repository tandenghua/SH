#ifndef __DRAW_RIM_H
#define __DRAW_RIM_H

#include "main.h"

#define STORE_DATA(a, b) {i2c_add[1] = a;\
									   write_i2c(pt->fd_config, i2c_add, pt->p_config + a, b);}

#define HF_NUM	50.0
#define HF_Y	300.0

extern COLOR_TYPE offset1[];/*间距(offset)未选中时的显示*/
extern COLOR_TYPE offset2[];/*间距(offset)选中时对显示*/
extern COLOR_TYPE offset3[];
extern COLOR_TYPE offset4[];
extern COLOR_TYPE offset5[];
extern COLOR_TYPE offset6[];

extern const unsigned int scheme[][12];/*颜色数据组*/

extern void (*p[4][5][4])();//按键处理函数指针数组

extern int senddsp_data();

extern void draw_rim(COLOR_TYPE *p_vmem,int col);
extern void draw_horizontal_text1(COLOR_TYPE *p_vmem);
extern void draw_horizontal_text2(COLOR_TYPE *p_vmem);
extern void draw_vertical_text1(COLOR_TYPE *p_vmem);
extern void draw_ver_text1(COLOR_TYPE *p_vmem, int i);

extern int change_db(COLOR_TYPE *p_vmem,int a);
extern int change_db_step(COLOR_TYPE *p_vmem,int a);

extern int draw_agat(COLOR_TYPE *p_vmem,int a);
extern int draw_bgat(COLOR_TYPE *p_vmem,int a);

extern int draw_scale(COLOR_TYPE *p_vmem,int a);

extern int draw_four_dot_frez(COLOR_TYPE *p_vmem,int x,int y,int x_off,int y_off,int col);
extern int draw_frez(COLOR_TYPE *p_vmem,int x,int y,int col);

extern int draw_battery(COLOR_TYPE *p_vmem,int a);
extern int battery_state;

extern int show_info(COLOR_TYPE *p_vmem,int a);

extern int do_copy(COLOR_TYPE *p_vmem,int a);
extern void calculate_measure_data();
extern int draw_dac_line(COLOR_TYPE *p_vmem,unsigned int *dac_data,int col);

extern void cal_dac_data1();
extern int cal_dac_data(unsigned int *dac_data,unsigned int *dac_data_x,float *dac_data_y,int dac_dot_quantity);
extern int cal_dac_data_off(unsigned int *dac_data,unsigned int *dac_dot_offdata,unsigned short off_db,int a);

extern int read_i2c(int fd,unsigned char *add,unsigned char *data,int count);
extern int write_i2c(int fd,unsigned char *add,unsigned char *data,int count);

extern int cal_range_add(int start,int *a,int *b,int g);/*增加各个数值 g为倍数*/
extern int cal_range_sub(int start,int *a,int *b,int g);/*减少各个数值 g为倍数*/
/*各个数值增减 a  操作数,b增长间隔,c步长数值,d,e为范围,f=1为增加,2为减少g=0为粗调,1为精调*/
extern int value_change(int a,int *b,int *c,int d,int e,int f,int g);

extern int test_val();/*测试能否改值 1可以0不可以*/
extern int write_db_val(int db,int old_db);/*修改db值*/
extern int test_float_length(float i);/*返回浮点数整数部分的位数*/
extern int display_value(COLOR_TYPE *p_vmem,int number,int x,int y,int zoom_x,int zoom_y,int col);

extern void recall_set_config();
extern void display_info(COLOR_TYPE *p_vmem);

extern void draw_hanfeng();
#endif
