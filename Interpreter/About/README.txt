Project is interpretator of artifical programming language realizing.
In this folder you can see 5 programs which demonstrate this interpretator work.
Interpretator output processing of each operation

STRUCTURE:
inter.y - grammatic descrition;
token.l - lexic tokens description;
(We use them for generating lexical and gramatical analysers);
gram_proc.cpp - implements the construction unparsed program of our artifical language on c++;
inter.cpp - all program constructions realizing (for example - cycles);
robot.cpp - file for realizing robot actions: XRAY(wall existing scan) and go <direction>;
map.txt - robot world description;
program.txt - this program text interpretator execute. You can copy examples text is program.txt for test it.

EXAMPLES:

Syntax.txt demonstrating syntax of my artifical language
In this example you can see base syntax of cycles, condition and other elementary operations.


UnrecFib.txt demonstrating unrecursion fibonacci
Fibonacci counting unrecursion algorithm execution


Robot.txt demonstrating robot escape from maze 10x10 (using depth search algorithm)
In file "map.txt" every 4 numbers code state of each maze cell: 
1010 - on top and left sides of cell are walls, on bottom and right - no
0011 - on top and bottom sides of cell are walls, on left and right - no
Robot start position is under desription of maze.
Robot should find out from maze.
If it is not impossible program return number cell in maze. Else - 101


Errors.txt demonstrating output errors
Interpretator can't execute program and returns message why.

RecFib.txt demonstration recursion fibonacci
Fibonacci counting recursion algorithm execution (if num >10 that very long)


USAGE
#in folder interpretator
#installing
sudo apt install bison 
sudo apt install flex

#analysers generating
bison -d inter.y
flex token.l

#build and run
g++ inter.tab.c inter.cpp gram_proc.cpp lex.yy.c robot.cpp
./a.out

