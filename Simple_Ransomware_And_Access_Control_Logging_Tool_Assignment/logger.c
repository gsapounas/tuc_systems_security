#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>
#include <libgen.h>

#include <assert.h>

void log_to_file(FILE *, int, const char *, int, int, unsigned char *);
unsigned char* read_from_file(FILE *);
int get_file_length(FILE *);
void calculate_hash(unsigned char *, unsigned char *, int);

FILE *fopen64(const char *path, const char *mode)
{
	FILE *fopen64_ret;

	fopen64_ret = fopen(path, mode);

	return fopen64_ret;
}

FILE *fopen(const char *path, const char *mode) 
{
	
	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);

	//Decide access type
	int access_type;

	//Decide if action is denied
	int is_action_denied = 1;

	if (access(path, F_OK) != 0) 
	{	
    	access_type = 0;

    	is_action_denied = 0;

    	if ((strcmp(mode, "r") == 0) || (strcmp(mode, "rb") == 0) || (strcmp(mode, "r+") == 0) || (strcmp(mode, "rb+") == 0))
		{
			is_action_denied = 1;
		}
	} 
	else
	{
		access_type = 1;

		if ((strcmp(mode, "r") == 0) || (strcmp(mode, "rb") == 0))
		{
			if (access(path, R_OK) == 0)
			{
				is_action_denied = 0;
			}	
			else
			{
				is_action_denied = 1;
			}	
		}
		else if ((strcmp(mode, "r+") == 0) || (strcmp(mode, "rb+") == 0))
		{
			if ((access(path, R_OK) == 0) && (access(path, W_OK) == 0))
			{
				is_action_denied = 0;
			}	
			else
			{
				is_action_denied = 1;
			}	
		}
		else if ((strcmp(mode, "w") == 0) || (strcmp(mode, "wb") == 0) || (strcmp(mode, "a") == 0) || (strcmp(mode, "ab") == 0))
		{
			if (access(path, W_OK) == 0)
			{
				is_action_denied = 0;
			}
			else
			{
				is_action_denied = 1;
			}
		}
		else if ((strcmp(mode, "w+") == 0) || (strcmp(mode, "wb+") == 0) || (strcmp(mode, "a+") == 0) || (strcmp(mode, "ab+") == 0))
		{
			if ((access(path, R_OK) == 0) && (access(path, W_OK) == 0))
			{
				is_action_denied = 0;
			}	
			else
			{
				is_action_denied = 1;
			}
		}
	
	}
	/* call the original fopen function */
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fopen_ret = (*original_fopen)(path, mode);


	/* add your code here */

	//Create file_logging.log
	FILE *log_file;
	log_file = (*original_fopen)("file_logging.log", "a");

	//If fwrite wants to open file_logging.log return original_fopen_ret to end fopen
	if (strcmp(path,"file_logging.log") == 0)
	{
		return original_fopen_ret;
	}
	
	//Get user id
	int uid = getuid();
	
	//srand(time(NULL)); 
	//int num = (rand() % (100 - 1 + 1)) + 1; 
	
	//Add random number to simulate different users
	//uid = uid + num;

	//Get file data & file length
	unsigned char *file_buffer;
	int file_length = 0;

	//assert(original_fopen_ret != NULL);

	//Calculate file hash
	unsigned char hash[32];
	memset(hash, 0x00, 32);
	
	//Handle Null original_fopen_ret pointer
	if (original_fopen_ret != NULL)
	{
		//Get file data & file length
	    fseek(original_fopen_ret, 0, SEEK_END);
	    file_length = ftell(original_fopen_ret);

	    fseek(original_fopen_ret, 0, SEEK_SET); 
	    file_buffer = (unsigned char*)calloc(file_length, sizeof(unsigned char));

		fread(file_buffer, 1, file_length, original_fopen_ret);

		calculate_hash((unsigned char *)file_buffer, hash, file_length);
	}

	/*
	//Log info to file simulating 6 different users
	for (int i = 0; i <= 5; i++)
	{
		log_to_file(log_file, uid + i, path, access_type, is_action_denied, hash);
	}
	*/

	char *abs_path = realpath(path, NULL);

	if (abs_path == NULL)
	{
		log_to_file(log_file, uid, path, access_type, is_action_denied, hash);
	}
	else
	{
		log_to_file(log_file, uid, abs_path, access_type, is_action_denied, hash);

		free(abs_path);
	}

	fclose(log_file);
	
	return original_fopen_ret;

}


size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 
{

	size_t original_fwrite_ret;
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);

	//Find file path from file pointer using file descriptor
	int fd;
  	char fd_path[255];
  	char *path = malloc(255);
  	size_t n;

  	fd = fileno(stream);
  	sprintf(fd_path, "/proc/self/fd/%d", fd);
  	n = readlink(fd_path, path, 255);
  	path[n] = '\0';

  	//Strip path to get only filename
  	//char *filename;
  	//filename = basename(path);

	//Decide access type
	int access_type;

	if (access(path, F_OK) == 0) 
	{
    	access_type = 2;
	} 

	//Decide if action is denied
	int is_action_denied = 1;

	if (access(path, W_OK) == 0)
	{
		is_action_denied = 0;
	}
	else
	{
		is_action_denied = 1;
	}		

	/* call the original fwrite function */
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
	original_fwrite_ret = (*original_fwrite)(ptr, size, nmemb, stream);


	/* add your code here */

	//Create file_logging.log
	FILE *log_file;
	log_file = fopen("file_logging.log", "a");

	//Get user id
	int uid = getuid();
	
	//srand(time(NULL)); 
	//int num = (rand() % (100 - 1 + 1)) + 1; 
	
	//Add random number to simulate different users
	//uid = uid + num;

	//Get file data & file length
	//unsigned char *file_buffer;
	//int file_length = 0;

	//assert(original_fwrite_ret != 0);

	//Calculate file hash
	unsigned char hash[32];
	memset(hash, 0x00, 32);

	//Handle Null original_fwrite_ret pointer
	if (original_fwrite_ret != 0)
	{
	    //fseek(stream, 0, SEEK_END);
	    //file_length = ftell(stream);
	    
	    //fseek(stream, 0, SEEK_SET); 
	    //file_buffer = (unsigned char*)calloc(file_length, sizeof(unsigned char));
		
		//fread(file_buffer, 1, file_length, stream);

		calculate_hash((unsigned char *)ptr, hash, size);
	}
	else
	{
		is_action_denied = 1;
	}

	/*
	//Log info to file simulating 6 different users
	for (int i = 0; i <= 5; i++)
	{
		log_to_file(log_file, uid + i, filename, access_type, is_action_denied, hash);
	}
	*/

	log_to_file(log_file, uid, path, access_type, is_action_denied, hash);

	fclose(log_file);
	
	return original_fwrite_ret;

}

void log_to_file(FILE *fp, int uid, const char *path, int access_type, int is_action_denied, unsigned char *hash)
{

	time_t t = time(NULL);
  	struct tm tm;

  	tm = *localtime(&t);

  	fprintf(fp, "%d\t%s\t%02d/%02d/%d\t%02d:%02d:%02d\t%d\t%d\t", uid, path, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, access_type, is_action_denied);

  	for(int i = 0; i < 16; ++i)
  	{
  	    fprintf(fp, "%02x", (unsigned int)hash[i]);
  	}

  	fprintf(fp, "\n");

  	rewind(fp);

}

void calculate_hash(unsigned char * file_buffer, unsigned char *hash, int file_length)
{

	MD5_CTX md5_ctx;

	MD5_Init(&md5_ctx);

	MD5_Update(&md5_ctx, file_buffer, file_length);

	MD5_Final(hash, &md5_ctx);

}


