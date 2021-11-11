#include <stdio.h>

void print_possible(int wanted)
{
	int i = 0;
	printf("for %2.2d : ", wanted);
	while (i < 127)
	{
		if (!isalpha(i))
		{
			i++;
			continue ;
		}
		if ((i & 0xF) == wanted)
			printf(" %c", i);
		i++;
	}
	printf("\n");
}

int get_index(char a, char *charset)
{
	int i;

	i = 0;
	while (charset[i])
	{
		if (charset[i] == a)
			return (i);
		i++;
	}
	return (0);
}

int main(int ac, char **av)
{
	char	charset[] = "isrveawhobpnutfg";
	char	result[7];
	char	giants[] = "giants";
	char	*input;
	char	letter;

	int i = 0;
	
	input = av[1];
	while (i <= 5)
	{
		printf("index : %d\n", input[i] & 0xF);
		letter = charset[(input[i] & 0xF)];
		result[i] = letter;
		i++;
	}
	result[6] = 0;
	printf("|%s|\n", result);
	printf("Now let's see other possibilities\n");
	i = 0;
	while (i <= 5)
	{
		int index = get_index(giants[i], charset);
		print_possible(index);
		i++;
	}
	return (0);
}
