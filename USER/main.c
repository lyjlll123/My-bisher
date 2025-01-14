#include "stm32f10x.h"
#include "sys.h" 
#include "oled.h"
#include "BMP.h"

extern u8 Result;
u8 Result_Old = 1;

u8 State = 0;
u8 State_Old = 1;

float Weight;
u8 Weight_1,Weight_2;

u16 count = 0;

u16 adcx;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Bluetooth_Init(115200);
	Openmv_Usart_Init(115200);
	//uart_init(115200);
	
	delay_init();	    	 //延时函数初始化
	Adc_Init();		  		//ADC初始化
	
	PWM_Init_TIM4(0,7199);
	Motor_Init();
	//Load(0,0);

	Follow_Init();
	
	OLED_Init();			//初始化OLED  
	OLED_Clear(); 
	
	OLED_ShowCHinese(0,3,19);	//物
	OLED_ShowCHinese(16,3,20);	//品
	OLED_ShowCHinese(32,3,21);	//重
	OLED_ShowCHinese(48,3,22);	//量	
	OLED_ShowString(64,3,":     g",16);
	
	OLED_ShowCHinese(0,6,8);	//状
	OLED_ShowCHinese(16,6,9);	//态
	OLED_ShowString(32,6,":",16);
	//printf("11111\n");
	
	while(1) 
	{	
//		count++;
//		if(count > 1000)
//		{
//			count = 0;
//			printf("等待识别结果....\n");
//			printf("物品重量：%.1fg\n",Weight);
//			printf("状态：\n");
//		}
		adcx = Get_Adc_Average(ADC_Channel_1,10);
		Weight = (float)adcx*(3.3/4096);
		//printf("%.1f\n",Weight);
		Weight_1 = (u8)Weight;
		Weight_2 = (u16)(Weight*10)%10;
		OLED_ShowNum(80,3,Weight_1,2,16);
		OLED_ShowChar(96,3,'.',16);
		OLED_ShowNum(104,3,Weight_2,1,16);
		
		if(Result^Result_Old)	//有变化
		{
			Result_Old = Result;
			switch(Result)
			{
				case 10:
				{
					OLED_ShowCHinese(0,0,0);	//等
					OLED_ShowCHinese(16,0,1);	//待
					OLED_ShowCHinese(32,0,2);	//识
					OLED_ShowCHinese(48,0,3);	//别
					OLED_ShowCHinese(64,0,4);	//结
					OLED_ShowCHinese(80,0,5);	//果
					OLED_ShowString(96,0,"....",16);
					
					printf("等待识别结果....\n");
					//printf("物品重量：%.1f\n",Weight);
				} break;
				default:
				{
					OLED_ShowCHinese(0,0,2);	//识
					OLED_ShowCHinese(16,0,3);	//别
					OLED_ShowCHinese(32,0,4);	//结
					OLED_ShowCHinese(48,0,5);	//果
					OLED_ShowString(64,0,": ",16);
					OLED_ShowNum(80,0,Result,1,16);
					OLED_ShowString(88,0,"     ",16);
					
					printf("识别结果：%d\n",Result);
					//printf("物品重量：%.1fg\n",Weight);
				}break;
			}
		}
		
		if(State^State_Old)
		{
			State_Old = State;
			switch(State)
			{
				case 0: 
				{
					OLED_ShowCHinese(48,6,10);	//停
					OLED_ShowCHinese(64,6,11);	//止
					OLED_ShowCHinese(80,6,8);	//状
					OLED_ShowCHinese(96,6,9);	//态
					OLED_ShowString(112,6,"..",16);
					printf("状态：停止！\n");
				}break;
				
				case 1: 
				{
					OLED_ShowCHinese(48,6,12);	//运
					OLED_ShowCHinese(64,6,13);	//输
					OLED_ShowCHinese(80,6,14);	//中
					OLED_ShowString(96,6,"...",16);
					printf("状态：运输中...\n");
				}break;
				
				case 2: 
				{
					OLED_ShowCHinese(48,6,0);	//等
					OLED_ShowCHinese(64,6,1);	//待
					OLED_ShowCHinese(80,6,15);	//卸
					OLED_ShowCHinese(96,6,16);	//货
					OLED_ShowString(112,6,"..",16);
					printf("状态：等待卸货...\n");
				}break;
				
				case 3: 
				{
					OLED_ShowCHinese(48,6,17);	//返
					OLED_ShowCHinese(64,6,18);	//程
					OLED_ShowCHinese(80,6,14);	//中
					OLED_ShowString(96,6,"...",16);
					printf("状态：返程中...\n");
				}break;
			}
		}
		
		
	}
}

