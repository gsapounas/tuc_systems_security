#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "simple_crypto.h"

char* read_string()//reads a string of variable length and returns a pointer to it
{
	size_t length = 0;
	char *input = malloc(1);
	assert(input); 
	int c;

	while((c = getchar()) != '\n' && c != EOF)
	{
  	 char *t = realloc(input, length+1);
     assert(t); 
  	 input = t;
  	 input[length++] = c;
	}
char *t = realloc(input, length+1);
assert(t); 
input = t;
input[length++] = '\0';

return input;

}

int otp_encryption_decryption(char a, char o_key)
{
  char c = a ^ o_key;
  return (int)c;
}

int caesars_encryption_decryption(int c, int c_key)
{

	if((c >= '0' && c <= '9'))
		{
			c = c + c_key;

			if(c > '9')
			{
				c = c - '9' + 'A' - 1;

				if(c > 'Z')
				{

					c = c - 'Z' + 'a' - 1;

					if(c > 'z')
					{
						c = c - 'z' + '0' - 1;
					}
				}
			}
			
		}
		else if(c >= 'A' && c <= 'Z')
		{
			c = c + c_key;
			
			if(c > 'Z')
			{

				c = c - 'Z' + 'a' - 1;

				if(c > 'z')
				{
					c = c - 'z' + '0' - 1;

					if(c > '9')
					{
						c = c - '9' + 'A' - 1;
					}
				}
			}
			
		}
		else if(c >= 'a' && c <= 'z')
		{
			c = c + c_key;
			
			if(c > 'z')
			{
				//printf("%d\n",ch);
				c = c - 'z' + '0' - 1;

				if(c > '9')
				{
					c = c - '9' + 'A' - 1;

					if( c > 'Z')
					{

						c = c - 'Z' + 'a' - 1;

					}
				}
			}	
		}

	return c;

}

int vigeneres_encryption(char c, char vigeneres_key)
{
	char encrypted = ((c + vigeneres_key) % 26 ) + 'A';
	return (int)encrypted;

}

int vigeneres_decryption(char c, char vigeneres_key)
{
	char decrypted=(((c - vigeneres_key) + 26) % 26) + 'A';
	return (int)decrypted;

}