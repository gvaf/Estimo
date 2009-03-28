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
 *  included in all copies or substantial portions of the Software.                    
 **************************************************************************
 */

#include <QtGui>

#include "sourceview.h"
#include "estimohighlight.h"

/// Constructor
SourceView::SourceView()
{
  QFont font;
  font.setFamily("Courier");
  font.setFixedPitch(true);
  font.setPointSize(10);

  editor = new QTextEdit;
  editor->setFont(font);

  highlighter = new EstimoHighLight(editor->document());
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(editor);
  setLayout(layout);

 connect(editor->document(), SIGNAL(modificationChanged(bool)), this, SLOT(whenModified(bool)));
}

/// Destructor
SourceView::~SourceView()
{
}

/// When the file changed change ico
void SourceView::whenModified(bool changed)
{

 if(changed)
  emit modified(this);
}

/// Load File from disk
bool SourceView::loadFile(const QString &fileName)
 {
    SourceView::fileName = fileName;

     QFile file(fileName);
     if (!file.open(QFile::ReadOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("SharpEye Studio"),
                              tr("Cannot read file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return false;
     }

     QTextStream in(&file);
     QApplication::setOverrideCursor(Qt::WaitCursor);
     editor->setPlainText(in.readAll());
     QApplication::restoreOverrideCursor();

  return true;
}

/// Return true if document is modified
bool SourceView::isModified() const
{ return editor->document()->isModified(); }

/// Return the current filename
QString SourceView::filename() const
{ return fileName; }


/// Save changes to disk
bool SourceView::save(QString fname)
{
  if(fname == "") fname = fileName;

  QFile file(fname);

  if (!file.open(QFile::WriteOnly))
        return false;

  QTextStream ts(&file);
  ts.setCodec(QTextCodec::codecForName("UTF-8"));
  ts << editor->document()->toPlainText();
  editor->document()->setModified(false);
  emit saved(this);
  return true;
}

/// Return Current Document
QTextDocument * SourceView::document()
{
 return editor->document();
}

/// Return the editor object
QTextEdit * SourceView::getEditor()
{
 return editor;
}
