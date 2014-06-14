//===================================================================================
//������
//http://www.raio.com.tw
//2010/xx/xx
//RA8875  
//Mountain	
//===================================================================================



//�Ĥ@���ϥνЭ��s��q�H�U�ƭ�,�̷ӭӧOTP&LCD�L��

#define XMIN 0	    //�w�q���W��X�bADC��   LCD(0,0)
#define YMIN 0	    //�w�q���W��Y�bADC�� 

#define XMAX 1791	//�w�q�k�U��X�bADC��   LCD(799,479)
#define YMAX 1023	//�w�q�k�U��*�bADD��
  


#define center_x  239
#define center_y  159

uchar backlight = 5;//define initail value for backlight 1~5

uchar counter1 = 0;
uchar counter2 = 0;

// 	unsigned char str[25];
//	unsigned char str1[1];

char  RAiO_DS1[] = "������(RaiO Tech)��88�~8��8��O�Ѥ@�s���O�ʹq�l�θ�T�q�l���~��h�~�]�p�B��P�B�޲z���M�~�H�~�ҳХߡA�D�n�q��IC�]�p�ξP��A�}�ӱ��Ȥ�e���]�p������޹q�l���q�A�����M�ΰꤺ�~�C" ;
char  RAiO_DS2[] = "RA8875 TFT LCD����A�䴩�r����ܡB2Dø�ϥ[�t�BDMA�ϸ�h���B����Touch panel ����A��TFT LCD�����²��A���ΰ�����CPU�]�i�H�F���}�n����ܮĪG�C" ;
//char  RAiO_DS3[] = "������ʾ֧Ԯ�Ϻ���ͨ�ֿ�IC���5�֣���16*16 24*24 32*32���壬֧ԮBIG5 GB2312 GB12345 UNICODE ASCII UNIJIS JIS0208 LATIN ����?" ;

bit Graphic_Cursor_on_flag = 0;

extern bit touch_valid;

extern uint LCD_X,LCD_Y;
extern uint LCD_X1,LCD_Y1,LCD_X2,LCD_Y2,LCD_X3,LCD_Y3,LCD_X4,LCD_Y4,LCD_X5,LCD_Y5;
extern uchar nr_of_touches;	 

//extern uint LCD_X_temp,LCD_Y_temp;

//extern uint ADC_value_X,ADC_value_Y;

//extern uint X1,Y1,X2,Y2,X3,Y3,X4,Y4;

extern bit  touch_flag ;//
extern bit  button_Font_flag; //
extern bit  button_Draw_flag ;
extern bit	button_Touch_flag;
extern bit	button_BTE_flag ;
extern bit	button_DMA_flag ;
extern bit	button_PWM_flag ;
//extern bit	button_Cursor_flag ;
extern bit button_Clear_flag ;
extern bit button_Exit_flag ;
extern bit button_Exit2_flag;

extern bit minus_button_flag ;
extern bit plus_button_flag ;


extern uchar str[25];
extern uchar str1[1];
extern bit touch_levle2_flag;


void RA_Box_create(unsigned int x0,unsigned int y0,unsigned int xsize,unsigned int ysize, char *str)
{

    Graphic_Mode();
    Text_Foreground_Color(0,0,0);
    Geometric_Coordinate(x0,x0+xsize-1,y0,y0+ysize-1);
    Draw_square();
    Chk_Busy();

    Text_Foreground_Color(20,40,20);
    Geometric_Coordinate(x0+1,x0+xsize-2,y0+1,y0+ysize-2);
    Draw_square_fill();
    Chk_Busy();

	Text_Foreground_Color(0x07,0x07,0xff);
    Geometric_Coordinate(x0+3,x0+xsize-4,y0+3,y0+30);
    Draw_square_fill();
    Chk_Busy();

	Text_Foreground_Color(0xff,0xff,0xff);
	Text_Background_Color(0x07,0x07,0x3f);

	Font_Coordinate(x0+5,y0+5);//set font start point
    Text_Mode();
	LCD_CmdWrite(0x02);
	while(*str != '\0')
	{
	 LCD_DataWrite(*str);
	 ++str;	 	
	 Chk_Busy();		
	}



}

void RA_Button_create(unsigned int x0,unsigned int y0,unsigned char xsize,unsigned char ysize,unsigned int fx,unsigned int fy,char *str,bit state,bit type)
{
  if(type==0)// type=0 2d button  type = 1 3d button
  {
    Graphic_Mode();
    Text_Foreground_Color(0x00,0x00,0x00);
    Geometric_Coordinate(x0,x0+xsize-1,y0,y0+ysize-1);
    Draw_square();
    Chk_Busy();
    if(state) //if = 0 normal  if = 1 click
    Text_Foreground_Color(0xff,0xff,0xff);
    else
    Text_Foreground_Color(0x10,0x20,0x10); 

    Geometric_Coordinate(x0+1,x0+xsize-2,y0+1,y0+ysize-2);
    Draw_square_fill();
    Chk_Busy(); 
    Text_Foreground_Color(0x00,0x00,0x00); //set font color
  
    if(state)   //if = 0 normal  if = 1 click
    Text_Background_Color(0xff,0xff,0xff);
    else
    Text_Background_Color(0x10,0x20,0x10);
  }
  else 
  { //3D button setting 
    Graphic_Mode();
    Text_Foreground_Color(0x00,0x00,0x00);
    Geometric_Coordinate(x0,x0+xsize-1,y0,y0+ysize-1);
    Draw_square();
    Chk_Busy();

    Text_Foreground_Color(0xff,0xff,0xff);
    Geometric_Coordinate(x0+1,x0+xsize-2,y0+1,y0+1);
	Draw_line();
    Chk_Busy();
    Text_Foreground_Color(0xff,0xff,0xff);
    Geometric_Coordinate(x0+1,x0+1,y0+1,y0+ysize-2);
	Draw_line();
    Chk_Busy();

	Text_Foreground_Color(0x0f,0x1f,0x0f);
    Geometric_Coordinate(x0+xsize-2,x0+xsize-2,y0+2,y0+ysize-2);
	Draw_line();
    Chk_Busy();
    Text_Foreground_Color(0x0f,0x1f,0x0f);
    Geometric_Coordinate(x0+2,x0+xsize-2,y0+ysize-2,y0+ysize-2);
	Draw_line();
    Chk_Busy();

    if(state) //if = 0 normal  if = 1 click
     Text_Foreground_Color(0x00,0x00,0xff);
    else
     Text_Foreground_Color(0x10,0x20,0x10);

    Geometric_Coordinate(x0+2,x0+xsize-3,y0+2,y0+ysize-3);
    Draw_square_fill();
    Chk_Busy(); 


    Text_Foreground_Color(0x00,0x00,0x00); //set font color 
    if(state)   //if = 0 normal  if = 1 click
    Text_Background_Color(0xff,0xff,0xff);
    else
    Text_Background_Color(0x10,0x20,0x10);

   }

    Font_Coordinate(fx,fy);//set font start point
    Text_Mode();
	LCD_CmdWrite(0x02);
	while(*str != '\0')
	{
	 LCD_DataWrite(*str);
	 ++str;	 	
	 Chk_Busy();		
	}
  
}





void button_Font(bit state)
{
  RA_Button_create(640,0,160,80,680,28,"Font",state,1);
}


void button_Draw(bit state)
{
  RA_Button_create(640,80,160,80,680,108,"Draw",state,1);
}

void button_Touch(bit state)
{
  RA_Button_create(640,160,160,80,680,188,"Touch",state,1);
}

void button_BTE(bit state)				 
{
  RA_Button_create(640,240,160,80,680,268,"BTE",state,1);
}

void button_DMA(bit state)
{
  RA_Button_create(640,320,160,80,680,348,"DMA",state,1);
}

void button_PWM(bit state)
{
  RA_Button_create(640,400,160,80,680,428,"PWM",state,1);
}


void button_Clear(bit state)
{
  RA_Button_create(121,400,100,60,141,416,"Clear",state,1);
}


void button_Exit(bit state)
{
  RA_Button_create(20,400,100,60,40,416,"Exit",state,1);
}


void button_Exit2(bit state)
{
  RA_Button_create(10,150,80,40,20,155,"Exit",state,1);
}


void button_Exit3(bit state)
{
//  RA_Button_create(719,439,80,40,20,155,"Exit",state,1);
  RA_Button_create(499,419,100,60,519,435,"Exit",state,1);
}

void button_Clear3(bit state)
{
  RA_Button_create(399,419,100,60,419,435,"Clear",state,1);
}

void detect_touch(void)
{  
	 
}



//==============================================================================
//Subroutine:	Show Hex
//Function:		
//==============================================================================
/*
void Print_Hex(uchar buf)
{
	uchar temp;
	temp=buf;

	temp = (temp >>4) & 0x0F;
	if(temp < 0x0A)
	{
		temp |= 0x30;
	}
	else temp = temp + 0x37;
	LCD_CmdWrite(0x02);
	LCD_DataWrite(temp);
	Delay1ms(1);
//	Delay100us(1);

	temp=buf;
	temp = temp & 0x0F;
	if(temp < 0x0A)
	{
		temp |= 0x30;
	}
	else temp = temp + 0x37;
	LCD_DataWrite(temp);
	Delay1ms(1);
//	Delay100us(1);
}
*/


void confirm_button_touch(void)
{
 
 if(button_Touch_flag==0)
 {  
 
 if((LCD_X1 >= 640) && (LCD_X1<800))
  {
    if((LCD_Y1 >= 0) && (LCD_Y1<80))
	  { button_Font_flag = 1 ;Clear_ALL_XY_Value();}
      else if((LCD_Y1 >= 80) && (LCD_Y1<160))
	   { button_Draw_flag = 1;Clear_ALL_XY_Value();}
		else if((LCD_Y1 >= 160) && (LCD_Y1<240))
	     { button_Touch_flag = 1;Clear_ALL_XY_Value();}
		  else if((LCD_Y1 >= 240) && (LCD_Y1<320))
	       { button_BTE_flag = 1;Clear_ALL_XY_Value();}
		    else if((LCD_Y1 >= 320) && (LCD_Y1<400))
	         { button_DMA_flag = 1;Clear_ALL_XY_Value();}
			  else if((LCD_Y1 >= 400) && (LCD_Y1<480))
	           { button_PWM_flag = 1;Clear_ALL_XY_Value();}
     }
   }
  if(button_Font_flag|button_Draw_flag|button_Touch_flag|button_BTE_flag|button_DMA_flag|button_PWM_flag)
  {
	 if	((LCD_Y1 >= 400) && (LCD_Y1<460))
	  {
	   	 if((LCD_X1 >= 121) && (LCD_X1<221))
		 {
	       if(button_Touch_flag)  
	       button_Clear_flag = 1 ;
		   {Clear_ALL_XY_Value();}
		  }

	   else if ((LCD_X1 >= 20) && (LCD_X1<120))
		 {button_Exit_flag = 1 ;
          Clear_ALL_XY_Value();}

	   } 	  				   
    }


	if(button_PWM_flag)
	{

	  if((LCD_X1 >= 10) && (LCD_X1<90))
	  {
	   if((LCD_Y1 >= 155) && (LCD_Y1<195))
	      {button_Exit2_flag=1;
		  Clear_ALL_XY_Value();}
    	}


	  if((LCD_Y1>= 65) && (LCD_Y1<115))
	  {
	   if((LCD_X1 >= 15) && (LCD_X1<55))
	    {  minus_button_flag=1;
		     Clear_ALL_XY_Value();}
		else if((LCD_X1 >= 264) && (LCD_X1<304))
		   {plus_button_flag=1;
		       Clear_ALL_XY_Value();}
    	}
	}
}
						   

void Clear_all_flag(void)
{
  button_Font_flag = 0;//
  button_Draw_flag = 0;
  button_Touch_flag = 0;
  button_BTE_flag = 0;
  button_DMA_flag = 0;
  button_PWM_flag = 0;
  button_Clear_flag = 0;
  button_Exit_flag = 0;
}


void Show_first_page(void)
{
    uchar i;

    Font_with_BackgroundTransparency();//�r��I�����z���
	Active_Window(0,639,0,479);
	Text_Background_Color(31,61,2);
	Memory_Clear();	
	
	Text_Foreground_Color(0xff,0x00,0xff);
	Text_Background_Color(0x00,0x00,0xff);
	for(i=0;i<5;i++)
	{
    Geometric_Coordinate(0+i,639-i,0+i,479-i);
    Draw_square();
    Chk_Busy();
	}
	Text_Foreground_Color(0x00,0x00,0xff);
    Horizontal_FontEnlarge_x2();
	//Vertical_FontEnlarge_x2();

	Font_Coordinate(10,10);
	Show_String("RA",0);
	Text_Foreground_Color(0xff,0x00,0x00);
	Show_String("i",0);
	Text_Foreground_Color(0x00,0x00,0xff);
	Show_String("O",0);

	Text_Foreground_Color(0,31,0);
    Horizontal_FontEnlarge_x1();
	Vertical_FontEnlarge_x1();

	Font_Coordinate(10,50);
	Show_String("RA8875 TFT LCD controller",0);
	Font_Coordinate(10,80);
	Show_String("Resolution : 480x272,640x480 up to 800x480",0);

	Font_Coordinate(10,110);
	Show_String("MPU interface : 8/16 Bits",0);
    Font_Coordinate(10,140);
	Show_String("Serial MCU I/F : SPI3/4 wire & IIC",0);
	Font_Coordinate(10,170);
    Show_String("Embedded display RAM : 768 KByte",0);
	Font_Coordinate(10,200);
    Show_String("Embedded ASCII Font : ISO-8859 Latin1~4",0);
    Font_Coordinate(10,230);
    Show_String("External ROM Font I/F : Serial (GENITOP)",0);
	Font_Coordinate(10,260);
    Show_String("Touch panel controller : 4 wire",0);
	Font_Coordinate(10,290);
    Show_String("Operation temperature : -30 ~ +85 degrees",0);

	Active_Window(0,799,0,479);

}

void Show_button_Font_function(void)
{
 uint i;
 uchar h;

 button_Font(1);
 Active_Window(0,639,0,479);	

 Text_Background_Color(0xff,0xff,0xff);
 Memory_Clear();

 button_Exit(0);

 Active_Window(10,629,10,479);

 Font_code_BIG5();
 Text_Foreground_Color(0x00,0x00,0x00);
 Font_Coordinate(10,10);			 
 Show_String("�c�餤��:",0);
 Text_Foreground_Color(0x1f,0x01,0x1f);
 Font_Coordinate(10,40);
 Show_String(RAiO_DS2,0);


 Font_code_GB2312();
 Font_Coordinate(10,140);
 Text_Foreground_Color(0x00,0x00,0x00);
 Show_String("��������:",0);
 Text_Foreground_Color(0x00,0x00,0x3f);
 Font_Coordinate(10,170);
 //Show_String(RAiO_DS3,0);


 Scroll_Window(0,639,0,399); //640*400 �ù�����

  while(button_Font_flag)
  {

	  do{
		  detect_touch();
	      confirm_button_touch();
		  Delay100ms(1);
		  h++;
	   }while(h<50& (!button_Exit_flag));
	   Text_Cursor_Disable();


     if(!button_Exit_flag)
	 {

	   for(i=0;i<400;i++)
      {
        Scroll_Offset(0,i);
	    h=0;
	    do{
		  detect_touch();
	      confirm_button_touch();
		  Delay10ms(1);
		  h++;
	    }while(h<1&(!button_Exit_flag));
	    if(button_Exit_flag)
	    break;
        }
	   }

	 if(!button_Exit_flag)
	 {
	   Scroll_Offset(0,0);

	  for(i=399;i>0;i--)
      {
        Scroll_Offset(0,i);
	    h=0;
	    do{
		  detect_touch();
	      confirm_button_touch();
		  Delay1ms(1);
		  h++;
	    }while(h<1&(!button_Exit_flag));
	    if(button_Exit_flag)
	    break;
        }
	   }
	  Scroll_Offset(0,0);
	
	 if(button_Exit_flag)
	  {
	   Text_Cursor_Disable();
	   button_Exit(1);
	   Delay100ms(5);
	   button_Exit(0);
	   button_Exit_flag = 0;
	   button_Font_flag = 0;
	  }


   }	 
 Clear_all_flag(); 
 Active_Window(0,799,0,479);
 button_Font(0); 
 Show_first_page();
}



void Show_button_Draw_function(void)
{
   uchar j;

    button_Draw(1);
  
    Active_Window(0,639,0,479);	
    Text_Background_Color(0x00,0x00,0x00); //
	Memory_Clear();

    button_Exit(0); 

do{

//-----Triangle------------------------ 
   if(!button_Exit_flag)
   {
    Active_Window(0,119,0,48);	
    Background_color(color_black); //
	Memory_Clear();
    Text_Foreground_Color(0xff,0xff,0xff);
    Font_Coordinate(0,0);
    Show_String("Triangle",0);

	Active_Window(120,639,0,399);	
    Background_color(color_grayscale10); //
	Memory_Clear();


	j=0;
    do{ 

    Text_Foreground_Color(j,j*6,j+4);
    Draw_Triangle_3point_Coordinate(120+j,399-j,120+j*2,0,639-j,399-j);
    Draw_Triangle();
    Chk_Busy();

	j++;
		detect_touch();
	    confirm_button_touch();
	    Delay10ms(3);

	}while(j<250 & !button_Exit_flag);
   }

  if(!button_Exit_flag)
   {
    Active_Window(0,119,0,48);	
    Background_color(color_black); //
	Memory_Clear();
	Text_Foreground_Color(0xff,0xff,0xff);
    Font_Coordinate(0,0);
    Show_String("Triangle",0);

	Font_Coordinate(0,24);
    Show_String("Fill",0);

    Active_Window(120,639,0,399);	
    Background_color(color_grayscale10); //
	Memory_Clear();

	j=0;
    do{ 

    Text_Foreground_Color(j*3,j+4,j*8);
    Draw_Triangle_3point_Coordinate(120+j,399-j,639-j*2,0,639-j,399-j);
    Draw_Triangle_fill();
    Chk_Busy();

	j++;
		detect_touch();
	    confirm_button_touch();
	    Delay10ms(3);

	}while(j<250 & !button_Exit_flag);

   }
//---------------------------------------
//-----Draw line------------------------- 

 if(!button_Exit_flag)
   {
    Active_Window(0,119,0,48);	
    Background_color(color_black); //
	Memory_Clear();

	Text_Foreground_Color(0xff,0xff,0xff);
    Font_Coordinate(0,0);
    Show_String("Line",0);

    Active_Window(120,639,0,399);	
    Background_color(color_grayscale10); //
	Memory_Clear();

	j=0;
    do{ 

    Text_Foreground_Color(j*2,j+4,j+7);
    Geometric_Coordinate(120,120+j*5,0,399);
    Draw_line();
    Chk_Busy();

	j++;
		detect_touch();
	    confirm_button_touch();
	    Delay10ms(1);

	}while(j<103 & !button_Exit_flag);

	if(!button_Exit_flag)
    {
	 	j=0;
     do{ 

      Text_Foreground_Color(j*5,j+8,j*2);
      Geometric_Coordinate(639,639-j*5,0,399);
      Draw_line();
      Chk_Busy();

	  j++;
		detect_touch();
	    confirm_button_touch();
	    Delay10ms(1);

	  }while(j<103 & !button_Exit_flag);

     }

   }

 //---------------------------------------

 //---------Circle------------------------
   if(!button_Exit_flag)
   {
    Active_Window(0,119,0,48);	
    Background_color(color_black); //
	Memory_Clear();
	Text_Foreground_Color(0xff,0xff,0xff);
    Font_Coordinate(0,0);
    Show_String("Circle",0);


    Active_Window(120,639,0,399);	
    Background_color(color_grayscale10); //
	Memory_Clear();

	j=0;
    do{ 

       Text_Foreground_Color(j+3,j*7,j*2);
    Circle_Coordinate_Radius(380,200,j*3);
    Draw_circle();
    Chk_Busy();

	j++;
		detect_touch();
	    confirm_button_touch();
	    Delay10ms(3);

	}while(j<64& !button_Exit_flag);

	 if(!button_Exit_flag)
     {
		Text_Foreground_Color(0xff,0xff,0xff);
        Font_Coordinate(0,24);
        Show_String("fill",0);

	  	j=0;
       do{ 

       Text_Foreground_Color(j+3,j*7,j*2);
       Circle_Coordinate_Radius(380,200,199-j*5);
       Draw_circle_fill();
       Chk_Busy();

	   j++;
		detect_touch();
	    confirm_button_touch();
	    Delay10ms(3);

	  }while(j<39 & !button_Exit_flag);
	 }
   }
//---------------------------------------

//--------------Square-------------------
   if(!button_Exit_flag)
   {
    Active_Window(0,119,0,48);	
    Background_color(color_black); //
	Memory_Clear();
	Text_Foreground_Color(0xff,0xff,0xff);
    Font_Coordinate(0,0);
    Show_String("Square",0);


    Active_Window(120,639,0,399);	
    Background_color(color_grayscale10); //
	Memory_Clear();

	j=0;
    do{ 
     Text_Foreground_Color(j*7,j*2,j);
     Geometric_Coordinate(120+j*5,639-j*5,0+j*4,399-j*4);
     Draw_square();
     Chk_Busy();

	 j++;
		detect_touch();
	    confirm_button_touch();
	    Delay10ms(5);

	}while(j<49 & !button_Exit_flag);

	 if(!button_Exit_flag)
     {
		Text_Foreground_Color(0xff,0xff,0xff);
        Font_Coordinate(0,24);
        Show_String("fill",0);

	  	j=0;
      do{ 
        Text_Foreground_Color(j,j*2,j*6);
        Geometric_Coordinate(120+j*5,639-j*5,0+j*4,399-j*4);
        Draw_square_fill();
        Chk_Busy();

	    j++;
		detect_touch();
	    confirm_button_touch();
	    Delay10ms(5);

	  }while(j<39 & !button_Exit_flag);
	 }
   }

//--------------------------------------- 


//-------------Draw_Circle_Square-------------------
   if(!button_Exit_flag)
   {
    Active_Window(0,119,0,48);	
    Background_color(color_black); //
	Memory_Clear();
	Text_Foreground_Color(0xff,0xff,0xff);
    Font_Coordinate(0,0);
    Show_String("Circle",0);
	Font_Coordinate(0,24);
    Show_String("square",0);

    Active_Window(120,639,0,399);	
    Background_color(color_grayscale10); //
	Memory_Clear();

	j=0;
    do{ 
       Text_Foreground_Color(j*7,j*2,j);
       Circle_Square_Coordinate_setting(120+j*18,639-j*18,0+j*15,399-j*15,10+j*5,10+j*5);
       Draw_Circle_Square();
       Chk_Busy();

	 j++;
		detect_touch();
	    confirm_button_touch();
	    Delay10ms(20);

	}while(j<10 & !button_Exit_flag);

	 if(!button_Exit_flag)
     {
		Text_Foreground_Color(0xff,0xff,0xff);
        Font_Coordinate(0,48);
        Show_String("fill",0);

	  	j=0;
      do{ 
        Text_Foreground_Color(j*10,j*2,j*6);
        Circle_Square_Coordinate_setting(120+j*18,639-j*18,0+j*15,399-j*15,10+j*5,10+j*5);
        Draw_Circle_Square_fill();
        Chk_Busy();

	    j++;
		detect_touch();
	    confirm_button_touch();
	    Delay10ms(20);

	  }while(j<10 & !button_Exit_flag);
	 }
   }

//--------------------------------------- 

//-------------Draw_Ellipse-------------------
   if(!button_Exit_flag)
   {
    Active_Window(0,119,0,72);	
    Background_color(color_black); //
	Memory_Clear();
	Text_Foreground_Color(0xff,0xff,0xff);
    Font_Coordinate(0,0);
    Show_String("Ellipse",0);

    Active_Window(120,639,0,399);	
    Background_color(color_grayscale10); //
	Memory_Clear();

	j=0;
    do{ 
       Text_Foreground_Color(j*2,j*7,j);
       Ellipse_Coordinate_setting(380,200,250-j*24,190-j*18);
       Draw_Ellipse();
       Chk_Busy(); 

	   j++;
		detect_touch();
	    confirm_button_touch();
	    Delay10ms(20);

	}while(j<10 & !button_Exit_flag);

	 if(!button_Exit_flag)
     {
		Text_Foreground_Color(0xff,0xff,0xff);
        Font_Coordinate(0,24);
        Show_String("fill",0);

	  	j=0;
      do{ 
         Text_Foreground_Color(j+5,j*2,j*6);
         Ellipse_Coordinate_setting(380,200,250-j*24,190-j*18);
         Draw_Ellipse_fill();
         Chk_Busy(); 

	     j++;
		 detect_touch();
	     confirm_button_touch();
	     Delay10ms(20);

	  }while(j<10 & !button_Exit_flag);
	 }
   }


//--------------------------------------- 
 }
 while(!button_Exit_flag);
	   Active_Window(0,639,0,479);
	   button_Exit(1);
	   Delay100ms(2);
	   button_Exit(0);
	   button_Exit_flag = 0;
	   button_Draw_flag = 0;
   
  Clear_all_flag();	 
  Active_Window(0,799,0,479);
  button_Draw(0);
  Show_first_page();

}																           


void Show_button_Touch_function(void)
{

  button_Touch(1);

  Active_Window(0,639,0,479);	
  //Text_Background_Color(20,20*2,20);
  Background_color(color_grayscale20);
  Memory_Clear();
  
  Text_color(color_blue);
  Geometric_Coordinate(0,639,0,399);
  Draw_square();
  Chk_Busy();

  Font_with_BackgroundColor();
  Text_Foreground_Color(0,0,0);
  //Text_Background_Color(20,20*2,20);
  Background_color(color_grayscale20);
  Font_Coordinate(250,400);   //�]�w��ܦ�m	
  Show_String("Demo multi touch",0);

  button_Clear(0);
  button_Exit(0);

  Text_color(color_red);
  Background_color(color_blue);

	Active_Window(1,638,1,398);

    do
    {
	  // EA  = 0;	


 switch(nr_of_touches)
   {
   case 1 :
    
	    if(LCD_X1>1&&LCD_X1<638)
   	     {
          if(LCD_Y1>1&&LCD_Y1<398) 
		  {  
           
		  Text_color(color_red);       	         
          Circle_Coordinate_Radius(LCD_X1,LCD_Y1,8);	  	
          Draw_circle_fill();
          Chk_Busy(); 

		   }
	     }

		break;

   case 2:

	    if(LCD_X1>1&&LCD_X1<638)
   	     {
          if(LCD_Y1>1&&LCD_Y1<398) 
		  {  
           
		  Text_color(color_red);       	         
          Circle_Coordinate_Radius(LCD_X1,LCD_Y1,8);	  	
          Draw_circle_fill();
          Chk_Busy(); 

		   }
	     }

	    if(LCD_X2>1&&LCD_X2<638)
   	     {
          if(LCD_Y2>1&&LCD_Y2<398) 
		  {  
		  Text_color(color_yellow);	       	         
          Circle_Coordinate_Radius(LCD_X2,LCD_Y2,8);	  	
          Draw_circle_fill();
          Chk_Busy(); 
		   }
	     }
	
	   break;

    case 3:

		if(LCD_X1>1&&LCD_X1<638)
   	     {
          if(LCD_Y1>1&&LCD_Y1<398) 
		  {  
           
		  Text_color(color_red);       	         
          Circle_Coordinate_Radius(LCD_X1,LCD_Y1,8);	  	
          Draw_circle_fill();
          Chk_Busy(); 

		   }
	     }

	    if(LCD_X2>1&&LCD_X2<638)
   	     {
          if(LCD_Y2>1&&LCD_Y2<398) 
		  {  
		  Text_color(color_yellow);	       	         
          Circle_Coordinate_Radius(LCD_X2,LCD_Y2,8);	  	
          Draw_circle_fill();
          Chk_Busy(); 
		   }
	     }

	    if(LCD_X3>1&&LCD_X3<638)
   	     {
          if(LCD_Y3>1&&LCD_Y3<398) 
		  { 
		
		  Text_color(color_green);	       	         
          Circle_Coordinate_Radius(LCD_X3,LCD_Y3,8);	  	
          Draw_circle_fill();
          Chk_Busy();

		   }
	     }

	      break;

    case 4:

		if(LCD_X1>1&&LCD_X1<638)
   	     {
          if(LCD_Y1>1&&LCD_Y1<398) 
		  {  
           
		  Text_color(color_red);       	         
          Circle_Coordinate_Radius(LCD_X1,LCD_Y1,8);	  	
          Draw_circle_fill();
          Chk_Busy(); 

		   }
	     }

	    if(LCD_X2>1&&LCD_X2<638)
   	     {
          if(LCD_Y2>1&&LCD_Y2<398) 
		  {  
		  Text_color(color_yellow);	       	         
          Circle_Coordinate_Radius(LCD_X2,LCD_Y2,8);	  	
          Draw_circle_fill();
          Chk_Busy(); 
		   }
	     }

	    if(LCD_X3>1&&LCD_X3<638)
   	     {
          if(LCD_Y3>1&&LCD_Y3<398) 
		  { 
		
		  Text_color(color_green);	       	         
          Circle_Coordinate_Radius(LCD_X3,LCD_Y3,8);	  	
          Draw_circle_fill();
          Chk_Busy();

		   }
	     }

		if(LCD_X4>1&&LCD_X4<638)
   	     {
          if(LCD_Y4>1&&LCD_Y4<398) 
		  {  
	
		  Text_color(color_cyan);	       	         
          Circle_Coordinate_Radius(LCD_X4,LCD_Y4,8);	  	
          Draw_circle_fill();
          Chk_Busy(); 
	
		   }
	     }  

	     break;

   case 5:

		if(LCD_X1>1&&LCD_X1<638)
   	     {
          if(LCD_Y1>1&&LCD_Y1<398) 
		  {  
           
		  Text_color(color_red);       	         
          Circle_Coordinate_Radius(LCD_X1,LCD_Y1,8);	  	
          Draw_circle_fill();
          Chk_Busy(); 

		   }
	     }

	    if(LCD_X2>1&&LCD_X2<638)
   	     {
          if(LCD_Y2>1&&LCD_Y2<398) 
		  {  
		  Text_color(color_yellow);	       	         
          Circle_Coordinate_Radius(LCD_X2,LCD_Y2,8);	  	
          Draw_circle_fill();
          Chk_Busy(); 
		   }
	     }

	    if(LCD_X3>1&&LCD_X3<638)
   	     {
          if(LCD_Y3>1&&LCD_Y3<398) 
		  { 
		
		  Text_color(color_green);	       	         
          Circle_Coordinate_Radius(LCD_X3,LCD_Y3,8);	  	
          Draw_circle_fill();
          Chk_Busy();

		   }
	     }

		if(LCD_X4>1&&LCD_X4<638)
   	     {
          if(LCD_Y4>1&&LCD_Y4<398) 
		  {  
	
		  Text_color(color_cyan);	       	         
          Circle_Coordinate_Radius(LCD_X4,LCD_Y4,8);	  	
          Draw_circle_fill();
          Chk_Busy(); 
	
		   }
	     }  

		if(LCD_X5>1&&LCD_X5<638)
   	     {
          if(LCD_Y5>1&&LCD_Y5<398) 
		  {  
	
		  Text_color(color_purple);	       	         
          Circle_Coordinate_Radius(LCD_X5,LCD_Y5,8);	  	
          Draw_circle_fill();
          Chk_Busy();
	
		   }
	     }  

		  break;

	default:
	      break;
 }
	  
//	Clear_ALL_XY_Value();
//	EA=1;

	  while(button_Clear_flag)
	  {
	   Active_Window(0,639,0,479);
	   Font_with_BackgroundTransparency();
	   button_Clear(1);
	   Delay100ms(2);
	   button_Clear(0);
	   Font_with_BackgroundColor();

	   button_Clear_flag = 0;

  	   Text_color(color_grayscale20);
       Geometric_Coordinate(0,639,0,399);
       Draw_square_fill();
       Chk_Busy();
       Text_color(color_blue);
       Geometric_Coordinate(0,639,0,399);
       Draw_square();
       Chk_Busy();
	   Clear_ALL_XY_Value();
	   Active_Window(1,638,1,398);
	  }
	  while(button_Exit_flag)
	  {
	   Active_Window(0,639,0,479);
	   Font_with_BackgroundTransparency();
	   button_Exit(1);
	   Delay100ms(2);
	   button_Exit(0);
	   button_Exit_flag = 0;
	   button_Touch_flag = 0;
	   Clear_ALL_XY_Value();
	   }
    }			
   while(button_Touch_flag);
   Clear_all_flag();
   Active_Window(0,799,0,479);
   button_Touch(0);
   Show_first_page();
 }

void Show_button_BTE_function(void)
{
  uint i,j;
  uchar h,k;

   button_BTE(1);
   Active_Window(0,639,0,479);	
   Text_Background_Color(20,20*2,20);
   Memory_Clear();
   button_Exit(0);

  //write pattern 16*16 to internal pattern ram
   Pattern_Set_16x16();	   
   Write_To_Pattern();        
   Pattern_Set_number(0);
   Graphic_Mode();
   LCD_CmdWrite(0x02);                
   for(i=0;i<512;i++)//
        {
         LCD_DataWrite(gImage_p16x16[i]);
         Chk_Busy();
        } 

   Pattern_Set_number(1);
   Graphic_Mode();
   LCD_CmdWrite(0x02);                
   for(i=0;i<512;i++)//
        {
         LCD_DataWrite(gImage_p16x16_2[i]);
         Chk_Busy();
        } 
   Pattern_Set_number(2);
   Graphic_Mode();
   LCD_CmdWrite(0x02);                
   for(i=0;i<512;i++)//
        {
         LCD_DataWrite(gImage_p16x16_3[i]);
         Chk_Busy();
        } 
 
     Write_To_Bank1and2();
	 Write_To_Bank1();



    
	Text_Foreground_Color(0,0,0);
    Text_Background_Color(20,20*2,20);
    Font_Coordinate(10,10);   //�]�w��ܦ�m	
    Show_String("solid fill",0);

	BTE_Size_setting(12,20);  										 
    BTE_ROP_Code(0xcc);
	for(i=0;i<32;i++)
	{
	Text_Foreground_Color(i,0,0);
	BTE_Source_Destination(0,160+i*12,0,10); 			 				  
    BTE_enable();
	Chk_Busy_BTE();
	Text_Foreground_Color(0,i*2,0);
	BTE_Source_Destination(0,160+i*12,0,30); 			 				  
    BTE_enable();
	Chk_Busy_BTE();
	Text_Foreground_Color(0,0,i);
	BTE_Source_Destination(0,160+i*12,0,50); 			 				  
    BTE_enable();
	Chk_Busy_BTE();
	Text_Foreground_Color(i,i*2,i);
	BTE_Source_Destination(0,160+i*12,0,70); 			 				  
    BTE_enable();
	Chk_Busy_BTE();
	}

	
	Text_Foreground_Color(0,0,0);
    Text_Background_Color(20,20*2,20);
    Font_Coordinate(10,120);   //�]�w��ܦ�m	
    Show_String("pattern fill",0);

     Text_Foreground_Color(0,0,0);
     Text_Background_Color(20,20*2,20);
     Font_Coordinate(10,200);   //�]�w��ܦ�m	
     Show_String("write with ROP",0);

     Font_Coordinate(350,200);   //�]�w��ܦ�m	
     Show_String("color expansion",0);	
	 Graphic_Mode();


    i=0;
	k=0;
   do{
	 
	 Pattern_Set_number(k);
     BTE_Source_Destination(0,160,0,110); 
     Source_Layer1();
     Destination_Layer1();			 				
     BTE_Size_setting(400,64);  										 
     BTE_ROP_Code(0xc6);
     BTE_enable();
	 Chk_Busy_BTE();
	 k++;

	 if(k>2)
	 k=0;


	  

	  Active_Window(10,89,230,309);
	  XY_Coordinate(10,230);
      show_pic();

	  Active_Window(150,229,230,309);
	  XY_Coordinate(150,230);
      show_pic();

	  
	  Active_Window(0,300,320,350);

	  Text_Foreground_Color(0,0,0);
      Text_Background_Color(20,20*2,20);
      Memory_Clear();


	  Active_Window(0,639,0,479);
      Font_Coordinate(10,320);   //�]�w��ܦ�m	
	 switch(i)
	   {
	    case 0 : 
               Show_String("ROP=0      black",0);	
			   break;
		case 1 :
		       Show_String("ROP=1      ~(S+D)",0);
			    break;
		case 2 :
		       Show_String("ROP=2      ~S & D",0);
			    break;
		case 3 :
		       Show_String("ROP=3      ~S",0);
			    break;
		case 4 :
		       Show_String("ROP=4      S & ~D",0);
			    break;
		case 5 :
		       Show_String("ROP=5      ~D",0);
			    break;
		case 6 :
		       Show_String("ROP=6      S XOR D",0);
			    break;
		case 7 :
		       Show_String("ROP=7      S & D",0);
			    break;
		case 8 :
		       Show_String("ROP=8      S & D",0);
			   Show_String("",0);
			    break;
		case 9 :
		       Show_String("ROP=9      ~(S XOR D)",0);

			    break;
		case 10 :
		       Show_String("ROP=10      D",0);
			    break;					
		case 11 :
		       Show_String("ROP=11      ~S+D",0);
			    break;				
		case 12 :
		       Show_String("ROP=12      S",0);
			    break;
		case 13 :
		       Show_String("ROP=13      S+~D",0);
			    break;	
		case 14 :
		       Show_String("ROP=14      S+D",0);
			    break;
		case 15 :
		       Show_String("ROP=15      white",0);
			    break;	
				
		default : break;

		}
																		  
	  //write BTE with ROP
	  BTE_Source_Destination(0,150,0,230);
      BTE_Size_setting(80,80);
	  BTE_write_data_with_ROP(i*16);

 
	  //write BTE color expansion
	  BTE_Background_color(i*2,i*3,i*4);
	  BTE_Foreground_color(i*6,i*12,i*8);
	  BTE_Source_Destination(0,400,0,240);
      BTE_Size_setting(120,100);
	  BTE_ROP_Code(0x79);		 //	color expansion 
	  BTE_enable();
	  LCD_CmdWrite(0x02);//MRWC
      for(j=0;j<1500;j++)
      {
       LCD_DataWrite(gImage_rabbit[j]);	 
	   Chk_Busy();	 
	  }

	   h=0;
	  do{
		 detect_touch();
	     confirm_button_touch();
		 Delay100ms(1);
		 h++;
	   }while(h<30 & !button_Exit_flag);
	    i++;
	   if(i>15)
	   i=0;	 	   
	}
	while(!button_Exit_flag);
	   
	   Active_Window(0,799,0,479);
	   Font_with_BackgroundTransparency();	
	   button_Exit(1);
	   Delay100ms(2);
	   button_Exit(0);
	   button_Exit_flag = 0;
	   button_BTE_flag = 0;
  Clear_all_flag();
  button_BTE(0);
  Show_first_page();
}

void Show_button_DMA_function(void)
{
  uchar i,
  uchar h;


  button_DMA(1);

  Active_Window(0,639,0,479);	
  Text_Background_Color(0xff,0xff,0xff);
  Memory_Clear();
  button_Exit(0);


   SROM_CLK_DIV(0);    
   Serial_ROM_select1(); 
   Select_Serial_Waveform_mode3();  
   SERIAL_ROM_Read_Cycle_5bus();
   SERIAL_ROM_DMA_mode();
   SERIAL_ROM_Signal_mode();
   SERIAL_ROM_Dual_mode0();
   DMA_Block_mode();

   i=0;
  do{
     
     DMA_show_pic(i);

	  h=0;
	  do{
	  detect_touch();
	  confirm_button_touch();
	  Delay100ms(1);
	  h++;
	  }while(h<30&button_Exit_flag==0);

	  Active_Window(0,639,0,399); 
      Text_Background_Color(0xff,0xff,0xff);
      Memory_Clear();

	  i++;

	  if(i>4)
	  i=0;

   }
  while(button_Exit_flag==0);

  SERIAL_ROM_Signal_mode();
  Serial_ROM_select0();
  SERIAL_ROM_Font_mode();
  SROM_CLK_DIV(2);


  Active_Window(0,799,0,479);
  Font_with_BackgroundTransparency();
  button_Exit(1);
  Delay100ms(2);
  button_Exit(0);
  button_Exit_flag = 0;
  button_DMA_flag = 0;
  Clear_all_flag();
  button_DMA(0);
  Show_first_page();

}



void Show_button_PWM_function(void)
{
  uchar light_bar_temp ;

  button_PWM(1);

  Active_Window(0,639,0,479);

  RA_Box_create(0,0,320,200,"PWM ADJUST");

  button_Exit2(0);

  light_bar_temp = backlight;

   Text_Foreground_Color(0,0,0);
   Draw_Triangle_3point_Coordinate(15,90,54,65,54,114);
   Draw_Triangle_fill();
   Chk_Busy();

   Text_Foreground_Color(10,20,10);
   Draw_Triangle_3point_Coordinate(15+1,90,54-1,65+1,54-1,114-1);
   Draw_Triangle_fill();
   Chk_Busy();

   Text_Foreground_Color(0,0,0);
   Draw_Triangle_3point_Coordinate(264,65,304,90,264,114);
   Draw_Triangle_fill();
   Chk_Busy();

   Text_Foreground_Color(10,20,10);
   Draw_Triangle_3point_Coordinate(264+1,65+1,304-1,90,264+1,114-1);
   Draw_Triangle_fill();
   Chk_Busy();

    switch(light_bar_temp)
   {
    case 1 : 
          show_pwm_lightbar(1);
	
		  break;
    case 2 : 
          show_pwm_lightbar(2);
	
		  break;
    case 3 : 
          show_pwm_lightbar(3);
	
		  break;
    case 4 : 
          show_pwm_lightbar(4);
		
		  break;
    case 5 : 
          show_pwm_lightbar(5);
	
		  break;

      default :  break;
     }


do{


 while(minus_button_flag)
 {

   if (light_bar_temp >1)
   light_bar_temp--;

   Text_Foreground_Color(31,0,0);
   Draw_Triangle_3point_Coordinate(15+1,90,54-1,65+1,54-1,114-1);
   Draw_Triangle_fill();
   Chk_Busy();

    switch(light_bar_temp)
   {
    case 1 : 
          show_pwm_lightbar(1);
	
		  break;
    case 2 : 
          show_pwm_lightbar(2);
		
		  break;
    case 3 : 
          show_pwm_lightbar(3);
	
		  break;
    case 4 : 
          show_pwm_lightbar(4);
	
		  break;
    case 5 : 
          show_pwm_lightbar(5);
		 
		  break;

      default :  break;
     }

   Delay100ms(2);
   Text_Foreground_Color(10,20,10);
   Draw_Triangle_fill();
   Chk_Busy();
   minus_button_flag = 0;

   
 }

 while(plus_button_flag)
  {
    if (light_bar_temp <5)
   light_bar_temp++;

   Text_Foreground_Color(31,0,0);
   Draw_Triangle_3point_Coordinate(264+1,65+1,304-1,90,264+1,114-1);
   Draw_Triangle_fill();
   Chk_Busy();

    switch(light_bar_temp)
   {
    case 1 : 
          show_pwm_lightbar(1);
	
		  break;
    case 2 : 
          show_pwm_lightbar(2);
		
		  break;
    case 3 : 
          show_pwm_lightbar(3);
	
		  break;
    case 4 : 
          show_pwm_lightbar(4);

		  break;
    case 5 : 
          show_pwm_lightbar(5);
	
		  break;

      default :  break;
     }

   Delay100ms(2);
   Text_Foreground_Color(10,20,10);
   Draw_Triangle_fill();
   Chk_Busy();

   plus_button_flag = 0;
  }

	 detect_touch();
	 confirm_button_touch();
}
while(!button_Exit2_flag);

  button_Exit2(1);
  Delay100ms(2);
  button_Exit2(0);
  Active_Window(0,799,0,479);
  button_Exit2_flag = 0;
  backlight = light_bar_temp;
  button_PWM_flag = 0;
  button_PWM(0);
  Show_first_page();

}

/*

void Show_button_Cursor_function(void)
{
  uchar light_bar_temp ;

  button_Cursor(1);
  Delay100ms(5);
  button_Cursor(0);

  counter1++;

  if(counter1==1)
   {	 
   Graphic_Cursor();
   Graphic_Cursor_on_flag = 1;
   }
  else if (counter1==2)
   {
   No_Graphic_Cursor();
   Graphic_Cursor_on_flag = 0;
   counter1 = 0;
   }
}
*/

void BTE_write_data_with_ROP(uchar setx)
{ uint i;


     BTE_ROP_Code(setx);
     BTE_enable();
     LCD_CmdWrite(0x02);//MRWC
	 for(i=0;i<6400;i++)
	 {
	  LCD_DataWrite(gImage_pic_80x80[i*2]);
	  LCD_DataWrite(gImage_pic_80x80[i*2+1]);
	  Chk_Busy();
	 }
}

void show_pic(void)
{ uint i;

     Graphic_Mode();
     LCD_CmdWrite(0x02);//MRWC
	 for(i=0;i<6400;i++)
	 {
	  LCD_DataWrite(gImage_pic_80x80[i*2]);
	  LCD_DataWrite(gImage_pic_80x80[i*2+1]);
	  Chk_Busy();
	 }
}



void DMA_show_pic(uchar setx)
{

 switch(setx)
 { 
    case 0 :
	
		   XY_Coordinate(0,0);
		   DMA_Start_address_setting(0);
           DMA_block_mode_size_setting(640,400,640);
           DMA_Start_enable();
	       Chk_DMA_Busy();
		   break;

    case 1 :
	
		   XY_Coordinate(0,0);
		   DMA_Start_address_setting(512000);
           DMA_block_mode_size_setting(640,400,640);
           DMA_Start_enable();
	       Chk_DMA_Busy();
		   break;

	 case 2 :
		 
		   XY_Coordinate(0,0);
		   DMA_Start_address_setting(1024000);
           DMA_block_mode_size_setting(640,400,640);
           DMA_Start_enable();
	       Chk_DMA_Busy();
		   break;

	  case 3 :

		   XY_Coordinate(0,0);
		   DMA_Start_address_setting(1536000);
           DMA_block_mode_size_setting(640,400,640);
           DMA_Start_enable();
	       Chk_DMA_Busy();
		   break;

	   case 4 :	   
		   XY_Coordinate(0,0);
		   DMA_Start_address_setting(2048000);
           DMA_block_mode_size_setting(640,400,640);
           DMA_Start_enable();
	       Chk_DMA_Busy();
		   break;

	    case 5 :   
		   XY_Coordinate(0,0);
		   DMA_Start_address_setting(3532800);
           DMA_block_mode_size_setting(640,400,640);
           DMA_Start_enable();
	       Chk_DMA_Busy();
		   break;

	default:
			 break;

  }
}



void show_pwm_lightbar(uchar setx)
{

 switch(setx)
 {
   case 1 : 
            Text_Foreground_Color(31,31,0);
            Circle_Coordinate_Radius(80,90,15);
            Draw_circle_fill();
            Chk_Busy();

            Text_Foreground_Color(16,16,0);
            Circle_Coordinate_Radius(120,90,15);
            Draw_circle_fill();
            Chk_Busy();

            Circle_Coordinate_Radius(160,90,15);
            Draw_circle_fill();
            Chk_Busy();

            Circle_Coordinate_Radius(200,90,15);
            Draw_circle_fill();
            Chk_Busy();

            Circle_Coordinate_Radius(240,90,15);
            Draw_circle_fill();
            Chk_Busy(); 

			PWM1_duty_cycle(0x80);
            break;

   case 2 :

            Text_Foreground_Color(31,31,0);
            Circle_Coordinate_Radius(80,90,15);
            Draw_circle_fill();
            Chk_Busy();

            
            Circle_Coordinate_Radius(120,90,15);
            Draw_circle_fill();
            Chk_Busy();
			Text_Foreground_Color(16,16,0);
            Circle_Coordinate_Radius(160,90,15);
            Draw_circle_fill();
            Chk_Busy();

            Circle_Coordinate_Radius(200,90,15);
            Draw_circle_fill();
            Chk_Busy();

            Circle_Coordinate_Radius(240,90,15);
            Draw_circle_fill();
            Chk_Busy();
		    
			PWM1_duty_cycle(0xa0);
            break;

    case 3 :

            Text_Foreground_Color(31,31,0);
            Circle_Coordinate_Radius(80,90,15);
            Draw_circle_fill();
            Chk_Busy();

            
            Circle_Coordinate_Radius(120,90,15);
            Draw_circle_fill();
            Chk_Busy();
			
            Circle_Coordinate_Radius(160,90,15);
            Draw_circle_fill();
            Chk_Busy();

			Text_Foreground_Color(16,16,0);
            Circle_Coordinate_Radius(200,90,15);
            Draw_circle_fill();
            Chk_Busy();

            Circle_Coordinate_Radius(240,90,15);
            Draw_circle_fill();
            Chk_Busy();

			PWM1_duty_cycle(0xc0);
            break;
   case 4 :

            Text_Foreground_Color(31,31,0);
            Circle_Coordinate_Radius(80,90,15);
            Draw_circle_fill();
            Chk_Busy();

            
            Circle_Coordinate_Radius(120,90,15);
            Draw_circle_fill();
            Chk_Busy();
			
            Circle_Coordinate_Radius(160,90,15);
            Draw_circle_fill();
            Chk_Busy();
			
            Circle_Coordinate_Radius(200,90,15);
            Draw_circle_fill();
            Chk_Busy();
			Text_Foreground_Color(16,16,0);
            Circle_Coordinate_Radius(240,90,15);
            Draw_circle_fill();
            Chk_Busy();

			PWM1_duty_cycle(0xe0);
            break;
   case 5 :

            Text_Foreground_Color(31,31,0);
            Circle_Coordinate_Radius(80,90,15);
            Draw_circle_fill();
            Chk_Busy();
   
            Circle_Coordinate_Radius(120,90,15);
            Draw_circle_fill();
            Chk_Busy();
			
            Circle_Coordinate_Radius(160,90,15);
            Draw_circle_fill();
            Chk_Busy();
			
            Circle_Coordinate_Radius(200,90,15);
            Draw_circle_fill();
            Chk_Busy();

            Circle_Coordinate_Radius(240,90,15);
            Draw_circle_fill();
            Chk_Busy();
			 PWM1_duty_cycle(0xff);
            break;

		default:   break;
	}
}




