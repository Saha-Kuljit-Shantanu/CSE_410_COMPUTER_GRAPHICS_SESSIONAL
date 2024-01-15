#!/bin/bash

if [ "$#" -lt 1 ];
	then echo "Usage:"
	echo "./glshell.sh <Input Folder name>"
	
	
elif [ "$#" -lt 2 ];
	then g++ "$1" -o demo -lglut -lGLU -lGL
	./demo

else

	g++ "$1" -o "$2" -lglut -lGLU -lGL
	./"$2" 
	
fi


#chmod +x script_name
