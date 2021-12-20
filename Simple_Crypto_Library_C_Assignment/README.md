####################################
#──────────╔╗────────────────╔╗────#
#──────────║║───────────────╔╝╚╗───#
#╔══╦╦╗╔╦══╣║╔══╗╔══╦═╦╗─╔╦═╩╗╔╬══╗#
#║══╬╣╚╝║╔╗║║║║═╣║╔═╣╔╣║─║║╔╗║║║╔╗║#
#╠══║║║║║╚╝║╚╣║═╣║╚═╣║║╚═╝║╚╝║╚╣╚╝║#
#╚══╩╩╩╩╣╔═╩═╩══╝╚══╩╝╚═╗╔╣╔═╩═╩══╝#
#───────║║────────────╔═╝║║║───────#
#───────╚╝────────────╚══╝╚╝───────#
####################################

/*This file exists to explain the implemetation of the given assignment.*/

simple_crypto.c: Contains 4 functions (read_string, otp_encryption_decryption, 
caesars_encryption_decryption, vigeneres_encryption and vigeneres_decryption)

  -read_string: is a function to read unlimited characters from the terminal 
   character by character. Returns a pointer to a string
  -otp_encryption_decryption: is a function to encrypt and decrypt the given 
   string with the given key, character by character. Returns a character.
  -caesars_encryption_decryption: is a function to encrypt and decrypt the 
   given string with the given key, character by character. Returns a character
  -vigeneres_encryption: is a function to encrypt the given string with the 
   given key, character by character.
  -vigeneres_decryption: is a function to decrypt the given string with the 
   given key, character by character.  


simple_crypto.h: Contains the declarations of the functions implemented in 
simple_crypto.c

demo.c: Is the main c program that interacts with the user through the terminal, 
calls the functions implemented in the simple_crypto.c to make the wanted 
calculations (encryption and decryption) and prints the output in the wanted 
format.

Makefile: Is a script that compiles the assets and gives the executable.

How to compile and run:

  -Open a linux terminal in the project directory.
  -Type "make" command.
  -Run executable by typing "./demo"
  -Enjoy!
