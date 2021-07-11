/*
cfileselect.h/cpp
-----------------
Begin: 2008/02/18
Author: Aaron Reeves <aaron.reeves@naadsm.org>
--------------------------------------------------
Copyright (C) 2008 - 2019 Aaron Reeves

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
  _firstClick = true;
}


/* This seems to be the only event that matters for my purposes, but there are others that
 *  might need to be explored if things don't work as expected:
 *
 * enterEvent(QEvent *) (I tried this one, and it seemed to be the wrong one)
 * leaveEvent(QEvent *)
 *
 * changeEvent(QEvent *)
 *
 * keyPressEvent(QKeyEvent *)
 * keyReleaseEvent(QKeyEvent *)
 *
 * focusInEvent(QFocusEvent *) (In use: see below)
 * focusOutEvent(QFocusEvent *) (In use: see below)
 *
 * contextMenuEvent(QContextMenuEvent *)
 * inputMethodEvent(QInputMethodEvent *)
 *
 * mouseDoubleClickEvent(QMouseEvent *)
 * mouseMoveEvent(QMouseEvent *)
 * mousePressEvent(QMouseEvent *)
 * mouseReleaseEvent(QMouseEvent *) (In use: see below)
 * wheelEvent(QWheelEvent *)
 *
 * dragEnterEvent(QDragEnterEvent *) (These could be useful some day, if I want drag/drop capability)
 * dragLeaveEvent(QDragLeaveEvent *)
 * dragMoveEvent(QDragMoveEvent *)
 * dropEvent(QDropEvent *)
 */

void CMyLineEdit::setActualPathName( const QString& val ) {
  _actualPathName = val;
}


CFileSelect::CFileSelect( QWidget* parent, Qt::WindowFlags f ) : QWidget( parent, f ), ui(new Ui::CFileSelect) {
  ui->setupUi(this);

  this->setFocusProxy( ui->btnBrowse );

  // Filter format: "Plain text files (*.txt);; All Files (*.*)"
  _filter = QStringLiteral( "All Files (*.*)" );
  _caption = QStringLiteral( "Please select a file" );
  _mode = ModeUnspecified;

  connect( ui->btnBrowse, SIGNAL( clicked() ), this, SLOT( selectFileOrFolder() ) );
  connect( ui->leFilePath, SIGNAL( editingFinished() ), this, SLOT( slotEditingFinished() ) );

  setReadOnly( false );

  ui->btnTest->setVisible( false );
  connect( ui->btnTest, SIGNAL( clicked() ), this, SLOT( debug() ) );
}


CFileSelect::~CFileSelect() {
  delete ui;
}

// Simple properties and helper functions
//---------------------------------------
void CFileSelect::setReadOnly( const bool readOnly ) {
  ui->btnBrowse->setEnabled( !readOnly );
  ui->leFilePath->setReadOnly( readOnly );

  _isReadOnly = readOnly;
}


bool CFileSelect::dirExists() const {
  if( ModeExistingDir == this->_mode )
    return fileExists();
  else {
    return QFileInfo( ui->leFilePath->_actualPathName ).dir().exists();
  }
}


bool CFileSelect::fileExists() const {
  return QFileInfo::exists( ui->leFilePath->_actualPathName );
}


void CFileSelect::clearPath() {
  if( _isReadOnly )
    return;

  if( !ui->leFilePath->_actualPathName.isEmpty() )
    _lastUsedPath = ui->leFilePath->_actualPathName;

  ui->leFilePath->_actualPathName = QString();
  ui->leFilePath->setText( QString() );
  emit pathNameChanged( ui->leFilePath->_actualPathName );
}

bool CFileSelect::isEmpty() const {
  return ui->leFilePath->_actualPathName.isEmpty();
}


QString CFileSelect::startDirectory() {
  QFileInfo fi;

  if( !ui->leFilePath->_actualPathName.isEmpty() )
    fi = QFileInfo( ui->leFilePath->_actualPathName );
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
QString CFileSelect::directory() const {
  Q_ASSERT( ModeUnspecified != _mode );

  if( ModeExistingDir & _mode )
    return QFileInfo( ui->leFilePath->_actualPathName ).absoluteFilePath();
  else if( ModeUnspecified != _mode )
    return QFileInfo( ui->leFilePath->_actualPathName ).absolutePath();
  else
    return QString();
}


QString CFileSelect::pathName() const {
  Q_ASSERT( ModeUnspecified != _mode );

  if( ModeUnspecified != _mode )
    return QFileInfo( ui->leFilePath->_actualPathName ).absoluteFilePath();
  else
    return QString();
}


void CFileSelect::selectFolder() {
  if( _isReadOnly )
    return;

  QString selectedDirectory;
  QString startingDirectory = startDirectory();

  selectedDirectory = QFileDialog::getExistingDirectory( this, _caption, startingDirectory );

  if( !selectedDirectory.isEmpty() ) {
    ui->leFilePath->_actualPathName = selectedDirectory;
    ui->leFilePath->setText( abbreviatePath( ui->leFilePath->_actualPathName, 90 ) );
    emit pathNameChanged( ui->leFilePath->_actualPathName );
  }
}


void CFileSelect::selectFile() {
  if( _isReadOnly )
    return;

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
      selectedFile = QString();
  }
  else if( _mode & ModeSaveFile ) {
    selectedFile = QFileDialog::getSaveFileName( this, _caption, startingDirectory, _filter );
  }
  else {
    qDb() << "There is a problem in CFileSelect::selectFile()" << _mode;
    Q_ASSERT( false );
    selectedFile = QString();
  }

  if( !selectedFile.isEmpty() ) {
    ui->leFilePath->_actualPathName = selectedFile;
    ui->leFilePath->setText( abbreviatePath( ui->leFilePath->_actualPathName, 90 ) );
    emit pathNameChanged( ui->leFilePath->_actualPathName );
  }
}


void CFileSelect::selectFileOrFolder() {
  if( _isReadOnly )
    return;

  if( _mode & ModeExistingDir )
    selectFolder();
  else
    selectFile();
}


void CFileSelect::setPathName( const QString& val ) {
  if( _isReadOnly )
    return;

  ui->leFilePath->_actualPathName = val.trimmed();
  ui->leFilePath->setText( abbreviatePath( ui->leFilePath->_actualPathName, 90 ) );
  emit pathNameChanged( ui->leFilePath->_actualPathName );
}


// Events, etc.
//-------------
void CMyLineEdit::focusInEvent( QFocusEvent* event ) {
  QLineEdit::focusInEvent( event );
  // qDb() << "focusInEvent" << _actualPathName;
  this->setText( _actualPathName );
  this->setCursorPosition( this->text().length() - 1 ); // Works with tab, but not on mouse click.  Use release event to catch mouse activity.
}


void CMyLineEdit::focusOutEvent( QFocusEvent* event ) {
  _firstClick = true;
  _actualPathName = this->text().trimmed();
  this->setText( abbreviatePath( _actualPathName, 90 ) );

  //qDb() << "focusOutEvent" << _actualPathName;
  QLineEdit::focusOutEvent( event );
}


void CMyLineEdit::mousePressEvent(QMouseEvent* event ) {
  //qDb() << "Mouse press event";
  QLineEdit::mousePressEvent( event );
}


void CMyLineEdit::mouseReleaseEvent(QMouseEvent* event) {
  //qDb() << "Mouse release event";
  QLineEdit::mouseReleaseEvent( event );
  if( _firstClick ) {
    this->setCursorPosition( this->text().length() );
    _firstClick = false;
  }
}


void CFileSelect::slotEditingFinished() {
  if( !_isReadOnly )
    emit pathNameChanged( ui->leFilePath->_actualPathName );
}





