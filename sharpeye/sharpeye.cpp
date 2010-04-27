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

#define SHARP_VERSION "v2010"

#include <QtGui>
#include "sharpeye.h"
#include "sourcebrowser.h"
#include "projecttree.h"
#include "printpreview.h"
#include "patternview.h"
#include "settings.h"
#include "timedelay.h"
#include "cycletimer.h"

#ifdef Q_WS_WIN
   #define ASSISTANT_PATH "assistant.exe"
#else
   #define ASSISTANT_PATH "assistant"
#endif

/// Constructor
SharpEye::SharpEye() 
{
  sourcetab = new SourceBrowser();
  sourcetab->setMinimumSize(400, 205);
  //setWindowTitle(tr("%1[*] - %2").arg("test.est").arg(tr("SharpEye Studio")));

  setWindowTitle(QString("SharpEye Studio %1 - A Complete Framework for the Development of Motion Estimation Processors and Algorithms").arg(SHARP_VERSION));

  createActions();
  createMenus();
  createToolBars();
  createStatusBar();
  createDockWindows();
  setCentralWidget(sourcetab);
  showMaximized();
  initializeAssistant();
  estimo = new QProcess(this);
  cycleTimer = 0;
  connect(estimo, SIGNAL(finished(int, QProcess::ExitStatus) ), this, SLOT(compilerFinished()));
}

/// Destructor
SharpEye::~SharpEye()
{}

/// Create Docking windows
void SharpEye::createDockWindows()
{
  dockMes = new QDockWidget(tr("Messages"), this);
  dockMes->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);

  status = new QTextEdit(this);
  status->setReadOnly(true);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(status);

  dockMes->setWidget(status);

  addDockWidget(Qt::BottomDockWidgetArea, dockMes);

  dockExplorer = new QDockWidget(tr("Project Explorer"), this);
  dockExplorer->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  projectExplorer = new ProjectTree();

  connect(sourcetab, SIGNAL(fileOpen(QString)), 
                                 projectExplorer, SLOT(openFile(QString)) );

  connect(sourcetab, SIGNAL(fileClose(QString)),
             projectExplorer, SLOT(closeFile(QString)) );

  dockExplorer->setWidget(projectExplorer);
  addDockWidget(Qt::LeftDockWidgetArea, dockExplorer);
}

/// Create All actions
void SharpEye::createActions()
{
  // file Menu Actions
  newAct = new QAction(QIcon(":/SharpEye/images/document_plain_new.png"), tr("&New"), this);
  newAct->setShortcut(tr("Ctrl+N"));
  newAct->setStatusTip(tr("Create a new file"));
  connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

  cancelRun = new QAction(QIcon(":/SharpEye/images/cpu_stop.png"), tr("Sto&p Running..."), this);
  cancelRun->setDisabled(true);

  openAct = new QAction(QIcon(":/SharpEye/images/folder.png"), tr("&Open..."), this);
  openAct->setShortcut(tr("Ctrl+O"));
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

  saveAct = new QAction(QIcon(":/SharpEye/images/disk_blue.png"), tr("&Save"), this);
  saveAct->setShortcut(tr("Ctrl+S"));
  saveAct->setStatusTip(tr("Save file to disk"));
  connect(saveAct, SIGNAL(triggered()), sourcetab, SLOT(save()));

  saveAsAct = new QAction(QIcon(":/SharpEye/images/save_as.png"), tr("Save &As..."), this);
  saveAsAct->setStatusTip(tr("Save the document under a new name"));
  connect(saveAsAct, SIGNAL(triggered()), sourcetab, SLOT(saveAs()));

  closeAct = new QAction(tr("Close"), this);
  connect(closeAct, SIGNAL(triggered()), sourcetab, SLOT(closeTab()));

  closeAllAct = new QAction( tr("Close All"), this);
  connect(closeAllAct, SIGNAL(triggered()), sourcetab, SLOT(closeAll()));

  printAct = new QAction( QIcon(":/SharpEye/images/printer.png"), tr("Print..."), this);
  connect(printAct, SIGNAL(triggered()), sourcetab, SLOT(filePrintPreview()));

  saveAllAct= new QAction(QIcon(":/SharpEye/images/disks.png"), tr("Save All"), this);
  connect(saveAllAct, SIGNAL(triggered()), sourcetab, SLOT(saveAll()));

  pdfAct = new QAction( QIcon(":/SharpEye/images/pdf.png"), tr("Export pdf..."), this);
  connect(pdfAct, SIGNAL(triggered()), sourcetab, SLOT(filePrintPdf()));

  exitAct = new QAction(QIcon(":/SharpEye/images/exit.png"), tr("&Exit"), this);
  exitAct->setShortcut(tr("Ctrl+Q"));
  exitAct->setStatusTip(tr("Exit SharpEye Studio"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(quitIDE()) );

  // Edit Menu Actions
  undoAct  = new QAction(QIcon(":/SharpEye/images/undo.png"), tr("Undo"), this);
  undoAct->setShortcut(tr("Ctrl+Z"));

  redoAct  = new QAction(QIcon(":/SharpEye/images/redo.png"), tr("Redo"), this);
  redoAct->setShortcut(tr("Ctrl+Y"));

  cutAct   = new QAction(QIcon(":/SharpEye/images/cut.png"), tr("Cut"), this);
  cutAct->setShortcut(tr("Ctrl+X"));

  copyAct  = new QAction(QIcon(":/SharpEye/images/copy.png"), tr("Copy"), this);
  copyAct->setShortcut(tr("Ctrl+C"));

  pasteAct = new QAction(QIcon(":/SharpEye/images/paste.png"), tr("Paste"), this);
  pasteAct->setShortcut(tr("Ctrl+P"));

  gotoAct = new QAction( tr("Go To..."), this);
  gotoAct->setShortcut(tr("Ctrl+G"));

  findAct = new QAction(QIcon(":/SharpEye/images/find.png"), tr("Find..."), this);
  findAct->setShortcut(tr("Ctrl+F"));

  findNextAct = new QAction(QIcon(":/SharpEye/images/find_again.png"), tr("Find again"), this);
  findNextAct->setShortcut(tr("F3"));

  replaceAct = new QAction(QIcon(":/SharpEye/images/replace.png"), tr("Replace..."), this);
  replaceAct->setShortcut(tr("Ctrl+R"));

  // Help Menu actions
  helpcontAct   = new QAction(QIcon(":/SharpEye/images/help.png"), tr("Help Contents"), this);
  quickstartAct = new QAction(QIcon(":/SharpEye/images/help2.png"), 
                                                    tr("Quick Start Guide"), this);
  estimoRefAct  = new QAction(tr("Estimo Language Reference"), this);
  usingAct      = new QAction(tr("Using the SharpEye IDE"), this);
  webAct        = new QAction(QIcon(":/SharpEye/images/helpearth.png"), 
                                                            tr("SharpEye Studio Website"), this);

  connect(webAct, SIGNAL(triggered()), this, SLOT(website()));
  helpcontAct->setShortcut(tr("F1"));
  connect(helpcontAct, SIGNAL(triggered()), this, SLOT(assistant()));

  welcomeAct    = new QAction(tr("Welcome Screen"), this);
  aboutAct      = new QAction(QIcon(":/SharpEye/images/about.png"), 
                                                   tr("About SharpEye Studio"), this);

  buildAct    = new QAction(QIcon(":/SharpEye/images/cpu_run.png"), tr("Build"), this);
  buildAct->setShortcut(tr("F5"));
  connect(buildAct, SIGNAL(triggered()), this, SLOT(run())); 

  showCyclesAct = new QAction(QIcon(":/SharpEye/images/gear_time.png"), tr("Cycle-accurate model"), this);
  connect(showCyclesAct, SIGNAL(triggered()), this, SLOT(showCycles()));

  newProjectAct = new QAction(QIcon(":/SharpEye/images/document_plain_new.png"), 
                                                                    tr("New Project"), this);

  openProjectAct = new QAction(QIcon(":/SharpEye/images/folder.png"), tr("Open Project"), this);

  closeProjectAct = new QAction(QIcon(":/SharpEye/images/close.png"), tr("Close Project"), this);

  connect(cancelRun, SIGNAL(triggered()), this, SLOT(killEstimo())); 
  connect(newProjectAct, SIGNAL(triggered()), this, SLOT(newProject()));
  connect(openProjectAct, SIGNAL(triggered()), this, SLOT(openProject()));
  connect(closeProjectAct, SIGNAL(triggered()), this, SLOT(closeProject()));

  debugAct    = new QAction(QIcon(":/SharpEye/images/debug_run.png"), tr("Debug..."), this);
  loadChipAct = new QAction(QIcon(":/SharpEye/images/PCI-card.png"), 
                                                           tr("Load Code to MCU..."), this);
  settingsAct = new QAction(QIcon(":/SharpEye/images/wrench.png"), tr("Configuration"), this);

  showTreeExplorerAct = new QAction(QIcon(":/SharpEye/images/window_sidebar.png"), 
                                                               tr("Project Explorer"), this);
  showErrorListAct    = new QAction(QIcon(":/SharpEye/images/window_split_ver.png"), 
                                                                   tr("Error List"), this);
  newTableAct = new QAction(QIcon(":/SharpEye/images/table_sql_create.png"),
                                                       tr("New Pattern"), this);

  cleanProject = new QAction(tr("Clean files..."), this);

  connect(settingsAct, SIGNAL(triggered()), this, SLOT(configuration()));
  connect(newTableAct, SIGNAL(triggered()), this, SLOT(patternEditor()));

  helpcontAct->setStatusTip(tr("Show help contents"));
  webAct->setStatusTip(tr("Open browser to project's website..."));

  connect(aboutAct, SIGNAL(triggered()), this, SLOT(aboutSharpEye()));
  connect(showTreeExplorerAct, SIGNAL(triggered()), this, SLOT(hideExplorer()));
  connect(showErrorListAct, SIGNAL(triggered()), this, SLOT(hideMessages()));
  connect(undoAct, SIGNAL(triggered()), sourcetab, SLOT(undo()));
  connect(redoAct, SIGNAL(triggered()), sourcetab, SLOT(redo()));
  connect(cutAct, SIGNAL(triggered()), sourcetab, SLOT(cut()));
  connect(copyAct, SIGNAL(triggered()), sourcetab, SLOT(copy()));
  connect(pasteAct, SIGNAL(triggered()), sourcetab, SLOT(paste()));
  connect(cleanProject, SIGNAL(triggered()), this, SLOT(clean()));

  cutAct->setEnabled(false);
  copyAct->setEnabled(false);
  saveAct->setEnabled(false);
  undoAct->setEnabled(false);
  redoAct->setEnabled(false);
  saveAllAct->setEnabled(false);
  saveAsAct->setEnabled(false);

  connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
  connect(sourcetab, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
  connect(sourcetab, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
  connect(sourcetab, SIGNAL(modificationChanged(bool)),
                             saveAct, SLOT(setEnabled(bool)));
  connect(sourcetab, SIGNAL(modificationChanged(bool)),
                             saveAsAct, SLOT(setEnabled(bool)));
  connect(sourcetab, SIGNAL(modificationChanged(bool)),
                             saveAllAct, SLOT(setEnabled(bool)));
  connect(sourcetab, SIGNAL(undoAvailable(bool)),
            undoAct, SLOT(setEnabled(bool)));

  connect(sourcetab, SIGNAL(redoAvailable(bool)),
            redoAct, SLOT(setEnabled(bool)));
}

void SharpEye::quitIDE()
{
	qApp->exit(0);
}

/// Show About Dialog
void SharpEye::aboutSharpEye()
{
  QMessageBox::about(this, tr("SharpEye Studio 2009"), 
  tr("<b>University of Bristol</b><p>Copyright (C) 2009 by George Vafiadis" \
	 "<p>Copyright (C) 2009 by Trevor Spiteri " \
     "<p>Copyright (C) 2009 by Jose Luis Nunez-Yanez " \
	 "<p>A complete Integrated Environment for the Development of Motion Estimation Algorithms."));
}

/// Create Menu Bar
void SharpEye::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  fileMenu->addSeparator();
  fileMenu->addAction(closeAct);
  fileMenu->addAction(closeAllAct);
  fileMenu->addSeparator();
  fileMenu->addAction(saveAct);
  fileMenu->addAction(saveAsAct);
  fileMenu->addAction(saveAllAct);
  fileMenu->addSeparator();
  fileMenu->addAction(printAct);

#ifndef QT_NO_PRINTER
  fileMenu->addAction(pdfAct);
#endif

  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(undoAct);
  editMenu->addAction(redoAct);
  editMenu->addSeparator();
  editMenu->addAction(cutAct);
  editMenu->addAction(copyAct);
  editMenu->addAction(pasteAct); 
/*
  editMenu->addSeparator();
  //editMenu->addAction(gotoAct);
  editMenu->addSeparator();
  editMenu->addAction(findAct);
  editMenu->addAction(findNextAct);
  editMenu->addAction(replaceAct);
  projectMenu = menuBar()->addMenu(tr("&Project"));
  projectMenu->addAction(newProjectAct);
  projectMenu->addAction(openProjectAct);
  projectMenu->addAction(closeProjectAct);
*/

  buildMenu   = menuBar()->addMenu(tr("&Build"));
  buildMenu->addAction(buildAct);
  buildMenu->addAction(cancelRun);
  buildMenu->addSeparator();
  buildMenu->addAction(cleanProject);
  buildMenu->addSeparator();
  buildMenu->addAction(showCyclesAct);

/*
  settingsMenu = menuBar()->addMenu(tr("&Settings")); 
  settingsMenu->addAction(settingsAct);
*/
  menuBar()->addSeparator();
  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(helpcontAct);
  // helpMenu->addSeparator();
  // helpMenu->addAction(quickstartAct);
  // helpMenu->addAction(estimoRefAct);
  // helpMenu->addAction(usingAct);
  helpMenu->addSeparator();
  helpMenu->addAction(webAct);
  helpMenu->addSeparator();
  // helpMenu->addAction(welcomeAct);
  helpMenu->addAction(aboutAct);
}

/// Create toolBars
void SharpEye::createToolBars()
{
 standardToolBar = addToolBar(tr("Standard"));

 standardToolBar->addAction(newAct);
 standardToolBar->addAction(openAct);
 standardToolBar->addAction(saveAct);
 standardToolBar->addAction(saveAllAct);
 standardToolBar->addSeparator();
 standardToolBar->addAction(cutAct);
 standardToolBar->addAction(copyAct);
 standardToolBar->addAction(pasteAct); 
 standardToolBar->addSeparator(); 
 standardToolBar->addAction(undoAct);
 standardToolBar->addAction(redoAct);
 //standardToolBar->addSeparator(); 
 //standardToolBar->addAction(findAct);

 buildToolBar = addToolBar(tr("Build"));
 buildToolBar->addAction(buildAct);
 buildToolBar->addSeparator(); 
 buildToolBar->addAction(cancelRun);
 buildToolBar->addSeparator();
 buildToolBar->addAction(showCyclesAct);
 //buildToolBar->addAction(debugAct);
 //buildToolBar->addAction(loadChipAct);
 //buildToolBar->addAction(settingsAct);
  
 toolbars = addToolBar(tr("ToolBars"));
 toolbars->addAction(newTableAct);
 toolbars->addAction(showTreeExplorerAct);
 toolbars->addAction(showErrorListAct);
 toolbars->addSeparator(); 
 toolbars->addAction(helpcontAct);
}

/// Create Status Bar
void SharpEye::createStatusBar()
{
  statusBar()->showMessage(tr("Ready"));
}

/// The user closed the main window
void SharpEye::closeEvent(QCloseEvent *)
{
 if (assistantClient)
        assistantClient->closeAssistant();
}

/// Create New File
void SharpEye::newFile()
{
 QString fileName = QFileDialog::getSaveFileName(this);

 if( fileName.isEmpty() ) return;
 QFile file(fileName);

 if( !file.exists() )
 {
     if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
         return;

     QTextStream out(&file);
     out << "/// Copyright (C) XXXX-XXX XXXXXXXXXXX" << "\n";
     out << "/// Source " << fileName << "\n" << "\n";
 }

  sourcetab->open(fileName);
}

/// Run Estimo Compiler
void SharpEye::run()
{
	 sourcetab->save();

	 QString appdir = QApplication::applicationDirPath();
     QString program = appdir + QString("/estimo");
     QStringList arguments;
     QString fname = sourcetab->getCurrentFile();
     if(fname == "" ) return;
     arguments << fname;

	 cancelRun->setDisabled(false);
	 buildAct->setDisabled(true);

	 QString output;

	 output  = "SharpEye Studio " +  QString(SHARP_VERSION) + " IDE\n";
	 output += "University of Bristol. ";
     output += "Copyright (C) 2009 Jose Nunez-Yanez, Trevor Spiteri, George Vafiadis\n";
     output += QString("Compiling '%1'...\n\n").arg(fname);
     output += QString("Estimo Interpreter is running...\n\n").arg(fname);

	 /// DEBUG
	 QProcess updatest;
	 updatest.start("update.bat", QStringList());

	 status->clear();
	 gsleep(1);

	 status->setText(output);

     estimo->start(program, arguments);
}

/// Time cycles using cycle accurate model
void SharpEye::showCycles()
{
	if (!cycleTimer)
		cycleTimer = new CycleTimer();
	cycleTimer->show();
	cycleTimer->activateWindow();
}

/// Compiler has Finished
void SharpEye::compilerFinished()
{
	QString output = status->toPlainText();

     QByteArray result = estimo->readAll();
     QString res(result);

	 bool allOK = false;

     if(res.contains("done", Qt::CaseInsensitive) )
     {
		 allOK = true;
	 }
     
     if(res == "") res = "Interpretation Failure!\n";
     output += res;

	 status->setText(output);
	 cancelRun->setDisabled(true);
	 buildAct->setDisabled(false);

	 if( allOK )
	 {
		     QString fname = sourcetab->getCurrentFile();
			 QFileInfo info(fname);

			 OutputDialog outputDlg(info.dir().path() + "/estimo.output", output, this);
			 
	         outputDlg.exec();
	 }
}

/// Open new file
void SharpEye::openFile(QString fileName)
{
	QString fn;

	if( fileName.isEmpty() )
        fn = QFileDialog::getOpenFileName(this, tr("Open File..."), QString(), tr("Estimo C Files (*.est)"));
	else
		fn = fileName;

    if( !fn.isEmpty() )
         sourcetab->open(fn);
}

/// Pattern Editor Dialog
void SharpEye::patternEditor()
{
  PatternDialog patDialog;
  patDialog.exec();
}

/// Show/Hide Project's Explorer
void SharpEye::hideExplorer()
{
  dockExplorer->setVisible(!dockExplorer->isVisible());
}

/// Show/Hide Compiler messages
void SharpEye::hideMessages()
{
  dockMes->setVisible(!dockMes->isVisible());
}

/// If data waiting in clipboard
void SharpEye::clipboardDataChanged()
{
  pasteAct->setEnabled(!QApplication::clipboard()->text().isEmpty());
}

/// Open Project's Web site
void SharpEye::website()
{
 QDesktopServices::openUrl(QUrl("http://sharpeye.borelspace.com"));
}

/// Call Assistant to show help
void SharpEye::assistant()
{
  /*assistantClient->showPage(QApplication::applicationDirPath() 
   + QDir::separator() + "manual" +  QDir::separator() + "index.html");*/

 QProcess process;
 QStringList arguments;
 arguments << "tutorial.pdf";
 process.startDetached(QApplication::applicationDirPath() + QDir::separator() + "SumatraPDF.exe", arguments);
}

/// Initialize Assistant 
void SharpEye::initializeAssistant()
{
   assistantClient = new QAssistantClient(QApplication::applicationDirPath()  
     +  QDir::separator() + ASSISTANT_PATH , this);

    QStringList arguments;
    arguments << "-profile" << QApplication::applicationDirPath()  +
     QDir::separator() + "manual" + QDir::separator() + QString("sharpeye.adp");
    assistantClient->setArguments(arguments);
}

/// Show Configuration Dialog
void SharpEye::configuration()
{
  ConfigDialog dialog;
  dialog.exec();
}

/// Create New Project
void SharpEye::newProject()
{
 QString fileName = QFileDialog::getSaveFileName(this);
}

/// Open Existing Project from disk
void SharpEye::openProject()
{}

/// Close Current Project and all files
void SharpEye::closeProject()
{}

/// Kill the compiler instance
void SharpEye::killEstimo()
{
 estimo->kill();

 buildAct->setDisabled(false);
 closeAct->setDisabled(true);

 status->setText("Interpreter stopped by the user.\n");
}

/// Clean Project and all output files
void SharpEye::clean()
{
 QFile as("estimo.output/assembly.asm");
 QFile pat("estimo.output/patterns.mif"); 
 QFile prg("estimo.output/program.mif");

 status->setText("Clean started...\n" \
	            "Deleting intermediate and output files.\n" \
				"Cleaning completed successfully.\n");
 as.remove();
 pat.remove();
 prg.remove();
}
