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

#include <QString>
#include "semanticerror.h"
#include "asmcommand.h"
#include "assemblydoc.h"
#include "patterntable.h"
#include "binaryword.h"
#include "entity.h"
#include "estimo_yacc.h"

/// Constructor
CheckCmd::CheckCmd(AssemblyDoc & doc, const QList<PatternTable::Point> &p) 
{
  patName = doc.addNewPattern(p);
  start   = doc.getPatternStart(patName);
  size    = doc.getPatternSize(patName);

  if( doc.isPatternIntegralPel(patName) )
	  isFullPel = true;
  else
	  isFullPel = false;

  for(int i = start; i < start + size; ++i)
	 doc.setPointPelType(i, isFullPel);
 
  doc.addNewCmd(this);
}

/// Return command in string representation
QString CheckCmd::evalString() const
{
 QString lbl = "";
 if( !label.isEmpty() ) lbl = QString("%1:  ").arg(getLabel());

 if( isFullPel ) 
    return QString("%1   chk %2    ; Check full pel pattern[%3, %4]").arg(lbl).arg(patName).arg(start).arg(size);
 else
    return QString("%1   chkfr %2    ; Check fractional pel pattern[%3, %4]").arg(lbl).arg(patName).arg(start).arg(size);

}

/// Return command in machine code
BinaryWord CheckCmd::eval(EvalParam *param) const
{
 if( isFullPel ) 
       return  BIN_T_CHK +  
	      BinaryWord::toBinary(size, CONF.chk_num, true)      // number of points
          + BinaryWord::toBinary(start, CONF.chk_start, true); // start address
 else
        return  BIN_T_CHKFR + 
		BinaryWord::toBinary(size, CONF.chkfr_num, true)          // number of points
 		+ BinaryWord::toBinary(start, CONF.chkfr_start, true);    // start address

}

//---------------------------------------------------------------------------

/// Constructor
JumpCmd::JumpCmd(AssemblyDoc & doc, QString _target, bool _unconditional) 
{
  target = _target;
  unconditional = _unconditional;
  doc.addNewCmd(this);
}

/// Return command in string representation
QString JumpCmd::evalString() const
{
 QString lbl = "";
 if( !label.isEmpty() ) lbl = QString("%1:  ").arg(getLabel());
 QString cmd = (unconditional ? "b" : "jmp");

 return QString("%1   %2 %3").arg(lbl).arg(cmd).arg(getRealLabel(target));
}

/// Return command in machine code
BinaryWord  JumpCmd::eval(EvalParam *param) const
{
 const int addr_bits = (unconditional ? CONF.b_address : CONF.jmp_address );

 return (unconditional ? BIN_T_BRUNCH : BIN_T_JMP ) + 
              BinaryWord::toBinary( Entity::doc.getLabelAddress(target), addr_bits, true);
}

//---------------------------------------------------------------------------

/// Constructor
CmpCmd::CmpCmd(AssemblyDoc & doc, CmpCmd::REGISTER _reg, CmpCmd::OPERATOR _opr, float _num) 
{
    reg = _reg;
    opr = _opr;
    num = _num;

    if(!( num >= AssemblyDoc::CMP_MIN  && num <= AssemblyDoc::CMP_MAX ) )
	  throw SemanticError(QString("Error: %1 number is out of range %2 ... %3").arg(num).arg(AssemblyDoc::CMP_MIN).arg(AssemblyDoc::CMP_MAX));

    doc.addNewCmd(this); 
}

/// Return command in string representation
QString CmpCmd::evalString() const
{
 QString lbl = "";
 if( !label.isEmpty() ) lbl = QString("%1:  ").arg(getLabel());

 QString cmd;
 QString regstr;

  if( opr == GREATER)   cmd = "cmpg";
  if( opr == LESS)      cmd = "cmpl";
  if( opr == EQUAL)     cmd = "cmpe";
  if( opr == NOT_EQUAL) cmd = "cmpne";

  if( reg == SAD ) regstr = "sad";
  if( reg == MX )  regstr = "Mx";
  if( reg == MY )  regstr = "My";

 return QString("%1   %2 %3, %4 ").arg(lbl).arg(cmd).arg(regstr).arg(num);
}

/// Return command in machine code
BinaryWord CmpCmd::eval(EvalParam *param) const
{
  BinaryWord opcode;
  BinaryWord registerBin;

  if( opr == GREATER)   opcode = BIN_T_GT;
  if( opr == LESS)      opcode = BIN_T_LT;
  if( opr == EQUAL)     opcode = BIN_T_EQ;
  if( opr == NOT_EQUAL) opcode = BIN_T_NE;

  if( reg == SAD ) registerBin = BIN_T_SAD;
  if( reg == MX )  registerBin = BIN_T_MX;
  if( reg == MY )  registerBin = BIN_T_MY;

  return opcode + registerBin + BinaryWord::toBinary(num, CONF.cmpg_num, true);
}
 
//---------------------------------------------------------------------------

/// Constructor
HaltCmd::HaltCmd(AssemblyDoc & doc) 
{ doc.addNewCmd(this); }

/// Return command in machine code
BinaryWord HaltCmd::eval(EvalParam *param) const
{
 return  BIN_T_HALT +
                   BinaryWord::toBinary(0, CONF.coordinate_bits, true) + 
                          BinaryWord::toBinary(0, CONF.coordinate_bits, true);
}

/// Return command in string representation
QString HaltCmd::evalString() const
{
 QString lbl = "";
 if( !label.isEmpty() ) lbl = QString("%1:  ").arg(getLabel());
 return QString("%1   halt").arg(lbl);
}

//---------------------------------------------------------------------------

/// Return command's Label
QString AsmCmd::getLabel() const
{ 
 if( label == "" || label.isEmpty() || label.isNull() )
  return "";
 else
  return Entity::doc.getUniqueLabelName(label); 
}

/// Return command's real Label
QString AsmCmd::getRealLabel(QString name)
{
  return Entity::doc.getUniqueLabelName(name); 
}


//---------------------------------------------------------------------------

/// Constructor
CheckJumpCmd::CheckJumpCmd(AssemblyDoc & doc, const QString &winning_point, const QString &target) 
{
  winPoint     = winning_point;
  winAddress = target;
  doc.addNewCmd(this);
}

/// Return command in string representation
QString CheckJumpCmd::evalString() const
{
 QString comment;
 QString lbl = "";
 
 if( !label.isEmpty() ) lbl = QString("%1:  ").arg(getLabel());

 if( winPoint == "0" )
	comment =  "\t  ; if no progress then jump to %3";
 else
    comment =  "\t  ; if winning point is #%2 then jump to %3";

 return QString("%1   chkjmp %2,  %3" + comment).arg(lbl).arg(winPoint).arg(getRealLabel(winAddress));
}

/// Return command in machine code
BinaryWord CheckJumpCmd::eval(EvalParam *param) const
{
   return  BIN_T_CHKJMP + BinaryWord::toBinary(winPoint.toFloat(), CONF.chkjmp_win, true) + 
	  BinaryWord::toBinary(Entity::doc.getLabelAddress(winAddress), CONF.chkjmp_address, true);
}

//---------------------------------------------------------------------------

/// Constructor
MVCModeCmd::MVCModeCmd(AssemblyDoc & doc, int m_mvc, int m_refer, int m_part)
  : mode_mvc(m_mvc), mode_refer(m_refer), mode_part(m_part)
{
  doc.addNewCmd(this);
}

/// Return command in string representation
QString MVCModeCmd::evalString() const
{
  QString lbl = "";
  QString mode_part_str;
  
  if( !label.isEmpty() ) lbl = QString("%1:  ").arg(getLabel());
  
  if( mode_part == E_MODE_16x16 )
	  mode_part_str = "16x16";
  else if( mode_part == E_MODE_8x16 )
	  mode_part_str = "8x16";
  else if( mode_part == E_MODE_16x8 )
	  mode_part_str = "16x8";
  else if( mode_part == E_MODE_8x8 )
	  mode_part_str = "8x8";

  return QString("%1   mvc %2, %3, %4       ; set mode: execute %2 instruction(s) with ref %3 and partition %5 for each MVC ").arg(lbl).arg(mode_mvc).arg(mode_refer).arg(mode_part).arg(mode_part_str);
}


/// Return command in machine code
BinaryWord  MVCModeCmd::eval(EvalParam *param) const
{
	return BIN_T_MVC_MODE + BinaryWord::toBinary(mode_part,  CONF.mvc_partition_mode, true)
		                  + BinaryWord::toBinary(mode_refer, CONF.mvc_reference_frame, true)
						  + BinaryWord::toBinary(mode_mvc,   CONF.mvc_mvc, true);
}


//---------------------------------------------------------------------------