#include <stdio.h>

int	func4(int arg)
{
	if (arg <= 1)
		return (1);
	return (func4(arg + 0xFFFFFFFE) + func4(arg + 0xFFFFFFFF));
}

int main(int ac, char **av)
{
	int appah;
	if (ac != 2)
		return (1);
	if (sscanf(av[1], "%d", &appah) == 1 && appah > 0)
	{
		printf("func4 : %d\n", func4(appah));
		return (0);
		int res = func4(appah);
		if (res == 0x37)
			return (res);
		printf("BOOM\n");
		return (0);
	}
	printf("BOOM\n");
	return (0);
}
