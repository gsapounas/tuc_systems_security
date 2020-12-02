#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/cmac.h>

#define BLOCK_SIZE 16 


/*
#########################################
#──────────────────╔╗──╔╗──────╔╗╔╗──╔╗─#
#──────────────────║║─╔╝╚╗─────║║║║─╔╝╚╗#
#╔══╦══╦══╦═╗╔══╦══╣║─╚╗╔╬══╦══╣║║║╔╬╗╔╝#
#║╔╗║╔╗║║═╣╔╗╣══╣══╣║──║║║╔╗║╔╗║║║╚╝╬╣║─#
#║╚╝║╚╝║║═╣║║╠══╠══║╚╗─║╚╣╚╝║╚╝║╚╣╔╗╣║╚╗#
#╚══╣╔═╩══╩╝╚╩══╩══╩═╝─╚═╩══╩══╩═╩╝╚╩╩═╝#
#───║║──────────────────────────────────#
#───╚╝──────────────────────────────────#
#########################################
*/



/* function prototypes */
void print_hex(unsigned char *, size_t);
void print_string(unsigned char *, size_t); 
void usage(void);
void check_args(char *, char *, unsigned char *, int, int);
void keygen(unsigned char *, unsigned char *, unsigned char *, int);
int encrypt(unsigned char *, int, unsigned char *, unsigned char *, unsigned char *, int);
int decrypt(unsigned char *, int, unsigned char *, unsigned char *, unsigned char *, int);
void gen_cmac(unsigned char *, size_t, unsigned char *, unsigned char *, int);
int verify_cmac(unsigned char *, unsigned char *);



/* TODO Declare your function prototypes here... */
unsigned char* read_from_file(char *);
int get_file_length(char *);
void write_plaintext_to_file(char *, unsigned char *, int, char *);
void write_cipher_to_file(char *, unsigned char *, int, char *);

/*
 * Prints the hex value of the input
 * 16 values per line
 */
void print_hex(unsigned char *data, size_t len)
{
	size_t i;

	if (!data)
		printf("NULL data\n");
	else {
		for (i = 0; i < len; i++) {
			if (!(i % 16) && (i != 0))
				printf("\n");
			printf("%02X ", data[i]);
		}
		printf("\n");
	}
}


/*
 * Prints the input as string
 */
void print_string(unsigned char *data, size_t len)
{
	size_t i;

	if (!data)
		printf("NULL data\n");
	else {
		for (i = 0; i < len; i++)
			printf("%c", data[i]);
		printf("\n");
	}
}


/*
 * Prints the usage message
 * Describe the usage of the new arguments you introduce
 */
void usage(void)
{
	printf(
	    "\n"
	    "Usage:\n"
	    "    assign_1 -i in_file -o out_file -p passwd -b bits" 
	        " [-d | -e | -s | -v]\n"
	    "    assign_1 -h\n"
	);
	printf(
	    "\n"
	    "Options:\n"
	    " -i    path    Path to input file\n"
	    " -o    path    Path to output file\n"
	    " -p    psswd   Password for key generation\n"
	    " -b    bits    Bit mode (128 or 256 only)\n"
	    " -d            Decrypt input and store results to output\n"
	    " -e            Encrypt input and store results to output\n"
	    " -s            Encrypt+sign input and store results to output\n"
	    " -v            Decrypt+verify input and store results to output\n"
	    " -h            This help message\n"
	);
	exit(EXIT_FAILURE);
}


/*
 * Checks the validity of the arguments
 * Check the new arguments you introduce
 */
void check_args(char *input_file, char *output_file, unsigned char *password, int bit_mode, int op_mode)
{
	if (!input_file) {
		printf("Error: No input file!\n");
		usage();
	}

	if (!output_file) {
		printf("Error: No output file!\n");
		usage();
	}

	if (!password) {
		printf("Error: No user key!\n");
		usage();
	}

	if ((bit_mode != 128) && (bit_mode != 256)) {
		printf("Error: Bit Mode <%d> is invalid!\n", bit_mode);
		usage();
	}

	if (op_mode == -1) {
		printf("Error: No mode\n");
		usage();
	}
}


/*
 * Generates a key using a password
 */
void keygen(unsigned char *password, unsigned char *key, unsigned char *iv, int bit_mode)
{

	/* TODO Task A */

	//unsigned int key_len = bit_mode / 8;
	int password_length = strlen((char*)password);


	if (bit_mode == 128)
	{

		EVP_BytesToKey(EVP_aes_128_ecb(), EVP_sha1(), NULL, password, password_length, 1, key, iv);

	}
	else
	{
		
		EVP_BytesToKey(EVP_aes_256_ecb(), EVP_sha1(), NULL, password, password_length, 1, key, iv);
	
	}
	


}


/*
 * Encrypts the data
 */
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext, int bit_mode)
{

	/* TODO Task B */

    int len;
	int ciphertext_len;

	EVP_CIPHER_CTX *cipher_ctx;
	cipher_ctx = EVP_CIPHER_CTX_new();
    
    if (bit_mode == 128)
    {
      EVP_EncryptInit_ex(cipher_ctx, EVP_aes_128_ecb(), NULL, key, iv);      
    }
    else
    {
      EVP_EncryptInit_ex(cipher_ctx, EVP_aes_256_ecb(), NULL, key, iv);
    }

    EVP_EncryptUpdate(cipher_ctx, ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len = len;

    EVP_EncryptFinal_ex(cipher_ctx, ciphertext + len, &len);
    ciphertext_len = ciphertext_len + len;

    EVP_CIPHER_CTX_free(cipher_ctx);

	return ciphertext_len;
}


/*
 * Decrypts the data and returns the plaintext size
 */
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext, int bit_mode)
{
	/*TODO Task C */

	EVP_CIPHER_CTX *cipher_ctx;
	cipher_ctx = EVP_CIPHER_CTX_new();

    int len;
	int plaintext_len = 0;

    if (bit_mode == 128)
    {
      EVP_DecryptInit_ex(cipher_ctx, EVP_aes_128_ecb(), NULL, key, iv);
    }
    else
    {
      EVP_DecryptInit_ex(cipher_ctx, EVP_aes_256_ecb(), NULL, key, iv);
    }

    EVP_DecryptUpdate(cipher_ctx, plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len = len;

    EVP_DecryptFinal_ex(cipher_ctx, plaintext + len, &len);
    plaintext_len = plaintext_len + len;

    EVP_CIPHER_CTX_free(cipher_ctx);

	return plaintext_len;
}


/*
 * Generates a CMAC
 */
void gen_cmac(unsigned char *data, size_t data_len, unsigned char *key, unsigned char *cmac, int bit_mode)
{

	/* TODO Task D */

	size_t cmac_len = 16;

	CMAC_CTX *cmac_ctx;
	cmac_ctx = CMAC_CTX_new();


	if (bit_mode == 128)
	{
		CMAC_Init(cmac_ctx, key, 16, EVP_aes_128_ecb(), NULL);	
	}	
	else
	{
		CMAC_Init(cmac_ctx, key, 32, EVP_aes_256_ecb(), NULL);		
	}
	
	CMAC_Update(cmac_ctx, data, data_len);

	CMAC_Final(cmac_ctx, cmac, &cmac_len);

	CMAC_CTX_free(cmac_ctx);

	//print_hex(cmac, 16);


}


/*
 * Verifies a CMAC
 */
int verify_cmac(unsigned char *cmac1, unsigned char *cmac2)
{
	int verify;
	verify = - 100;

	/* TODO Task E */

	verify = memcmp(cmac1, cmac2, 16);								//verify returns 0 if cmac1 is equal to cmac2

	//printf("\nverification =%d \n", verify);

	return verify;
}



/* TODO Develop your functions here... */

unsigned char* read_from_file(char *input_file) 
{
	FILE *fp;
    unsigned char *buffer = NULL;
    size_t len;

    fp = fopen(input_file, "rb+");
    if (fp == NULL)  
    {
     // abort if file cannot be opened
     printf("Cannot open file\n");
     exit(-1);
    }
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET); 
    buffer = (unsigned char*)calloc(len, sizeof(unsigned char));
    if (NULL == buffer)
    {
        // Handle malloc failure & exit
        exit(1);
    }

    if (fread(buffer, 1, len, fp))
    {
    	fclose(fp);
	    //buffer[len] = '\0';
	    //printf("%s",buffer);
	    //printf("%ld",len);

	    return buffer;

	    free(buffer);
    	buffer = NULL;
    }
    else
    {
    	//Handle fread failure & exit
    	exit(1);
    }

    // buffer now contains the content of your file; do what you want with it
    
}

int get_file_length(char *input_file) 
{
	FILE *fp;
    size_t len;

    fp = fopen(input_file, "rb+");
    if (fp == NULL)  
    {
     // abort if file cannot be opened
     printf("Cannot open file");
     exit(1);
    }
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET); 

   
    fclose(fp);
	  
	return len;

    // buffer now contains the content of your file; do what you want with it
    
}

void write_cipher_to_file(char *output_file, unsigned char *ciphertext, int ciphertext_len, char *write_file_mode)
{

	FILE *fp = fopen(output_file, write_file_mode);
	
	
	if(fp == NULL)
	{
	    printf("Cannot open file\n");
	    exit(1);
	}

	// Write Buffer
	fwrite(ciphertext, 1, ciphertext_len, fp);
	// Close File
	fclose(fp);
	fp = NULL;

}

void write_plaintext_to_file(char *output_file, unsigned char *plaintext, int plaintext_len, char *write_file_mode)
{

	FILE *fp = fopen(output_file, write_file_mode);
	//printf("plaintext_len=%d",plaintext_len);
	
	if(fp == NULL)
	{
	    printf("Cannot open file\n");
	    exit(1);
	}

	// Write Buffer
	fwrite(plaintext, 1, plaintext_len, fp);
	// Close File
	fclose(fp);
	fp = NULL;

}


/*
 * Encrypts the input file and stores the ciphertext to the output file
 *
 * Decrypts the input file and stores the plaintext to the output file
 *
 * Encrypts and signs the input file and stores the ciphertext concatenated with 
 * the CMAC to the output file
 *
 * Decrypts and verifies the input file and stores the plaintext to the output
 * file
 */
int main(int argc, char **argv)
{
	int opt;			/* used for command line arguments */
	int bit_mode;			/* defines the key-size 128 or 256 */
	int op_mode;			/* operation mode */
	char *input_file;		/* path to the input file */
	char *output_file;		/* path to the output file */
	unsigned char *password;	/* the user defined password */


	unsigned char *file_buffer;
	char *write_file_mode;

	/* Init arguments */
	input_file = NULL;
	output_file = NULL;
	password = NULL;
	bit_mode = -1;
	op_mode = -1;




	/*
	 * Get arguments
	 */
	while ((opt = getopt(argc, argv, "b:i:m:o:p:desvh:")) != -1) {
		switch (opt) {
		case 'b':
			bit_mode = atoi(optarg);
			break;
		case 'i':
			input_file = strdup(optarg);
			break;
		case 'o':
			output_file = strdup(optarg);
			break;
		case 'p':
			password = (unsigned char *)strdup(optarg);
			break;
		case 'd':
			/* if op_mode == 1 the tool decrypts */
			op_mode = 1;
			break;
		case 'e':
			/* if op_mode == 0 the tool encrypts */
			op_mode = 0;
			break;
		case 's':
			/* if op_mode == 2 the tool signs */
			op_mode = 2;
			break;
		case 'v':
			/* if op_mode == 3 the tool verifies */
			op_mode = 3;
			break;
		case 'h':
		default:
			usage();
		}
	}


	/* check arguments */
	check_args(input_file, output_file, password, bit_mode, op_mode);



	/* TODO Develop the logic of your tool here... */

	/* INITIALIZE THE LIBRARY */

	/* Initialize IV */
	unsigned char iv[16];
    memset(iv, 0x00, 16);

    /* Initialize the KEY */
    unsigned char key[bit_mode/8 + 1];
    memset(key, 0x00, bit_mode/8 + 1);

    /* Initialize the CMAC */
    unsigned char cmac[16];
    memset(cmac, 0x00, 16);

    //print_hex(iv, BLOCK_SIZE);
    //print_string(iv, BLOCK_SIZE);
    //print_hex(password, BLOCK_SIZE);
	//print_string(password, BLOCK_SIZE);
    
	int decryptedtext_len = 0;
    int ciphertext_len = 0;
    int file_length = 0;

	/* Operate on the data according to the mode */
	/* encrypt */
	if (op_mode == 0)
	{
		/* Keygen from password */
		file_buffer = read_from_file(input_file);
		file_length = get_file_length(input_file);

		unsigned char ciphertext[file_length];

		keygen(password, key, iv, bit_mode);

		//printf("Key is:\n");
		//print_hex(key,bit_mode/8);

		ciphertext_len = encrypt(file_buffer, file_length, key, iv, ciphertext, bit_mode);
		
		//printf("Ciphertext is:\n");
   		//print_hex(ciphertext, ciphertext_len);

   		write_file_mode = "wb+";
   		write_cipher_to_file(output_file, ciphertext, ciphertext_len, write_file_mode);
   		
   		//printf("%d\n", ciphertext_len);
	}

	/* decrypt */
	if (op_mode == 1)
	{	
		file_buffer = read_from_file(input_file);
		file_length = get_file_length(input_file);

		unsigned char decryptedtext[file_length];

		/* Keygen from password */
		keygen(password, key, iv, bit_mode);

		//printf("Key is:\n");
		//print_hex(key,bit_mode/8);

		decryptedtext_len = decrypt(file_buffer, file_length, key, iv, decryptedtext, bit_mode);
		 /* Add a NULL terminator. We are expecting printable text */
   		decryptedtext[decryptedtext_len] = '\0';
   		
   		//printf("Ciphertext is:\n");
		//print_hex(file_buffer, file_length);

		write_file_mode = "wb+";
		write_plaintext_to_file(output_file, decryptedtext, decryptedtext_len, write_file_mode);
    	
    	//printf("%d\n", decryptedtext_len);
	}
	/* sign */
	if (op_mode == 2)
	{
		file_buffer = read_from_file(input_file);
		file_length = get_file_length(input_file);

		unsigned char ciphertext[file_length];

		keygen(password, key, iv, bit_mode);

		ciphertext_len = encrypt(file_buffer, file_length, key, iv, ciphertext, bit_mode);

		gen_cmac(file_buffer, file_length, key, cmac, bit_mode);

		//print_hex(ciphertext, ciphertext_len);

		//printf("\n");

		//print_hex(cmac,16);

		//printf("\n");

		unsigned char cipher_cmac_concat[ciphertext_len + 16];

		memcpy(cipher_cmac_concat, ciphertext, ciphertext_len);
		memcpy(cipher_cmac_concat + ciphertext_len, cmac, 16);

		//print_hex(cipher_cmac_concat, ciphertext_len + 16);

		write_file_mode = "wb+";
   		write_cipher_to_file(output_file, cipher_cmac_concat, sizeof(cipher_cmac_concat), write_file_mode);

	}
	/* verify */
	if (op_mode == 3)
	{
		file_buffer = read_from_file(input_file);
		file_length = get_file_length(input_file);

		//print_hex(file_buffer, file_length);

		unsigned char ciphertext[file_length - 16];
		unsigned char decryptedtext[file_length - 16];

		unsigned char cmac_obtained[16];

		memcpy(cmac_obtained, file_buffer + file_length - 16, 16);
		memcpy(ciphertext, file_buffer, file_length - 16);

		//printf("Obtained cipher:\n");
		//print_hex(ciphertext, file_length - 16);

		//printf("Obtained cmac:\n");
		//print_hex(cmac_obtained, 16);



		keygen(password, key, iv, bit_mode);

		decryptedtext_len = decrypt(ciphertext, file_length - 16, key, iv, decryptedtext, bit_mode);
		 /* Add a NULL terminator. We are expecting printable text */
   		//decryptedtext[decryptedtext_len] = '\0';
   		
   		//print_string(decryptedtext, decryptedtext_len);
   		

		gen_cmac(decryptedtext, decryptedtext_len, key, cmac, bit_mode);

		//printf("Generated cmac:\n");
		//print_hex(cmac, 16);

		//printf("Generated key:\n");
		//print_hex(key, bit_mode/8);



		int verify = verify_cmac(cmac_obtained, cmac);

		//unsigned char verification_message[25];
		//unsigned char decrypted_verification_message_concat[decryptedtext_len + 25];

		
		if (verify == 0)
		{
			printf("Message verified.\n");
		}
		else
		{
			printf("Message not verified.\n");
		}

		
		//memcpy(decrypted_verification_message_concat, decryptedtext, decryptedtext_len);
		//memcpy(decrypted_verification_message_concat + decryptedtext_len, verification_message, 22);

		write_file_mode = "wb+";
   		write_cipher_to_file(output_file, decryptedtext, decryptedtext_len, write_file_mode);
		
				
	}	

	/* Clean up */
	free(input_file);
	free(output_file);
	free(password);

	EVP_cleanup();


	/* END */
	return 0;
}
