sbit LCD_RS at GPIOB_ODR.B1;
sbit LCD_EN at GPIOB_ODR.B0;
sbit LCD_D4 at GPIOB_ODR.B12;
sbit LCD_D5 at GPIOB_ODR.B13;
sbit LCD_D6 at GPIOB_ODR.B14;
sbit LCD_D7 at GPIOB_ODR.B15;

unsigned int ratio,ratio1;
float PV=0.0f;
float dutyP=0;
int dist=1;// LiDAR actually measured distance value
int strength;// LiDAR signal strength
int check;// check numerical value storage
int i,z;
int uart[9];// store data measured by LiDAR
const int HEADER=0x59;// data package frame header
char str[50];
unsigned int measured[181]  ;
int  max =0;
int deg,deg2;
int avgdeg1=0,avgdeg2=0,avgdeg3=0;
int degSave;
int maxdeg=0;
int dataReady=0;
double D1,D2,D3,D4,D5;
int j=0;
double tempmax;
float Per1,Per2,Per3,Per4,Per5;
int digital_value;
int period;
int period1;
float Digital[5],percentage[5];
int PercentageCount=0;
int RightArr[60], StraightArr[60], LeftArr[60] , decision;
int RorD;
double Digital_InfraRed;
int flag=0;
int SendTounity =0;


void InitTimer2()
  {
  RCC_APB1ENR.TIM2EN = 1;
  TIM2_CR1.CEN = 0;
  TIM2_PSC = 1;
  TIM2_ARR = 39999;
  NVIC_IntEnable(IVT_INT_TIM2);
  TIM2_DIER.UIE = 1;
  TIM2_CR1.CEN = 1;


  }
  int InfraRed()
 {
 int v,d;
   Digital_InfraRed=ADC2_Get_Sample(8);
        v=(Digital_InfraRed/819.2)*0.68;
       d=pow(v,-1.1)*65;
       //PWM_TIM1_Start(_PWM_CHANNEL1, &_GPIO_MODULE_TIM1_CH1_PA8);
       return d;

 }
int Lidar()
     {
     while(dataReady == 0)
     {
     if(UART3_Data_Ready())
     {
            if(UART3_Read()==HEADER)
       {
               uart[0]=HEADER;
             if(UART3_Read()==HEADER)//determine data package frame header 0x59
             {

                               uart[1]=HEADER;
                               for( i=2;i<9;i++)// store data to array
                                   {
                                     uart[i]=UART3_Read();
                                   }
               check=uart[0]+uart[1]+uart[2]+uart[3]+uart[4]+uart[5]+uart[6]+uart[7];
               if(uart[8]==(check&0xff))// check the received data as per protocols
               {
                 dist=uart[2]+uart[3]*256;// calculate distance value
                 strength=uart[4]+uart[5]*256;
                 dataReady=1;
                }
             }
       }
     }
     }
     return  dist;
     }
  float convertDuty( float inp )
       {
        float fValue = inp;//1500.0f;
        float tf = (fValue/20000.0)*53332.0f;
        return tf;
       }
float ConvertFromDegree( int degree )
       {
        return ConvertDuty(   1880.0* degree / 180.0 +480);
       }

       float ConvertFromDegree_pump( int degree )
       {
        return ConvertDuty(   1970.0* degree / 180.0 +480);
       }
 void InitTimer1()
 {
    ratio= PWM_TIM3_Init(50);  // Set frequency to 50Hz or period to 20ms
    PWM_TIM3_Start(_PWM_CHANNEL2, &_GPIO_MODULE_TIM3_CH2_PA7);
    PWM_TIM3_Set_Duty(ratio/20.0f, _PWM_NON_INVERTED, _PWM_CHANNEL2);  // Set duty cycle to one-half of period
    ratio1= PWM_TIM1_Init(50);  // Set frequency to 50Hz or period to 20ms
    PWM_TIM1_Start(_PWM_CHANNEL1, &_GPIO_MODULE_TIM1_CH1_PA8);
    PWM_TIM1_Set_Duty(ratio1/20.0f, _PWM_NON_INVERTED, _PWM_CHANNEL1);  // Set duty cycle to one-half of period

   }
   void GetValueFlameSensor ()
{
   D5=ADC2_Get_Sample(0);
Per5=(D5/4096)*100;

D4=ADC2_Get_Sample(1);
Per4=(D4/4096)*100;

D3=ADC2_Get_Sample(2);
Per3=(D3/4096)*100;

D2=ADC2_Get_Sample(3);
Per2=(D2/4096)*100;

D1=ADC2_Get_Sample(6);
Per1=(D1/4096)*100;

   Digital[0]=D1;
   Digital[1]=D2;
   Digital[2]=D3;
   Digital[3]=D4;
   Digital[4]=D5;

   percentage[0]=Per1;
   percentage[1]=Per2;
   percentage[2]=Per3;
   percentage[3]=Per4;
   percentage[4]=Per5;
  // find maximum number to identify who is the higher reading of heat senesor

   GPIOB_ODR.B12=0;
      GPIOB_ODR.B13=1;
      GPIOB_ODR.B14=0;
      GPIOB_ODR.B15=1;   //IN1
      PWM_TIM4_Set_Duty(0.61f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL3);
      PWM_TIM4_Set_Duty(0.6f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL4);

    for( j=1; j<=4; j++)
    {
       if(tempmax<digital[j])
         tempmax=digital[j];
    }
    percentageCount=0;
    for( j=0; j<=4; j++)
    {
       if(percentage[j]>= 45)
        PercentageCount++;

    }

}
  void pump()
  {
  int p;
  float digital_value;
  if(SendToUnity != 8)
  {
     UART1_Write_text("8");
     SendTounity=8;
     delay_ms(5);


  }
   PWM_TIM4_Set_Duty(0.0f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL3);
     PWM_TIM4_Set_Duty(0.0f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL4);
     while (PercentageCount >0)
     {
     PWM_TIM4_Set_Duty(0.0f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL3);
     PWM_TIM4_Set_Duty(0.0f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL4);
     for(p=45;p<=135;p++)
      {
       PWM_TIM1_Set_Duty(ConvertFromDegree_pump((float)p), _PWM_NON_INVERTED, _PWM_CHANNEL1);
        InitTimer2();
        //delay_ms(16);
        GPIOB_ODR.B6=1;
        digital_value=ADC2_Get_Sample(4);
        if(digital_value<3000)
        {
           GPIOC_ODR.B14=1;
           if(SendToUnity != 9)
  {
     UART1_Write_text("9");
     SendTounity=9;
     delay_ms(5);


  }
        }
        else
        {
          GPIOC_ODR.B14=0;
        }
      }
       for(p=135;p>=45;p--)
      {
       PWM_TIM1_Set_Duty(ConvertFromDegree_pump((float)p), _PWM_NON_INVERTED, _PWM_CHANNEL1);
        InitTimer2();
        //delay_ms(16);
        GPIOB_ODR.B6=1;
        digital_value=ADC2_Get_Sample(4);
        if(digital_value<3000)
        {
           GPIOC_ODR.B14=1;
           if(SendToUnity != 9)
  {
     UART1_Write_text("9");
     SendTounity=9;
     delay_ms(5);

  }
        }
        else
        {
          GPIOC_ODR.B14=0;
        }
      }

      GetValueFlameSensor ();
      }
        GPIOB_ODR.B6=0;

  }

int LCS(int arr[], int max ,int avg)
{
    int Rc=0,Sc=0,Lc=0,RcMax=0, ScMax = 0, LcMax = 0;
   for ( i = 0; i < 60; i++)
    {

        RightArr[i] = arr[i];
        StraightArr[i] = arr[i + 60];
        LeftArr[i] = arr[i + 120];
    }
    for ( i = 0; i < 60; i++)
    {
        if (RightArr[i] <= max * 1.025 && RightArr[i] >= max * 0.975)
        {
            Rc++;
        }
        else
        {
            if(Rc>RcMax)
            RcMax = Rc;
            Rc = 0;
        }

        if (StraightArr[i] <= max * 1.025 && StraightArr[i] >= max * 0.975)
        {
            Sc++;
        }
        else
        {
            if (Sc > ScMax)
            ScMax = Sc;
            Sc = 0;
        }
        if (LeftArr[i] <= max * 1.025 && LeftArr[i] >= max * 0.975)
        {
            Lc++;
        }
        else
        {
            if (Lc > LcMax)
            LcMax = Lc;
            Lc = 0;
        }
        if (Rc > RcMax)
            RcMax = Rc;
        if (Sc > ScMax)
            ScMax = Sc;
        if (Lc > LcMax)
            LcMax = Lc;

    }
    if(avg ==0)   {
    if (RcMax > ScMax && RcMax > LcMax)
        return 1;
    else if (ScMax > RcMax && ScMax > LcMax)
        return 2;
    else if (LcMax > RcMax && LcMax > ScMax)
        return 3;
    else
        return 2;
        }
        else if (avg == 1)
        {
           if (RcMax > LcMax)
        return 1;
    else if ( LcMax > LcMax)
        return 2;
        }

}
void fire()
{
     GetValueFlameSensor();


     PWM_TIM4_Set_Duty(0.61f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL3);
     PWM_TIM4_Set_Duty(0.6f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL4);

    if(PercentageCount>=3)
    {
    PWM_TIM4_Set_Duty(0.0f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL3);
     PWM_TIM4_Set_Duty(0.0f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL4);
     pump();
    }
    else
    {

  // find maximum number to identify who is the higher reading of heat senesor



    for( j=1; j<=4; j++)
    {
       if(tempmax<digital[j])
         tempmax=digital[j];
    }
    if(tempmax==D1)    //A6
    {
      //  UART1_Write_Text("left \n\r");

      GPIOB_ODR.B12=0;
      GPIOB_ODR.B13=1;
      GPIOB_ODR.B14=1;
      GPIOB_ODR.B15=0;   //IN1
      if(SendToUnity != 4)
  {
     UART1_Write_text("4");
     SendTounity=4;
     delay_ms(5);


  }
      delay_ms(400);
    }
    else if (tempmax==D2)        //A3
    {
       //UART1_Write_Text("left diagonal \n\r");

      GPIOB_ODR.B12=0;
      GPIOB_ODR.B13=1;
      GPIOB_ODR.B14=1;
      GPIOB_ODR.B15=0;
      if(SendToUnity != 5)
  {
     UART1_Write_text("5");
     SendTounity=5;
     delay_ms(5);


  }   //IN1
      delay_ms(250);
    }
    else if (tempmax==D3)         //A2
    {
        //UART1_Write_Text("straight \n\r");

      GPIOB_ODR.B12=0;
      GPIOB_ODR.B13=1;
      GPIOB_ODR.B14=0;
      GPIOB_ODR.B15=1;
      if(SendToUnity != 1)
  {

     UART1_Write_text("1");
     SendTounity=1;
     delay_ms(5);


  }   //IN1

    }
    else if (tempmax==D4)        //A1
    {
      // UART1_Write_Text("right diagonal \n\r");

       GPIOB_ODR.B12=1;
       GPIOB_ODR.B13=0;
       GPIOB_ODR.B14=0;
       GPIOB_ODR.B15=1;
        if(SendToUnity != 3)
  {
     UART1_Write_text("3");
     SendTounity=3;
     delay_ms(5);


  }   //IN1
       delay_ms(250);
    }
    else if (tempmax==D5)         //A0
    {
      //UART1_Write_Text("right \n\r");

       GPIOB_ODR.B12=1;
       GPIOB_ODR.B13=0;
       GPIOB_ODR.B14=0;
       GPIOB_ODR.B15=1;
       if(SendToUnity != 2)
  {
     UART1_Write_text("2");
     SendTounity=2;
    delay_ms(5);


  }   //IN1
       delay_ms(400);
    }
    else
     {
      //UART1_Write_Text("straight \n\r");
      if(SendToUnity != 1)
  {

     UART1_Write_text("1");
     SendTounity=1;
     delay_ms(5);


  }
      GPIOB_ODR.B12=0;
      GPIOB_ODR.B13=1;
      GPIOB_ODR.B14=0;
      GPIOB_ODR.B15=1;   //IN1

    }

    }

}
void findmax()
{

       avgdeg3=avgdeg3/61;
    if(decision == 1)
    {

     PWM_TIM4_Set_Duty(0.61f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL3);
     PWM_TIM4_Set_Duty(0.6f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL4);
      // UART1_Write_Text("right \n\r");
       GPIOB_ODR.B12=1;
       GPIOB_ODR.B13=0;
       GPIOB_ODR.B14=0;
       GPIOB_ODR.B15=1; //IN1
       if(SendToUnity != 6)
  {
     UART1_Write_text("6");
     SendTounity=6;
     delay_ms(5);


  }
       delay_ms(300);
       avgdeg1=0;
    }

    else  if (decision == 3)
     {

      PWM_TIM4_Set_Duty(0.61f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL3);
     PWM_TIM4_Set_Duty(0.6f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL4);
     // UART1_Write_Text("left \n\r");
      GPIOB_ODR.B12=0;
      GPIOB_ODR.B13=1;
      GPIOB_ODR.B14=1;
      GPIOB_ODR.B15=0;//IN1
      if(SendToUnity != 7)
  {
     UART1_Write_text("7");
     SendTounity=7;
     delay_ms(5);


  }
      delay_ms(300);
      avgdeg2=0;
    }
    else
    {
       //UART1_Write_Text("straight \n\r");
       for(i = 60 ; i<120 ; i++)
       {
       if(measured[i]<=50)
       {
       RorD=-1;
       break;
       }
       else
       {
         RorD=1;
       }
       }
       if(RorD==-1)
       {

        int d = LCS(measured,max,1);
         if(d==0){

  }
        PWM_TIM4_Set_Duty(0.61f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL3);
       PWM_TIM4_Set_Duty(0.6f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL4);
        GPIOB_ODR.B12=1;
       GPIOB_ODR.B13=0;
       GPIOB_ODR.B14=0;
       GPIOB_ODR.B15=1;
                      if(SendToUnity != 6)
  {
     UART1_Write_text("6");
     SendTounity=6;
     delay_ms(5);


       delay_ms(300);
       }
          else
          {

            PWM_TIM4_Set_Duty(0.61f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL3);
       PWM_TIM4_Set_Duty(0.6f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL4);
        GPIOB_ODR.B12=0;
       GPIOB_ODR.B13=1;
       GPIOB_ODR.B14=1;
       GPIOB_ODR.B15=0;
       if(SendToUnity != 7)
  {

     UART1_Write_text("7");
     SendTounity=7;
     delay_ms(5);


  }
       delay_ms(300);
          }
        }
       else if(RorD ==1)

       {

       fire();
       }

    }




    }
void servo_servo()
{

      delay_ms(100);
      for(deg = 0; deg <=180; deg++)
      {
       if(SendToUnity != 10)
  {
     UART1_Write_text("10");
     SendTounity=10;
     delay_ms(5);

  }

       PWM_TIM3_Set_Duty(ConvertFromDegree((float)deg), _PWM_NON_INVERTED, _PWM_CHANNEL2);
        InitTimer2();
       measured[deg] = Lidar();
       if(deg>=60 && deg<=120)
       avgdeg3=measured[deg];
      dataReady=0;
      if(measured[deg] > max  )
      {
            max=measured[deg];
            maxdeg=deg;
            }


              }

            decision = LCS(measured,max,0);
            findmax();
            InitTimer2();
            max=0;

       }

 void NormalScan()
{
       PWM_TIM4_Set_Duty(0.61f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL3);
       PWM_TIM4_Set_Duty(0.6f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL4);

      for(deg2 = 60; deg2 <=120; deg2++)
      { //deg2=90;
       PWM_TIM3_Set_Duty(ConvertFromDegree((float)deg2), _PWM_NON_INVERTED, _PWM_CHANNEL2);
        InitTimer2();
        dataReady=0;
       measured[deg2] = Lidar();

            if (measured[deg2] <=50)
            {
            PWM_TIM4_Set_Duty(0.0f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL3);
            PWM_TIM4_Set_Duty(0.0f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL4);
            servo_servo();

            }
            else
            {
            GPIOB_ODR.B12=0;
            GPIOB_ODR.B13=1;
            GPIOB_ODR.B14=0;
            GPIOB_ODR.B15=1;   //IN1
            PWM_TIM4_Set_Duty(0.61f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL3);
            PWM_TIM4_Set_Duty(0.6f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL4);

            fire();
            }



              }

            InitTimer2();;
      for(deg2 = 120; deg2 >=60; deg2--)
      { //deg2=90;
       PWM_TIM3_Set_Duty(ConvertFromDegree((float)deg2), _PWM_NON_INVERTED, _PWM_CHANNEL2);
       InitTimer2();
       dataReady=0;
       measured[deg] = Lidar();

            if (measured[deg2] <=50)
            {
            PWM_TIM4_Set_Duty(0.0f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL3);
            PWM_TIM4_Set_Duty(0.0f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL4);
            servo_servo();

            }
            else
            {
            GPIOB_ODR.B12=0;
            GPIOB_ODR.B13=1;
            GPIOB_ODR.B14=0;
            GPIOB_ODR.B15=1;   //IN1
            PWM_TIM4_Set_Duty(0.61f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL3);
            PWM_TIM4_Set_Duty(0.6f*period1, _PWM_NON_INVERTED, _PWM_CHANNEL4);

            fire();
            }



               }
            InitTimer2();
}

void Timer2_interrupt() iv IVT_INT_TIM2
{
  TIM2_SR.UIF = 0;
}

void main()
 {
  GPIO_Config(&GPIOA_BASE,_GPIO_PINMASK_0,_GPIO_CFG_ANALOG_INPUT);    //flame
 GPIO_Config(&GPIOA_BASE,_GPIO_PINMASK_1,_GPIO_CFG_ANALOG_INPUT);    //flame
 GPIO_Config(&GPIOA_BASE,_GPIO_PINMASK_2,_GPIO_CFG_ANALOG_INPUT);    //flame
 GPIO_Config(&GPIOA_BASE,_GPIO_PINMASK_3,_GPIO_CFG_ANALOG_INPUT);    //flame
 GPIO_Config(&GPIOB_BASE,_GPIO_PINMASK_0,_GPIO_CFG_ANALOG_INPUT);     //Infrared
 GPIO_Config(&GPIOA_BASE,_GPIO_PINMASK_6,_GPIO_CFG_ANALOG_INPUT);    //flame
 GPIO_Config(&GPIOB_BASE,_GPIO_PINMASK_8, _GPIO_CFG_MODE_ALT_FUNCTION | _GPIO_CFG_SPEED_50MHZ); //pwm  car motor
 GPIO_Config(&GPIOB_BASE,_GPIO_PINMASK_9, _GPIO_CFG_MODE_ALT_FUNCTION | _GPIO_CFG_SPEED_50MHZ); //pwm  car motor
 GPIO_Config(&GPIOA_BASE,_GPIO_PINMASK_7, _GPIO_CFG_MODE_ALT_FUNCTION | _GPIO_CFG_SPEED_MAX);    //pwm servo lidar
 GPIO_Config(&GPIOA_BASE,_GPIO_PINMASK_8, _GPIO_CFG_MODE_ALT_FUNCTION | _GPIO_CFG_SPEED_MAX);    //pwm servo pump
 GPIO_Config(&GPIOB_BASE,_GPIO_PINMASK_12,_GPIO_CFG_DIGITAL_OUTPUT); //control side 1
 GPIO_Config(&GPIOB_BASE,_GPIO_PINMASK_15,_GPIO_CFG_DIGITAL_OUTPUT); //control side 1
 GPIO_Config(&GPIOB_BASE,_GPIO_PINMASK_13,_GPIO_CFG_DIGITAL_OUTPUT); //control side 2
 GPIO_Config(&GPIOB_BASE,_GPIO_PINMASK_14,_GPIO_CFG_DIGITAL_OUTPUT); //control side2
  GPIO_Config(&GPIOB_BASE,_GPIO_PINMASK_6,_GPIO_CFG_DIGITAL_OUTPUT);  // pump
 GPIO_Config(&GPIOA_BASE,_GPIO_PINMASK_4,_GPIO_CFG_ANALOG_INPUT);       //water_level
 GPIO_Config(&GPIOC_BASE,_GPIO_PINMASK_14,_GPIO_CFG_DIGITAL_OUTPUT);    //lamp_water_level
// lcd_init();
 UART3_Init(115200);
 UART1_Init(9600);
 TIM1EN_bit = 1;
 TIM2EN_bit = 1;
 TIM3EN_bit = 1;
 TIM4EN_bit = 1;
// period = PWM_TIM1_Init(500);  //Init freq for Timer 1 PWM to 500
 period1= PWM_TIM4_Init(500);
 GPIOB_ODR.B12=0;
 GPIOB_ODR.B13=1;
 GPIOB_ODR.B14=0;
 GPIOB_ODR.B15=1;   //IN1
 InitTimer1();
  ADC2_Init();                  //added start
  ADC_Set_Input_Channel(0);
  ADC_Set_Input_Channel(1);
  ADC_Set_Input_Channel(2);
  ADC_Set_Input_Channel(3);
  ADC_Set_Input_Channel(6);
  ADC_Set_Input_Channel(8);
  PWM_TIM4_Start(_PWM_CHANNEL3, &_GPIO_MODULE_TIM4_CH3_PB8);
  PWM_TIM4_Start(_PWM_CHANNEL4, &_GPIO_MODULE_TIM4_CH4_PB9);

  PWM_TIM4_Set_Duty(0, _PWM_NON_INVERTED, _PWM_CHANNEL3);
  PWM_TIM4_Set_Duty(0, _PWM_NON_INVERTED, _PWM_CHANNEL4);



  while(1)
  {
            NormalScan();



    }
    }