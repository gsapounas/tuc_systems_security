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

/*This file exists to explain the implemetation of the given assignment.*/

In this assignment we were asked to develop an openssl toolkit using the 
appropriate functions from the EVP and CMAC API.

More specifically, this tool encrypts, decrypts, signs and verifies the given 
files by the user.

To do this, appropriate functions are designed and implemented.

-Read_from_file: This is a function that reads the given file and stores it to 
 a buffer for further processing.
-Get_file_length: This is a function that calculates the file's data size so we 
 can know how much size to allocate for the buffer that stores the data for 
 further processing.
-Write_cipher_to_file: This is a function that writes the ciphertext to file.
-Write_plaintext_to_file: This is a function that writes the plaintext to file.
-Keygen: This is a function that gets a password from the user and generates the 
 appropriate key using the EVP API from OPENSSL.
-Encrypt: This is a function that encrypts the given data using the EVP API from 
 OPENSSL.
-Decrypt: This is a function that decrypts the given data using the EVP API from 
 OPENSSL.
-Gen_cmac: This is a function that generates a CMAC to sign the given data with 
 the key generated from Keygen function, using the EVP and CMAC APIS.
-Verify_cmac: This is a function that verifies that the obtained CMAC from a 
 given file is equal to the generated CMAC of the decrypted text contained in 
 the given file. If these CMACS aren't equal, then the file isn't verified.

Answer to Task F4:
None of the given files was verified. That's because the CMAC of the given files 
was generated in AES_CBC mode, where our CMAC had to be generated in AES_ECB 
mode.

To compile and run:

-Open a linux terminal in the project directory.
-Type "make" command.
-Run executable by copy/paste commands from commands.txt
-Enjoy!
