#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern char **environ;

int main(int argc, char **argv, char **envp)
// int main(void)
{
	int i = 0;

	while(environ[i] != NULL)
	{
		fprintf(stdout, "[%s]\n", environ[i]);
		i++;
	}
	char	*new;
	char	new_str[] = {"h=hello"};

	new = malloc(sizeof(new_str));
	strcpy(new, new_str);
	environ[i] = new;
	environ[i + 1] = NULL;
	fprintf(stdout, "[%s]\n", environ[i]);
	fprintf(stdout, "[%s : %s]\n", "h", getenv("h"));
}
