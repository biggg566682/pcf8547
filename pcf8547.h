#ifndef PCF8547_H
#define PCF8547_H

void i2c_send_byte(unsigned char );
void i2c_command(unsigned char  );
void i2c_lcd_address(int , int );
void string_slide(void);
void i2c_init(void);


#endif
