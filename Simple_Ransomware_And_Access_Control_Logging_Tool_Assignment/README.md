+-----------------------------------------------------------------------+
|     ╔═══╗                ╔═══╗     ╔╗     ╔╗  ╔═╗╔═╗      ╔╗          |
|     ║╔═╗║                ║╔═╗║    ╔╝╚╗    ║║  ║║╚╝║║     ╔╝╚╗         |
|     ║║ ║╠══╦══╦══╦══╦══╗ ║║ ╚╬══╦═╬╗╔╬═╦══╣║  ║╔╗╔╗╠══╦═╗╠╗╔╬══╦═╗    |
|     ║╚═╝║╔═╣╔═╣║═╣══╣══╣ ║║ ╔╣╔╗║╔╗╣║║╔╣╔╗║║  ║║║║║║╔╗║╔╗╬╣║║╔╗║╔╝    |
|     ║╔═╗║╚═╣╚═╣║═╬══╠══║ ║╚═╝║╚╝║║║║╚╣║║╚╝║╚╗ ║║║║║║╚╝║║║║║╚╣╚╝║║     |
|     ╚╝ ╚╩══╩══╩══╩══╩══╝ ╚═══╩══╩╝╚╩═╩╝╚══╩═╝ ╚╝╚╝╚╩══╩╝╚╩╩═╩══╩╝     |
+-----------------------------------------------------------------------+
|         ╔═══╗      ╔╗        ╔═══╗                                    |
|         ║╔═╗║      ║║        ║╔═╗║                                    |
|         ║╚══╦╦╗╔╦══╣║╔══╗    ║╚═╝╠══╦═╗╔══╦══╦╗╔╦╗╔╗╔╦══╦═╦══╗        |
|         ╚══╗╠╣╚╝║╔╗║║║║═╣    ║╔╗╔╣╔╗║╔╗╣══╣╔╗║╚╝║╚╝╚╝║╔╗║╔╣║═╣        |
|         ║╚═╝║║║║║╚╝║╚╣║═╣    ║║║╚╣╔╗║║║╠══║╚╝║║║╠╗╔╗╔╣╔╗║║║║═╣        |
|         ╚═══╩╩╩╩╣╔═╩═╩══╝    ╚╝╚═╩╝╚╩╝╚╩══╩══╩╩╩╝╚╝╚╝╚╝╚╩╝╚══╝        |
+-----------------╚╝----------------------------------------------------+


/*This is a readme file to explain the implemetation of the given assignment.*/

In this assignment we were asked to develop a Ransomware in Bash script and to enrich the Access Control Monitor in order to detect it.

More specifically, the ransomware creates X files and decrypts them.

Then the acmonitor finds out if X files at least were created in the last 20 minutes. Also it finds and prints the name and the path of the original files that were encrypted and deleted by the ransomware.

In order to create X files and log this act, the ransomware calls test_ransomware program. Also test_ransomware creates 100 files, reads the files contents, encrypts them and stores them in a proper file with suffix ".encrypt".

Ransomware gets as argument a directory path, not a directory name. This was done because when there are 2 directories with the same name in the system, we don't know which directory is the right one. Also when providing directory path with spaces as argument, please do this in the following form: /home/username/Documents/Dir_Name1/Dir_Name2\ with\ spaces/

Important note: Inside ransomware.sh in the comment section, there is an implementation that gets as argument the directory name instead of the directory path. This will work only if the directory name has no spaces in it and there's no other directory in the system with the same name.

Useful Commands:
-make
- ./ransomware.sh <Path or Directory name> <Number of files>
- ./acmonitor -v <Number of files>
-./acmonitor -e

file_logging.log is saved in the project directory.
