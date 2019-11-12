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
#include "Serial.h"
#include "DHT11.h"
#include <stdio.h>

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

//// funcion para enviar un string
//void send_string(const unsigned char *vector, unsigned int numero){
//    for(unsigned char i=0;i<numero;i++){
//        TXREG = vector[i];
//        while(TXSTAbits.TRMT == 0);
//    }
//}
//
//// funcion para enviar un char
//void send_char(unsigned char letrita){
//    TXREG = letrita;
//    while(TXSTAbits.TRMT == 0);
//}
//
//// funcion para enviar una nueva linea
//void send_newline(void){
//    TXREG = 0x0A;
//    while(TXSTAbits.TRMT == 0);
//    TXREG = 0x0D;    
//    while(TXSTAbits.TRMT == 0);
//}

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

//// funcion de configuración el micro
//void uc_config(void){
//    BAUDCONbits.BRG16 = 0;      //Para configurar la velocidad a 9600 con 32MHz
//    TXSTAbits.SYNC = 0;
//    TXSTAbits.BRGH = 0;
//    SPBRGH = 0;
//    SPBRG = 51;
//    TXSTAbits.TXEN = 1;         //Habilitamos la transmision en el EUSART
//    RCSTAbits.CREN = 1;         //Habilitamos el receptor del EUSART
//    RCSTAbits.SPEN = 1;         //Encendemos el EUSART
//    TRISCbits.RC6 = 0;
//    INTCONbits.GIE = 1;         //Habilitamos interrupciones globales
//    INTCONbits.PEIE = 1;        //Habilitamos interrupciones de perifericos
//    PIE1bits.RC1IE = 1;         //Habilitamos interrupciojn de recepcion en EUSART
//    ADCON1 = 0x0F;              //Para que los puertos RA y RE sean digitales
//}

// funcion principal
void main(void){
    uc_config();
    // OSCCON = 0x72;              // pone el oscilador a 8Mhz
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
            send_char(0xDE);
            send_char('C');
            send_newline();
            send_newline();
            break;
            
        case 'b':
            send_string(hume, 15);
            send_newline();
            send_char(dhu);
            send_char(hu);
            send_char('.');
            send_char(dh);
            send_char('%');
            send_newline();
            send_newline();
            break;
            
        case 'c':
            send_string(dis,17);
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