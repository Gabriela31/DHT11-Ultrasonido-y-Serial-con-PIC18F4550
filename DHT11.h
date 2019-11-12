#define _XTAL_FREQ 32000000UL

void DHT11_init(void);
int DHT11_CheckResponse(void);
int DHT11_ReadData(int *hum,int *dhum,int *temp, int *dtemp);