//#define _115200 25
//#define _57600 51
//#define _38400 77
//#define _19200 155
//#define _9600 311
//#define _4800 624
#define _XTAL_FREQ 32000000UL

void uc_config(void);
void send_string(const unsigned char *vector, unsigned int numero);
void send_char(unsigned char letrita);
void send_newline(void);
