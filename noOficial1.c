//Bits de configuracion o directivas de preprocesador
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC2_PLL3// System Clock Postscaler Selection bits ([Primary Oscillator Src: /2][96 MHz PLL Src: /3])
#pragma config FOSC = XTPLL_XT  // Oscillator Selection bits (XT oscillator, PLL enabled (XTPLL))
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

#define _XTAL_FREQ 32000000UL   //Frecuencia de trabajo actual
#include <xc.h>
#include "DHT11.h"
#include <stdio.h>
#include "Serial.h"

//cadenas para el terminal
unsigned char cadena[] = {"UPC 2019-2 Microcontroladores"};
unsigned char menu1[] = {"Bienvenidos al LB4 "};
unsigned char menu2[] = {"Opciones del menu: "};
unsigned char menu3[] = {"(A) Visualizar la temperatura (C) "};
unsigned char menu4[] = {"(B) Visualizar la humedad (%RH)  "};
unsigned char menu5[] = {"(C) Visualizar la distancia (cm) "};
unsigned char menu6[] = {"(D) Visualizar autores"};
unsigned char menu7[] = {"(m) Visualizar menu "};
unsigned char autor[] = {"Gabriela Vega Lopez  "};
unsigned char error[] = {"Tecla erronea       "};
unsigned char barra[] = {"-----------------------------------"};
unsigned char tempe[] = {"La temperatura es: "};
unsigned char hume[] = {"La humedad es: "};
unsigned char dis[] = {"La distancia es: "};
unsigned char signo[] = {" C "}, signo2[] = {"% "};

int temp,dtemp,hum,dhum,status;
char dtem,tem,dte,dhu,hu,dh;
int chekin;

//funcion DTH11
void valores(void){
    DHT11_init();
    status = DHT11_CheckResponse();
    if (status){
        chekin = DHT11_ReadData(&hum,&dhum,&temp,&dtemp);
        if(chekin){
            dtem = (temp/10+'0');
            tem = (temp%10+'0');
            dte = (dtemp+'0');
            dhu = (hum/10+'0');
            hu = (hum%10+'0');
            dh = (dtemp+'0');
        }
    }
}

char centena_ultra, decena_ultra, unidad_ultra;
// funcion ultrasonido
void ultrasonido(void){
    int rango=0;
    TMR1H = 0;                //Sets the Initial Value of Timer
    TMR1L = 0;                //Sets the Initial Value of Timer

    LATBbits.LATB1 = 1;                  //TRIGGER HIGH
    __delay_us(10);           //10uS Delay 
    LATBbits.LATB1 = 0;                  //TRIGGER LOW

    while(!PORTBbits.RB4);              //Waiting for Echo
    TMR1ON = 1;               //Timer Starts
    while(PORTBbits.RB4);               //Waiting for Echo goes LOW
    TMR1ON = 0;               //Timer Stops

    rango=TMR1;
    rango=rango/58.82;
    rango=rango+1;

    if(rango>=2 && rango<=400){         //el rango de trabajo del HSCR04 2cm a 400cm
        centena_ultra = rango/100 + '0';
        decena_ultra=(rango%100)/10 + '0';
        unidad_ultra=rango%10 + '0';
    }      
}

// funciona para mostrar el menú
void show_menu(void){
    send_string(menu1,18);
    send_newline();
    send_string(barra,35);
    send_newline();
    send_string(menu2,19);
    send_newline();
    send_string(menu3,34);
    send_newline();
    send_string(menu4,31);
    send_newline();
    send_string(menu5,32);
    send_newline();
    send_string(menu6,22);
    send_newline();
    send_string(menu7,19);
    send_newline();
    send_string(barra,35);
    send_newline();
}

// funcion principal
void main(void){
    uc_config();
    send_string(cadena,29);     // enviar la cadena de UPC 2019-02 Microcontroladores
    send_newline(); 
    while(1);
}

// funcion de interrupción cuando se preciona una tecla
void __interrupt () RCIsr(void){    
    PIR1bits.RC1IF = 0;
    switch(RCREG){
        case 'a':
            valores();
            send_string(tempe,20);
            send_char(dtem);    
            send_char(tem);
            send_char('.');
            send_char(dte);
            send_string(signo, 3);
            send_newline();
            break;
            
        case 'b':
            send_string(hume, 15);
            send_char(dhu);
            send_char(hu);
            send_char('.');
            send_char(dh);
            send_char('%');
            send_newline();
            break;
            
        case 'c':
            send_string(dis,17);
            ultrasonido();
            //send_char(centena_ultra);
            send_char(decena_ultra);
            send_char(unidad_ultra);
            send_string(" cm", 3);
            send_newline();
            break;
            
        case 'd':
            send_string(autor,20);
            send_newline();
            break;
            
        case 'm':
            show_menu();
            send_newline();
            break;
            
        default:
            send_string(error,20);
            send_newline();
    }
}



