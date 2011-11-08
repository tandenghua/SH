/*
 *文件名	main.c
 *文件描述	主函数
 *创建人	tandenghua	2008.01
 *
 *修改记录	xx
 */

#include "device.h"

int main(int argc,char* argv[])
{
	mem_init();
//	i2c_init();
//	gpio_init();
//	spi_init();
	fb_init();
//	store_init();
//	config_init();


	loop_tt();

	return 0;
}
