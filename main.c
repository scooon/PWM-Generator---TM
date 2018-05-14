#include <REG51.H>

extern void a_func (void);



// WYSW segmentowy LED
xdata unsigned char WLED _at_ 0x8018;
unsigned char WYSW[4];
code const unsigned char cyferki[17] = {  0x3f,	0x06, 0x5b,	0x4f,	0x66,	0x6d,	0x7d,	0x07,	0x7f,	0x6f,	0x77, 0x79, 0x0F, 0x1F,	0x0E, 0X71, 0x00};
char q, z,k, seg;

// Klawiatura numeryczna
unsigned char key_val;
sbit P1_0=0x90;                      
sbit P1_1=0x91;												
sbit P1_2=0x92;
sbit P1_3=0x93;
sbit P1_4=0x94;
sbit P1_5=0x95;

char locked = 0;
int k0,k1,o=0,tempfreq;
unsigned int klik[3];

	
	

// PWM_Pin
sbit PWM_Pin = 0x96;

// Deklaracje
void Init(void);
int lenght(int x);

// Zmienne PWM
unsigned char PWM = 0;	  //  0 (0%) - 255 (100%)
unsigned int temp = 0;    // Dla przerwania

int PWMfreq = 0;	// Czestotliwosc

// LCD
xdata unsigned char CTRLW _at_ 0x8010;
xdata unsigned char DATW _at_ 0x8011;
xdata unsigned char CTRLR _at_ 0x8012;
xdata unsigned char DATR _at_ 0x8013;



unsigned int cycles = 0;
unsigned int cyfra = 0;
 unsigned int b;
 int h = 0, liczba;

void Init_LCD(void)
{
	for(b=0;b<15000;b++);
 CTRLW=0x38;
 for(b=0;b<4200;b++);
 CTRLW=0x38;
 for(b=0;b<120;b++);
 CTRLW=0x38;
 while (CTRLR&0x80);
 CTRLW=0x38;
 while (CTRLR&0x80);
 CTRLW=0x0c;
 while (CTRLR&0x80);
 CTRLW=0x02;
 CTRLW=0x01;
 while (CTRLR&0x80);
 CTRLW=0x06;
 while (CTRLR&0x80);
 CTRLW=0x80;
 while (CTRLR&0x80);
}

void main (void)
{

  Init();              // Inicjalizacja         
	Init_LCD();
	
	klik[0] = 0;
	klik[1] = 0;
	klik[2] = 0;

 while(1)
 {
	 tempfreq = (klik[0]*100)+(klik[1]*10)+klik[2];
	 if(tempfreq>0){
		 PWMfreq = tempfreq;
	 }else{
	 PWMfreq = 1;
	 klik[2] = 1;
	 }
	 if(tempfreq>112){
	 PWMfreq = 100;
	 klik[0] = 100;
	 }
  b=potencjometr();
  PWM = b; 
  DATW = 'W';
		while (CTRLR&0x80);
  DATW = 'Y';
		while (CTRLR&0x80);
  DATW = 'P';
	 	while (CTRLR&0x80);
	DATW = ':';
		while (CTRLR&0x80);
  DATW = ' ';
		while (CTRLR&0x80);
	b=(b*100)/255;
  DATW=b/100+'0';
		while (CTRLR&0x80);
  DATW=(b/10)%10+'0';
		while (CTRLR&0x80);
  DATW=b%10+'0';
		while (CTRLR&0x80);
	DATW=' ';
		while (CTRLR&0x80);
	DATW='%';
		while (CTRLR&0x80);		
	CTRLW=0x80; 
		while (CTRLR&0x80);
	
	// Wypelnienie WLED
	for(z=0;z<4;z++){WYSW[z] = 16;};

	liczba = (PWM*100)/255;
	if(lenght(liczba)==2)
		{
		WYSW[1] = liczba%10;
		WYSW[2] = liczba/10;
		}
	else if(lenght(liczba)==1)
		{
		WYSW[2] = liczba%10;
		}
	else if(lenght(liczba)==3)
		{
		WYSW[0] = liczba%10;
		WYSW[1] = (liczba/10)%10;
		WYSW[2] = liczba/100;
		}
	

	
 	DATW = 'P';
		while (CTRLR&0x80);
  DATW = 'W';
		while (CTRLR&0x80);
  DATW = 'M';
		while (CTRLR&0x80);
	DATW = ':';
		while (CTRLR&0x80);
  DATW = ' ';
		while (CTRLR&0x80);
  DATW=klik[0]+'0';
		while (CTRLR&0x80);
	DATW =klik[1]+'0';
		while (CTRLR&0x80);
  DATW='.';
		while (CTRLR&0x80);
	DATW=klik[2]+'0';
		while (CTRLR&0x80);
	DATW=' ';
		while (CTRLR&0x80);
	DATW='H';
		while (CTRLR&0x80);
	DATW='z';
		while (CTRLR&0x80);
  CTRLW=0xC0; 
		while (CTRLR&0x80);
	

}
}


// Timer0 init
void InitTimers(void)
{
TMOD = 0x11;    // Zalaczanie przerwania 1 i 2
TCON = 0x50;
TH0 = 0x00;      // First time value
TL0 = 0x00; 
TH1 = 0x00;      // First time value
TL1 = 0x00;	
ET0 = 1;         // Enable przerwan
EA  = 2;        
TR0 = 1; 
TR1 = 1;	
ET1 = 1;
}




// Init PWM & Timerów
void Init(void)
{
PWM = 0;         // Wypelnienie 0
InitTimers();    // Uruchom timer przerwan
}


int lenght(int x) 
{
    if(x>=1000)	return 4;
		else if(x>=100) return 3;
		else if(x>=10) return 2;
		else return 1;
}


// Timer0
void Timer0 (void) interrupt 1  
{
TR0 = 0;    // Stop Timer 0
TH0 = (unsigned char)(-5000 >> 8);
TL0 = (unsigned char)(-5000 & 0x00ff);
	
if(PWM_Pin)	// Na stan wysoki
	{
	PWM_Pin = 0;
	temp = (255-PWM)*PWMfreq;
	TH0  = 0xFF - (temp>>16)&0xFF;
	TL0  = 0xFF - temp&0xFF;
	}
else	     // Na stan niski
	{
	PWM_Pin = 1;
	temp = PWM*PWMfreq;
	TH0  = 0xFF - (temp>>16)&0xFF;
	TL0  = 0xFF - temp&0xFF;
	}
 
TF0 = 0;     // Czyszczenie flagi przerwania
TR0 = 1;     // Start Timera
}


void Timer1 (void) interrupt 3  
{

TR1 = 0;    // Stop Timer 0
 
TH1 = (unsigned char)(-2500 >> 8);
TL1 = (unsigned char)(-2500 & 0x00ff);
	
		seg=++seg&3; 
		P1=(P1&0xc0)|0x3c|seg;
	
		WLED = cyferki[WYSW[q]];
		if(++q>3){q=0;} 
	
		k0=P1_0;                         
		k1=P1_1;                         
		
		P1_0=0;
		P1_1=0;
		P1_2=1;
		P1_3=1;
		P1_4=1;
		P1_5=1;
		if (P1_2 == 0)
			{
				key_val=0;
				locked=1;
			}else if(locked==1 && key_val==0){
				klik[o] = key_val;
				o=o+1;
				locked=0;
			}
		
		if (P1_3 == 0)
			{
				key_val=4;
				locked=1;
			}else if(locked==1 && key_val==4){
				klik[o] = key_val;
				o=o+1;
				locked=0;
			}
		if (P1_4 == 0) 
			{
				key_val=8;
				locked=1;
			}else if(locked==1 && key_val==8){
				klik[o] = key_val;
				locked=0;
				o=o+1;
			}
		P1_0=1;
		P1_2=1;
		P1_3=1;
		P1_4=1;
		P1_5=1;
		if (P1_2 == 0)
			{
			key_val=1;
			locked=1;
			}else if(locked==1 && key_val==1){
				klik[o] = key_val;
				o=o+1;
				locked=0;
			}
		if (P1_3 == 0)
			{
			key_val=5;
			locked=1;
			}else if(locked==1 && key_val==5){
				klik[o] = key_val;
				o=o+1;
				locked=0;
			}
		if (P1_4 == 0) 
			{
			key_val=9;
			locked=1;
			}else if(locked==1 && key_val==9){
				klik[o] = key_val;
				o=o+1;
				locked=0;
			}
		P1_0=0;
		P1_1=1;
		P1_2=1;
		P1_3=1;
		P1_4=1;
		P1_5=1;
		if (P1_2 == 0)
			{
				key_val=2;
			  locked=1;
			}else if(locked==1 && key_val==2){
				klik[o] = key_val;
				o=o+1;
				locked=0;
			}
		if (P1_3 == 0)
			{
				key_val=6;
				locked=1;
			}else if(locked==1 && key_val==6){
				klik[o] = key_val;
				o=o+1;
				locked=0;
			}
		P1_0=1;
		P1_2=1;
		P1_3=1;
		P1_4=1;
		P1_5=1;
		if (P1_2 == 0)
			{
				key_val=3;
				locked=1;
			}else if(locked==1 && key_val==3){
				klik[o] = key_val;
				o=o+1;
				locked=0;
			}
		if (P1_3 == 0)
			{
				key_val=7;
				locked=1;
			}else if(locked==1 && key_val==7){
				klik[o] = key_val;
				o=o+1;
				locked=0;
			}
		P1_0=k0;
		P1_1=k1;
	
	
	if(o>2){o=0;} 
		
TF1 = 0;     // Czyszczenie flagi przerwania
TR1 = 1;     // Start Timera
}