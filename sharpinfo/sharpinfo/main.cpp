/**************************************************************************
 *  sharpInfo -- decompiler for Estimo C Compiler
 **************************************************************************
 *  University of Bristol
 *  Department of Electrical and Electronic Engineering
 *
 *  Copyright (C) 2008 George Vafiadis  <vafiadis@ieee.org>
 *
 **************************************************************************
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software
 **************************************************************************
 */

#include <iostream>
#include <math.h>
#include <fstream>
#include <map>
#include <vector>
#include <string>
using namespace std;

#define ARG_PROGRAM "-pro"
#define ARG_MEMORY  "-mem"


enum E_PARTITIONS
{
	E_MODE_16x16 = 0,   
	E_MODE_8x16  = 1,
	E_MODE_16x8  = 2, 
	E_MODE_8x8   = 3   
};


#define SI_SUPPORT_IOSTREAMS
#include "SimpleIni.h"

void parseArg(char ** argv);

void infoProgram(char * filename);
void infoMemory(char * filename);
int convertToInt(std::string num);
float convertToFloat(std::string num);

// General
std::string version;
int instruction_bits;
int opcode_bits;
int coordinate_bits;

// Memory
int program_memory_size;
int point_memory_size;

// Reference Frame
int min_width;
int max_width;
int min_height;
int max_height;

// Registers
std::string COST;
std::string LENGTH;

std::string SAD; // deprecated
std::string Mx;  // deprecated
std::string My; // deprecated

// chk <start_pattern_address>,  <number_of_points>
std::string chk_opcode;
int     chk_start;
int     chk_num;

// chkfr <start_pattern_address>,  <number_of_points>
std::string chkfr_opcode;
int     chkfr_start;
int     chkfr_num;

// chkjmp <winning_point>, <afterwin_program_address>
std::string chkjmp_opcode;
int     chkjmp_win;
int     chkjmp_address;

// cmpl <register>, <number>
std::string cmpl_opcode;
int     cmpl_reg;
int     cmpl_num;

// cmpg <register>, <number>
std::string  cmpg_opcode;
int      cmpg_reg;
int      cmpg_num;

// cmpe <register>, <number>
std::string cmpe_opcode;
int     cmpe_reg;
int     cmpe_num;

// cmpne <register>, <number>
std::string cmpne_opcode;
int     cmpne_reg;
int     cmpne_num;

// jmp <program_address>
std::string jmp_opcode;
int     jmp_address;

// b <program_address>
std::string b_opcode;
int     b_address;

// halt
std::string halt_opcode;

/// Set MVC mode
std::string mvc_opcode;
int mvc_partition_mode;
int mvc_reference_frame;
int mvc_mvc;

void loadIni(std::string filename);
int str2int(std::string num);

string getBits(float num, const int size, bool fullpel)
{
  vector<bool> b(size);
  bool isNeg = false;

  for(int i = 0; i < size; ++i)
    b[i] = false;

  int n = static_cast<int>(num);

    if( num < 0 )
    {
        isNeg = true;
        n = -n;
    }

    if( !fullpel )
    {
        bool first, second;
        float frac = fabs(num) - n;

              if( frac == 0.00) { first = 0; second = 0; }
         else if( frac == 0.25) { first = 0; second = 1; }
         else if( frac == 0.50) { first = 1; second = 0; }
         else if( frac == 0.75) { first = 1; second = 1; }

         b[size-1] = second;
         b[size-2] = first;
    }

    int nd = size - (fullpel ? 0 : 2);
    do
    {
      b[--nd] = (n % 2 ? 1 : 0);
      n >>= 1;
    } while( n != 0 );

    if( isNeg )
    {
     int curry = 1;
     for(int i = size - 1; i >= 0; --i )
     {
      int digit = !b[i];
      b[i] = (digit != curry);
      curry  = digit * curry;

     }
    }


   cout << "B = ";
   for(int j = 0; j < size; ++j)
     cout << b[j];
   cout << endl;


 return "b";
}

int main(int argc, char **argv)
{
    cout << "SharpInfo: Decompiler for Estimo C" << endl;

	loadIni("configuration.ini");

    if( argc == 3 )
    {
      parseArg(argv);
    }
    else
    {

	  cout << "USAGE:  sharpinfo -pro|-mem  <input-file>" << endl;
      cout << "\t" << ARG_PROGRAM << "\tOutput Program Info" << endl;
      cout << "\t" << ARG_MEMORY << "\tOutput Point Memory Info" << endl << endl;
      return 0;
    }

    return 0;
}

void parseArg(char ** argv)
{
  std::string arg = argv[1];

  if( arg == ARG_PROGRAM )
  {
     infoProgram(argv[2]);
  }
  else if( arg == ARG_MEMORY )
  {
    infoMemory(argv[2]);
  }
  else
  {
    cout << "Error: Uknown input command line switch." << endl;
  }
}

std::string getRegisterName(std::string bits)
{
  if( bits == "0000" ) return "cost    ";
  if( bits == "0001" ) return "length  ";
}


void getInfo(string line)
{
 string opcode = line.substr(0, 4);   // 4 bits
 string arg1   = line.substr(4, 8);   // 8 bits
 string arg2   = line.substr(12, 8);  // 8 bits
 string arg12  = line.substr(4, 16);  // 16 bits
 string arg2_12bits = line.substr(8, 12); // 12 bits
 string regName = line.substr(4, 4);   // 4 bits

 if( opcode == halt_opcode && arg12 == "0000000000000000" )
 {
   cout << "halt" << endl;
   return;
 }

 if( opcode == chk_opcode )
 {
   cout << "chk      " << "NumPoints: " << convertToInt("0" + arg1) << "\tstartAddr: " << convertToInt("0" + arg2) << endl;
 }

 if( opcode == chkfr_opcode )
 {
   cout << "chkfr    " <<  "NumPoints: " << convertToInt("0" + arg1) << "\tstartAddr: " << convertToInt("0" + arg2) << endl;
 }

 if( opcode == chkjmp_opcode )
 {
   cout << "chkjmp   " << "WIN: " << convertToInt("0" + arg1)        << "\t\tgoto: " << convertToInt("0" + arg2) << endl;
 }

 if( opcode == b_opcode )
 {
   cout << "b        " << "goto: " << convertToInt("0" + arg12) << endl;
 }

 if( opcode == jmp_opcode )
 {
   cout << "jmp      " << "goto: " << convertToInt("0" + arg12) << endl;
 }

 // [Instruction: Compare if reg Less than number]  cmpl <register>, <number>
if( opcode == cmpl_opcode )
 {
   cout << "cmpl     " << getRegisterName(regName) << convertToInt("0" + arg2_12bits) << endl;
 }


//[Instruction: Compare if reg greater than number] ; cmpg <register>, <number>
if( opcode == cmpg_opcode )
 {
   cout << "cmpg     " << getRegisterName(regName) << convertToInt("0" + arg2_12bits) << endl;
 }

// [Instruction: Compare if reg is equal to number]  ; cmpe <register>, <number>
if( opcode == cmpe_opcode )
 {
   cout << "cmpe     " << getRegisterName(regName) << convertToInt("0" + arg2_12bits) << endl;
 }


//[Instruction: Compare if reg is not equal to number] ; cmpne <register>, <number>
if( opcode == cmpne_opcode )
 {
   cout << "cmpne     " << getRegisterName(regName) << convertToInt("0" + arg2_12bits) << endl;
 }


// [Instruction: Set mode for MVC] ;  mvc   <null 2-bits> <partition-mode 3-bits> <reference-frame 3-bits> <mvc 8-bits>

if( opcode == mvc_opcode )
{
	int mode_part = convertToInt(line.substr(4, 5).c_str());
	std::string mode_part_str;

	if( mode_part == E_MODE_16x16 )
		mode_part_str = "16x16";
	else if( mode_part == E_MODE_8x16 )
		mode_part_str = "8x16";
	else if( mode_part == E_MODE_16x8 )
		mode_part_str = "16x8";
	else if( mode_part == E_MODE_8x8 )
		mode_part_str = "8x8";

	cout << "mvc      part: " << mode_part_str << "\tref: " << convertToInt( "0" + line.substr(9, 3)) << "\tmvc: " << convertToInt( "0" + line.substr(12, 8)) << endl;
}

}

void infoProgram(char * filename)
{
 ifstream inp(filename);
 int n = 0;
 map<string, string> inst;

 cout << "Program Instructions: " << endl << endl;

 if( inp.is_open() )
  {
    string line;

    cout << "Line\tOPCODE\tArguments" << endl;
    cout << "----\t------\t------------------" << endl;

    while (! inp.eof() )
    {
      getline(inp, line);
	  if( line == "" ) break;

      string opcode = line.substr(0, 4);
      string arg1   = line.substr(4, 8);
      string arg2   = line.substr(12, 8);

      cout << n++ << "\t" << opcode << "\t" << arg1 << "     " << arg2 << "\t";
      getInfo(line);
    }

    inp.close();
  }
  else
  {
    cout << "Error: Cannot find memory file '" << filename << "'" << endl;
  }
}

int convertToInt(std::string num)
{
  int dec = 0;
  bool isneg = (num[0] == '1');

  if( !isneg )
  {
      for(int i = 0; i < (int)num.size(); ++i)
      {
        int digit = (num[i] == '1' ? 1 : 0);

        dec +=  digit * (1 << (num.size() - 1 - i) );
      }
  }
  else // complement 2
  {
      for(int i = 0; i < (int)num.size(); ++i)
      {
        int digit = (num[i] == '1' ? 0 : 1);

        dec +=  digit * (1 << (num.size() - 1 - i) ) ;
      }

      dec = -(dec + 1);
  }

  return dec;
}


float convertToFloat(std::string num)
{
  bool isneg = (num[0] == '1');
  float result = 1.0;

  if( isneg ) // complement 2
  {
      /// invert
      for(int i = 0; i < (int)num.size(); ++i)
      {
         num[i] = (num[i] == '1' ? '0' : '1');
      }

     int curry = 1;
     for(int i = num.size() - 1; i >= 0; --i )
     {
      int digit = (num[i] == '1' ? 1 : 0);
      num[i] = (digit != curry) ? '1' : '0';
      curry  = digit * curry;
     }

     result = -1.0;
  }

  int dec = convertToInt( num.substr(0,6) );
  std::string frac = num.substr(6, 2);

  float f;

  if( frac == "00" )
    f = 0.0f;
  else if( frac == "01" )
    f = 0.25f;
  else if( frac == "10" )
    f = 0.50f;
  else if( frac == "11" )
    f = 0.75f;

  result *= (float)dec + f;
  return result;
}

void infoMemory(char * filename)
{
  ifstream inp(filename);

  cout << "\nMemory File:" << endl << endl;

  int n = 0;

  if( inp.is_open() )
  {
    string line;

    cout << "ID\tX-Point\tY-Point\tAs-Int-pel\t As-Frac-pel" << endl;
    cout << "--\t--------\t--------\t----------\t-------------" << endl;
    while (! inp.eof() )
    {
      getline(inp, line);

      string x = line.substr(0, 8);
      string y = line.substr(8, 8);

      cout << n++ << "\t" << x << "\t" << y;
      cout << "\t(" << convertToInt(x) << ", " << convertToInt(y) << ")"
           << "\tor ("  << convertToFloat(x) << ", " << convertToFloat(y) << ")" << endl;
    }

    inp.close();
  }
  else
  {
    cout << "Error: Cannot find memory file '" << filename << "'" << endl;
  }

}


void loadIni(std::string filename)
{
	CSimpleIniA ini(false, true, true);
	char * empty = "";

	SI_Error rc = ini.LoadFile( filename.c_str() );

	if( rc != SI_OK)
	{
      cout << "Cannot load 'configuration.ini' file." << endl;   
	  exit(0);
	}

	CSimpleIniA::TNamesDepend sections;
	ini.GetAllSections(sections);

	// [General]
	version = std::string(ini.GetValue("General", "Version", empty ) );

	instruction_bits = str2int( std::string( ini.GetValue("General", "Instruction-bits", empty )) );

	opcode_bits =  str2int( std::string( ini.GetValue("General", "opcode-bits", empty )) );
	coordinate_bits =  str2int( std::string( ini.GetValue("General", "coordinate-bits", empty )) );

	// [Memory]
	program_memory_size = str2int( std::string( ini.GetValue("Memory", "program-memory-size", empty )) );
	point_memory_size =  str2int( std::string( ini.GetValue("Memory", "point-memory-size", empty )) );

	// [Reference Frame]
	min_width  = str2int( std::string( ini.GetValue("Reference Frame", "min-width", empty )));
	max_width  = str2int( std::string( ini.GetValue("Reference Frame", "max-width", empty )));
	min_height = str2int( std::string( ini.GetValue("Reference Frame", "min-height", empty )));
	max_height = str2int( std::string( ini.GetValue("Reference Frame", "max-height", empty )));

	// [Registers]
	COST = std::string( ini.GetValue("Registers", "COST", empty ));
	LENGTH = std::string( ini.GetValue("Registers", "LENGTH", empty ));

	SAD = std::string( ini.GetValue("Registers", "SAD", empty ));  // deprecated!
	Mx  = std::string( ini.GetValue("Registers", "Mx", empty ));   // deprecated!
	My  = std::string( ini.GetValue("Registers", "My", empty ));   // deprecated!

	// chk <start_pattern_address>,<number_of_pos>
	chk_opcode = std::string( ini.GetValue("Instruction: Check full-pel Pattern", "opcode", empty ));
	chk_start = str2int( std::string( ini.GetValue("Instruction: Check full-pel Pattern", "start-pattern-address-bits", empty )));
	chk_num = str2int( std::string( ini.GetValue("Instruction: Check full-pel Pattern", "number-of-points-bits", empty )));

	// chkfr <start_pattern_address>,<number_of_pos>
	chkfr_opcode= std::string( ini.GetValue("Instruction: Check fractional Pattern", "opcode", empty ));
	chkfr_start= str2int( std::string( ini.GetValue("Instruction: Check fractional Pattern", "start-pattern-address-bits", empty )));
	chkfr_num= str2int( std::string( ini.GetValue("Instruction: Check fractional Pattern", "number-of-points-bits", empty )));

	// chkjmp <winning_po>, <afterwin_program_address>
	chkjmp_opcode= std::string( ini.GetValue("Instruction: Check and Jump", "opcode", empty ));

	chkjmp_win= str2int( std::string( ini.GetValue("Instruction: Check and Jump", "winning-point-bits", empty )));
	chkjmp_address= str2int( std::string( ini.GetValue("Instruction: Check and Jump", "afterwin-program-address-bits", empty )));

	// cmpl <register>, <number>
	cmpl_opcode= std::string( ini.GetValue("Instruction: Compare if reg Less than number", "opcode", empty ));
	cmpl_reg= str2int( std::string( ini.GetValue("Instruction: Compare if reg Less than number", "register-bits", empty )));
	cmpl_num= str2int( std::string( ini.GetValue("Instruction: Compare if reg Less than number", "number-bits", empty )));

	// cmpg <register>, <number>
	cmpg_opcode= std::string( ini.GetValue("Instruction: Compare if reg greater than number", "opcode", empty ));
	cmpg_reg= str2int( std::string( ini.GetValue("Instruction: Compare if reg greater than number", "register-bits", empty )));
	cmpg_num= str2int( std::string( ini.GetValue("Instruction: Compare if reg greater than number", "number-bits", empty )));

	// cmpe <register>, <number>
	cmpe_opcode= std::string( ini.GetValue("Instruction: Compare if reg is equal to number", "opcode", empty ));
	cmpe_reg= str2int( std::string( ini.GetValue("Instruction: Compare if reg is equal to number", "register-bits", empty )));
	cmpe_num= str2int( std::string( ini.GetValue("Instruction: Compare if reg is equal to number", "number-bits", empty )));

	// cmpne <register>, <number>
	cmpne_opcode= std::string( ini.GetValue("Instruction: Compare if reg is not equal to number", "opcode", empty ));
	cmpne_reg= str2int( std::string( ini.GetValue("Instruction: Compare if reg is not equal to number", "register-bits", empty )));
	cmpne_num= str2int( std::string( ini.GetValue("Instruction: Compare if reg is not equal to number", "number-bits", empty )));

	// jmp <program_address>
	jmp_opcode= std::string( ini.GetValue("Instruction: Conditional jump", "opcode", empty ));
	jmp_address= str2int( std::string( ini.GetValue("Instruction: Conditional jump", "program-address-bits", empty )));

	// b <program_address>
	b_opcode= std::string( ini.GetValue("Instruction: Unconditional jump", "opcode", empty ));
	b_address= str2int( std::string( ini.GetValue("Instruction: Unconditional jump", "program-address-bits", empty )));

	// halt
	halt_opcode = std::string( ini.GetValue("Instruction: Halt", "opcode", empty ));

	// Set MVC mode
	mvc_opcode          = std::string( ini.GetValue("Instruction: Set mode for MVC", "opcode", empty ));
	mvc_partition_mode  = str2int( std::string( ini.GetValue("Instruction: Set mode for MVC", "partition-mode", empty )) );
    mvc_reference_frame = str2int( std::string( ini.GetValue("Instruction: Set mode for MVC", "reference-frame", empty )) );
	mvc_mvc             = str2int( std::string( ini.GetValue("Instruction: Set mode for MVC", "mvc", empty )) );
}

int str2int(std::string num)
{
  return atoi( num.c_str() );
}

