#include "ROM8X16.H"
#include "draw_rim.h"
#include "fbdraw.h"
#include "device.h"

//draw a dot on coordinate (x,y) 在 (x,y) 画点
void	fbdot(COLOR_TYPE *p_vmem, int x, int y, COLOR_TYPE i)
{
	*(p_vmem + (640*y) + x) = i;
}

//draw a line from coordinate(x,y1) to coordinate (x,y2) 画线(x,y1) 到 (x,y2)
void	fbliney(COLOR_TYPE *p_vmem, int x, int y1, int y2, COLOR_TYPE i)
{
	int yy1, yy2;
	yy1 = MAX(y1, y2); yy2 = MIN(y1, y2);
	for (;yy2 < yy1 + 1; yy2++)
		fbdot(p_vmem, x, yy2, i);
}

//draw a line from coordinate(x1,y) to coordinate (x2,y) 画线(x1,y) 到 (x2,y)
void	fblinex(COLOR_TYPE *p_vmem, int y, int x1, int x2, COLOR_TYPE i)
{
	int xx1, xx2;
	xx1 = MAX(x1, x2); xx2 = MIN(x1, x2);
	for (;xx2 < xx1 + 1; xx2++)
		fbdot(p_vmem, xx2, y, i);
}

//draw a line from coordinate(x1,y1) ro coordinate(x2,y2)  画线(x1,y1)到(x2,y2)
void	fbline(COLOR_TYPE *p_vmem, int x1, int y1, int x2, int y2, COLOR_TYPE i)
{
	int M, N, n, r, s, t;

	if (x1 > x2) {
		M = x1 ; x1 = x2; x2 = M;
		M = y1 ; y1 = y2; y2 = M;
	}

	//算出,两个点之间的x坐标的差值.
	M = abs(x1 - x2);
	//算出,两个点之间的y坐标的差值.
	N = abs(y1 - y2);
	//如果两个点之间的x坐标间差值是0的话,则调用画竖线函数fbliney来完成
	if (M == 0) { 
		fbliney(p_vmem, x1, y1, y2, i);
	}
	//如果两个点之间的y坐标间差值是0的话,则调用画横线函数fblinex来完成
	if (N == 0) {	
		fblinex(p_vmem, y1, x1, x2, i);
	}
	//如果两个点之间的斜率是1的话
	if ((M == N) && (M != 0)) {
		for (t = x1; t <= x2; t++) {
			s = (t - x1) * (y2 - y1)/M+y1;
			fbdot(p_vmem, t, s, i);
		}
	}
	if ((M < N) && (M != 0)) {
		for (t = x1, r = y1; t <= x2; t++) {
			s = (t - x1) * (y2 - y1)/M+y1;
			fbliney(p_vmem, t, r, s, i);
			r = s;
		}
	}
	if ((M > N) && (N != 0)) {
		if (y1 > y2) {
			n = x1; x1 = x2;x2 = n;
			n = y1; y1 = y2;y2 = n;
		}
		for(t=y1,r=x1;t<=y2;t++){
			s = (t - y1) * (x2 - x1)/N+x1;
			fblinex(p_vmem, t, r, s, i);
			r=s;
		}
	}
}

//draw a rectangle width ,length on coordinate(x,y) color i 画矩形左上角坐标(x,y) 宽width 高height 颜色 i
void fbrec(COLOR_TYPE *p_vmem, int x, int y, int width, int height, COLOR_TYPE i)
{
	int k;
	for (k = 0; k < height; k++)
		fblinex(p_vmem, y + k , x + width, x, i);
}

void draw_data2(COLOR_TYPE *p_vmem, COLOR_TYPE i, float *data)
{
	int k;
	char str_tmp[] = "-6dB";
	for (k = 0; k < 400 - 1; k++)
		fbline(p_vmem, k, 399 - (unsigned short)(data[k]), k + 1, 
				399 - (unsigned short)(data[k + 1]), i);
	fbline(p_vmem, fftx1_tmp + 1, 399 - (unsigned short)(ffty_tmp), 
			fftx2_tmp, 399 - (unsigned short)(ffty_tmp),i);
	draw_str(p_vmem, str_tmp, fftx2_tmp + 2, 399 - (unsigned short)(ffty_tmp), 1, 1,i);
}

//draw dots of array:t1 to framebuffer 把数组data 点信息 显示到fb  envelope_data
void draw_data1(COLOR_TYPE *p_vmem, COLOR_TYPE i, unsigned short *data)
{
	int k, bit = !(pt->hanfeng == 0);
	if (0 == VAL(zoomed)) {
		for (k = 0; k < 480 - 1; k++) {
			if (VAL(filled)) {
				if (VAL(rectify) == 3) 
					fbline(p_vmem, k + 1, data[k]>>bit, k + 1, 200>>bit, i);
				else 
					fbline(p_vmem, k + 1, data[k]>>bit, k + 1, 400>>bit, i);
			}else
				fbline(p_vmem, k + 1, data[k]>>bit, k + 1, data[k + 1]>>bit, i);
		}
		if (1 == VAL(a_scan)) {
			envelope_data[0] = MIN(envelope_data[0], data[0]);
			for (k = 0; k < 480 - 1; k++) {
				envelope_data[k + 1] = MIN(envelope_data[k + 1], data[k + 1]);
				fbline(p_vmem, k, envelope_data[k]>>bit, k + 1, envelope_data[k + 1]>>bit, 49);
			}		
		}
		fbline(p_vmem, 0, 0, 479, 0, WAVE_BACK);
	} else {
		unsigned short data_tmp1[480];
		for (k = 0; k < 480; k++)
			data_tmp1[k] = data[k] * 9/8;
		for (k = 0; k < 213; k++) {
			current_data1[k * 3] = data_tmp1[k * 2];
			current_data1[k * 3 + 2] = data_tmp1[k * 2 + 1];
			current_data1[k * 3 + 1] = (data_tmp1[k * 2] + data_tmp1[k * 2 + 1]) / 2;
		}
		for (k = 0; k < 638; k++) {
			if (VAL(filled))
				fbline(p_vmem, k + 1, current_data1[k], k + 1, 450, i);
			else
				fbline(p_vmem, k + 1, current_data1[k], k + 1, current_data1[k + 1], i);
		}
		if (1 == VAL(a_scan)) {
			envelope_data[0] = MIN(envelope_data[0], data[0]);
			for (k = 0; k < 480 - 1; k++) {
				envelope_data[k + 1] = MIN(envelope_data[k + 1], data[k + 1]);
				fbline(p_vmem, k, envelope_data[k], k + 1, envelope_data[k + 1], 49);
			}		
		}
//		fbline(p_vmem, 0, 0, 639, 0, WAVE_BACK);
	}
}

//draw ascii on coordinate (x,y) 画ascii码字符在左上角坐标(x,y) 放大zoon_xXzoom_y倍 颜色 i
void draw_ascii(COLOR_TYPE *p_vmem, char c, 
		int x, int y, int zoom_x, int zoom_y, COLOR_TYPE col)
{
	unsigned char k[16];		/*store ascii code*/
	char m[8];			/*store per/bit information*/
	int j,l,p;

	for (j = 0; j < 16; j++) {
		k[j] = rom8x16_bits[c * 16 + j];	/*read ascii code to k[16]*/
	}
	for (j = 0; j < 16; j++) {
		for (l = 0; l < 8; l++) {
			m[8 - l - 1] = (k[j] & (1 << l));		/*read per/bit information ro k[8]*/
		}
		for (l = 0; l < 8; l++) {
			if (m[l]) {
				for (p = 0; p < zoom_y; p++) {
					fbline(p_vmem, x + zoom_x * l, y + j * zoom_y + p,
							x + zoom_x * (l + 1) - 1, y + j * zoom_y + p, col);
				}
			}
		}
	}
}

//draw a string 显示一个字符串在指定位置
void draw_str(COLOR_TYPE *p_vmem, char *str, int x, int y, 
		int zoom_x, int zoom_y, COLOR_TYPE col)
{
	while (*str != '\0') {
		draw_ascii(p_vmem, *str, x, y, zoom_x, zoom_y, col);
		x = x + 8 * zoom_x;
		str++;
	}
}

void draw_dot_24(COLOR_TYPE *p_vmem, int x, int y, int i)
{
	p_vmem[1843200 - (640 * y + (640 - x)) * 3 + 0] = i & 0xff;
	p_vmem[1843200 - (640 * y + (640 - x)) * 3 + 1] = (i >> 8) & 0xff;
	p_vmem[1843200 - (640 * y + (640 - x)) * 3 + 2] = (i >> 16) & 0xff;
}

void draw_liney_24(COLOR_TYPE *p_vmem, int x, int y1, int y2, int i)
{
	int yy1, yy2;
	yy1 = MAX(y1, y2);
	yy2 = MIN(y1, y2);
	for (; yy2 < yy1 + 1; yy2++)
		draw_dot_24(p_vmem, x, yy2, i);
}

void draw_linex_24(COLOR_TYPE *p_vmem, int y, int x1, int x2, int i)
{
	int xx1,xx2;
	xx1 = MAX(x1, x2);
	xx2 = MIN(x1, x2);
	for (; xx2 < xx1 + 1; xx2++)
		draw_dot_24(p_vmem, xx2, y, i);
}

void fbrec_24(COLOR_TYPE *p_vmem, int x, int y, int width, int height, int i)
{
	int k;
	for (k = 0; k < height; k++)
		draw_linex_24(p_vmem, y + k, x + width, x, i);
}

void draw_line_24(COLOR_TYPE *p_vmem, int x1, int y1, int x2, int y2, int i)
{
	int M,N,n,r,s,t;
	if (x1 > x2) {
		M = x1;x1 = x2;x2 = M;
		M = y1;y1 = y2;y2 = M;
	}
	M = abs(x1 - x2);
	N = abs(y1 - y2);
	if (M == 0)
		draw_liney_24(p_vmem, x1, y1, y2, i);
	if (N == 0)
		draw_linex_24(p_vmem, y1, x1, x2, i);
	if ((M == N) && (M != 0)) {
		for (t = x1; t <= x2; t++) {
			s = (t - x1) * (y2 - y1) / M + y1;
			draw_dot_24(p_vmem, t, s, i);
		}
	}
	if ((M < N) && (M != 0)) {
		for (t = x1,r = y1; t <= x2; t++) {
			s = (t - x1) * (y2 - y1) / M + y1;
			draw_liney_24(p_vmem, t, r, s, i);
			r = s;
		}
	}
	if ((M > N) && (N != 0)) {
		if (y1 > y2) {
			n = x1;x1 = x2;x2 = n;
			n = y1;y1 = y2;y2 = n;
		}
		for (t = y1,r = x1; t <= y2; t++) {
			s = (t - y1) * (x2 - x1) / N + x1;
			draw_linex_24(p_vmem, t, r, s, i);
			r = s;
		}
	}
}

void draw_ascii_24(COLOR_TYPE *p_vmem, unsigned char c, int x, int y,
		int zoom_x, int zoom_y, int i, int n)
{
	unsigned char k[16];//store ascii code
	char m[8];			//store per/bit information
	int j,l,p;

	for (j = 0; j < 16; j++) {
		k[j] = rom8x16_bits[c * 16 + j];	//read ascii code to k[16]
	}
	for (j = 2; j < 12; j++) {
		for (l = 0; l < 8; l++) {
			m[8 - l - 1] = (k[j] & (1 << l));		//read per/bit information ro k[8]
		}
		for (l = 0; l < 8; l++) {
			if (m[l]) {
				for (p = 0; p < zoom_y; p++) {
					draw_line_24(p_vmem, x + zoom_x * l, y + j * zoom_y + p,
							x + zoom_x * (l + 1) - 1,y + j * zoom_y + p, i);
				}
			}
		}
	}
}

void draw_str_24(COLOR_TYPE *p_vmem, char *str, int x, int y,
		int zoom_x, int zoom_y, int i, int n)
{
	while (*str != '\0') {
		draw_ascii_24(p_vmem, *str, x, y, zoom_x, zoom_y, i, n);
		x = x + 8 * zoom_x;
		str++;
	}
}

void fetch_data()
{
	unsigned int i,j,temp_1,temp_2;
	unsigned int temp_buffer[480] = {0}; 
	unsigned short *data_tmp;

	int xsub, ysub, temp, before_temp, end_temp, step, u = 0, k = 0;
	const int float_long = 20;

	//判断一下数据是从那里来的??
		data_tmp = (unsigned short *)pt->sram_base;

	//小于480点吗??
	if (pt->dot_quantily < 480) {
		//进入这里则代表小于480点,那么就要插值了.

		//估计是从数据源里面copy480个点的数据出来,存放在data_tmp指针所指向的地方.
		//不过,不是太清楚,为什么不足480个点,还要copy480个点出来,而不是copy实际上面的点数出来!!
		memcpy (data_tmp, (unsigned long *)(pt->map_base + 128 * 1024), 480 * 2);

		//#if 0   
		//用定点数来求的.
		//先求步进 
		step = ( 480 << float_long ) / pt->dot_quantily; 

		//先做一个阶保持的工作
		for (i = 0; i < pt->dot_quantily; i++) {
			temp = ( ( ( i + 1 ) * step ) + ( 1 << ( float_long -  1 ) ) ) >> float_long; 
			temp_buffer[temp -  1] = data_tmp[i] << float_long;
		}
		j = 1;
		temp = ( ( step + ( 1 << ( float_long - 1 ) ) ) >> float_long ) - 1;
		end_temp = ( ( ( ( j * step ) + ( 1 << ( float_long - 1 ) ) ) >> float_long ) - 1 );
		before_temp = 0;

		for (i = 0; i < 480; i++) {
			if (i < temp) {
				temp_buffer[i] = temp_buffer[temp];
				continue;
			}
			if (i == end_temp) {
				j++;
				end_temp = ((((j * step) + (1 << (float_long - 1))) >> float_long ) - 1 );	
				before_temp = (((((j - 1) * step) + (1 << (float_long - 1))) >> float_long) - 1 );

			}

			if ( ( i == before_temp ) && ( j <= pt->dot_quantily ) ) {
				xsub = end_temp - before_temp;
				ysub = temp_buffer[end_temp] - temp_buffer[before_temp];
				k = ysub / xsub;
				u = 0;
				continue;
			}

			if ( i == before_temp ) {
				continue;
			}

			u++;

			temp_buffer[i] = ( ( k * u ) + temp_buffer[before_temp] + ( 1 << ( float_long - 1 ) ));
		}

		for (i = 0; i < 480; i++) {
			temp_buffer[i] = temp_buffer[i] >> float_long;
			org_data[i] = temp_buffer[i];

			if (VAL(rectify) == 3)
				pt->current_data[i] = 
					400 - (org_data[i] > 455 ? 400 : (org_data[i] < 56 ? 0 : (org_data[i] - 56)));
			else	
				pt->current_data[i] = 400 - (org_data[i] > 400 ? 400 : org_data[i]);
		}
	} else {
//		while ( val == 0x101 ) {
//			ioctl(pt->fd_gpio, DMA_OK, &val);
//		}
//			ioctl(pt->fd_gpio, DMA_OK, &val);
		memcpy(data_tmp, (unsigned long *)(pt->map_base + 128 * 1024), pt->dot_quantily * 2);
		for (i = 0; i < 480; i++) {
			temp_2 = pt->dot_quantily / 480;
			temp_1 = i * pt->dot_quantily / 480;
//			gat_max[i] = i * pt->dot_quantily / 480;
			gat_max[i] = temp_1;
//			org_data[i] = data_tmp[i * pt->dot_quantily / 480];
			org_data[i] = data_tmp[temp_1];
			for (j = 1; j < temp_2; j++) {
				if (data_tmp[temp_1 + j] > org_data[i]) {
					gat_max[i] = temp_1 + j;
					org_data[i] = data_tmp[temp_1 + j];
//				if (data_tmp[i * pt->dot_quantily / 480 + j] > org_data[i]) {
//					gat_max[i] = i * pt->dot_quantily / 480 + j;
//					org_data[i] = data_tmp[i * pt->dot_quantily / 480 + j];
				}
			}
			if (VAL(rectify) == 3)
				pt->current_data[i] =
					400 - (org_data[i] > 455 ? 400 : (org_data[i] < 56 ? 0 : (org_data[i] - 56)));
			else	
				pt->current_data[i] = 400 - (org_data[i] > 400 ? 400 : org_data[i]);
		}
	}
//		ioctl(pt->fd_gpio, NO_FREZ, &val);
}



/*把内存中的图标信息显示在FB上, hbmpmem显示大小为128*52的,vbmpmem显示大小为160*50的lbmpmem为80*49*/
void show_hbmpmem(COLOR_TYPE *p_vmem, COLOR_TYPE *p_bmp, int x, int y)
{
	int k,j;
	for (k = 0;k < Y_1; k++){
		for (j = 0; j < X_1; j++){
			fbdot(p_vmem, x + j, y + k, p_bmp[k * X_1 + j]);
		}
	}
}

void show_vbmpmem(COLOR_TYPE *p_vmem, COLOR_TYPE *p_bmp, int x, int y)
{
	int k,j;
	for (k = 0; k < Y_2; k++){
		for (j = 0; j < X_2; j++){
			fbdot(p_vmem, x + j, y + k, p_bmp[k * X_2 + j]);
		}
	}
}

void show_lbmpmem(COLOR_TYPE *p_vmem, COLOR_TYPE *p_bmp, int x, int y)
{
	int k,j;
	for (k = 0; k < Y_2; k++){
		for (j = 0; j < X_3; j++){
			fbdot(p_vmem, x - j, y + k, p_bmp[k * X_3 + j]);
		}
	}
}

void show_bmpmem(COLOR_TYPE *p_vmem, COLOR_TYPE *p_bmp, int x, int y, int length, int height)
{
	int k,j;
	for (k = 0; k < height; k++){
		for (j = 0; j < length; j++){
			fbdot(p_vmem, x + j , y + k, p_bmp[k * length + j]);
		}
	}
}

void show_bmpmem1(COLOR_TYPE *p_vmem, COLOR_TYPE *p_bmp, 
		int x, int y, int length, int height, int l1, int h1)
{
	/* l1 小于 length h1 小于 height */
	int k,j;
	for (k = 0; k < h1; k++){
		for (j = 0; j < l1; j++){
			fbdot(p_vmem, x + j , y + k, p_bmp[k * length + j]);
		}
	}
}
