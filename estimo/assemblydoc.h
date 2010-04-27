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

#ifndef ASSEMBLY_DOC_H
#define ASSEMBLY_DOC_H

#include <QHash>
#include <QSet>
#include <QVector>
#include <QList>
#include <QString>
#include "patterntable.h"
#include "asmcommand.h"
#include "binaryword.h"
#include "settings.h"

#define BIN_T_BRUNCH     BinaryWord::toBinary(CONF.b_opcode)
#define BIN_T_JMP        BinaryWord::toBinary(CONF.jmp_opcode)
#define BIN_T_CHK        BinaryWord::toBinary(CONF.chk_opcode)
#define BIN_T_CHKFR      BinaryWord::toBinary(CONF.chkfr_opcode)
#define BIN_T_CHKJMP     BinaryWord::toBinary(CONF.chkjmp_opcode)
#define BIN_T_LT         BinaryWord::toBinary(CONF.cmpl_opcode)
#define BIN_T_GT         BinaryWord::toBinary(CONF.cmpg_opcode)
#define BIN_T_EQ         BinaryWord::toBinary(CONF.cmpe_opcode)
#define BIN_T_NE         BinaryWord::toBinary(CONF.cmpne_opcode)
#define BIN_T_COST       BinaryWord::toBinary(CONF.COST)
#define BIN_T_LENGTH     BinaryWord::toBinary(CONF.LENGTH)
#define BIN_T_WINID      BinaryWord::toBinary(CONF.WINID)
#define BIN_T_HALT       BinaryWord::toBinary(CONF.halt_opcode)
#define BIN_T_MVC_MODE   BinaryWord::toBinary(CONF.mvc_opcode)

/// Assembly Document 
class AssemblyDoc
{
 public: 
  
   // FIXME: to be removed
   static int CMPNUM;     ///< Number of Bits for the number in compare commands
   static int CMP_MAX;   ///< The biggest number in compare commands
   static int CMP_MIN;  ///< The lowest number in compare commands
   static int MIN_OFFSET;    ///< Minimum Value for the search window
   static int MAX_OFFSET;      ///< Maximum Value for the search window

 /// Constructor
 AssemblyDoc();

 /// Initialize hash tables
 void init();

 /// Finalize the assembly document
 void  endDoc();

 /// Output assembly language file
 void outputAssembly(const QString &filename, const QString &binfile);

 /// Output program in machine code
 void outputBinary(QVector <AsmCmd *> & optProgram, const QString &filename) const;

 /// Dump Point memory to file
 void dumpMemory(const QString &filename) const;

 /// Check if the point offset is valid range
 static bool isValidPoint(float x, float y, bool fullpel = true);

 /// Set the label for the next command
 void setLabel(QString lbl);

 /// Return a unique label
 QString newLabel();

 /// Add a new Command
 void addNewCmd(AsmCmd *cmd);

 /// Add the pattern and return a unique name
 QString addNewPattern(const QList<PatternTable::Point> & p);

 /// Return a new unique pattern name
 QString newPatternName();

 /// Return the number of points
 int getPatternSize(const QString &pname) const;
 
  /// Return the start address of the pattern in point memory
 int getPatternStart(const QString &pName);

 /// Return the pattern with the input name
 const QVector<PatternTable::Point> getPattern(const QString &patName) const;

 /// Return the address of the label in memory
 int getLabelAddress(QString label);

 /// Return a globally unique label name
 QString getUniqueLabelName(QString name);

 /// Check if pattern with name is integral pel or half/quarter pel
 bool isPatternIntegralPel(QString name);

 /// Return a reference to the labels hash table
 QHash<QString, int> & getLabelHashTable() { return labels; }

 /// Set Point as Inttype or FloatType
 void setPointPelType(int index, bool fullpel);

 /// Return current program's size
 int getProgramSize();

 private: 
 int labNum;             ///< Counting labels for unique label names
 int patNum;             ///< Counting patterns for unique names
 QString curLabel;       ///< label for the next command
 QVector <AsmCmd *> program;              ///< Holding the program
 QVector <PatternTable::Point> pointMem;  ///< Point Memory

 QHash<QString, int> checksIndex;         ///< hash table holding the adddress of the 
                                                 ///patterns in point memory
 QHash<QString, QString> matchlabels; /// match two labels
 QHash<QString, int> labels;   ///< hash table holding labels and the assosiated address
 QHash< QString, QVector<PatternTable::Point> > patterns;  ///< hash table holding all patterns

 /// Return the label for the current command
 QString getNextLabel();

 /// Trace the hash table until you find the real match  
 QString traceLabel(QString label);
};


/// Perform Assembly Optimization removing useless commands
QVector<AsmCmd *> assembly_optimization(AssemblyDoc & doc, const QVector<AsmCmd *> & program);

#endif 
