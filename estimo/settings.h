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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Settings
{
 public:

  // Comstructor
  Settings(QString conFile);

  // General
  QString version;
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
  QString COST;
  QString LENGTH;
  QString WINID;
  

  // chk <start_pattern_address>,  <number_of_points>
  QString chk_opcode;
  int     chk_start;
  int     chk_num;

  // chkfr <start_pattern_address>,  <number_of_points>
  QString chkfr_opcode;
  int     chkfr_start;
  int     chkfr_num;

  // chkjmp <winning_point>, <afterwin_program_address>
  QString chkjmp_opcode;
  int     chkjmp_win;
  int     chkjmp_address;

  // cmpl <register>, <number>
  QString cmpl_opcode;
  int     cmpl_reg;
  int     cmpl_num;

  // cmpg <register>, <number>
  QString  cmpg_opcode;
  int      cmpg_reg;
  int      cmpg_num;

  // cmpe <register>, <number>
  QString cmpe_opcode;
  int     cmpe_reg;
  int     cmpe_num;

  // cmpne <register>, <number>
  QString cmpne_opcode;
  int     cmpne_reg;
  int     cmpne_num;

  // jmp <program_address>
  QString jmp_opcode;
  int     jmp_address;

  // b <program_address>
  QString b_opcode;
  int     b_address;

  // halt
  QString halt_opcode;

  /// Set MVC mode
  QString mvc_opcode;
  int mvc_partition_mode;
  int mvc_reference_frame;
  int mvc_mvc;

  static Settings *settings;
};


const Settings & getSettings();

#define CONF getSettings()

#endif
