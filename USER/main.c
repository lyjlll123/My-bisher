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
	
	delay_init();	    	 //��ʱ������ʼ��
	Adc_Init();		  		//ADC��ʼ��
	
	PWM_Init_TIM4(0,7199);
	Motor_Init();
	//Load(0,0);

	Follow_Init();
	
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear(); 
	
	OLED_ShowCHinese(0,3,19);	//��
	OLED_ShowCHinese(16,3,20);	//Ʒ
	OLED_ShowCHinese(32,3,21);	//��
	OLED_ShowCHinese(48,3,22);	//��	
	OLED_ShowString(64,3,":     g",16);
	
	OLED_ShowCHinese(0,6,8);	//״
	OLED_ShowCHinese(16,6,9);	//̬
	OLED_ShowString(32,6,":",16);
	//printf("11111\n");
	
	while(1) 
	{	
//		count++;
//		if(count > 1000)
//		{
//			count = 0;
//			printf("�ȴ�ʶ����....\n");
//			printf("��Ʒ������%.1fg\n",Weight);
//			printf("״̬��\n");
//		}
		adcx = Get_Adc_Average(ADC_Channel_1,10);
		Weight = (float)adcx*(3.3/4096);
		//printf("%.1f\n",Weight);
		Weight_1 = (u8)Weight;
		Weight_2 = (u16)(Weight*10)%10;
		OLED_ShowNum(80,3,Weight_1,2,16);
		OLED_ShowChar(96,3,'.',16);
		OLED_ShowNum(104,3,Weight_2,1,16);
		
		if(Result^Result_Old)	//�б仯
		{
			Result_Old = Result;
			switch(Result)
			{
				case 10:
				{
					OLED_ShowCHinese(0,0,0);	//��
					OLED_ShowCHinese(16,0,1);	//��
					OLED_ShowCHinese(32,0,2);	//ʶ
					OLED_ShowCHinese(48,0,3);	//��
					OLED_ShowCHinese(64,0,4);	//��
					OLED_ShowCHinese(80,0,5);	//��
					OLED_ShowString(96,0,"....",16);
					
					printf("�ȴ�ʶ����....\n");
					//printf("��Ʒ������%.1f\n",Weight);
				} break;
				default:
				{
					OLED_ShowCHinese(0,0,2);	//ʶ
					OLED_ShowCHinese(16,0,3);	//��
					OLED_ShowCHinese(32,0,4);	//��
					OLED_ShowCHinese(48,0,5);	//��
					OLED_ShowString(64,0,": ",16);
					OLED_ShowNum(80,0,Result,1,16);
					OLED_ShowString(88,0,"     ",16);
					
					printf("ʶ������%d\n",Result);
					//printf("��Ʒ������%.1fg\n",Weight);
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
					OLED_ShowCHinese(48,6,10);	//ͣ
					OLED_ShowCHinese(64,6,11);	//ֹ
					OLED_ShowCHinese(80,6,8);	//״
					OLED_ShowCHinese(96,6,9);	//̬
					OLED_ShowString(112,6,"..",16);
					printf("״̬��ֹͣ��\n");
				}break;
				
				case 1: 
				{
					OLED_ShowCHinese(48,6,12);	//��
					OLED_ShowCHinese(64,6,13);	//��
					OLED_ShowCHinese(80,6,14);	//��
					OLED_ShowString(96,6,"...",16);
					printf("״̬��������...\n");
				}break;
				
				case 2: 
				{
					OLED_ShowCHinese(48,6,0);	//��
					OLED_ShowCHinese(64,6,1);	//��
					OLED_ShowCHinese(80,6,15);	//ж
					OLED_ShowCHinese(96,6,16);	//��
					OLED_ShowString(112,6,"..",16);
					printf("״̬���ȴ�ж��...\n");
				}break;
				
				case 3: 
				{
					OLED_ShowCHinese(48,6,17);	//��
					OLED_ShowCHinese(64,6,18);	//��
					OLED_ShowCHinese(80,6,14);	//��
					OLED_ShowString(96,6,"...",16);
					printf("״̬��������...\n");
				}break;
			}
		}
		
		
	}
}

