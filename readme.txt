How to Compile in Windows
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Install Qt 4.x
Change directory to estimo
run qmake -t vcapp
Open estimo.vcproj

Find Line    

CommandLine="byacc -d -p estimo estimo.y &amp;&amp; 	 del estimo_yacc.h estimo_yacc.cpp &amp;&amp; 	 move y.tab.h estimo_yacc.h &amp;&amp; 	 move y.tab.c estimo_yacc.cpp&#x0D;&#x0A;"
    
and replace it with

CommandLine="byacc -d -p estimo estimo.y &amp;&amp; 	 del estimo_yacc.h estimo_yacc.cpp &amp;&amp; 	 move estimo.tab.h estimo_yacc.h &amp;&amp; 	 move estimo.tab.c estimo_yacc.cpp&#x0D;&#x0A;"

we replaced y.tab  to estimo.tab



Manually Calling bison/flex
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@echo OFF
call byacc -d -p estimo estimo.y
del estimo_yacc.h estimo_yacc.cpp 
move estimo.tab.h estimo_yacc.h 
move estimo.tab.c estimo_yacc.cpp
@echo ON
