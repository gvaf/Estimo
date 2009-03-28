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

#include <QtGui>
#include <QFileInfo>

#include "sourcebrowser.h"
#include "sourceview.h"
#include "printpreview.h"

#define ESTIMO_SUFFIX     "est"
#define ASM_SUFFIX        "asm"
#define ESTIMO_SOURCE_ICO ":/SharpEye/images/source.png"
#define ASM_SOURCE_ICO    ":/SharpEye/images/source_s.png"
#define UNSAVED_ICO       ":/SharpEye/images/disk_blue.png"

/// Constructor
SourceBrowser::SourceBrowser()
{
  QToolButton * closeTabButton = new QToolButton(this);
  setCornerWidget(closeTabButton, Qt::TopRightCorner);
  closeTabButton->setCursor(Qt::ArrowCursor);
  closeTabButton->setAutoRaise(true);
  closeTabButton->setIcon(QIcon(":/SharpEye/images/close.png"));
  closeTabButton->setToolTip(tr("Close page"));
  closeTabButton->setEnabled(true);

  QObject::connect(closeTabButton, SIGNAL(clicked()), this, SLOT(closeTab()));
}

/// Destructor
SourceBrowser::~SourceBrowser()
{
 QHashIterator<QString, SourceView *> i(pages);

 while( i.hasNext() )
 {
     i.next();
     delete i.value();
 }
}

/// The user pressed the close tab button
void SourceBrowser::closeTab()
{
 if( count() == 0 ) return;
  
  SourceView * currentpage = static_cast<SourceView *>(currentWidget());
  
  if( currentpage->isModified() )
  {
   int ret = QMessageBox::warning(this, tr("SharpEye Studio"),
                   tr("The file '%1' has been modified.\n"
                      "Do you want to save your changes?").arg(currentpage->filename()),
                   QMessageBox::Save | QMessageBox::Discard
                   | QMessageBox::Cancel,
                   QMessageBox::Save);

    if(ret == QMessageBox::Save)
        currentpage->save();
    else if(ret == QMessageBox::Cancel)
       return;
 }

 const QString & currentfile = currentpage->filename();

 if( pages.contains(currentfile) )
  {
   removeTab(indexOf(pages[currentfile])); 
   delete pages[currentfile];
   pages.remove(currentfile);
   emit fileClose(currentfile);
  }
}

/// Load a file from disk if it is not already loaded
void SourceBrowser::open(const QString & fileName)
{
 QFileInfo info(fileName);

 /// Verify the existance of the file
 if( !info.exists() || !info.isReadable() )
  {
    QMessageBox::warning(this, tr("SharpEye Studio"),
                               tr("Cannot read file '%1'.")
                              .arg(fileName));
    return;
  }

 /// if fileName is already open then make it current
 if( pages.contains(fileName) )
  {
    setCurrentWidget(pages[fileName]);
  }
 else
  {
   /// Create a new source view and load the file
    pages[fileName] = new SourceView();
    pages[fileName]->loadFile(fileName);

    connect( pages[fileName], SIGNAL(modified(QWidget *)), this, SLOT(unsavedICO(QWidget *)));
    connect( pages[fileName], SIGNAL(saved(QWidget *)), this, SLOT(savedICO(QWidget *)));
    QTextEdit *edi = pages[fileName]->getEditor();

    /// Show the new textedit control
    addTab(pages[fileName], QIcon( icoType(info.suffix()) ), info.fileName());

    connect( edi->document(), SIGNAL(modificationChanged(bool)), this,
                              SIGNAL(modificationChanged(bool)));
    connect( edi->document(), SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
    connect( edi->document(), SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));
    connect( edi, SIGNAL(copyAvailable(bool)), this, SIGNAL(copyAvailable(bool)));

    emit fileOpen(fileName);
    setCurrentWidget(pages[fileName]);
  }
}

/// Return the appropriate file icon based on file suffix
QString SourceBrowser::icoType(const QString & suffix)
{
   QString ico;

    /// Choose the right icon based on file type
    if( suffix.compare(ESTIMO_SUFFIX, Qt::CaseInsensitive) == 0)
      ico = ESTIMO_SOURCE_ICO;
    else if( suffix.compare(ASM_SUFFIX, Qt::CaseInsensitive) == 0 )
      ico = ASM_SOURCE_ICO;

 return ico;
}

/// Change ICO to unsaved ICO
void SourceBrowser::unsavedICO(QWidget *w)
{
  setTabIcon(indexOf(w), QIcon(UNSAVED_ICO));
}

/// Restore ICO
void SourceBrowser::savedICO(QWidget *w)
{
 SourceView * sv = (SourceView *)w;

 QFileInfo info(sv->filename());

  setTabIcon(indexOf(w), QIcon( icoType(info.suffix()) ));
}

/// Save Current file
void SourceBrowser::save()
{
 if( count() == 0 ) return;
  
  SourceView * currentpage = static_cast<SourceView *>(currentWidget());
  currentpage->save(); 
}

/// Save all currently opened files
void SourceBrowser::saveAll()
{
 for(int i = 0; i < count(); ++i)
 {
  SourceView * currentpage = static_cast<SourceView *>(widget(i));
  currentpage->save(); 
 }
}

/// Close All Tabs
void SourceBrowser::closeAll()
{
 while(count() != 0) 
 {
  setCurrentIndex(0);
  closeTab();
 }
}

/// Save Current file with a new name
void SourceBrowser::saveAs()
{
 if( count() == 0 ) return;
 QString fileName = QFileDialog::getSaveFileName(this);

 if( fileName.isEmpty() ) return;

  SourceView * currentpage = static_cast<SourceView *>(currentWidget());
  currentpage->save(fileName);  
}

/// Show Print Preview
void SourceBrowser::filePrintPreview()
{
 if( count() == 0 ) return;

    SourceView * currentpage = static_cast<SourceView *>(currentWidget());
    PrintPreview *preview = new PrintPreview(currentpage->document(), this);
    preview->setWindowModality(Qt::WindowModal);
    preview->setAttribute(Qt::WA_DeleteOnClose);
    preview->show();
}

/// Output as pdf file
void  SourceBrowser::filePrintPdf()
{
#ifndef QT_NO_PRINTER

 if( count() == 0 ) return;
    SourceView * currentpage = static_cast<SourceView *>(currentWidget());

    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",
                                                    QString(), "*.pdf");
    if (!fileName.isEmpty()) {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        currentpage->document()->print(&printer);
    }
#endif
}

/// Get current file
QString SourceBrowser::getCurrentFile()
{
 if( count() == 0 ) return "";

 SourceView * currentpage = static_cast<SourceView *>(currentWidget());
 const QString & currentfile = currentpage->filename();

 return currentfile;
}

/// Undo last operation at the current texteditor
void SourceBrowser::undo()
{
 if( count() == 0 ) return;

 SourceView * currentpage = static_cast<SourceView *>(currentWidget());
 currentpage->getEditor()->undo();
}

/// Redo last operation at the current texteditor
void SourceBrowser::redo()
{
 if( count() == 0 ) return;

 SourceView * currentpage = static_cast<SourceView *>(currentWidget());
 currentpage->getEditor()->redo();
}

/// cut text at the current texteditor
void SourceBrowser::cut()
{
 if( count() == 0 ) return;

 SourceView * currentpage = static_cast<SourceView *>(currentWidget());
 currentpage->getEditor()->cut();
}

/// Copy text at the current texteditor
void SourceBrowser::copy()
{
 if( count() == 0 ) return;

 SourceView * currentpage = static_cast<SourceView *>(currentWidget());
 currentpage->getEditor()->copy();
}

/// Paste text at the current texteditor
void SourceBrowser::paste()
{
 if( count() == 0 ) return;

 SourceView * currentpage = static_cast<SourceView *>(currentWidget());
 currentpage->getEditor()->paste();
}
