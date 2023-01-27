#include <stdlib.h>
#include "unistd.h"

int main(int argc, char **argv)
{
	(void)argc;
	unsigned char octet = (unsigned char)argv[1][0];
	unsigned char bit;
	int i = 7;
	while (i > 0)
	{
		bit =  (octet >> i & 1) + '0';
		write(1,&bit , 1);
		i--;
	}
	return (0);
}