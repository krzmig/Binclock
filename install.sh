#!/bin/sh
command -v g++ > /dev/null 2>&1 || { echo >&2 "I require g++ but it's not installed. Aborting."; exit 1; }
echo "Compilation...";
g++ ./binclock.cpp -o binclock
if [ -h "/usr/local/bin/binclock" ]
then
	echo "Link to program is exist."
else
	if ln -sr ./binclock /usr/local/bin/binclock
	then
		echo "Link to program created."
	else
		echo "No write permission to make symbolic link... run this installer as root."
	fi
fi
echo "Hey, it's done here!";
