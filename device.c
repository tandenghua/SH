#include "device.h"
#include "fbdraw.h"
#include "draw_rim.h"
#include <linux/spi/spidev.h>

unsigned char	i2c_add[2] = {0x00, 0x00};	/*写i2c 连接的EEROM 用到的地址*/
signed short	temp_db = 0;				/*临时增益*/
unsigned int	refresh_count = 0;			/*计数器 被8整除时 计算测量值*/
unsigned int	module_symbol = 0;			/*加载模块标志0 未加载 1已加载*/
float			cal_time[2];				/*保存校准时间*/
float			cost_time;					/*保存采集波形的时间*/
float			fftx1_tmp,fftx2_tmp,ffty_tmp; /*fft 最中心频率位置*/
unsigned int	agatopen, bgatopen, probe_test = 0;/*记录闸门A B 都开关状态,探头测试状态 */

COLOR_TYPE *bmp = bmp1;						/*指向当前图标bmp1或者bmp2*/
COLOR_TYPE bmp1[V_SIZE*208];				/*保存图标数据的数组*/
COLOR_TYPE bmp2[V_SIZE*208];				/*保存图标数据的数组(英文)*/
COLOR_TYPE bmp_content[V_SIZE*140];			/*保存子菜单内容*/
COLOR_TYPE bmp_else[V_SIZE*10];				/*其他栏图标*/
COLOR_TYPE bmp_warn[W_SIZE*40*2];			/*警告图标*/

P_DPL_CONFIG pt;

unsigned short	envelope_data[640];/*包络数据组*/
unsigned short	gat_max[480];
float			probe_test2[400];
/*current_data1为放大时数据*/
unsigned short	org_data[512], current_data1[640];

int dac_x[20];
float dac_y[20];


/*颜色表 用来切换各种显示颜色*/
static u16 red1[]  = {
	0x0000,0x7ed3,0x51a5,0x3523,0x1061,0x38e0,0x7ed3,0x624a,
	0x3523,0x1061,0x38e0,0x7ed3,0x666a,0x3523,0x6d00,0x1061,
};
static u16 red2[]  = {
	0x000,0x7ed3,0x51a5,0x3523,0x1061,0x38e0,0x7ed3,0x624a,
	0x3523,0x1061,0x38e0,0x7ed3,0x666a,0x3523,0x6d00,0x1061,
};
static u16 red3[]  = {
	0x0000,0x7ed3,0x51a5,0x3523,0x1061,0x38e0,0x7ed3,0x624a,
	0x3523,0x1061,0x38e0,0x7ed3,0x666a,0x3523,0x6d00,0x1061,
};
static u16 red4[]  = {
	0x0000,0x5ba9,0x2e40,0x08c0,0x0040,0x08c0,0x5ba9,0x2ea0,
	0x08c0,0x0040,0x08c0,0x5ba9,0x2e40,0x08c0,0x0900,0x0040,
};
static u16 red5[]  = {
	0x0000,0x5ba9,0x2e40,0x08c0,0x0040,0x08c0,0x5ba9,0x2ea0,
	0x08c0,0x0040,0x08c0,0x5ba9,0x2e40,0x08c0,0x0900,0x0040,
};
static u16 red6[]  = {
	0x0000,0x5ba9,0x2e40,0x08c0,0x0040,0x08c0,0x5ba9,0x2ea0,
	0x08c0,0x0040,0x08c0,0x5ba9,0x2e40,0x08c0,0x0900,0x0040,
};

struct fb_cmap tt_cmap0 = {
	.len = 16, .red = red1, .green = red2, .blue = red3
};
struct fb_cmap tt_cmap1 = {
	.len = 16, .red = red2, .green = red2, .blue = red3
};
struct fb_cmap tt_cmap2 = {
	.len = 16, .red = red3, .green = red2, .blue = red3
};
struct fb_cmap tt_cmap3 = {
	.len = 16, .red = red4, .green = red2, .blue = red3
};
struct fb_cmap tt_cmap4 = {
	.len = 16, .red = red5, .green = red2, .blue = red3
};
struct fb_cmap tt_cmap5 = {
	.len = 16, .red = red6, .green = red2, .blue = red3
};

/*延时函数,iSec 为秒数,iUsec为微秒数*/
void Sleep(int iSec,int	iUSec)  
{  
	struct timeval	tv;
	tv.tv_sec =	iSec;
	tv.tv_usec = iUSec;
	select(0,NULL,NULL,NULL,&tv);
}

/*改变颜色*/
void change_col()
{
	switch (VAL(scheme)) {
		case 0:
			ioctl(pt->fd_fb, FBIOPUTCMAP, &tt_cmap0);break;
		case 1:
			ioctl(pt->fd_fb, FBIOPUTCMAP, &tt_cmap1);break;
		case 2:
			ioctl(pt->fd_fb, FBIOPUTCMAP, &tt_cmap2);break;
		case 3:
			ioctl(pt->fd_fb, FBIOPUTCMAP, &tt_cmap3);break;
		case 4:
			ioctl(pt->fd_fb, FBIOPUTCMAP, &tt_cmap4);break;
		case 5:
			ioctl(pt->fd_fb, FBIOPUTCMAP, &tt_cmap5);break;
		default:
			break;
	}
}

/*把图标信息从文件中读到数组中 open后没检查是否成功*/
void bmpfile2mem()
{
	int fd;
	fd = open("/bmpdata/bmp.data", O_RDONLY);
	read(fd, bmp1, V_SIZE * 208);
	close(fd);
	fd = open("/bmpdata/bmp_en.data", O_RDONLY);
	read(fd, bmp2, V_SIZE * 208);
	close(fd);
	fd = open("/bmpdata/bmp_content.data", O_RDONLY);
	read(fd, bmp_content, V_SIZE*140);
	close(fd);
	fd = open("/bmpdata/bmp_else.data", O_RDONLY);
	read(fd, bmp_else, V_SIZE * 10);
	close(fd);
	fd = open("/bmpdata/bmp_warning.data", O_RDONLY);
	read(fd, bmp_warn, W_SIZE * 40 * 2);
	close(fd);
}

int initialize_serial()
{
	struct termios newtermios;
	pt->fd_key = open(TTY_DEVICE, O_RDWR | O_NOCTTY );
	if (pt->fd_key < 0) {
		perror(TTY_DEVICE); 
		return 1;
	}
	bzero(&newtermios, sizeof(newtermios)); /* 清除结构体以放入新的序列埠设定值 */
	tcgetattr(pt->fd_key, &newtermios);
	newtermios.c_cc[VMIN] = 0;
	newtermios.c_iflag = 0;
	newtermios.c_oflag = 0;
	newtermios.c_lflag = 0; 
	newtermios.c_cflag	= B115200 | CS8 | PARENB | CLOCAL | CREAD;
	if (tcsetattr(pt->fd_key, TCSANOW, &newtermios)) {
		perror("tcsetattr   error");  
		exit(1);  
	}
	return 0;
}

int mem_init()
{
	/**/
	int fd_mem;
	if ((fd_mem = open(MEM_DEVICE, O_RDWR | O_SYNC)) == -1) {
		perror(MEM_DEVICE);
		return 1;
	}

	if((pt = (P_DPL_CONFIG)malloc(sizeof(DPL_CONFIG))) <= 0) {
		perror("malloc error");
		return 1;
	}

	memset(pt, 0x00, sizeof(DPL_CONFIG));

	pt->map_base = (unsigned long int)
		mmap(0, 256 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd_mem, 0x70000000);

	pt->sram_base = (unsigned long int)
		mmap(0, 80 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd_mem, 0x00300000);

	pt->trans_data = (unsigned short *)pt->map_base;

	memset(envelope_data, 0xff, sizeof(envelope_data));

	if ((pt->virtual_video_add = (COLOR_TYPE *)malloc(VIDEO_RAM)) <= 0)	
		return 1;/*申请的内存做临时画图用*/
	if ((pt->virtual_video_add1 = (COLOR_TYPE *)malloc(VIDEO_RAM)) <= 0)	
		return 1;/*申请的内存做临时画图用*/

	pt->current_data		=	(unsigned short *)malloc(2048);	
	pt->measure_cur_data	=	(MEASURE_P)malloc(sizeof(MEASURE));
	pt->config_current		=	(CONFIG_P)malloc(sizeof(CONFIG));
	pt->p_config			=	(unsigned char *)pt->config_current;
	/*把current measuren_cur_data config_current 指向 sram 地址*/
//	pt->current_data		=	(unsigned short *)(pt->sram_base + 66 * 1024);  
//	pt->measure_cur_data	=	(MEASURE_P)(pt->sram_base + 67 * 1024);
//	pt->config_current		=	(CONFIG_P)(pt->sram_base + 67 * 1024 + sizeof(MEASURE));
//	pt->p_config			=	(COLOR_TYPE*)pt->config_current;
//	pt->usart2_data			=	(unsigned int *)(pt->sram_base + 65*1024);

	//	memset(pt->p_config, 0x00, sizeof(CONFIG));
	//	memset((void *)pt->measure_cur_data, 0x00, sizeof(MEASURE));
	//	memset(dac_dot_data, 0x00, sizeof(dac_dot_data));
	memset(pt->current_data, 0x00, 2048);

	pt->save_data_p = (unsigned short *)malloc(3*1024*1024*sizeof(char));
	return 0;
}

int i2c_init()
{
	/*open I2C 设备*/
	if ((pt->fd_config = open(I2C_DEVICE, O_RDWR)) < 0) {
		perror(I2C_DEVICE);
		return 1;
	}
	/*设定 I2C 设备总线地址*/
	if ((ioctl(pt->fd_config, I2C_SLAVE, 0x53)) < 0) {
		perror("ioctl i2c address error!");
		return 1;
	}

	if (-1 == (read_i2c(pt->fd_config, i2c_add, pt->p_config, 224)))
		return 1;/*读取默认配置文件*/

	i2c_add[1] = 228;
	/*开机设置默认选择为第一主菜单第一附属菜单*/
	write_i2c(pt->fd_config, i2c_add, pt->p_config + 228, 12);
	i2c_add[1] = 240;
	/*开机设置默认选择为第一主菜单第一附属菜单*/
	write_i2c(pt->fd_config, i2c_add, pt->p_config  +240, 16);

	return 0;
}

int gpio_init()
{
	/*打开GPIO设备接口 阻尼 功率理等 */
	if ((pt->fd_gpio = open(OUR_DEVICE, O_RDWR)) < 0) {
		perror(OUR_DEVICE);
		return 1;
	}
	return 0;
}

int spi_init()
{
	int val = 0x01;
	const unsigned char data_spi3_init1[] = {0x90, 0xB5},
		  data_spi3_init2[] = {0x91, 0x16},
		  data_spi3_init3[] = {0x92, 0x16};
	/*spi0 1设备 控制DAC*/
	if ((pt->fd_spi = open(DAC_DEVICE, O_RDWR)) == -1) {
		perror(DAC_DEVICE);
		return 1;
	}

	if (ioctl(pt->fd_spi, SPI_IOC_WR_MODE, &val) == -1) {
		perror("ioctl spi write  error1\n");
		return 1;
	}
	/*spi0 3设备 初始化skyworks*/
	if ((pt->spi_sky = open(SKY_DEVICE, O_RDWR)) == -1) {
		perror(SKY_DEVICE);
		return 1;
	}

	if (ioctl(pt->spi_sky, SPI_IOC_RD_MODE, &val) == -1) {
		perror("ioctl spi read error2\n");
		return 1;
	}

	val = (val & 0xfc) | 0x00;
	if (ioctl(pt->spi_sky, SPI_IOC_WR_MODE, &val) == -1) {
		perror("ioctl spi write error3\n");
		return 1;
	}
	if (ioctl(pt->spi_sky, SPI_IOC_RD_MODE, &val) == -1) {
		perror("ioctl spi read error4\n");
		return 1;
	}

	/*初始Skyworks*/
	ioctl(pt->fd_gpio, SK_LOW, &val);

	write(pt->spi_sky, data_spi3_init1, 2);
	write(pt->spi_sky, data_spi3_init2, 2);
	write(pt->spi_sky, data_spi3_init3, 2);
	return 0;
}

int fb_init()
{
	int *fbmem;
	/*打开framebuffer设备*/
	if ((pt->fd_fb = open(FB_DEVICE, O_RDWR)) == -1) {
		perror(FB_DEVICE);
		return 1;
	}
	fbmem= 	mmap(0, 640 * 480 * 2, PROT_READ | PROT_WRITE, MAP_SHARED, pt->fd_fb, 0);
	pt->true_video_add = (COLOR_TYPE *)(fbmem);
//	memset (fbmem, 0x00, 640*480*2);
//	*(pt->true_video_add + 640 * 1 + 1) = 0xf800;
	/*初始化颜色*/
//	change_col();
	return 0;
}

int store_init()
{
	/*打开存储设备*/
	if ((pt->fd_store = open(MTD4_DEVICE, O_RDWR)) < 0) {
		perror(MTD4_DEVICE);
		return 1;
	}
	return 0;
}

int config_init()
{
	const unsigned char str_init_sky[2] = {0x5a, 0x5a};
	unsigned char key = 0;
	int i, val;
	/*初始化 增益  阻尼 功率 双晶 在skyworks 之后*/
	switch (VAL(damping)) {
		case 0:
			ioctl(pt->fd_gpio, DAMPING_4, &val);break;
		case 1:
			ioctl(pt->fd_gpio, DAMPING_3, &val);break;
		case 2:
			ioctl(pt->fd_gpio, DAMPING_2, &val);break;
		case 3:
			ioctl(pt->fd_gpio, DAMPING_1, &val);break;
		default:
			break;
	}
	write_db_val(VAL(db), 1200); /*初始化增益*/

	if (VAL(dual) == 1) 
		ioctl(pt->fd_gpio, DUAL_ON, &val);
	else
		ioctl(pt->fd_gpio, DUAL_OFF, &val);

	if (VAL(power) == 1)
		ioctl(pt->fd_gpio, POWER_HIGH, &val);
	else
		ioctl(pt->fd_gpio, POWER_LOW, &val);

	if (initialize_serial() != 0) {
		perror("initialize_serial error");
		return 1;
	}

	val = 0;
	ioctl(pt->fd_gpio, FREEZE, &val);

	/*等待FPGA初始化 tandenghua*/
	write(pt->fd_key, str_init_sky, 1);
	//	while (key != 0x5a) {
	//		if (read(pt->fd_key, &key, 1) > 0);
	//	}
	/*果用户在开机时,在按KEY_DB_REG键就恢复出厂默认设置*/
	for (i = 0; i < 100; i++) {
		if (read(pt->fd_key, &key, 1) > 0 ) {	
			if (key == KEY_DB_REG) {	
				//运行恢复默认配置函数
				recall_set_config();
				break;
			}
		}
		Sleep(0, 100);
	}
	if (VAL(power) == 1) 
		*(pt->trans_data + 1024*2 + 4*2) = 0x01;
	else 
		*(pt->trans_data + 1024*2 + 4*2) = 0x00;

	/*液晶屏 亮度*/
	switch (VAL(light)) {
		case 2:
			write(pt->fd_key, key_light_high, 1);	break;
		case 1:
			write(pt->fd_key, key_light_mid, 1);	break;
		case 0:
			write(pt->fd_key, key_light_low, 1);	break;
		default:
			break;
	}

	/*语言设置*/
	if (VAL(lang) == 1)
		bmp = bmp2; 
	else
		bmp = bmp1;

	/*初始化范围 延时 检波*/	
	pt->trans_data = (unsigned short *) pt->map_base;
	if (1 == VAL(magnify)) {
		pt->delay_tmp=(unsigned long)((VAL(astart)*2/(VAL(mtlvel)/1000.0)+VAL(p_delay))/10+16384);
		*(pt->trans_data+1024*2)=pt->delay_tmp&0xffff;
		*(pt->trans_data+1024*2+1)=(pt->delay_tmp>>16)&0xffff;
		*(pt->trans_data+1024*2+3*2)=VAL(rectify);
		pt->true_delay=VAL(astart);
		pt->true_range=VAL(awidth);
	} else if (0 == VAL(magnify)) {
		pt->delay_tmp=(unsigned long)((VAL(d_delay)*2/(VAL(mtlvel)/1000.0)+VAL(p_delay))/10+16384);
		*(pt->trans_data+1024*2)=pt->delay_tmp&0xffff;
		*(pt->trans_data+1024*2+1)=(pt->delay_tmp>>16)&0xffff;
		*(pt->trans_data+1024*2+3*2)=VAL(rectify);
		pt->true_delay=VAL(d_delay);
		pt->true_range=VAL(range);
	}

	if (test_val() != 1) {
		VAL(range) = 100*1000;
		VAL(mtlvel) = 5920;
		VAL(d_delay) = 0;
		VAL(p_delay) = 0;
	}
	senddsp_data();

	/*射频时候 闸门保持关*/
	agatopen = VAL(alogic);
	bgatopen = VAL(blogic);
	if (VAL(rectify) == 3)
		VAL(alogic) = 0, VAL(blogic) = 0;

	/*读取图标*/
	bmpfile2mem();
//	memcpy(offset1, bmp1 + 80*160*50, 160*50*6);
	memcpy(offset2, bmp1 + 192*160*50, 160*50*6);
//	memcpy(offset1 + 160*50*6, bmp2 + 80*160*50, 160*50*6);
	memcpy(offset2 + 160*50*6, bmp2 + 192*160*50, 160*50*6);
	memcpy(offset3, bmp1 + 66*160*50, 160*50*4);
	memcpy(offset4, bmp1 + 204*160*50, 160*50*4);
	memcpy(offset3 + 160*50*4, bmp2 + 66*160*50, 160*50*4);
	memcpy(offset4 + 160*50*4, bmp2 + 204*160*50, 160*50*4);
	memcpy(offset5, bmp1 + 120*160*50, 160*50*2);
	memcpy(offset5 + 160*50*2, bmp1 + 198*160*50, 160*50*2);
	memcpy(offset5 + 160*50*4, bmp2 + 120*160*50, 160*50*2);
	memcpy(offset5 + 160*50*6, bmp2 + 198*160*50, 160*50*2);

	/*读取时间*/
	p[3][3][0](pt->virtual_video_add, 0);
	p[3][3][1](pt->virtual_video_add, 0);

	p[0][2][0](pt->virtual_video_add, 0);
	p[0][1][3](pt->virtual_video_add, 0);

	return 0;
}

int loop_tt()
{
	unsigned char key = 0;
	unsigned long val;
	int fd, len, i, j;
	char a[70];
	unsigned short b[76*63];
	unsigned short c[76*63];
	fd = open("/b1.bmp", O_RDONLY);
	len = read(fd, a, 70);
	len = read(fd, b, 76*63*2);

	for (i = 0; i < 63; i++)
		for (j = 0 ; j < 76; j++)
		{
			c[76*(62 - i) + j] = b[76*i +j] ;

		}
//	memcpy (pt->true_video_add +  640 * (49 - i), b + 160 * i, 160*2);
//	memcpy (pt->true_video_add, c, 640*480*2);

	draw_rim (pt->true_video_add, 0xf79e);
//	draw_horizontal_text1(pt->virtual_video_add);
//	draw_vertical_text1(pt->virtual_video_add);


	show_bmpmem (pt->true_video_add, c, 0, 0, 76, 63);
	show_bmpmem (pt->true_video_add, c, 0, 63, 76, 63);
	show_bmpmem (pt->true_video_add, c, 0, 63*2, 76, 63);
	show_bmpmem (pt->true_video_add, c, 0, 63*3, 76, 63);
	show_bmpmem (pt->true_video_add, c, 0, 63*4, 76, 63);

//	show_bmpmem (pt->true_video_add, c, 0, 437, 92, 43);
//	show_bmpmem (pt->true_video_add, c, 92, 437, 92, 43);
//	show_bmpmem (pt->true_video_add, c, 184, 437, 92, 43);
//	show_bmpmem (pt->true_video_add, c, 276, 437, 92, 43);
//	show_bmpmem (pt->true_video_add, c, 368, 437, 92, 43);
//	show_bmpmem (pt->true_video_add, c, 460, 437, 92, 43);
//	show_bmpmem1 (pt->true_video_add, c, 552, 437, 92, 43, 88, 43);
	return 0;

//	if (VAL(dacecho) && VAL(dacmode)) cal_dac_data1();

	/*	pt->play_mark = 0;*/
	pt->fps = 10;
	pt->cur_data_num = 1;

	//	senddsp_data();

	while (1) {/*死循环读取串口发过来的值*/
#if 0
		if (read(pt->fd_key, &key, 1) > 0 ) {

			/*printf("key = %x\n", key);*/
			/***/
			close(pt->fd_key);
			/*切换到缩放 打印 复制 探头检测项*/
			if (((key == KEY_ELSE) || (key == 0x0c)) && (0 == VAL(zoomed))) {
				draw_horizontal_text2(pt->virtual_video_add);
				VAL(hor_pos)[5] = 1;
				key = 0;
			} else {
				switch (key & 0xff) {
					case 0:
						key = 0;	break;
						/*电池电量*/
					case 0x01:
					case 0x02:
					case 0x03:
					case 0x04:
					case 0x05:
					case 0x06:
						battery_state = key;
						key=0;	break;					
						/*次菜单按键切换*/
					case KEY13:
					case KEY12:
					case KEY11:
					case KEY10:
						if (VAL(zoomed) == 1) break;
						if (KEY13 == key) key = 0;
						else if (KEY12 == key) key = 1;
						else if (KEY11 == key) key = 2;
						else if (KEY10 == key) key = 3;
						if (VAL(ver_pos)[VAL(pos)][VAL(hor_pos)[VAL(pos)]] == key) {
							p[VAL(pos)][(VAL(hor_pos)[VAL(pos)])]
								[(VAL(ver_pos)[VAL(pos)][VAL(hor_pos)[VAL(pos)]])]
								(pt->virtual_video_add, 3);

						} else {
							VAL(ver_pos)[VAL(pos)][VAL(hor_pos)[VAL(pos)]] = key;
							draw_vertical_text1(pt->virtual_video_add);
							i2c_add[1] = VER_POS + VAL(pos)*4 + VAL(hor_pos)[VAL(pos)];
							write_i2c(pt->fd_config, i2c_add, pt->p_config + i2c_add[1], 1);
						}
						key = 0;	break;
						/*次菜单功能调节*/
					case KEY14:	
						if (VAL(zoomed) == 0)		
							p[VAL(pos)][VAL(hor_pos)[VAL(pos)]] 
								[(VAL(ver_pos)[VAL(pos)][VAL(hor_pos)[VAL(pos)]])]
								(pt->virtual_video_add,ADD_PARA);
						key = 0;	break;
					case KEY15:
						if (VAL(zoomed) == 0)
							p[VAL(pos)][VAL(hor_pos)[VAL(pos)]]
								[(VAL(ver_pos)[VAL(pos)][VAL(hor_pos)[VAL(pos)]])]
								(pt->virtual_video_add,SUB_PARA);
						key = 0;	break;
						/*主菜单按键切换 选择*/
					case KEY5:
					case KEY6:
					case KEY7:
					case KEY8:
					case KEY9:
						if (KEY5 == key) key = 0;
						else if (KEY6 == key) key = 1;
						else if (KEY7 == key) key = 2;
						else if (KEY8 == key) key = 3;
						else if (KEY9 == key) key = 4;
						if (VAL(hor_pos)[5]) 
							do_copy(pt->virtual_video_add, key + 1);
						else {
							VAL(hor_pos)[VAL(pos)] = key;
							draw_horizontal_text1(pt->virtual_video_add);
							draw_vertical_text1(pt->virtual_video_add);
							i2c_add[1] = HOR_POS + VAL(pos);
							write_i2c(pt->fd_config, i2c_add, pt->p_config + i2c_add[1], 1);	
						}						
						key = 0;	break;
						/*KEY3 KEY4 切换主菜单*/
					case KEY4:
						if (VAL(zoomed))  ; /*放大状态 按键无效 */
						else if (1 == VAL(hor_pos)[5]) {
							VAL(hor_pos)[5] = 0;
							draw_horizontal_text1(pt->virtual_video_add);
						} else {
							if (VAL(pos) == 3) ;
							else {
								VAL(pos)++;
								draw_horizontal_text1(pt->virtual_video_add);
								draw_vertical_text1(pt->virtual_video_add);
								i2c_add[1] = POS;
								write_i2c(pt->fd_config, i2c_add, pt->p_config + i2c_add[1], 1);
							}
						}
						key = 0;	break;
					case KEY3:
						if(VAL(zoomed)) ;
						else if (1 == VAL(hor_pos)[5]) {
							VAL(hor_pos)[5] = 0;
							draw_horizontal_text1(pt->virtual_video_add);
						} else {
							if (VAL(pos) == 0) ;
							else {
								VAL(pos)--;
								draw_horizontal_text1(pt->virtual_video_add);
								draw_vertical_text1(pt->virtual_video_add);
								i2c_add[1] = POS;
								write_i2c(pt->fd_config, i2c_add, pt->p_config + i2c_add[1], 1);
							}
						}
						key = 0;	break;
						/*增益调节*/
					case KEY1:
						if (VAL(db) == 1100) ;
						else {
							change_db(pt->virtual_video_add, 1);
						}
						key = 0;	break;
					case KEY2:
						if (VAL(db) == 0) ;
						else {
							change_db(pt->virtual_video_add, 2);
						}
						key = 0;	break;
						/*增益微调粗调切换*/
					case KEY_DB_REG:
						change_db_step(pt->virtual_video_add, 1);
						key = 0;	break;
					case KEY_FREZ:
						VAL(frez) = !VAL(frez);
						val = VAL(frez);
						ioctl(pt->fd_gpio, POWER_LOW, &val);
						break;
						/*冻结*/
						/**
						  if (VAL(frez) == 0) 
						  VAL(frez)=1;
						  else 
						  VAL(frez)=0, probe_test=0;
						  draw_rim(virtual_video_add, 0x0);
						  if (VAL(zoomed) == 0) {
						  if (VAL(hor_pos)[4] == 1)
						  draw_horizontal_text2(virtual_video_add);
						  else
						  draw_horizontal_text1(virtual_video_add);
						  }
						  draw_vertical_text1(virtual_video_add);
						  i2c_add[1] = FREZ_1;
						  write_i2c(pt->fd_config, i2c_add, pt->p_config + i2c_add[1], 1);
						  key = 0;
						  break;					
						 **/
					default:
						key = 0;	break;
				}
			}
			initialize_serial();
		}
#endif

//		(pt->cur_data_num >> 3) ? (pt->cur_data_num = 1) : (pt->cur_data_num++);

		memcpy(pt->virtual_video_add1, pt->virtual_video_add, VIDEO_RAM);/*背景 复制到 临时1*/
//		if (!VAL(frez))	
//			fetch_data();

//		draw_data1(pt->virtual_video_add1, WAVE_GROUND, pt->current_data);/*临时1 画波形*/
//		if (VAL(frez) && probe_test)		
//			draw_data2(pt->virtual_video_add1, 49, probe_test2);

		/*临时1 画闸门 刻度值 电池等*/
//		draw_agat(pt->virtual_video_add1, 0);
//		draw_bgat(pt->virtual_video_add1, 0);
//		draw_battery(pt->virtual_video_add1, battery_state);
//		draw_scale(pt->virtual_video_add1, 0);
//		calculate_measure_data();
		memcpy(pt->true_video_add, pt->virtual_video_add1, VIDEO_RAM);/*复制到 显存显示*/

		return 0;
		/*当前 菜单为时间 时候 刷新时间 */
//		if ((VAL(zoomed) == 0) && (3==VAL(pos)) && 
//				(3 == VAL(hor_pos)[VAL(pos)]) && 
//				(1 != VAL(ver_pos)[VAL(pos)][VAL(hor_pos)[VAL(pos)]]))
//			p[3][3][1](pt->virtual_video_add, 0);

	}
	return 0;
}
