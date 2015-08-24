#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "lcd.h"
#include <time.h>

#define DEFAULT_DEV  "/dev/i2c-1"
#define DEFAULT_ADDRESS 0x27

// Line addresses for LCDs which use
// the Hitachi HD44780U controller chip
#define LCD_PIC_LINE_1_ADDRESS 0x00
#define LCD_PIC_LINE_2_ADDRESS 0x40
#define LCD_PIC_LINE_3_ADDRESS 0x14
#define LCD_PIC_LINE_4_ADDRESS 0x54


extern int i2cfile;


unsigned char  LCD_BL_Status = 1;     // 1 for POSITIVE control, 0 for NEGATIVE control

unsigned char  pin_E;//   =    I2C_BYTE.2
unsigned char  pin_RW;//  =    I2C_BYTE.1
unsigned char  pin_RS;//  =    I2C_BYTE.0
unsigned char  pin_D4;//  =    I2C_BYTE.4
unsigned char  pin_D5;//  =    I2C_BYTE.5
unsigned char  pin_D6;//  =    I2C_BYTE.6
unsigned char  pin_D7;//  =    I2C_BYTE.7
unsigned char  pin_BL;//  =    I2C_BYTE.3




void WriteI2C2(unsigned char ch)
{
    unsigned char write_buf[2];
    write_buf[0] = ch;
    write(i2cfile, write_buf, 1);
//    usleep(10);
}


    static const unsigned char cyr_table[]={
'A',	// �
160,	// �
'B',	// �
161,	// �
224,	// �
'E',	// �
//	162,	// �
163,	// �
164,	// �
165,	// �
166,	// �
'K',	// K
167,	// �
'M',	// M
'H',	// H
'O',	// O
168,	// �
'P',	// P
'C',	// C
'T',	// T
169,	// �
170,	// �
'X',	// X
225,	// �
171,	// �
172,	// �
226,	// �
173,	// �
174,	// �
'b',	// �
175,	// �
176,	// �
177,	// �
'a',	// �
178,	// �
179,	// �
180,	// �
227,	// �
'e',	// e
//	181,	// �
182,	// �
183,	// �
184,	// �
185,	// �
186,	// �
187,	// �
188,	// �
189,	// �
'o',	// o
190,	// �
'p',	// �
'c',	// �
191,	// �
'y',	// y
228,	// �
'x',	// x
229,	// �
192,	// �
193,	// �
230,	// �
194,	// �
195,	// �
196,	// �
197,	// �
198,	// �
199,	// �
};
/*******************************************************/
unsigned char chr_to_cyr (unsigned char ch){
if (ch==168)    return 162;
if (ch==184)    return 181;
if (ch>=192)    return cyr_table[ch-192];
return ch;
}


unsigned char cgram[64] = {
0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00, // символ 0x00 |
0x00,0b10111,0b00101,0b10111,0b10100,0b10100,0x00,0x00, // символ 0x01 IP
0b1,0b1,0b11,0b11,0b111,0b1111,0b11111,0, // символ 0x02 // inet
0b100,0b0,0b10100,0b1100,0b100,0b110,0b101,0x00, // символ 0x03 //no inet
0b10101,0b10100,0b10101,0b11101,0b10101,0b10101,0b10101,0x00, // символ 0x04 //sound hi 320kb
0b00,0b11000,0b10111,0b10010,0b1010,0b1010,0b11010,0x00, // символ 0x05//sound st 128kb
0x0,0b10111,0b10101,0b10101,0b10101,0b10111,0b11000,0x00, // символ 0x06 //sound lo <128kb
0x0,0x0,0x0,0x0,0x0,0x1f,0x1f,0x00  // символ 0x07//vol 2
};
 
void load_cgram (){
 unsigned char x;
 LCD_Write_Byte(0x00,0x40);                        // вызов записи в регистры ЖКИ
 for (x = 0; x < 64; x++) {
   LCD_Write_Byte(0x01,cgram[x]);                  // вызов записи данных в ЖКИ
 }
 LCD_Write_Byte(0x00,0x80);                        // вызов записи в регистры ЖКИ
}


void LCD_Open()
        {

        if ((i2cfile = open("/dev/i2c-1",O_RDWR)) < 0)
                {
                printf("Failed to open the bus.");
                /* ERROR HANDLING; you can check errno to see what went wrong */
//                com_serial=0;
                exit(1);
                }

    if (ioctl(i2cfile,I2C_SLAVE,0x27) < 0)
                {
                printf("Failed to acquire bus access and/or talk to slave.\n");
                /* ERROR HANDLING; you can check errno to see what went wrong */
//                com_serial=0;
                exit(1);
                }
//        return i2cfile;

}


void LCD_BL(unsigned char status)
{
    LCD_BL_Status = status;
    LCD_Write_Byte(0x00, 0x00);
}


unsigned char lcd_data;
#define STROBE_EN() do {lcd_data |= 4; WriteI2C2(lcd_data); lcd_data &= ~4; WriteI2C2(lcd_data); }while(0)

void LCD_Init()
{
    LCD_Open();
  usleep(100000);

/*
  usleep(40000);
//   i2c_start(LCD_ADDRESS + I2C_WRITE);
   WriteI2C2(0b00110000);  // Установить сигналы RS = О, R/W = О, DB7 = О, DB6 = О, DB5 = 1, DB4 = 1, DB3 = *, DB2 = *, DB1 = *, DB0 = *
   usleep(8100);   
   WriteI2C2(0b00110000); // Установить сигналы RS = О, R/W = О, DB7 = О, DB6 = О, DB5 = 1, DB4 = 1, DB3 = *, DB2 = *, DB1 = *, DB0 = *
   usleep(1000);      
   WriteI2C2(0b00110000); // Установить сигналы RS = О, R/W = О, DB7 = О, DB6 = О, DB5 = 1, DB4 = 1, DB3 = *, DB2 = *, DB1 = *, DB0 = *
   usleep(2000);
   WriteI2C2(0b00100000); // выбор 4 битного режима
   usleep(2000);      
   WriteI2C2(0b00100000); // повтор выбора 4 битного режима
   usleep(2000);
   WriteI2C2(0b10000000); // выбор числа строк и размера матрицы символов.
   usleep(2000);
   WriteI2C2(0b00000000);
   usleep(2000);
   WriteI2C2(0b10000000); //выкл дисплей
   usleep(2000);      
   WriteI2C2(0b00000000);
   usleep(2000);
   WriteI2C2(0b11000000); //вкл дисплей
   usleep(2000);      
   WriteI2C2(0b00000000);
   usleep(2000);
   WriteI2C2(0b01100000);   //тут выбор параметров
   usleep(2000);   
//   i2c_stop();
*/

   WriteI2C2(0b00000000);
   usleep(200000);//   usleep(40000);
// 3 x 0x03h, 8bit
   lcd_data = 0b00110000;
   WriteI2C2(lcd_data);
   STROBE_EN();
   usleep(10000);//   usleep(5000);
   STROBE_EN();
   usleep(1000);//usleep(100);
   STROBE_EN();
   usleep(1000);//usleep(100);
// 4bit - 0x02h
   lcd_data = 0b00100000;
   WriteI2C2(lcd_data);
   STROBE_EN();
   usleep(500);//usleep(100);
   WriteI2C2(lcd_data);
   usleep(500);//usleep(100);
// From now on in 4-bit-Mode
//   LCD_Write_Byte(LCD_4BIT | LCD_2LINE | LCD_5X7);
//   LCD_Write_Byte(LCD_DISPLAYOFF);         
//   LCD_Write_Byte(LCD_CLEAR);                  
//   LCD_Write_Byte(LCD_INCREASE | LCD_DISPLAYSHIFTOFF);   
//   LCD_Write_Byte(LCD_DISPLAYON | LCD_BLINKINGOFF);      
    LCD_Write_Byte(0x00, 0x2);   //
    LCD_Write_Byte(0x00, 0x0c);   // Write Nibble 0x02 once (per HD44780U initialization spec)
    LCD_Write_Byte(0x00, 0x01);   // Clear display



/*
    // Following bytes are all Command bytes, i.e. address = 0x00
    WriteI2C2(0xff);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    usleep(100000);                // (per HD44780U initialization spec)
    usleep(50000);                // (per HD44780U initialization spec)


    // Following bytes are all Command bytes, i.e. address = 0x00
    WriteI2C2(0x0);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    usleep(10000);                // (per HD44780U initialization spec)
    usleep(5000);                // (per HD44780U initialization spec)
  
    // Following bytes are all Command bytes, i.e. address = 0x00
    WriteI2C2(0x0);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    usleep(10000);                // (per HD44780U initialization spec)
    usleep(5000);                // (per HD44780U initialization spec)
    WriteI2C2(0x4);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    WriteI2C2(0x0);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    usleep(5000);                // (per HD44780U initialization spec)
    WriteI2C2(0x4);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    WriteI2C2(0x0);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    usleep(200000);                // (per HD44780U initialization spec)
    WriteI2C2(0x4);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    WriteI2C2(0x0);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    usleep(20000);                // (per HD44780U initialization spec)
//    LCD_Write_Byte(0x00, 0x03);   //
    usleep(10000);                // (per HD44780U initialization spec)
    LCD_Write_Byte(0x00, 0x2);   //
//    LCD_Write_Byte(0x00, 0x02);   //
    usleep(10000);                // (per HD44780U initialization spec)
//    LCD_Write_Byte(0x00, 0x0a);   // Write Nibble 0x02 once (per HD44780U initialization spec)
//    __delay_ms(5);
    LCD_Write_Byte(0x00, 0x0c);   // Write Nibble 0x02 once (per HD44780U initialization spec)
    usleep(5000);
    LCD_Write_Byte(0x00, 0x01);   // Clear display
    usleep(5000);                // (per HD44780U initialization spec)
*/





/*
  // Send Command 0x30
  LCD_Write_Byte(0x00,0x30);

  // Wait for more than 4.1 ms
  usleep(41000);

  // Send Command 0x30
  LCD_Write_Byte(0x00,0x30);

  // Wait for more than 100 us
  usleep(2000);           

  // Send Command 0x30
  LCD_Write_Byte(0x00,0x30);
  usleep(2000);  

  // Function set: Set interface to be 4 bits long (only 1 cycle write).
  LCD_Write_Byte(0x00,0x20); 
  usleep(2000);   

  // Function set: DL=0;Interface is 4 bits, N=1; 2 Lines, F=0; 5x8 dots font)
  LCD_Write_Byte(0x00,0x28);
  usleep(2000);  
  LCD_Write_Byte(0x00,0x28);
  usleep(2000);  

  // Display Off: D=0; Display off, C=0; Cursor Off, B=0; Blinking Off
  LCD_Write_Byte(0x00,0x08);
  usleep(2000);  
  LCD_Write_Byte(0x00,0x08);
  usleep(2000);  

  // Display Clear
  LCD_Write_Byte(0x00,0x01);
  usleep(2000);  
  LCD_Write_Byte(0x00,0x01);
  usleep(2000);  

  // Entry Mode Set: I/D=1; Increament, S=0; No shift
  LCD_Write_Byte(0x00,0x06);
  usleep(2000);  
  LCD_Write_Byte(0x00,0x06);
  usleep(2000);  

  // Display On, Cursor Off
  LCD_Write_Byte(0x00,0x0C);
  usleep(2000);  
  LCD_Write_Byte(0x00,0x0C);
  usleep(2000);  
*/


/*    
    // Following bytes are all Command bytes, i.e. address = 0x00
    WriteI2C2(0x0);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    usleep(10000);                // (per HD44780U initialization spec)
    usleep(5000);                // (per HD44780U initialization spec)
    WriteI2C2(0x4);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    WriteI2C2(0x0);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    usleep(5000);                // (per HD44780U initialization spec)
    WriteI2C2(0x4);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    WriteI2C2(0x0);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    usleep(200);                // (per HD44780U initialization spec)
    WriteI2C2(0x4);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    WriteI2C2(0x0);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    usleep(200);                // (per HD44780U initialization spec)
//    LCD_Write_Byte(0x00, 0x03);   //
//    usleep(10000);                // (per HD44780U initialization spec)
//    LCD_Write_Byte(0x00, 0x2);   //
//    LCD_Write_Byte(0x00, 0x02);   //
    usleep(10000);                // (per HD44780U initialization spec)
    LCD_Write_Byte(0x00, 0x0a);   // Write Nibble 0x02 once (per HD44780U initialization spec)
//    __delay_ms(5);
    LCD_Write_Byte(0x00, 0x0c);   // Write Nibble 0x02 once (per HD44780U initialization spec)
    usleep(5000);
    LCD_Write_Byte(0x00, 0x01);   // Clear display
    usleep(5000);                // (per HD44780U initialization spec)
*/
/*
WriteI2C2(0x30);
usleep(4100);   
WriteI2C2(0x30);
usleep(4100);   
WriteI2C2(0x30);
usleep(4100);   
// Function Set (4Bit Mode, 2 Lines, 5x10 Characters)
WriteI2C2(0x20);
// usleep(1000);   
WriteI2C2(0x20);
// usleep(1000);   
WriteI2C2(0xF0);
 usleep(80000);   
// Display On
LCD_Write_Byte(0,0x00);
 usleep(1000);   
LCD_Write_Byte(0,0xF0);
 usleep(1000);   
// Display Clear
LCD_Write_Byte(0,0x00);
 usleep(1000);   
LCD_Write_Byte(0,0x10);
 usleep(1000);   
// Entry Mode
LCD_Write_Byte(0,0x00);
 usleep(1000);   
LCD_Write_Byte(0,0x60);
 usleep(1000);   
*/
}

void LCD_Goto(unsigned char x, unsigned char y)
{
unsigned char address;

   switch(y)
     {
      case 1:
        address = LCD_PIC_LINE_1_ADDRESS;
        break;

      case 2:
        address = LCD_PIC_LINE_2_ADDRESS;
        break;

      case 3:
        address = LCD_PIC_LINE_3_ADDRESS;
        break;

      case 4:
        address = LCD_PIC_LINE_4_ADDRESS;
        break;

      default:
        address = LCD_PIC_LINE_1_ADDRESS;
        break;
     }

   address += x-1;
   LCD_Write_Byte(0, 0x80 | address);
}

//===================================
void LCD_Write_String(const char *str)
{
   // Writes a string text[] to LCD via I2C
   pin_RS  = 1;
   pin_RW  = 0;
   pin_E   = 0;
   pin_BL  = LCD_BL_Status;

   while (*str)
   {
        // Send upper nibble
        _LCD_Write_Upper_Nibble(chr_to_cyr(*str));

        // Send lower nibble
        _LCD_Write_Lower_Nibble(chr_to_cyr(*str));

        str++;
   }
}

void LCD_Write_Char(unsigned char c)
{
    LCD_Write_Byte(0x01,chr_to_cyr(c));
}

void LCD_Write_Int(long num)
{
    if (num < 0) { LCD_Write_String("-"); num *= -1; }

    unsigned char number[10];
    unsigned char num_count = 0;

    do {        
        number[num_count] = num % 10;
        num_count++;
        num /= 10;
    } while (num > 0);

    for (int i = num_count-1; i>= 0; i--)
    {
        LCD_Write_Char(number[i] + 0b00110000);
    }
}

void LCD_Write_Int1Dec(char v,long num,char pm)
{
    if (num < 0) { LCD_Write_String("-"); num *= -1; }else{if(pm==1){LCD_Write_String(" ");}}

    unsigned char number[10];
    unsigned char num_count = 0;


    do {
        number[num_count] = num % 10;
        num_count++;
        num /= 10;
    } while (num > 0);

    for (int i = v-1; i>= 0; i--)
    {
        if(i==0){
            LCD_Write_Char('.');
        }
    if(num_count<=i){
        LCD_Write_Char('0');
    
    }else{
        LCD_Write_Char(number[i] + 0b00110000);
    }
    }
}

void LCD_Write_Int1(char v,long num)
{
    if (num < 0) { LCD_Write_String("-"); num *= -1; }

    unsigned char number[10];
    unsigned char num_count = 0;


    do {
        number[num_count] = num % 10;
        num_count++;
        num /= 10;
    } while (num > 0);

    for (int i = v-1; i>= 0; i--)
    {
    if(num_count<=i){
        LCD_Write_Char('0');

    }else{
        LCD_Write_Char(number[i] + 0b00110000);
    }
    }
}



//===================================
void LCD_Write_Byte(unsigned char address, unsigned char n)
{
    if (address)
    {
        pin_RS=1;   // Data
    }
    else
    {
        pin_RS=0;   // Command
    }

    pin_RW  = 1;
    pin_E   = 0;
    pin_BL  = LCD_BL_Status;

    // Send upper nibble
   _LCD_Write_Upper_Nibble(n);
//   usleep(2);
    // Send lower nibble
   _LCD_Write_Lower_Nibble(n);
 //  usleep(2);


}

void LCD_Clear()
{
    LCD_Write_Byte(0x00,0x01);
    usleep(1500);

}

void LCD_Clear_Line(unsigned char line)
{
    LCD_Goto(1,line);
    for (int i = 0; i<20; i++)
    {
        LCD_Write_String(" ");
    }
    LCD_Goto(1,line);
}

void _LCD_Write_Upper_Nibble(unsigned char u)
{
    // Send upper nibble
    if((u & (1 << 7)))
        pin_D7=1;
    else
        pin_D7=0;

    if((u & (1 << 6)))
        pin_D6=1;
    else
        pin_D6=0;

    if((u & (1 << 5)))
        pin_D5=1;
    else
        pin_D5=0;

    if((u & (1 << 4)))
        pin_D4=1;
    else
        pin_D4=0;

   pin_E = 0;
   WriteI2C2(_LCD_Build_Byte());
   pin_E = 1;
   WriteI2C2(_LCD_Build_Byte());
   pin_E = 0;
   WriteI2C2(_LCD_Build_Byte());
}

void _LCD_Write_Lower_Nibble(unsigned char l)
{
    // Send lower nibble
    if((l & (1 << 3)))
        pin_D7=1;
    else
        pin_D7=0;

    if((l & (1 << 2)))
        pin_D6=1;
    else
        pin_D6=0;

    if((l & (1 << 1)))
        pin_D5=1;
    else
        pin_D5=0;

    if((l & (1 << 0)))
        pin_D4=1;
    else
        pin_D4=0;

    pin_E = 0;
    WriteI2C2(_LCD_Build_Byte());
    pin_E = 1;
    WriteI2C2(_LCD_Build_Byte());
    pin_E = 0;
    WriteI2C2(_LCD_Build_Byte());
}

unsigned char _LCD_Build_Byte()
{
    unsigned char ret = 0x00;

    ret |= pin_E    << 2;
    ret |= 0    << 1;
//    ret |= pin_RW   << 1;
    ret |= pin_RS   << 0;
    ret |= pin_D4   << 4;
    ret |= pin_D5   << 5;
    ret |= pin_D6   << 6;
    ret |= pin_D7   << 7;
    ret |= pin_BL   << 3;

    return ret;
}


