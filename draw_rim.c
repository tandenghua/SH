#include "draw_rim.h"
#include "device.h"
#include "fbdraw.h"
#include "color256.h"

#define H(a) (a)

unsigned int dac_dot_offdata1[400],dac_dot_offdata2[400],
			 dac_dot_offdata3[400], dac_dot_data[400],avg_dot_data[400],avg_dot_offdata[400];

/*日期 时间*/ 
int date_pos = 0;
unsigned int date[3];
int time_pos = 0;
unsigned int time_c[3];

/*RANGE MTLEVEL D_DELAY P_DELAY 范围微米μm 声速 扫描延时μm 探头延时μs/1000 范围以及步长*/
int range_range[] = {500,10000,100000,1000000,9999000};
int range_step0[] = {500,1000,2500,5000,10000,20000,30000,
	40000,50000,60000,70000,80000,90000,100000,110000,120000,
	130000,140000,150000,160000,170000,180000,190000,200000,
	250000,300000,400000,500000,600000,700000,800000,900000,
	1000000,1100000,1200000,1300000,1400000,1500000,2000000,
	2500000,3000000,3500000,4000000,4500000,5000000,5500000,
	6000000,6500000,7000000,7500000,8000000,8500000,9000000,9500000,9999000};
int range_step1[] = {1,10,100,1000};
int mtlvel_range[] = {1000,15000};
int mtlvel_step0[] = {1000,1450,1600,2000,2730,3000,3130,3250,
	4000,5000,5920,6000,6320,7000,8000,9000,10000,11000,12000,13000,14000,15000};
int mtlvel_step1[] = {1};
int d_delay_range[] = {-10000,10000,100000,1000000,2000000};
int d_delay_step0[] = {-10000,0,5000,10000,20000,30000,40000,
	50000,60000,70000,80000,90000,100000,200000,300000,400000,
	500000,600000,700000,800000,900000,1000000,1100000,1200000,
	1300000,1400000,1500000,1600000,1700000,1800000,1900000,2000000};
int d_delay_step1[] = {1,10,100,1000};
int p_delay_range[] = {0,10000,100000,199900};
int p_delay_step0[] = {0,100,200,300,400,500,600,700,800,900,1000,
	1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,3000,4000,
	5000,6000,7000,8000,9000,10000,11000,12000,13000,14000,15000,
	16000,17000,18000,19000,20000,30000,40000,50000,60000,70000,
	80000,90000,100000,110000,120000,130000,140000,150000,160000,170000,180000,190000,199900};
int p_delay_step1[] = {1,10,100};

/*AGAT LOGIC START WIDTH THRSH 闸门A 起点 宽度μm 范围以及步长*/
int gat_start_range[] = {0,10000,100000,1000000,9999000};
int gat_start_step0[] = {0,500,1000,1500,2000,2500,3000,3500,4000,
	4500,5000,6000,7000,8000,9000,10000,12000,14000,16000,18000,
	20000,25000,30000,35000,40000,45000,50000,60000,70000,80000,
	90000,100000,120000,140000,160000,180000,200000,250000,300000,
	350000,400000,450000,500000,600000,700000,800000,900000,1000000,
	2000000,2500000,3000000,3500000,4000000,5000000,6000000,7000000,8000000,9000000,9999000};
int gat_start_step1[] = {1,10,100,1000};

/*孔径 孔深度*/
int avg_d_range[] = {500, 50000};
int avg_d_step[] = {500};
int cal_h_range[] = {5, 70000};
int cal_h_step[] = {100};


/*参照物 1,2 闸门A 起点μm  校准*/
int s_ref_range[] = {0,10000,100000,1000000,5000000};
int s_ref_range1[] = {0,10000,100000,1000000,2500000};
int s_ref_step0[] = {0,500,1000,2500,5000,10000,12500,20000,25000,
	30000,40000,50000,60000,70000,75000,80000,90000,100000,110000,
	120000,130000,140000,150000,160000,170000,180000,190000,200000,
	250000,300000,350000,400000,500000,600000,700000,800000,900000,
	1000000,1100000,1200000,1300000,1400000,1500000,2000000,2500000,
	3000000,3500000,4000000,4500000,5000000};
int s_ref_step00[] = {0,500,1000,2500,5000,10000,12500,20000,25000,
	30000,40000,50000,60000,70000,75000,80000,90000,100000,110000,
	120000,130000,140000,150000,160000,170000,180000,190000,200000,
	250000,300000,350000,400000,500000,600000,700000,800000,900000,
	1000000,1100000,1200000,1300000,1400000,1500000,2000000,2500000};
int s_ref_step1[] = {1,10,100,1000};

/*角度 前沿μm 厚度μm 直径μm*/
int angle_range[] = {0,850};
int angle_step0[] = {0,350,450,600,700,800,850};
int angle_step1[] = {1};
int x_value_range[] = {0,100000};
int x_value_step[] = {100};
int thickne_range[] = {1000,1000000};
int thickne_step0[] = {1000,2500,5000,10000,20000,30000,40000,50000,
	60000,70000,80000,90000,100000,110000,120000,130000,140000,150000,
	160000,170000,180000,190000,200000,250000,300000,400000,500000,
	600000,700000,800000,900000,1000000};

int thickne_step1[] = {100};
int diamet_range[] = {10000,100000,1000000,2001000};
int  diamet_step0[] = {10000,100000,200000,300000,400000,500000,
	600000,700000,800000,900000,1000000,1100000,1200000,1300000,
	1400000,1500000,1600000,1700000,1800000,1900000,2000000,2001000};
int diamet_step1[] = {10,100,1000};

/*探头位置 焊缝位置*/
int probe_pos_range[] = {1,500};
int probe_pos_step[] = {1};

int hanfeng_w_range[] = {1,200};
int hanfeng_w_step[] = {1};

/*间距(offset)*/
int offset_range[] = {0,140};
int offset_range1[] = {-140,140};
int offset_step[] = {5};
//COLOR_TYPE offset1[V_SIZE*3*2];
COLOR_TYPE offset2[V_SIZE*6*2];
COLOR_TYPE offset3[V_SIZE*4*2];
COLOR_TYPE offset4[V_SIZE*4*2];
COLOR_TYPE offset5[V_SIZE*4*2];
//COLOR_TYPE offset6[V_SIZE*4*2];

/*屏幕显示 内容*/
char *display[] = {"off","Sa","Sb","Sb-a","Ha %","Hb %","Ha dB",
	"Hb dB","R-start","R-end","La","Lb","Da","Db","Pa","Pb",
	"Ra","Rb","Ha %crv","Hb %crv","DAC DB"};
char *display_top[] = {"off","Sa","Sb","ba","Ha","Hb","ha",
	"hb","Rs","Re","La","Lb","Da","Db","Pa","Pb","Ra","Rb","Ca","Cb","db"};

/*色彩 1菜单未选种背景 2菜单为选种前景 3菜单选中背景 4菜单选中前景 5数值菜单背景 
 * 6数值选中背景 7数值菜单前景  8数值选中前景 9其他 10 dacline 11 波形背景 12波形前景 */
const unsigned int scheme[][12] = {
	{COL0,COL1,COL2,COL3,COL4,COL5,COL17,COL16,COL40,COL23,COL16,COL18},
	{COL0,COL1,COL2,COL3,COL4,COL5,COL17,COL16,COL40,COL17,COL18,COL16},
	{COL0,COL1,COL2,COL3,COL4,COL5,COL17,COL16,COL40,COL17,COL22,COL16},
	{COL0,COL1,COL2,COL3,COL4,COL5,COL17,COL16,COL41,COL23,COL16,COL18},
	{COL0,COL1,COL2,COL3,COL4,COL5,COL17,COL16,COL41,COL17,COL18,COL16},
	{COL0,COL1,COL2,COL3,COL4,COL5,COL17,COL16,COL41,COL17,COL22,COL16}};

static const unsigned short battery_col[] = {COL24,COL24,COL25,COL26,COL27,COL28,COL30};

/*增益步长*/
int db_step0[] = {0,1,5,10,20,60};

/*t0=t80 参数a 0 只是画出当前值,1加,2减,3粗调微调*/
static void t0(COLOR_TYPE *p_vmem,int a);static void t1(COLOR_TYPE *p_vmem,int a);
static void t2(COLOR_TYPE *p_vmem,int a);static void t3(COLOR_TYPE *p_vmem,int a);
static void t4(COLOR_TYPE *p_vmem,int a);static void t5(COLOR_TYPE *p_vmem,int a);
static void t6(COLOR_TYPE *p_vmem,int a);static void t7(COLOR_TYPE *p_vmem,int a);
static void t8(COLOR_TYPE *p_vmem,int a);static void t9(COLOR_TYPE *p_vmem,int a);
static void t10(COLOR_TYPE *p_vmem,int a);static void t11(COLOR_TYPE *p_vmem,int a);
static void t12(COLOR_TYPE *p_vmem,int a);static void t13(COLOR_TYPE *p_vmem,int a);
static void t14(COLOR_TYPE *p_vmem,int a);static void t15(COLOR_TYPE *p_vmem,int a);

static void t16(COLOR_TYPE *p_vmem,int a);static void t17(COLOR_TYPE *p_vmem,int a);
static void t18(COLOR_TYPE *p_vmem,int a);static void t19(COLOR_TYPE *p_vmem,int a);
static void t20(COLOR_TYPE *p_vmem,int a);static void t21(COLOR_TYPE *p_vmem,int a);
static void t22(COLOR_TYPE *p_vmem,int a);static void t23(COLOR_TYPE *p_vmem,int a);
static void t24(COLOR_TYPE *p_vmem,int a);static void t25(COLOR_TYPE *p_vmem,int a);
static void t26(COLOR_TYPE *p_vmem,int a);static void t27(COLOR_TYPE *p_vmem,int a);
static void t28(COLOR_TYPE *p_vmem,int a);static void t29(COLOR_TYPE *p_vmem,int a);
static void t30(COLOR_TYPE *p_vmem,int a);static void t31(COLOR_TYPE *p_vmem,int a);

static void t32(COLOR_TYPE *p_vmem,int a);static void t33(COLOR_TYPE *p_vmem,int a);
static void t34(COLOR_TYPE *p_vmem,int a);static void t35(COLOR_TYPE *p_vmem,int a);
static void t36(COLOR_TYPE *p_vmem,int a);static void t37(COLOR_TYPE *p_vmem,int a);
static void t38(COLOR_TYPE *p_vmem,int a);static void t39(COLOR_TYPE *p_vmem,int a);
static void t40(COLOR_TYPE *p_vmem,int a);static void t41(COLOR_TYPE *p_vmem,int a);
static void t42(COLOR_TYPE *p_vmem,int a);static void t43(COLOR_TYPE *p_vmem,int a);
static void t44(COLOR_TYPE *p_vmem,int a);static void t45(COLOR_TYPE *p_vmem,int a);
static void t46(COLOR_TYPE *p_vmem,int a);static void t47(COLOR_TYPE *p_vmem,int a);

static void t48(COLOR_TYPE *p_vmem,int a);static void t49(COLOR_TYPE *p_vmem,int a);
static void t50(COLOR_TYPE *p_vmem,int a);static void t51(COLOR_TYPE *p_vmem,int a);
static void t52(COLOR_TYPE *p_vmem,int a);static void t53(COLOR_TYPE *p_vmem,int a);
static void t54(COLOR_TYPE *p_vmem,int a);static void t55(COLOR_TYPE *p_vmem,int a);
static void t56(COLOR_TYPE *p_vmem,int a);static void t57(COLOR_TYPE *p_vmem,int a);
static void t58(COLOR_TYPE *p_vmem,int a);static void t59(COLOR_TYPE *p_vmem,int a);
static void t60(COLOR_TYPE *p_vmem,int a);static void t61(COLOR_TYPE *p_vmem,int a);
static void t62(COLOR_TYPE *p_vmem,int a);static void t63(COLOR_TYPE *p_vmem,int a);

static void t64(COLOR_TYPE *p_vmem,int a);static void t65(COLOR_TYPE *p_vmem,int a);
static void t66(COLOR_TYPE *p_vmem,int a);static void t67(COLOR_TYPE *p_vmem,int a);
static void t68(COLOR_TYPE *p_vmem,int a);static void t69(COLOR_TYPE *p_vmem,int a);
static void t70(COLOR_TYPE *p_vmem,int a);static void t71(COLOR_TYPE *p_vmem,int a);
static void t72(COLOR_TYPE *p_vmem,int a);static void t73(COLOR_TYPE *p_vmem,int a);
static void t74(COLOR_TYPE *p_vmem,int a);static void t75(COLOR_TYPE *p_vmem,int a);
static void t76(COLOR_TYPE *p_vmem,int a);static void t77(COLOR_TYPE *p_vmem,int a);
static void t78(COLOR_TYPE *p_vmem,int a);static void t79(COLOR_TYPE *p_vmem,int a);

void draw_rim(COLOR_TYPE *p_vmem,int col);
void draw_horizontal_text1(COLOR_TYPE *p_vmem);
void draw_horizontal_text2(COLOR_TYPE *p_vmem);
void draw_vertical_text1(COLOR_TYPE *p_vmem);
void draw_ver_text1(COLOR_TYPE *p_vmem, int i);

static int read_config();/*读取配置文件*/
static int read_config1();/*读取数据文件*/
static int store_config();/*储存配置文件*/
static int store_config1();/*储存数据文件*/
static int delete_config();/*删除配置文件*/
static int delete_config1();/*删除数据文件*/
static int test_file();/*判断配置是否存在*/
static int test_file1();/*判断数据是否存在*/

int change_db(COLOR_TYPE *p_vmem,int a);
int change_db_step(COLOR_TYPE *p_vmem,int a);
int write_db_val(int db,int old_db);/*修改db值*/

int draw_agat(COLOR_TYPE *p_vmem,int a);
int draw_bgat(COLOR_TYPE *p_vmem,int a);

int draw_scale(COLOR_TYPE *p_vmem,int a);

int draw_four_dot_frez(COLOR_TYPE *p_vmem,int x,int y,int x_off,int y_off,int col);
int draw_frez(COLOR_TYPE *p_vmem,int x,int y,int col);

int draw_battery(COLOR_TYPE *p_vmem,int a);
int battery_state = 0x1;

int show_info(COLOR_TYPE *p_vmem,int a);

int do_copy(COLOR_TYPE *p_vmem,int a);
void calculate_measure_data();

int draw_dac_line(COLOR_TYPE *p_vmem,unsigned int *dac_data,int col);
void cal_dac_data1();
static void cal_avg_data();

int read_i2c(int fd,unsigned char *add,unsigned char *data,int count);
int write_i2c(int fd,unsigned char *add,unsigned char *data,int count);
int senddsp_data();

int cal_range_add(int start,int *a,int *b,int g);/*增加各个数值*/
int cal_range_sub(int start,int *a,int *b,int g);/*减少各个数值*/
/*各个数值增减 a  操作数,b增长间隔,c步长数值,d,e为范围,f=1为增加,2为减少g=0为粗调,1为精调*/
int value_change(int a,int *b,int *c,int d,int e,int f,int g);

int test_val();/*测试能否改值 1可以 其他不可以*/
int test_magval();/*测试能否改值 1可以 其他不可以*/
int test_float_length(float i);/*返回浮点数整数部分的位数*/
int display_value(COLOR_TYPE *p_vmem,int number,int x,int y,int zoom_x,int zoom_y,int col);

void set_time();/*set 时间日期 */

/*pos 表示是否 在当前图标位置,x y为显示对图标位置reg 代表 几级微调*/
void cal_str_display(COLOR_TYPE *p_vmem,char *temp,int class,int value,int reg,int pos,int x,int y);

void (*p[4][5][4])() ={
	{{t0,t1,t2,t3},{t4,t5,t6,t7},{t8,t9,t10,t11},{t12,t13,t14,t15},{t16,t17,t18,t19}},
	{{t20,t21,t22,t23},{t24,t25,t26,t27},{t28,t29,t30,t31},{t32,t33,t34,t35},{t16,t17,t18,t19}},
	{{t40,t41,t42,t43},{t44,t45,t46,t47},{t48,t49,t50,t51},{t52,t53,t54,t55},{t16,t17,t18,t19}},
	{{t60,t61,t62,t63},{t64,t65,t66,t67},{t68,t69,t70,t71},{t72,t73,t74,t75},{t16,t17,t18,t19}}};
/*按键处理函数指针数组*/

//恢复出厂配置的设置
void recall_set_config();

char temp_string[12];
char *s_str = temp_string;

unsigned char key_light_high[] = {0xe2,0xe2};
unsigned char key_light_mid[] = {0xe1,0xe1};
unsigned char key_light_low[] = {0xe0,0xe0};

/*读取配置 set_pos 序号*/
int read_config()
{
	lseek(pt->fd_store, 528 * VAL(set), SEEK_SET);
	read(pt->fd_store, pt->p_config, sizeof(CONFIG));
	return 0;
}

int read_config1()
{
	lseek(pt->fd_store, 2112 + 528 * 32 + 2112 * VAL(job), SEEK_SET);
	read(pt->fd_store, pt->current_data, 960);
	read(pt->fd_store, pt->measure_cur_data, sizeof(MEASURE));
	read(pt->fd_store, pt->p_config, sizeof(CONFIG));
	return 0;
}

/*测试配置是否存在 存在返回1 不存在返回 0 */
int test_file()
{
	char temp[1];
	lseek(pt->fd_store, VAL(set) - 1, SEEK_SET);
	read(pt->fd_store, temp, 1);
	return (!temp[0]);
}

int test_file1()
{
	char temp[1];
	lseek(pt->fd_store, 32 + VAL(job) - 1, SEEK_SET);
	read(pt->fd_store, temp, 1);
	return (!temp[0]);
}
/*储存配置,set_pos为序号*/
int store_config()
{	
	lseek(pt->fd_store, 528 * VAL(set), SEEK_SET);
	write(pt->fd_store, pt->p_config, sizeof(CONFIG));
	lseek(pt->fd_store, VAL(set) - 1, SEEK_SET);
	write(pt->fd_store, "\000", 1);
	return 0;
}

int store_config1()
{	
	lseek(pt->fd_store, 2112 + 528 * 32 + 2112 * VAL(job), SEEK_SET);
	write(pt->fd_store, pt->current_data,960);
	write(pt->fd_store, pt->measure_cur_data, sizeof(MEASURE));
	write(pt->fd_store, pt->p_config, sizeof(CONFIG));
	lseek(pt->fd_store, 32 + VAL(job) - 1, SEEK_SET);
	write(pt->fd_store, "\000", 1);
	return 0;
}
/*删除已有配置*/
int delete_config()
{
	lseek(pt->fd_store, VAL(set) - 1, SEEK_SET);
	write(pt->fd_store, "\377", 1);
	return 0;
}
int delete_config1()
{
	lseek(pt->fd_store, 32 + VAL(job) - 1,SEEK_SET);
	write(pt->fd_store, "\377", 1);
	return 0;
}


/*画 栅格 (112, 50) 528x350 */
void draw_rim(COLOR_TYPE *p_vmem, int col)
{
	int i,j;
	for (i = 0; i < 480; i++)
		memset(p_vmem + 640 * i, 0x0, 640*2);

	/* 栅格 */
	for (i = 1; i < 5; i++)
		for (j = 1; j < 66; j++)
		{
			fbdot(p_vmem, 112 + j * 8, 50 + i * 70, col);
		}

	for (i = 1; i < 50; i++)
		for (j = 1; j < 11; j++)
		{
			fbdot(p_vmem, 112 + j * 48, 50 + i * 7, col);
		}


	/*  */
	//	fbrec(p_vmem, X0, Y1 + 1, 640, 29, OTHER_COL);
	//	if(VAL(frez)==1) draw_frez(p_vmem,512,407,17);
	//	change_db(p_vmem,0);
	//	change_db_step(p_vmem,0);
}

/*画主菜单*/
void draw_horizontal_text1(COLOR_TYPE *p_vmem)
{	
	if(1 == VAL(zoomed)) {
	} else { 
		switch(VAL(hor_pos)[VAL(pos)]){
			case 	0:
				show_hbmpmem(p_vmem,bmp+(BASE+VAL(pos)*10)*V_SIZE,X0,Y2-2);		
				show_hbmpmem(p_vmem,bmp+(BASE+2+1+VAL(pos)*10)*V_SIZE,X3,Y2-2);		
				show_hbmpmem(p_vmem,bmp+(BASE+4+1+VAL(pos)*10)*V_SIZE,X4,Y2-2);	
				show_hbmpmem(p_vmem,bmp+(BASE+6+1+VAL(pos)*10)*V_SIZE,X5,Y2-2);
				show_hbmpmem(p_vmem,bmp+(BASE+8+1+VAL(pos)*10)*V_SIZE,X6,Y2-2);	break;
			case 	1:
				show_hbmpmem(p_vmem,bmp+(BASE+1+VAL(pos)*10)*V_SIZE,X0,Y2-2);		
				show_hbmpmem(p_vmem,bmp+(BASE+2+VAL(pos)*10)*V_SIZE,X3,Y2-2);		
				show_hbmpmem(p_vmem,bmp+(BASE+4+1+VAL(pos)*10)*V_SIZE,X4,Y2-2);	
				show_hbmpmem(p_vmem,bmp+(BASE+6+1+VAL(pos)*10)*V_SIZE,X5,Y2-2);
				show_hbmpmem(p_vmem,bmp+(BASE+8+1+VAL(pos)*10)*V_SIZE,X6,Y2-2);	break;
			case 	2:
				show_hbmpmem(p_vmem,bmp+(BASE+1+VAL(pos)*10)*V_SIZE,X0,Y2-2);		
				show_hbmpmem(p_vmem,bmp+(BASE+2+1+VAL(pos)*10)*V_SIZE,X3,Y2-2);		
				show_hbmpmem(p_vmem,bmp+(BASE+4+VAL(pos)*10)*V_SIZE,X4,Y2-2);	
				show_hbmpmem(p_vmem,bmp+(BASE+6+1+VAL(pos)*10)*V_SIZE,X5,Y2-2);
				show_hbmpmem(p_vmem,bmp+(BASE+8+1+VAL(pos)*10)*V_SIZE,X6,Y2-2);	break;
			case 	3:
				show_hbmpmem(p_vmem,bmp+(BASE+1+VAL(pos)*10)*V_SIZE,X0,Y2-2);		
				show_hbmpmem(p_vmem,bmp+(BASE+2+1+VAL(pos)*10)*V_SIZE,X3,Y2-2);		
				show_hbmpmem(p_vmem,bmp+(BASE+4+1+VAL(pos)*10)*V_SIZE,X4,Y2-2);	
				show_hbmpmem(p_vmem,bmp+(BASE+6+VAL(pos)*10)*V_SIZE,X5,Y2-2);
				show_hbmpmem(p_vmem,bmp+(BASE+8+1+VAL(pos)*10)*V_SIZE,X6,Y2-2);	break;
			case 	4:
				show_hbmpmem(p_vmem,bmp+(BASE+1+VAL(pos)*10)*V_SIZE,X0,Y2-2);		
				show_hbmpmem(p_vmem,bmp+(BASE+2+1+VAL(pos)*10)*V_SIZE,X3,Y2-2);		
				show_hbmpmem(p_vmem,bmp+(BASE+4+1+VAL(pos)*10)*V_SIZE,X4,Y2-2);	
				show_hbmpmem(p_vmem,bmp+(BASE+6+1+VAL(pos)*10)*V_SIZE,X5,Y2-2);
				show_hbmpmem(p_vmem,bmp+(BASE+8+VAL(pos)*10)*V_SIZE,X6,Y2-2);	break;
			default:break;
		}
	}
}

void draw_horizontal_text2(COLOR_TYPE *p_vmem)
{
	show_hbmpmem(p_vmem,bmp_else+VAL(lang)*V_SIZE,X0,Y2-2);		
	show_hbmpmem(p_vmem,bmp_else+(2+VAL(lang))*V_SIZE,X3,Y2-2);		
	show_hbmpmem(p_vmem,bmp_else+(4+VAL(lang))*V_SIZE,X4,Y2-2);	
	show_hbmpmem(p_vmem,bmp_else+(6+VAL(lang))*V_SIZE,X5,Y2-2);
	show_hbmpmem(p_vmem,bmp_else+(8+VAL(lang))*V_SIZE,X6,Y2-2);
}




/*画主菜单所有的次菜单*/
void draw_vertical_text1(COLOR_TYPE *p_vmem)
{
	if(VAL(zoomed)==1) ;
	else {
		memcpy(bmp + 86 * V_SIZE, offset2 + VAL(lang) * V_SIZE * 6 + pt->dac_off_mode * 2 * V_SIZE, V_SIZE * 2);
		memcpy(bmp + 120 * V_SIZE, offset5 + VAL(lang) * V_SIZE * 4 + pt->store_mode * 2 * V_SIZE, V_SIZE * 2);
		if(pt->cal_mode==0) memcpy(bmp+66*V_SIZE,offset3+VAL(lang)*V_SIZE*4,V_SIZE*4);
		else memcpy(bmp+66*V_SIZE,offset4+VAL(lang)*V_SIZE*4,V_SIZE*4);
		pt->date_pos=0;	pt->time_pos=0;
		p[VAL(pos)][VAL(hor_pos)[VAL(pos)]][0](p_vmem,0);
		p[VAL(pos)][VAL(hor_pos)[VAL(pos)]][1](p_vmem,0);
		p[VAL(pos)][VAL(hor_pos)[VAL(pos)]][2](p_vmem,0);
		p[VAL(pos)][VAL(hor_pos)[VAL(pos)]][3](p_vmem,0);
		if ((VAL(hor_pos)[VAL(pos)]) != 4) {
			switch(VAL(ver_pos)[VAL(pos)][VAL(hor_pos)[VAL(pos)]]){
				case	0:
					show_vbmpmem(p_vmem,bmp+(40+0+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y0);
					show_vbmpmem(p_vmem,bmp+(40+2+1+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y4);
					show_vbmpmem(p_vmem,bmp+(40+4+1+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y5);
					show_vbmpmem(p_vmem,bmp+(40+6+1+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y6);
					break;
				case	1:
					show_vbmpmem(p_vmem,bmp+(40+0+1+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y0);
					show_vbmpmem(p_vmem,bmp+(40+2+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y4);
					show_vbmpmem(p_vmem,bmp+(40+4+1+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y5);
					show_vbmpmem(p_vmem,bmp+(40+6+1+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y6);
					break;
				case	2:	
					show_vbmpmem(p_vmem,bmp+(40+0+1+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y0);
					show_vbmpmem(p_vmem,bmp+(40+2+1+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y4);
					show_vbmpmem(p_vmem,bmp+(40+4+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y5);
					show_vbmpmem(p_vmem,bmp+(40+6+1+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y6);
					break;
				case	3:
					show_vbmpmem(p_vmem,bmp+(40+0+1+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y0);
					show_vbmpmem(p_vmem,bmp+(40+2+1+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y4);
					show_vbmpmem(p_vmem,bmp+(40+4+1+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y5);
					show_vbmpmem(p_vmem,bmp+(40+6+VAL(pos)*40+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y6);
					break;
				default:break;
			}
		} else {
			switch(VAL(ver_pos)[VAL(pos)][VAL(hor_pos)[VAL(pos)]]){
				case	0:
					show_vbmpmem(p_vmem,bmp+(40+0+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y0);
					show_vbmpmem(p_vmem,bmp+(40+2+1+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y4);
					show_vbmpmem(p_vmem,bmp+(40+4+1+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y5);
					show_vbmpmem(p_vmem,bmp+(40+6+1+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y6);
					break;
				case	1:
					show_vbmpmem(p_vmem,bmp+(40+0+1+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y0);
					show_vbmpmem(p_vmem,bmp+(40+2+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y4);
					show_vbmpmem(p_vmem,bmp+(40+4+1+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y5);
					show_vbmpmem(p_vmem,bmp+(40+6+1+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y6);
					break;
				case	2:	
					show_vbmpmem(p_vmem,bmp+(40+0+1+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y0);
					show_vbmpmem(p_vmem,bmp+(40+2+1+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y4);
					show_vbmpmem(p_vmem,bmp+(40+4+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y5);
					show_vbmpmem(p_vmem,bmp+(40+6+1+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y6);
					break;
				case	3:
					show_vbmpmem(p_vmem,bmp+(40+0+1+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y0);
					show_vbmpmem(p_vmem,bmp+(40+2+1+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y4);
					show_vbmpmem(p_vmem,bmp+(40+4+1+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y5);
					show_vbmpmem(p_vmem,bmp+(40+6+VAL(hor_pos)[VAL(pos)]*8)*V_SIZE,X1,Y6);
					break;
				default:break;
			}
		}
	}
}

/*增益修改函数*/
int write_db_val(int db,int old_db)
{
	unsigned char data_spi[]={0x00, 0x00};
	long val, val1, tmp;
	if (db <= 200) {
		db += pt->compensation;
		tmp = db + 65; val = 0x01;
	} else if (db <= 400) { 
		db += pt->compensation;
		tmp = db - 200 + 145; val = 0x11;
	} else if (db <= 740) {
		tmp = db - 400 + 65; val = 0x14;
	} else {
		db += 5;
		tmp = db - 740 + 85; val = 0x1a;
	}
	if (old_db <= 200) {
		val1 = 0x01;
	} else if (old_db <= 400) { 
		val1 = 0x11;
	} else if (old_db <= 740) {
		val1 = 0x14;
	} else if (old_db <= 1100){
		val1 = 0x1a;
	} else val1=0;
	if (val1 != val) {
		ioctl(pt->fd_gpio, DB_CHANGE, &val);
		Sleep(0, 100000);
	}
	tmp=(tmp - 0) * 3.2768;
	data_spi[0] = (tmp>>7)&0x3f;data_spi[1]=(tmp<<1)&0xfc; /*原始*/
	write(pt->fd_spi, data_spi, 2);
	return 0;
}

int test_val()/*测试值是否允许更改 允许返回1 不允许返回指定警告的代码*/
{
	unsigned int temp_val = VAL(range) * 2 / (VAL(mtlvel) / 1000.0);
	if (VAL(rectify) == 3) {/* 射频*/
		if((temp_val<16800)&&(temp_val>=100)) return 1;
		return	WARNING_RF;
	} else {
		if( VAL(power)) {/*功率高*/
			if(((temp_val) < 3500000) && (temp_val >= 100)) 
				return 1;
			return WARNING_POWERH;
		} else {/*功率低*/
			if ((VAL(range) > (VAL(mtlvel) / 12)) && 
					((VAL(range) * 11 / 10 + VAL(d_delay) + VAL(p_delay) * VAL(mtlvel) / 2000) < 
					 VAL(mtlvel) * 340) && (VAL(range) < (VAL(mtlvel) * 240)) && 
					(VAL(d_delay) < VAL(mtlvel) * 173) && (temp_val >= 100)) 
				return 1;
			return WARNING_POWERL;
		}
	}
}

/*a=1是闸门a a=2闸门b*/
int test_magval(int a)
{	
	int start,width;
	if(a==1) start=VAL(astart),width=VAL(awidth);
	else start=VAL(astart),width=VAL(bwidth);
	unsigned int temp_val = width*2/(VAL(mtlvel)/1000.0);
	if(VAL(rectify)==3){/* 射频*/
		if((temp_val<16800)&&(temp_val>=100)) return 1;
		return	WARNING_RF;
	}
	else{
		if(VAL(power)){/*功率高*/
			if(((temp_val)<3840000)&&(temp_val>=100)) return 1;
			return WARNING_POWERH;
		}
		else{/*功率低*/
			if((width>(VAL(mtlvel)/12))&&((width*11/10+start+VAL(p_delay)*VAL(mtlvel)/2000)<VAL(mtlvel)*340)&&
					(width<(VAL(mtlvel)*240))&&(start<VAL(mtlvel)*173)&&(temp_val>=100))
				return 1;
			return WARNING_POWERL;
		}
	}
}

int test_float_length(float i)
{
	if (i == 0) return 2;
	if (i< - 9.999) return 2;
	if ((- 9.999 < i) && (i < 0))	return 1;
	else if (i < 1.0)	return 1;
	else if (i < 10.0) return 1;
	else if (i < 100.0) return 2;
	else if (i < 1000.0) return 3;
	else if (i < 10000.0) return 4;
	else return 4;
}



void cal_str_display(COLOR_TYPE *p_vmem, char *temp,int class,int value,int reg,int pos,int x,int y)
{
	float temp_val;
	int temp_val1;
	if (VAL(unit) == 1) {/* 0为inch 1为mm */
		temp_val = ((float)(value)) / 1000;
		temp_val1 = test_float_length(temp_val);
		temp_val1 = 4 - temp_val1;
		if (reg == 0)
			sprintf(temp, "%.*f", temp_val1, temp_val);	/*temp_val1为小数点后的位数 微调123有个123 '*' */
		else if(reg==1)
			sprintf(temp, "\052%.*f", temp_val1, temp_val);
		else if(reg==2) 
			sprintf(temp, "\053%.*f", temp_val1, temp_val);
		else
			sprintf(temp, "\054%.*f", temp_val1, temp_val);
	} else {
		temp_val = (float)value / (1000 * 25.4);
		temp_val1 = test_float_length(temp_val);
		temp_val1 = 4 - (temp_val1 > 0 ? temp_val1 : 0);
		if (reg == 0) 
			sprintf(temp, "%.*f", temp_val1, temp_val);	/*temp_val1为小数点后的位数 微调123有个123 '*' */
		else if (reg == 1) 
			sprintf(temp, "\052%.*f", temp_val1, temp_val);
		else if (reg == 2)
			sprintf(temp, "\053%.*f", temp_val1, temp_val);
		else 
			sprintf(temp, "\054%.*f", temp_val1, temp_val);
	}
	switch (class) {
		case 1:
			if (VAL(unit) == 1)
				strcat(temp, "mm");
			else
				strcat(temp, "in");
			break;
		case 2:
			if (VAL(unit) == 1)
				strcat(temp, "m/s");
			else
				strcat(temp, "in/s");
			break;
		case 3:
			strcat(temp, "\346s");
			break;
		default:
			break;
	}
	if (pos) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, x, y);
		draw_str(p_vmem, temp, x + 1 + 3, y + 3, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE*117, x, y);
		draw_str(p_vmem, temp, x + 1 + 3, y + 3, 2, 3, FONT_COL2);
	}
}

static void t0(COLOR_TYPE *p_vmem, int a)/*范围修改函数*/
{ 
	char temp[12];
	int temp_value = VAL(range),info;
	if (NOTHING == a) ;
	else if (VAL(frez))  /*冻结状态*/
		show_info(p_vmem, WARNING_FREZ);
	else if (VAL(magnify)) /*放大状态*/
		//		show_info2(WARNING_FREZ);
		//		show_info2(WARNING_MAGNIFY);
		show_info(p_vmem, WARNING_MAGNIFY);
	else if (a == 3) {    /*切换微调*/
		if (VAL(range_reg) < 3)
			VAL(range_reg) += 1;
		else
			VAL(range_reg) = 0;
	} else {   /* 改变大小 */
		VAL(range) = value_change(temp_value, range_range,
				(VAL(range_reg) == 0) ? range_step0 : range_step1,
				range_range[sizeof(range_range) / sizeof(int) - 1], 
				range_range[0], a, (int)(VAL(range_reg)));
		info = test_val();
		if (info != 1)
			VAL(range) = temp_value, show_info(p_vmem, info);/*如果值不允许修改就要把值还原*/
	}
	memset(temp, 0, sizeof(temp));
	cal_str_display(p_vmem, temp, DISTANCE, VAL(range),
			VAL(range_reg), VAL(ver_pos)[0][0] == 0, X1, Y0 + 50);

	if (((a == 1) || (a == 2)) && (temp_value != VAL(range))) {
		STORE_DATA(RANGE, sizeof(VAL(range)))
//			true_dot_num = (VAL(range) / 1000) * 800 / (VAL(mtlvel) / 1000);
		senddsp_data();

		cal_avg_data();

		if ((VAL(dacecho) != 0) && (VAL(dacmode) != 0)) 
			cal_dac_data1();

	} else if (a == 3) 
		STORE_DATA(RANGE_REG, sizeof(VAL(range_reg)))
}

static void t1(COLOR_TYPE *p_vmem,int a)/*声速*/
{
	char temp[12];
	int temp_value=VAL(mtlvel),info;
	if (NOTHING ==a) ;
	else if (VAL(frez)) 
		show_info(p_vmem,WARNING_FREZ);
	else if (VAL(magnify)) 
		show_info(p_vmem,WARNING_MAGNIFY);
	else if (VAL(dacmode) != 0) 
		show_info(p_vmem,WARNING_DAC_MTL);/*dac 模式 不可调节*/
	else if (VAL(avg_status))
		show_info(p_vmem, WARNING_DAC_MTL);/*avg模式 不能调节*/
	else if (a == 3) {
		if (VAL(mtlvel_reg) < 3)
			VAL(mtlvel_reg) += 1;
		else
			VAL(mtlvel_reg) = 0;
	} else {
		VAL(mtlvel) = value_change(temp_value, mtlvel_range,
				(VAL(mtlvel_reg) == 0) ? mtlvel_step0 : mtlvel_step1,
				mtlvel_range[sizeof(mtlvel_range) / sizeof(int) - 1], 
				mtlvel_range[0], a, (int)(VAL(mtlvel_reg)));
		info = test_val();
		if (info != 1)
			VAL(mtlvel) = temp_value, show_info(p_vmem, info);/*如果值不允许修改就要把值还原*/
	}
	memset(temp, 0, sizeof(temp));
	cal_str_display(p_vmem, temp, RATE, VAL(mtlvel) * 1000,
			VAL(mtlvel_reg), VAL(ver_pos)[0][0] == 1, X1, Y4 + 50);
	if (((a == 1) || (a == 2)) && (temp_value != VAL(mtlvel))) {
		STORE_DATA(MTLVEL, sizeof(VAL(mtlvel)))
//			true_dot_num = (VAL(range) / 1000) * 800 / (VAL(mtlvel) / 1000);
		senddsp_data();
		cal_avg_data();
	}else if (a == 3) 
		STORE_DATA(MTLVEL_REG, sizeof(VAL(mtlvel_reg)))
}

static void t2(COLOR_TYPE *p_vmem,int a)/*扫描延时*/
{
	char temp[12];
	int temp_value=VAL(d_delay),info;
	if (NOTHING ==a) ;
	else if (VAL(frez))
		show_info(p_vmem,WARNING_FREZ);
	else if (VAL(magnify))
		show_info(p_vmem,WARNING_MAGNIFY);
	else if (VAL(dacmode) != 0) 
		show_info(p_vmem,WARNING_DAC_MTL);/*dac 模式 不可调节*/
	else if (a == 3) {
		if (VAL(d_delay_reg) < 3) 
			VAL(d_delay_reg) ++;
		else
			VAL(d_delay_reg) = 0;
	} else {
		VAL(d_delay) = value_change(temp_value, d_delay_range,
				(VAL(d_delay_reg) == 0) ? d_delay_step0 : d_delay_step1,
				d_delay_range[sizeof(d_delay_range) / sizeof(int) - 1], 
				d_delay_range[0], a, (int)(VAL(d_delay_reg)));
		info = test_val();
		if (info != 1)
			VAL(d_delay) = temp_value, show_info(p_vmem, info);/*如果值不允许修改就要把值还原*/
	}
	memset(temp, 0, sizeof(temp));
	cal_str_display(p_vmem, temp, DISTANCE, VAL(d_delay), 
			VAL(d_delay_reg), VAL(ver_pos)[0][0] == 2, X1, Y5 + 50);
	if (((a == 1) || (a == 2)) && (temp_value != VAL(d_delay))) {
		STORE_DATA(D_DELAY, sizeof(VAL(d_delay)))
			senddsp_data();
		cal_avg_data();
	} else if (a == 3) 
		STORE_DATA(D_DELAY_REG, sizeof(VAL(d_delay_reg)))
}

static void t3(COLOR_TYPE *p_vmem,int a)//厚度
{
	char temp[12];
	int temp_value = VAL(thickne);
	if ( NOTHING == a) ;
	else if (VAL(frez)) 
		show_info(p_vmem, WARNING_FREZ);
	else if (a == 3){
		if (VAL(thickne_reg) < 3)
			VAL(thickne_reg)++;
		else	
			VAL(thickne_reg) = 0;
	} else VAL(thickne) = value_change(temp_value, thickne_range,
			(VAL(thickne_reg) == 0) ? thickne_step0 : thickne_step1,
			thickne_range[sizeof(thickne_range) / sizeof(int) - 1],
			thickne_range[0], a, (int)VAL(thickne_reg));
	VAL(thickne) = 
		VAL(thickne) > ((VAL(diamet) > 2000000 ? 2000000 : VAL(diamet)) / 2) ? 
		temp_value : VAL(thickne);//限制代码
	memset(temp, 0, sizeof(temp));
	cal_str_display(p_vmem, temp, DISTANCE, VAL(thickne), 
			VAL(thickne_reg), VAL(ver_pos)[0][0] == 3, X1, Y6 + 50);
	if (((a == 1) || (a == 2)) && (temp_value != VAL(thickne))) 
		STORE_DATA(THICKNE, sizeof(VAL(thickne)))
	else if (a == 3) 
		STORE_DATA(THICKNE_REG, sizeof(VAL(thickne_reg)))
}

static void t4(COLOR_TYPE *p_vmem,int a)/*阻尼*/
{
	char temp_value = VAL(damping);
	int i;
	if (a == 0) ;
	else if (VAL(frez)) 
		show_info(p_vmem, WARNING_FREZ);
	else if (3== a) 
		(VAL(damping) < 3) ? (VAL(damping)++) : (VAL(damping = 0));
	else if (a == 1) 
		(VAL(damping) < 3) ? (VAL(damping)++) : (VAL(damping = 3));
	else if (a == 2)  
		(VAL(damping) > 0) ? (VAL(damping)--) : (VAL(damping = 0));
	memset(s_str, 0x00, sizeof(temp_string));
	switch (VAL(damping)) {
		case 0:
			sprintf(s_str, "%d\352", 50);break;
		case 1:
			sprintf(s_str, "%d\352", 100);break;
		case 2:
			sprintf(s_str, "%d\352", 200);break;
		case 3:
			sprintf(s_str, "%d\352", 500);break;
		default:
			break;
	}
	if (VAL(ver_pos)[0][1] == 0) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y0 + 50);
		draw_str(p_vmem, s_str, X1 + 4, 55, 2, 3, FONT_COL1);
	}else{
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, Y0 + 50);
		draw_str(p_vmem, s_str, X1 + 4, 55, 2, 3, FONT_COL2);
	}

	if (a && (temp_value != VAL(damping))) {
		switch (VAL(damping)) {
			case 0:
				ioctl(pt->fd_gpio,DAMPING_1,&i);break;
			case 1:
				ioctl(pt->fd_gpio,DAMPING_2,&i);break;
			case 2:
				ioctl(pt->fd_gpio,DAMPING_3,&i);break;
			case 3:
				ioctl(pt->fd_gpio,DAMPING_4,&i);break;
			default:
				break;
		}
		STORE_DATA(DAMPING, sizeof(VAL(damping)))
			Sleep(0, 16000);
	}
}

static void t5(COLOR_TYPE *p_vmem,int a)/*功率*/
{
	char temp_value=VAL(power);
	int i;
	if (a == 0) ;
	else if (VAL(frez)) 
		show_info(p_vmem, WARNING_FREZ);
	else if (3 == a) {
		if (VAL(power))
			t5(p_vmem,2);
		else
			t5(p_vmem,1);
	} else if ((a == 1) && (VAL(power) != 1)) {
		VAL(power) = 1;
		ioctl(pt->fd_gpio, POWER_HIGH, &i);
		*(pt->trans_data + 1024 * 2 + 4 * 2) = 0x01;
	}
	else if ((a == 2) && (VAL(power) != 0)) { 
		VAL(power) = 0;
		if (test_val() == 1){
			ioctl(pt->fd_gpio, POWER_LOW, &i);
			*(pt->trans_data + 1024 * 2 + 4 * 2) = 0x0;
		} else
			show_info(p_vmem, WARNING_HPOWER), VAL(power) = 1;
	}
	if (VAL(ver_pos)[0][1] == 1) {
		show_vbmpmem(p_vmem,
				bmp_content + VAL(power) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2, X1, Y4 + 50);
	} else {
		show_vbmpmem(p_vmem,
				bmp_content + VAL(power) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2 + 1 * V_SIZE, X1, Y4 + 50);
	}
	if (a && (temp_value != VAL(power))) {
		STORE_DATA(POWER, sizeof(VAL(power)))
			Sleep(0, 16000);
	}
}

static void t6(COLOR_TYPE *p_vmem,int a)/*双晶*/
{
	char temp_value=VAL(dual);
	int i;
	if (a == 0) ;
	else if (VAL(frez)) 
		show_info(p_vmem,WARNING_FREZ);
	else if (3 == a) {
		if (VAL(dual))
			t6(p_vmem,2);
		else
			t6(p_vmem,1);
	} else if ((a == 1) && (VAL(dual) != 1)) {
		VAL(dual) = 1;
		ioctl(pt->fd_gpio, DUAL_ON, &i);

	} else if ((a == 2) && (VAL(dual) != 0)) {
		VAL(dual) = 0;
		ioctl(pt->fd_gpio, DUAL_OFF, &i);
	}
	if (VAL(ver_pos)[0][1] == 2) {
		show_vbmpmem(p_vmem, bmp_content + 
				(2 + VAL(dual)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2, X1, Y5 + 50);
	} else {
		show_vbmpmem(p_vmem,bmp_content + 
				(2 + VAL(dual)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2 + 1 * V_SIZE, X1, Y5 + 50);
	}
	if (a && (temp_value != VAL(dual))) {
		STORE_DATA(DUAL, sizeof(VAL(dual)))
			Sleep(0, 16000);
	}
}

static void t7(COLOR_TYPE *p_vmem,int a)/*检波 频带 */
{
	unsigned char temp_value=VAL(f_band), *p;
	unsigned char data_spi3_init1[] = {0x90, 0xb5},
		  data_spi3_init2[] = {0x91, 0x16},
		  data_spi3_init3[] = {0x92, 0x16};
	if (a == 0) ;
	else if (3 == a)
		(VAL(f_band) < 3) ? VAL(f_band)++ : (VAL(f_band) = 0);
	else if ((a == 1) &&( VAL(f_band) < 3))
		VAL(f_band)++;
	else if ((a == 2) && (VAL(f_band) > 0))
		VAL(f_band)--;
	if (VAL(f_band) == 0)
		p = bmp_content + VAL(lang) * V_SIZE * 2;
	else if (VAL(f_band) == 1) 
		p = bmp_content + 16 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else if (VAL(f_band == 2)) 
		p = bmp_content + V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else 
		p = bmp_content + 15 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;

	switch(VAL(f_band)) {
		case 0: data_spi3_init1[1] = 0x1f,data_spi3_init2[1] = 0x03, data_spi3_init3[1] = 0x03;
				break;
		case 1: data_spi3_init1[1] = 0x47,data_spi3_init2[1] = 0x08, data_spi3_init3[1] = 0x08;
				break;
		case 2: 
		case 3: data_spi3_init1[1] = 0xb5,data_spi3_init2[1] = 0x16, data_spi3_init3[1] = 0x16;
				break;
		default:break;
	}
	write(pt->spi_sky, data_spi3_init1, 2);
	write(pt->spi_sky, data_spi3_init2, 2);
	write(pt->spi_sky, data_spi3_init3, 2);

	if (VAL(ver_pos)[0][1] == 3) {
		show_vbmpmem(p_vmem, p, X1, Y6 + 50);
	} else {
		show_vbmpmem(p_vmem, p + V_SIZE, X1, Y6 + 50);
	}
	if (a && (temp_value != VAL(f_band))) {
		STORE_DATA(F_BAND, sizeof(VAL(f_band)))
	}
#if 0	
	char temp_value = VAL(rectify);
	unsigned long *tdata = (unsigned long *)pt->map_base;
	if (a == 0) ;
	else if (VAL(frez)) 
		show_info(p_vmem, WARNING_FREZ);
	else if (3 == a)
		(VAL(rectify) < 3) ? (VAL(rectify)++) : (VAL(rectify) = 0);
	else if (a == 1)
		(VAL(rectify) < 3) ? (VAL(rectify)++) : (VAL(rectify) = 3);
	else if(a == 2)
		(VAL(rectify) > 0) ? (VAL(rectify)--) : (VAL(rectify) = 0);
	if (test_val() != 1) 
		show_info(p_vmem, WARNING_RF1), VAL(rectify) = temp_value;
	if (VAL(rectify) == 3) 
		VAL(alogic) = 0, VAL(blogic) = 0;
	else 
		VAL(alogic) = agatopen, VAL(blogic) = bgatopen;
	if (VAL(ver_pos)[0][1] == 3) {
		show_vbmpmem(p_vmem, bmp_content + 
				(4 + VAL(rectify)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2, X1,Y6 + 50);
	} else {
		show_vbmpmem(p_vmem, bmp_content + 
				(4 + VAL(rectify)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2 + 1 * V_SIZE, X1, Y6 + 50);
	}
	if (a && (temp_value != VAL(rectify))) {
		STORE_DATA(RECTIFY, sizeof(VAL(rectify)))
			*(tdata + 512 * 2 + 3) = VAL(rectify);
		Sleep(0, 16000);
	}
#endif
}


static void t8(COLOR_TYPE *p_vmem, int a)/*种类规格*/
{
	char temp2[6], temp3[3], temp4[5] ;
	char temp_char;
	unsigned int val;
	if (a == 0) ;
	else if (a == 3) {
		if (VAL(probe_class) == 1) {
			if (pt->probe_pos < 4) 
				pt->probe_pos++;
			else 
				pt->probe_pos = 0;
		} else {
			if (pt->probe_pos < 3) 
				pt->probe_pos++;
			else 
				pt->probe_pos = 0;
		}
	} else if (a == 1) {
		switch (pt->probe_pos) {
			case 1: VAL(probe_class) = !VAL(probe_class);break;
			case 2: if (VAL(probe_frequency) < 50) VAL(probe_frequency)++;
						else VAL(probe_frequency) = 1; break;
			case 3: if(VAL(probe_x) < 28) VAL(probe_x)++;
						else VAL(probe_x) = 3; break;
			case 4: if(VAL(probe_y) < 28) VAL(probe_y)++;
						else VAL(probe_y) = 3; break;
			default:break;
		}
	}else if(a==2){
		switch(pt->probe_pos){
			case 1: VAL(probe_class) = !VAL(probe_class);break;
			case 2: if (VAL(probe_frequency) > 1) VAL(probe_frequency)--;
						else VAL(probe_frequency) = 50; break;
			case 3: if(VAL(probe_x) > 3) VAL(probe_x)--;
						else VAL(probe_x) = 28; break;
			case 4: if(VAL(probe_y) > 3) VAL(probe_y)--;
						else VAL(probe_y) = 28; break;
			default:break;
		}
	}
	if (VAL(probe_frequency) <= 2 ) {
		ioctl(pt->fd_gpio,LOWF_1,&val);
	} else if (VAL(probe_frequency) <= 5 ) {
		ioctl(pt->fd_gpio,LOWF_2,&val);
	} else if (VAL(probe_frequency) <= 30) {
		ioctl(pt->fd_gpio,LOWF_3,&val);
	} else 
		ioctl(pt->fd_gpio,LOWF_4,&val);

	if (VAL(probe_frequency) <= 6 ) {
		pt->compensation = 3;
	} else if (VAL(probe_frequency) <= 8 ) {
		pt->compensation = 7;
	} else if (VAL(probe_frequency) <= 10) {
		pt->compensation = 10;
	} else if (VAL(probe_frequency) <= 20) {
		pt->compensation = 18;
	}
	memset(temp2, 0, sizeof(temp2));
	memset(temp3, 0, sizeof(temp3));
	memset(temp4, 0, sizeof(temp4));
	temp_char = VAL(probe_class) ? 'A' : 'N';
	sprintf(temp2, "%.1f", (float)(VAL(probe_frequency)) / 2.0);
	sprintf(temp3, "%d", VAL(probe_x));
	sprintf(temp4, "%d", VAL(probe_y));
	if (VAL(ver_pos)[0][2] == 0) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, 50);
		if (1 == pt->probe_pos) {
			fbrec(p_vmem, X1 + 9, 50, 16, 49, FONT_COL1);
			draw_ascii(p_vmem, temp_char, X1 + 10, 50, 2, 3, VER_DATA_COL1);
		} else	
			draw_ascii(p_vmem, temp_char, X1 + 10, 50, 2, 3, FONT_COL1);
		if (2 == pt->probe_pos) {
			fbrec(p_vmem, X1 + 40, 50, 32, 49, FONT_COL1);
			draw_str(p_vmem, temp2, X1 + 40, 50, 1, 3, VER_DATA_COL1);
		} else	
			draw_str(p_vmem, temp2, X1 + 40, 50, 1, 3, FONT_COL1);
		draw_ascii(p_vmem, 'P', X1 + 75, 50, 1, 3, FONT_COL1);
		if (3 == pt->probe_pos) {
			fbrec(p_vmem, X1 + 85, 50, 20, 49, FONT_COL1);
			draw_str(p_vmem, temp3, X1 + 87, 50, 1, 3, VER_DATA_COL1);
		} else	
			draw_str(p_vmem, temp3, X1 + 87, 50, 1, 3, FONT_COL1);
		if (VAL(probe_class)) {
			draw_ascii(p_vmem, 'X', X1 + 110, 50, 1, 3, FONT_COL1);
			if (4 == pt->probe_pos) {
				fbrec(p_vmem, X1 + 123, 50, 20, 49, FONT_COL1);
				draw_str(p_vmem, temp4, X1 + 123, 50, 1, 3, VER_DATA_COL1);
			} else	
				draw_str(p_vmem, temp4, X1 + 123, 50, 1, 3, FONT_COL1);
		}
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, 50);
		draw_ascii(p_vmem, temp_char, X1 + 10, 50, 2, 3, FONT_COL2);
		draw_str(p_vmem, temp2, X1 + 40, 50, 1, 3, FONT_COL2);
		draw_ascii(p_vmem, 'P', X1 + 75, 50, 1, 3, FONT_COL2);
		draw_str(p_vmem, temp3, X1 + 87, 50, 1, 3, FONT_COL2);
		if (VAL(probe_class)) {
			draw_ascii(p_vmem, 'X', X1 + 110, 50, 1, 3, FONT_COL2);
			draw_str(p_vmem, temp4, X1 + 123, 50, 1, 3, FONT_COL2);
		}
	}
	if (VAL(probe_class) == 1)
		VAL(angle) = 450;
	else
		VAL(angle) = 0;
	t10(p_vmem, 0);

	if ((a == 1) || (a == 2)) 
		STORE_DATA(PROBE_CLASS, sizeof(VAL(probe_class))*4)
}

static void t9(COLOR_TYPE *p_vmem,int a)/*探头延时*/
{
	char temp[12];
	int temp_value=VAL(p_delay),info;
	if (NOTHING == a) ;
	else if (VAL(frez))
		show_info(p_vmem,WARNING_FREZ);
	else if (VAL(dacmode) != 0) 
		show_info(p_vmem,WARNING_DAC_MTL);/*dac 模式 不可调节*/
	else if (a == 3) {
		if (VAL(p_delay_reg) < 3)
			VAL(p_delay_reg) += 1;
		else	
			VAL(p_delay_reg) = 0;
	} else {
		VAL(p_delay) = value_change(temp_value, p_delay_range,
				(VAL(p_delay_reg) == 0) ? p_delay_step0 : p_delay_step1,
				p_delay_range[sizeof(p_delay_range) / sizeof(int) - 1], 
				p_delay_range[0], a, (int)(VAL(p_delay_reg)));
		info = test_val();
		if (info != 1)
			VAL(p_delay) = temp_value, show_info(p_vmem, info);/*如果值不允许修改就要把值还原*/
	}
	memset(temp,0,sizeof(temp));
	cal_str_display(p_vmem, temp, ELSE, VAL(p_delay), 
			VAL(p_delay_reg), VAL(ver_pos)[0][2] == 1, X1, Y4 + 50);
	if (((a == 1) || (a == 2)) && (temp_value != VAL(p_delay))) {
		STORE_DATA(P_DELAY, sizeof(VAL(p_delay)))
			senddsp_data();
		cal_avg_data();
	} else if (a == 3) 
		STORE_DATA(P_DELAY_REG, sizeof(VAL(p_delay_reg)))
}

static void t10(COLOR_TYPE *p_vmem,int a)/*角度*/
{
	char temp[12];
	int temp_value=VAL(angle);
	if(a==0) ;
	else if (VAL(frez))
		show_info(p_vmem,WARNING_FREZ);
	else if (a == 3) {
		if (VAL(angle_reg) < 2)
			VAL(angle_reg)++;
		else
			VAL(angle_reg) = 0;
	}else VAL(angle) = value_change(temp_value, angle_range,
			VAL(angle_reg) ? angle_step1 : angle_step0,
			angle_range[sizeof(angle_range) / sizeof(int) - 1], angle_range[0], a, VAL(angle_reg));
	memset(temp, 0, sizeof(temp));
	if (VAL(angle_reg) == 0) 
		sprintf(temp, "%.1f/%.1f", (float)VAL(angle) / 10,
				tan(VAL(angle)*3.1415926/(10*180)));	// 微调前面会有个 '*'
	else if (VAL(angle_reg) == 1)
		sprintf(temp, "\052%.1f/%.1f", (float)VAL(angle) / 10,	// 微调前面会有个 '*'
				tan(VAL(angle)*3.1415926/(10*180)));	// 微调前面会有个 '*'
	else	
		sprintf(temp, "\053%.1f/%.1f", (float)VAL(angle) / 10,
				tan(VAL(angle)*3.1415926/(10*180)));	// 微调前面会有个 '*'
	if(VAL(ver_pos)[0][2]==2){
		show_vbmpmem(p_vmem,bmp_content+V_SIZE*116,X1,Y5+50);
		draw_str(p_vmem, temp, X1 + 1 + 3,Y5 + 55, 2, 3,FONT_COL1);
	}else{
		show_vbmpmem(p_vmem,bmp_content+V_SIZE*117,X1,Y5+50);
		draw_str(p_vmem, temp, X1 + 1 + 3,Y5 + 55, 2, 3,FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(angle))) {
		STORE_DATA(ANGLE, sizeof(VAL(angle)))
	} else if (a == 3)
		STORE_DATA(ANGLE_REG, sizeof(VAL(angle_reg)))
}

static void t11(COLOR_TYPE *p_vmem,int a) /*前沿*/
{
	char temp[12];
	int temp_value=VAL(x_value);
	float temp_val;
	if (a==0) ;
	else if (VAL(frez)) 
		show_info(p_vmem, WARNING_FREZ);
	else if (3 == a) {
		if(VAL(x_value_reg) < 3) 
			VAL(x_value_reg)++;
		else	
			VAL(x_value_reg) = 1;
	} else VAL(x_value) = value_change(temp_value, x_value_range, x_value_step,
			x_value_range[sizeof(x_value_range) / sizeof(int) - 1],
			x_value_range[0], a ,VAL(x_value_reg));
	memset(temp, 0, sizeof(temp));
	if (VAL(unit) == 1) {/* 0为inch 1为mm */
		temp_val = ((float)(VAL(x_value))) / 1000;
		if(VAL(x_value_reg) == 1)
			sprintf(temp, "\052%.1fmm", temp_val);
		else if(VAL(x_value_reg) == 2) 
			sprintf(temp, "\053%.1fmm",  temp_val);
		else
			sprintf(temp, "\054%.1fmm", temp_val);
	} else {
		temp_val = (float)VAL(x_value) / (1000 * 25.4);
		if (VAL(x_value_reg) == 1) 
			sprintf(temp, "\052%.1fIN",  temp_val);
		else if (VAL(x_value_reg) == 2)
			sprintf(temp, "\053%.1fIN",  temp_val);
		else 
			sprintf(temp, "\054%.1fIN",  temp_val);
	}
	if (VAL(ver_pos)[0][2] == 3) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y6 + 50);
		draw_str(p_vmem, temp, X1 + 5, Y6 + 55, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE*117, X1, Y6 + 50);
		draw_str(p_vmem, temp, X1 + 5, Y6 + 55, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(x_value))) 
		STORE_DATA(X_VALUE, sizeof(VAL(x_value)))
	else if (a == 3) 
		STORE_DATA(X_VALUE_REG, sizeof(VAL(x_value_reg)))
}

static void t12(COLOR_TYPE *p_vmem,int a) /*校准选项 声速 或者 角度*/
{
	int refresh_mark=0;
	if (a == 0) ;
	else if (VAL(frez))
		show_info(p_vmem, WARNING_FREZ);
	else if ((a == 3) )
		pt->cal_mode = !pt->cal_mode, refresh_mark = 1;

	if(refresh_mark){
		draw_rim(pt->virtual_video_add,0x0);
		draw_horizontal_text1(pt->virtual_video_add);
		draw_vertical_text1(pt->virtual_video_add);
	}

	if (VAL(ver_pos)[0][3] == 0) {
		if (!pt->cal_mode) 
			show_vbmpmem(p_vmem, bmp_content + V_SIZE * 120 + VAL(lang) * V_SIZE * 2, X1, 50);
		else
			show_vbmpmem(p_vmem, bmp_content + V_SIZE * 124 + VAL(lang) * V_SIZE * 2, X1, 50);

	} else {
		if (!pt->cal_mode)
			show_vbmpmem(p_vmem, bmp_content + V_SIZE * 120 + V_SIZE + VAL(lang) * V_SIZE * 2, X1, 50);
		else
			show_vbmpmem(p_vmem, bmp_content + V_SIZE * 124 + V_SIZE + VAL(lang) * V_SIZE * 2, X1, 50);
	}
}

static void t13(COLOR_TYPE *p_vmem,int a) /*参照物1  圆孔深度*/
{
	char temp[12];
	if (pt->cal_mode == 0) {
		int temp_value=VAL(s_ref1);
		if(a==0) ;
		else if(a==3){
			if(VAL(s_ref1_reg) < 3) VAL(s_ref1_reg)++;
			else	VAL(s_ref1_reg)=0;
		}else VAL(s_ref1)=value_change(temp_value,s_ref_range1,
				VAL(s_ref1_reg) ? s_ref_step1 : s_ref_step00, 
				s_ref_range1[sizeof(s_ref_range1) / sizeof(int) - 1], s_ref_range1[0], a, VAL(s_ref1_reg));
		memset(temp,0,sizeof(temp));
		cal_str_display(p_vmem,temp,1,VAL(s_ref1),VAL(s_ref1_reg),(VAL(ver_pos)[0][3] == 1),X1,Y4 + 50);

		if (((a == 1) || (a == 2)) && (temp_value != VAL(s_ref1))) {
			STORE_DATA(S_REF1, sizeof(VAL(s_ref1)))
				VAL(s_ref2)=2*VAL(s_ref1);
			t14(p_vmem, 0);
		}else if (a == 3)
			STORE_DATA(S_REF1_REG, sizeof(VAL(s_ref1_reg)))	
	} else {
		int temp_value=VAL(cal_h);
		float temp_val;
		if (a==0) ;
		else if (VAL(frez)) 
			show_info(p_vmem, WARNING_FREZ);
		else if (3 == a) {
			if(VAL(cal_h_reg) < 3) 
				VAL(cal_h_reg)++;
			else	
				VAL(cal_h_reg) = 1;
		} else VAL(cal_h) = value_change(temp_value, cal_h_range, cal_h_step,
				cal_h_range[sizeof(cal_h_range) / sizeof(int) - 1],
				cal_h_range[0], a ,VAL(cal_h_reg));

		memset(temp, 0, sizeof(temp));
		if (VAL(unit) == 1) {/* 0为inch 1为mm */
			temp_val = ((float)(VAL(cal_h))) / 1000;
			if(VAL(cal_h_reg) == 1)
				sprintf(temp, "\052%.1fmm", temp_val);
			else if(VAL(cal_h_reg) == 2) 
				sprintf(temp, "\053%.1fmm",  temp_val);
			else
				sprintf(temp, "%.1fmm", temp_val);
		} else {
			temp_val = (float)VAL(x_value) / (1000 * 25.4);
			if (VAL(cal_h_reg) == 1) 
				sprintf(temp, "\052%.1fIN",  temp_val);
			else if (VAL(cal_h_reg) == 2)
				sprintf(temp, "\053%.1fIN",  temp_val);
			else 
				sprintf(temp, "%.1fIN",  temp_val);
		}
		if (VAL(ver_pos)[0][3] == 1) {
			show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y4 + 50);
			draw_str(p_vmem, temp, X1 + 5,Y4 + 55, 2, 3, FONT_COL1);
		} else {
			show_vbmpmem(p_vmem, bmp_content + V_SIZE*117, X1, Y4 + 50);
			draw_str(p_vmem, temp, X1 + 5,Y4 + 55, 2, 3, FONT_COL2);
		}
		if (((a == 1) || (a == 2)) && (temp_value != VAL(x_value))) 
			STORE_DATA(CAL_H,sizeof(VAL(cal_h)))
		else if (a == 3) 
			STORE_DATA(CAL_H_REG, sizeof(VAL(cal_h_reg)))
	}

}

static void t14(COLOR_TYPE *p_vmem,int a) /*参照物2 孔径*/
{
	char temp[12];
	if (pt->cal_mode == 0) {
		int temp_value = VAL(s_ref2);
		if (0 == a) ;
		else if (a == 3) {
			if (VAL(s_ref2_reg) != 3)
				VAL(s_ref2_reg)++;
			else
				VAL(s_ref2_reg) = 0;
		} else VAL(s_ref2)=value_change(temp_value, s_ref_range,
				VAL(s_ref2_reg) ? s_ref_step1 : s_ref_step0, 
				s_ref_range[sizeof(s_ref_range) / sizeof(int) - 1], s_ref_range[0], a, VAL(s_ref2_reg));
		memset(temp, 0, sizeof(temp));
		cal_str_display(p_vmem, temp, 1, VAL(s_ref2), VAL(s_ref2_reg), (VAL(ver_pos)[0][3] == 2), X1, Y5 + 50);
		if (((a == 1) || (a == 2)) && (temp_value != VAL(s_ref2))) 
			STORE_DATA(S_REF2, sizeof(VAL(s_ref2)))
		else if(a == 3)
			STORE_DATA(S_REF2_REG, sizeof(VAL(s_ref2_reg)))	
	} else {
		int temp_value=VAL(cal_d);
		float temp_val;
		if (a==0) ;
		else if (VAL(frez)) 
			show_info(p_vmem, WARNING_FREZ);
		else if (3 == a) {
			if(VAL(cal_d_reg) < 2) 
				VAL(cal_d_reg)++;
			else	
				VAL(cal_d_reg) = 1;
		} else VAL(cal_d) = value_change(temp_value, avg_d_range, avg_d_step,
				avg_d_range[sizeof(avg_d_range) / sizeof(int) - 1],
				avg_d_range[0], a ,VAL(cal_d_reg));

		memset(temp, 0, sizeof(temp));
		if (VAL(unit) == 1) {/* 0为inch 1为mm */
			temp_val = ((float)(VAL(cal_d))) / 1000;
			if(VAL(cal_d_reg) == 1)
				sprintf(temp, "\052%.1fmm", temp_val);
			else if(VAL(cal_d_reg) == 2) 
				sprintf(temp, "\053%.1fmm",  temp_val);
			else
				sprintf(temp, "%.1fmm", temp_val);
		} else {
			temp_val = (float)VAL(x_value) / (1000 * 25.4);
			if (VAL(cal_d_reg) == 1) 
				sprintf(temp, "\052%.1fIN",  temp_val);
			else if (VAL(cal_d_reg) == 2)
				sprintf(temp, "\053%.1fIN",  temp_val);
			else 
				sprintf(temp, "%.1fIN",  temp_val);
		}
		if (VAL(ver_pos)[0][3] == 2) {
			show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y5 + 50);
			draw_str(p_vmem, temp, X1 + 5,Y5 + 55, 2, 3, FONT_COL1);
		} else {
			show_vbmpmem(p_vmem, bmp_content + V_SIZE*117, X1, Y5 + 50);
			draw_str(p_vmem, temp, X1 + 5,Y5 + 55, 2, 3, FONT_COL2);
		}
		if (((a == 1) || (a == 2)) && (temp_value != VAL(x_value))) 
			STORE_DATA(CAL_D,sizeof(VAL(cal_d)))
		else if (a == 3) 
			STORE_DATA(CAL_D_REG, sizeof(VAL(cal_d_reg)))
	}
}

static void t15(COLOR_TYPE *p_vmem,int a) /*校准*/
{
	char temp[20];
	if (pt->cal_mode == 0) {
		int i,j;
		if (a == 0) ;
		else if (a == 3) {
		} else if (VAL(frez))
			show_info(p_vmem,WARNING_FREZ);
		else if (VAL(alogic) == 0)
			show_info(p_vmem,WARNING_GATE);	
		else if (a == 1) {
			if (pt->cal_pos < 2) pt->cal_pos++;
		} else if (a == 2) 
			pt->cal_pos = 0;
			

		if ((pt->cal_pos==1)&&(a==1)) {
			if(pt->measure_cur_data->Sa!=0)
				cal_time[0]=cost_time*(gat_max[pt->a_gata_max]*1.2/(float)(pt->dot_quantily));
			else {
				show_info(p_vmem,WARNING_CAL_ERROR);
				pt->cal_pos=0;
			}
		}
		if((pt->cal_pos==2)&&(a==1)){
			if(pt->measure_cur_data->Sa!=0){
				cal_time[1]=cost_time*(gat_max[pt->a_gata_max]*1.2/(float)(pt->dot_quantily));

				i=((VAL(s_ref2)-VAL(s_ref1))*2.0)/((cal_time[1]-cal_time[0])/1000.0);
				j=cal_time[0]-(VAL(s_ref1)*2.0/(i/1000.0));

				if((i>=1000)&&(i<=15000)&&(j>0)){
					VAL(mtlvel)=i;
					VAL(p_delay)=cal_time[0]-(VAL(s_ref1)*2.0/(VAL(mtlvel)/1000.0));
					senddsp_data();
					i2c_add[1]=4;
					write_i2c(pt->fd_config,i2c_add,pt->p_config+4,4);
					i2c_add[1]=12;
					write_i2c(pt->fd_config,i2c_add,pt->p_config+12,4);
					show_info(p_vmem,WARNING_CAL_OK);
				}else show_info(p_vmem,WARNING_CAL_ERROR);
				pt->cal_pos=0;
			}else {
				show_info(p_vmem,WARNING_CAL_ERROR);
				pt->cal_pos=0;
			}
		}
		memset(temp,0,sizeof(temp));
		sprintf(temp," %dm/s  %.2f %d",VAL(mtlvel),VAL(p_delay)/1000.0,pt->cal_pos == 1 ? 1 : 0);
		if (VAL(ver_pos)[0][3] == 3) {
			show_vbmpmem(p_vmem, bmp_content + 116 * V_SIZE, X1, Y6 + 50);
			draw_str(p_vmem, temp, X1 + 5, Y6 + 55, 1, 3, FONT_COL1);
		} else {
			show_vbmpmem(p_vmem, bmp_content + 117 * V_SIZE, X1, Y6 + 50);
			draw_str(p_vmem, temp, X1 + 5, Y6 + 55, 1, 3, FONT_COL2);
		}
	} else {
		int temp_angle;
		if (a == 0) ;

		else if (a == 1) {
			temp_angle = 1800.0 * acos((float)(VAL(cal_h)) / \
					(float)(pt->measure_cur_data->Sa + VAL(cal_d) / 2.0)) / 3.1415926;  
			if ((temp_angle <= 850)  && (temp_angle >= 0)) {
				VAL(angle) = temp_angle;
				//				t10(p_vmem,0);
			} else 
				show_info(p_vmem,WARNING_CAL_ERROR);
		}
		memset(temp,0,sizeof(temp));
		sprintf(temp,"  %.1f\370  %.1f",VAL(angle)/10.0,
				tan(VAL(angle)*3.1415926/(10*180)));	
		if (VAL(ver_pos)[0][3] == 3) {
			show_vbmpmem(p_vmem, bmp_content + 116 * V_SIZE, X1, Y6 + 50);
			draw_str(p_vmem, temp, X1 + 5, Y6 + 55, 1, 3, FONT_COL1);
		} else {
			show_vbmpmem(p_vmem, bmp_content + 117 * V_SIZE, X1, Y6 + 50);
			draw_str(p_vmem, temp, X1 + 5, Y6 + 55, 1, 3, FONT_COL2);
		}
	}
}


static void t16(COLOR_TYPE *p_vmem,int a)/*闸门A逻辑*/
{
	unsigned char *p, temp_value = VAL(alogic);
	if (a == 0 || a == 3) ;
	else if (VAL(rectify) == 3 )
		show_info(p_vmem,WARNING_RF);
	else if ((a == 1) && (VAL(alogic) <2 )) 
		VAL(alogic)++;
	else if (a == 2){ 
		switch(VAL(alogic)){
			case 0:break;
			case 1:if(1==VAL(magnify)) break;
			default:VAL(alogic)--;break;
		}
	}
	if (VAL(alogic) == 0)
		p = bmp_content + 2 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else if (VAL(alogic) == 1)
		p = bmp_content + 8 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else p = bmp_content + 9 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;

	if (VAL(ver_pos)[VAL(pos)][4] == 0) {
		show_vbmpmem(p_vmem, p, X1, 50);
	} else {
		show_vbmpmem(p_vmem, p + V_SIZE, X1, 50);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(alogic))) {
		agatopen = VAL(alogic);
		STORE_DATA(A_LOGIC, sizeof(VAL(alogic)))
	}
}

static void t17(COLOR_TYPE *p_vmem,int a)/*闸门A起点*/
{
	char temp[12];
	gat_start_range[0] = 0; gat_start_step0[0] = 0;
	int temp_value=VAL(astart);
	if (a == 0) ;
	else if (VAL(alogic) == 0)
		show_info(p_vmem,WARNING_GATE);
	else if (a == 3) {
		if(VAL(astart_reg)<3) VAL(astart_reg)+=1;
		else	VAL(astart_reg)=0;
	} else VAL(astart) = value_change(temp_value, gat_start_range,
			VAL(astart_reg) ? gat_start_step1 : gat_start_step0,
			gat_start_range[sizeof(gat_start_range) / sizeof(int) - 1],
			gat_start_range[0], a, VAL(astart_reg));
	memset(temp, 0, sizeof(temp));
	cal_str_display(p_vmem, temp, DISTANCE, VAL(astart), 
			VAL(astart_reg), VAL(ver_pos)[VAL(pos)][4] == 1, X1,Y4 + 50);
	if (((a == 1) || (a == 2)) && (temp_value != VAL(astart))) {
		STORE_DATA(A_START, sizeof(VAL(astart)))
			senddsp_data();
		pt->agat_y = 512, pt->agat_x = 0;
	} else if (a == 3)
		STORE_DATA(ASTART_REG, sizeof(VAL(astart_reg)))

			int i;
	for (i = 0; i < VAL(dacecho); i++)
		printf("VALx(%d)= %d y = %f\n", i + 1, VAL(dac_data_x)[i], VAL(dac_data_y)[i]);
}

static void t18(COLOR_TYPE *p_vmem,int a)/*闸门A宽度*/
{
	char temp[12];
	gat_start_range[0] = 20; gat_start_step0[0] = 20;
	int temp_value=VAL(awidth);
	if (a == 0) ;
	else if (VAL(alogic) == 0) 
		show_info(p_vmem, WARNING_GATE);
	else if (a == 3) {
		if (VAL(awidth_reg) < 3) VAL(awidth_reg)++;
		else VAL(awidth_reg) = 0;
	} else VAL(awidth) = value_change(temp_value, gat_start_range,
			VAL(awidth_reg) ? gat_start_step1 : gat_start_step0,
			gat_start_range[sizeof(gat_start_range) / sizeof(int) - 1],
			gat_start_range[0], a, VAL(awidth_reg));
	if ((VAL(awidth) == gat_start_range[0]) && (1 == VAL(magnify)))	
		VAL(awidth)=temp_value;
	memset(temp, 0, sizeof(temp));
	cal_str_display(p_vmem, temp, DISTANCE, VAL(awidth), 
			VAL(awidth_reg), VAL(ver_pos)[VAL(pos)][4] == 2, X1, 250);
	if (((a == 1) || (a == 2)) && (temp_value != VAL(awidth))) {
		STORE_DATA(A_WIDTH, sizeof(VAL(awidth)))
			senddsp_data();
		pt->agat_y=512,pt->agat_x=0;
	} else if (a == 3) 
		STORE_DATA(AWIDTH_REG, sizeof(VAL(awidth_reg)))
}

static void t19(COLOR_TYPE *p_vmem,int a)/*闸门A高度*/
{
	char temp[12], *ss="% ", temp_value = VAL(athrsh);
	if (a == 0 || a == 3) ;
	else if (VAL(alogic) == 0)
		show_info(p_vmem,WARNING_GATE);
	else if ((a == 1) && (VAL(athrsh) < 92))
		VAL(athrsh) += 3;
	else if ((a == 2) && (VAL(athrsh) > 8))
		VAL(athrsh) -= 3;

	memset(temp, 0, sizeof(temp));
	sprintf(temp, "%d%s" , VAL(athrsh), ss);
	if(VAL(ver_pos)[VAL(pos)][4]==3){
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, 350);
		draw_str(p_vmem, temp, X1 + 1 + 3, 355, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, 350);
		draw_str(p_vmem, temp, X1 + 1 + 3, 355, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(athrsh)))
		STORE_DATA(A_THRSH, sizeof(VAL(athrsh)))
}

static void t20(COLOR_TYPE *p_vmem,int a)/*DAC模式*/
{
	unsigned char *p, temp_value = VAL(dacmode);
	if (a == 0);
	else if (VAL(frez))
		show_info(p_vmem, WARNING_FREZ);
	else if (a == 3)
		(VAL(dacmode) < 3) ? (VAL(dacmode)++) : (VAL(dacmode) = 0);
	else if (a == 1)
		(VAL(dacmode) < 3) ? (VAL(dacmode)++) : (VAL(dacmode) = 3);
	else if ((a == 2) && (VAL(dacmode) > 0))
		(VAL(dacmode) > 0) ? (VAL(dacmode)--) : (VAL(dacmode) = 0);

	if (VAL(dacmode) == 0)
		p = bmp_content + 2 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else if (VAL(dacmode) == 1)
		p = bmp_content + 28 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else if (VAL(dacmode) == 2)
		p = bmp_content + 27 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else 
		p = bmp_content + 32 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;

	if (VAL(ver_pos)[1][0] == 0) 
		show_vbmpmem(p_vmem, p, X1 , 50);
	else 
		show_vbmpmem(p_vmem, p + V_SIZE, X1 , 50);

	if (((a == 1) || (a == 2)) && (temp_value != VAL(dacmode)))
		STORE_DATA(DACMODE, sizeof(VAL(dacmode)))
}

static void t21(COLOR_TYPE *p_vmem,int a)/*DAC回波*/
{
	char temp[12];
	char temp_value = VAL(dacecho);
	if (VAL(dacmode == 1)) {
		if (a == 0 || a == 3) ;
		else if (VAL(frez)) 
			show_info(p_vmem, WARNING_FREZ);
		else if (VAL(dacmode) != 1) 
			show_info(p_vmem, WARNING_DAC);
		else if (a == 1) {
			switch (VAL(dacecho)) {
				case 8:break;
				case 0:
					   if ((pt->agat_y < 380) && (pt->agat_y > 1)) {
						   VAL(dacecho) = 1;
						   VAL(dac_data_x)[0] = pt->measure_cur_data->Sa;
						   VAL(dac_data_y)[0] = (float)((400 - pt->agat_y) > 0 ? (400-pt->agat_y) : 0);
					   } else
						   show_info(p_vmem, WARNING_DACECHO);
					   break;
				default:
					   if (pt->agat_y < 380 && ((400 - pt->agat_y) < VAL(dac_data_y)[VAL(dacecho-1)])) {
						   VAL(dac_data_x)[VAL(dacecho)] = pt->measure_cur_data->Sa;
						   VAL(dac_data_y)[VAL(dacecho)] = (float)((400 - pt->agat_y) > 0 ? (400-pt->agat_y) : 0);
						   VAL(dacecho)++;
					   } else {
						   VAL(dac_data_x)[VAL(dacecho)] = 0;
						   VAL(dac_data_y)[VAL(dacecho)] = 0;
						   show_info(p_vmem, WARNING_DACECHO);
					   }
					   break;
			}
		} else if (a == 2) {
			switch (VAL(dacecho)) {
				case 0:break;
				default:VAL(dacecho)--;
						VAL(dac_data_y)[VAL(dacecho)] = 0;
						break;
			}
		}
		memset(temp, 0, sizeof(temp));
		sprintf(temp, "%d", VAL(dacecho));
		if (VAL(dacecho) && VAL(dacmode))
			cal_dac_data1();
	} else if (VAL(dacmode) == 3) {
		if (a == 3)
			(pt->dac_cal_pos < VAL(dacecho)) ? (pt->dac_cal_pos++) : (pt->dac_cal_pos = 0);
		else if (a == 1) {
			if ( (VAL(dac_data_y)[pt->dac_cal_pos] > 0 )&&
						   (VAL(dac_data_y)[pt->dac_cal_pos] < 400.0) )
						   VAL(dac_data_y)[pt->dac_cal_pos] += 1.0;
		} else if (a == 2) { 
			if ( (VAL(dac_data_y)[pt->dac_cal_pos] > 0 )&&
						   (VAL(dac_data_y)[pt->dac_cal_pos] < 400.0) )
						   VAL(dac_data_y)[pt->dac_cal_pos] -= 1.0;
		}
		sprintf(temp, "%d", pt->dac_cal_pos);
		if (VAL(dacecho) && VAL(dacmode))
			cal_dac_data1();
	}
	if (VAL(dacmode) != 3) {
		memset(temp, 0, sizeof(temp));
		sprintf(temp, "%d", VAL(dacecho));
	}


	if (VAL(ver_pos)[1][0] == 1) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y4 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, 155, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, Y4 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, 155, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(dacecho))) {
		STORE_DATA(DACECHO, sizeof(VAL(dacecho)))
			STORE_DATA(DAC_DATA_X, (sizeof(int) * 5))
			STORE_DATA(128, (sizeof(int) * 8))
			STORE_DATA(160, (sizeof(int) * 3))
	}
}

static void t22(COLOR_TYPE *p_vmem,int a)/*补偿 */
{
	char temp[12];
	char temp_value = VAL(rec);
	if ((a == 0) || VAL(frez) || (a == 3)) ;
	else if ((1 == a) && (VAL(rec) < 28 ))
		VAL(rec)++;
	else if ((2 == a) && (VAL(rec) > 0))
		VAL(rec)--;
	memset(temp, 0, sizeof(temp));
	sprintf(temp, "%.1fdB ",(float)VAL(rec) / 2);
	if (VAL(ver_pos)[1][0] == 2){
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y5 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, Y5 + 55, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, Y5 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, Y5 + 55, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(rec))) {
		STORE_DATA(REC, (sizeof(VAL(rec))))
		if (VAL(dacecho) && VAL(dacmode))
			cal_dac_data1();
	}
}

static void t23(COLOR_TYPE *p_vmem,int a)/*间距*/
{
	char temp[12];
	int temp_value =  VAL(off_db)[pt->dac_off_mode], refresh_mark=0;
	if (a == 0) ;
	else if (VAL(frez))
		show_info(p_vmem, WARNING_FREZ);
	else if (VAL(dacmode) != 1)
		show_info(p_vmem,WARNING_DAC);
	else if (a == 3) {
		if (pt->dac_off_mode < 2) pt->dac_off_mode++;
		else pt->dac_off_mode = 0;
		refresh_mark = 1;
		temp_value = VAL(off_db)[pt->dac_off_mode];
	} else VAL(off_db)[pt->dac_off_mode] = value_change(VAL(off_db)[pt->dac_off_mode], offset_range1, offset_step,
			offset_range1[sizeof(offset_range1) / sizeof(int) - 1],
			offset_range1[0], a, 1);

	if (refresh_mark) {
		draw_rim(pt->virtual_video_add,0x0);
		draw_horizontal_text1(pt->virtual_video_add);
		draw_vertical_text1(pt->virtual_video_add);
	}

	memset(temp,0,sizeof(temp));
	sprintf(temp, "%.1fdB ",(float)VAL(off_db)[pt->dac_off_mode] / 10);

	if (VAL(ver_pos)[1][0] == 3){
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y6 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, Y6 + 55, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, Y6 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, Y6 + 55, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(off_db)[pt->dac_off_mode])) {
		STORE_DATA(OFF_DB + 2 * pt->dac_off_mode, (sizeof(short)))
			if (VAL(dacecho) && VAL(dacmode))
				cal_dac_data1();
	}
}

static void t24(COLOR_TYPE *p_vmem,int a)/*目标孔径*/
{
	char temp[12];
	int temp_value=VAL(avg_d1);
	float temp_val;
	if (a==0) ;
	else if (VAL(frez)) 
		show_info(p_vmem, WARNING_FREZ);
	else if (3 == a) {
		if(VAL(avg_d1_reg) < 2) 
			VAL(avg_d1_reg)++;
		else	
			VAL(avg_d1_reg) = 1;
	} else VAL(avg_d1) = value_change(temp_value, avg_d_range, avg_d_step,
			avg_d_range[sizeof(avg_d_range) / sizeof(int) - 1],
			avg_d_range[0], a ,VAL(avg_d1_reg));

	memset(temp, 0, sizeof(temp));
	if (VAL(unit) == 1) {/* 0为inch 1为mm */
		temp_val = ((float)(VAL(avg_d1))) / 1000;
		if(VAL(avg_d1_reg) == 1)
			sprintf(temp, "\052%.1fmm", temp_val);
		else if(VAL(avg_d1_reg) == 2) 
			sprintf(temp, "\053%.1fmm",  temp_val);
		else
			sprintf(temp, "%.1fmm", temp_val);
	} else {
		temp_val = (float)VAL(x_value) / (1000 * 25.4);
		if (VAL(avg_d1_reg) == 1) 
			sprintf(temp, "\052%.1fIN",  temp_val);
		else if (VAL(avg_d1_reg) == 2)
			sprintf(temp, "\053%.1fIN",  temp_val);
		else 
			sprintf(temp, "%.1fIN",  temp_val);
	}
	if (VAL(ver_pos)[1][1] == 0) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y0 + 50);
		draw_str(p_vmem, temp, X1 + 5, 55, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE*117, X1, Y0 + 50);
		draw_str(p_vmem, temp, X1 + 5, 55, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(x_value))) 
		STORE_DATA(AVG_D1, sizeof(VAL(avg_d1)))
	else if (a == 3) 
		STORE_DATA(AVG_D1_REG, sizeof(VAL(avg_d1_reg)))
}

static void t25(COLOR_TYPE *p_vmem,int a)/*参考曲线*/
{
	char temp[12];
	int temp_value=VAL(avg_d2);
	float temp_val;
	if (a==0) ;
	else if (VAL(frez)) 
		show_info(p_vmem, WARNING_FREZ);
	else if (3 == a) {
		if(VAL(avg_d2_reg) < 2) 
			VAL(avg_d2_reg)++;
		else	
			VAL(avg_d2_reg) = 1;
	} else VAL(avg_d2) = value_change(temp_value, avg_d_range, avg_d_step,
			avg_d_range[sizeof(avg_d_range) / sizeof(int) - 1],
			avg_d_range[0], a ,VAL(avg_d2_reg));

	memset(temp, 0, sizeof(temp));
	if (VAL(unit) == 1) {/* 0为inch 1为mm */
		temp_val = ((float)(VAL(avg_d2))) / 1000;
		if(VAL(avg_d2_reg) == 1)
			sprintf(temp, "\052%.1fmm", temp_val);
		else if(VAL(avg_d2_reg) == 2) 
			sprintf(temp, "\053%.1fmm",  temp_val);
		else
			sprintf(temp, "%.1fmm", temp_val);
	} else {
		temp_val = (float)VAL(x_value) / (1000 * 25.4);
		if (VAL(avg_d2_reg) == 1) 
			sprintf(temp, "\052%.1fIN",  temp_val);
		else if (VAL(avg_d2_reg) == 2)
			sprintf(temp, "\053%.1fIN",  temp_val);
		else 
			sprintf(temp, "%.1fIN",  temp_val);
	}
	if (VAL(ver_pos)[1][1] == 1) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y4 + 50);
		draw_str(p_vmem, temp, X1 + 5, Y4 + 55, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE*117, X1, Y4 + 50);
		draw_str(p_vmem, temp, X1 + 5, Y4 + 55, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(x_value))) 
		STORE_DATA(AVG_D2, sizeof(VAL(avg_d2)))
	else if (a == 3) 
		STORE_DATA(AVG_D2_REG, sizeof(VAL(avg_d2_reg)))
}


static void cal_avg_data()
{
	if (VAL(avg_status) && VAL(avg_data_x) && VAL(avg_data_y) && (VAL(probe_class == 0))) {
		float tmp;
		int i;
		avg_dot_data[0] = 512;
		for (i = 1; i < 400; i++) {
			tmp = (float)(VAL(avg_data_x)) / (pt->true_delay + (float)(pt->true_range) * (i/400.0));
			avg_dot_data[i] = tmp * tmp * VAL(avg_data_y);
		}
	}
}

static void t26(COLOR_TYPE *p_vmem,int a)/*制作*/
{
	int temp_value = VAL(avg_status);
	float tmp;
	if (a == 0) ;
	else if (VAL(frez)) 
		show_info(p_vmem, WARNING_FREZ);
	else if (3 == a) {
		if (pt->measure_cur_data->Sa && VAL(alogic)) {
			tmp = (float)(VAL(avg_d2)) /VAL(avg_d1);
			VAL(avg_data_x) = pt->measure_cur_data->Sa;
			VAL(avg_data_y) = tmp * tmp * (400.0 - pt->current_data[pt->a_gata_max]);
			//			VAL(avg_data_x) = 50000;
			//			VAL(avg_data_y) = 400.0 - 350;
			cal_avg_data();
		} else 
			show_info(p_vmem, WARNING_GATE);
	} else 
		VAL(avg_status) = !VAL(avg_status);

	if (VAL(ver_pos)[1][1] == 2) {
		show_vbmpmem(p_vmem, bmp_content + 
				(2 + VAL(avg_status)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2, X1, Y5 + 50);
	} else {
		show_vbmpmem(p_vmem,bmp_content + 
				(2 + VAL(avg_status)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2 + 1 * V_SIZE, X1, Y5 + 50);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(avg_status))) 
		STORE_DATA(AVG_STATUS, sizeof(VAL(avg_status)))
	else if (3 == a) {
		STORE_DATA(AVG_DATA_X, sizeof(VAL(avg_data_x)))
			STORE_DATA(AVG_DATA_Y, sizeof(VAL(avg_data_y)))
	}
}

static void t27(COLOR_TYPE *p_vmem,int a)/*空*/
{
	if (VAL(ver_pos)[1][1] == 3) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y6 + 50);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE*117, X1, Y6 + 50);
	}
}

static void t28(COLOR_TYPE *p_vmem,int a)/*自动增益*/
{
	int i;
	temp_db = 0;
	if (a == 0) ;
	else if ( a == 1) {
		t28(p_vmem, 3);
		t28(p_vmem, 3);
		t28(p_vmem, 3);
	} else if (VAL(alogic) == 0)
		show_info(p_vmem,WARNING_GATE);
	else if((3 == a) && (pt->measure_cur_data->Sa != 0) && 
			(pt->current_data[pt->a_gata_max] < 380) &&
			(pt->current_data[pt->a_gata_max] > 20)) {
		temp_db = 	10 * 20 * log10((float)((VAL(auto_db_h)) * 4.0) / \
				(400 - pt->current_data[pt->a_gata_max]));

		if (temp_db) {
			temp_db = temp_db + VAL(db);
			if (temp_db < 1100 && temp_db > 0) {
				change_db(p_vmem, 3);
				//			write_db_val(temp_db, VAL(db));
				//			VAL(db) =  temp_db;
			}
			for(i=0;i<32;i++)
				fetch_data();
			draw_agat(pt->true_video_add,0);
		}
	}

	if (VAL(ver_pos)[1][2] == 0)  {
		show_vbmpmem(p_vmem, bmp_content + 10 * V_SIZE * 4, X1, Y0 + 50);
	} else {
		show_vbmpmem(p_vmem, bmp_content + 10 * V_SIZE * 4 + V_SIZE, X1, Y0 + 50);
	}
}

static void t29(COLOR_TYPE *p_vmem,int a)/*自动增益高度*/
{
	char temp[12], *ss="% ", temp_value = VAL(auto_db_h);
	if (a == 0 || a == 3) ;
	else if (VAL(alogic) == 0)
		show_info(p_vmem,WARNING_GATE);
	else if (a == 1) {
		if (VAL(auto_db_h) < 80)
			VAL(auto_db_h)++;
		else
			VAL(auto_db_h) = 20;
	} else if (a == 2) {
		if (VAL(auto_db_h) > 20)
			VAL(auto_db_h)--;
		else
			VAL(auto_db_h) = 80;
	}

	memset(temp, 0, sizeof(temp));
	sprintf(temp, "%d%s" , VAL(auto_db_h), ss);
	if (VAL(ver_pos)[1][2] == 1) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y4 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, 155, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, Y4 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, 155, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(auto_db_h)))
		STORE_DATA(AUTO_DB_H, sizeof(VAL(auto_db_h)))
}


static void t30(COLOR_TYPE *p_vmem,int a)/*曲面修正*/
{
	char temp[12];
	unsigned char *p;
	int temp_value=VAL(diamet);
	if (a == 0) ;
	else if (VAL(frez)) 
		show_info(p_vmem,WARNING_FREZ);
	else if (a == 3) {
		if(VAL(diamet_reg) < 3) VAL(diamet_reg)++;
		else	VAL(diamet_reg) = 0;
	} else VAL(diamet) = value_change(temp_value, diamet_range,
			VAL(diamet_reg) ? diamet_step1: diamet_step0,
			diamet_range[sizeof(diamet_range) / sizeof(int) - 1],
			diamet_range[0], a, VAL(diamet_reg));

	VAL(diamet) = VAL(diamet) < VAL(thickne) * 2 ? temp_value : VAL(diamet);//限制代码
	memset(temp,0,sizeof(temp));
	cal_str_display(p_vmem, temp, DISTANCE, 
			VAL(diamet), VAL(diamet_reg), VAL(ver_pos)[1][2] == 2, X1, Y5 + 50);

	if (2001000 == VAL(diamet)) {
		p = bmp_content + 26 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
		if (VAL(ver_pos)[1][2] == 2) {
			show_vbmpmem(p_vmem, p, X1, Y5 + 50);
		} else {
			show_vbmpmem(p_vmem, p + V_SIZE, X1, Y5 + 50);
		}
	}

	if (((a == 1) || (a == 2)) && (temp_value != VAL(diamet)))
		STORE_DATA(DIAMET, sizeof(VAL(diamet)))
	else if (a == 3)
		STORE_DATA(DIAMET_REG, sizeof(VAL(diamet_reg)))
}

static void t31(COLOR_TYPE *p_vmem,int a)/*空*/
{
	if (VAL(ver_pos)[1][2] == 3) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y6 + 50);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE*117, X1, Y6 + 50);
	}
}



static void t32(COLOR_TYPE *p_vmem,int a)/* 坡口类型 */
{
	unsigned char *p;
	int	old_hanfeng = pt->hanfeng;
	if (a == 0) ;
	else if (3 == a)
		(pt->hanfeng >= 2) ? (pt->hanfeng = 0) : pt->hanfeng++;
	else if ((a == 1) && pt->hanfeng < 2 ) 
		pt->hanfeng++;
	else if (a == 2 && pt->hanfeng > 0)
		pt->hanfeng--;
	if (pt->hanfeng == 0)
		p = bmp_content + 2 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else if (pt->hanfeng == 1)
		p = bmp_content + 24 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else p = bmp_content + 25 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;

	if (VAL(ver_pos)[1][3] == 0) {
		show_vbmpmem(p_vmem, p, X1, 50);
	} else {
		show_vbmpmem(p_vmem, p + V_SIZE, X1, 50);
	}
	if (old_hanfeng != pt->hanfeng) {
		draw_rim(pt->virtual_video_add,0x0);
		draw_horizontal_text1(pt->virtual_video_add);
		draw_vertical_text1(pt->virtual_video_add);
	}

}

//int probe_pos_range[] = {1,500};
//int probe_pos_step[] = {1};
static void t33(COLOR_TYPE *p_vmem,int a)/* 探头位置*/
{
	char temp[12];
	int temp_value = VAL(probe_pos);
	float temp_val;
	if (a==0) ;
	else if (3 == a) {
		VAL(probe_pos_reg) < 3 ? VAL(probe_pos_reg)++ : (VAL(probe_pos_reg) = 1);
	} else VAL(probe_pos) = value_change(temp_value, probe_pos_range, probe_pos_step,
			probe_pos_range[sizeof(probe_pos_range) / sizeof(int) - 1],
			probe_pos_range[0], a ,VAL(probe_pos_reg));
	memset(temp, 0, sizeof(temp));
	if (VAL(unit) == 1) {/* 0为inch 1为mm */
/*		temp_val = ((float)(VAL(x_value))) / 1000;*/
		if(VAL(probe_pos_reg) == 1)
			sprintf(temp, "\052%dmm", VAL(probe_pos));
		else if(VAL(probe_pos_reg) == 2) 
			sprintf(temp, "\053%dmm", VAL(probe_pos));
		else
			sprintf(temp, "\054%dmm", VAL(probe_pos));
	} else {
		temp_val = (float)VAL(probe_pos) / 25.4;
		if (VAL(probe_pos_reg) == 1) 
			sprintf(temp, "\052%.2fIN",  temp_val);
		else if (VAL(probe_pos_reg) == 2)
			sprintf(temp, "\053%.2fIN",  temp_val);
		else 
			sprintf(temp, "\054%.2fIN",  temp_val);
	}
	if (VAL(ver_pos)[1][3] == 1) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y4 + 50);
		draw_str(p_vmem, temp, X1 + 5, Y4 + 55, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, Y4 + 50);
		draw_str(p_vmem, temp, X1 + 5, Y4 + 55, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(probe_pos))) 
		STORE_DATA(PROBE_POS, sizeof(VAL(probe_pos)))
	else if (a == 3) 
		STORE_DATA(PROBE_POS_REG, sizeof(VAL(probe_pos_reg)))

}

static void t34(COLOR_TYPE *p_vmem,int a)/*焊缝位置*/
{
	char temp[12];
	int temp_value = VAL(hanfeng_width);
	float temp_val;
	if (a==0) ;
	else if (3 == a) {
		VAL(hanfeng_w_reg) < 2 ? VAL(hanfeng_w_reg)++ : (VAL(hanfeng_w_reg) = 1);
	} else VAL(hanfeng_width) = value_change(temp_value, hanfeng_w_range, hanfeng_w_step,
			hanfeng_w_range[sizeof(hanfeng_w_range) / sizeof(int) - 1],
			hanfeng_w_range[0], a ,VAL(hanfeng_w_reg));
	memset(temp, 0, sizeof(temp));
	if (VAL(unit) == 1) {/* 0为inch 1为mm */
/*		temp_val = ((float)(VAL(x_value))) / 1000;*/
		if(VAL(hanfeng_w_reg) == 1)
			sprintf(temp, "\052%dmm", VAL(hanfeng_width));
		else if(VAL(hanfeng_w_reg) == 2) 
			sprintf(temp, "\053%dmm", VAL(hanfeng_width));
		/**
		else
			sprintf(temp, "\054%dmm", VAL(probe_pos));**/
	} else {
		temp_val = (float)VAL(hanfeng_width) / 25.4;
		if (VAL(hanfeng_w_reg) == 1) 
			sprintf(temp, "\052%.2fIN",  temp_val);
		else if (VAL(hanfeng_w_reg == 2))
			sprintf(temp, "\053%.2fIN",  temp_val);
		/*
		else 
			sprintf(temp, "\054%.2fIN",  temp_val);**/
	}
	if (VAL(ver_pos)[1][3] == 2) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y5 + 50);
		draw_str(p_vmem, temp, X1 + 5, Y5 + 55, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, Y5 + 50);
		draw_str(p_vmem, temp, X1 + 5, Y5 + 55, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(probe_pos))) 
		STORE_DATA(HANFENG_W, sizeof(VAL(hanfeng_width)))
	else if (a == 3) 
		STORE_DATA(HANFENG_W_REG, sizeof(VAL(hanfeng_w_reg)))

}

void draw_hanfeng()
{
	int width, leg, sa_x, sa_y, l, l1, sa, x_tmp, length, i;/*l1 前沿折算值 sa 声程折算值*/
	int temp_h1, temp_h2;
	float temp_l;
/**	pt->hanfeng_refresh = 1;**/

	draw_rim(pt->virtual_video_add,0x0);
	draw_horizontal_text1(pt->virtual_video_add);
	draw_vertical_text1(pt->virtual_video_add);

	width = (VAL(hanfeng_width) / (VAL(thickne) / 1000.0)) * HF_NUM;
	l = (VAL(probe_pos) / (VAL(thickne) / 1000.0)) * HF_NUM;
	l1 = (((float)VAL(x_value) / (VAL(thickne)))) * HF_NUM;
	x_tmp = HF_NUM * SIN(VAL(angle)) / COS(VAL(angle));
							   //xr[i]=sin((i+256)*3.1415826/5)*128;

	if ((VAL(angle) == 0) || !pt->hanfeng)
		return;

	sa = (((float)pt->measure_cur_data->Sa / VAL(thickne))) * HF_NUM;
	sa_y = sa * (float)(COS(VAL(angle)) / 10000.0);
	sa_x = sa * (float)(SIN(VAL(angle)) / 10000.0);

/*	leg = sa_y / 100;*/
	leg = 0;

	length = width / 2 + l + l1;

	length = sa_x;

	if (sa_x <= 0 || sa_x > (width / 2 + l + l1 + x_tmp))
		return ;

	fbline(pt->virtual_video_add, 0, HF_Y, 400, HF_Y, WAVE_GROUND);
	fbline(pt->virtual_video_add, 0, HF_Y + HF_NUM, 400, HF_Y + HF_NUM, WAVE_GROUND);

	if ((width < 300) && (pt->hanfeng == 1)) {
		fbline(pt->virtual_video_add, 50, HF_Y, (50 + width / 2), HF_Y + HF_NUM, WAVE_GROUND);/*2根直线*/
		fbline(pt->virtual_video_add, (50 + width / 2), HF_Y + HF_NUM, (50 + width), HF_Y, WAVE_GROUND);
		fbline(pt->virtual_video_add, 50, HF_Y, 50 + width, HF_Y, WAVE_BACK);
		temp_l = sqrt(50 * 50 + width * width /4);
		/*圆弧*/
		for (i = width /2; i > 0; i--) {
			temp_h1 = sqrt(temp_l * temp_l - i * i );
			temp_h2 = sqrt(temp_l * temp_l - (i - 1) * (i - 1));
			fbline(pt->virtual_video_add, 50 + width / 2 - i, HF_Y - (temp_h1 - HF_NUM), 
					50 + width / 2 - (i - 1), HF_Y - (temp_h2 -HF_NUM), WAVE_GROUND);
			fbline(pt->virtual_video_add, 50 + width / 2 + i, HF_Y - (temp_h1 - HF_NUM),
					50 + width / 2 + (i - 1), HF_Y -(temp_h2 - HF_NUM),WAVE_GROUND);

		}
	} else if ((width < 300) && (pt->hanfeng == 2)) {
		fbline(pt->virtual_video_add, 50, HF_Y, 50 + width, HF_Y + HF_NUM, WAVE_GROUND);/*2根直线*/
		fbline(pt->virtual_video_add, 50, HF_Y + HF_NUM, 50 + width, HF_Y, WAVE_GROUND);
		fbline(pt->virtual_video_add, 50, HF_Y, 50 + width, HF_Y, WAVE_BACK);
		fbline(pt->virtual_video_add, 50, HF_Y + HF_NUM, 50 + width, HF_Y + HF_NUM, WAVE_BACK);
		fbline(pt->virtual_video_add, 50 + width, HF_Y + HF_NUM, 50 + width, HF_Y + HF_NUM, WAVE_BACK);
		temp_l = sqrt(50 * 50 + width * width /4);
		/*圆弧*/
		for (i = width / 2; i > 0; i--) {
			temp_h1 = sqrt(temp_l * temp_l - i * i );
			temp_h2 = sqrt(temp_l * temp_l - (i - 1) * (i - 1));
			fbline(pt->virtual_video_add, 50 + width / 2 - i, HF_Y - (temp_h1 - HF_NUM), 
					50 + width / 2 - (i - 1), HF_Y - (temp_h2 -HF_NUM), WAVE_GROUND);
			fbline(pt->virtual_video_add, 50 + width / 2 + i, HF_Y - (temp_h1 - HF_NUM),
					50 + width / 2 + (i - 1), HF_Y - (temp_h2 - HF_NUM),WAVE_GROUND);
			fbline(pt->virtual_video_add, 50 + width / 2 - i, HF_Y + temp_h1, 
					50 + width / 2 - (i - 1), HF_Y + temp_h2, WAVE_GROUND);
			fbline(pt->virtual_video_add, 50 + width / 2 + i, HF_Y + temp_h1,
					50 + width / 2 + (i - 1), HF_Y + temp_h2 ,WAVE_GROUND);

		}
	} else return ;

	/*探头*/
	if ((width / 2  + l)	< 300) {
		fbline(pt->virtual_video_add, 50 + width / 2 + l, HF_Y - 30, 
				50 + width / 2 + l, HF_Y, WAVE_GROUND);
		fbline(pt->virtual_video_add, 50 + width / 2 + l, HF_Y - 30,
				70 + width / 2 + l, HF_Y - 30, WAVE_GROUND);
		fbline(pt->virtual_video_add, 70 + width / 2 + l, HF_Y - 30,
				80 + width / 2 + l, HF_Y - 20, WAVE_GROUND);
		fbline(pt->virtual_video_add, 80 + width / 2 + l, HF_Y - 20,
				80 + width / 2 + l, HF_Y, WAVE_GROUND);
		int xw = 75 + width / 2 + l, yw = HF_Y - 25; /*探头尾巴坐标*/
		fbdot(pt->virtual_video_add, xw + 1, yw, WAVE_GROUND);
		fbline(pt->virtual_video_add, xw + 2, yw, xw + 10, yw - 8, WAVE_GROUND);
		fbline(pt->virtual_video_add, xw + 10, yw - 9, xw + 11, yw - 10, WAVE_GROUND);
		fbline(pt->virtual_video_add, xw + 11, yw - 10, xw + 11, yw - 13, WAVE_GROUND);
		fbline(pt->virtual_video_add, xw + 11, yw - 14, xw + 9, yw - 16,WAVE_GROUND);
		fbline(pt->virtual_video_add, xw + 9, yw - 16, xw + 5, yw - 16, WAVE_GROUND);
		fbline(pt->virtual_video_add, xw + 5, yw - 16, xw + 3, yw - 14, WAVE_GROUND);
		fbline(pt->virtual_video_add, xw + 3, yw - 14, xw + 3, yw - 10, WAVE_GROUND);
		fbline(pt->virtual_video_add, xw + 3, yw - 10, xw + 9, yw - 4, WAVE_GROUND);
		fbline(pt->virtual_video_add, xw + 9, yw - 4, xw + 12, yw - 4, WAVE_GROUND);
		fbline(pt->virtual_video_add, xw + 12, yw - 4, xw + 20, yw - 12, WAVE_GROUND);
	} else return;

	/*波形路线*/
	while( length > x_tmp) {
		if ((leg & 0x01) == 0)
			fbline(pt->virtual_video_add, 50 + width / 2 + l + l1 - x_tmp * leg, HF_Y,
					50 + width / 2 + l + l1 - x_tmp * (leg + 1), HF_Y + HF_NUM, COL21);
		else
			fbline(pt->virtual_video_add, 50 + width / 2 + l + l1 - x_tmp * leg, HF_Y + HF_NUM,
					50 + width / 2 + l + l1 - x_tmp * (leg + 1), HF_Y, COL21);

		leg++;
		length -= x_tmp;
	}

	if ((leg & 0x01 ) == 0) {
		fbline(pt->virtual_video_add, 50 + width / 2 + l + l1 - x_tmp * leg, HF_Y,
				50 + width / 2 + l + l1 - sa_x, HF_Y + (sa_y - HF_NUM * leg), COL21);
		fbrec(pt->virtual_video_add,
				50 + width / 2 + l + l1 - sa_x, HF_Y + (sa_y - HF_NUM * leg), 2, 3, COL21);
	} else {
		fbline(pt->virtual_video_add, 50 + width / 2 + l + l1 - x_tmp * leg, HF_Y + HF_NUM,
				50 + width / 2 + l + l1 - sa_x, HF_Y + HF_NUM - (sa_y - HF_NUM * leg), COL21);
		fbrec(pt->virtual_video_add,
				50 + width / 2 + l + l1 - sa_x, HF_Y + HF_NUM - (sa_y -HF_NUM * leg), 2, 3, COL21);
	}

	return ;
}

static void t35(COLOR_TYPE *p_vmem,int a)/*刷新*/
{

	if (a) 
		draw_hanfeng();

	if (VAL(ver_pos)[1][3] == 3)  {
		show_vbmpmem(p_vmem, bmp_content + 10 * V_SIZE * 4, X1, Y6 + 50);
	} else {
		show_vbmpmem(p_vmem, bmp_content + 10 * V_SIZE * 4 + V_SIZE, X1, Y6 + 50);
	}

}


static void t36(COLOR_TYPE *p_vmem,int a){}/**/
static void t37(COLOR_TYPE *p_vmem,int a){}/**/
static void t38(COLOR_TYPE *p_vmem,int a){}/**/
static void t39(COLOR_TYPE *p_vmem,int a){}/**/

static void t40(COLOR_TYPE *p_vmem,int a)/* 通道编号 数据 编号*/
{
	if (a == 3) {
		pt->store_mode = !pt->store_mode;
		draw_rim(pt->virtual_video_add,0x0);
		draw_horizontal_text1(pt->virtual_video_add);
		draw_vertical_text1(pt->virtual_video_add);
	}
	if(!pt->store_mode) {
		char temp[12], temp_value = VAL(set);
		if (a==1)
			(VAL(set) < CONFIG_QUA) ? (VAL(set)++) : (VAL(set) = 30);
		else if (a==2)
			(VAL(set) > 1) ? (VAL(set)--) : (VAL(set) = 1);
		memset(temp,0,sizeof(temp));
		if (test_file()) 
			sprintf(temp, "*%d", VAL(set));
		else
			sprintf(temp, "%d", VAL(set));
		if (VAL(ver_pos)[2][0] == 0) {
			show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, 50);
			draw_str(p_vmem, temp, X1 + 1 + 3, 55, 2, 3, FONT_COL1);
		} else {
			show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, 50);
			draw_str(p_vmem, temp, X1 + 1 + 3, 55, 2, 3, FONT_COL2);
		}
		if (((a == 1) || (a == 2)) && (temp_value != VAL(set)))
			STORE_DATA(SET, (sizeof(VAL(set))))
	} else {
		char temp[12], temp_value = VAL(job);
		if ((a == 1) && (VAL(job) < 200))
			VAL(job)++;
		else if ((a == 2) && (VAL(job) > 1))
			VAL(job)--;
		memset(temp,0,sizeof(temp));
		if(test_file1()) 
			sprintf(temp, "*%d", VAL(job));
		else
			sprintf(temp, "%d", VAL(job));
		if (VAL(ver_pos)[2][0] == 0) {
			show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, 50);
			draw_str(p_vmem, temp, X1 + 1 + 3, 55, 2, 3, FONT_COL1);
		} else {
			show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, 50);
			draw_str(p_vmem, temp, X1 + 1 + 3, 55, 2, 3, FONT_COL2);
		}
		if (((a == 1) || (a == 2)) && (temp_value != VAL(job)))
			STORE_DATA(JOB, sizeof(VAL(job)))
	}
}

static void t41(COLOR_TYPE *p_vmem,int a)/*读取 储存的信息*/
{
	int val;
	if (a == 0 || a == 3|| VAL(frez)) ;
	else if (1 == a && ((test_file() && !pt->store_mode) || (test_file1() && pt->store_mode))) {
		if(!pt->store_mode)	read_config();
		else read_config1();
		change_col();
		if (VAL(lang) == 0) bmp = bmp1;
		else bmp = bmp2;
		switch (VAL(damping)) {
			case 0:
				ioctl(pt->fd_gpio, DAMPING_1, &val);
				break;
			case 1:
				ioctl(pt->fd_gpio, DAMPING_2, &val);
				break;
			case 2:
				ioctl(pt->fd_gpio, DAMPING_3, &val);
				break;
			case 3:
				ioctl(pt->fd_gpio, DAMPING_4, &val);
				break;
			default:
				break;
		}

		write_db_val(VAL(db), 1200);
		if (VAL(dual) == 1) 
			ioctl(pt->fd_gpio, DUAL_ON, &val);
		else
			ioctl(pt->fd_gpio, DUAL_OFF, &val);
		if (VAL(power) == 1) 
			*(pt->trans_data + 1024*2 + 4*2) = 0x01;
		else 
			*(pt->trans_data + 1024*2 + 4*2) = 0x00;
		switch (VAL(light)) {
			case 2:
				write(pt->fd_key, key_light_high, 1);
				break;
			case 1:
				write(pt->fd_key, key_light_mid, 1);
				break;
			case 0:
				write(pt->fd_key, key_light_low, 1);
				break;
			default:
				break;
		}
		if(pt->store_mode)	VAL(frez) = 1;
		memset(pt->virtual_video_add, WAVE_BACK, 0x4B000);
		draw_rim(pt->virtual_video_add, 0x0);
		draw_horizontal_text1(pt->virtual_video_add);
		draw_vertical_text1(pt->virtual_video_add);
		if (VAL(dacecho) && VAL(dacmode))	cal_dac_data1();
		i2c_add[0] = 0;
		i2c_add[1] = 0;
		if(write_i2c(pt->fd_config, i2c_add, pt->p_config, 32)) ;i2c_add[1]=32;
		if(write_i2c(pt->fd_config, i2c_add, pt->p_config+i2c_add[1], 32)) ; i2c_add[1]=64;
		if(write_i2c(pt->fd_config, i2c_add, pt->p_config+i2c_add[1], 32)) ; i2c_add[1]=96;
		if(write_i2c(pt->fd_config, i2c_add, pt->p_config+i2c_add[1], 32)) ; i2c_add[1]=128;
		if(write_i2c(pt->fd_config, i2c_add, pt->p_config+i2c_add[1], 32)) ; i2c_add[1]=160;
		if(write_i2c(pt->fd_config, i2c_add, pt->p_config+i2c_add[1], 32)) ; i2c_add[1]=192;
		if(write_i2c(pt->fd_config, i2c_add, pt->p_config+i2c_add[1], 32)) ; i2c_add[1]=0;
		senddsp_data();
	}

	if (VAL(ver_pos)[2][0] == 1) {
		show_vbmpmem(p_vmem, bmp_content + 10 * V_SIZE * 4, X1, 150);
	} else {
		show_vbmpmem(p_vmem, bmp_content + 10 * V_SIZE * 4 + V_SIZE, X1, 150);
	}
}

static void t42(COLOR_TYPE *p_vmem,int a)/*储存信息*/
{
	if (a == 0 || a == 3) ;
	else if (1 == a && ((!test_file() && !pt->store_mode) || (!test_file1() && pt->store_mode))) {
		if (!pt->store_mode) store_config();
		else store_config1();
	}
	if (VAL(ver_pos)[2][0] == 2) {
		show_vbmpmem(p_vmem, bmp_content + 10 * V_SIZE * 4 + 2 * V_SIZE, X1, 250);
	} else {
		show_vbmpmem(p_vmem, bmp_content + 10 * V_SIZE * 4 + 3 * V_SIZE, X1, 250);
	}
	if (a) draw_vertical_text1(pt->virtual_video_add);
}

static void t43(COLOR_TYPE *p_vmem,int a)/*删除信息*/
{
	if(a==0||a==3) ;
	else if (1 == a && ((test_file() && !pt->store_mode) || (test_file1() && pt->store_mode))) {
		if (!pt->store_mode) delete_config();
		else delete_config1();
	}
	if (VAL(ver_pos)[2][0] == 3) {
		show_vbmpmem(p_vmem, bmp_content + 10 * V_SIZE * 4 + 4 * V_SIZE, X1, 350);
	} else {
		show_vbmpmem(p_vmem, bmp_content + 10 * V_SIZE * 4 + 5 * V_SIZE, X1, 350);
	}
	if (a) draw_vertical_text1(pt->virtual_video_add);
}



static void t44(COLOR_TYPE *p_vmem,int a)/*记录速度 多少帧*/
{
	char temp[12];
	if (a == 0 || a == 3) ;
	else if((a==1) && pt->fps < 15)
		pt->fps++;
	else if((a==2) && pt->fps > 5)
		pt->fps--;
	memset(temp,0,sizeof(temp));
	sprintf(temp, "%d fps", pt->fps);
	if (VAL(ver_pos)[2][1] == 0) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, 55, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, 55, 2, 3, FONT_COL2);
	}
}

void *store_data(void *arg)
{
	while( pt->start_pos && pt->count < 3000) {
		memcpy(pt->save_data_p + 480 * pt->count, pt->current_data, 960);
		Sleep(0,1000000/pt->fps);
		pt->count++;
	}
	*pt->save_data_p = pt->fps;
	pt->count = 0;
	return (void *)(NULL);
}

static void t45(COLOR_TYPE *p_vmem,int a)/*开始 记录*/
{
	char temp[12];
	if (a == 0 || a == 1 || a == 2) ;
	else if(a == 3)
		pt->start_pos = !pt->start_pos;
	if (pt->start_pos == 1 && pt->count == 0) {
			pthread_t thread_id;
			pthread_create(&thread_id,NULL,&store_data,&a);
	}
	memset(temp,0,sizeof(temp));
	if (pt->start_pos == 0)
		sprintf(temp, "STOPPED");
	else 
		sprintf(temp, "RECORDING");
	if (VAL(ver_pos)[2][1] == 1) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, 150);
		draw_str(p_vmem, temp, X1 + 1 + 3, 155, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, 150);
		draw_str(p_vmem, temp, X1 + 1 + 3, 155, 2, 3, FONT_COL2);
	}
}

static void t46(COLOR_TYPE *p_vmem,int a)/*保存记录在u盘上面*/
{
	if (a == 0) ;
	else if (a == 1 || 3 == a) {
		char temp[52];
		int fd_usb_storage;
		int fd_usb;
		fd_usb_storage = open(SD_DEVICE, O_RDWR);
		if (fd_usb_storage < 0) {
			show_info(p_vmem, WARNING_USTORE_ERROR);
			return ;
		} else close(fd_usb_storage);
		sprintf(temp, "mount -t vfat /dev/sda1 usbkey");
		system(temp);
		fd_usb = open("/usbkey/record",O_CREAT|O_RDWR);
		if(fd_usb<0) { 
			show_info(p_vmem,WARNING_USTORE_ERROR);
			close(fd_usb);
			return ;
		} else {
			write(fd_usb, pt->save_data_p, 3*1024*1024);
		}
		close(fd_usb);
		sprintf(temp, "umount usbkey");
		system(temp);
		printf("复制ok\n");
		show_info(p_vmem, WARNING_COPY_OK);
	}
	if (VAL(ver_pos)[2][1] == 2) {
		show_vbmpmem(p_vmem, bmp_content + 10 * V_SIZE * 4 + 2 * V_SIZE, X1, 250);
	} else {
		show_vbmpmem(p_vmem, bmp_content + 10 * V_SIZE * 4 + 3 * V_SIZE, X1, 250);
	}
}

void *read_data(void *arg)
{
	VAL(frez) = 1;
	pt->fps = *pt->save_data_p;
	while( pt->play_mark && pt->count < 3000) {
		memcpy( pt->current_data, pt->save_data_p + 480 * pt->count,960);
		Sleep(0,1000000/pt->fps);
		pt->count++;
	}
	pt->count = 0;
	VAL(frez) = 0;
	return (void *)(NULL);
}

static void t47(COLOR_TYPE *p_vmem,int a)/*回放*/
{
	char temp[12];
	if (a == 0 || a == 1 || a == 2) ;
	else if(a==3)
		pt->play_mark = !pt->play_mark;
	if (pt->play_mark == 1 && pt->count == 0) {
			pthread_t thread_id;
			pthread_create(&thread_id,NULL,&read_data,&a);
	} else
		pt->play_mark = 0;
	memset(temp,0,sizeof(temp));
	if (pt->play_mark == 0)
		sprintf(temp, "STOPPED");
	else 
		sprintf(temp, "REPLAYING");
	if (VAL(ver_pos)[2][1] == 3) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, 350);
		draw_str(p_vmem, temp, X1 + 1 + 3, 355, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, 350);
		draw_str(p_vmem, temp, X1 + 1 + 3, 355, 2, 3, FONT_COL2);
	}
}

static void t48(COLOR_TYPE *p_vmem,int a)/*闸门B逻辑*/
{
	unsigned char *p, temp_value = VAL(blogic);
	if (a == 0 || a == 3) ;
	else if (VAL(rectify) == 3 )
		show_info(p_vmem,WARNING_RF);
	else if ((a == 1) && (VAL(blogic) < 2))
		VAL(blogic)++;
	else if ((a == 2) && (VAL(blogic) > 0))
		VAL(blogic)--;
	if (VAL(blogic) == 0) 
		p = bmp_content + 2 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else if (VAL(blogic) == 1) p = bmp_content + 8 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else p = bmp_content + 9 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;

	if (VAL(ver_pos)[2][2] == 0) {
		show_vbmpmem(p_vmem, p, X1, 50);
	} else {
		show_vbmpmem(p_vmem, p + V_SIZE, X1, 50);
	}
	if(((a == 1) || (a == 2)) && (temp_value != VAL(blogic))) {
		bgatopen = VAL(blogic);
		STORE_DATA(B_LOGIC, (sizeof(VAL(blogic))))
	}
}

static void t49(COLOR_TYPE *p_vmem,int a)/*闸门B起点*/
{
	char temp[12];
	gat_start_range[0] = 0; gat_start_step0[0] = 0;
	int temp_value=VAL(bstart);
	if (a == 0) ;
	else if (VAL(blogic) == 0)
		show_info(p_vmem, WARNING_GATE);
	else if (a == 3) {
		if(VAL(bstart_reg) < 3) VAL(bstart_reg)++;
		else	VAL(bstart_reg)=0;
	} else VAL(bstart) = value_change(temp_value, gat_start_range,
			VAL(bstart_reg) ? gat_start_step1 : gat_start_step0,
			gat_start_range[sizeof(gat_start_range) / sizeof(int) - 1],
			gat_start_range[0], a, VAL(bstart_reg));
	memset(temp, 0, sizeof(temp));
	cal_str_display(p_vmem, temp, DISTANCE, VAL(bstart), 
			VAL(bstart_reg), VAL(ver_pos)[2][2] == 1, X1,Y4 + 50);
	if (((a == 1) || (a == 2)) && (temp_value != VAL(bstart))) {
		STORE_DATA(B_START, sizeof(VAL(bstart)))
			senddsp_data();
		pt->agat_y = 512, pt->agat_x = 0;
	} else if (a == 3)
		STORE_DATA(BSTART_REG, sizeof(VAL(bstart_reg)))
}

static void t50(COLOR_TYPE *p_vmem,int a)/*闸门B宽度*/
{
	char temp[12];
	gat_start_range[0] = 20; gat_start_step0[0] = 20;
	int temp_value=VAL(bwidth);
	if (a == 0) ;
	else if (VAL(blogic) == 0) 
		show_info(p_vmem, WARNING_GATE);
	else if (a == 3) {
		if (VAL(bwidth_reg) < 3) VAL(bwidth_reg)++;
		else VAL(bwidth_reg) = 0;
	} else VAL(bwidth) = value_change(temp_value, gat_start_range,
			VAL(bwidth_reg) ? gat_start_step1 : gat_start_step0,
			gat_start_range[sizeof(gat_start_range) / sizeof(int) - 1],
			gat_start_range[0], a, VAL(bwidth_reg));
	memset(temp, 0, sizeof(temp));
	cal_str_display(p_vmem, temp, DISTANCE, VAL(bwidth), 
			VAL(bwidth_reg), VAL(ver_pos)[2][2] == 2, X1, 250);
	if (((a == 1) || (a == 2)) && (temp_value != VAL(bwidth))) {
		STORE_DATA(B_WIDTH, sizeof(VAL(bwidth)))
			senddsp_data();
	} else if (a == 3) 
		STORE_DATA(BWIDTH_REG, sizeof(VAL(bwidth_reg)))
}

static void t51(COLOR_TYPE *p_vmem,int a)/*闸门B高度*/
{
	char temp[12], *ss="% ", temp_value = VAL(bthrsh);
	if (a == 0 || a == 3) ;
	else if (VAL(blogic) == 0)
		show_info(p_vmem,WARNING_GATE);
	else if ((a == 1) && (VAL(bthrsh) < 95))
		VAL(bthrsh)++;
	else if ((a == 2) && (VAL(bthrsh) > 5))
		VAL(bthrsh)--;

	memset(temp, 0, sizeof(temp));
	sprintf(temp, "%d%s" , VAL(bthrsh), ss);
	if(VAL(ver_pos)[2][2]==3){
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, 350);
		draw_str(p_vmem, temp, X1 + 1 + 3, 355, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, 350);
		draw_str(p_vmem, temp, X1 + 1 + 3, 355, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(athrsh)))
		STORE_DATA(A_THRSH, sizeof(VAL(athrsh)))
}

static void t52(COLOR_TYPE *p_vmem,int a)/*TOF*/
{
	unsigned char temp_value = VAL(tof), *p;
	if (a == 0) ;
	else if (3 == a)
		VAL(tof) = !VAL(tof);
	else if (a == 1) 
		VAL(tof) = 1;
	else if (a == 2)
		VAL(tof) = 0;
	if (VAL(tof) == 0)
		p = bmp_content + 13 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else  
		p = bmp_content + 12 * V_SIZE * 4 + VAL(lang) * V_SIZE  *2;
	if (VAL(ver_pos)[2][3] == 0) {
		show_vbmpmem(p_vmem, p, X1, 50);
	} else {
		show_vbmpmem(p_vmem, p + V_SIZE, X1, 50);
	}
	if (a && (temp_value != VAL(tof)))
		STORE_DATA(TOF, sizeof(VAL(tof)))
}

static void t53(COLOR_TYPE *p_vmem,int a) /*屏幕显示 检波*/
{
	char temp_value = VAL(rectify);
	unsigned long *tdata = (unsigned long *)pt->map_base;
	if (a == 0) ;
	else if (VAL(frez)) 
		show_info(p_vmem, WARNING_FREZ);
	else if (3 == a)
		(VAL(rectify) < 3) ? (VAL(rectify)++) : (VAL(rectify) = 0);
	else if (a == 1)
		(VAL(rectify) < 3) ? (VAL(rectify)++) : (VAL(rectify) = 3);
	else if(a == 2)
		(VAL(rectify) > 0) ? (VAL(rectify)--) : (VAL(rectify) = 0);
	if (test_val() != 1) 
		show_info(p_vmem, WARNING_RF1), VAL(rectify) = temp_value;
	if (VAL(rectify) == 3) 
		VAL(alogic) = 0, VAL(blogic) = 0;
	else 
		VAL(alogic) = agatopen, VAL(blogic) = bgatopen;
	if (VAL(ver_pos)[2][3] == 1) {
		show_vbmpmem(p_vmem, bmp_content + 
				(4 + VAL(rectify)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2, X1,Y4 + 50);
	} else {
		show_vbmpmem(p_vmem, bmp_content + 
				(4 + VAL(rectify)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2 + 1 * V_SIZE, X1, Y4 + 50);
	}
	if (a && (temp_value != VAL(rectify))) {
		STORE_DATA(RECTIFY, sizeof(VAL(rectify)))
			*(tdata + 512 * 2 + 3) = VAL(rectify);
		Sleep(0, 16000);
	}
}

static void t54(COLOR_TYPE *p_vmem,int a) /*放大*/
{
	unsigned char temp_val = VAL(magnify), *p = NULL;
	if (a == 0 || a == 3) ;
	else if ((a == 1) && (VAL(awidth) != 20) && (0 != VAL(alogic))) 
		VAL(magnify) = 1;
	else if (a == 2)
		VAL(magnify) = 0; 
	if(VAL(magnify) == 0)
		p = bmp_content + 2 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else if (VAL(magnify) == 1)
		p = bmp_content + 14 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	if (VAL(ver_pos)[2][3] == 2){
		show_vbmpmem(p_vmem, p, X1, 250);
	} else {
		show_vbmpmem(p_vmem, p + V_SIZE, X1, 250);
	}
	if ((VAL(magnify) != temp_val) && ((a == 1) || (a ==2 ))) {
		STORE_DATA(MAGNIFY, sizeof(VAL(magnify)))
			senddsp_data();
	}
}

static void t55(COLOR_TYPE *p_vmem,int a) /*A扫描*/
{
	unsigned char temp_value = VAL(a_scan);
	int i;
	if (a == 0) ;
	else if (3 == a)
		VAL(a_scan) = !VAL(a_scan);
	else if ((a == 1) && (VAL(a_scan) != 1))
		VAL(a_scan) = 1;
	else if ((a == 2) && (VAL(a_scan) == 1)){ 
		VAL(a_scan) = 0;
	}
	if (!VAL(a_scan)) {
		for (i = 0; i < 640; i++)
			envelope_data[i] = 399;
	}
	if (VAL(ver_pos)[2][3] == 3) {
		show_vbmpmem(p_vmem, bmp_content + 
				(2 + VAL(a_scan)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2, X1, Y6 + 50);
	} else {
		show_vbmpmem(p_vmem,bmp_content + 
				(2 + VAL(a_scan)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2 + 1 * V_SIZE, X1, Y6 + 50);
	}
	if (a && (temp_value != VAL(a_scan))) 
		STORE_DATA(A_SCAN, sizeof(VAL(a_scan)))
}

static void t56(COLOR_TYPE *p_vmem,int a){}/**/
static void t57(COLOR_TYPE *p_vmem,int a){}/**/
static void t58(COLOR_TYPE *p_vmem,int a){}/**/
static void t59(COLOR_TYPE *p_vmem,int a){}/**/

void set_time()/*保存 时间 日期*/
{
	int ret,fd;
	struct rtc_time rtc_tm;
	char temp[32];
	memset(temp,0,sizeof(temp));
	fd=open(RTC_DEVICE,O_RDWR);
	rtc_tm.tm_sec=time_c[2];
	rtc_tm.tm_min=time_c[1];
	rtc_tm.tm_hour=time_c[0];
	rtc_tm.tm_mday=date[2];
	rtc_tm.tm_mon=date[1]-1;
	rtc_tm.tm_year=date[0]-1900;
	rtc_tm.tm_wday=0;
	rtc_tm.tm_isdst=0;
	sprintf(temp,"date %02d%02d%02d%02d%04d",date[1],date[2],time_c[0],time_c[1],date[0]);
	ret=ioctl(fd,RTC_SET_TIME,&rtc_tm);
	system(temp);
	close(fd);
#if 0	
	int ret,fd;
	struct rtc_time rtc_tm;
	char temp[32];
	memset(temp,0,sizeof(temp));
	fd=open(RTC_DEVICE, O_RDWR);
	rtc_tm.tm_sec=pt->time_c[2];
	rtc_tm.tm_min=pt->time_c[1];
	rtc_tm.tm_hour=pt->time_c[0];
	rtc_tm.tm_mday=pt->date[2];
	rtc_tm.tm_mon=pt->date[1]-1;
	rtc_tm.tm_year=pt->date[0]-1900;
	rtc_tm.tm_wday=0;
	rtc_tm.tm_isdst=0;
	sprintf(temp,"date %02d%02d%02d%02d%04d",pt->date[1],pt->date[2],pt->time_c[0],pt->time_c[1],pt->date[0]);
	ret=ioctl(fd,RTC_SET_TIME,&rtc_tm);
	system(temp);
	close(fd);
#endif
}


static void t60(COLOR_TYPE *p_vmem,int a) /*测量点一*/
{ 
	char temp[12];
	unsigned char temp_value = VAL(meas1);
	if (a == 0 || a == 3) ;
	else if ((a == 1) && (VAL(meas1) < 20))
		VAL(meas1)++;
	else if ((a == 2) && (VAL(meas1) > 0))
		VAL(meas1)--;
	memset(temp, 0, sizeof(temp));
	sprintf(temp, "%s", display[VAL(meas1)]);
	if(VAL(ver_pos)[3][0] == 0) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, 55, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, 55, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(meas1)))
		STORE_DATA(MEAS1, sizeof(VAL(meas1)))
}

static void t61(COLOR_TYPE *p_vmem,int a)/*测量点二*/
{ 
	char temp[12];
	unsigned char temp_value = VAL(meas2);
	if (a == 0 || a == 3) ;
	else if ((a == 1) && (VAL(meas2) < 20))
		VAL(meas2)++;
	else if ((a == 2) && (VAL(meas2) > 0))
		VAL(meas2)--;
	memset(temp, 0, sizeof(temp));
	sprintf(temp, "%s", display[VAL(meas2)]);
	if(VAL(ver_pos)[3][0] == 1) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y4 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, Y4 + 55, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, Y4 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, Y4 + 55, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(meas2)))
		STORE_DATA(MEAS2, sizeof(VAL(meas2)))
}

static void t62(COLOR_TYPE *p_vmem,int a)/*测量点三*/
{ 
	char temp[12];
	unsigned char temp_value = VAL(meas3);
	if (a == 0 || a == 3) ;
	else if ((a == 1) && (VAL(meas3) < 20))
		VAL(meas3)++;
	else if ((a == 2) && (VAL(meas3) > 0))
		VAL(meas3)--;
	memset(temp, 0, sizeof(temp));
	sprintf(temp, "%s", display[VAL(meas3)]);
	if(VAL(ver_pos)[3][0] == 2) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y5 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, Y5 + 55, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, Y5 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, Y5 + 55, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(meas3)))
		STORE_DATA(MEAS3, sizeof(VAL(meas3)))
}

static void t63(COLOR_TYPE *p_vmem,int a)/*测量点四*/
{ 
	char temp[12];
	unsigned char temp_value = VAL(meas4);
	if (a == 0 || a == 3) ;
	else if ((a == 1) && (VAL(meas4) < 20))
		VAL(meas4)++;
	else if ((a == 2) && (VAL(meas4) > 0))
		VAL(meas4)--;
	memset(temp, 0, sizeof(temp));
	sprintf(temp, "%s", display[VAL(meas4)]);
	if(VAL(ver_pos)[3][0] == 3) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y6 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, Y6 + 55, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, Y6 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, Y6 + 55, 2, 3, FONT_COL2);
	}
	if (((a == 1) || (a == 2)) && (temp_value != VAL(meas4)))
		STORE_DATA(MEAS4, sizeof(VAL(meas4)))
}

static void t64(COLOR_TYPE *p_vmem,int a)/*填充*/
{
	char temp_value = VAL(filled);
	if (a == 0);
	else if (3 == a)
		VAL(filled) = !VAL(filled);
	else if ((a == 1) && (VAL(filled) != 1))
		VAL(filled) = 1;
	else if ((a == 2) && (VAL(filled) != 0))
		VAL(filled) = 0;
	if (VAL(ver_pos)[3][1] == 0) {
		show_vbmpmem(p_vmem, bmp_content + (2 + VAL(filled)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2, X1, 50);
	} else {
		show_vbmpmem(p_vmem, bmp_content + (2 + VAL(filled)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2 + V_SIZE, X1, 50);
	}
	if (a && (temp_value != VAL(filled)))
		STORE_DATA(FILLED, sizeof(VAL(filled)))
}

static void t65(COLOR_TYPE *p_vmem,int a)/*色彩*/
{
	char temp_value=VAL(scheme),temp[12];
	if (a == 0 || a == 3) ;
	else if ((a == 1) && VAL(scheme) < 5)
		VAL(scheme)++;
	else if ((a == 2) && (VAL(scheme) > 0))
		VAL(scheme)--;
	if (((a == 1) || (a == 2)) && (temp_value != VAL(scheme))) {
		change_col();
		draw_rim(pt->virtual_video_add, 0x0);
		draw_horizontal_text1(pt->virtual_video_add);
		draw_vertical_text1(pt->virtual_video_add);
	}
	memset(temp, 0, sizeof(temp));
	sprintf(temp, "%d", VAL(scheme) + 1);
	if (VAL(ver_pos)[3][1] == 1) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, Y4 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, 155, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, Y4 + 50);
		draw_str(p_vmem, temp, X1 + 1 + 3, 155, 2, 3, FONT_COL2);
	}
	if(((a == 1) || (a == 2)) && (temp_value != VAL(scheme)))
		STORE_DATA(SCHEME, sizeof(VAL(scheme)))
}

static void t66(COLOR_TYPE *p_vmem,int a)/*背光*/
{
	unsigned char temp_value=VAL(light), *p;
	if (a == 0) ;
	else if (3 == a)
		(VAL(light) < 2) ? VAL(light)++ : (VAL(light) = 0);
	else if ((a == 1) &&( VAL(light) < 2))
		VAL(light)++;
	else if ((a == 2) && (VAL(light) > 0))
		VAL(light)--;
	if (VAL(light) == 0)
		p = bmp_content + VAL(lang) * V_SIZE * 2;
	else if (VAL(light) == 1) 
		p = bmp_content + 16 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else 
		p = bmp_content + V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	if (VAL(ver_pos)[3][1] == 2) {
		show_vbmpmem(p_vmem, p, X1, Y5 + 50);
	} else {
		show_vbmpmem(p_vmem, p + V_SIZE, X1, Y5 + 50);
	}
	if (a && (temp_value != VAL(light))) {
		STORE_DATA(LIGHT, sizeof(VAL(light)))
			if (initialize_serial()!=0){
				perror("initialize_serial error");
				return ;
			}
		switch(VAL(light)){
			case 2:write(pt->fd_key,key_light_high,2);break;
			case 1:write(pt->fd_key,key_light_mid,2);break;
			case 0:write(pt->fd_key,key_light_low,2);break;
			default:break;
		}
		close(pt->fd_key);
	}
}

static void t67(COLOR_TYPE *p_vmem,int a)/*刻度*/
{
	unsigned char temp_value = VAL(scale);
	if (a == 0) ;
	if (3 == a) 
		(VAL(scale) == 1) ? (VAL(scale) = 2) : (VAL(scale) = 1);
	else if ((a == 1) && (VAL(scale) != 2))
		VAL(scale) = 2;
	else if ((a == 2) && (VAL(scale) != 1))
		VAL(scale) = 1;
	if (VAL(ver_pos)[3][1] == 3) {
		show_vbmpmem(p_vmem, bmp_content + (17 + VAL(scale)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2, X1, Y6 + 50);
	} else {
		show_vbmpmem(p_vmem, bmp_content + (17 + VAL(scale)) * V_SIZE * 4 + VAL(lang) * V_SIZE *2 + V_SIZE,X1, Y6 + 50);
	}
	if (a && (temp_value != VAL(scale)))
		STORE_DATA(SCALE, sizeof(VAL(scale)))
}


static void t68(COLOR_TYPE *p_vmem,int a)/*语言*/
{
	unsigned char temp_value=VAL(lang);
	if (a == 0) ;
	else {
		/*tandenghua*/
//		VAL(lang) = !VAL(lang);
		VAL(lang) = 1;
		if (VAL(lang) == 1) 
			bmp = bmp2;
		else 
			bmp = bmp1;
		show_hbmpmem(pt->virtual_video_add,bmp+V_SIZE, 513, 431);
		draw_horizontal_text1(pt->virtual_video_add);
		draw_vertical_text1(pt->virtual_video_add);			
	}	
	if (VAL(ver_pos)[3][2] == 0){
		show_vbmpmem(p_vmem, bmp_content + 20 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2, X1, 50);
	} else {
		show_vbmpmem(p_vmem, bmp_content + 20 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2 + V_SIZE, X1, 50);
	}
	if (a && (temp_value != VAL(lang)))
		STORE_DATA(LANG, sizeof(VAL(lang)))
}

static void t69(COLOR_TYPE *p_vmem,int a)/*打印选项*/
{
	unsigned char temp_value = VAL(copymode);
	if (a == 0) ;
	else if (3 == a) 
		VAL(copymode) = !VAL(copymode);
	else if ((a == 1) && (VAL(copymode) != 1))
		VAL(copymode) = 1;
	else if ((a==2) && (VAL(copymode) != 0))
		VAL(copymode) = 0;
	if(VAL(ver_pos)[3][2] == 1) {
		show_vbmpmem(p_vmem, bmp_content + (21 + VAL(copymode)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2, X1, Y4 + 50);
	}else{
		show_vbmpmem(p_vmem, bmp_content + (21 + VAL(copymode)) * V_SIZE * 4 + VAL(lang) * V_SIZE * 2 + V_SIZE, X1, Y4 + 50);
	}
	if (a && (temp_value != VAL(copymode)))
		STORE_DATA(COPYMODE, sizeof(VAL(copymode)))
}

static void t70(COLOR_TYPE *p_vmem,int a)/*打印机型号*/
{
	unsigned char temp_value = VAL(printer);
	char temp[12];
	if (a == 0) ;
	else if (3 == a)
		VAL(printer) = !VAL(printer);
	else if ((a == 1) && (VAL(printer) != 1))
		VAL(printer) = 1;
	else if ((a == 2) && (VAL(printer) != 0))
		VAL(printer) = 0;
	memset(temp, 0x00, sizeof(temp));
	if (VAL(printer) == 0)
		sprintf(temp, "hp print1");
	else 
		sprintf(temp, "hp print2");
	if (VAL(ver_pos)[3][2] == 2) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE *116, X1, 250);
		draw_str(p_vmem, temp, X1 + 1 + 3, 255, 2, 2, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, 250);
		draw_str(p_vmem, temp, X1 + 1 + 3, 255, 2, 2, FONT_COL2);
	}
	if (a && (temp_value != VAL(scheme)))
		STORE_DATA(PRINTER, sizeof(VAL(printer)))
}

static void t71(COLOR_TYPE *p_vmem,int a) /*NULL 更新固件*/
{
	char temp[60];
	int fd_usb_storage, fd_kernel, fd_uboot_config, fd_rootfs, size, fd_dataflash;
	char data[0x120000];
	if (module_symbol == 0) {
		sprintf(temp,"insmod /modules/fat.ko");
		system(temp);
		sprintf(temp,"insmod /modules/vfat.ko");
		system(temp);
		sprintf(temp,"insmod /modules/scsi_mod.ko");
		system(temp);
		sprintf(temp,"insmod /modules/sd_mod.ko");
		system(temp);
		sprintf(temp,"insmod /modules/scsi_wait_scan.ko");
		system(temp);
		sprintf(temp,"insmod /modules/usbcore.ko");
		system(temp);
		sprintf(temp,"insmod /modules/ohci-hcd.ko");
		system(temp);
		sprintf(temp,"insmod /modules/usb-storage.ko");
		system(temp);
		sprintf(temp,"insmod /modules/usblp.ko");
		system(temp);
		module_symbol=1;
	}
	if (a == 3) {
		fd_usb_storage=open(SD_DEVICE, O_RDWR);
		if (fd_usb_storage < 0){
			show_info(p_vmem,WARNING_USTORE_ERROR);
			return ;
		} else close(fd_usb_storage);
		sprintf(temp, "mount -t vfat /dev/sda1 usbkey");
		system(temp);
		fd_dataflash = open(MTD3_DEVICE, O_RDWR);
		show_info(p_vmem, WARNING_UPDATING);

		fd_kernel=open("/usbkey/zImage.bin",O_RDONLY);
		if(fd_kernel<0){ 
			printf("no kernel \n");
			close(fd_kernel);
		}else{
			size=read(fd_kernel,data,0x140000);
			close(fd_kernel);
			lseek(fd_dataflash,0x80000,SEEK_SET);
			write(fd_dataflash,data,size);
			printf("kernel wrote\n");
		}
#if 0
		fd_uboot=open("/usbkey/uboot.bin",O_RDONLY);
		if(fd_uboot<0){ 
			printf("no uboot\n");
			close(fd_uboot);

		}else{
			size=read(fd_uboot,data,0x30000);
			close(fd_uboot);
			lseek(fd_dataflash,0x8000,SEEK_SET);
			write(fd_dataflash,data,size);
			printf("uboot wrote\n");
		}
#endif

		fd_rootfs=open("/usbkey/rootfs.bin",O_RDONLY);
		if(fd_rootfs<0){ 
			printf("no rootfs\n");
			close(fd_rootfs);
		}else{
			close(fd_rootfs);
			if(VAL(block_position)){
				sprintf(temp,"eraseall /dev/mtd2");
				system(temp);
				sprintf(temp,"cp /usbkey/rootfs.bin /dev/mtd2");
				system(temp);
				printf("rootfs2 wrote\n");
				fd_uboot_config=open("/block2.bin",O_RDONLY);
			}else{
				sprintf(temp,"eraseall /dev/mtd1");
				system(temp);
				sprintf(temp,"cp /usbkey/rootfs.bin /dev/mtd1");
				system(temp);
				printf("rootfs1 wrote\n");
				fd_uboot_config=open("/block1.bin",O_RDONLY);
			}
			size=read(fd_uboot_config,data,0x4000);
			close(fd_uboot_config);
			lseek(fd_dataflash,0,SEEK_SET);
			write(fd_dataflash,data,size);
			printf("uboot config wrote\n");
			VAL(block_position)=!VAL(block_position);
		}

		close(fd_dataflash);

		sprintf(temp,"umount /usbkey");
		system(temp);
		printf("复制ok\n");
		Sleep(1,0);
		i2c_add[1]=105;
		write_i2c(pt->fd_config,i2c_add,pt->p_config+105,1);
		printf("block store\n");


		show_info(p_vmem,WARNING_UPDATE_OK);
	}
	if(VAL(ver_pos)[3][2]==3){
		show_vbmpmem(p_vmem,bmp_content+V_SIZE*116,X1,350);
	}else{
		show_vbmpmem(p_vmem,bmp_content+V_SIZE*117,X1,350);
	}
}

static void t72(COLOR_TYPE *p_vmem,int a)/*日期*/
{
	char temp1[8], temp2[8], temp3[8];
	time_t timep;
	struct tm *p;
	if (a == 0) ;
	else if(a == 3) {
		if(date_pos < 3) 
			date_pos++;
		else 
			date_pos=0;
	} else if (a == 1) {
		switch (date_pos) {
			case 1:
				if (date[0] < 2038) date[0]++;
				else date[0] = 2009; break;
			case 2:
				if (date[1] < 12) date[1]++;
				else date[1] = 1; break;
			case 3:
				if (date[2] < 31) date[2]++;
				else date[2] = 1; break;
			default:break;
		}
	} else if(a == 2) {
		switch (date_pos) {
			case 1:
				if (date[0] > 2009) date[0]--;
				else date[0] = 2038; break;
			case 2:
				if (date[1] > 1) date[1]--;
				else date[1] = 12; break;
			case 3:
				if (date[2] > 1) date[2]--;
				else date[2] = 31; break;
			default:break;
		}
	}
	if ((VAL(pos) == 3 ) && (VAL(hor_pos)[3] == 3) && (VAL(ver_pos)[3][3] == 0)) {

	} else {
		time(&timep);
		p = gmtime(&timep);
		date[0] = 1900 + p->tm_year;
		date[1] = 1 + p->tm_mon;
		date[2] = p->tm_mday;
	}
	memset(temp1, 0, sizeof(temp1));
	memset(temp2, 0, sizeof(temp1));
	memset(temp3, 0, sizeof(temp1));
	sprintf(temp1, "%d", date[0]);
	sprintf(temp2, "%02d", date[1]);
	sprintf(temp3, "%02d", date[2]);
	if (VAL(ver_pos)[3][3] == 0) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, 50);
		if (1 == date_pos) {
			fbrec(p_vmem, X1 + 1, 50, 64, 49, FONT_COL1);
			draw_str(p_vmem, temp1, X1 + 1, 50, 2, 3, VER_DATA_COL1);
		} else	
			draw_str(p_vmem, temp1, X1 + 1, 50, 2, 3, FONT_COL1);
		if (2 == date_pos) {
			fbrec(p_vmem, X1 + 75, 50, 32, 49, FONT_COL1);
			draw_str(p_vmem, temp2, X1 + 75, 50, 2, 3, VER_DATA_COL1);
		} else	
			draw_str(p_vmem, temp2, X1 + 75, 50, 2, 3, FONT_COL1);
		if (3 == date_pos) {
			fbrec(p_vmem, X1 + 117, 50, 32, 49, FONT_COL1);
			draw_str(p_vmem, temp3, X1 + 117, 50, 2, 3, VER_DATA_COL1);
		} else	
			draw_str(p_vmem, temp3, X1 + 117, 50, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, 50);
		draw_str(p_vmem, temp1, X1 + 1, 50, 2, 3, FONT_COL2);
		draw_str(p_vmem, temp2, X1 + 75, 50, 2,3 , FONT_COL2);
		draw_str(p_vmem, temp3, X1 + 117, 50, 2, 3, FONT_COL2);
	}
	if ((a == 1) || (a == 2))
		set_time(); 
}

static void t73(COLOR_TYPE *p_vmem,int a)/*时间*/
{
	char temp1[8], temp2[8], temp3[8];
	time_t timep;
	struct tm *p;
	if (a == 0) ;
	else if (a == 3) {
		if(time_pos < 3)
			time_pos++;
		else 
			time_pos=0;
	} else if (a == 1) {
		switch (time_pos) {
			case 1:
				if (time_c[0] < 23) time_c[0]++;
				else time_c[0] = 0; break;
			case 2:
				if (time_c[1] < 59) time_c[1]++;
				else time_c[1] = 0; break;
			case 3:
				if (time_c[2] < 59) time_c[2]++;
				else time_c[2] = 0; break;
			default:break;
		}					
	}else if (a == 2) { 
		switch (time_pos) {
			case 1:
				if (time_c[0] > 0) time_c[0]--;
				else time_c[0] = 23; break;
			case 2:
				if(time_c[1] > 0) time_c[1]--;
				else time_c[1] = 59; break;
			case 3:
				if(time_c[2] > 0) time_c[2]--;
				else time_c[2] = 59; break;
			default:break;
		}					
	}
	if ((VAL(pos) == 3) && (VAL(hor_pos)[3] == 3) && (VAL(ver_pos)[3][3] == 1)){
	} else {
		time(&timep);
		p = gmtime(&timep);
		time_c[0] = p->tm_hour;
		time_c[1] = p->tm_min;
		time_c[2] = p->tm_sec;
	}
	memset(temp1, 0, sizeof(temp1));
	memset(temp2, 0, sizeof(temp1));
	memset(temp3, 0, sizeof(temp1));
	sprintf(temp1,"%02d",time_c[0]);
	sprintf(temp2,"%02d",time_c[1]);
	sprintf(temp3,"%02d",time_c[2]);
	if (VAL(ver_pos)[3][3] == 1) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, 150);
		if (1 == time_pos){
			fbrec(p_vmem, X1 + 10, 150, 32, 49, FONT_COL1);
			draw_str(p_vmem, temp1, X1 + 10, 150, 2, 3,VER_DATA_COL1);
		} else	
			draw_str(p_vmem, temp1, 490, 150, 2, 3, FONT_COL1);
		if (2 == time_pos) {
			fbrec(p_vmem, X1 + 53, 150, 32, 49, FONT_COL1);
			draw_str(p_vmem, temp2, X1 + 53, 150, 2, 3, VER_DATA_COL1);
		} else	
			draw_str(p_vmem, temp2, X1 + 53, 150, 2, 3, FONT_COL1);
		if (3 == time_pos) {
			fbrec(p_vmem, X1 + 95, 150, 32, 49, FONT_COL1);
			draw_str(p_vmem, temp3, X1 + 95, 150, 2, 3, VER_DATA_COL1);
		} else	
			draw_str(p_vmem, temp3, X1 + 95, 150, 2, 3, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, 150);
		draw_str(p_vmem, temp1, 490,  150, 2, 3, FONT_COL2);
		draw_str(p_vmem, temp2, X1 + 53, 150, 2, 3, FONT_COL2);
		draw_str(p_vmem, temp3, X1 + 105, 150, 2, 3, FONT_COL2);
	}
	if ((a == 1) || (a == 2))
		set_time(); 
}

static void t74(COLOR_TYPE *p_vmem,int a)/*蜂鸣*/
{
	unsigned char temp_value=VAL(horn), *p;
	if (a == 0) ;
	else if (3 == a)
		VAL(horn) = !VAL(horn);
	else if ((a == 1) && (VAL(horn) != 1))
		VAL(horn) = 1;
	else if ((a == 2) && (VAL(horn) != 0))
		VAL(horn) = 0;
	if (VAL(horn) == 0) 
		p = bmp_content + 2 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	else  
		p = bmp_content + 23 * V_SIZE * 4 + VAL(lang) * V_SIZE * 2;
	if (VAL(ver_pos)[3][3] == 2) {
		show_vbmpmem(p_vmem, p, X1, Y5 + 50);
	} else {
		show_vbmpmem(p_vmem, p + V_SIZE, X1, Y5 + 50);
	}
	if (a && (temp_value != VAL(horn)))
		STORE_DATA(HORN, sizeof(VAL(horn)))
}

static void t75(COLOR_TYPE *p_vmem,int a)/*单位*/
{
	char ss1[]="inch", ss2[]="mm", *ss;
	unsigned char temp_value=VAL(unit);
	if (a == 0);
	else if (3 == a) 
		VAL(unit) = !VAL(unit);
	else if ((a == 1) && (VAL(unit) != 1))
		VAL(unit) = 1;
	else if ((a == 2) && (VAL(unit) != 0))
		VAL(unit) = 0;
	if (VAL(unit) == 0)
		ss=ss1;
	else
		ss=ss2;
	if (VAL(ver_pos)[3][3] == 3) {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 116, X1, 350);
		draw_str(p_vmem, ss, X1 + 1 + 3, 355, 2, 2, FONT_COL1);
	} else {
		show_vbmpmem(p_vmem, bmp_content + V_SIZE * 117, X1, 350);
		draw_str(p_vmem, ss, X1 + 1 + 3, 355, 2, 2, FONT_COL2);
	}
	if (a && (temp_value != VAL(unit)))
		STORE_DATA(UNIT, sizeof(VAL(unit)))
}



static void t76(COLOR_TYPE *p_vmem,int a){}
static void t77(COLOR_TYPE *p_vmem,int a){}
static void t78(COLOR_TYPE *p_vmem,int a){}
static void t79(COLOR_TYPE *p_vmem,int a){}

int change_db(COLOR_TYPE *p_vmem,int a)//修改增益
{
	char temp[12];
	unsigned char tmp_add[]={0,90};
	unsigned short temp_val=VAL(db);
	int i;
	if(a==0) ;
	else if(a==1)	{
		if((VAL(db)+db_step0[VAL(db_step)])>=1100) VAL(db)=1100;
		else VAL(db)+=db_step0[VAL(db_step)];
	} else if(a==2){ 
		if((VAL(db)-db_step0[VAL(db_step)])<=0) VAL(db)=0;
		else VAL(db)-=db_step0[VAL(db_step)];
	} else if ( 3 == a) 
		VAL(db) = temp_db;
	memset(temp,0,sizeof(temp));

	if((temp_val!=VAL(db)) && a){
		write_db_val(VAL(db),temp_val);
		write_i2c(pt->fd_config,tmp_add,pt->p_config+90,2);
		Sleep(0,16000);
		if (VAL(avg_data_y) && VAL(avg_data_x) && (VAL(avg_status))) {
			if(VAL(db)>temp_val)
				VAL(avg_data_y) = (float)((VAL(avg_data_y))*db_table[VAL(db)-temp_val]);
			else
				VAL(avg_data_y) = (float)((VAL(avg_data_y))/(db_table[temp_val-VAL(db)]));
			cal_avg_data();

		}
		if((VAL(dacecho) != 0) && (VAL(dacmode) != 0)){
			if(VAL(db)>temp_val)
				for(i=0;i<VAL(dacecho);i++)
					VAL(dac_data_y)[i] = (float)((VAL(dac_data_y)[i])*db_table[VAL(db)-temp_val]);

			else{
				for(i=0;i<VAL(dacecho);i++)
					VAL(dac_data_y)[i] = (float)((VAL(dac_data_y)[i])/(db_table[temp_val-VAL(db)]));
			}
			STORE_DATA(DAC_DATA_X, (sizeof(int) * 5))
				STORE_DATA(128, (sizeof(int) * 8))
				STORE_DATA(160, (sizeof(int) * 3))
				cal_dac_data1();
			//			cal_dac_data(dac_dot_data,VAL(dac_data_x),VAL(dac_data_y),VAL(dacecho));
		}
	}
	return 0;
}

int change_db_step(COLOR_TYPE *p_vmem,int a)//修改增益步长
{
	char temp[12];
	unsigned char tmp_add[]={0,85},temp_val=VAL(db_step);
	if(a==0) ;
	else if(a==1)	{
		if (VAL(db_step) >= 5)
			VAL(db_step) = 0;
		else
			VAL(db_step) += 1;
	}
	else if(a==2){ 
		if (VAL(db_step) <= 5)
			VAL(db_step) = 5;
		else
			VAL(db_step) -= 1;
	}
	memset(temp,0,sizeof(temp));
	/**
	  sprintf(temp,"\040%.1fdB",(float)db_step0[VAL(db_step)]/10.0);
	  fbrec(p_vmem,10,37,96,32,WAVE_BACK);
	  draw_str(p_vmem,temp,10,41,2,2,WAVE_GROUND);
	 **/
	if((temp_val!=VAL(db_step))&&a)	write_i2c(pt->fd_config,tmp_add,pt->p_config+85,1);
	return 0;
}

int draw_agat(COLOR_TYPE *p_vmem,int a)//画闸门A   pt->dot_quantily   pt->current_data_tmp
{
	if(VAL(alogic)){
		unsigned char key_alarm[2]={0xed,0xed};
		int x1,y1,x2,tx,t1,i,x_tmp=400,max_tmp,y_height,old_bit=!(pt->hanfeng == 0),abcd;
		//		if(VAL(zoomed)) {x_tmp=600,y_tmp=450;}
		tx=(float)(VAL(astart)-pt->true_delay)*400.0/((float)pt->true_range);
		t1=(float)(VAL(awidth))*400.0/((float)pt->true_range);
		x1=(tx>x_tmp)?x_tmp:((tx<0)?0:tx);
		x2=((t1+x1)>x_tmp)?x_tmp:(t1+x1);
		y1=(100-VAL(athrsh))*400.0/100;
		if(VAL(tof)==1){
			for(i=x1,pt->a_gata_max=x1;i<x2;i++)
				if(pt->current_data[i]<=y1){
					pt->a_gata_max=i;
					break ;
				}
		}else{
			for(i=x2,pt->a_gata_max=x2;i>x1;i--)
				if((pt->current_data[pt->a_gata_max]>pt->current_data[i-1])&&(pt->current_data[i-11]>20))
					pt->a_gata_max=i-1;
		}
		max_tmp = pt->a_gata_max;
		if(max_tmp>0){
			if((pt->current_data[max_tmp-1]<pt->current_data[max_tmp])&&(pt->current_data[max_tmp-1]>20))
				pt->a_gata_max=max_tmp-1;
		}
		if(max_tmp<400){
			if((pt->current_data[max_tmp+1]<pt->current_data[pt->a_gata_max])&&(pt->current_data[max_tmp+1]>20))
				pt->a_gata_max=max_tmp+1;
		}

		if(VAL(zoomed))
			x1=x1*1.5>600?600:x1*1.5,y1=y1*9.0/8,x2=x2*1.5>600?600:x2*1.5,
				max_tmp=pt->a_gata_max*1.5>600?600:pt->a_gata_max*1.5;
		else max_tmp=pt->a_gata_max;
		if(pt->current_data[pt->a_gata_max]<pt->agat_y)	pt->agat_y=pt->current_data[pt->a_gata_max],pt->agat_x=max_tmp;
		y_height = pt->agat_y>400?400:pt->agat_y;
		fbline(p_vmem, x1, y1>>old_bit, x1+5, (y1-5)>>old_bit, COL21);
		fbline(p_vmem, x1, y1>>old_bit, x1+5, (y1+5)>>old_bit, COL21);
		fbline(p_vmem, x2, y1>>old_bit, x2+5, (y1-5)>>old_bit, COL21);
		fbline(p_vmem, x2, y1>>old_bit, x2+5, (y1+5)>>old_bit, COL21);
		fbline(p_vmem, x1, (y1 + 1)>>old_bit, x2, (y1 + 1)>>old_bit, COL21);
		fbline(p_vmem, x1, (y1 - 1)>>old_bit, x2, (y1 - 1)>>old_bit, COL21);
		fbline(p_vmem, x1, y1>>old_bit, x2, y1>>old_bit, COL21);
		if(pt->current_data[pt->a_gata_max]<380){
			if(1==VAL(alogic)) 
				fbrec(p_vmem, (max_tmp - 2) > 0 ? (max_tmp - 1) : 0, (y1 - 4)>>old_bit,
						5 + ((max_tmp - 2) < 0 ? (max_tmp - 2) : 0), 3, COL21);
			else	
				fbrec(p_vmem, (max_tmp - 2) > 0 ? (max_tmp - 2) : 0, (y1 + 4)>>old_bit,
						5 + ((max_tmp - 2) < 0 ? (max_tmp - 2) : 0), 3, COL21);
			fbline(p_vmem,(pt->agat_x-5)<0?0:(pt->agat_x-5), 
					((y_height-5)<0?0:(y_height-5))>>old_bit,
					pt->agat_x+5, (y_height+5)>>old_bit,49);
			fbline(p_vmem,(pt->agat_x-5)<0?0:(pt->agat_x-5),
					(y_height+5)>>old_bit,pt->agat_x+5,
					((y_height-5)<0?0:(y_height-5))>>old_bit,49);
		}
		if (y1>pt->current_data[pt->a_gata_max]) {
			if((VAL(horn)==1)&&(VAL(alogic)==1)) { 
				write(pt->fd_key,key_alarm,1);
				abcd = 1;
				ioctl(pt->fd_gpio,LED1,&abcd);
			} else {
				abcd = 0;
				ioctl(pt->fd_gpio,LED1,&abcd);
			}
		} else if(y1<pt->current_data[pt->a_gata_max]){
			if((VAL(horn)==1)&&(VAL(alogic)==2)) {
				write(pt->fd_key,key_alarm,1);
				abcd = 1;
				ioctl(pt->fd_gpio,LED1,&abcd);
			} else {
				abcd = 0;
				ioctl(pt->fd_gpio,LED1,&abcd);
			}
		}
	}
	return 0;
}

int draw_bgat(COLOR_TYPE *p_vmem,int a)//画闸门B
{
	if(VAL(blogic)){
		int x1,y1,x2,tx,t1,i,x_tmp=400,max_tmp,old_bit=!(pt->hanfeng == 0);
//		unsigned char key_alarm[2]={0xed,0xed};
		//		if(VAL(zoomed)) {x_tmp=600,y_tmp=450;}
		tx=(float)(VAL(bstart)-pt->true_delay)*400.0/((float)pt->true_range);
		t1=(float)(VAL(bwidth))*400.0/((float)pt->true_range);
		x1=(tx>x_tmp)?x_tmp:((tx<0)?0:tx);
		x2=((t1+x1)>x_tmp)?x_tmp:(t1+x1);
		y1=(100-VAL(bthrsh))*400.0/100;
		if(VAL(tof)==1) pt->b_gata_max=x1;
		else {
			for(i=x2,pt->b_gata_max=x2;i>x1;i--)
				if((pt->current_data[pt->b_gata_max]>pt->current_data[i-1])&&(pt->current_data[i-11]>20))
					pt->b_gata_max=i-1;
		}
		max_tmp = pt->b_gata_max;
		if(max_tmp>0){
			if((pt->current_data[max_tmp-1]<pt->current_data[max_tmp])&&(pt->current_data[max_tmp-1]>20))
				pt->b_gata_max=max_tmp-1;
		}
		if(max_tmp<400){
			if((pt->current_data[max_tmp+1]<pt->current_data[pt->b_gata_max])&&(pt->current_data[max_tmp+1]>20))
				pt->b_gata_max=max_tmp+1;
		}

		if(VAL(zoomed))
			x1=x1*1.5>600?600:x1*1.5,y1=y1*9.0/8,x2=x2*1.5>600?600:x2*1.5,
				max_tmp=pt->b_gata_max*1.5>600?600:pt->b_gata_max*1.5;
		else max_tmp=pt->b_gata_max;
		fbline(p_vmem,x1,y1>>old_bit,x1+5,(y1-5)>>old_bit,COL24);
		fbline(p_vmem,x1,y1>>old_bit,x1+5,(y1+5)>>old_bit,COL24);
		fbline(p_vmem,x2,y1>>old_bit,x2+5,(y1-5)>>old_bit,COL24);
		fbline(p_vmem,x2,y1>>old_bit,x2+5,(y1+5)>>old_bit,COL24);
		fbline(p_vmem,x1,y1>>old_bit,x2,y1>>old_bit,COL24);
		fbline(p_vmem,x1,(y1 + 1)>>old_bit,x2,(y1 + 1)>>old_bit,COL24);
		fbline(p_vmem,x1,(y1 - 1)>>old_bit,x2,(y1 - 1)>>old_bit,COL24);
		if(pt->current_data[pt->a_gata_max]<380){
			if(1==VAL(blogic)) 
				fbrec(p_vmem, (max_tmp - 2) > 0 ? (max_tmp - 1) : 0, (y1 - 4)>>old_bit,
						5 + ((max_tmp - 2) < 0 ? (max_tmp - 2) : 0), 3, COL24);
			else	
				fbrec(p_vmem, (max_tmp - 2) > 0 ? (max_tmp - 2) : 0, (y1 + 4)>>old_bit,
						5 + ((max_tmp - 2) < 0 ? (max_tmp - 2) : 0), 3, COL24);
		}
#if 0
		if(y1>current_data[b_gata_max]){
			if((VAL(horn)==1)&&(VAL(blogic)==1)) {
				write(fd_key,key_alarm,1);
				abcd = 1;
				ioctl(fd_gpio,LED1,&abcd);
			} else {
				abcd = 0;
				ioctl(fd_gpio,LED1,&abcd);
			}
		}else if(y1<current_data[b_gata_max]){
			if((VAL(horn)==1)&&(VAL(blogic)==2)) {
				write(fd_key,key_alarm,1);
				abcd = 1;
				ioctl(fd_gpio,LED1,&abcd);
			} else {
				abcd = 0;
				ioctl(fd_gpio,LED1,&abcd);
			}
		}
#endif
	}
	return 0;
}

int draw_scale(COLOR_TYPE *p_vmem,int a)//画屏幕下方的刻度以及屏幕显示数据
{
	char temp1[20];
	int i1, temp, temp2, y_tmp;

	if (0 == VAL(zoomed)) {
		draw_str(p_vmem,display_top[VAL(meas1)],21,Y1 + 13,1,1,WAVE_GROUND);
		display_value(p_vmem, VAL(meas1), 41, Y1 + 13, 1, 1, 1);
		draw_str(p_vmem,display_top[VAL(meas2)],141,Y1 + 13,1,1,WAVE_GROUND);
		display_value(p_vmem,VAL(meas2),161,Y1 + 13,1,1,1);
		draw_str(p_vmem,display_top[VAL(meas3)],261,Y1 + 13,1,1,WAVE_GROUND);
		display_value(p_vmem,VAL(meas3),281,Y1 + 13,1,1,1);
		draw_str(p_vmem,display_top[VAL(meas4)],381,Y1 + 13,1,1,WAVE_GROUND);
		display_value(p_vmem,VAL(meas4),401,Y1 + 13,1,1,1);
	} else {
		draw_str(p_vmem,display_top[VAL(meas1)],21,465,1,1,WAVE_GROUND);
		display_value(p_vmem, VAL(meas1), 41, 465 ,1, 1, 1);
		draw_str(p_vmem,display_top[VAL(meas2)],141,465,1,1,WAVE_GROUND);
		display_value(p_vmem,VAL(meas2),161,465,1,1,1);
		draw_str(p_vmem,display_top[VAL(meas3)],261,465,1,1,WAVE_GROUND);
		display_value(p_vmem,VAL(meas3),281,465,1,1,1);
		draw_str(p_vmem,display_top[VAL(meas4)],381,465,1,1,WAVE_GROUND);
		display_value(p_vmem,VAL(meas4),401,465,1,1,1);
	}

	if (VAL(scale) == 1) { //声程计算
		if (0 == VAL(zoomed)) {
			temp=80;
			y_tmp = Y1 + 1;
		} else {
			temp = 119;
			y_tmp=451;
		}
		if(1==VAL(unit))
			temp2=1000;
		else
			temp2=1000*25.4;
		for	(i1 = 0; i1 < 6; i1++) {
			memset(temp1, 0, sizeof(temp1));
			sprintf(temp1, "%.*f", 
					4-test_float_length((float)(pt->true_range*i1/5+pt->true_delay)/temp2),
					(float)(pt->true_range*i1/5+pt->true_delay)/temp2);
			if ((pt->true_range*i1 / 5 + pt->true_delay) < 9999001.0)	
				draw_str(p_vmem,temp1, i1 * temp + 1, y_tmp, 1, 1, WAVE_GROUND);
			else 
				draw_ascii(p_vmem, '*', i1 * temp + 1, y_tmp, 1, 1,WAVE_GROUND);
		}
	} else {   /*分度*/
		if (0 == VAL(zoomed) )	{
			temp = 40;
			y_tmp=401;
		} else {
			temp = 60;
			y_tmp = 451;
		}
		for (i1 = 1; i1 < 11; i1++) {
			memset(temp1, 0, sizeof(temp1));
			sprintf(temp1, "%d", i1);
			draw_str(p_vmem, temp1, i1 * temp, y_tmp, 1, 1, WAVE_GROUND);
		}
	}


	if (VAL(alogic)) {
		draw_str(p_vmem, display_top[1], 401, Y0 + 1, 1, 1, WAVE_GROUND);
		display_value(p_vmem, 1, 421, Y0 + 1, 1, 1, 1);
		draw_str(p_vmem, display_top[12], 401, Y0 + 17, 1, 1, WAVE_GROUND);
		display_value(p_vmem, 12, 421, Y0 + 17, 1, 1, 1);
		draw_str(p_vmem, display_top[14], 401, Y0 + 33, 1, 1, WAVE_GROUND);
		display_value(p_vmem, 14, 421, Y0 + 33, 1, 1, 1);
		if (VAL(dacmode)== 3) {
		}

	} else if (VAL(blogic)) {
		draw_str(p_vmem, display_top[2], 401, Y0 + 1, 1, 1, WAVE_GROUND);
		display_value(p_vmem, 1, 421, Y0 + 1, 1, 1, 1);
		draw_str(p_vmem, display_top[13], 401, Y0 + 17, 1, 1, WAVE_GROUND);
		display_value(p_vmem, 12, 421, Y0 + 17, 1, 1, 1);
		draw_str(p_vmem, display_top[15], 401, Y0 + 33, 1, 1, WAVE_GROUND);
		display_value(p_vmem, 14, 421, Y0 + 33, 1, 1, 1);
	}

	if (VAL(dacmode) && (VAL(dacecho) >= 2)) {
		draw_dac_line(p_vmem,dac_dot_data,WAVE_GROUND);
		draw_dac_line(p_vmem,dac_dot_offdata1,COL24);
		draw_dac_line(p_vmem,dac_dot_offdata2,DAC_COL);
		draw_dac_line(p_vmem,dac_dot_offdata3,COL21);
	}

	if(VAL(avg_status) && VAL(avg_data_x) && VAL(avg_data_y)) {
		draw_dac_line(p_vmem, avg_dot_data, DAC_COL);
	}


	if(VAL(frez)&&(probe_test)){
		for	(i1=1;i1<6;i1++){
			memset(temp1,0,sizeof(temp1));
			sprintf(temp1,"%d",i1*4);
			draw_str(p_vmem,temp1,i1*80+1,380,1,1,49);
		}
		draw_str(p_vmem,"Mhz",420,380,1,1,49);
	}

	memset(temp1, 0, sizeof(temp1));
	if(VAL(db) >= 1000) 
		sprintf(temp1, "%.1fdB", (float)VAL(db)/10);
	else if(VAL(db)>=100) 
		sprintf(temp1, "\040%.1fdB", (float)VAL(db)/10);
	else 
		sprintf(temp1,"\040\040%.1fdB",(float)VAL(db)/10);
	draw_str(p_vmem,temp1,10,1,2,3,WAVE_GROUND);

	memset(temp1, 0, sizeof(temp1));
	sprintf(temp1, "\040\040%.1f", (float)db_step0[VAL(db_step)]/10.0);
	draw_str(p_vmem,temp1,10,41,2,2,WAVE_GROUND);

	return 0;
}

int draw_four_dot_frez(COLOR_TYPE *p_vmem,int x,int y,int x_off,int y_off,int col)
{
	fbdot(p_vmem,x+x_off,y+y_off,col);
	fbdot(p_vmem,x+16-x_off,y+y_off,col);
	fbdot(p_vmem,x+x_off,y+16-y_off,col);
	fbdot(p_vmem,x+16-x_off,y+16-y_off,col);
	return 0;
}

int draw_frez(COLOR_TYPE *p_vmem,int x,int y,int col)
{
	fbline(p_vmem,x+8,y,x+8,y+16,col);

	draw_four_dot_frez(p_vmem,x,y,3,1,col);
	draw_four_dot_frez(p_vmem,x,y,6,1,col);
	draw_four_dot_frez(p_vmem,x,y,1,2,col);
	draw_four_dot_frez(p_vmem,x,y,3,2,col);
	draw_four_dot_frez(p_vmem,x,y,7,2,col);
	draw_four_dot_frez(p_vmem,x,y,2,3,col);
	draw_four_dot_frez(p_vmem,x,y,5,3,col);
	draw_four_dot_frez(p_vmem,x,y,0,4,col);
	draw_four_dot_frez(p_vmem,x,y,1,4,col);
	draw_four_dot_frez(p_vmem,x,y,3,4,col);
	draw_four_dot_frez(p_vmem,x,y,5,4,col);
	draw_four_dot_frez(p_vmem,x,y,6,4,col);
	draw_four_dot_frez(p_vmem,x,y,4,5,col);
	draw_four_dot_frez(p_vmem,x,y,7,5,col);
	draw_four_dot_frez(p_vmem,x,y,2,6,col);
	draw_four_dot_frez(p_vmem,x,y,3,6,col);	
	draw_four_dot_frez(p_vmem,x,y,5,6,col);
	draw_four_dot_frez(p_vmem,x,y,6,7,col);
	draw_four_dot_frez(p_vmem,x,y,7,8,col);
	return 0;

}

int draw_battery(COLOR_TYPE *p_vmem,int a)
{
	int x_tmp,y_tmp,battery_quantily=a-1,i;
	if(VAL(zoomed)) x_tmp=554,y_tmp=456;
	else x_tmp=594,y_tmp=407;
	fbline(p_vmem,x_tmp+4,y_tmp,x_tmp+36,y_tmp,battery_col[battery_quantily]);
	fbline(p_vmem,x_tmp+4,y_tmp+17,x_tmp+36,y_tmp+17,battery_col[battery_quantily]);
	fbline(p_vmem,x_tmp+3,y_tmp+1,x_tmp+3,y_tmp+16,battery_col[battery_quantily]);
	fbline(p_vmem,x_tmp+37,y_tmp+1,x_tmp+37,y_tmp+16,battery_col[battery_quantily]);
	fbline(p_vmem,x_tmp,y_tmp+6,x_tmp,y_tmp+11,battery_col[battery_quantily]);
	fbline(p_vmem,x_tmp,y_tmp+6,x_tmp+2,y_tmp+6,battery_col[battery_quantily]);
	fbline(p_vmem,x_tmp,y_tmp+11,x_tmp+2,y_tmp+11,battery_col[battery_quantily]);
	for(i=0;i<5-battery_quantily;i++){
		fbrec(p_vmem,x_tmp+31-i*6,y_tmp+1,3,16,battery_col[battery_quantily]);
	}
	if(battery_quantily==5){
		for(i=0;i<11;i++){
			fbrec(p_vmem,x_tmp+9+i*2,y_tmp-2+i*2,4,2,COL29);
			fbrec(p_vmem,x_tmp+9+i*2,y_tmp-2+20-i*2,4,2,COL29);
		}
	}
	return 0;
}

int do_copy(COLOR_TYPE *p_vmem, int a)//其他项目放大 打印
{
	int fd_usb_print=-1,fd_usb_storage=-1,val;
	char temp[60];
	if(module_symbol==0){
		sprintf(temp,"insmod /modules/fat.ko");
		system(temp);
		sprintf(temp,"insmod /modules/vfat.ko");
		system(temp);
		sprintf(temp,"insmod /modules/scsi_mod.ko");
		system(temp);
		sprintf(temp,"insmod /modules/sd_mod.ko");
		system(temp);
		sprintf(temp,"insmod /modules/scsi_wait_scan.ko");
		system(temp);
		sprintf(temp,"insmod /modules/usbcore.ko");
		system(temp);
		sprintf(temp,"insmod /modules/ohci-hcd.ko");
		system(temp);
		sprintf(temp,"insmod /modules/usb-storage.ko");
		system(temp);
		sprintf(temp,"insmod /modules/usblp.ko");
		system(temp);
		module_symbol=1;
	}
	switch(a){
		case 1:
			//			memset(envelope_data,0xff,sizeof(envelope_data));
			VAL(zoomed) = !VAL(zoomed);
			draw_rim(p_vmem,1);
			break;
		case 2:if(!VAL(zoomed)){
				   switch(VAL(copymode)){
					   case 0:
						   fd_usb_print=open("/dev/usblp0",O_RDWR);
						   if(fd_usb_print<0){
							   show_info(p_vmem,WARNING_PRINT_ERROR);
							   //							   break;
						   }
						   show_info(p_vmem,WARNING_PRINT);
						   //						   close(pt->fd_key);
						   int i,j,fd_temp;
						   unsigned char c[960*640*3],b[54];
						   memset(c,0xff,sizeof(c));memset(b,0x00,sizeof(b));memset(temp,0x00,sizeof(temp));
						   for(i=0;i<480;i++){
							   for(j=0;j<640;j++){
								   /*				printf("%x\n",pt->true_video_add[640*i+j]);*/
								   // 			c[1843200-(640*i+(640-j))*3+0]=0xc8;c[1843200-(640*i+(640-j))*3+1]=0xa9;c[1843200-(640*i+(640-j))*3+2]=0x81;

								   if(VAL(scheme)<3){
									   switch(pt->true_video_add[640*i+j]){
										   case 0:
										   case 18:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0x00;c[1843200-(640*i+(640-j))*3+2]=0x00;break;
										   case 1:
										   case 6:
										   case 11:
											   c[1843200-(640*i+(640-j))*3+0]=0xfd;c[1843200-(640*i+(640-j))*3+1]=0xb7;c[1843200-(640*i+(640-j))*3+2]=0x9a;break;
										   case 2:
											   c[1843200-(640*i+(640-j))*3+0]=0xa0;c[1843200-(640*i+(640-j))*3+1]=0x6d;c[1843200-(640*i+(640-j))*3+2]=0x2f;break;
										   case 3:
										   case 8:
										   case 13:
											   c[1843200-(640*i+(640-j))*3+0]=0x6a;c[1843200-(640*i+(640-j))*3+1]=0x48;c[1843200-(640*i+(640-j))*3+2]=0x1d;break;
										   case 4:
										   case 9:
										   case 15:
											   c[1843200-(640*i+(640-j))*3+0]=0x27;c[1843200-(640*i+(640-j))*3+1]=0x1b;c[1843200-(640*i+(640-j))*3+2]=0x0d;break;
										   case 5:
										   case 10:
											   c[1843200-(640*i+(640-j))*3+0]=0x70;c[1843200-(640*i+(640-j))*3+1]=0x3f;c[1843200-(640*i+(640-j))*3+2]=0x02;break;
										   case 7:
											   c[1843200-(640*i+(640-j))*3+0]=0xc1;c[1843200-(640*i+(640-j))*3+1]=0x90;c[1843200-(640*i+(640-j))*3+2]=0x54;break;
										   case 12:
											   c[1843200-(640*i+(640-j))*3+0]=0xcf;c[1843200-(640*i+(640-j))*3+1]=0x9e;c[1843200-(640*i+(640-j))*3+2]=0x54;break;
										   case 14:
											   c[1843200-(640*i+(640-j))*3+0]=0x7e;c[1843200-(640*i+(640-j))*3+1]=0x47;c[1843200-(640*i+(640-j))*3+2]=0x02;break;
										   case 16:
											   c[1843200-(640*i+(640-j))*3+0]=0xff;c[1843200-(640*i+(640-j))*3+1]=0xff;c[1843200-(640*i+(640-j))*3+2]=0xff;break;
										   case 17:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0xff;c[1843200-(640*i+(640-j))*3+2]=0xff;break;
										   case 19:
											   c[1843200-(640*i+(640-j))*3+0]=0x02;c[1843200-(640*i+(640-j))*3+1]=0x29;c[1843200-(640*i+(640-j))*3+2]=0x63;break;
										   case 20:
											   c[1843200-(640*i+(640-j))*3+0]=0x06;c[1843200-(640*i+(640-j))*3+1]=0x68;c[1843200-(640*i+(640-j))*3+2]=0xfa;break;
										   case 21:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0x00;c[1843200-(640*i+(640-j))*3+2]=0xff;break;
										   case 22:
											   c[1843200-(640*i+(640-j))*3+0]=0x60;c[1843200-(640*i+(640-j))*3+1]=0x00;c[1843200-(640*i+(640-j))*3+2]=0x00;break;
										   case 23:
											   c[1843200-(640*i+(640-j))*3+0]=0xe0;c[1843200-(640*i+(640-j))*3+1]=0x40;c[1843200-(640*i+(640-j))*3+2]=0x00;break;
										   case 24:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0xff;c[1843200-(640*i+(640-j))*3+2]=0x00;break;
										   case 25:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0xe0;c[1843200-(640*i+(640-j))*3+2]=0xff;break;
										   case 26:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0xff;c[1843200-(640*i+(640-j))*3+2]=0xff;break;
										   case 27:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0x90;c[1843200-(640*i+(640-j))*3+2]=0xff;break;
										   case 28:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0x4f;c[1843200-(640*i+(640-j))*3+2]=0xff;break;
										   case 29:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0x00;c[1843200-(640*i+(640-j))*3+2]=0xff;break;
										   case 30:
											   c[1843200-(640*i+(640-j))*3+0]=0xc8;c[1843200-(640*i+(640-j))*3+1]=0xc8;c[1843200-(640*i+(640-j))*3+2]=0xc8;break;
										   case 40:
											   c[1843200-(640*i+(640-j))*3+0]=0xd0;c[1843200-(640*i+(640-j))*3+1]=0x78;c[1843200-(640*i+(640-j))*3+2]=0x30;break;
										   case 41:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0x68;c[1843200-(640*i+(640-j))*3+2]=0x00;break;
										   case 49:
											   c[1843200-(640*i+(640-j))*3+0]=0xff;c[1843200-(640*i+(640-j))*3+1]=0x00;c[1843200-(640*i+(640-j))*3+2]=0xff;break;
										   default:break;
									   }
								   }else{
									   switch(pt->true_video_add[640*i+j]){
										   case 0:
										   case 2:
											   c[1843200-(640*i+(640-j))*3+0]=0x11;c[1843200-(640*i+(640-j))*3+1]=0x32;c[1843200-(640*i+(640-j))*3+2]=0x05;break;
										   case 1:
											   c[1843200-(640*i+(640-j))*3+0]=0x5c;c[1843200-(640*i+(640-j))*3+1]=0x93;c[1843200-(640*i+(640-j))*3+2]=0x00;break;
										   case 3:
										   case 5:
											   c[1843200-(640*i+(640-j))*3+0]=0x5d;c[1843200-(640*i+(640-j))*3+1]=0xa8;c[1843200-(640*i+(640-j))*3+2]=0x01;break;
										   case 4:
											   c[1843200-(640*i+(640-j))*3+0]=0x10;c[1843200-(640*i+(640-j))*3+1]=0x42;c[1843200-(640*i+(640-j))*3+2]=0x05;break;
										   case 6:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0xff;c[1843200-(640*i+(640-j))*3+2]=0xff;break;
										   case 10:
										   case 16:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0x00;c[1843200-(640*i+(640-j))*3+2]=0x00;break;
										   case 7:
										   case 11:
										   case 17:
											   c[1843200-(640*i+(640-j))*3+0]=0xff;c[1843200-(640*i+(640-j))*3+1]=0xff;c[1843200-(640*i+(640-j))*3+2]=0xff;break;
										   case 8:
											   c[1843200-(640*i+(640-j))*3+0]=0x02;c[1843200-(640*i+(640-j))*3+1]=0x6d;c[1843200-(640*i+(640-j))*3+2]=0x02;break;
										   case 9:
											   c[1843200-(640*i+(640-j))*3+0]=0xff;c[1843200-(640*i+(640-j))*3+1]=0x5e;c[1843200-(640*i+(640-j))*3+2]=0x5e;break;
										   case 18:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0x00;c[1843200-(640*i+(640-j))*3+2]=0xff;break;
										   case 19:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0xff;c[1843200-(640*i+(640-j))*3+2]=0x00;break;
										   case 20:
											   c[1843200-(640*i+(640-j))*3+0]=0xff;c[1843200-(640*i+(640-j))*3+1]=0x00;c[1843200-(640*i+(640-j))*3+2]=0x00;break;
										   case 21:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0xff;c[1843200-(640*i+(640-j))*3+2]=0xff;break;
										   case 22:
											   c[1843200-(640*i+(640-j))*3+0]=0xff;c[1843200-(640*i+(640-j))*3+1]=0x5e;c[1843200-(640*i+(640-j))*3+2]=0x5e;break;
										   case 23:
											   c[1843200-(640*i+(640-j))*3+0]=0xff;c[1843200-(640*i+(640-j))*3+1]=0x5e;c[1843200-(640*i+(640-j))*3+2]=0x5e;break;
										   case 24:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0xff;c[1843200-(640*i+(640-j))*3+2]=0x00;break;
										   case 25:
											   c[1843200-(640*i+(640-j))*3+0]=0x00;c[1843200-(640*i+(640-j))*3+1]=0xff;c[1843200-(640*i+(640-j))*3+2]=0xc8;break;
										   default:break;
									   }
								   }
							   }
						   }
						   sprintf(temp,"RANGE:%.3fmm",(float)VAL(range)/1000);
						   draw_str_24(c,temp,20,500,1,1,1,0);
						   sprintf(temp,"MTLVEL:%dM/S",VAL(mtlvel));
						   draw_str_24(c,temp,20,516,1,1,1,0);
						   sprintf(temp,"D-DELAY:%.3fmm",(float)VAL(d_delay)/1000);
						   draw_str_24(c,temp,20,532,1,1,1,0);
						   sprintf(temp,"P-DELAY:%.3fmm",(float)VAL(p_delay)/1000);
						   draw_str_24(c,temp,20,548,1,1,1,0);
						   switch(VAL(damping)){
							   case 0:sprintf(temp,"DAMPING:%d\352",50);break;
							   case 1:sprintf(temp,"DAMPING:%d\352",100);break;
							   case 2:sprintf(temp,"DAMPING:%d\352",200);break;
							   case 3:sprintf(temp,"DAMPING:%d\352",500);break;
							   default:break;
						   }draw_str_24(c,temp,20,564,1,1,1,0);
						   switch(VAL(power)){
							   case 0:sprintf(temp,"POWER:LOW");break;
							   case 1:sprintf(temp,"POWER:HIGH");break;
							   default:break;
						   }draw_str_24(c,temp,20,580,1,1,1,0);
						   switch(VAL(dual)){
							   case 0:sprintf(temp,"DUAL:OFF");break;
							   case 1:sprintf(temp,"DUAL:ON");break;
							   default:break;
						   }draw_str_24(c,temp,20,596,1,1,1,0);
						   switch(VAL(rectify)){
							   case 0:sprintf(temp,"RECTIFY:FULL-W");break;
							   case 1:sprintf(temp,"RECTIFY:POS.H-W");break;
							   case 2:sprintf(temp,"RECTIFY:NEG.H-W");break;
							   case 3:sprintf(temp,"RECTIFY:RF");break;
							   default:break;
						   }draw_str_24(c,temp,20,612,1,1,1,0);

						   sprintf(temp,"ANGLE:%.1f",(float)VAL(angle)/10);
						   draw_str_24(c,temp,20,628,1,1,1,0);

						   sprintf(temp,"X_VALUE:%.3fmm",(float)VAL(x_value)/1000);
						   draw_str_24(c,temp,20,644,1,1,1,0);

						   sprintf(temp,"Sa:%.2fmm",((float)(pt->measure_cur_data->Sa))/(1000));
						   draw_str_24(c,temp,200,500,1,1,1,0);

						   sprintf(temp,"Sb:%.2fmm",((float)(pt->measure_cur_data->Sb))/(1000));
						   draw_str_24(c,temp,200,516,1,1,1,0);

						   //		fbrec_24(c,100,100,100,100,0xffffffff);
						   fd_temp=open("/temp/bmp_header",O_RDONLY);read(fd_temp,b,54);close(fd_temp);
						   fd_temp=open("/tmp/bmp_temp.bmp",O_RDWR|O_CREAT|O_TRUNC);
						   write(fd_temp,b,sizeof(b));
						   write(fd_temp,c,sizeof(c));
						   close(fd_temp);
						   memset(temp,0x00,sizeof(temp));
						   /*抓图用*/
						   //						   sprintf(temp,"udevstart");
						   //						    system(temp);
						   fd_usb_storage=open(SD_DEVICE,O_RDWR);
						   if(fd_usb_storage<0){
							   show_info(p_vmem,WARNING_USTORE_ERROR);
							   break;
						   }else close(fd_usb_storage);
						   sprintf(temp,"mount -t vfat /dev/sda1 usbkey");
						   system(temp);
						   sprintf(temp,"cp /tmp/bmp_temp.bmp usbkey -f");
						   system(temp);
						   sprintf(temp,"umount usbkey");
						   system(temp);
						   printf("复制ok\n");
						   show_info(p_vmem,WARNING_COPY_OK);
						   //						   sprintf(temp,"bmptopnm /tmp/bmp_temp.bmp > /tmp/bmp_temp.pnm");
						   //						   system(temp);memset(temp,0x00,sizeof(temp));
						   //						   sprintf(temp,"pnmtops /tmp/bmp_temp.pnm > /tmp/bmp_temp.ps");
						   //						   system(temp);
						   Sleep(1,0);
						   //						   if(initialize_serial()!=0){
						   //							   perror("initialize_serial error");
						   //							   return 1;
						   //						   }
						   break;
					   case 1:
						   fd_usb_print=open("/dev/usblp0",O_RDWR);
						   if(fd_usb_print<0){
							   show_info(p_vmem,WARNING_PRINT_ERROR);
							   break;
						   }
						   break;
					   default:break;}
			   }
			   break;
		case 3:if(!VAL(zoomed)){
				   if(VAL(rectify)!=3) show_info(p_vmem,WARNING_PROBE_TEST_RF);
				   else if(VAL(frez)) show_info(p_vmem,WARNING_FREZ);
				   else{ probe_test=!probe_test;
					   if(probe_test==1){
						   //						   close(pt->fd_key);
						   show_info(p_vmem,WARNING_CALING);
						   unsigned short *data_tmp;
						   int i,j,x11 = 0,x12,x21,x22 = 0, xmax_tmp;
						   float xr[2048*4],xi[2048*4];
						   j=5120*(VAL(mtlvel/1000.0))*0.5;
						   if(VAL(range)!=j){
							   VAL(range)=j;
							   senddsp_data();
							   for(i=0;i<7;i++)
								   fetch_data();
						   }
						   data_tmp=(unsigned short *)pt->sram_base;
						   VAL(frez)=1;
						   memset(xi,0x00,sizeof(xi));
						   //						   memset(xr,0x00,sizeof(xr));
						   for(i=0;i<2048*4;i++){
							   xr[i]=256;
						   }
						   for(i=1024+2048-1,j=0;j<2048;i++,j++){
							   xr[i]=data_tmp[j];
							   //xr[i]=sin((i+256)*3.1415826/5)*128;
						   }
						   fft(xr,xi,2048*4,0);
						   for(i=0,xmax_tmp=1;i<400;i++){
							   xr[i]=sqrt(xr[i]*xr[i]+xi[i]*xi[i]);
							   if((xr[i])&&(i<400)){
								   probe_test2[i]=80*log10(xr[i]);
								   probe_test2[i]=(probe_test2[i]>399.0)?399.0:probe_test2[i];
								   probe_test2[i]=(probe_test2[i]<0)?0:probe_test2[i];
							   }
							   if(i!=0){
								   if(((probe_test2[i]<399.0)||(probe_test2[i]>399.0))&&
										   (probe_test2[xmax_tmp]<probe_test2[i])) xmax_tmp=i;
							   }
							   printf("probe_test1[%d]=%f\n",i,probe_test2[i]);
						   }
						   //						   for(j=1;j<80;j++){
						   j=24;
						   ffty_tmp=probe_test2[xmax_tmp]-j;
						   for(i=xmax_tmp;probe_test2[i]>(probe_test2[xmax_tmp]-j);i--)
							   x11=i;
						   x12=x11+1;
						   //						   if(!(probe_test2[x12]-probe_test2[x11]))
						   //								probe_test=0;
						   fftx1_tmp=((float)(probe_test2[xmax_tmp]-j)-(float)((probe_test2[x11]*x12-probe_test2[x12]*x11)))/
							   ((float)(probe_test2[x12]-probe_test2[x11]));
						   for(i=xmax_tmp;probe_test2[i]>(probe_test2[xmax_tmp]-j);i++)
							   x22=i;
						   x21=x22-1;
						   fftx2_tmp=((probe_test2[xmax_tmp]-j)-((probe_test2[x21]*x22-probe_test2[x22]*x21)))/(probe_test2[x22]-probe_test2[x21]);
					   }
				   }
			   }
			   break;
		case 4:if(!VAL(zoomed)){
				   //				   sprintf(temp,"udevstart");
				   //				   system(temp);
				   fd_usb_storage=open(SD_DEVICE,O_RDWR);
				   int fd_usb;
				   unsigned short *data_tmp;
				   data_tmp = (unsigned short *)pt->sram_base;
				   if(fd_usb_storage<0){
					   show_info(p_vmem,WARNING_USTORE_ERROR);
					   break;
				   }else close(fd_usb_storage);
				   sprintf(temp,"mount -t vfat /dev/sda1 usbkey");
				   system(temp);
				   fd_usb=open("usbkey/config.cfg",O_CREAT|O_RDWR);
				   if(fd_usb<0){ 
					   show_info(p_vmem,WARNING_USTORE_ERROR);
					   close(fd_usb);
					   break;
				   }else{
					   //					   write(fd_usb,data_tmp,32*1000*2);
					   write(fd_usb,pt->current_data,960);
					   write(fd_usb,pt->measure_cur_data,sizeof(MEASURE));
					   write(fd_usb,pt->p_config,sizeof(CONFIG));
				   }
				   close(fd_usb);
				   sprintf(temp,"umount usbkey");
				   system(temp);
				   printf("复制ok\n");
				   show_info(p_vmem,WARNING_COPY_OK);
			   }break;
		case 5:
			   VAL(frez) = !VAL(frez);
			   val = VAL(frez);
				ioctl(pt->fd_gpio, POWER_LOW, &val);
			   break;
		default:break;
	}
	if(0 == VAL(zoomed)){
		draw_rim(pt->virtual_video_add,0);
		draw_horizontal_text2(pt->virtual_video_add);
		draw_vertical_text1(pt->virtual_video_add);
	}

	return 0;
}

int senddsp_data()
{
	if(VAL(magnify)){
		if(1==VAL(magnify)){
			*(pt->trans_data+1024*2)=(unsigned long)(((VAL(astart))*2/(VAL(mtlvel)/1000.0)+VAL(p_delay))/10+16384);
			// 			*(pt->trans_data+512)=(unsigned long)32768;
			*(pt->trans_data+1024+1)=VAL(awidth)*2/(VAL(mtlvel)/100.0);
			*(pt->trans_data+1024*2+3*2)=VAL(rectify);
			pt->true_delay=VAL(astart);		
			pt->true_range=VAL(awidth);
		}else{
			*(pt->trans_data+1024*2)=(unsigned long)(((VAL(bstart))*2/(VAL(mtlvel)/1000.0)+VAL(p_delay))/10+16384);
			//	*(pt->trans_data+512*2)=(unsigned long)32768;		
			*(pt->trans_data+1024*2+1)=VAL(bwidth)*2/(VAL(mtlvel)/100.0);
			*(pt->trans_data+1024*2+3*2)=VAL(rectify);
			pt->true_delay=VAL(bstart);
			pt->true_range=VAL(bwidth);
		}
	}else{
		pt->delay_tmp=(unsigned long)((VAL(d_delay)*2/(VAL(mtlvel)/1000.0)+VAL(p_delay))/10+16384);
		*(pt->trans_data+1024*2)=pt->delay_tmp&0xffff;
		*(pt->trans_data+1024*2+1)=(pt->delay_tmp>>16)&0xffff;
		*(pt->trans_data+1024*2+3*2)=VAL(rectify);
		pt->true_delay=VAL(d_delay);
		pt->true_range=VAL(range);
	}

	pt->measure_cur_data->Rs = pt->true_delay;
	pt->measure_cur_data->Re = pt->true_delay + pt->true_range;

	//	if(VAL(power)==1){
	//			*(pt->trans_data+1024*2+4*2)=0x01;
	//	}else{
	//			*(pt->trans_data+1024*2+4*2)=0x00;
	//	}

	cost_time=pt->true_range*2/(VAL(mtlvel)/1000.0);
//	true_dot_num=480;
	if(cost_time<1200){
		pt->dot_quantily=cost_time*0.48;
		*(unsigned long *)(pt->map_base+(1024+5)*4)=0x01;
	}else if(cost_time<64000){
		pt->dot_quantily=cost_time*0.48;
		*(unsigned long *)(pt->map_base+(1024+5)*4)=0x01;
	}else if(cost_time<128000){
		pt->dot_quantily=cost_time*0.24;
		*(unsigned long *)(pt->map_base+(1024+5)*4)=0x02;
	}else if(cost_time<256000){
		pt->dot_quantily=cost_time*0.12;
		*(unsigned long *)(pt->map_base+(1024+5)*4)=0x04;
	}else if(cost_time<480000){
		pt->dot_quantily=cost_time*0.06;
		*(unsigned long *)(pt->map_base+(1024+5)*4)=0x08;
	}else if(cost_time<1000000){
		pt->dot_quantily=cost_time*0.03;
		*(unsigned long *)(pt->map_base+(1024+5)*4)=0x12;
	}else if(cost_time<2000000){
		pt->dot_quantily=cost_time*0.015;
		*(unsigned long *)(pt->map_base+(1024+5)*4)=0x14;
	}else if(cost_time<4000000){
		pt->dot_quantily=cost_time*0.0075;
		*(unsigned long *)(pt->map_base+(1024+5)*4)=0x18;
	}
	Sleep(0,16000);
	return 0;
}

void calculate_measure_data()
{
/*	pt->measure_cur_data->Rs = pt->true_delay;
	pt->measure_cur_data->Re = pt->true_delay + pt->true_range;*/

	if (VAL(alogic))
		pt->Sa_array[pt->cur_data_num] = 
			pt->true_delay + pt->true_range * (gat_max[pt->a_gata_max] * 1.2 / pt->dot_quantily);
	else
		pt->Sa_array[pt->cur_data_num] = 0;

	if (VAL(blogic))
		pt->Sb_array[pt->cur_data_num] =
			pt->true_delay + pt->true_range * (gat_max[pt->b_gata_max] * 1.2/ pt->dot_quantily);
	else 
		pt->Sb_array[pt->cur_data_num] = 0;

	if (pt->cur_data_num >> 3) {
		if (VAL(alogic) && (pt->current_data[pt->a_gata_max] < 380)) {
			pt->measure_cur_data->Sa=
				(pt->Sa_array[8] + pt->Sa_array[1] + pt->Sa_array[2] + pt->Sa_array[3] +
				 pt->Sa_array[4] + pt->Sa_array[5] + pt->Sa_array[5] + pt->Sa_array[7]) >> 3;
			/*闸门A内回波高度占显示高度百分比*/
			pt->measure_cur_data->Ha = org_data[pt->a_gata_max]/4.0;
			/*闸门A位置处高度与闸门内最高回波点的分贝差*/
			pt->measure_cur_data->ha = 20 * 100 * 
				log10((400.0 - pt->current_data[pt->a_gata_max]) / (VAL(athrsh)*4.0));
			if(2001000 == VAL(diamet)) {
				pt->d_tmp = pt->measure_cur_data->Sa * ((float)((float)(COS(VAL(angle)))/10000));
				if (pt->d_tmp < VAL(thickne)) {
					pt->measure_cur_data->la = 0;
					pt->measure_cur_data->Da = pt->d_tmp;
				} else {
					pt->measure_cur_data->la = (pt->d_tmp-VAL(thickne)) / (2 * VAL(thickne)) + 1;
					pt->measure_cur_data->Da = abs(pt->d_tmp - 2 * pt->measure_cur_data->la * VAL(thickne));
				}
				pt->measure_cur_data->Pa = pt->measure_cur_data-> Sa * ((float)((float)(SIN(VAL(angle))) / 10000));
				pt->measure_cur_data->Ra = pt->measure_cur_data-> Pa - VAL(x_value);
			} else {
				float a, b, c;
				a = (float)(pt->measure_cur_data->Sa) / 1000.0;
				b = (float)(VAL(diamet)) / 2000.0;
				c = sqrt(a * a + b * b - 2.0 * a * b * (COS(VAL(angle)) / 10000.0));
				pt->measure_cur_data->la = 0;
				pt->measure_cur_data->Da = (int)((b - c)*1000);
				pt->measure_cur_data->Pa =  1000.0 * b * asin((float)(a/c)* (SIN(VAL(angle)) / 10000.0));
				pt->measure_cur_data->Ra = pt->measure_cur_data-> Pa - VAL(x_value);
				
			}
			if (VAL(dacmode) == 2 && dac_dot_data[pt->a_gata_max])
				pt->measure_cur_data->dB_db=
					10 * 20 * log10((float)((400.0 - pt->current_data[pt->a_gata_max]) /
								(dac_dot_data[pt->a_gata_max]))); 
			else
				pt->measure_cur_data->dB_db=100000;
		} else {
			pt->measure_cur_data->Sa=0;
			pt->measure_cur_data->Ha=128;
			pt->measure_cur_data->ha=0;
			pt->measure_cur_data->la=100000;
			pt->measure_cur_data->Da=10000000;
			pt->measure_cur_data->Pa=0;
			pt->measure_cur_data->Ra=10000000;
			pt->measure_cur_data->dB_db=100000;
		}
		if(VAL(blogic)&&(pt->current_data[pt->b_gata_max]<380)){
			pt->measure_cur_data->Sb=
				(pt->Sb_array[8]+pt->Sb_array[1]+pt->Sb_array[2]+pt->Sb_array[3]+
				 pt->Sb_array[4]+pt->Sb_array[5]+pt->Sb_array[5]+pt->Sb_array[7])>>3;
			pt->measure_cur_data->Hb=org_data[pt->b_gata_max]/4.0;
			pt->measure_cur_data->hb= 20 * 100 *
				log10((400.0 - pt->current_data[pt->b_gata_max]) / (VAL(bthrsh)*4.0));
			pt->d_tmp=pt->measure_cur_data->Sb*((float)((float)(COS(VAL(angle)))/10000));
			if(pt->d_tmp<VAL(thickne)){
				pt->measure_cur_data->lb=0;
				pt->measure_cur_data->Db=pt->d_tmp;
			}else{
				pt->measure_cur_data->lb=(pt->d_tmp-VAL(thickne))/(2*VAL(thickne))+1;
				pt->measure_cur_data->Db=abs(pt->d_tmp-2*pt->measure_cur_data->lb*VAL(thickne));
			}
			pt->measure_cur_data->Pb=pt->measure_cur_data->Sb*((float)((float)(SIN(VAL(angle)))/10000));
			pt->measure_cur_data->Rb=pt->measure_cur_data->Pb-VAL(x_value);
		}
		else{
			pt->measure_cur_data->Sb=0;
			pt->measure_cur_data->Hb=128;
			pt->measure_cur_data->hb=0;
			pt->measure_cur_data->lb=100000;
			pt->measure_cur_data->Db=10000000;
			pt->measure_cur_data->Pb=0;
			pt->measure_cur_data->Rb=10000000;			
		}
		if(VAL(blogic)&&VAL(blogic)&&pt->a_gata_max!=0&&(pt->a_gata_max!=400)&&pt->b_gata_max!=0&&
				(pt->b_gata_max!=400)&&(pt->current_data[pt->a_gata_max]<380)&&(pt->current_data[pt->b_gata_max]<380))
		{
			pt->measure_cur_data->ba=
				MAX(pt->measure_cur_data->Sb,pt->measure_cur_data->Sa)-MIN(pt->measure_cur_data->Sb,pt->measure_cur_data->Sa);
		}
		else{
			pt->measure_cur_data->ba=0;
		}
	}
	return ;
}

int show_info(COLOR_TYPE *p_vmem,int a)
{
	switch(a){
		case WARNING_FREZ:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_MAGNIFY:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_RF:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_POWERH:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_POWERL:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_GATE:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_DAC:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_DACECHO:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_HPOWER:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_RF1:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_PRINT:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_CAL_OK:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_CAL_ERROR:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_DAC_MTL:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_PROBE_TEST_RF:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_PRINT_ERROR:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_USTORE_ERROR:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_CALING:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_COPY_OK:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_PRINT_OK:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_UPDATING:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_UPDATE_OK:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		case WARNING_NAVIGATION:
			show_bmpmem(pt->true_video_add,bmp_warn+(a*2+VAL(lang))*W_SIZE,410,1,X_5,Y_5);break;

		default:break;
	}
	Sleep(0,2000000);
	if (VAL(hor_pos)[5])
		draw_horizontal_text2(p_vmem);
	else
		draw_horizontal_text1(p_vmem);
	return 0;
}


int cal_range_add(int start,int *a,int *b,int g)
{
	if(start>=a[0]&&start<a[1])	return b[0]*g;
	if(start>=a[1]&&start<a[2])	return b[1]*g;
	if(start>=a[2]&&start<a[3])	return b[2]*g;
	if(start>=a[3]&&start<a[4])	return b[3]*g;
	if(start>=a[4]&&start<a[5])	return b[4]*g;
	if(start>=a[5]&&start<a[6])	return b[5]*g;
	if(start>=a[6]&&start<a[7])	return b[6]*g;
	if(start>=a[7]&&start<a[8])	return b[7]*g;
	if(start>=a[8]&&start<a[9])	return b[8]*g;
	if(start>=a[9]&&start<=a[10])	return b[9]*g;
	return 0;
}

int cal_range_sub(int start,int *a,int *b,int g)
{
	if(start>=a[0]&&start<=a[1])	return b[0]*g;
	if(start>a[1]&&start<=a[2])	return b[1]*g;
	if(start>a[2]&&start<=a[3])	return b[2]*g;
	if(start>a[3]&&start<=a[4])	return b[3]*g;
	if(start>a[4]&&start<=a[5])	return b[4]*g;
	if(start>a[5]&&start<=a[6])	return b[5]*g;
	if(start>a[6]&&start<=a[7])	return b[6]*g;
	if(start>a[7]&&start<=a[8])	return b[7]*g;
	if(start>a[8]&&start<=a[9])	return b[8]*g;
	if(start>a[9]&&start<=a[10])	return b[9]*g;
	return 0;
}

int value_change(int a,int *b,int *c,int d,int e,int f,int g)
	//各个数值增减 a  操作数,b增长间隔,c步长数值,d,e为范围,f=1为增加,2为减少g=0为粗调,123为精调
{
	if(!g){
		int temp=0;
		while(c[temp]<a)	temp++;
		if((f==1)&&(a!=d)){
			a=c[temp+1];
		}
		else if((f==2)&&(a!=e)){
			a=c[temp-1];
		}
	}
	else{
		if((f==1)&&(a!=d)){
			a+=cal_range_add(a,b,c,pow(10,(g-1)));
			if(a>=d) a=d;
		}
		else if((f==2)&&(a!=e)){
			a-=cal_range_sub(a,b,c,pow(10,(g-1)));
			if(a<=e) a=e;
		}
	}
	return a;
}

int read_i2c(int fd,unsigned char *add,unsigned char *data,int count)
{
	if(write(fd,add,2)!=2) return 1;
	if(read(fd,data,count)<0) return 2;
	Sleep(0,100);
	return 0;
}	

int write_i2c(int fd,unsigned char *add,unsigned char *data,int count)
{
	char tmp[count+2];
	memcpy(tmp,add,2);
	memcpy(tmp+2,data,count);
	if(write(fd,tmp,count+2)<0) return 1;
	Sleep(0,100);
	return 0;
}

int display_value(COLOR_TYPE *p_vmem,int measure_pos,int x,int y,int zoom_x,int zoom_y,int col)
{
	unsigned char bcol,gcol;
	float temp_val = ((VAL(unit) == 1) ? 1.0 : 25.4);
	char temp_dis[12];
	//	if(col==1) bcol=OTHER_COL,gcol=17;
	bcol = WAVE_BACK, gcol = WAVE_GROUND;
	memset(temp_dis, 0x00, sizeof(temp_dis));

	switch (measure_pos) {
		case 0:break;
		case 1: 
			   if(0 == pt->measure_cur_data->Sa) break;
			   sprintf(temp_dis,"%.2f",((float)(pt->measure_cur_data->Sa))/(temp_val*1000));
			   break;

		case 2: 
			   if(0 == pt->measure_cur_data->Sb) break;			
			   sprintf(temp_dis,"%.2f",((float)(pt->measure_cur_data->Sb))/(temp_val*1000));
			   break;

		case 3:if(0==pt->measure_cur_data->Sb||0==pt->measure_cur_data->Sa) break;
				   sprintf(temp_dis,"%.2f",((float)(pt->measure_cur_data->ba))/(temp_val*1000));break;

		case 4: if (128 == pt->measure_cur_data->Ha) {
					break ;
				}else if(125 <= pt->measure_cur_data->Ha) {
					sprintf(temp_dis, "overflow");
					break;
				}
				sprintf(temp_dis,"%d",pt->measure_cur_data->Ha);break;

		case 5: if (128 == pt->measure_cur_data->Ha) {
					break;
				}else if (125 <= pt->measure_cur_data->Hb) {
					sprintf(temp_dis, "overflow");
					break;
				}
				sprintf(temp_dis,"%d",pt->measure_cur_data->Hb);break;

		case 6: if(0==pt->measure_cur_data->ha) break;
					sprintf(temp_dis,"%.1f",pt->measure_cur_data->ha/100.0);break;

		case 7: if(0==pt->measure_cur_data->hb) break;
					sprintf(temp_dis,"%.1f",pt->measure_cur_data->hb/100.0);break;

		case 8:	sprintf(temp_dis,"%.2f",((float)(pt->measure_cur_data->Rs))/(temp_val*1000));break;

		case 9: sprintf(temp_dis,"%.2f",((float)(pt->measure_cur_data->Re))/(temp_val*1000));break;

		case 10:if(100000==pt->measure_cur_data->la) break;
					sprintf(temp_dis,"%d",pt->measure_cur_data->la);break;

		case 11:if(100000==pt->measure_cur_data->lb) break;
					sprintf(temp_dis,"%d",pt->measure_cur_data->lb);break;

		case 12:if(10000000==pt->measure_cur_data->Da) break;
					sprintf(temp_dis,"%.2f",((float)(pt->measure_cur_data->Da))/(temp_val*1000));break;

		case 13:if(10000000==pt->measure_cur_data->Db) break;
					sprintf(temp_dis,"%.2f",((float)(pt->measure_cur_data->Db))/(temp_val*1000));break;

		case 14:if(0==pt->measure_cur_data->Pa) break;
					sprintf(temp_dis,"%.2f",((float)(pt->measure_cur_data->Pa))/(temp_val*1000));break;

		case 15:if(0==pt->measure_cur_data->Pb) break;
					sprintf(temp_dis,"%.2f",((float)(pt->measure_cur_data->Pb))/(temp_val*1000));break;

		case 16:if(10000000==pt->measure_cur_data->Ra) break;
					sprintf(temp_dis,"%.2f",((float)(pt->measure_cur_data->Ra))/(temp_val*1000));break;

		case 17:if(10000000==pt->measure_cur_data->Rb) break;
					sprintf(temp_dis,"%.2f",((float)(pt->measure_cur_data->Rb))/(temp_val*1000));break;

		case 18:
				break;
		case 19:
				break;
		case 20:if(100000==pt->measure_cur_data->dB_db) break;
					sprintf(temp_dis,"%d",pt->measure_cur_data->dB_db / 10);break;

		case 66:
				break;
		case 67:
				break;
		default:break;
	}

	if (strlen(temp_dis) != 0) {
		if(col == 2) fbrec(p_vmem,x,y+4,strlen(temp_dis)*8*zoom_x,32,bcol);
		draw_str(p_vmem,temp_dis,x,y,zoom_x,zoom_y,gcol);
	}
	return 0;
}


int draw_dac_line(COLOR_TYPE *p_vmem,unsigned int *dac_data,int col)
{
	int k, bit = !(pt->hanfeng == 0);
	unsigned int *dac_data_tmp=dac_data, data_tmp1[600];
	for (k = 0; k < 400; k++) {
		data_tmp1[k]=((400-(unsigned short)(dac_data_tmp[k]))>0?(400-(unsigned short)(dac_data_tmp[k])):0);
	}
	if(0==VAL(zoomed)){
		for(k=0;k<400-1;k++){
			fbline(p_vmem,k,data_tmp1[k]>>bit,k+1,data_tmp1[k+1]>>bit,col);
		}
	}else{
		for(k=0;k<200;k++){
			data_tmp1[k*3+0]=dac_data_tmp[2*k]*9/8;
			data_tmp1[k*3+2]=dac_data_tmp[2*k+1]*9/8;
			data_tmp1[k*3+1]=(dac_data_tmp[2*k]+dac_data_tmp[2*k+1])/2;
		}	
		for(k=0;k<600-1;k++){
			fbline(p_vmem,k,data_tmp1[k]>>bit,k+1,data_tmp1[k+1]>>bit,col);
		}
	}
	return 0;
}

void cal_dac_data1()
{
	int i, j, l;
	unsigned int *p_tmp;
	for (l = 0; l < 4; l++) {
		for (i = 0 ; i < VAL(dacecho); i++)	
			dac_x[i] = ((VAL(dac_data_x)[i] - VAL(d_delay)) * 400.0) / VAL(range);
		if (l == 0)	{
			p_tmp = dac_dot_data;
			for (i = 0 ; i < VAL(dacecho); i++)	
				dac_y[i] = VAL(dac_data_y)[i] / db_table[(VAL(rec) * 5)];		
		} else {
			for (i = 0 ; i < VAL(dacecho); i++)	{
				if (VAL(off_db)[l - 1] >= 0)
					dac_y[i] = (float)((VAL(dac_data_y)[i]) / db_table[(VAL(rec)  * 5)] * 
							db_table[VAL(off_db)[l - 1]]);
				else if (VAL(off_db)[l - 1] < 0) 
					dac_y[i] = (float)((VAL(dac_data_y)[i]) / db_table[(VAL(rec)  * 5)] /
							db_table[-VAL(off_db)[l - 1]]);
			}
			if (l == 1) p_tmp = dac_dot_offdata1;
			else if (l == 2) p_tmp = dac_dot_offdata2;
			else if (l == 3) p_tmp = dac_dot_offdata3;
		}

		if (VAL(dacecho) > 1) {
			for (i = 0; i < VAL(dacecho) - 1; i++) {
				if (dac_x[i] > 399) break;

				for (j = dac_x[i] ; (j < dac_x[i + 1]) && (j < 400); j++) {
					p_tmp[j] = ((float)((j-dac_x[i + 1]))/(dac_x[i]-dac_x[i + 1]))*dac_y[i] + 
						((float)((j-dac_x[i]))/(dac_x[i + 1]-dac_x[i]))*dac_y[i + 1];
				}
			}
			for (i = 0; (i < dac_x[0]) && (i < 400); i++)
				p_tmp[i] = dac_y[0];
			for (i = dac_x[VAL(dacecho) -1]; i < 400; i++)
				p_tmp[i] = dac_y[VAL(dacecho) -1];
		}
	}
}

/*恢复出厂默认配置函数*/
void recall_set_config()
{
	const char config[] = {
		0xa0,0x86,0x01,0x00,0x70,0x17,0x00,0x00,0x00,0x00,0x00,0x00,0x8c,0x19,0x00,0x00,
		0xe0,0x2e,0x00,0x00,0xb8,0x88,0x00,0x00,0xb0,0x36,0x00,0x00,0x10,0x27,0x00,0x00,
		0xa0,0x86,0x01,0x00,0x40,0x0d,0x03,0x00,0xab,0x02,0x00,0x00,0xc0,0x2b,0x00,0x00,
		0xb0,0xad,0x01,0x00,0x68,0x88,0x1e,0x00,0xfb,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
		0x01,0x18,0x00,0x32,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x0f,0x00,0x00,0x01,
		0x00,0x01,0x02,0x02,0x00,0x03,0x00,0x01,0x00,0x01,0x56,0x01,0x00,0x00,0x00,0x02,
		0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x02,0x01,0x01,0x00,0x73,0xbb,0x00,0x00,
		0x96,0x73,0x01,0x00,0x8c,0x48,0x02,0x00,0x00,0x00,0x00,0x00,0x2c,0x55,0x05,0x00,
		0x00,0x00,0x48,0x43,0x00,0x00,0x16,0x43,0x00,0x00,0xc8,0x42,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x25,0x4e,0x00,0x00,0xe8,0x03,0x00,0x00,0xe8,0x03,0x00,0x00,0x05,0x00,0x14,0x00,
		0xe7,0xff,0x01,0x00,0x01,0x04,0x14,0x0d,0x00,0x00,0x19,0x00,0xb8,0x0b,0x00,0x00,
		0x25,0x4e,0x00,0x00,0x04,0x29,0x00,0x00,0x7c,0x15,0x00,0x00,0x02,0x01,0x01,0x02,
		0x99,0xbe,0x00,0x00,0xfb,0x7e,0x86,0x42,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00};
	memcpy(pt->p_config,config,sizeof(CONFIG));
	i2c_add[0] = 0;
	i2c_add[1] = 0;
	t36((void*)(NULL),0);t37((void*)(NULL),0);t38((void*)(NULL),0);t39((void*)(NULL),0);
	t56((void*)(NULL),0);t57((void*)(NULL),0);t58((void*)(NULL),0);t59((void*)(NULL),0);
	t76((void*)(NULL),0);t77((void*)(NULL),0);t78((void*)(NULL),0);t79((void*)(NULL),0);
	if(write_i2c(pt->fd_config, i2c_add, pt->p_config, 32)) ;i2c_add[1]=32;
	if(write_i2c(pt->fd_config, i2c_add, pt->p_config+i2c_add[1], 32)) ; i2c_add[1]=64;
	if(write_i2c(pt->fd_config, i2c_add, pt->p_config+i2c_add[1], 32)) ; i2c_add[1]=96;
	if(write_i2c(pt->fd_config, i2c_add, pt->p_config+i2c_add[1], 32)) ; i2c_add[1]=128;
	if(write_i2c(pt->fd_config, i2c_add, pt->p_config+i2c_add[1], 32)) ; i2c_add[1]=160;
	if(write_i2c(pt->fd_config, i2c_add, pt->p_config+i2c_add[1], 32)) ; i2c_add[1]=192;
	if(write_i2c(pt->fd_config, i2c_add, pt->p_config+i2c_add[1], 32)) ; i2c_add[1]=0;

}
