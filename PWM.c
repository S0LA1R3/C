#include <8051.h>

#define saida P2_6

char HT = 4;
char LT = 4;
char HTC = 4;
char LTC = 4;
char flag = 0;
int t1 = 0xFF;
int t2 = 0xFA;

void main(){
	TMOD = 0x01;
	TH0 = t1;
	TL0 = t2;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
	EX0 = 1;
	EX1 = 1;
	PX0 = 1;
	PX1 = 1;

	while(1);
}

void wave() __interrupt(1){
	TH0 = t1;
	TL0 = t2;

	if(HTC > 0){
		saida = 0;
		HTC--;
	} else if(LTC > 0){
		saida = 1;
		LTC--;
	} else if(flag == 0) {
		HT++;
		LT--;
		HTC = HT;
		LTC = LT;

		if(HT > HT + LT - 2) {flag = 1;}
	} else {
		HT--;
		LT++;
		HTC = HT;
		LTC = LT;

		if(HT <= 1) {
			flag = 0;
		}
	}
}

void increaseFrequency() __interrupt(0) {
	if(t2 <= 0xFE) {
		t2 += 0x1;
	} else if(t1 < 0xFF) {
		t1 += 0x1;
		t2 = 0x0;
	} else {
		t1 = 0xFF;
		t2 = 0xFF;
	}

	TH0 = t1;
	TL0 = t2;
}

void decreaseFrequency() __interrupt(2){
	if(t2 >= 0x1) {
		t2 -= 0x1;
	} else if(t1 > 0x0) {
		t1 -= 0x1;
		t2 = 0xFF;
	} else {
		t1 = 0x0;
		t2 = 0x0;
	}
}
