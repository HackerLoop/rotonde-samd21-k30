/*
* k30.c
*
* Created: 27/01/2016 20:42:06
*  Author: GroundLayer
*/

#include "k30.h"

static struct usart_module k30_uart;
uint8_t buffer[7];
uint8_t cmd[7] = { 0xFE , 0x44 , 0x00 , 0x08 , 0x02 , 0x9F , 0x25};
	
bool k30_init(void){
	struct usart_config usart_conf;
	
	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = USART_RX_3_TX_2_XCK_3;
	usart_conf.pinmux_pad0 = PINMUX_UNUSED;
	usart_conf.pinmux_pad1 = PINMUX_UNUSED;
	usart_conf.pinmux_pad2 = PINMUX_PA10C_SERCOM0_PAD2;
	usart_conf.pinmux_pad3 = PINMUX_PA11C_SERCOM0_PAD3;
	usart_conf.baudrate    = 9600;

	if(usart_init(&k30_uart, SERCOM0, &usart_conf) != STATUS_OK ) {
		return false;
	}

	usart_enable(&k30_uart);
	return true;
	
}

bool k30_read(float* CO2){
	
	uint16_t garbage;
	uint16_t counter = 0;
	while(usart_read_wait(&k30_uart , &garbage) == STATUS_OK && counter < 50){
		counter++;
	}
	if(counter == 50){
		return false;
	}
	
	usart_write_buffer_wait(&k30_uart , cmd , 7);
	usart_read_buffer_wait(&k30_uart , buffer , 7);
	if(buffer[0] != 0xFE || buffer[1] != 0x44 || buffer[2] != 0x02){
		return false;
	}
	
	
	*CO2 = (float) (buffer[3] << 8 | buffer[4]);
	return true;
}
