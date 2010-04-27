How to Compile in Windows
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Install: Qt 4.x
Change: directory to estimo
run: qmake -t vcapp
Open: estimo.vcproj


If for some reason visual studio issues an error while Yaccing... Open estimo.vcproj
and change y.tab.c y.tab.h to estimo.tab.c estimo.tab.h


Manually Calling bison/flex
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Install GNUwin32 utilities.

@echo OFF
flex -Pestimo estimo.l
del estimo_lex.cpp
move lex.estimo.c estimo_lex.cpp
call byacc -d -p estimo estimo.y
del estimo_yacc.h estimo_yacc.cpp 
move estimo.tab.h estimo_yacc.h 
move estimo.tab.c estimo_yacc.cpp
@echo ON
