/*
 * File:   l.h
 * Author: danny
 *
 * Created on November 4, 2014, 9:45 PM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif


#define LCD_PIC_I2C_ADDR  0x4e


// Line addresses for LCDs which use
// the Hitachi HD44780U controller chip
#define LCD_PIC_LINE_1_ADDRESS 0x00
#define LCD_PIC_LINE_2_ADDRESS 0x40
#define LCD_PIC_LINE_3_ADDRESS 0x14
#define LCD_PIC_LINE_4_ADDRESS 0x54

int i2cfile;

unsigned char chr_to_cyr (unsigned char ch);



void load_cgram();
void LCD_Init();
void LCD_Goto(unsigned char x, unsigned char y);
void LCD_Write_Char(unsigned char c);
void LCD_Write_String(const char *str);
void LCD_Write_Int(long num);
void LCD_Write_Int1Dec(char v,long num,char pm);
void LCD_Write_Int2Dec(char v,long num,char pm);
void LCD_Write_Int1(char v,long num);
void LCD_Write_Byte(unsigned char address, unsigned char n);
void LCD_Clear();
void LCD_Clear_Line(unsigned char line);

void LCD_Open(); // Open I2C Connection
void LCD_BL(unsigned char status); // set blacklight

unsigned char _LCD_Build_Byte();
void _LCD_Write_Upper_Nibble(unsigned char u);
void _LCD_Write_Lower_Nibble(unsigned char l);



#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

