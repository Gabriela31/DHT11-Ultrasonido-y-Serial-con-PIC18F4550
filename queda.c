//Bits de configuracion o directivas de preprocesador
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC2_PLL3// System Clock Postscaler Selection bits ([Primary Oscillator Src: /2][96 MHz PLL Src: /3])
#pragma config FOSC = XTPLL_XT  // Oscillator Selection bits (XT oscillator, PLL enabled (XTPLL))
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

#define _XTAL_FREQ 32000000UL   //Frecuencia de trabajo actual
#include <xc.h>
#include <stdio.h>

// variables para el DHT11
#define Data_Out LATA0         /* assign Port pin for data*/
#define Data_In PORTAbits.RA0  /* read data from Port pin*/
#define Data_Dir TRISAbits.RA0 /* Port direction */

// cadenas
unsigned char cadena[] = {"UPC 2019-2 Microcontroladores"};
unsigned char menu1[] = {"Bienvenidos al LB4 "};
unsigned char menu2[] = {"Opciones del menú: "};
unsigned char menu3[] = {"(A) Visualizar la temperatura (°C) "};
unsigned char menu4[] = {"(B) Visualizar la humedad (%RH)  "};
unsigned char menu5[] = {"(C) Visualizar la distancia (cm) "};
unsigned char menu6[] = {"(D) Visualizar autores"};
unsigned char menu7[] = {"(m) Visualizar menu "};
unsigned char autor[] = {"Gabriela Vega López  "};
unsigned char error[] = {"Tecla erronea       "};
unsigned char barra[] = {"-----------------------------------"};
// cadenas de prueba
unsigned char temp[] = {"La temperatura es:  "};
unsigned char hume[] = {"La humedad es:   "};
unsigned char dis[] = {"La distancia es:   "};
//variables
unsigned char final_temp_int[];
unsigned char final_temp_des[];
unsigned char final_hume_int[];
unsigned char final_hume_des[];
unsigned char final_dis[];


// funcion de configuracion del DH11
char DHT11_ReadData(){
  char i,data = 0;  
    for(i=0;i<8;i++)
    {
        while(!(Data_In & 1));  /* wait till 0 pulse, this is start of data pulse */
        __delay_us(30);         
        if(Data_In & 1)  /* check whether data is 1 or 0 */    
          data = ((data<<1) | 1); 
        else
          data = (data<<1);  
        while(Data_In & 1);
    }
  return data;
}

void DHT11_Start(){    
    Data_Dir = 0;  /* set as output port */
    Data_Out = 0;  /* send low pulse of min. 18 ms width */
    __delay_ms(18);
    Data_Out = 1;  /* pull data bus high */
    __delay_us(20);
    Data_Dir = 1;  /* set as input port */    
}

void DHT11_CheckResponse(){
    while(Data_In & 1);  /* wait till bus is High */     
    while(!(Data_In & 1));  /* wait till bus is Low */
    while(Data_In & 1);  /* wait till bus is High */
}

// funciones para mostrar en terminal
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

// funcion de configuración el micro
void uc_config(void){
    //Para configurar la velocidad a 9600 con 32MHz
    BAUDCONbits.BRG16 = 0;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 0;
    SPBRGH = 0;
    SPBRG = 51;
    TXSTAbits.TXEN = 1;         //Habilitamos la transmision en el EUSART
    RCSTAbits.CREN = 1;         //Habilitamos el receptor del EUSART
    RCSTAbits.SPEN = 1;         //Encendemos el EUSART
    TRISCbits.RC6 = 0;
    INTCONbits.GIE = 1;         //Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;        //Habilitamos interrupciones de perifericos
    PIE1bits.RC1IE = 1;         //Habilitamos interrupciojn de recepcion en EUSART
    ADCON1 = 0x0F;              //Para que los puertos RA y RE sean digitales
}

DHT11_Start();
DHT11_CheckResponse();
DHT11_ReadData();

void sensor_DHT(void){
    
    //para el DHT11
    unsigned char RH_Decimal,RH_Integral,T_Decimal,T_Integral; //variables de humedad y temperatura
    unsigned char Checksum;  //checksum
    unsigned char value[10]; //lo que bota el DHT11  
    
    DHT11_Start();  /* send start pulse to DHT11 module */
    DHT11_CheckResponse();  /* wait for response from DHT11 module */
    
    /* read 40-bit data from DHT11 module */
    RH_Integral = DHT11_ReadData();  /* read Relative Humidity's integral value */
    RH_Decimal = DHT11_ReadData();   /* read Relative Humidity's decimal value */
    T_Integral = DHT11_ReadData();   /* read Temperature's integral value */
    T_Decimal = DHT11_ReadData();    /* read Relative Temperature's decimal value */
    Checksum = DHT11_ReadData();     /* read 8-bit checksum value */
    
    /* convert humidity value to ascii and send it to display*/    
    sprintf(final_hume_int,"%d",RH_Integral);
    sprintf(final_hume_des,".%d ",RH_Decimal);
    
    /* convert temperature value to ascii and send it to display*/
    sprintf(final_temp_int,"%d",T_Integral);
    sprintf(final_temp_des,".%d",T_Decimal);
    __delay_ms(500);
    
}
void main(void){
    uc_config();
    send_string(cadena,29);
    send_newline(); 
    OSCCON = 0x72;  /* set internal oscillator with frequency 8 MHz*/

}

void __interrupt () RCIsr(void){
    PIR1bits.RC1IF = 0;
    switch(RCREG){
        case 'a':
            sensor_DHT();
            send_string(temp,20);
            send_string(final_temp_int,4);
            send_newline();
            break;
            
        case 'b':
            sensor_DHT();
            send_string(final_hume_int,15);
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

