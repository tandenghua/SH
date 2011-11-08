#ifndef __DEVICE_H_
#define __DEVICE_H_
#include "main.h"

extern unsigned char i2c_add[2];	/*写i2c 连接的EEROM 用到的地址*/
extern short temp_db;
extern unsigned int refresh_count;
extern float cal_time[];/*保存校准时间*/
extern float cost_time;
extern unsigned int module_symbol;
extern float fftx1_tmp,fftx2_tmp,ffty_tmp;
extern unsigned int agatopen,bgatopen,probe_test;

extern COLOR_TYPE *bmp;				/*指向当前主菜单数组对指针*/
extern COLOR_TYPE bmp1[];			/*图标数据的数组中文*/
extern COLOR_TYPE bmp2[];			/*图标数据的数组英文*/
extern COLOR_TYPE bmp_content[];	/*保存副菜单内容*/
extern COLOR_TYPE bmp_else[];		/*保存副菜单内容*/
extern COLOR_TYPE bmp_warn[];		/*保存副菜单内容*/
extern COLOR_TYPE bmp_nav[];		/*其他栏图标*/

extern P_DPL_CONFIG	pt;

extern unsigned short envelope_data[640];/*包络数据组*/
extern unsigned short gat_max[480];
extern float probe_test2[400];
extern unsigned short org_data[512], current_data1[640];

extern int dac_x[20];
extern float dac_y[20];


extern void Sleep(int iSec,int	iUSec);
extern void change_col();			/*转换颜色*/
extern void bmpfile2mem();			/*把图标信息从文件中读到数组中 open后没检查是否成功*/

extern int mem_init();
extern int i2c_init();
extern int gpio_init();
extern int spi_init();
extern int fb_init();
extern int store_init();
extern int config_init();

extern int initialize_serial();		/*初始化串口*/
extern int loop_tt();

#endif
