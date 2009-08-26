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

#ifndef ASMCMD_H
#define ASMCMD_H

#include <QString>
#include <QList>
#include "patterntable.h"
#include "binaryword.h"

class EvalParam;
class AssemblyDoc;

/// An Assembly Command inherited by all assembly commands
class AsmCmd
{
 public:

  enum Types
  {
    ABSTRACT,
	GO_JUMP,
	JUMP,
    CHECK_JUMP,
	CHECK_PATTERN,
	COMPARE,
	HALT,
	SETMVC
  };

  static QString describeType(Types id)
  {
	  QString typeNames[] = { "Abstract", "b", "Jump", "checkJump", "CheckPattern", "Compare", "Halt", "SetMVC" };
	  return typeNames[id];
  }

  /// Constructor
  AsmCmd() {}

  /// Destruct
  virtual ~AsmCmd() {}

  /// Return the type of the command
  virtual AsmCmd::Types getType() const { return AsmCmd::ABSTRACT; }

  /// Return command in machine code
  virtual BinaryWord eval(EvalParam *param) const = 0;

  /// Return command in string representation
  virtual QString evalString() const = 0;

  /// Return if the class is a jump/brunch command
  virtual bool isJump() const { return false; }

  /// Return if the class is a check and jump command
  virtual bool isCheckJump() const { return false; }

 /// Return true if it is unconditional
  virtual bool isUncond() const { return false; }

  /// Set command's Label
  void setLabel(const QString &lbl) { label = lbl;  }

  /// Return command's Label
  QString getLabel() const;

  /// Return command's real Label
  static QString getRealLabel(QString name);

  /// Return the hashtable Label
  QString getHashLabel() const { return label; }

 protected:
  QString label;
};

//---------------------------------------------------------------------------

/// Check pattern command 
class CheckCmd : public AsmCmd
{
 public:

   /// Constructor
   CheckCmd(AssemblyDoc & doc, const QList<PatternTable::Point> &p);

   /// Return command in string representation
   virtual QString evalString() const;
  
  /// Return the type of the command
  virtual AsmCmd::Types getType() const { return AsmCmd::CHECK_PATTERN; }

  /// Destructor
   virtual ~CheckCmd() {}

   /// Return command in machine code
   virtual BinaryWord eval(EvalParam *param) const;
  
 private:
  QString patName;
  int start;  ///< Start address of the pattern in point memory
  int size;   ///< Number of points for the pattern in point memory
  bool isFullPel; ///< Specifies if the pattern contains only full pel points
};

//---------------------------------------------------------------------------

/// Jump Command,  conditional jmpF, unconditional jmp
class JumpCmd : public AsmCmd
{
 public:

 /// Constructor
 JumpCmd(AssemblyDoc & doc, QString _target, bool _unconditional = true);

 /// Return command in string representation
 virtual QString evalString() const;

 /// Destructor
 virtual ~JumpCmd() {}

 /// Return command in machine code
 virtual BinaryWord eval(EvalParam *param) const;
 
  /// Return if the class is a jump/brunch command
  virtual bool isJump() const { return true; }

  /// Return true if it is unconditional
  virtual bool isUncond() const { return unconditional; }

  /// Return the target Label
  QString getTarget() const { return target; }

  /// Set target
  void setTarget(QString newTarget) { target = newTarget; }
   
    /// Return the type of the command
  virtual AsmCmd::Types getType() const { return (unconditional ? AsmCmd::GO_JUMP : AsmCmd::JUMP); }

 private:
  QString target;        ///< label of the target label
  bool unconditional;    ///< if true the jump is unconditional otherwise jump only if flag==false
};

//---------------------------------------------------------------------------

/// Compare Command  SAD|MX|MY (> >= < <= !=) value 
class CmpCmd : public AsmCmd
{
 public:
  enum REGISTER {SAD, COST, LENGTH, MX, MY};
  enum OPERATOR {GREATER, LESS, EQUAL, NOT_EQUAL};

  /// Constructor
  CmpCmd(AssemblyDoc & doc, REGISTER _reg, OPERATOR _opr, float _num);

  /// Destructor
  virtual ~CmpCmd() {}

  /// Return command in string representation
  virtual QString evalString() const;

  /// Return command in machine code
  virtual BinaryWord eval(EvalParam *param) const;
    
  /// Return the type of the command
  virtual AsmCmd::Types getType() const { return AsmCmd::COMPARE; }

 private:
   REGISTER reg;
   OPERATOR opr; 
   float num;
};

//---------------------------------------------------------------------------

/// Halt Command terminates the program
class HaltCmd : public AsmCmd
{
 public:
   /// Constructor
   HaltCmd(AssemblyDoc & doc);

   /// Return command in string representation
   virtual QString evalString() const;

   /// Destructor
   virtual ~HaltCmd() {}

   /// Return the type of the command
   virtual AsmCmd::Types getType() const { return AsmCmd::HALT; }

   /// Return command in machine code
   virtual BinaryWord eval(EvalParam *param) const;
 };

//---------------------------------------------------------------------------

/// Check and Jump command
class CheckJumpCmd : public AsmCmd
{
 public:

   /// Constructor
   CheckJumpCmd(AssemblyDoc & doc, const QString &winning_point, const QString &target); 

   /// Return command in string representation
   virtual QString evalString() const;

   /// Destructor
   virtual ~CheckJumpCmd() {}

   /// Return command in machine code
   virtual BinaryWord eval(EvalParam *param) const;
  
   /// Return if the class is a check and jump command
   virtual bool isCheckJump() const { return true; }

   /// Return the targeting address
   QString getTarget() const { return winAddress; }

   /// Set winning target address
   void setTarget(const QString &target) { winAddress = target; }

    /// Return the type of the command
   virtual AsmCmd::Types getType() const { return AsmCmd::CHECK_JUMP; }

 private:  
   QString winPoint;      ///< The winning point from the previous checked pattern
   QString winAddress;    ///< The address of the jump
};

//---------------------------------------------------------------------------

/// Halt Command terminates the program
class MVCModeCmd : public AsmCmd
{
public:
	/// Constructor
	MVCModeCmd(AssemblyDoc & doc, int m_mvc, int m_refer, int m_part);

	/// Return command in string representation
	virtual QString evalString() const;

	/// Destructor
	virtual ~MVCModeCmd() {}

	/// Return the type of the command
	virtual AsmCmd::Types getType() const { return AsmCmd::SETMVC; }

	/// Return command in machine code
	virtual BinaryWord eval(EvalParam *param) const;

private:
    int mode_mvc;
	int mode_refer;
	int mode_part;
};

//---------------------------------------------------------------------------

#endif
