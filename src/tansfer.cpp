#include <stdio.h>
#include <cstring>

FILE *fi, *fo;
class NOTE {
public:
	unsigned char _note; //note
	unsigned char _velo; //velocity
	unsigned short _inst; //instrument
	unsigned short _begn; //tick begin
	unsigned short _dura; //tick mount
	void out() {
		printf("NOTE = %d\nVELO = %d\nINST = %d\nBEGN = %d\nDURA = %d\n\n",_note,_velo,_inst,_begn,_dura);
	}
};
NOTE note;
char buf[32]; 
int stoi(char* str) {
	int tmp=0;
	while (*str != 0) {
		tmp*=10;
		tmp+=(*str)&0xf;
		str++;
	}
	return tmp;
}
int ctoihx(char c) {
	switch (c) {
		case '0':return 0;
		case '1':return 1;
		case '2':return 2;
		case '3':return 3;
		case '4':return 4;
		case '5':return 5;
		case '6':return 6;
		case '7':return 7;
		case '8':return 8;
		case '9':return 9;
		case 'a':return 10;
		case 'b':return 11;
		case 'c':return 12;
		case 'd':return 13;
		case 'e':return 14;
		case 'f':return 15;
	}
}
int stoihx(char* str) {
	int tmp=0;
	while (*str != 0) {
		tmp*=16;
		tmp+=ctoihx((*str))&0xf;
		str++;
	}
	return tmp;
}
short getnote(char c) {
	switch (c) {
		case 'z':return 0;
		case 's':return 1;
		case 'x':return 2;
		case 'd':return 3;
		case 'c':return 4;
		case 'v':return 5;
		case 'g':return 6;
		case 'b':return 7;
		case 'h':return 8;
		case 'n':return 9;
		case 'j':return 10;
		case 'm':return 11;
		case 'q':return 12;
		case '2':return 13;
		case 'w':return 14;
		case '3':return 15;
		case 'e':return 16;
		case 'r':return 17;
		case '5':return 18;
		case 't':return 19;
		case '6':return 20;
		case 'y':return 21;
		case '7':return 22;
		case 'u':return 23;
		case 'i':return 24;
		case '9':return 25;
		case 'o':return 26;
		case '0':return 27;
		case 'p':return 28;
	}
}
int pt;
char temp;
int main() {
	
	printf("%d",stoihx("64"));
	
	fi=fopen("melody.txt","rb");
	fo=fopen("melody.bin","wba");
	bool block=1;
	int stage=0;
	int a=0;
	memset(buf,0,8);
	pt=0;
	int col=0;
	while (!feof(fi)) {
		fread(&temp,1,1,fi);
		switch (temp) {
			case '`': {
				a=1;
				break;
			}
			case ';': {
				stage=0;
				note._velo=ctoihx(buf[0]);
				fwrite(&note,sizeof(note),1,fo);
				printf("COL = %d\n",col);
				col++;
				note.out();
				memset(buf,0,32);
				memset(&note,0,sizeof(note));
				pt=0;
				a=0;
				break;
			}
			case ',': {
				switch (stage) {
					case 0: {
						note._note=getnote(buf[0]);
						break;
					}
					case 1: {
						note._note+=ctoihx(buf[0])*12;
						break;
					}
					case 2: {
						note._inst=stoi(buf);
						break;
					}
					case 3: {
						note._begn=stoihx(buf);
						break;
					}
					case 4: {
						note._dura=stoihx(buf);
						break;
					}
				}
				stage++;
				memset(buf,0,32);
				pt=0;
				break;
			}
			default: {
				if (a==1) {
					buf[pt]=temp;
					pt++;
				}
				break;
			}
		}
	}
	return 0;
}
