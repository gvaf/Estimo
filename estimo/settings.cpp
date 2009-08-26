/**************************************************************************
 *  SharpEye Studio - Integrated Development Environment
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

#include "settings.h"

#include <QtGui>

#include <iostream>
using namespace std;

#define SI_SUPPORT_IOSTREAMS
#include "SimpleIni.h"

Settings::Settings(QString conFile)
{
 CSimpleIniA ini(false, true, true);

 SI_Error rc = ini.LoadFile( conFile.toStdString().c_str() );

 if( rc != SI_OK)
 {
	 cout << "Cannot load 'configuration.ini' file." << endl;   
	 return;
 }

 CSimpleIniA::TNamesDepend sections;
 ini.GetAllSections(sections);

 // [General]
  version = QString(ini.GetValue("General", "Version", NULL ) );
  instruction_bits = QString( ini.GetValue("General", "Instruction-bits", NULL )).toInt();
  opcode_bits = QString( ini.GetValue("General", "opcode-bits", NULL )).toInt();
  coordinate_bits = QString( ini.GetValue("General", "coordinate-bits", NULL )).toInt();

 // [Memory]
  program_memory_size = QString( ini.GetValue("Memory", "program-memory-size", NULL )).toInt();
  point_memory_size = QString( ini.GetValue("Memory", "point-memory-size", NULL )).toInt();

  // [Reference Frame]
  min_width  = QString( ini.GetValue("Reference Frame", "min-width", NULL )).toInt();
  max_width  = QString( ini.GetValue("Reference Frame", "max-width", NULL )).toInt();
  min_height = QString( ini.GetValue("Reference Frame", "min-height", NULL )).toInt();
  max_height = QString( ini.GetValue("Reference Frame", "max-height", NULL )).toInt();

  // [Registers]
  SAD  = QString( ini.GetValue("Registers", "SAD", NULL ));
  COST   = QString( ini.GetValue("Registers", "COST", NULL ));
  LENGTH = QString( ini.GetValue("Registers", "LENGTH", NULL ));

  Mx  = QString( ini.GetValue("Registers", "Mx", NULL ));
  My  = QString( ini.GetValue("Registers", "My", NULL ));

	// chk <start_pattern_address>,<number_of_pos>
	chk_opcode= QString( ini.GetValue("Instruction: Check full-pel Pattern", "opcode", NULL ));
	chk_start= QString( ini.GetValue("Instruction: Check full-pel Pattern", "start-pattern-address-bits", NULL )).toInt();
	chk_num= QString( ini.GetValue("Instruction: Check full-pel Pattern", "number-of-points-bits", NULL )).toInt();

	// chkfr <start_pattern_address>,<number_of_pos>
	chkfr_opcode= QString( ini.GetValue("Instruction: Check fractional Pattern", "opcode", NULL ));
	chkfr_start= QString( ini.GetValue("Instruction: Check fractional Pattern", "start-pattern-address-bits", NULL )).toInt();
	chkfr_num= QString( ini.GetValue("Instruction: Check fractional Pattern", "number-of-points-bits", NULL )).toInt();

	// chkjmp <winning_po>, <afterwin_program_address>
	chkjmp_opcode= QString( ini.GetValue("Instruction: Check and Jump", "opcode", NULL ));

	chkjmp_win= QString( ini.GetValue("Instruction: Check and Jump", "winning-point-bits", NULL )).toInt();
	chkjmp_address= QString( ini.GetValue("Instruction: Check and Jump", "afterwin-program-address-bits", NULL )).toInt();

	// cmpl <register>, <number>
	cmpl_opcode= QString( ini.GetValue("Instruction: Compare if reg Less than number", "opcode", NULL ));
	cmpl_reg= QString( ini.GetValue("Instruction: Compare if reg Less than number", "register-bits", NULL )).toInt();
	cmpl_num= QString( ini.GetValue("Instruction: Compare if reg Less than number", "number-bits", NULL )).toInt();

	// cmpg <register>, <number>
	 cmpg_opcode= QString( ini.GetValue("Instruction: Compare if reg greater than number", "opcode", NULL ));
	 cmpg_reg= QString( ini.GetValue("Instruction: Compare if reg greater than number", "register-bits", NULL )).toInt();
	 cmpg_num= QString( ini.GetValue("Instruction: Compare if reg greater than number", "number-bits", NULL )).toInt();

	// cmpe <register>, <number>
	cmpe_opcode= QString( ini.GetValue("Instruction: Compare if reg is equal to number", "opcode", NULL ));
	cmpe_reg= QString( ini.GetValue("Instruction: Compare if reg is equal to number", "register-bits", NULL )).toInt();
	cmpe_num= QString( ini.GetValue("Instruction: Compare if reg is equal to number", "number-bits", NULL )).toInt();

	// cmpne <register>, <number>
	cmpne_opcode= QString( ini.GetValue("Instruction: Compare if reg is not equal to number", "opcode", NULL ));
	cmpne_reg= QString( ini.GetValue("Instruction: Compare if reg is not equal to number", "register-bits", NULL )).toInt();
	cmpne_num= QString( ini.GetValue("Instruction: Compare if reg is not equal to number", "number-bits", NULL )).toInt();

	// jmp <program_address>
	jmp_opcode= QString( ini.GetValue("Instruction: Conditional jump", "opcode", NULL ));
	jmp_address= QString( ini.GetValue("Instruction: Conditional jump", "program-address-bits", NULL )).toInt();

	// b <program_address>
	b_opcode= QString( ini.GetValue("Instruction: Unconditional jump", "opcode", NULL ));
	b_address= QString( ini.GetValue("Instruction: Unconditional jump", "program-address-bits", NULL )).toInt();

	// halt
	halt_opcode= QString( ini.GetValue("Instruction: Halt", "opcode", NULL ));

	// Set MVC mode
	mvc_opcode          = QString( ini.GetValue("Instruction: Set mode for MVC", "opcode", NULL ) );
	mvc_partition_mode  = QString( ini.GetValue("Instruction: Set mode for MVC", "partition-mode", NULL ) ).toInt();
	mvc_reference_frame = QString( ini.GetValue("Instruction: Set mode for MVC", "reference-frame", NULL )  ).toInt();
	mvc_mvc             = QString( ini.GetValue("Instruction: Set mode for MVC", "mvc", NULL ) ).toInt();
}

Settings * Settings::settings = 0;

const Settings & getSettings()
{
  if( Settings::settings == 0 )
  {
	   QString conFile = QApplication::applicationDirPath() + "/configuration.ini";
	   Settings::settings = new Settings(conFile);
  }
  return *Settings::settings;
}
