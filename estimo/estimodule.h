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

#ifndef ESTIMODULE_H
#define ESTIMODULE_H

#include <QHash>
#include <QString>
#include <QDebug>
#include <QVector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "entity.h"
#include "patterntable.h"
#include "semanticerror.h"

class RunTimeException
{
 public:
 
 enum TYPES_EXCEPTION
 {
   BREAK_LOOP ,
   INVALID_WIN_COMPARE
  };

 /// Constructor
 RunTimeException(TYPES_EXCEPTION t) 
  { type = t; }

 /// Return the type of the exception
 int getType() const { return type; }

 private:
  TYPES_EXCEPTION type;

};
//------------------------------------------------------------------

/// Wrapper Class for number constant values
class NumClass : public Entity
{
 public:

  /// Constructor
  NumClass(float n)  : val(n) {}

  /// Destructor
  virtual ~NumClass() {} 

  /// Return the number value
  virtual float eval(EvalParam *param) const {return val;}

 private:
   float val;
};

//------------------------------------------------------------------

/// Wrapper Class for variables
class VarClass : public Entity
{
 public:

  /// Constructor
  VarClass(const char *_name)
   : name(_name)
  {}

  /// Destructor
  virtual ~VarClass() {}
 
 /// Return the variable's value
 virtual float eval(EvalParam *param) const;

 private:
  QString name; ///< Symbolic Name
};

//------------------------------------------------------------------

/// Wrapper Class for assignments
class AssignClass : public Entity
{
 public:

  /// Constructor
  AssignClass(const char * _name, Entity *_rhnd)
   : name(_name), rhnd(_rhnd)
  {}

  /// Destructor
  virtual ~AssignClass() {}

  /// Evaluate expression -- assign a value to the variable
  virtual float eval(EvalParam *param) const;

 private:
  QString name;   ///< Name of the variable
  Entity *rhnd;    ///< The right hand expression
};

//------------------------------------------------------------------

/// printer class
class PrintClass : public Entity
{
 public:

  /// Constructor
	 PrintClass(std::string txt)
     :  text(txt)
  {}

 /// Destructor
 virtual ~PrintClass() {}

 virtual float eval(EvalParam *param) const;

 private:
	 std::string text;
};

//------------------------------------------------------------------

/// Wrapper Class for operators, numerical and logical expressions
class SetMVCmodeClass : public Entity
{
public:

	/// Constructor
	SetMVCmodeClass(Entity * _mvc, Entity * _refer, int _partition)
		: mode_mvc(_mvc), mode_refer(_refer), mode_part(_partition)
	{}

	/// Destructor
	virtual ~SetMVCmodeClass() {}

	virtual float eval(EvalParam *param) const;

private:
	Entity * mode_mvc;    
	Entity * mode_refer;
	int      mode_part;
};


//------------------------------------------------------------------

/// Wrapper Class for operators, numerical and logical expressions
class ExprClass : public Entity
{
 public:

  /// Constructor
  ExprClass(int operName, Entity *leftOp, Entity *rightOp = 0)
     :  opr(operName),  left(leftOp),  right(rightOp)
  {}

 /// Destructor
 virtual ~ExprClass() {}

 /// Evaluate Recursively the expression
 virtual float eval(EvalParam *param) const;

 private:
   int opr;            ///< The token ID of the operator
   Entity * left;     ///< The left operand
   Entity * right;   ///< The right operand
};

//------------------------------------------------------------------

/// Break Class exit a loop
class BreakClass : public Entity
{
 public:

  /// Constructor
  BreakClass(bool emb = false )
  { isEmbed = emb; }

  /// Destructor
  virtual ~BreakClass() {}

  /// If inside a Normal Loop throw exception to move over otherwise emit jump command
  virtual float eval(EvalParam *param) const;

  bool type() const { return isEmbed; }

 private:
  bool isEmbed;

};
//------------------------------------------------------------------

/// Wrapper Class for while loop structure
class WhileClass : public Entity
{
 public:

  /// Constructor
  WhileClass(Entity *_cond, Entity *_body)
   : cond(_cond), body(_body)
  {}

 /// Destructor
 virtual ~WhileClass() {}

 /// Loop while the condition is true and execute body
 virtual float eval(EvalParam *param) const;

 private:
  Entity *cond;
  Entity *body;
};

//------------------------------------------------------------------

/// Wrapper Class for do-while loop structure
class DoWhileClass : public Entity
{
 public:

  /// Constructor
  DoWhileClass(Entity *_cond, Entity *_body)
    : cond(_cond), body(_body)
  {}

 /// Destructor
 virtual ~DoWhileClass() {}

  /// Loop while the condition is true
 virtual float eval(EvalParam *param) const;

 private:  
  Entity *cond;
  Entity *body;
};

//------------------------------------------------------------------

/// Wrapper Class for 'for' loop structure
class ForClass : public Entity
{
 public:

  /// Constructor
  ForClass(const char *_var, Entity *_start, Entity *_last, Entity *_step, Entity * _body)
   :  var(_var), start(_start), last(_last), step(_step), body(_body)
  {}

  /// Destructor
  virtual ~ForClass() {}

  /// for loop -- for(i = 0 to 12 step 1)
  virtual float eval(EvalParam *param) const;

 private:
   QString var;
   Entity * start;   
   Entity * last;
   Entity * step;
   Entity * body;
};

//------------------------------------------------------------------

/// Wrapper Class for the if and if-else structure
class IFClass : public Entity
{
 public:

 /// Constructor
 IFClass(Entity *_cond, Entity * _ifbody, Entity *_elsebody = 0)
      : cond(_cond), ifbody(_ifbody), elsebody(_elsebody)
  {}

 /// Destructor
 virtual ~IFClass() {}

 /// Evaluate if command
 virtual float eval(EvalParam *param) const;

 private:
  Entity *cond;     ///< Condition
  Entity *ifbody;   ///< The true body
  Entity *elsebody;  ///< The false body
};

//------------------------------------------------------------------

/// Wrapper Class for the update command
class UpdateClass : public Entity
{
 public:

  /// Constructor
  UpdateClass()
  {}

 /// Destructor
 virtual ~UpdateClass() {}

 /// Emit an assembly command for checking points add the new pattern to point memory
 virtual float eval(EvalParam *param) const;

};

//------------------------------------------------------------------

/// Wrapper Class for the check command defined inside a pattern type
class ConstCheckClass : public Entity
{
 public:

  /// Constructor
  ConstCheckClass(float _x, float _y)
   : x(_x), y(_y)
  {}

  /// Destructor
  virtual ~ConstCheckClass() {}

  /// Return X-coord
  float getX() const {return x;}

  /// Return Y-coord
  float getY() const {return y;}

 private:
   float x;
   float y;
};

//---------------------------------------------------------

/// A Check list of items
class CheckListClass : public Entity
{
 public:

   /// Constructor -- Creates a new list appending the new item
  CheckListClass(Entity *elist, Entity *item);

  /// Destructor
  virtual ~CheckListClass() {}

  QList<Entity *> checks;
};

//---------------------------------------------------------

/// Cases List of items
class CasesListClass : public Entity
{
 public:

	 /// Constructor -- Creates a new list appending the new item 
	 CasesListClass(Entity *clist, Entity *item, int win);

	 /// Destructor
	 virtual ~CasesListClass() {}

	 QHash<int, Entity *> caseBlocks;
};


//---------------------------------------------------------

/// Wrapper Class for pattern definition
class PatternClass : public Entity
{
 public:

  /// Constructor -- update the Pattern Table with the new pattern
  PatternClass(const char *name, Entity * list);

  /// Destructor
  virtual ~PatternClass() {}

};

//------------------------------------------------------------------

/// Wrapper class for sequential commands
class SeqClass : public Entity
{
 public:

  /// Constructor
  SeqClass(Entity *_first, Entity *_sec)
     : first(_first), sec(_sec)
  {}

 /// Destructor
  virtual ~SeqClass() {}

 /// Evaluate -- first command then second command
 virtual float eval(EvalParam *param) const;

 private:
     Entity *first;  ///< First Command
     Entity *sec;   ///< Second Command
};

//------------------------------------------------------------------

/// Condition for #if, #while, #do running embeded
class AsmCondition : public Entity
{
 public:

   /// Constructor
   AsmCondition(float _reg, float _oper, float _num);

  /// Destructor
  virtual ~AsmCondition() {};

  /// Emit machine code
  virtual float eval(EvalParam *param) const; 

  /// Return the operation
  float getOperation();

  /// Return the class Name
  virtual QString className() { return "AsmCondition"; };

  /// Returns true if the condition is the operand "not_equal"
  bool isNotEqual();

  enum
  {
   UNKNOWN_REGISTER,
   REGISTER,
   WINNING_EQUAL,
   WINNING_NOT_EQUAL,
   EQUAL_OPERATOR,
   NOT_EQUAL_OPERATOR
  };

  private:
   float reg;
   float oper;
   float num;
};

//------------------------------------------------------------------

/// Wrapper Class for the if and if-else structure running embeded
class IFSharp : public Entity
{
 public:

 /// Constructor
 IFSharp(Entity *_cond, Entity * _ifbody, Entity *_elsebody = 0)
      : cond(_cond), ifbody(_ifbody), elsebody(_elsebody)
  {}

 /// Destructor
 virtual ~IFSharp() {}

 /// Evaluate if command if (cond) <code1> else <code2>
 virtual float eval(EvalParam *param) const;

 private: 
  Entity *cond;      ///< Condition
  Entity *ifbody;    ///< The true body
  Entity *elsebody;  ///< The false body
};

//------------------------------------------------------------------

/// Wrapper Class for while loop structure runnng embeded
class WhileSharp : public Entity
{
 public:

  /// Constructor
  WhileSharp(Entity *_cond, Entity *_body)
   : cond(_cond), body(_body)
  {}

 /// Destructor
 virtual ~WhileSharp() {}

 /// Loop
 virtual float eval(EvalParam *param) const;

 private:
  Entity *cond;
  Entity *body;
};

//------------------------------------------------------------------

/// Wrapper Class for do-while loop structure running embeded
class DoWhileSharp : public Entity
{
 public:

  /// Constructor
  DoWhileSharp(Entity *_cond, Entity *_body)
    : cond(_cond), body(_body)
  {}

 /// Destructor
 virtual ~DoWhileSharp() {}

 /// Loop while the condition is true
 virtual float eval(EvalParam *param) const;

 private:  
  Entity *cond;
  Entity *body;
};

//------------------------------------------------------------------

/// Wrapper Class for the check command -> { code }
class CheckJumpClass : public Entity
{
 public:

	 enum TYPE
	 {
       E_SINGLE_CHECK,
	   E_NAME_CHECK,
	   E_CASE_CHECK
	 };

  /// Constructor
  CheckJumpClass(Entity *_x, Entity *_y, Entity *_code = 0)
   : x(_x), y(_y), codeBlock(_code)
  { name = ""; 
    type = E_SINGLE_CHECK;
  }

 /// Destructor
 virtual ~CheckJumpClass() {}

 /// Check Cases
 CheckJumpClass(const char *pname, Entity *cases)
  : name(pname), x(0), y(0), codeBlock(cases)
 {
   type = E_CASE_CHECK;
 }

 /// Check giving a pattern variable name
 CheckJumpClass(const char *_name)
  : name(_name), x(0), y(0), codeBlock(0)
  {
	type = E_NAME_CHECK;

    if( !Entity::patterns.isDefined(name) )
      throw SemanticError( QString("Error: '%1' Unknown pattern name").arg(name) );
  }

  /// Evaluate -- add to the current pattern
  virtual float eval(EvalParam *param) const;
 
  /// Return X coordinate
 float getX() const {return x->eval(0); }

 /// Return Y Coordinate
 float getY() const {return y->eval(0); }

 /// Return the code block (delegate)
 Entity * getCodeBlock() const { return codeBlock; }

 TYPE type;

 private:
  QString name;
  Entity *x;
  Entity *y;
  Entity *codeBlock;
};

//------------------------------------------------------------------

/// An assembly command entity inherited by all assembly commands
class AsmCommand : public Entity
{
 public:

  /// Constructor
  AsmCommand() {}

  /// Destructor
  virtual ~AsmCommand() {}

  /// Set command's label
  void setLabel(QString label) 
   { this->label = label; }

 protected:
  QString label;
};

//------------------------------------------------------------------

/// A Series of assembly command
class AsmList : public AsmCommand
{
 public:
  
  /// Constructor
  AsmList(Entity *command) {push_back(command);};

  /// Emit machine code
  virtual float eval(EvalParam *param) const
  {
    for(int i = 0; i < program.size(); ++i)
     program[i]->eval(param);

    return 0;
  }

  /// Destructor
  virtual ~AsmList() {};

  /// Add the new command to the main program
  void push_back(Entity * command) { program.push_back(command); }

 private:
  QVector<Entity *> program;
};

//------------------------------------------------------------------

/// Assembly check pattern command
class AsmChk : public AsmCommand
{
 public:

  /// Constructor
  AsmChk(QString pattern) {patternName = pattern;}

  /// Destructor
  virtual ~AsmChk() {};

  /// Emit machine code
  virtual float eval(EvalParam *param) const;

 private:
  QString patternName;
};

//------------------------------------------------------------------

/// Assembly Jump command
class AsmJmp : public AsmCommand
{
 public:

  /// Constructor type: brunch/Jmp target: label
  AsmJmp(float type, QString target) 
  {
   this->type = type; 
   this->target = target;
  };

  /// Emit machine code
  virtual float eval(EvalParam *param) const;

  /// Destructor
  virtual ~AsmJmp() {}

 private:
  float type;
  QString target;
};

//------------------------------------------------------------------

/// Assembly Compare Command
class AsmCmp : public AsmCommand
{
 public:

  /// Constructor
  AsmCmp(float type, float reg, float num);

  /// Emit machine code
  virtual float eval(EvalParam *param) const;

  /// Destructor
  virtual ~AsmCmp() {}

 private:
  float type;
  float reg;
  float num;
};

//------------------------------------------------------------------

/// Assembly Halt machine execution command
class AsmHalt : public AsmCommand
{
 public:

  /// Constructor
  AsmHalt() {};

  /// Desructor
  virtual ~AsmHalt() {};

  /// Emit machine code
  virtual float eval(EvalParam *param) const;
};

//------------------------------------------------------------------

#endif
