#include <stdio.h>
#include "STM32F7xx.h"

char dato=0;
char conteo=0x30;
int time=1000,j=0;
int bandera=0;

char clear = 0x01;  //0b00000001;
char home = 0x02;   //0b00000010;
char set = 0x3C;    //0b001111XX;  //Bus a 8 bits, LCD 2 lineas, caracter 5x10
char set1 = 0x3C;    //0b001110XX;  //Bus a 8 bits, LCD 2 lineas, caracter 5x10

char disp_on  = 0x0C;  //0b00001100;  //Display ON, cursor OFF, NO parpadeo
char disp_off = 0x08;  //0b00001000;  //Display OFF, cursor OFF, NO parpadeo

char mode_set1 = 0x06;  //0b00000110;  //Incremento del cursor y modo normal
char mode_set2 = 0x04;  //0b00000100;  //Incremento del cursor y desplaza la visual. cada vez que se escribe un dato

char disp_shift  = 0x1C;  //0b00011100;   //desplaza el display -- a la derecha
char disp_shift1 = 0x18;  //0b00011000;   //desplaza el display -- a la izquierda
char disp_shift2 = 0x14;  //0b00010100;   //mueve el cursor -- a la derecha
char disp_shift3 = 0x10;  //0b00010000;   //mueve el cursos -- a la izquierda

char pos_LCD =0;
char w_linea1 = (0x80+ pos_LCD);   //0b1000000 posicion cero primera fila
char w_linea2 = (0xC0 + pos_LCD);  //0b1100000 posicion cero segunda fila
char w_linea3 = (0x94 + pos_LCD);
char w_linea4 = (0xD4 + pos_LCD);

//RS = 0; indica instruccion
//RS = 1; indica dato
//E=0 no se podria utilizar el display y cuando E=1 se podran trnsferir datos y realizar las demas operaciones. 

//Para escribir un dato: RS=1 y E=0, envia dato y luego E=1 (por minimo 450ns)
//Para escribir un comando: RS=0 y E=0, envia dato y luego E=1 (por minimo 450ns)

//**																																																	

void comando_lcd(char b){
	//RS=PB8, Enable=PB9, DATA= PB0(LSB)-PB7(MSB)
	GPIOB->ODR = b;
	GPIOB->ODR &=  ~(1UL <<8);  //RS=0
	GPIOB->ODR |= (1UL << 9);   //Enable = 1
	for( j=0;j<time;j++);        //delay para que el comando sea ejecutado por la LCD
	GPIOB->ODR &=  ~(1UL <<9);  //Enable = 0
	
}

void dato_lcd(char c){ //funcion para la visualizacion
	//RS=PB8, Enable=PB9, DATA= PB0(LSB)-PB7(MSB)
	GPIOB->ODR = c;
	GPIOB->ODR |= (1UL << 8);  //RS=1
	GPIOB->ODR |= (1UL << 9);   //Enable = 1
	for(j=0;j<time;j++);        //delay para que el comando sea ejecutado por la LCD
	GPIOB->ODR &=  ~(1UL <<9);  //Enable = 0

}

char  linterna [8][8] =     { {0x0,0x0,0x3,0x3,0x0,0x1,0x1,0x3}, //Arreglo para la visualizacion del logo de linterna verde
							{0x0,0x0,0x1F,0x1f,0x1C,0x10,0x1,0x3},
							{0x0,0x0,0x1F,0x1F,0x7,0x1,0x10,0x18},
              {0x0,0x0,0x18,0x18,0x0,0x10,0x10,0x18},
							{0x3,0x1,0x1,0x0,0x3,0x3,0x0,0x0},
							{0x3,0x1,0x10,0x1C,0x1F,0x1F,0x0,0x0},
							{0x18,0x10,0x1,0x7,0x1F,0x1F,0x0,0x0},
							{0x18,0x10,0x10,0x0,0x18,0x18,0x0,0x0}
                          };

int main(void)
{
	RCC->AHB1ENR =0x6; //Puerto B y C
	
	//RS=PB8, Enable=PB9, DATA= PB0(LSB)-PB7(MSB)
	GPIOB->MODER =  0x555555;        //Pines del PB0 al PB11 como salida 
	GPIOB->OTYPER = 0;
	GPIOB->OSPEEDR = 0x555555;       //medium speed
	GPIOB->PUPDR =  0x555555;       //pull up

	GPIOC->MODER =  0x0; 
	//**
	
	//Configurar la LCD
	  comando_lcd(clear);
		comando_lcd(home);
		comando_lcd(set);
		comando_lcd(disp_on);
		comando_lcd(mode_set1);
		comando_lcd(w_linea1);
	
	

	comando_lcd(0x40);
for(int i=0;i<8;i++){
for(int l=0;l<8;l++){
	dato_lcd(linterna[i][l]);}
}
				comando_lcd(clear);   //limpia la lcd 
			comando_lcd(0x86);    // direccionamiento de filas y columnas de la lcd
			dato_lcd(0x0);
			comando_lcd(0x87);
			dato_lcd(0x1);
			comando_lcd(0x88);
			dato_lcd(0x2);
			comando_lcd(0x89);
			dato_lcd(0x3);
			comando_lcd(0xC6);
			dato_lcd(0x4);
			comando_lcd(0xC7);
			dato_lcd(0x5);
			comando_lcd(0xC8);
			dato_lcd(0x6);
			comando_lcd(0xC9);
			dato_lcd(0x7);


		/*	for(j=0;j<2000000;j++);
			comando_lcd(clear);
		  comando_lcd(home);*/
			
	while(true){
		
		if(GPIOC->IDR & 0x2000 ){
		dato=conteo;
			conteo++;
		for(j=0;j<100000;j++){}
		}	
		
		
		switch(dato){
			case '0':
			dato_lcd('0');	
			dato=0xF;
				break; 
			
						case '1':
			dato_lcd('1');	
			dato=0xF;
				break; 
						
						case '2':
			dato_lcd('2');	
			dato=0xF;
				break; 
						
						case '3':
			dato_lcd('3');	
			dato=0xF;
				break; 
						
					 case '4':
			dato_lcd('4');	
			dato=0xF;
				break; 
					 
					 case '5':
			dato_lcd('5');	
			dato=0xF;
				break; 
					 
					 case '6':
			dato_lcd('6');	
			dato=0xF;
				break; 	
		
					 case '7':
			dato_lcd('7');	
			dato=0xF;
				break; 
					 case '8':
			dato_lcd('8');	
			dato=0xF;
				break; 
					 case '9':
			dato_lcd('9');	
			dato=0xF;
			comando_lcd(w_linea2);
				break; 
			     case ':':
	       	conteo='0';
					 bandera++;
					 if(bandera>1){
					 comando_lcd(clear);
					 comando_lcd(w_linea1);
					 bandera=0;
					 };
					 dato=0xF;
				break; 
					 default:
			break;
		}
	
	
	}
}