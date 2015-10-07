#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "lcd.h"
#include <string.h>

typedef unsigned char uint8;
typedef unsigned short uint16;

char string_lcd[500];
char result_string[500]; //Строка в 20 символов
char result_stringn[500]; //Строка в 20 символов
char *result_rssi[20];
int oldstat=1;
char refreshbitrate=1;

int say40=0;
int say98=0;

char mode=0;

FILE *fp;

time_t now;
struct tm local_date_time;
 char c_time_string[50];
char rev=0;

 long probeg;
 long sutochn;
 long savedprobeg;


unsigned char power=1;

int readnumstation(){
   int num;
    int pr;
    FILE *tmp;
//    tmp=fopen("/automedia/oldstation.db","r");
    tmp=fopen("/sys/devices/platform/sunxi-i2c.1/i2c-1/1-0068/nvram","r");

    if(tmp == NULL)
    {
 	printf("Error '%s'",tmp);
	return;
    }


    fscanf(tmp,"%i\n",  &pr);
    fclose(tmp);
    return pr;
}


int readintparam(char* fname){
   int num;
    int pr;
    FILE *tmp;
//    tmp=fopen("/automedia/oldstation.db","r");
    tmp=fopen(fname,"r");

    if(tmp == NULL)
    {
 	printf("Error '%s'",tmp);
	return;
    }


    fscanf(tmp,"%i\n",  &pr);
    fclose(tmp);
    return pr;
}


long readlongparam(char* fname){
   long num;
    long pr;
    FILE *tmp;
    tmp=fopen(fname,"r");

    if(tmp == NULL)
    {
 	printf("Error '%s'",tmp);
	return;
    }


    fscanf(tmp,"%lu\n",  &pr);
    fclose(tmp);
    return pr;
}


char* readstringparam(char* fname){
   char* num;
    char* pr;
    FILE *tmp;
    tmp=fopen(fname,"r");

    if(tmp == NULL)
    {
 	printf("Error '%s'",tmp);
	return;
    }


    fscanf(tmp,"%s\n",  &pr);
    fclose(tmp);
    return pr;
}



void writelongparam(char* fname,long parm){
    FILE *tmp;
    tmp=fopen(fname,"w");

    if(tmp == NULL)
    {
 	printf("Error '%s'",tmp);
	return;
    }


    fprintf(tmp,"%lu\n",  parm);
    fclose(tmp);
}


float readfloatparam(char* fname){
   float num;
    float pr;
    FILE *tmp;
    tmp=fopen(fname,"r");

    if(tmp == NULL)
    {
 	printf("Error '%s'",tmp);
	return;
    }


    fscanf(tmp,"%f\n",  &pr);
    fclose(tmp);
    return pr;
}





int readbitrate(){
   int num;
    int pr;
    FILE *tmp;
    tmp=fopen("/tmp/bitrate","r");

    if(tmp == NULL)
    {
 	printf("Error '%s'",tmp);
	return;
    }


    fscanf(tmp,"%i\n",  &pr);
    fclose(tmp);
    return pr;
}


int readpingtest(){
   int num;
    int pr;
    FILE *tmp;
    tmp=fopen("/tmp/pingtest","r");

    if(tmp == NULL)
    {
 	printf("Error '%s'",tmp);
	return;
    }


    fscanf(tmp,"%i\n",  &pr);
    fclose(tmp);
    return pr;
}



void getstring(){
FILE *file; 
    char *fname = "/tmp/title.cp1251";
 
    file = fopen(fname,"r");

    result_string[0]=0x00;

 
    if(file == NULL)
    {
 	printf("Error '%s'",fname);
	return;
    }
 
    int i=0;
    char *real_tail;

    
    while(fgets(result_string+strlen(result_string),sizeof(result_string),file))
    {
	real_tail="";
        
/*        int k;       

        for(k=strlen(result_string);k<strlen(result_string)+17;k++){
           result_string[k]=" ";
        }
           result_string[k]=0x00;
*/
	if(result_string[strlen(result_string)-1] == '\n')//проверяем является ли последний элемент в строке символом её окончания
	{
	    real_tail="\\n";
	    result_string[strlen(result_string)-1]=' ';
	};// эта часть кода добавлена лишь для отображения символа конца строки в консоль без перевода на новую строку	
 
 }
 
    fclose(file);
}


void getnamestation(){
FILE *file; 
    char *fname = "/tmp/streamname.cp1251";
 
    file = fopen(fname,"r");

    result_stringn[0]=0x00;

 
    if(file == NULL)
    {
	printf("Error '%s'",fname);
        fclose(file);
	return;
    }
 
    int i=0;
    char *real_tail;


    
    while(fgets(result_stringn+strlen(result_stringn),sizeof(result_stringn),file))
    {
	real_tail="";
        
/*        int k;       

        for(k=strlen(result_string);k<strlen(result_string)+17;k++){
           result_string[k]=" ";
        }
           result_string[k]=0x00;
*/
	if(result_stringn[strlen(result_stringn)-1] == '\n')//проверяем является ли последний элемент в строке символом её окончания
	{
	    real_tail="\\n";
	    result_stringn[strlen(result_stringn)-1]=' ';
	};// эта часть кода добавлена лишь для отображения символа конца строки в консоль без перевода на новую строку	
 
 }
 
    fclose(file);
}


int getrssi(){
FILE *file; 
    char *fname = "/tmp/rssi";

    result_rssi[0]=0x00;
    result_rssi[1]=0x00;
    result_rssi[2]=0x00;
    result_rssi[3]=0x00;
    result_rssi[4]=0x00;
    result_rssi[5]=0x00;
    result_rssi[6]=0x00;
    result_rssi[7]=0x00;

 
    file = fopen(fname,"r");
 
    if(file == NULL)
    {
 	printf("Error '%s'",fname);
//        fclose(file);
	return -1;
    }
 
    int i=0;
    char *real_tail;

    
    while(fgets(result_rssi+strlen(result_rssi),sizeof(result_rssi),file))
    {
	real_tail="";
        
	if(result_rssi[strlen(result_rssi)-1] == '\n')//проверяем является ли последний элемент в строке символом её окончания
	{
	    real_tail="\\n";
	    result_rssi[strlen(result_rssi)-1]=' ';
	};// эта часть кода добавлена лишь для отображения символа конца строки в консоль без перевода на новую строку	
 
 }
 
    fclose(file);
    return 0;
}



void loop() {
 // int len_str  = 0;
  unsigned char k=0;
   k=0;
   for (unsigned char i=0 ; i < 30 ; i++) string_lcd[i] =0x20; // Производим побайтовый сдвиг влево,

    while (result_string[k])  {  // Пока не конец строки ...    
           for (unsigned char i=2 ; i < 13 ; i++) string_lcd[i] = string_lcd[i+1] ; // Производим побайтовый сдвиг влево,
           string_lcd[13] = result_string[k] ;                                  // отображаемой на LCD строки, и добавляем
           k++;                                                  // символ в последний байт.
//           len_str++ ; // Счётчик длины строки.
//           LCD_Clear();
            for (unsigned char i=13 ; i >=3 ; i--) {  // Отображаем на LCD посимвольно.
                 LCD_Goto(i+1,1);
                 LCD_Write_Char(string_lcd[i]);
//                 LCD_Goto(1,1);
//                 LCD_Write_Char(' ');
             }
             usleep(200000);

        if ((fp = fopen("/tmp/lirc/source", "r")) != NULL){
	    return;
        }

        if ((fp = fopen("/tmp/lirc/0", "r")) != NULL){
	    return;
        }

        if ((fp = fopen("/tmp/lirc/1", "r")) != NULL){
	    return;
        }

        if ((fp = fopen("/tmp/lirc/2", "r")) != NULL){
	    return;
        }

        if ((fp = fopen("/tmp/lirc/3", "r")) != NULL){
	    return;
        }

        if ((fp = fopen("/tmp/lirc/4", "r")) != NULL){
	    return;
        }

        if ((fp = fopen("/tmp/lirc/5", "r")) != NULL){
	    return;
        }

        if ((fp = fopen("/tmp/lirc/6", "r")) != NULL){
	    return;
        }

        if ((fp = fopen("/tmp/lirc/7", "r")) != NULL){
	    return;
        }


        if ((fp = fopen("/tmp/lirc/8", "r")) != NULL){
	    return;
        }


        if ((fp = fopen("/tmp/lirc/9", "r")) != NULL){
	    return;
        }


        if ((fp = fopen("/tmp/lirc/power", "r")) != NULL){
	    return;
        }

 
if ((fp = fopen("/tmp/newchannel.flg", "r")) != NULL){
  remove("/tmp/newchannel.flg");
  refreshbitrate=1;
  return;
}


	    rev++;
	    if(rev==4){rev=0;}
//         Пишем номер станции
	    LCD_Goto(1,1);
	    int num=readnumstation();
	    LCD_Write_Int(num);
	    if(num<10){
	    LCD_Goto(2,1);
	    LCD_Write_Char(' ');
	    }
/*	    if((num<100)&&(num>9)){
	    LCD_Goto(3,1);
	    LCD_Write_Char(' ');
	    }*/
	    //Состояние интернета
	    LCD_Goto(2,2);
	    int net=readpingtest();
	    oldstat=net;
	    if(net==0){
		if(rev==0){
			LCD_Write_Char(0x03);
		};
		if(rev==2){
			LCD_Write_Char(' ');
		};
		
	    }else{
			LCD_Write_Char(0x02);
	    }
	    //Источник звука
//	    LCD_Goto(4,1);
//            LCD_Write_Char(0x00);

	//Выведем время
        char buf[100];
        time_t t = time(NULL);
        strftime(buf, BUFSIZ, "%R", localtime(&t));
//        printf("%s\n", buf);
    
        LCD_Goto(12,2);
        LCD_Write_String(buf);
          
         //Выведем RSSI

         if(getrssi()==0){
            LCD_Goto(4,2);
            LCD_Write_String(result_rssi);
	 }


  }
}


int main (int argc, char *argv[]) { 
// обрабатываем коммандную строку
   int i=0;
   int st_init=1;
         for(i = 0 ; i < argc; i++) {
//		printf("Argument %d: %s\n", i, argv[i]);
                 if(!strcmp(argv[i], "-noinit")){
		    printf("Init display: no");
		    st_init=0;
		 }
         }
// закончили искать инициализацию
    if(st_init==1){
      LCD_Init();
//      sleep(0.2);
      LCD_Clear();
//      sleep(0.2);
      load_cgram();
//      sleep(0.5);
//      sleep(1);
    }else{
	LCD_Open();
    }

	    LCD_Goto(1,2);
            LCD_Write_Char(0x01);
	    LCD_Goto(3,1);
            LCD_Write_Char(0x00);
	    LCD_Goto(3,2);
            LCD_Write_Char(0x00);
	    LCD_Goto(15,1);
            LCD_Write_Char(0x00);

          //Инициализация констант
          savedprobeg=readlongparam("/automedia/savedprobeg.const");

//          system( "/automedia/says/start.sh &");


	while(1){
           
           if(mode==0){//Режим инет радио

	   if(refreshbitrate==1){
           //рисуем битрейт    
           int bitrate=readbitrate();
           if((bitrate<128)&(bitrate>0)){
	    LCD_Goto(16,1);
            LCD_Write_Char(0x06);
            refreshbitrate=0;
           }
           if(bitrate==128){
	    LCD_Goto(16,1);
            LCD_Write_Char(0x05);
            refreshbitrate=0;
           }
           //рисуем битрейт    
           if(bitrate>128){
	    LCD_Goto(16,1);
            LCD_Write_Char(0x04);
            refreshbitrate=0;
           }
           //рисуем битрейт    
           if(bitrate==0){
	    LCD_Goto(16,1);
            LCD_Write_Char(' ');
           }
	}

// рисуем первую бегущую строку
	    getstring();
	    loop();

        }//Конец режима инет радио

       //Здесь сообщения о превышениях температур
       int toj=readlongparam("/tmp/mikas/temp");
       if(say40==0){
            if(toj>40){
    	    say40=1;
            system( "/automedia/says/temp40.sh &");
            }
	}

       if(say98==0){
            if(toj>98){
    	    say98=1;
            system( "/automedia/says/temp98.sh &");
            }
	}	

       if(toj<97){
          if(say98==1){
             say98=0;
          }
       }
       if(toj<39){
          if(say40==1){
             say40=0;
          }
       }

       if(mode==1){//Режим одометра
         
         probeg=readlongparam("/tmp/mikas/probeg");
         sutochn=probeg-savedprobeg;
         LCD_Goto(8,1);
	 LCD_Write_Int2Dec(8,probeg,2);
         LCD_Goto(8,2);
	 LCD_Write_Int2Dec(8,sutochn,2);
         usleep(300000);

       }//Конец режима одометра


       if(mode==2){//Режим ресходомера
         
         long rashchas=(float)readfloatparam("/tmp/mikas/rashchas")*100;
         long rashod=(float)readfloatparam("/tmp/mikas/rashod")*100;
         LCD_Goto(12,1);
	 LCD_Write_Int2Dec(4,rashchas,2);
         LCD_Goto(12,2);
	 LCD_Write_Int2Dec(4,rashod,2);
         usleep(300000);

       }//Конец режима расходомера


       if(mode==3){//Режим температуры и оборотов
         
         int oboroti=readlongparam("/tmp/mikas/oboroti");
         LCD_Goto(12,1);
	 LCD_Write_Int(toj);
         LCD_Goto(12,2);
	 LCD_Write_Int(oboroti);
         usleep(300000);

       }//Конец режима одометра


       if(mode==4){//Режим скорость и обороты
         
         int speed=readlongparam("/tmp/mikas/speed");
         int oboroti=readlongparam("/tmp/mikas/oboroti");
         LCD_Goto(12,1);
	 LCD_Write_Int(speed);
         LCD_Goto(12,2);
	 LCD_Write_Int(oboroti);
         usleep(300000);

       }//Конец режима одометра



       
        //Обработка файлов статусов
        if ((fp = fopen("/tmp/lirc/mute", "r")) != NULL){
           remove("/tmp/lirc/mute");
           if(mode==1){
	     writelongparam("/automedia/savedprobeg.const",probeg);
	     savedprobeg=probeg;
	   }
        }

        if ((fp = fopen("/tmp/lirc/power", "r")) != NULL){
           remove("/tmp/lirc/power");
	     if(power==0){
	        power=1;
                LCD_BL(power);
                system( "echo 1 > /sys/bus/usb/devices/2-1/authorized");
                system( "echo 1 > /sys/bus/usb/devices/4-1/authorized");
                system( "/automedia/startupcron.sh &");
	     }else{
	        power=0;
                LCD_BL(power);
                system( "echo 0 > /sys/bus/usb/devices/2-1/authorized");
                system( "echo 0 > /sys/bus/usb/devices/4-1/authorized");
                system( "killall startupcron.sh");
                system( "killall watchdog.sh");
                system( "killall mikas");
                system( "killall pingtest.sh");
	     }
        }

        if ((fp = fopen("/tmp/lirc/0", "r")) != NULL){
           remove("/tmp/lirc/0");
	   mode=0;
	    LCD_Goto(1,1);
    	    LCD_Write_String("                ");
	    LCD_Goto(1,2);
    	    LCD_Write_String("                ");

	    LCD_Goto(1,2);
            LCD_Write_Char(0x01);
	    LCD_Goto(3,1);
            LCD_Write_Char(0x00);
	    LCD_Goto(3,2);
            LCD_Write_Char(0x00);
	    LCD_Goto(15,1);
            LCD_Write_Char(0x00);

        }

        if ((fp = fopen("/tmp/lirc/1", "r")) != NULL){
           remove("/tmp/lirc/1");
	   mode=1;

		    LCD_Goto(1,1);
    		    LCD_Write_String("Пробег          ");
		    LCD_Goto(1,2);
    		    LCD_Write_String("Суточн          ");

        }

        if ((fp = fopen("/tmp/lirc/2", "r")) != NULL){
           remove("/tmp/lirc/2");
	   mode=2;

		    LCD_Goto(1,1);
    		    LCD_Write_String("л/час           ");
		    LCD_Goto(1,2);
    		    LCD_Write_String("л/100км         ");

        }

        if ((fp = fopen("/tmp/lirc/3", "r")) != NULL){
           remove("/tmp/lirc/3");
	   mode=3;

		    LCD_Goto(1,1);
    		    LCD_Write_String("ОЖ град         ");
		    LCD_Goto(1,2);
    		    LCD_Write_String("Обороты         ");

        }
        if ((fp = fopen("/tmp/lirc/4", "r")) != NULL){
           remove("/tmp/lirc/4");
	   mode=4;

		    LCD_Goto(1,1);
    		    LCD_Write_String("Скорость        ");
		    LCD_Goto(1,2);
    		    LCD_Write_String("Обороты         ");

        }


        if ((fp = fopen("/tmp/lirc/source", "r")) != NULL){
           remove("/tmp/lirc/source");

	   mode++;
           if(mode==5){
              mode=0;
           }


	    //Инициализация дисплея
	    if(mode==0){
	    LCD_Goto(1,1);
    	    LCD_Write_String("                ");
	    LCD_Goto(1,2);
    	    LCD_Write_String("                ");

	    LCD_Goto(1,2);
            LCD_Write_Char(0x01);
	    LCD_Goto(3,1);
            LCD_Write_Char(0x00);
	    LCD_Goto(3,2);
            LCD_Write_Char(0x00);
	    LCD_Goto(15,1);
            LCD_Write_Char(0x00);
	    }
	    if(mode==1){
		    LCD_Goto(1,1);
    		    LCD_Write_String("Пробег          ");
		    LCD_Goto(1,2);
    		    LCD_Write_String("Суточн          ");
	    }
	    if(mode==2){
		    LCD_Goto(1,1);
    		    LCD_Write_String("л/час           ");
		    LCD_Goto(1,2);
    		    LCD_Write_String("л/100км         ");
	    }

	    if(mode==3){
		    LCD_Goto(1,1);
    		    LCD_Write_String("ОЖ град         ");
		    LCD_Goto(1,2);
    		    LCD_Write_String("Обороты         ");
	    }

	    if(mode==4){
		    LCD_Goto(1,1);
    		    LCD_Write_String("Скорость        ");
		    LCD_Goto(1,2);
    		    LCD_Write_String("Обороты         ");
	    }


        }


	}


return 0;
}
