#include <8051.h>
#include <lcd.h>

#define saida P2_6
#define FU P1_0
#define FD P1_1

int t1 = 0xFF;
int t2 = 0xFD;
float freq = 0.000002;
int up, upCopy, down, downCopy = 2;
char flag = 0;

void freqU(){
	if(t2 <= 0xFE && t1 != 0xff){
		t2 += 0x1;
	        freq--;
	} else if (t1 < 0xFF) {
		t1 += 0x1;
		t2 = 0x0;
	        freq--;
	}
	
	TH0 = t1;
	TL0 = t2;
}

void freqD() {
	if(t2 >= 0x1 && t1 != 0x0){
		t2 -= 0x1;
		freq++;
	} else if (t1 > 0x0){
		t1 -= 0x1;
		t2 = 0xFF;
		freq++;
	}

	TH0 = t1;
	TL0 = t2;
}

void timer() __interrupt(1){
	TH0 = t1;
	TL0 = t2;

	if(upCopy >= 0){
		saida = 0;
		upCopy--;
	} else if(downCopy >= 0) {
		saida = 1;
		downCopy--;
	}
	
	if(downCopy < 0) {
		if(flag) {
			up++;
			down--;

			if(down < 0){
				flag = 1;
				down++;
				up--;
			}
		} else {
			up--;
			down++;

			if(up < 0){
				flag = 0;
				down--;
				up++;
			}
		}
	}
		
	upCopy = up;
	downCopy = down;

	if(!FU){
		freqU();
	}

	if(!FD){
		freqD();
	}
}

void main(){
	Lcd4_init();
        Lcd4_clear();
	TMOD = 0x01;
	TH0 = t1;
	TL0 = t2;
	EA = 1;
	ET0 = 1;
	TR0 = 1;

	while(1){
		Lcd4_Set_Cursor(1, 1);
                Lcd4_Write_String("Frequency: ");
                Lcd4_Set_Cursor(2, 1);
                Lcd4_Write_Char((float)1/freq + 0x30);
                Lcd4_Write_Char(' Hz\0');
	}
}
