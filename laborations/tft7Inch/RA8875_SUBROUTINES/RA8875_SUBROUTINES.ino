

void RA8875_PLL_ini(void)
{
    LCD_CmdWrite(0x88);       
    LCD_DataWrite(0x0c);
    Delay1ms(1);    
    LCD_CmdWrite(0x89);       
    LCD_DataWrite(0x02);  
    Delay1ms(1);
}	

//------------------------------------------//
void LCD_Initial(void)
{ 	

    RA8875_PLL_ini();
    LCD_CmdWrite(0x10);  //SYSR   bit[4:3]=00 256 color  bit[2:1]=  00 8bit MPU interface
    LCD_DataWrite(0x0c);   // if 8bit MCU interface   and 65k color display  						    
    //LCD_DataWrite(0x0F); // if 16bit MCU interface   and 65k color display  


//AT070TN92  setting
//==============	Display Window	800x480 ==================	
  /*
   LCD_CmdWrite(0x04);     //PCLK inverse
   LCD_DataWrite(0x81);
   Delay1ms(1);
   
   //Horizontal set
   LCD_CmdWrite(0x14); //HDWR//Horizontal Display Width Setting Bit[6:0]                   
   LCD_DataWrite(0x63);//Horizontal display width(pixels) = (HDWR + 1)*8                   
   LCD_CmdWrite(0x15); //Horizontal Non-Display Period Fine Tuning Option Register (HNDFTR)
   LCD_DataWrite(0x03);//Horizontal Non-Display Period Fine Tuning(HNDFT) [3:0]            
   LCD_CmdWrite(0x16); //HNDR//Horizontal Non-Display Period Bit[4:0]                      
   LCD_DataWrite(0x03);//Horizontal Non-Display Period (pixels) = (HNDR + 1)*8             
   LCD_CmdWrite(0x17); //HSTR//HSYNC Start Position[4:0]                                   
   LCD_DataWrite(0x02);//HSYNC Start Position(PCLK) = (HSTR + 1)*8                         
   LCD_CmdWrite(0x18); //HPWR//HSYNC Polarity ,The period width of HSYNC.                  
   LCD_DataWrite(0x00);//HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8     
   //Vertical set    
   LCD_CmdWrite(0x19); //VDHR0 //Vertical Display Height Bit [7:0]                         
   LCD_DataWrite(0xdf);//Vertical pixels = VDHR + 1                                        
   LCD_CmdWrite(0x1a); //VDHR1 //Vertical Display Height Bit [8]                           
   LCD_DataWrite(0x01);//Vertical pixels = VDHR + 1                                        
   LCD_CmdWrite(0x1b); //VNDR0 //Vertical Non-Display Period Bit [7:0]                     
   LCD_DataWrite(0x14);//Vertical Non-Display area = (VNDR + 1)                            
   LCD_CmdWrite(0x1c); //VNDR1 //Vertical Non-Display Period Bit [8]                       
   LCD_DataWrite(0x00);//Vertical Non-Display area = (VNDR + 1)                            
   LCD_CmdWrite(0x1d); //VSTR0 //VSYNC Start Position[7:0]                                 
   LCD_DataWrite(0x06);//VSYNC Start Position(PCLK) = (VSTR + 1)                           
   LCD_CmdWrite(0x1e); //VSTR1 //VSYNC Start Position[8]                                   
   LCD_DataWrite(0x00);//VSYNC Start Position(PCLK) = (VSTR + 1)                           
   LCD_CmdWrite(0x1f); //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]                     
   LCD_DataWrite(0x01);//VSYNC Pulse Width(PCLK) = (VPWR + 1)                              

 //LCD_CmdWrite(0xf2);  
 //LCD_DataWrite(0x01);
   */
//HSD050IDW1 setting
//==============	Display Window	800x480 ==================	
 LCD_CmdWrite(0x04);  //PCLK inverse
 LCD_DataWrite(0x81);
 Delay1ms(1);
 
 //Horizontal set
 LCD_CmdWrite(0x14); //HDWR//Horizontal Display Width Setting Bit[6:0]                      
 LCD_DataWrite(0x63);//Horizontal display width(pixels) = (HDWR + 1)*8                      
 LCD_CmdWrite(0x15);//Horizontal Non-Display Period Fine Tuning Option Register (HNDFTR)   
 LCD_DataWrite(0x00);//Horizontal Non-Display Period Fine Tuning(HNDFT) [3:0]               
 LCD_CmdWrite(0x16); //HNDR//Horizontal Non-Display Period Bit[4:0]                         
 LCD_DataWrite(0x03);//Horizontal Non-Display Period (pixels) = (HNDR + 1)*8                
 LCD_CmdWrite(0x17); //HSTR//HSYNC Start Position[4:0]                                      
 LCD_DataWrite(0x03);//HSYNC Start Position(PCLK) = (HSTR + 1)*8                            
 LCD_CmdWrite(0x18); //HPWR//HSYNC Polarity ,The period width of HSYNC.                     
 LCD_DataWrite(0x0B);//HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8    
 //Vertical set                             
 LCD_CmdWrite(0x19); //VDHR0 //Vertical Display Height Bit [7:0]                            
 LCD_DataWrite(0xdf);//Vertical pixels = VDHR + 1                                           
 LCD_CmdWrite(0x1a); //VDHR1 //Vertical Display Height Bit [8]                              
 LCD_DataWrite(0x01);//Vertical pixels = VDHR + 1                                           
 LCD_CmdWrite(0x1b); //VNDR0 //Vertical Non-Display Period Bit [7:0]                        
 LCD_DataWrite(0x20);//Vertical Non-Display area = (VNDR + 1)                               
 LCD_CmdWrite(0x1c); //VNDR1 //Vertical Non-Display Period Bit [8]                          
 LCD_DataWrite(0x00);//Vertical Non-Display area = (VNDR + 1)                               
 LCD_CmdWrite(0x1d); //VSTR0 //VSYNC Start Position[7:0]                                    
 LCD_DataWrite(0x16);//VSYNC Start Position(PCLK) = (VSTR + 1)                              
 LCD_CmdWrite(0x1e); //VSTR1 //VSYNC Start Position[8]                                      
 LCD_DataWrite(0x00);//VSYNC Start Position(PCLK) = (VSTR + 1)                              
 LCD_CmdWrite(0x1f); //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]                        
 LCD_DataWrite(0x01);//VSYNC Pulse Width(PCLK) = (VPWR + 1)                                 
  
  //Active window  set 
   //setting active window X
    LCD_CmdWrite(0x30); //Horizontal Start Point 0 of Active Window (HSAW0)
    LCD_DataWrite(0x00); //Horizontal Start Point of Active Window [7:0]
    LCD_CmdWrite(0x31); //Horizontal Start Point 1 of Active Window (HSAW1)	   
    LCD_DataWrite(0x00); //Horizontal Start Point of Active Window [9:8]
    LCD_CmdWrite(0x34); //Horizontal End Point 0 of Active Window (HEAW0)
    LCD_DataWrite(0x1F); //Horizontal End Point of Active Window [7:0]
    LCD_CmdWrite(0x35); //Horizontal End Point 1 of Active Window (HEAW1)	   
    LCD_DataWrite(0x03); //Horizontal End Point of Active Window [9:8]

   //setting active window Y
    LCD_CmdWrite(0x32); //Vertical Start Point 0 of Active Window (VSAW0)
    LCD_DataWrite(0x00); //Vertical Start Point of Active Window [7:0]
    LCD_CmdWrite(0x33); //Vertical Start Point 1 of Active Window (VSAW1)	   
    LCD_DataWrite(0x00); //Vertical Start Point of Active Window [8]
    LCD_CmdWrite(0x36); //Vertical End Point of Active Window 0 (VEAW0)
    LCD_DataWrite(0xdf); //Vertical End Point of Active Window [7:0]
    LCD_CmdWrite(0x37); //Vertical End Point of Active Window 1 (VEAW1)	   
    LCD_DataWrite(0x01); //Vertical End Point of Active Window [8]


}

//----------------------------------------------------------//


				
//------------------------//
//STATUS 
void Chk_Busy(void)
{
	uchar temp; 	
	do
	{
	temp=LCD_StatusRead();
	}while((temp&0x80)==0x80);
	
		   
}
void Chk_Busy_BTE(void)
{
	uchar temp;
   	
	do
	{
     temp=LCD_StatusRead();
	}while((temp&0x40)==0x40);	   
}

void Chk_Busy_DIR_Access(void)
{
	uchar  temp;
   	
	do
	{
     temp=LCD_StatusRead();
	}while((temp&0x01)==0x01);	   
}

//REG[BFh]
void Chk_DMA_Busy(void)
{
	uchar temp; 	
	do
	{
	LCD_CmdWrite(0xBF);
	temp = LCD_DataRead();
	}while((temp&0x01)==0x01);   
}
//REG[01h]		 

void Display_ON(void)
{	
    LCD_CmdWrite(0x01);//PWRR
	LCD_DataWrite(0x80);
}

void Display_OFF(void)
{
	LCD_CmdWrite(0x01);//PWRR
	LCD_DataWrite(0x00);
}

void Normal_Mode_exitSleep(void)
{	
    LCD_CmdWrite(0x01);//PWRR
    LCD_DataWrite(0x00);
}
void Sleep_Mode(void)
{
	LCD_CmdWrite(0x01);//PWRR
	LCD_DataWrite(0x02);
}
void Software_Reset(void)
{	
	LCD_CmdWrite(0x01);//PWRR
    LCD_DataWrite(0x01);
    LCD_DataWrite(0x00);
    Delay1ms(1);// 
}							   
//-------------------------------//
//REG[04h] for test 
void PCLK_inversion(void)
{ 	uchar temp;
  	LCD_CmdWrite(0x04);//PWRR
	temp = LCD_DataRead();
	temp |= cSetD7;
	LCD_DataWrite(temp);
}
void PCLK_non_inversion(void)
{ 	uchar temp;
  	LCD_CmdWrite(0x04);//PWRR
	temp = LCD_DataRead();
	temp &= cClrD7;
	LCD_DataWrite(temp);
}

void PCLK_width(uchar setx) //bit[1:0]
{ 	uchar temp;
  	LCD_CmdWrite(0x04);//PWRR
	temp = LCD_DataRead();
	temp &=0x80;
    temp |= setx;
	LCD_DataWrite(temp);
}

//-------------------------------//
//REG[05h]
void Serial_ROM_select0(void)
{   uchar temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=cClrD7;
	LCD_DataWrite(temp);
}

void Serial_ROM_select1(void)
{   uchar temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp |=cSetD7;
	LCD_DataWrite(temp);
}

void Serial_ROM_Address_set_24bit(void)
{   uchar temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=cClrD6;
	LCD_DataWrite(temp);
}

void Serial_ROM_Address_set_32bit(void)
{   uchar temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp |=cSetD6;
	LCD_DataWrite(temp);
}

void Select_Serial_Waveform_mode0(void)
{	uchar temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=cClrD5;
	LCD_DataWrite(temp);
}

void Select_Serial_Waveform_mode3(void)
{	uchar temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp |=cSetD5;
	LCD_DataWrite(temp);
}

void SERIAL_ROM_Read_Cycle_4bus(void)
{   uchar temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=0xE7;
	LCD_DataWrite(temp);
}

void SERIAL_ROM_Read_Cycle_5bus(void)
{   uchar temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=0xE7;
    temp |=cSetD3;
	LCD_DataWrite(temp);
}

void SERIAL_ROM_Read_Cycle_6bus(void)
{   uchar temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=0xE7;
    temp |=cSetD4;
	LCD_DataWrite(temp);
}

void SERIAL_ROM_Font_mode(void)
{   uchar temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=cClrD2;
	LCD_DataWrite(temp);
}

void SERIAL_ROM_DMA_mode(void)
{   uchar temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp |=cSetD2;
	LCD_DataWrite(temp);
}

void SERIAL_ROM_Signal_mode(void)
{   uchar temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=0xFC;
	LCD_DataWrite(temp);
}

void SERIAL_ROM_Dual_mode0(void)
{   uchar temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=0xFC;
    temp |=cSetD1;
	LCD_DataWrite(temp);
}

void SERIAL_ROM_Dual_mode1(void)
{   uchar temp;
  	LCD_CmdWrite(0x05);//PWRR
	temp = LCD_DataRead();
	temp &=0xFC;
    temp |=cSetD1;
    temp |=cSetD0;
	LCD_DataWrite(temp);
}

void SROM_CLK_DIV(uchar CLK_DIV)
{
	LCD_CmdWrite(0x06);
    LCD_DataWrite(CLK_DIV);
}


//-------------------------------//
//REG[10h]
void Color_256(void)
{
	uchar temp;
	LCD_CmdWrite(0x10);//SYSR
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	LCD_DataWrite(temp);
} 

void Color_65K(void)
{
	uchar temp;
	LCD_CmdWrite(0x10);//SYSR
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	LCD_CmdWrite(0x10);//SYSR
	LCD_DataWrite(temp);
}
void MPU_8bit_Interface(void)
{
	uchar temp;
	LCD_CmdWrite(0x10);//SYSR
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	LCD_DataWrite(temp);
}

void MPU_16bit_Interface(void)
{
	uchar temp;
	LCD_CmdWrite(0x10);//SYSR
	temp = LCD_DataRead();
	temp |= cSetD1ClrD0 ;
	LCD_DataWrite(temp);
} 								 
//--------------------------------//
//REG[12h] 

uchar GPI_data(void)
{   uchar temp;
	LCD_CmdWrite(0x12);//SYSR
	temp = LCD_DataRead();
	return temp;
}

void GPO_data(uchar setx)
{
  LCD_CmdWrite(0x13); 
  LCD_DataWrite(setx);
}

							
//---------------------------------//
//REG[20h]

void One_Layer(void)
{
	uchar temp;
	LCD_CmdWrite(0x20);//DPCR
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	LCD_DataWrite(temp);
}	
				 
void Two_Layers(void)
{	uchar temp;
	LCD_CmdWrite(0x20);//DPCR
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	LCD_DataWrite(temp);
}

void HDIR_SEG0_SEGn(void)
{	uchar temp;
	LCD_CmdWrite(0x20);//DPCR
	temp = LCD_DataRead();
	temp &= cClrD3 ;
	LCD_DataWrite(temp);
}
void HDIR_SEGn_SEG0(void)
{
	uchar temp;

	LCD_CmdWrite(0x20);//DPCR
	temp = LCD_DataRead();
	temp |= cSetD3 ;
	LCD_DataWrite(temp);
}
void VDIR_COM0_COMn(void)
{
	uchar temp;

	LCD_CmdWrite(0x20);//DPCR
	temp = LCD_DataRead();
	temp &= cClrD2 ;
	LCD_DataWrite(temp);
}
void VDIR_COMn_COM0(void)
{
	uchar temp;

	LCD_CmdWrite(0x20);//DPCR
	temp = LCD_DataRead();
	temp |= cSetD2 ;
	LCD_DataWrite(temp);
}


//----------------------------------//
//REG[21h] 
void CGROM_Font(void)
{
	uchar temp;
	LCD_CmdWrite(0x21);//FNCR0
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	LCD_DataWrite(temp);
}
void CGRAM_Font(void)
{
	uchar temp;
	LCD_CmdWrite(0x21);//FNCR0
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	LCD_DataWrite(temp);
}

void Internal_CGROM(void)
{
	uchar temp;
	LCD_CmdWrite(0x21);//FNCR0
	temp = LCD_DataRead();
	temp &= cClrD5 ;
	LCD_DataWrite(temp);
}
void External_CGROM(void)
{
	uchar temp;
	LCD_CmdWrite(0x21);//FNCR0
	temp = LCD_DataRead();
	temp |= cSetD5 ;
	LCD_DataWrite(temp);
}

void ISO8859_1(void)
{
	uchar temp;
	LCD_CmdWrite(0x21);//FNCR0
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	LCD_DataWrite(temp);
}
void ISO8859_2(void)
{
	uchar temp;
	LCD_CmdWrite(0x21);//FNCR0
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	temp |= cClrD1SetD0 ;
	LCD_DataWrite(temp);
}
void ISO8859_3(void)
{
	uchar temp;
	LCD_CmdWrite(0x21);//FNCR0
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	temp |= cSetD1ClrD0 ;
	LCD_DataWrite(temp);
}
void ISO8859_4(void)
{
	uchar temp;
	LCD_CmdWrite(0x21);//FNCR0
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	temp |= cSetD1D0 ;
	LCD_DataWrite(temp);
}								 
//--------------------------------//
//REG[22h]

void No_FullAlignment(void)
{
	uchar temp;
	LCD_CmdWrite(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	LCD_DataWrite(temp);
}
void FullAlignment(void)
{
	uchar temp;
	LCD_CmdWrite(0x22);//FNCR1
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	LCD_DataWrite(temp);
}
void Font_with_BackgroundColor(void)
{
	uchar temp;
	LCD_CmdWrite(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD6 ;
	LCD_DataWrite(temp);
}
void Font_with_BackgroundTransparency(void)
{
	uchar temp;
	LCD_CmdWrite(0x22);//FNCR1
	temp = LCD_DataRead();
	temp |= cSetD6 ;
	LCD_DataWrite(temp);
}


void NoRotate_Font(void)
{
	uchar temp;
	LCD_CmdWrite(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD4 ;
	LCD_DataWrite(temp);
}
void Rotate90_Font(void)
{
	uchar temp;
	LCD_CmdWrite(0x22);//FNCR1
	temp = LCD_DataRead();
	temp |= cSetD4 ;
	LCD_DataWrite(temp);
}


void Horizontal_FontEnlarge_x1(void)
{
	uchar temp;
	LCD_CmdWrite(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	LCD_DataWrite(temp);
}
void Horizontal_FontEnlarge_x2(void)
{
	uchar temp;
	LCD_CmdWrite(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	temp |= cClrD3SetD2 ;
	LCD_DataWrite(temp);
}
void Horizontal_FontEnlarge_x3(void)
{
	uchar temp;
	LCD_CmdWrite(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	LCD_DataWrite(temp);
}
void Horizontal_FontEnlarge_x4(void)
{
	uchar temp;
	LCD_CmdWrite(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3D2 ;
	LCD_DataWrite(temp);
}
void Vertical_FontEnlarge_x1(void)
{
	uchar temp;
	LCD_CmdWrite(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	LCD_DataWrite(temp);
}
void Vertical_FontEnlarge_x2(void)
{
	uchar temp;
	LCD_CmdWrite(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	temp |= cClrD1SetD0 ;
	LCD_DataWrite(temp);
}
void Vertical_FontEnlarge_x3(void)
{
	uchar temp;
	LCD_CmdWrite(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	temp |= cSetD1ClrD0 ;
	LCD_DataWrite(temp);
}
void Vertical_FontEnlarge_x4(void)
{
	uchar temp;
	LCD_CmdWrite(0x22);//FNCR1
	temp = LCD_DataRead();
	temp &= cClrD1D0 ;
	temp |= cSetD1D0 ;
	LCD_DataWrite(temp);
}

//-------------------------------------------//
//REG[23h]									 
void CGRAM_Select_Number(uchar setx)
{ 
    LCD_CmdWrite(0x23);//CGSR
	LCD_DataWrite(setx);
}	
								
//-------------------------------------------//
//REG[24h]REG[25h]REG[26h]REG[27h]
void Scroll_Offset(uint X,uint Y)
{

    LCD_CmdWrite(0x24);//HOFS0
	LCD_DataWrite(X);
  
    LCD_CmdWrite(0x25);//HOFS1	   
	LCD_DataWrite(X>>8);

	  
    LCD_CmdWrite(0x26);//VOFS0
	LCD_DataWrite(Y);  

    LCD_CmdWrite(0x27);//VOFS1	   
	LCD_DataWrite(Y>>8); 
}	 
//-------------------------------------------//
//REG[29h]
void Line_distance(uchar setx)
{ 
    LCD_CmdWrite(0x29);//LDS
	LCD_DataWrite(setx);
}	  
//-------------------------------------------//
//REG[2Ah]REG[2Bh]REG[2Ch]REG[2Dh]
void Font_Coordinate(uint X,uint Y)
{
 
    LCD_CmdWrite(0x2A);//HOFS0
	LCD_DataWrite(X);

    LCD_CmdWrite(0x2B);//HOFS1	   
	LCD_DataWrite(X>>8);

 
    LCD_CmdWrite(0x2C);//VOFS0
	LCD_DataWrite(Y);

    LCD_CmdWrite(0x2D);//VOFS1	   
	LCD_DataWrite(Y>>8); 
}


//-------------------------------------------//
//REG[2Eh]
void Font_size_16x16_8x16(void) 
{
 	uchar temp;
	LCD_CmdWrite(0x2E);//FNCR1
	temp = LCD_DataRead();
	temp &= 0x3f;
	LCD_DataWrite(temp);
}

void Font_size_24x24_12x24(void) 
{
 	uchar temp;
	LCD_CmdWrite(0x2E);//FNCR1
	temp = LCD_DataRead();
	temp &= 0x3f;
	temp |= 0x40; 
	LCD_DataWrite(temp);
}

void Font_size_32x32_16x32(void) 
{
 	uchar temp;
	LCD_CmdWrite(0x2E);//FNCR1
	temp = LCD_DataRead();
	temp &= 0x3f;
	temp |= 0x80; 
	LCD_DataWrite(temp);
}


void Font_spacing_set(uchar setx) //bit[5:0]
{   uchar temp,temp1;
    temp1=setx&0x3F;
   	LCD_CmdWrite(0x2E);//FNCR1
	temp = LCD_DataRead();
	temp &= 0xc0;
	temp |= temp1; 
	LCD_DataWrite(temp);

}

//-------------------------------------------//
//REG[2Fh]
void GT_serial_ROM_select_GT21L16T1W(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0x1f;
  LCD_DataWrite(temp);
}

void GT_serial_ROM_select_GT23L16U2W(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0x1f;
  temp |= 0x20;
  LCD_DataWrite(temp);
}

void GT_serial_ROM_select_GT23L24T3Y(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0x1f;
  temp |= 0x40;
  LCD_DataWrite(temp);
}

void GT_serial_ROM_select_GT23L24M1Z(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0x1f;
  temp |= 0x60;
  LCD_DataWrite(temp);
}

void GT_serial_ROM_select_GT23L32S4W(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0x1f;
  temp |= 0x80;
  LCD_DataWrite(temp);
}

void Font_code_GB2312(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  LCD_DataWrite(temp);
}

void Font_code_GB12345(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
   temp |= 0x04;
  LCD_DataWrite(temp);
}

void Font_code_BIG5(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  temp |= 0x08;
  LCD_DataWrite(temp);
}

void Font_code_UNICODE(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  temp |= 0x0C;
  LCD_DataWrite(temp);
}

void Font_code_ASCII(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  temp |= 0x10;
  LCD_DataWrite(temp);
}

void Font_code_UNIJIS(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  temp |= 0x14;
  LCD_DataWrite(temp);
}

void Font_code_JIS0208(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  temp |= 0x18;
  LCD_DataWrite(temp);
}

void Font_code_LATIN(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xE3;
  temp |= 0x1C;
  LCD_DataWrite(temp);
}

void Font_Standard(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xFC;
  LCD_DataWrite(temp);
}

void Font_Arial(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xFC;
  temp |= 0x01;
  LCD_DataWrite(temp);
}

void Font_Roman(void)
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= 0xFC;
  temp |= 0x02;
  LCD_DataWrite(temp);
}

void Font_Bold(void) //for ASCII
{ uchar temp;
  LCD_CmdWrite(0x2F);//FNCR1
  temp = LCD_DataRead();
  temp &= cClrD1D0;
  temp |= cSetD1D0;
  LCD_DataWrite(temp);
}

//-------------------------------------------//
//REG[30h]~REG[37h]
void Active_Window(uint XL,uint XR ,uint YT ,uint YB)
{
    //setting active window X
  
    LCD_CmdWrite(0x30);//HSAW0
	LCD_DataWrite(XL); 
    LCD_CmdWrite(0x31);//HSAW1	   
	LCD_DataWrite(XL>>8);

    LCD_CmdWrite(0x34);//HEAW0
	LCD_DataWrite(XR);
    LCD_CmdWrite(0x35);//HEAW1	   
	LCD_DataWrite(XR>>8);

    //setting active window Y
  
    LCD_CmdWrite(0x32);//VSAW0
	LCD_DataWrite(YT);  
    LCD_CmdWrite(0x33);//VSAW1	   
	LCD_DataWrite(YT>>8);
  
    LCD_CmdWrite(0x36);//VEAW0
	LCD_DataWrite(YB); 
    LCD_CmdWrite(0x37);//VEAW1	   
	LCD_DataWrite(YB>>8);
}
//--------------------------------------------//
//REG[38h]~REG[3Fh]

void Scroll_Window(uint XL,uint XR ,uint YT ,uint YB)
{
 
    LCD_CmdWrite(0x38);//HSSW0
	LCD_DataWrite(XL);
    LCD_CmdWrite(0x39);//HSSW1	   
	LCD_DataWrite(XL>>8);
  
    LCD_CmdWrite(0x3c);//HESW0
	LCD_DataWrite(XR);  
    LCD_CmdWrite(0x3d);//HESW1	   
	LCD_DataWrite(XR>>8);   
    
    LCD_CmdWrite(0x3a);//VSSW0
	LCD_DataWrite(YT);
    LCD_CmdWrite(0x3b);//VSSW1	   
	LCD_DataWrite(YT>>8);
  
    LCD_CmdWrite(0x3e);//VESW0
	LCD_DataWrite(YB);  
    LCD_CmdWrite(0x3f);//VESW1	   
	LCD_DataWrite(YB>>8);
}  


//--------------------------------------------//
//REG[40h]
void Graphic_Mode(void)
{	uchar temp;
	LCD_CmdWrite(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	LCD_DataWrite(temp);
}

void Text_Mode(void)
{	uchar temp;
	LCD_CmdWrite(0x40);//MWCR0
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	LCD_DataWrite(temp);	 
} 

void Text_Cursor_Disable(void)
{	uchar temp;
	LCD_CmdWrite(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD6 ;
	LCD_DataWrite(temp);
}

void Text_Cursor_Enable(void)
{	uchar temp;
	LCD_CmdWrite(0x40);//MWCR0
	temp = LCD_DataRead();
	temp |= cSetD6 ;
	LCD_DataWrite(temp);	 	
} 

void Text_Cursor_Blink_Disable(void)
{	uchar temp;
	LCD_CmdWrite(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD5 ;
	LCD_DataWrite(temp);
}

void Text_Cursor_Blink_Enable(void)
{	uchar temp;
	LCD_CmdWrite(0x40);//MWCR0
	temp = LCD_DataRead();
	temp |= cSetD5 ;
	LCD_DataWrite(temp);
}

void Memory_Write_LeftRight_TopDown(void)
{	uchar temp;
	LCD_CmdWrite(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	LCD_DataWrite(temp);
}

void Memory_Write_RightLeft_TopDown(void)
{	uchar temp;
	LCD_CmdWrite(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	temp |= cClrD3SetD2 ;
	LCD_DataWrite(temp);
}

void Memory_Write_DownTop_LeftRight(void)
{	uchar temp;	
	LCD_CmdWrite(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3D2 ;
	LCD_DataWrite(temp);
}

void Memory_Write_TopDown_LeftRight(void)
{	uchar temp;
	LCD_CmdWrite(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	LCD_DataWrite(temp);
} 

void MemoryWrite_Cursor_autoIncrease(void)
{	uchar temp;
	LCD_CmdWrite(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD1 ;
	LCD_DataWrite(temp);
}

void MemoryWrite_Cursor_NoautoIncrease(void)
{	uchar temp;
	LCD_CmdWrite(0x40);//MWCR0
	temp = LCD_DataRead();
	temp |= cSetD1 ;
	LCD_DataWrite(temp);
}

void MemoryRead_Cursor_autoIncrease(void)
{	uchar temp;
	LCD_CmdWrite(0x40);//MWCR0
	temp = LCD_DataRead();
	temp &= cClrD0 ;
	LCD_DataWrite(temp);
}

void MemoryRead_Cursor_NoautoIncrease(void)
{
	uchar temp;

	LCD_CmdWrite(0x40);//MWCR0
	temp = LCD_DataRead();
	temp |= cSetD0 ;
	LCD_DataWrite(temp);
}  										 

//--------------------------------------------//
//REG[41h]
void No_Graphic_Cursor(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	LCD_DataWrite(temp);
}

void Graphic_Cursor(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	LCD_DataWrite(temp);
}

void Graphic_Cursor_Set1(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4;
	LCD_DataWrite(temp);
}

void Graphic_Cursor_Set2(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6D5SetD4 ;
	LCD_DataWrite(temp);
}

void Graphic_Cursor_Set3(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6SetD5ClrD4 ;
	LCD_DataWrite(temp);
}

void Graphic_Cursor_Set4(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6SetD5D4 ;
	LCD_DataWrite(temp);
}

void Graphic_Cursor_Set5(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6ClrD5D4 ;
	LCD_DataWrite(temp);
}

void Graphic_Cursor_Set6(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6ClrD5SetD4 ;
	LCD_DataWrite(temp);
}

void Graphic_Cursor_Set7(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6D5ClrD4 ;
	LCD_DataWrite(temp);
}

void Graphic_Cursor_Set8(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6D5D4 ;
	LCD_DataWrite(temp);
}

void Write_To_Bank1and2(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD3D2 ;
	LCD_DataWrite(temp);
}

void Write_To_CGRAM(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= 0xf3;
	temp |= cClrD3SetD2 ;
	LCD_DataWrite(temp);
}

void Write_To_GraphicCursor(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= 0xf3;
	temp |= cSetD3ClrD2 ;
	LCD_DataWrite(temp);
}

void Write_To_Pattern(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= 0xf3;
	temp |= cSetD3D2 ;
	LCD_DataWrite(temp);
}
						   
void Write_To_Bank1(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp &= cClrD0 ;
	LCD_DataWrite(temp);
}

void Write_To_Bank2(void)
{	uchar temp;
	LCD_CmdWrite(0x41);//MWCR1
	temp = LCD_DataRead();
	temp |= cSetD0 ;
	LCD_DataWrite(temp);  
}	
							
//--------------------------------------------//
//--------------------------------------------//
//REG[44h]
void Text_Blink_Time(uchar setx)
{	
	LCD_CmdWrite(0x44);//CURS
	LCD_DataWrite(setx);
}

//--------------------------------------------//
//REG[45h]  
void Memory_read_LeftRight_TopDown(void)
{	uchar temp;
	LCD_CmdWrite(0x45);//MRCD
	temp = LCD_DataRead();
	temp &= cClrD1D0;
	LCD_DataWrite(temp);
}
void Memory_read_RightLeft_TopDown(void)
{	uchar temp;
	LCD_CmdWrite(0x45);//MRCD
	temp = LCD_DataRead();
	temp |= cClrD1SetD0;
	LCD_DataWrite(temp);
}
void Memory_read_TopDown_LeftRight(void)
{	uchar temp;
	LCD_CmdWrite(0x45);//MRCD
	temp = LCD_DataRead();
	temp |= cSetD1ClrD0;
	LCD_DataWrite(temp);
}
void Memory_read_DownTop_LeftRight(void)
{	uchar temp;
	LCD_CmdWrite(0x45);//MRCD
	temp = LCD_DataRead();
	temp |= cSetD1D0;
	LCD_DataWrite(temp);
}			

//--------------------------------------------//
//REG[46h]~REG[49h]
void XY_Coordinate(uint X,uint Y)
{
    LCD_CmdWrite(0x46);
	LCD_DataWrite(X);  
    LCD_CmdWrite(0x47);	   
	LCD_DataWrite(X>>8);
 
    LCD_CmdWrite(0x48);
	LCD_DataWrite(Y);  
    LCD_CmdWrite(0x49);	   
	LCD_DataWrite(Y>>8);
}



//--------------------------------------------//
//REG[4Ah]~REG[4Dh]
void Memory_read_Coordinate(uint X,uint Y)
{

    LCD_CmdWrite(0x4a);
	LCD_DataWrite(X);   
    LCD_CmdWrite(0x4b);	   
	LCD_DataWrite(X>>8);
   
    LCD_CmdWrite(0x4c);
	LCD_DataWrite(Y);
    LCD_CmdWrite(0x4d);	   
	LCD_DataWrite(Y>>8);
} 
						  
//--------------------------------------------//
//REG[4Eh]
void Text_Cursor_Horizontal_Size(uchar setx)//bit[3:0]
{
    LCD_CmdWrite(0x4E);//CURS	   
	LCD_DataWrite(setx);
} 
								
//REG[4Fh] 
void Text_Cursor_Vertical_Size(uchar setx)//bit[3:0]
{
    LCD_CmdWrite(0x4F);//CURS	   
	LCD_DataWrite(setx);
} 	
	
//--------------------------------------------//
//REG[50h] 
void no_BTE_write(void)
{	uchar temp;
	LCD_CmdWrite(0x50);//BECR0
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	LCD_DataWrite(temp);  
}

void BTE_enable(void)
{	uchar temp;
	LCD_CmdWrite(0x50);//BECR0
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	LCD_DataWrite(temp);  
}

void BTE_contiguous_Data(void)
{	uchar temp;
	LCD_CmdWrite(0x50);//BECR0
	temp = LCD_DataRead();
	temp |= cSetD6 ;
	LCD_DataWrite(temp);  
}

void BTE_rectangular_Data(void)
{	uchar temp;												   
	LCD_CmdWrite(0x50);//BECR0
	temp = LCD_DataRead();
	temp &= cClrD6 ;
	LCD_DataWrite(temp);  
}									  

//---------------------------------------------//
//REG[51h] 
void BTE_ROP_Code(uchar setx)
{	 
    LCD_CmdWrite(0x51);//BECR1	   
	LCD_DataWrite(setx); 
}									   

//---------------------------------------------//
//REG[52h] 
void Layer1_Visible(void)
{	uchar temp;
	LCD_CmdWrite(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0xf8;
	LCD_DataWrite(temp);  
}

void Layer2_Visible(void)
{   uchar temp;
	LCD_CmdWrite(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0xf8;
	temp|=0x01;
	LCD_DataWrite(temp);  
}	
						
void Transparent_Mode(void)
{	uchar temp;
	LCD_CmdWrite(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0xf8;
	temp|=0x03;
	LCD_DataWrite(temp);  
}

void Lighten_Overlay_Mode(void)
{   uchar temp;
	LCD_CmdWrite(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0xf8;
	temp|=0x02;
	LCD_DataWrite(temp);  
}	
						
void Boolean_OR(void)
{   uchar temp;
	LCD_CmdWrite(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0xf8;
	temp|=0x04;
	LCD_DataWrite(temp);
}

void Boolean_AND(void)
{	uchar temp;
	LCD_CmdWrite(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0xf8;
	temp|=0x05;
	LCD_DataWrite(temp);  
}  	 
	
  
void Floating_window_mode(void)
{	uchar temp;
	LCD_CmdWrite(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0xf8;
	temp|=0x06;
	LCD_DataWrite(temp);  
} 

void Floating_Window_transparent_with_BGTR_enable(void)
{   uchar temp;
	LCD_CmdWrite(0x52);//LTPR0
	temp = LCD_DataRead();
	temp|=cSetD5;
	LCD_DataWrite(temp);  
}

void Floating_Window_transparent_with_BGTR_disable(void)
{   uchar temp;
	LCD_CmdWrite(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=cClrD5;
	LCD_DataWrite(temp);  
}


void Layer1_2_scroll(void)
{	uchar temp;
	LCD_CmdWrite(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0x3f;
	LCD_DataWrite(temp);  
}  	 

void Layer1_scroll(void)
{	uchar temp;
	LCD_CmdWrite(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0x3f;
	temp|=0x40;
	LCD_DataWrite(temp);  
} 

void Layer2_scroll(void)
{	uchar temp;
	LCD_CmdWrite(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0x3f;
	temp|=0x80;
	LCD_DataWrite(temp);  
} 

void Buffer_scroll(void)
{	uchar temp;
	LCD_CmdWrite(0x52);//LTPR0
	temp = LCD_DataRead();
	temp&=0x3f;
	temp|=0xC0;
	LCD_DataWrite(temp);  
} 

//---------------------------------------------//
//REG[53h] 
void layer2_1_transparency(uchar setx)
{  
   LCD_CmdWrite(0x53);//LTPR1
   LCD_DataWrite(setx); 
}

//---------------------------------------------//
//REG[54h]~ [5Fh]
void BTE_Source_Destination	(uint XL,uint XR ,uint YT ,uint YB)
{
	uchar temp,temp1;
    
	temp=XL;   
    LCD_CmdWrite(0x54);//HSBE0
	LCD_DataWrite(temp);
	temp=XL>>8;   
    LCD_CmdWrite(0x55);//HSBE1	   
	LCD_DataWrite(temp);

	temp=XR;   
    LCD_CmdWrite(0x58);//HDBE0
	LCD_DataWrite(temp);
	temp=XR>>8;   
    LCD_CmdWrite(0x59);//HDBE1	   
	LCD_DataWrite(temp); 
    
	temp=YT;   
    LCD_CmdWrite(0x56);//VSBE0
	LCD_DataWrite(temp);
	temp=YT>>8;   
    LCD_CmdWrite(0x57);//VSBE1
	temp1 = LCD_DataRead();
	temp1 &= 0x80;
    temp=temp|temp1; 
	LCD_CmdWrite(0x57);//VSBE1  
	LCD_DataWrite(temp);


	temp=YB;   
    LCD_CmdWrite(0x5a);//VDBE0
	LCD_DataWrite(temp);
	temp=YB>>8;   
    LCD_CmdWrite(0x5b);//VDBE1
	temp1 = LCD_DataRead();
	temp1 &= 0x80;
	temp=temp|temp1;	
	LCD_CmdWrite(0x5b);//VDBE1   
	LCD_DataWrite(temp);
}							

void Source_Layer1(void)
{   uchar temp;	
	LCD_CmdWrite(0x57);//VSBE1
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	LCD_DataWrite(temp);
}

void Source_Layer2(void)
{	uchar temp;	
	LCD_CmdWrite(0x57);//VSBE1
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	LCD_DataWrite(temp);
}

void Destination_Layer1(void)
{	uchar temp;	
	LCD_CmdWrite(0x5b);//VDBE1
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	LCD_DataWrite(temp);
}

void Destination_Layer2(void)
{	uchar temp;	
	LCD_CmdWrite(0x5b);//VDBE1
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	LCD_DataWrite(temp);
}
		  
//---------------------------------------------//
//---------------------------------------------//
void BTE_Size_setting(uint X,uint Y)
{	 
    LCD_CmdWrite(0x5c);//BEWR0
	LCD_DataWrite(X); 
    LCD_CmdWrite(0x5d);//BEWR1	   
	LCD_DataWrite(X>>8);
  
    LCD_CmdWrite(0x5e);//BEHR0
	LCD_DataWrite(Y);   
    LCD_CmdWrite(0x5f);//BEHR1	   
	LCD_DataWrite(Y>>8);
}									 

//---------------------------------------------//
//REG[60h]~REG[67h]

void Background_color(uint color)
{
 #ifdef Color65k
    LCD_CmdWrite(0x60);
	LCD_DataWrite((color&0xf800)>>11);
	
    LCD_CmdWrite(0x61);
	LCD_DataWrite((color&0x07e0)>>5);
	
	LCD_CmdWrite(0x62);
	LCD_DataWrite((color&0x001f));
 #endif
	
 #ifdef Color256
    LCD_CmdWrite(0x60);
	LCD_DataWrite((color&0xf800)>>13);
	
    LCD_CmdWrite(0x61);
	LCD_DataWrite((color&0x07e0)>>8);
	
	LCD_CmdWrite(0x62);
	LCD_DataWrite((color&0x001f)>>3);
 #endif
}

 void Text_color(uint color) 
{
 #ifdef Color65k
    LCD_CmdWrite(0x63);
	LCD_DataWrite((color&0xf800)>>11);
	
    LCD_CmdWrite(0x64);
	LCD_DataWrite((color&0x07e0)>>5);
	
	LCD_CmdWrite(0x65);
	LCD_DataWrite((color&0x001f));
 #endif
 
  #ifdef Color256
    LCD_CmdWrite(0x63);
	LCD_DataWrite((color&0xf800)>>13);
	
    LCD_CmdWrite(0x64);
	LCD_DataWrite((color&0x07e0)>>8);
	
	LCD_CmdWrite(0x65);
	LCD_DataWrite((color&0x001f)>>3);
 #endif
 
}



//--------------------------------------------//
void Text_Background_Color(uchar setR,uchar setG,uchar setB)
{
    LCD_CmdWrite(0x60);//BGCR0
	LCD_DataWrite(setR);
   
    LCD_CmdWrite(0x61);//BGCR0
	LCD_DataWrite(setG);

    LCD_CmdWrite(0x62);//BGCR0
	LCD_DataWrite(setB);
} 

void Text_Foreground_Color(uchar setR,uchar setG,uchar setB)
{	    
    LCD_CmdWrite(0x63);//BGCR0
	LCD_DataWrite(setR);
   
    LCD_CmdWrite(0x64);//BGCR0
	LCD_DataWrite(setG);

    LCD_CmdWrite(0x65);//BGCR0
	LCD_DataWrite(setB);

}


void BTE_Background_color(uchar setR,uchar setG,uchar setB)
{
    LCD_CmdWrite(0x60);//BGCR0
	LCD_DataWrite(setR);
    LCD_CmdWrite(0x61);//BGCR0
	LCD_DataWrite(setG);
    LCD_CmdWrite(0x62);//BGCR0
	LCD_DataWrite(setB);
}


void BTE_Foreground_color(uchar setR,uchar setG,uchar setB)
{
    LCD_CmdWrite(0x63);//BGCR0
	LCD_DataWrite(setR);
    LCD_CmdWrite(0x64);//BGCR0
	LCD_DataWrite(setG);
    LCD_CmdWrite(0x65);//BGCR0
	LCD_DataWrite(setB);
}


//======================================
//for test
void BTE_Background_red(uchar setx)
{	  
    LCD_CmdWrite(0x60);//BGCR0
	LCD_DataWrite(setx);
}	
						
void BTE_Background_green(uchar setx)
{	  
    LCD_CmdWrite(0x61);//BGCR1
	LCD_DataWrite(setx);
}							  

void BTE_Background_blue(uchar setx)
{	 
    LCD_CmdWrite(0x62);//BGCR2
	LCD_DataWrite(setx);
} 								

void BTE_Foreground_red(uchar setx)
{	
    LCD_CmdWrite(0x63);//FGCR0
	LCD_DataWrite(setx);
}								 

void BTE_Foreground_green(uchar setx)
{	  
    LCD_CmdWrite(0x64);//FGCR1
	LCD_DataWrite(setx);
}							 

void BTE_Foreground_blue(uchar setx)
{  
    LCD_CmdWrite(0x65);//FGCR2
	LCD_DataWrite(setx);
} 	

//======================================
	
void Pattern_Set_8x8(void)
{  uchar temp;

 LCD_CmdWrite(0x66);//PTNO
 temp = LCD_DataRead();
 temp &= cClrD7;
 LCD_DataWrite(temp);
}	

void Pattern_Set_16x16(void)
{  uchar  temp;

 LCD_CmdWrite(0x66);//PTNO
 temp = LCD_DataRead();
 temp |= cSetD7; 
 LCD_DataWrite(temp);
}								


void Pattern_Set_number(uchar setx)
{   uchar temp;
        
    LCD_CmdWrite(0x66);//PTNO
    temp = LCD_DataRead();
    temp &= 0x80;    
	LCD_DataWrite(temp|setx);
} 	
							
void BackgroundColor_TransparentMode(uchar setR,uchar setG,uchar setB)
{
    LCD_CmdWrite(0x67);//BGTR_R
	LCD_DataWrite(setR);

    LCD_CmdWrite(0x68);//BGTR_G
	LCD_DataWrite(setG);

    LCD_CmdWrite(0x69);//BGTR_B
	LCD_DataWrite(setB);

}	  

//REG[70h]~REG[74h]
//==============================================================================
//Subroutine:	Enable_TP 
//==============================================================================
void Enable_TP(void)
{   uchar temp;	  
	LCD_CmdWrite(0x70);//TPCR0	
	temp = LCD_DataRead();
	temp |= cSetD7;   
	LCD_DataWrite(temp);
}
//==============================================================================
//Subroutine:	Disable_TP 
//==============================================================================
void Disable_TP(void)
{ uchar temp;
  LCD_CmdWrite(0x70);//TPCR0
  temp = LCD_DataRead();
  temp &= cClrD7;   
  LCD_DataWrite(temp);
 }

//==============================================================================
//Subroutine:	Enable_Touch_WakeUp
//==============================================================================
void Enable_Touch_WakeUp(void)
{
    uchar temp;
	LCD_CmdWrite(0x70);//TPCR0	
	temp = LCD_DataRead();
	temp |= cSetD3;   //cSetb3 = 00001000
	LCD_DataWrite(temp);
}
//==============================================================================
//Subroutine:	Disable_Touch_WakeUp
//==============================================================================
void Disable_Touch_WakeUp(void)
{	uchar temp;
	LCD_CmdWrite(0x70);//TPCR0	
	temp = LCD_DataRead();
	temp &= cClrD3;
	LCD_DataWrite(temp);
}

//==============================================================================
//Subroutine:	TP manual or auto  mode enable
//==============================================================================
void TP_manual_mode(void)
{	uchar temp;
  	LCD_CmdWrite(0x71);//TPCR1	
	temp = LCD_DataRead();
	temp |= cSetD6;
	LCD_DataWrite(temp);
}
void TP_auto_mode(void)
{	uchar temp;
  	LCD_CmdWrite(0x71);//TPCR1	
	temp = LCD_DataRead();
	temp &= cClrD6;
	LCD_DataWrite(temp);
}
//==============================================================================
//Subroutine:	TP_IDLE_mode
//==============================================================================
void TP_IDLE_mode(void)
{ uchar temp;
  LCD_CmdWrite(0x71);//TPCR1
  temp=LCD_DataRead(); 
  temp &= 0xf0;
  LCD_DataWrite(temp);
 }
//==============================================================================
//Subroutine:	TP_event_mode
//==============================================================================
void TP_event_mode(void)
{ uchar temp;
  LCD_CmdWrite(0x71);//TPCR1
  temp=LCD_DataRead(); 
  temp &= 0xf0;
  temp |= 0x01;
  LCD_DataWrite(temp);
//  LCD_DataWrite(0xc1);
}
//==============================================================================
//Subroutine:	TP_latch_X
//==============================================================================
void TP_latch_X(void)
{ uchar temp;
  LCD_CmdWrite(0x71);//TPCR1
  temp=LCD_DataRead(); 
  temp &= 0xc0;
  temp |= 0x02;
  LCD_DataWrite(temp);
 }
 //==============================================================================
//Subroutine:	TP_latch_Y
//==============================================================================
void TP_latch_Y(void)
{ uchar temp;
  LCD_CmdWrite(0x71);//TPCR1
  temp=LCD_DataRead(); 
  temp &= 0xc0;
  temp |= 0x03;
  LCD_DataWrite(temp);
 }
//==============================================================================
//Subroutine:	Detect_TP_X_Y coordinate 
//==============================================================================
uchar ADC_X(void)
{	uchar temp;
	LCD_CmdWrite(0x72);//TPXH	 X_coordinate high byte
	//Chk_Busy();
	temp=LCD_DataRead();
	return temp;
}

uchar ADC_Y(void)
{	uchar temp;
	LCD_CmdWrite(0x73);//TPYH	  Y_coordinate high byte
	//Chk_Busy();
	temp=LCD_DataRead();
	return temp;
}

uchar ADC_XY(void)
{	uchar temp;
 	LCD_CmdWrite(0x74);//TPXYL	  bit[3:2] Y_coordinate low byte  bit[1:0] X_coordinate low byte 
	temp=LCD_DataRead();
	return temp;
}   



//REG[80h]~REG[83h]
void Graphic_Cursor_Coordinate(uint X,uint Y)
{
    LCD_CmdWrite(0x80);
	LCD_DataWrite(X);  
    LCD_CmdWrite(0x81);	   
	LCD_DataWrite(X>>8);
  
    LCD_CmdWrite(0x82);
	LCD_DataWrite(Y);  
    LCD_CmdWrite(0x83);	   
	LCD_DataWrite(Y>>8);
}

//---------------------------------//
//REG[84h]

void Graphic_Cursor_Color_0(uchar setx)
{	  
    LCD_CmdWrite(0x84);//GCC0
	LCD_DataWrite(setx); 
}  									  

void Graphic_Cursor_Color_1(uchar setx)
{	 
    LCD_CmdWrite(0x85);//GCC1
	LCD_DataWrite(setx); 
}  	
 	 						  
//---------------------------------------------//
//REG[8A]
void PWM1_enable(void)
{
	uchar temp;
	LCD_CmdWrite(0x8a);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	LCD_DataWrite(temp);  
 }

void PWM1_disable(void)
{
	uchar temp;
	LCD_CmdWrite(0x8a);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	LCD_DataWrite(temp);  
 }

void PWM1_disable_level_low(void)
{
	uchar temp;
	LCD_CmdWrite(0x8a);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD6 ;
	LCD_DataWrite(temp);  
}

void PWM1_disable_level_high(void)
{
	uchar temp;
	LCD_CmdWrite(0x8a);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD6 ;
	LCD_DataWrite(temp);  
}

 void PWM1_fnuction_sel(void)
{
	uchar temp;
	LCD_CmdWrite(0x8a);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD4 ;
	LCD_DataWrite(temp);  
 }

 void PWM1_system_clk_out(void)
{
	uchar temp;
	LCD_CmdWrite(0x8a);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD4 ;
	LCD_DataWrite(temp);  
 }

 void PWM1_clock_ratio(uchar setx) //bit0~3 
{
	uchar temp,temp1;
	temp1= setx&0x0f;
	LCD_CmdWrite(0x8a);//MCLR
	temp = LCD_DataRead();
	temp &= 0xf0;
	temp |= temp1 ;
	LCD_DataWrite(temp);  
 }

 void PWM1_duty_cycle(uchar setx) //bit0~7
{
  
    LCD_CmdWrite(0x8b);//PTNO
	LCD_DataWrite(setx);
} 	

void PWM2_enable(void)
{
	uchar temp;
	LCD_CmdWrite(0x8c);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	LCD_DataWrite(temp);  
 }

void PWM2_disable(void)
{
	uchar temp;
	LCD_CmdWrite(0x8c);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	LCD_DataWrite(temp);  
 }

void PWM2_disable_level_low(void)
{
	uchar temp;
	LCD_CmdWrite(0x8c);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD6 ;
	LCD_DataWrite(temp);  
}

void PWM2_disable_level_high(void)
{
	uchar temp;
	LCD_CmdWrite(0x8c);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD6 ;
	LCD_DataWrite(temp);  
}

 void PWM2_fnuction_sel(void)
{
	uchar temp;
	LCD_CmdWrite(0x8c);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD4 ;
	LCD_DataWrite(temp);  
 }

  void PWM2_system_clk_out(void)
{
	uchar temp;
	LCD_CmdWrite(0x8c);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD4 ;
	LCD_DataWrite(temp);  
 }

  void PWM2_clock_ratio(uchar setx) //bit0~3 
{
	uchar temp,temp1;
	temp1= setx&0x0f;
	LCD_CmdWrite(0x8c);//MCLR
	temp = LCD_DataRead();
	temp &= 0xf0;
	temp |= temp1 ;
	LCD_DataWrite(temp);  
 }
 void PWM2_duty_cycle(uchar setx) //bit0~7
{  
    LCD_CmdWrite(0x8d);//PTNO
	LCD_DataWrite(setx);
} 	


//---------------------------------------------//
//REG[8Eh]
void Stop_Memory_Clear(void)
{
	uchar temp;
	LCD_CmdWrite(0x8e);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD7 ;
	LCD_DataWrite(temp);  
}						 
void Memory_Clear(void)
{
	uchar temp;
	LCD_CmdWrite(0x8e);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD7 ;
	LCD_DataWrite(temp); 
	Chk_Busy(); 
}
void Clear_Full_Window(void)
{
	uchar temp;
	LCD_CmdWrite(0x8e);//MCLR
	temp = LCD_DataRead();
	temp &= cClrD6 ;
	LCD_DataWrite(temp);  
}
void Clear_Active_Window(void)
{
	uchar temp;
	LCD_CmdWrite(0x8e);//MCLR
	temp = LCD_DataRead();
	temp |= cSetD6 ;
	LCD_DataWrite(temp);  
}

									
//---------------------------------------------//
//REG[90h] 
void Draw_line(void)
{ 
  LCD_CmdWrite(0x90);//DCR
  LCD_DataWrite(0x00);
  LCD_DataWrite(0x80);
}

void Draw_square(void)
{
  LCD_CmdWrite(0x90);//DCR
  LCD_DataWrite(0x10);
  LCD_DataWrite(0x90);
}

void Draw_square_fill(void)
{
  LCD_CmdWrite(0x90);//DCR
  LCD_DataWrite(0x10);
  LCD_DataWrite(0xb0);
}

void Draw_circle(void)
{
  LCD_CmdWrite(0x90);//DCR
  LCD_DataWrite(0x40);
}

void Draw_circle_fill(void)
{
  LCD_CmdWrite(0x90);//DCR
  LCD_DataWrite(0x60);
}

void Draw_Triangle(void)
{
   LCD_CmdWrite(0x90);//DCR
   LCD_DataWrite(0x00);
   LCD_DataWrite(0x81);
}

void Draw_Triangle_fill(void)
{
   LCD_CmdWrite(0x90);//DCR
   LCD_DataWrite(0x01);
   LCD_DataWrite(0xA1);
}

//---------------------------------------------//
//REG[90h]~/EG[9Dh]  									
void Geometric_Coordinate(uint XL,uint XR ,uint YT ,uint YB)
{	  
    LCD_CmdWrite(0x91);
	LCD_DataWrite(XL);   
    LCD_CmdWrite(0x92);	   
	LCD_DataWrite(XL>>8);
  
    LCD_CmdWrite(0x95);
	LCD_DataWrite(XR);  
    LCD_CmdWrite(0x96);	   
	LCD_DataWrite(XR>>8);
	     
    LCD_CmdWrite(0x93);
	LCD_DataWrite(YT);  
    LCD_CmdWrite(0x94);	   
	LCD_DataWrite(YT>>8);
   
    LCD_CmdWrite(0x97);
	LCD_DataWrite(YB);   
    LCD_CmdWrite(0x98);	   
	LCD_DataWrite(YB>>8);
}

void Circle_Coordinate_Radius(uint X,uint Y,uint R)
{
      
    LCD_CmdWrite(0x99);
	LCD_DataWrite(X);  
    LCD_CmdWrite(0x9a);	   
	LCD_DataWrite(X>>8);  
	   
    LCD_CmdWrite(0x9b);
	LCD_DataWrite(Y); 
    LCD_CmdWrite(0x9c);	   
	LCD_DataWrite(Y>>8);
 
    LCD_CmdWrite(0x9d);
	LCD_DataWrite(R);
} 

//---------------------------------------------//
//REG[A0h] 
void Draw_Ellipse(void)
{
   LCD_CmdWrite(0xA0);//DCR
   LCD_DataWrite(0x00);
   LCD_DataWrite(0x80);
}

void Draw_Ellipse_Curve(void)
{  uchar temp;

   LCD_CmdWrite(0xA0);//DCR
   temp = LCD_DataRead();
   temp &=0x1F;
   temp |= 0x90;
   LCD_DataWrite(temp);
}

void Draw_Ellipse_Curve_Fill(void)
{  uchar temp;

   LCD_CmdWrite(0xA0);//DCR
   temp = LCD_DataRead();
   temp &=0x1F;
   temp |= 0xD0;
   LCD_DataWrite(temp);
}

void Draw_Ellipse_fill(void)
{
   LCD_CmdWrite(0xA0);//DCR
   LCD_DataWrite(0x40);
   LCD_DataWrite(0xC0);
}

void Draw_Circle_Square(void)
{
   LCD_CmdWrite(0xA0);//DCR
   LCD_DataWrite(0x20);
   LCD_DataWrite(0xA0);
}

void Draw_Circle_Square_fill(void)
{
   LCD_CmdWrite(0xA0);//DCR
   LCD_DataWrite(0x60);
   LCD_DataWrite(0xE0);
}


void Draw_Ellipse_Curve_part(uchar setx)
{
   uchar temp,temp1;
   LCD_CmdWrite(0xA0);//DCR
   temp = LCD_DataRead();
   temp1 = setx&0x03;
   temp &=0xfc;
   temp |=temp1;
   LCD_DataWrite(temp);   
}


void Ellipse_Coordinate_setting(uint X,uint Y,uint ELL_A,uint ELL_B)
{

    LCD_CmdWrite(0xA5);
	LCD_DataWrite(X);
    LCD_CmdWrite(0xA6);	   
	LCD_DataWrite(X>>8);  
	   
    LCD_CmdWrite(0xA7);
	LCD_DataWrite(Y); 
    LCD_CmdWrite(0xA8);	   
	LCD_DataWrite(Y>>8);
  
    LCD_CmdWrite(0xA1);
	LCD_DataWrite(ELL_A);
	LCD_CmdWrite(0xA2);
	LCD_DataWrite(ELL_A>>8);
  
    LCD_CmdWrite(0xA3);
	LCD_DataWrite(ELL_B);
	LCD_CmdWrite(0xA4);
	LCD_DataWrite(ELL_B>>8);
} 



void Circle_Square_Coordinate_setting(uint XL,uint XR ,uint YT ,uint YB ,uint ELL_A,uint ELL_B)
{	    
  
    LCD_CmdWrite(0x91);
	LCD_DataWrite(XL);  
    LCD_CmdWrite(0x92);	   
	LCD_DataWrite(XL>>8);

    LCD_CmdWrite(0x95);
	LCD_DataWrite(XR);   
    LCD_CmdWrite(0x96);	   
	LCD_DataWrite(XR>>8);
	     
    LCD_CmdWrite(0x93);
	LCD_DataWrite(YT);  
    LCD_CmdWrite(0x94);	   
	LCD_DataWrite(YT>>8);

    LCD_CmdWrite(0x97);
	LCD_DataWrite(YB);  
    LCD_CmdWrite(0x98);	   
	LCD_DataWrite(YB>>8);
  
    LCD_CmdWrite(0xA1);
	LCD_DataWrite(ELL_A);
	LCD_CmdWrite(0xA2);
	LCD_DataWrite(ELL_A>>8);
 
    LCD_CmdWrite(0xA3);
	LCD_DataWrite(ELL_B);
	LCD_CmdWrite(0xA4);
	LCD_DataWrite(ELL_B>>8); 
}

 
void Draw_Triangle_3point_Coordinate(uint P1_X,uint P1_Y,uint P2_X,uint P2_Y,uint P3_X,uint P3_Y)
{	    
  
    LCD_CmdWrite(0x91);
	LCD_DataWrite(P1_X);  
    LCD_CmdWrite(0x92);	   
	LCD_DataWrite(P1_X>>8);
  
    LCD_CmdWrite(0x93);
	LCD_DataWrite(P1_Y);  
    LCD_CmdWrite(0x94);	   
	LCD_DataWrite(P1_Y>>8);
 
    LCD_CmdWrite(0x95);
	LCD_DataWrite(P2_X);  
    LCD_CmdWrite(0x96);	   
	LCD_DataWrite(P2_X>>8);
 
    LCD_CmdWrite(0x97);
	LCD_DataWrite(P2_Y);  
    LCD_CmdWrite(0x98);	   
	LCD_DataWrite(P2_Y>>8);

    LCD_CmdWrite(0xA9);
	LCD_DataWrite(P3_X);  
    LCD_CmdWrite(0xAA);	   
	LCD_DataWrite(P3_X>>8);
  
    LCD_CmdWrite(0xAB);
	LCD_DataWrite(P3_Y);   
    LCD_CmdWrite(0xAC);	   
	LCD_DataWrite(P3_Y>>8);   
}

//---------------------------------------------//
//REG[B0]~REG[B3]
void DMA_Start_address_setting(ulong set_address)
{ 
  LCD_CmdWrite(0xB0);
  LCD_DataWrite(set_address);

  LCD_CmdWrite(0xB1);
  LCD_DataWrite(set_address>>8);

  LCD_CmdWrite(0xB2);
  LCD_DataWrite(set_address>>16);

  LCD_CmdWrite(0xB3);
  LCD_DataWrite(set_address>>24);

}

//input pic data quantity  for example  pic size = 800*480 = 384000
void DMA_Continuous_mode_size_setting(ulong set_size)
{ 
  LCD_CmdWrite(0xB4);
  LCD_DataWrite(set_size);

  LCD_CmdWrite(0xB6);
  LCD_DataWrite(set_size>>8);
 
  LCD_CmdWrite(0xB8);
  LCD_DataWrite(set_size>>16);

}

void DMA_block_mode_size_setting(uint BWR,uint BHR,uint SPWR)
{
  LCD_CmdWrite(0xB4);
  LCD_DataWrite(BWR);
  LCD_CmdWrite(0xB5);
  LCD_DataWrite(BWR>>8);

  LCD_CmdWrite(0xB6);
  LCD_DataWrite(BHR);
  LCD_CmdWrite(0xB7);
  LCD_DataWrite(BHR>>8);

  LCD_CmdWrite(0xB8);
  LCD_DataWrite(SPWR);
  LCD_CmdWrite(0xB9);
  LCD_DataWrite(SPWR>>8);  
}

//---------------------------------------------//
//REG[BFh]
void DMA_Continuous_mode(void)
{ 
  LCD_CmdWrite(0xBF);
  LCD_DataWrite(0x00);
}

void DMA_Block_mode(void)
{ 
  LCD_CmdWrite(0xBF);
  LCD_DataWrite(0x02);
}

void DMA_Start_enable(void)
{  uchar temp;
  LCD_CmdWrite(0xBF);
  temp = LCD_DataRead();
  temp |= 0x01;
  LCD_DataWrite(temp);
}

//---------------------------------------------//
//REG[C0h]
void Key_Scan_Enable(void)
{ uchar temp;
  LCD_CmdWrite(0xC0);
  temp = LCD_DataRead();
  temp |= 0x80;
  LCD_DataWrite(temp);  
}


void Key_Scan_setting(uchar setx)//bit[2:0]
{ uchar temp;
  LCD_CmdWrite(0xC0);
  temp = LCD_DataRead();
  temp &= 0xf0;
  temp|= (setx&0x07);
  LCD_DataWrite(temp);  
} 

void Long_Key_Scan_enable(void)
{ uchar temp;
  LCD_CmdWrite(0xC0);
  temp = LCD_DataRead();
  temp |= 0x40;
  LCD_DataWrite(temp);
}

void Key_Scan_Wakeup_Function_Enable(void)
{ uchar temp;
  LCD_CmdWrite(0xC1);	
  temp = LCD_DataRead();
  temp |= 0x80;	// 
  LCD_DataWrite(temp);
}

void Long_Key_Timing_Adjustment(uchar setx)
{
   uchar temp,temp1;
  temp = setx & 0x1c;
  LCD_CmdWrite(0xC1);
  temp1 = LCD_DataRead();
  temp1|=temp;
  LCD_DataWrite(temp1);   
 
}

//---------------------------------------------//
//REG[D0]
void Floating_window_start_point(uint X,uint Y)
{ 
   LCD_CmdWrite(0xD0);
   LCD_DataWrite(X);
   LCD_CmdWrite(0xD1);
   LCD_DataWrite(X>>8);

   LCD_CmdWrite(0xD2);
   LCD_DataWrite(Y);
   LCD_CmdWrite(0xD3);
   LCD_DataWrite(Y>>8);  
}


void Floating_window_size(uint width_X,uint high_Y)
{

   LCD_CmdWrite(0xD4);
   LCD_DataWrite(width_X);
   LCD_CmdWrite(0xD5);
   LCD_DataWrite(width_X>>8);

   LCD_CmdWrite(0xD6);
   LCD_DataWrite(high_Y);
   LCD_CmdWrite(0xD7);
   LCD_DataWrite(high_Y>>8);

}

void Floating_window_display_point(uint X,uint Y)
{

   LCD_CmdWrite(0xD8);
   LCD_DataWrite(X);
   LCD_CmdWrite(0xD9);
   LCD_DataWrite(X>>8);

   LCD_CmdWrite(0xDA);
   LCD_DataWrite(Y);
   LCD_CmdWrite(0xDB);
   LCD_DataWrite(Y>>8);

}


//REG[E0h] 
void Serial_ROM_direct_access_mode_enable(void)
{
   LCD_CmdWrite(0xE0);
   LCD_DataWrite(0x01);
}

void Serial_ROM_direct_access_mode_disable(void)
{
   LCD_CmdWrite(0xE0);
   LCD_DataWrite(0x00);
}

void Direct_access_mode_Address_32bits(ulong set_address)
{
  uchar temp;  
  temp = set_address>>24;
  LCD_CmdWrite(0xE1);
  LCD_DataWrite(temp);
  temp = set_address>>16;
  LCD_DataWrite(temp);
  temp = set_address>>8;
  LCD_DataWrite(temp);
  temp = set_address;
  LCD_DataWrite(temp);

}

void Direct_access_mode_Address_24bits(ulong set_address)
{
  uchar temp=0;
  
  temp = set_address>>16;
  LCD_CmdWrite(0xE1);
  LCD_DataWrite(temp);  
  temp = set_address>>8;
  LCD_CmdWrite(0xE1);
  LCD_DataWrite(temp);   
  temp = set_address;
  LCD_CmdWrite(0xE1);
  LCD_DataWrite(temp);

}

uchar Direct_access_mode_Read_Data_buffer(void)
{
   uchar Serial_Data;

   LCD_CmdWrite(0xE2);
   Serial_Data=LCD_DataRead(); 
   return Serial_Data;
}

//---------------------------------------------//



//---------------------------------------------//
//REG[F0h] 

void KEYSCAN_Interrupt_Enable(void)
{ uchar temp;
  LCD_CmdWrite(0xF0);	
  temp = LCD_DataRead();
  temp |= 0x10;	// 
  LCD_DataWrite(temp); 
}

void KEYSCAN_Interrupt_Disable(void)
{ uchar temp;
  LCD_CmdWrite(0xF0);	
  temp = LCD_DataRead();
  temp &= 0x0F;	// 
  LCD_DataWrite(temp); 
}


void DMA_Interrupt_Enable(void)
{ uchar temp;
  LCD_CmdWrite(0xF0);	
  temp = LCD_DataRead();
  temp |= 0x08;	// 
  LCD_DataWrite(temp); 
}

void DMA_Interrupt_Disable(void)
{ uchar temp;
  LCD_CmdWrite(0xF0);	
  temp = LCD_DataRead();
  temp &= 0x17;	// 
  LCD_DataWrite(temp); 
}

void Touch_Panel_Interrupt_Enable(void)
{ uchar temp;
  LCD_CmdWrite(0xF0);//INTC
  temp=LCD_DataRead(); 
  temp |= 0x04;
  LCD_DataWrite(temp);
 }

void Touch_Panel_Interrupt_Disable(void)
{ uchar temp;
  LCD_CmdWrite(0xF0);//INTC
  temp=LCD_DataRead(); 
  temp &= 0x1B;
  LCD_DataWrite(temp);
 }

void BTE_Interrupt_Enable(void)
{
  uchar temp;
  LCD_CmdWrite(0xF0);//INTC
  temp=LCD_DataRead(); 
  temp |= 0x02;
  LCD_DataWrite(temp);
}

void BTE_Interrupt_Disable(void)
{
  uchar temp;
  LCD_CmdWrite(0xF0);//INTC
  temp=LCD_DataRead(); 
  temp &= 0x1D;
  LCD_DataWrite(temp);
}

void Font_BTE_read_write_Interrupt_Enable(void)
{
  uchar temp;
  LCD_CmdWrite(0xF0);//INTC
  temp=LCD_DataRead(); 
  temp |= 0x01;
  LCD_DataWrite(temp);
}

void Font_BTE_read_write_Interrupt_Disable(void)
{
  uchar temp;
  LCD_CmdWrite(0xF0);//INTC
  temp=LCD_DataRead(); 
  temp &= 0x1E;
  LCD_DataWrite(temp);
}

//---------------------------------------------//
//REG[F1h] 
void Clear_keyscan_interrupt(void)
{ uchar temp;
  LCD_CmdWrite(0xF1);//INTC
  temp=LCD_DataRead(); 
  temp |=cSetD4 ;
  LCD_DataWrite(temp);
}

bit Keyscan_Status(void)
{   uchar temp;
	LCD_CmdWrite(0xF1);//INTC	
	temp = LCD_DataRead();
	if ((temp&0x10)==0x10)
	return 1;
	else 
	return 0;
}

void Clear_DMA_interrupt(void)
{ uchar temp;
  LCD_CmdWrite(0xF1);//INTC
  temp=LCD_DataRead(); 
  temp |=cSetD3 ;
  LCD_DataWrite(temp);

}

bit DMA_Status(void)
{   uchar temp;
	LCD_CmdWrite(0xF1);//INTC	
	temp = LCD_DataRead();
	if ((temp&0x08)==0x08)
	return 1;
	else 
	return 0;
}

void clear_TP_interrupt(void)
{ uchar temp;
  LCD_CmdWrite(0xF1);//INTC
  temp=LCD_DataRead(); 
  temp |= cSetD2;
  LCD_DataWrite(temp);
 }


bit Touch_Status(void)
{	uchar temp;
	LCD_CmdWrite(0xF1);//INTC	
	temp = LCD_DataRead();
	if ((temp&0x04)==0x04)
	return 1;
	else 
	return 0;
}

void Clear_BTE_interrupt(void)
{ uchar temp;
  LCD_CmdWrite(0xF1);//INTC
  temp=LCD_DataRead(); 
  temp |= cSetD1;
  LCD_DataWrite(temp);
}

bit BTE_Status(void)
{	uchar temp;
	LCD_CmdWrite(0xF1);//INTC	
	temp = LCD_DataRead();
	if ((temp&0x02)==0x02)
	return 1;
	else 
	return 0;
}

void Clear_Font_BTE_read_write_Interrupt(void)
{ uchar temp;
  LCD_CmdWrite(0xF1);//INTC
  temp=LCD_DataRead(); 
  temp |= cSetD0;
  LCD_DataWrite(temp);
}

bit Font_BTE_read_write_Status(void)
{	uchar temp;
	LCD_CmdWrite(0xF1);//INTC	
	temp = LCD_DataRead();
	if ((temp&0x01)==0x01)
	return 1;
	else 
	return 0;
}



void Chk_Wait(void)
{
	
delay(10);	   
}


void Show_String(char *str,int n)
{   Text_Mode();
	LCD_CmdWrite(0x02);
	while(*str != '\0')
	{
	 LCD_DataWrite(*str);
	 ++str;	 	
	 Chk_Busy();
     //Chk_Wait();		
	}
   Delay1ms(n);
}


/*

void LCD_Print_J_Str(uchar *ptr, int char_num)
{
	int temp = 0;
	unsigned char SJIS_HB, SJIS_LB, JIS_HB, JIS_LB, LSB, Num=0;

	while(temp < char_num)
	{
		if(ptr[temp] <= 0x7F)//127
		{
			LCD_DataWrite(ptr[temp]);
			temp++;
		}
		else
		{
 	 		SJIS_HB = ptr[temp];
			temp++;
			SJIS_LB = ptr[temp];
			temp++;

			//========================================
			// Transform SJIS Low Byte
			//========================================
			if(SJIS_LB >= 0x9F)
			{
				JIS_LB = SJIS_LB - 0x9E;//0x7E;
				LSB = 0x01;
			}
			else if(SJIS_LB <= 0x7E) //126
			{
				JIS_LB = SJIS_LB - 0x3f;//0x1F;
			LSB = 0x00;
			}
			else
			{
				JIS_LB = SJIS_LB - 0x40;//0x20;
				LSB = 0x00;
			}

			//========================================
			// Transform SJIS High Byte
			//========================================
			if(SJIS_HB >= 0xE0)
			{
				SJIS_HB = SJIS_HB - 0xC1;	
			}
			else if(SJIS_HB <= 0x9F)
			{
				SJIS_HB = SJIS_HB - 0x81;	
			}

			JIS_HB = (((SJIS_HB << 1) & 0xFE) | LSB) + 0x01;//0x21;

			//========================================
			// Write JIS Code
			//========================================
			//JIS_HB = JIS_HB + 0x80;
			//if(JIS_LB >= 0x60)
				//JIS_LB = JIS_LB + 0x40;
			//else
			 	//JIS_LB = JIS_LB + 0x20;

			LCD_DataWrite(JIS_HB);
			Delay1ms(1);
			LCD_DataWrite(JIS_LB);
			Delay1ms(1);
		}
	}
}
*/




void Get_ALL_XY(void)
{
// uint X_temp,Y_temp;

Serial.println(nr_of_touches);
if (nr_of_touches>0){
   LCD_X1 = coordinates[0 * 2] ;
   LCD_Y1 = coordinates[0 * 2 +1] ;   
    Serial.print("X = ");
 Serial.print(LCD_X1);
 Serial.print("  Y = ");
 Serial.println(LCD_Y1);
}
if (nr_of_touches>1){
   LCD_X2 = coordinates[1 * 2] ;
   LCD_Y2 = coordinates[1 * 2 +1] ;   
}
if (nr_of_touches>2){
   LCD_X3 = coordinates[2 * 2] ;
   LCD_Y3 = coordinates[2 * 2 +1] ;   
}
if (nr_of_touches>3){
   LCD_X4 = coordinates[3 * 2] ;
   LCD_Y4 = coordinates[3 * 2 +1] ;   
}
if (nr_of_touches>4){
   LCD_X5 = coordinates[4 * 2] ;
   LCD_Y5 = coordinates[4 * 2 +1] ;   
}
}
void Clear_ALL_XY_Value(void)
{
LCD_X1 = 0;
LCD_Y1 = 0;
LCD_X2 = 0;
LCD_Y2 = 0;
LCD_X3 = 0;
LCD_Y3 = 0;
LCD_X4 = 0;
LCD_Y4 = 0;
LCD_X5 = 0;
LCD_Y5 = 0;
//touch_counter = 0;
}
