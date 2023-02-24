#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>


#define I2C_BUS                 "/dev/i2c-0"
#define I2C_ADDR                0x27
#define BACKLIGHT       0X8
#define EN                      0X4
#define READ                    0X1

int filename;
int i2cstrat(void)
{
                if((filename = open(I2C_BUS, O_RDWR)) < 0)
                {
                                perror("error failed to open i2c");
                                return -1;
                }
                if(ioctl(filename, I2C_SLAVE, I2C_ADDR) < 0)
                {
                                perror("error failed to set I2C adress");
                                return -1;

                }
                return 0;

}

void i2c_send_byte(unsigned char data)
{
                unsigned char *byte;
                byte= &data;
                *byte |= EN;
                write(filename, byte, sizeof(*byte));
                usleep(1);
                *byte &=0xfb;
                write(filename, byte, sizeof(*byte));
                usleep(1);
}

void i2c_command(unsigned char data )
{
                unsigned char *byte = &data;
                i2c_send_byte((*byte & 0xf0)|BACKLIGHT);
                i2c_send_byte(((*byte & 0x0f)<<4) | BACKLIGHT)  ;
}

void i2c_write_string(char *w)
{
                char *ch = w;
                int i = 0;
                while(i < 16 && *ch)
                {
                        i2c_send_byte((*ch & 0xf0) | BACKLIGHT | READ);
                        i2c_send_byte(((*ch & 0x0f)<<4) | BACKLIGHT | READ);
                        i++;
                        ch++;
                }

}
void i2c_lcd_address(int x, int y)
{
        i2c_command((0x80+0x40*y+x ) );
        usleep(40);
}

void string_slide( )
{
        while(1)
        {
                i2c_command(0x18);
                usleep(400000);
        }


}
void i2c_init(void)
{
                i2cstrat();
                usleep(15000);
                i2c_send_byte(0x30);
                usleep(4100);
                i2c_send_byte(0x30);
                usleep(110);
                i2c_send_byte(0x30);
                usleep(40);
                i2c_send_byte(0x20);
                usleep(40);
                i2c_command(0x28);
                usleep(40);
                i2c_command(0x08);
        
                usleep(40);
                i2c_command(0x01);
         
                usleep(2000);
                i2c_command(0x06);
                usleep(40);
                i2c_command(0x0e);
                usleep(40);


}
int main()
{
                i2c_init();
                i2c_write_string("HONG XIANG HAO");
                usleep(40);
                i2c_lcd_address(5 ,1);
                usleep(40);

                i2c_write_string("1");
                string_slide();

                return 0;



