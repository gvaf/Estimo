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

#ifndef SHARPEYE_H
#define SHARPEYE_H

#include <QtGui/QMainWindow>
#include <QTextEdit>
#include <QAssistantClient>

class SourceBrowser; 
class QTreeWidget;
class ProjectTree;

namespace CT {
class CycleTimer;
}
using CT::CycleTimer;

class SharpEye : public QMainWindow
{
  Q_OBJECT
 public:
    SharpEye();
   ~SharpEye();

 public slots:
    void openFile(QString fileName = "");

 private slots:
    void aboutSharpEye();
    void newFile();
   
    void patternEditor();
    void run();
    void showCycles();
    void hideExplorer();
    void hideMessages();
    void clipboardDataChanged();
    void website();
    void assistant();
    void configuration();
    void newProject();
    void openProject();
    void closeProject();
    void clean();
    void compilerFinished();
    void killEstimo();
	void quitIDE();

 private:
    void createActions();
    void createMenus();
    void createDockWindows();
    void createToolBars();
    void createStatusBar();
    void initializeAssistant();

 protected:
    void closeEvent(QCloseEvent *event);

 private:	
     SourceBrowser *sourcetab;
     QTextEdit     *status;
     ProjectTree   *projectExplorer;
     QDockWidget   *dockMes;
     QDockWidget   *dockExplorer;
     QAssistantClient *assistantClient;
     QProcess *estimo;
     CycleTimer *cycleTimer;

     QMenu *fileMenu;
     QMenu *editMenu;
     QMenu *viewMenu;
     QMenu *projectMenu;
     QMenu *buildMenu;
     QMenu *debugMenu;
     QMenu *settingsMenu;
     QMenu *toolsMenu;
     QMenu *helpMenu;

     QToolBar *buildToolBar;
     QToolBar *standardToolBar;
     QToolBar *toolbars;

	 QAction *cancelRun;
     QAction *newAct;
     QAction *openAct;
     QAction *closeAct;
     QAction *closeAllAct;
     QAction *printAct;
     QAction *saveAct;
     QAction *saveAsAct;
     QAction *saveAllAct; 
     QAction *pdfAct;
     QAction *exitAct;

	 QAction *cleanProject;
     QAction *undoAct;
     QAction *redoAct;
     QAction *cutAct;
     QAction *copyAct;
     QAction *pasteAct;
     QAction *selectAllAct;
     QAction *gotoAct;
     QAction *findAct;
     QAction *findNextAct; 
     QAction *replaceAct;

     QAction *buildAct;
     QAction *showCyclesAct;

     QAction *newProjectAct;
     QAction *openProjectAct;
     QAction *closeProjectAct;


     QAction *debugAct;
     QAction *loadChipAct;
     QAction *settingsAct;
     QAction *showTreeExplorerAct;
     QAction *showErrorListAct;
     QAction *newTableAct;

     QAction *helpcontAct;
     QAction *quickstartAct;
     QAction *webAct;
     QAction *estimoRefAct;
     QAction *usingAct;
     QAction *welcomeAct; 
     QAction *aboutAct;
};

#endif // SHARPEYE_H
