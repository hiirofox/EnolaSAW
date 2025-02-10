#include <stdio.h>

FILE *fi, *fo;

const char hx[]="0123456789ABCDEF";
unsigned char temp;
void out(unsigned long i) {
	printf("0x%c",hx[(i>>4)&0xf]);
	printf("%c, ",hx[(i)&0xf]);
}
int main() {
	fi=fopen("melody.bin","rb");
	while (!feof(fi)) {
		fread(&temp,1,1,fi);
		out(temp);
	}
	return 0;
}
