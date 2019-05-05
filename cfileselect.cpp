/*
cfileselect.h/cpp
-----------------
Begin: 2008/02/18
Author: Aaron Reeves <aaron.reeves@naadsm.org>
--------------------------------------------------
Copyright (C) 2008 Aaron Reeves

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/

#include "cfileselect.h"
#include "ui_cfileselect.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>

#include <ar_general_purpose/arcommon.h>

CMyLineEdit::CMyLineEdit( QWidget* parent ) : QLineEdit( parent ) {
  // Nothing to do here
}


void CMyLineEdit::focusInEvent( QFocusEvent* event ) {
  this->setText( _actualPathName );

  QLineEdit::focusInEvent( event );

  this->end( false );
}


void CMyLineEdit::setActualPathName( QString val ) {
  _actualPathName = val;
}


CFileSelect::CFileSelect( QWidget* parent, Qt::WindowFlags f ) : QWidget( parent, f ), ui(new Ui::CFileSelect) {
  ui->setupUi(this);

  this->setFocusProxy( ui->leFilePath );

  // Filter format: "Plain text files (*.txt);; All Files (*.*)"
  _filter = "All Files (*.*)";
  _caption = "Please select a file";
  _actualPathName = QString();
  _mode = ModeUnspecified;

  connect( ui->btnBrowse, SIGNAL( clicked() ), this, SLOT( selectFileOrFolder() ) );
  connect( this, SIGNAL( pathNameChanged( QString ) ), this, SLOT( updateLineEdit( QString ) ) );
  connect( this, SIGNAL( pathNameChanged( QString ) ), ui->leFilePath, SLOT( setActualPathName( QString ) ) );
  connect( ui->leFilePath, SIGNAL( editingFinished() ), this, SLOT( slotEditingFinished() ) );

  ui->btnTest->setVisible( false );
  connect( ui->btnTest, SIGNAL( clicked() ), this, SLOT( debug() ) );
}


CFileSelect::~CFileSelect() {
  delete ui;
}

// Simple properties and helper functions
//---------------------------------------
bool CFileSelect::dirExists() const {
  if( ModeExistingDir == this->_mode )
    return fileExists();
  else {
    return QFileInfo( _actualPathName ).dir().exists();
  }
}

bool CFileSelect::fileExists() const {
  return QFileInfo( _actualPathName ).exists();
}

void CFileSelect::clearPath() {
  if( !_actualPathName.isEmpty() )
    _lastUsedPath = _actualPathName;

  _actualPathName = QString();
  emit pathNameChanged( _actualPathName );
}

QString CFileSelect::startDirectory() {
  QFileInfo fi;

  if( !_actualPathName.isEmpty() )
    fi = QFileInfo( _actualPathName );
  else if( !_lastUsedPath.isEmpty() )
    fi = QFileInfo( _lastUsedPath );

  if( fi.isDir() )
    return fi.absoluteFilePath();
  else
    return fi.absolutePath();
}

void CFileSelect::setLabel( const QString& val ){
  ui->label->setText( val );
}

QString CFileSelect::label() const {
  return ui->label->text();
}


void CFileSelect::debug() {
  qDb() << "Mode:" << this->mode();
  qDb() << "Filter:" << this->specifiedFilter();
  qDb() << "File name:" << this->pathName();
  qDb() << "Directory name:" << this->directory();
  qDb() << "Last used path:" << _lastUsedPath;
}


// Getter and setter-type functions
//---------------------------------
QString CFileSelect::directory( void ) const {
  Q_ASSERT( ModeUnspecified != _mode );

  if( ModeExistingDir & _mode )
    return QFileInfo( _actualPathName ).absoluteFilePath();
  else if( ModeUnspecified != _mode )
    return QFileInfo( _actualPathName ).absolutePath();
  else
    return QString();
}


QString CFileSelect::pathName( void ) const {
  Q_ASSERT( ModeUnspecified != _mode );

  if( ModeUnspecified != _mode )
    return QFileInfo( _actualPathName ).absoluteFilePath();
  else
    return QString();
}


void CFileSelect::selectFolder() {
  QString selectedDirectory;
  QString startingDirectory = startDirectory();

  selectedDirectory = QFileDialog::getExistingDirectory( this, _caption, startingDirectory );

  if( !selectedDirectory.isEmpty() ) {
    _actualPathName = selectedDirectory;
    emit pathNameChanged( _actualPathName );
  }
}


void CFileSelect::selectFile() {
  QString startingDirectory = startDirectory();
  QString selectedFile;
  QFileDialog dialog(this);

  if( _mode & ModeOpenFile ) {
    dialog.setDirectory( startingDirectory );
    dialog.setNameFilter( _filter );

    if( _mode & ModeExistingFile )
      dialog.setFileMode( QFileDialog::ExistingFile );
    else
      dialog.setFileMode( QFileDialog::AnyFile );

    if( dialog.exec() )
      selectedFile = dialog.selectedFiles().at(0);
    else
      selectedFile = "";
  }
  else if( _mode & ModeSaveFile ) {
    selectedFile = QFileDialog::getSaveFileName( this, _caption, startingDirectory, _filter );
  }
  else {
    qDb() << "There is a problem in CFileSelect::selectFile()" << _mode;
    Q_ASSERT( false );
    selectedFile = "";
  }

  if( !selectedFile.isEmpty() ) {
    _actualPathName = selectedFile;
    emit pathNameChanged( _actualPathName );
  }
}


void CFileSelect::selectFileOrFolder() {
  if( _mode & ModeExistingDir )
    selectFolder();
  else
    selectFile();
}


void CFileSelect::setPathName( const QString& val ) {
  _actualPathName = val.trimmed();
  emit pathNameChanged( _actualPathName );
}


// Events, etc.
//-------------
void CFileSelect::updateLineEdit( QString newPathName ) {
  ui->leFilePath->setText( abbreviatePath( newPathName, 90 ) );
}


void CFileSelect::slotEditingFinished() {
  _actualPathName = ui->leFilePath->text().trimmed();
  emit pathNameChanged( _actualPathName );
}





