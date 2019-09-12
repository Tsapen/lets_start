# This project is an interpretator for my artifical programming language.  
In this folder you can see all files needed to demostrate the abilities of it.  
The main idea of it is describing a robot, its actions and special features, that can help it find a way out of the maze.  
***
# STRUCTURE:
inter.y - grammar description;  
token.l - lexic tokens description;  
(they're used for lexical and gramatical analysers);  
gram_proc.cpp - implements the construction of the program of our artifical language on c++;  
inter.cpp - main file for the interpretator  
robot.cpp -  robot actions description: XRAY(wall existing scan) and go DIRECTION;  
map.txt - robot world description;  
program.txt - execute file. You can copy examples into program.txt to test the program.  
***
# EXAMPLES:  
  
Syntax.txt shows the syntax of my artifical language for cycle, condition and other basic operations.  
    
UnrecFib.txt contains the algorithm for nonrecursive fibonacci.  
RecFib.txt  contains the algorithm for recursive fibonacci.  
  
Robot.txt contains the algorithm of robot's escape from the maze 10x10 (using depth search algorithm)  
In file "map.txt" every 4-digit code states for each maze cell:   
1010 -  there are walls on the top and left sides of a cell, on the bottom and right - none  
0011 - there are walls on the  top and bottom sides of a cell, on the left and right - none  
Robot's start position is under the desription of maze.  
Robot should find the exit.  
If it's possible, program returns the number of the last cell in maze. Else - 101  

Errors.txt contains examples of input errors.  
The name and place of the mistake will be in the output after unsuccessful try to run the program.  
***
# USAGE  
Installing: 
sudo apt install bison  
sudo apt install flex  
  
Analysers generating:   
bison -d inter.y && flex token.l  
  
Build and run:  
g++ inter.tab.c inter.cpp gram_proc.cpp lex.yy.c robot.cpp  && ./a.out  