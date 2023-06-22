#define F_CPU 8000000UL			// Define CPU Frequency e.g. here 8MHz
#include <avr/io.h>				// Include AVR std. library file
#include <util/delay.h>			// Include inbuilt defined Delay header file
#include <avr/interrupt.h>		// Include the interrupt library

#define LCD_Data_Dir DDRC		// Define LCD data port direction
#define LCD_Data_Port PORTC		// Define LCD data port

#define Signal_Dir DDRA			// Define signal port direction register
#define Signal_Port PORTA		// Define signal port

#define Actuator_Dir DDRD		// Define actuator port direction register
#define Actuator_Port PORTD		// Define actuator port

#define RS PA4					// Define Register Select (data/command reg.)pin
#define EN PA5					// Define Enable signal pin

#define ACtuator PD6			// Define actuator pin (pin output to control fan

#define MODE1 PA0				// Define pin of LED MODE1
#define MODE2 PA1				// Define pin of LED MODE2
#define MODE3 PA2				// Define pin of LED MODE3

#define ADC_VREF 0x40			//Voltage of AVCC to reference, ADLAR=0
//-----------------------------------------------------------

void LCD_Command(unsigned char cmnd)
{
	LCD_Data_Port= cmnd;
	Signal_Port &= ~(1<<RS);	// RS=0 command reg
	Signal_Port |= (1<<EN);		// Enable pulse
	_delay_us(1);
	Signal_Port &= ~(1<<EN);
	_delay_ms(3);
}
//
void LCD_Init (void)			// LCD Initialize function
{
	_delay_ms(20);				// LCD Power ON delay always >15ms
	
	LCD_Command (0x38);			// Initialization of 16X2 LCD in 8bit mode
	LCD_Command (0x0C);			// Display ON Cursor OFF
	LCD_Command (0x06);			// Auto Increment cursor
	LCD_Command (0x01);			// Clear display
	LCD_Command (0x80);			// Cursor at home position
}
//
void init(void)
{
	Signal_Dir = 0xF7;			// Set pin of signal port input, output
	LCD_Data_Dir = 0xFF;		// Make LCD data port direction as o/p
	Actuator_Dir = 0xF3;		// Set pin of Actuator port input, output
	DDRB = 0xFB;
	LCD_Init();					// Call LCD initialize function
	GICR = 1<<INT0 | 1<<INT1 | 1<<INT2;	// Turn interrupt 0, 1, 2 on 
	MCUCR = 1<<ISC11 | 1<<ISC10 | 1<<ISC01 | 1<<ISC00 ; // Choose interrupt mode of INT0,1; Mode of INT0,1 are rising edge
	MCUCSR = 1<<ISC2 ;			// Choose interrupt mode of INT2; Mode of INT2 is rising edge
	ADMUX=ADC_VREF;				// Set reference voltage
	ADCSRA=0x87;				// Fosc/128
}
//
void LCD_Char (unsigned char char_data)	// LCD data write function
{
	LCD_Data_Port= char_data;
	Signal_Port |= (1<<RS);
	Signal_Port |= (1<<EN);
	_delay_us(1);
	Signal_Port &= ~(1<<EN);
	_delay_ms(1);
}
//
void LCD_String (const char *str)		// Send string to LCD function
{
	int i;
	for(i=0;str[i]!=0;i++)		// Send each char of string till the NULL
	{
		LCD_Char (str[i]);
	}
}
//
void LCD_Clear()
{
	LCD_Command (0x01);		// Clear display
	LCD_Command (0x80);		// Cursor at home position
}
//
void LCD_number (const char str)		// Send number to LCD function
{
	unsigned char temp;
	temp = str/100 + 48;
	LCD_Char(temp);
	temp = (str%100/10)+48;
	LCD_Char(temp);
	temp = (str%10)+48;
	LCD_Char(temp);
}
//
void LCD_SHL(unsigned char shl) // LCD shift right cursor function
{
	for (int i = 0;i<shl;i++)
	{
		LCD_Command(0x10);
	}
}
//
void LCD_SHR(unsigned char shl) // LCD shift right cursor function
{
	for (int i = 0;i<shl;i++)
	{
		LCD_Command(0x14);
	}
}
//
//
unsigned char read_adc(unsigned char adc_input){
	ADMUX=adc_input | ADC_VREF; // Choose ADC pin and voltage reference
	ADCSRA|=0x40;				// Start ADC
	while (!(ADCSRA & 0x10));	// Check ADC
	ADCSRA|=0x10;
	return ADCW;				// ADC 10 bit
}
//

unsigned char MODE, temperature, timer, status, adctemp; //Declare variable
int dem;
//
ISR(INT0_vect) // Function of INT0
{
	_delay_ms(100);
	status = 2;
	if (MODE != 1)
	{
		if (MODE == 2)
		{
			if (temperature<256)
			{
				temperature++;
			} else
			{
				temperature = 255;
			}
		} else
		{
			if (timer <240)
			{
				timer+=10;
			} else
			{
				timer = 240;
			}
		}
	}
}
//
ISR(INT1_vect) // Function of INT1
{
	_delay_ms(100);
	status = 2;
	if (MODE != 1)
	{
		if (MODE == 2)
		{
			if (temperature>0)
			{
				temperature--;
			} else
			{
				temperature = 0;
			}
		} else
		{
			if (timer > 10)
			{
				timer-=10;
			} else
			{
				timer = 0;
			}
		}
	}
}
//
ISR(INT2_vect) // Function of INT2
{
	_delay_ms(100);
	status = 1;
	if (MODE == 3)
	{
		MODE = 1;
	} else
	{
		MODE++;
	}	
}
//Main function
int main()
{
	init(); //Initialize
	sei();
	//Introduction
	_delay_ms(500);
	LCD_String("LE TRONG HOANG");
	LCD_Command(0xC0);
	LCD_String("SENIOR PROJECT 1");
	_delay_ms(2000);
	LCD_Clear();
	//Set initial value
	MODE = 1;
	temperature = 25;
	timer = 0;
	//Function
	while(1)
	{
		switch (MODE)
		{
			case 1: //Function of MODE1
			LCD_Clear();
			Signal_Port |= (1<<MODE1);
			Signal_Port &= ~(1<<MODE2);
			Signal_Port &= ~(1<<MODE3);
			LCD_String("MODE1");
			Actuator_Port |= (1<<ACtuator);
			while(1)
			{
				if (status == 1)
				{
					status =0;
					break;
				}
			}
			break;
			case 2: //Function of MODE2
			LCD_Clear();
			Signal_Port &= ~(1<<MODE1);
			Signal_Port |= (1<<MODE2);
			Signal_Port &= ~(1<<MODE3);
			LCD_String("MODE2: Set: ");
			LCD_number(temperature);
			LCD_Command(0xC0);
			LCD_String("Environment :");
			LCD_number(read_adc(3)/2);
			while (1)
			{
				_delay_ms(500); //sample frequency : 2Hz
				LCD_SHL(3);
				do 
				{
					adctemp =read_adc(3)/2;
				} while (adctemp==0);
				LCD_number(adctemp);
				if (adctemp>temperature)
					Actuator_Port |= (1<<ACtuator);
				else
				Actuator_Port &= ~(1<<ACtuator);				
				if (status == 2)
				{
					status = 0;
					LCD_Command(0x80);
					LCD_SHR(12);
					LCD_number(temperature);
					LCD_Command(0xC0);
					LCD_SHR(16);
				}
				if (status == 1)
				{
					status = 0;
					break;
				}
			}
			break;
			case 3: //Function of MODE3
			LCD_Clear();
			Signal_Port &= ~(1<<MODE1);
			Signal_Port &= ~(1<<MODE2);
			Signal_Port |= (1<<MODE3);
			LCD_String("MODE3: TIMER ");
			LCD_Command(0xC0);
			LCD_String("Remaining : ");
			LCD_number(timer);
			LCD_String("m");
			while(1)
			{
				if (timer>0)
				{
					Actuator_Port |= (1<<ACtuator);
				}
				else
				{
					Actuator_Port &= ~(1<<ACtuator);
				}
				for(dem=0;dem<600;dem++)// delay 1 minute
				{
					_delay_ms(100);
					if ((status==1)||(status==2))
					break;
				}
				if (!((status==1)||(status==2)))
				{
					if (timer>0)
					timer--;
				} else
				{
					if (status ==1)
					{	
						status = 0;
						break;
					} else
					{
						status = 0;
					}
				}
				LCD_SHL(4);
				LCD_number(timer);
				LCD_SHR(1);
			}
			break;
		}
	}
	return 0;
}
