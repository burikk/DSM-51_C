#include <at89x52.h>
#include <absacc.h>
#include "LCD_lib.c"  //biblioteka do LCD

int main() {
unsigned char i,tab[] = "";
unsigned char zapraszamy[] = "Zapraszamy!\n";
unsigned char czekac[] = "Prosze czekac!\n";
unsigned char zamkniete[] = "Zamkniete";
unsigned char duzo[] = "Za duzo osob!\n";
unsigned char koniec[] = "Dziekujemy!\n";
unsigned char szybaA[] = "SOS! A!";
unsigned char szybaB[] = "SOS! B!";
unsigned char szyby[] = "SOS! A i B!";
unsigned char a, b, bit00, bit01, bit05, bitA, bitB, bitC, bitD;
unsigned char ilosc_jednostki;
unsigned char ilosc_dziesiatki;
int ilosc_ludzi;
int max_ludzi;
ilosc_ludzi=0; //wchodzacy do muzeum
max_ludzi=10;  //max ilosc ludzi przy jednoczesnym wchodzeniu;
  //do tego miejsca zostaly zadeklarowane potrzebujace zmienne
	lcd_init();
	do{               //glowna petla wykonania programu
a=~XBYTE[0xF021];
b=~XBYTE[0xF022];
bit00=(a&0x01);	//0
bit01=(a&0x04); //2
bit05=(a&0x20); //5 ludzi na wejscie	
bitA=(b&0x04); //A
bitB=(b&0x08); //B
bitC=(b&0x10); //C - przycisk ktorym zasymulowac przekrocenie max ludzi
bitD=(b&0x20); //D - przycisk ktorym mozna zlapac osob ponad maksa i ich wyrzucic		

		if(bit00!=0x01)  //muzeum zamkniety
			{
				for (i=0;zamkniete[i];i++) //napis zamkniete
				{
					lcd(zamkniete[i]);
					wait(250);
				}
				lcd_clr();
			
	//ochrona muzeum od przenikniecia zlodzieja xd - czujniki rozbicia szyb//
			
				if (bitA==0x04)  //rozbicie szyby A
					{
						lcd_clr();
						P1_5 = 0; //sygnal breczyka o awarii
						for (i=0;szybaA[i];i++)
							{
								lcd(szybaA[i]);  //wyswietla sie napis ze szyba A jest rozbita
							}
						if (bitB==0x08) //rozbicie szyby B
							{
								lcd_clr();
								for (i=0;szyby[i];i++)  //szyba B jest rozbita
									{
										lcd(szyby[i]);  
									}
							}
							wait(15000); //15 sekund 'naprawy' tak jakby
							P1_5=1; //juz po naprawie, brzeczyk sie nie swieci
							lcd_clr();
							//w ciagu 15 sekund trzeba wylaczyc przycisk A lub B w zaleznosci od awarii
					}
					
		//to samo dla szyby B/////////////////////////////////
					
					if (bitB==0x08)  //rozbicie szyby B
					{
						lcd_clr();
						P1_5 = 0; //sygnal breczyka o awarii
						for (i=0;szybaB[i];i++)
							{
								lcd(szybaB[i]);  //wyswietla sie napis ze szyba B jest rozbita
							}
						if (bitA==0x04)  //rozbicie szyby A
							{
								lcd_clr();
								for (i=0;szyby[i];i++)  //szyba A jest rozbita
									{
										lcd(szyby[i]);  
									}
							}
							wait(15000); //15 sekund 'naprawy' tak jakby
							P1_5=1; //juz po naprawie
							lcd_clr();
							//w ciagu 15 sekund trzeba wylaczyc przycisk A lub B w zaleznosci od awarii
					}
				}
			else if(bit00==0x01)  //muzeum otwarty
				{
						if (bit05!=0x20) //jezeli "5" nie jest nacisnieta, to wyswietla sie ten napis
						{
							for (i=0;zapraszamy[i];i++) //napis zapraszamy
							{
								lcd(zapraszamy[i]);
								wait(250);
							}
						}
						lcd_clr();
						if (bit05==0x20)  //jesli wcisniety przycisk 5 zaczynaja wchodzic osoby
						{
							if(ilosc_ludzi<max_ludzi) //zwiekszanie ilosci osob poki osiagnie wartosc max
							{
								ilosc_ludzi++;  //inkrementacja osob
								for (i=0;zapraszamy[i];i++) lcd(zapraszamy[i]);
								P1_7=1;  //dioda off
								P1_5=1;  //brzeczyk off
								//
								ilosc_jednostki=ilosc_ludzi%10;
								ilosc_dziesiatki=(ilosc_ludzi-ilosc_jednostki)/10;
								lcd(ilosc_dziesiatki + '0');
								lcd(ilosc_jednostki + '0');   //sposob na wytwarzanie liczby 2cyfrowej
							}
							if(ilosc_ludzi==max_ludzi)  //kiedy osiaga 10 osob zaczyna sie wycieczka
							{
								lcd_clr();
								for (i=0;czekac[i];i++) lcd(czekac[i]);
								P1_7=0;  //dioda on
								P1_5=1;  //brzeczyk off
								//
								ilosc_jednostki=ilosc_ludzi%10;
								ilosc_dziesiatki=(ilosc_ludzi-ilosc_jednostki)/10;
								lcd(ilosc_dziesiatki + '0');
								lcd(ilosc_jednostki + '0');  //sposob na wytwarzanie liczby 2cyfrowej
								wait(1000);
							}
							if(bitC==0x10)  //symulacja przechodzenia liczby osob wiecej od zadeklarowanej
							{
								if(ilosc_ludzi>=max_ludzi)
								{
									lcd_clr();
									ilosc_ludzi++;
									for (i=0;duzo[i];i++) lcd(duzo[i]);
									P1_7=0;  //dioda on
									P1_5=0;  //brzeczyk on
									//
									ilosc_jednostki=ilosc_ludzi%10;
									ilosc_dziesiatki=(ilosc_ludzi-ilosc_jednostki)/10;
									lcd(ilosc_dziesiatki + '0');
									lcd(ilosc_jednostki + '0');  //sposob na wytwarzanie liczby 2cyfrowej
									wait(2000);
								}
							}
							if(ilosc_ludzi==max_ludzi && bitC!=0x10)  //sprawdza czy osoby <10 i
							{                                         //czy nie jest wlaczona "C" dla symulacji 
								wait(10000);//czas wycieczki            //przekracania osob. jezeli tak, to prowadzi wycieczke
								lcd_clr();
								ilosc_ludzi=0;  //czysci ludzi i ustawia wartosc na 0 dla ponownej wycieczki
								P1_7=1;  //dioda off
								for (i=0;koniec[i];i++) lcd(koniec[i]);
								//
								ilosc_jednostki=ilosc_ludzi%10;
								ilosc_dziesiatki=(ilosc_ludzi-ilosc_jednostki)/10;
								lcd(ilosc_dziesiatki + '0');
								lcd(ilosc_jednostki + '0');  //sposob na wytwarzanie liczby 2cyfrowej
							}
							if(bitC!=0x10)  //jezeli wylaczylismy przekracanie osob
							{
								if(ilosc_ludzi>max_ludzi)
								{
									ilosc_ludzi=max_ludzi;  //to ustawia wartosc na wartosc max osob dla przeprowadzenia wycieczki
									P1_5=1;  //brzeczyk off
								}
							}
							wait(1500);
							lcd_clr();
						}
					}
	}while(1);

while (1);
}
