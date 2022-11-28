#include <stdio.h>

unsigned char a[9] = {0x00, 0, 0, 0, 0x03, 0, 0, 0x00, 0x04};

int main(void) {
	int *p;
	int b, c;
	
	p = (int*)(a + 4);
	b = *p;
	p = (int*)(a + 8);
	c = *p;
	
	printf("%d\r\n", b);
	printf("%d\r\n", c);
	return 0; 
}
