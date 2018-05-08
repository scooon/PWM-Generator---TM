#include <REG51.H>

// Diody
xdata unsigned char PTWY _at_ 0x8008;


// WYSW segmentowy LED
xdata unsigned char WLED _at_ 0x8018;
unsigned char WYSW[4];
code const unsigned char cyferki[17] = {  0x3f,	0x06, 0x5b,	0x4f,	0x66,	0x6d,	0x7d,	0x07,	0x7f,	0x6f,	0x77, 0x79, 0x0F, 0x1F,	0x0E, 0X71, 0x00};
char q, z;

// Klawiatura numeryczna
unsigned char kol_val = 0, wiersz_val = 0, key_val =0, key_int = 0;
int Pr = 0, liczba, locked;
unsigned char freq[4];

// PWM_Pin
sbit PWM_Pin = 0x96;

// Deklaracje
void Init(void);
int lenght(int x);

// Zmienne PWM
unsigned char PWM = 0;	  //  0 (0%) - 255 (100%)
unsigned int temp = 0;    // Dla przerwania

int PWMfreq = 1;	// Czestotliwosc

// LCD
xdata unsigned char CTRLW _at_ 0x8010;
xdata unsigned char DATW _at_ 0x8011;
xdata unsigned char CTRLR _at_ 0x8012;
xdata unsigned char DATR _at_ 0x8013;


// ADC
xdata unsigned char PTAC0 _at_ 0x8005;
unsigned char POT0(void);


unsigned int cycles = 0;
unsigned int cyfra = 0;
 unsigned int b;
 int h = 0;
void BUSYLCD(void)
{
 while (CTRLR&0x80);
}

void main (void)
{

  Init();              // Inicjalizacja         
	


	// LCD INIT	
	

 for(b=0;b<15000;b++);
 CTRLW=0x38;
 for(b=0;b<4200;b++);
 CTRLW=0x38;
 for(b=0;b<120;b++);
 CTRLW=0x38;
 BUSYLCD();
 CTRLW=0x38;
 BUSYLCD();
 CTRLW=0x0c;
 BUSYLCD();
 CTRLW=0x02;
 CTRLW=0x01;
 BUSYLCD();
 CTRLW=0x06;
 BUSYLCD();
 CTRLW=0x80;
 BUSYLCD();


 
 while(1)
 {
	PWMfreq = 250;
	 
  b=POT0();
  PWM = b; 
  DATW = 'W';
  BUSYLCD();
  DATW = 'Y';
  BUSYLCD();
  DATW = 'P';
  BUSYLCD();
  DATW = ' ';
  BUSYLCD();
	b=(b*100)/255;
  DATW=b/100+'0';
  BUSYLCD();
  DATW=(b/10)%10+'0';
  BUSYLCD();
  DATW=b%10+'0';
  BUSYLCD();
  CTRLW=0x80; 
  BUSYLCD();
	
	
	// Wypelnienie WLED
	for(z=0;z<4;z++){WYSW[z] = 16;};

	liczba = (POT0()*100)/255;
	if(lenght(liczba)==2){
	WYSW[1] = liczba%10;
	WYSW[2] = liczba/10;
	}else if(lenght(liczba)==1){
	WYSW[2] = liczba%10;
	}else if(lenght(liczba)==3){
	WYSW[0] = liczba%10;
	WYSW[1] = (liczba/10)%10;
	WYSW[2] = liczba/100;
	}
	

	
  DATW = 'P';
  BUSYLCD();
  DATW = 'W';
  BUSYLCD();
  DATW = 'M';
  BUSYLCD();
  DATW = ' ';
  BUSYLCD();
  DATW = 'f';
	BUSYLCD();
  DATW = ' ';
  BUSYLCD();
  DATW=freq[0]+'0';
  BUSYLCD();
  DATW=freq[1]+'0';
  BUSYLCD();
  DATW=freq[2]+'0';
  BUSYLCD();
	DATW=freq[3]+'0';
  BUSYLCD();
	DATW=' ';
  BUSYLCD();
	DATW='H';
  BUSYLCD();
	DATW='z';
  BUSYLCD();
  CTRLW=0xC0; 
  BUSYLCD();
  
  
 
  
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


// Odczyt z ADCka POT0
unsigned char POT0(void)
{
 unsigned char a=0;
 PTAC0=0;
 for(a=0;a<120;a++);
 return PTAC0;
}



// Init PWM & Timerów
void Init(void)
{
PWM = 0;         // Wypelnienie 0
InitTimers();    // Uruchom timer przerwan

}


int lenght(int x) {
    if(x>=1000){
			return 4;
		}else if(x>=100){
			return 3;
		}else if(x>=10){
			return 2;
		}else{
    return 1;
		}
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
TH0  = 0xFF - (temp>>8)&0xFF;
TL0  = 0xFF - temp&0xFF;
}
else	     // Na stan niski
{
PWM_Pin = 1;
temp = PWM*PWMfreq;
TH0  = 0xFF - (temp>>8)&0xFF;
TL0  = 0xFF - temp&0xFF;
}
 
TF0 = 0;     // Czyszczenie flagi przerwania
TR0 = 1;     // Start Timera
}


void Timer1 (void) interrupt 3  
{
TR1 = 0;    // Stop Timer 0
 
TH1 = (unsigned char)(-2500 >> 16);
TL1 = (unsigned char)(-2500 & 0xffff);
	
		key_val=++key_val&3; 
		P1=(P1&0xc0)|0x3c|key_val;
	  //PSEG=cyferki[Pr];
	
			WLED = cyferki[WYSW[q]];
		if(++q>3){q=0;} 
	
	for(wiersz_val = 0;wiersz_val<4; wiersz_val++)
		{
				if(!(P1&(4<<wiersz_val)))
			{
				key_val = key_val + wiersz_val*4;
				Pr = key_val;
				PTWY = Pr;
			
				// Super kod filtrujacy klawisze
				locked = Pr;
				if(locked==999){
				}else{
						if(key_int>4){
				key_int = 0;
				}
				freq[key_int]= Pr;
				key_int = key_int+1;
				locked = 999;
				}
			
				
		
				
			}

		}
	
	
	
 
TF1 = 0;     // Czyszczenie flagi przerwania
TR1 = 1;     // Start Timera
}