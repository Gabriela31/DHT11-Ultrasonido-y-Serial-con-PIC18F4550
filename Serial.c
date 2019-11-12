#include <xc.h>
#include "Serial.h"

void uc_config(void){
    //Para configurar la velocidad a 38400 con 32MHz
    BAUDCONbits.BRG16 = 0;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 0;
    SPBRGH = 0;
    SPBRG = 12;
    TXSTAbits.TXEN = 1;         //Habilitamos la transmision en el EUSART
    RCSTAbits.CREN = 1;         //Habilitamos el receptor del EUSART
    RCSTAbits.SPEN = 1;         //Encendemos el EUSART
    TRISCbits.RC6 = 0;
    INTCONbits.GIE = 1;         //Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;        //Habilitamos interrupciones de perifericos
    PIE1bits.RC1IE = 1;         //Habilitamos interrupcion de recepcion en EUSART
    ADCON1 = 0x0F;              //Para que los puertos RA y RE sean digitales
     // PORTAbits.RA0= 1;
    TRISBbits.RB4 = 1;          //RB4 puerta de entrada (ECHO) 
    T1CON=0xB0;                 // timer 1
}

void send_string(const unsigned char *vector, unsigned int numero){
    for(unsigned char i=0;i<numero;i++){
        TXREG = vector[i];
        while(TXSTAbits.TRMT == 0);
    }
}

void send_char(unsigned char letrita){
    TXREG = letrita;
    while(TXSTAbits.TRMT == 0);
}

void send_newline(void){
    TXREG = 0x0A;
    while(TXSTAbits.TRMT == 0);
    TXREG = 0x0D;    
    while(TXSTAbits.TRMT == 0);
}

