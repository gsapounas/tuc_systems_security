#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>

#include "simple_crypto.h"

int main()
{
printf("\n");
printf("####################################\n");
printf("#──────────╔╗────────────────╔╗────#\n");
printf("#──────────║║───────────────╔╝╚╗───#\n");
printf("#╔══╦╦╗╔╦══╣║╔══╗╔══╦═╦╗─╔╦═╩╗╔╬══╗#\n");
printf("#║══╬╣╚╝║╔╗║║║║═╣║╔═╣╔╣║─║║╔╗║║║╔╗║#\n");
printf("#╠══║║║║║╚╝║╚╣║═╣║╚═╣║║╚═╝║╚╝║╚╣╚╝║#\n");
printf("#╚══╩╩╩╩╣╔═╩═╩══╝╚══╩╝╚═╗╔╣╔═╩═╩══╝#\n");
printf("#───────║║────────────╔═╝║║║───────#\n");
printf("#───────╚╝────────────╚══╝╚╝───────#\n");
printf("####################################\n");
printf("\n");
//----------------------------------------------------------------------------------------------------------
//OTP ENCRYPTION

  srand(time(NULL));

  int i;
  
  char* otp_input;

  printf("[OTP] input: ");
  
  otp_input = read_string();
  

  size_t otp_inputlength = strlen(otp_input);
  


  size_t otp_keysize = rand() % (otp_inputlength+1)+otp_inputlength;

  

  char otp_key[otp_keysize];
  char otp_cipher[otp_inputlength], otp_decrypted[otp_inputlength];

  

  FILE *fp;
  fp = fopen("/dev/urandom", "r");

  

  fread(&otp_key, 1, otp_keysize, fp);

  fclose(fp); 

  printf("[OTP] encrypted: ");

  for(i=0; i < otp_inputlength; i++)
  {
  
    otp_cipher[i] = (char)otp_encryption_decryption(otp_key[i], otp_input[i]);  
    printf("%x", (otp_cipher[i])); //prints encrypted message in HEX because we have a problem with non printable characters
    
  }
  otp_cipher[i] = '\0';

  

  printf("\n");

//----------------------------------------------------------------------------------------------------------
//OTP DECRYPTION


  int otp_cipherlength = strlen(otp_cipher);

  printf("[OTP] decrypted: ");
  
  for(i=0; i < otp_inputlength; i++)
  {

    otp_decrypted[i] = (char)otp_encryption_decryption(otp_cipher[i], otp_key[i]);
    printf("%c", (otp_decrypted[i])); 
  }
  otp_decrypted[i] = '\0';

  printf("\n");

  free(otp_input);

//----------------------------------------------------------------------
//CAESARS ENCRYPTION

  
  char* caesars_input;
  

  int ch;
  size_t caesars_key;
  
  printf("[Caesars] input: ");
  caesars_input = read_string();

  

  printf("[Caesars] key: ");
  scanf("%ld", &caesars_key);

  while ((getchar()) != '\n'); //Line of code to consume \n, to read with fgets in Vigeneres Cipher without problems

  size_t caesars_inputlength = strlen(caesars_input);

  size_t caesars_cipherlength;

  char caesars_cipher[caesars_inputlength], caesars_decrypted[caesars_inputlength];
  

  
  
  caesars_key = caesars_key % 62;

  for(i=0; i < caesars_inputlength; i++)
  {
    caesars_cipher[i] = caesars_encryption_decryption(caesars_input[i], caesars_key);
  }

  caesars_cipher[i+1] = '\0';

  caesars_cipherlength = strlen(caesars_cipher);

  

  printf("[Caesars] encrypted: ");


  for(i=0; i < caesars_inputlength; i++)
  {
    printf("%c", caesars_cipher[i]);
  }
  
  printf("\n");

//----------------------------------------------------------------------
//CAESARS DECRYPTION

  caesars_key = caesars_key % 62;
  
  caesars_key = 62 - caesars_key ;

  for(i=0; i < caesars_inputlength; i++)
  {
    caesars_decrypted[i] = caesars_encryption_decryption(caesars_cipher[i], caesars_key);
  }

  printf("[Caesars] decrypted: ");

  for(i=0; i < caesars_inputlength; i++)
  {
    printf("%c", caesars_decrypted[i]);
  }
  


  free(caesars_input);
  printf("\n");

//----------------------------------------------------------------------
//VIGENERES ENCRYPTION

  char* vigeneres_input;
  char* vigeneres_key;

    
  printf("[Vigeneres] input: ");
  
  vigeneres_input = read_string();
  

  printf("[Vigeneres] key: ");
  vigeneres_key = read_string();

  


  size_t vigeneres_inputlength = (strlen(vigeneres_input));
  size_t vigeneres_keylength = (strlen(vigeneres_key));
  int j;

  char vigeneres_newkey[vigeneres_inputlength];
  char vigeneres_encrypted[vigeneres_inputlength];
  char vigeneres_decrypted[vigeneres_inputlength];


  //Making the new key the same length as the input length
  for(i = 0, j = 0; i < vigeneres_inputlength; i++, j++)
  {
      if(j == vigeneres_keylength)
      {
          j = 0;
      }
 
      vigeneres_newkey[i] = vigeneres_key[j];
  }
 
  vigeneres_newkey[i] = '\0';

  for(i = 0; i < vigeneres_inputlength; i++)
  {
      vigeneres_encrypted[i] = (char)vigeneres_encryption(vigeneres_input[i], vigeneres_newkey[i]);
  }

  vigeneres_encrypted[i] = '\0';
  
  
  printf("[Vigeneres] encrypted: %s\n", vigeneres_encrypted);

//----------------------------------------------------------------------
//VIGENERES DECRYPTION

  for(i = 0; i < vigeneres_inputlength; i++)
  {
      vigeneres_decrypted[i] = (char)vigeneres_decryption(vigeneres_encrypted[i], vigeneres_newkey[i]);
  }
 
  vigeneres_decrypted[i] = '\0';
  
  printf("[Vigeneres] decrypted: %s\n", vigeneres_decrypted);
 

  free(vigeneres_input);
  free(vigeneres_key);

  return 0;

}
