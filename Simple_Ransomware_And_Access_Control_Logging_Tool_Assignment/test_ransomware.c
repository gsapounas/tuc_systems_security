#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int string_ends_with(const char *str, const char *suffix);

int main(int argc, char *argv[]) 
{
	int i;
	size_t bytes;

	// printf("%s\n", argv[0]);
	// printf("%s\n", argv[1]);
	// printf("%s\n", argv[2]);

	// if (argc < 3)
	// {	
	// 	printf("Print");
	// 	exit(-1);
	// }

	int number_of_files = atoi(argv[2]);

	FILE *file;

	char *filename = calloc(50, sizeof(char));

	if (filename == NULL)
	{
		perror("ERROR");
		return -1;
	} 
		
	char *filenumber = calloc(20, sizeof(char));

	if (filenumber == NULL)
	{
		perror("ERROR");
		return -1;
	} 

	char *path = calloc(100, sizeof(char));

	if (path == NULL)
	{
		perror("ERROR");
		return -1;
	} 

	char *path_and_filename = calloc(150, sizeof(char));

	if (path_and_filename == NULL)
	{
		perror("ERROR");
		return -1;
	} 

	strcpy(path, argv[1]);

	/* example source code */

	for (i = 0; i < 100; i++) 
	{
		strcpy(filename, "file_");

		sprintf(filenumber, "%d", i + 1);

		strcat(filename, filenumber);

		strcat(filename, ".txt");

		if (string_ends_with(path, "/") == 0)
		{
			strcat(path, "/");
		}

		strcat(path_and_filename, path);

		strcat(path_and_filename, filename);
		
		file = fopen(path_and_filename, "w+");

		if (file == NULL) 
		{
			printf("fopen error \n");
		}
		else 
		{
			bytes = fwrite(path_and_filename, strlen(path_and_filename), 1, file);

			fclose(file);
		}
		
		memset(filenumber, 0 , 20);
		memset(filename, 0 , 50);
		memset(path_and_filename, 0, 150);
	}

	for (i = 0; i < number_of_files; i++) 
	{
		strcpy(filename, "f");

		sprintf(filenumber, "%d", i + 1);

		strcat(filename, filenumber);

		strcat(filename, ".txt");

		if (string_ends_with(path, "/") == 0)
		{
			strcat(path, "/");
		}

		strcat(path_and_filename, path);

		strcat(path_and_filename, filename);
		
		file = fopen(path_and_filename, "w+");

		if (file == NULL) 
		{
			printf("fopen error \n");
		}
		else 
		{
			bytes = fwrite(path_and_filename, strlen(path_and_filename), 1, file);

			fclose(file);
		}
		
		memset(filenumber, 0 , 20);
		memset(filename, 0 , 50);
		memset(path_and_filename, 0, 150);
	}

	free(filename);
	free(filenumber);
	free(path);
	free(path_and_filename);
}


int string_ends_with(const char *str, const char *suffix)
{
    if (!str || !suffix)
    {
        return 0;
    }

    size_t string_len = strlen(str);

    size_t len_suffix = strlen(suffix);

    if (len_suffix >  string_len)
    {
        return 0;
    }
    return strncmp(str + string_len - len_suffix, suffix, len_suffix) == 0;
}