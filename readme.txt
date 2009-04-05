How to Compile in Windows
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Install Qt 4.x
Change directory to estimo
run qmake -t vcapp
Open estimo.vcproj



Manually Calling bison/flex
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@echo OFF
call byacc -d -p estimo estimo.y
del estimo_yacc.h estimo_yacc.cpp 
move estimo.tab.h estimo_yacc.h 
move estimo.tab.c estimo_yacc.cpp
@echo ON
