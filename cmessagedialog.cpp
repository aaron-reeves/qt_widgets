/*
cmessagedialog.h/cpp
--------------------
Begin: 2008/02/18
Author: Aaron Reeves <aaron.reeves@naadsm.org>
--------------------------------------------------
Copyright (C) 2008 Aaron Reeves

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/

#include "cmessagedialog.h"
#include "ui_cmessagedialog.h"

#include <QtCore>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QClipboard>

CMessageDialog::CMessageDialog(QWidget *parent) : QDialog(parent), ui(new Ui::CMessageDialog) {
  initializeUi( QMessageBox::NoIcon );
}


CMessageDialog::CMessageDialog( const QString& title, const QString& caption, QMessageBox::Icon iconStyle, QWidget *parent ) : QDialog(parent), ui(new Ui::CMessageDialog) {
  initializeUi( iconStyle );

  setWindowTitle( title );
  setCaption( caption );
  clear();

  _error = false;
}


CMessageDialog::~CMessageDialog() {
  delete ui;
}


void CMessageDialog::initializeUi( QMessageBox::Icon iconStyle ) {
  ui->setupUi(this);
  ui->label_2->setFrameShape( QFrame::NoFrame );

  connect( ui->btnOK, SIGNAL( clicked() ), this, SLOT( close() ) );
  connect( ui->btnCopy, SIGNAL( clicked() ), this, SLOT( copyToClipboard() ) );
  connect( ui->btnSave, SIGNAL( clicked() ), this, SLOT( save() ) );

  QMessageBox* mb = new QMessageBox();

  QStyle *style = mb ? mb->style() : QApplication::style();

  QIcon tmpIcon;

  switch( iconStyle ) {
    case QMessageBox::NoIcon:
      // Do nothing
      break;
    case QMessageBox::Information:
      tmpIcon = style->standardIcon( QStyle::SP_MessageBoxInformation );
      break;
    case QMessageBox::Warning:
      tmpIcon = style->standardIcon( QStyle::SP_MessageBoxWarning );
      break;
    case QMessageBox::Critical:
      tmpIcon = style->standardIcon( QStyle::SP_MessageBoxCritical );
      break;
    case QMessageBox::Question:
      tmpIcon = style->standardIcon( QStyle::SP_MessageBoxQuestion );
      break;
  }

  if( tmpIcon.isNull() )
    ui->label_2->setGeometry( ui->label_2->x(), ui->label_2->y(), 0, 0 );
  else {
    int iconSize = style->pixelMetric( QStyle::PM_MessageBoxIconSize, nullptr, mb );
    ui->label_2->setGeometry( ui->label_2->x(), ui->label_2->y(), iconSize, iconSize );
    ui->label_2->setPixmap( tmpIcon.pixmap( iconSize, iconSize ) );
  }

  delete mb;
}


void CMessageDialog::setCaption( const QString& caption ) {
  ui->label->setText( caption );
}


void CMessageDialog::setLineWrapMode( QPlainTextEdit::LineWrapMode mode ) {
  ui->plainTextEdit->setLineWrapMode( mode );
}


void CMessageDialog::clear() {
  ui->plainTextEdit->clear();
}


void CMessageDialog::append( const QString& str ) {
  ui->plainTextEdit->appendPlainText( str );
}


void CMessageDialog::setText( const QString& text ) {
  ui->plainTextEdit->setPlainText( text );
  ui->plainTextEdit->moveCursor( QTextCursor::Start );
}


void CMessageDialog::setText( const QStringList& list ) {
  ui->plainTextEdit->clear();
  for( int i = 0; i < list.count(); ++i ) {
    ui->plainTextEdit->appendPlainText( list.at(i) );
  }

  ui->plainTextEdit->moveCursor( QTextCursor::Start );
}


void CMessageDialog::setTextFromFile( const QString& filename ) {
  this->clear();

  QFile file( filename );
  if( !file.open(QIODevice::ReadOnly | QIODevice::Text ) ) {
    _error = true;
    _errMsg = QStringLiteral("Could not read from file 'filename'");
    return;
  }

  QString line;
  QTextStream in( &file );
  while( !in.atEnd() ) {
    line = in.readLine();
    this->append( line.trimmed() );
  }

  ui->plainTextEdit->moveCursor( QTextCursor::Start );
}


QString CMessageDialog::caption() {
  return ui->label->text();
}


QString CMessageDialog::text() {
  return ui->plainTextEdit->toPlainText();
}


void CMessageDialog::copyToClipboard() {
  QApplication::clipboard()->setText( ui->plainTextEdit->toPlainText() );
  QMessageBox::information( this, QStringLiteral("Message copied"), QStringLiteral("Contents of this message have been copied to the clipboard.") );
}


void CMessageDialog::save() {
  QString s = QFileDialog::getSaveFileName(
    this,
    QStringLiteral("Save message as file"),
    QDir::currentPath(),
    QStringLiteral("Plain-text files (*.txt);; All files (*.*)")
  );

  if( !s.isEmpty() ) {
    QFile file( s );
    if( !file.open( QFile::WriteOnly | QFile::Truncate ) )
      QMessageBox::warning( this, QStringLiteral("Cannot write file"), QStringLiteral( "Cannot open error log for writing: %1" ).arg( s ) );
    else {
      QTextStream out( &file );
      out << ui->plainTextEdit->toPlainText();
      out << flush;
      file.close();
    }
  }
}


void CMessageDialog::setDimensions( const int newWidth, const int newHeight ) {
  if( nullptr != this->parentWidget() ) {
    QWidget* parent = this->parentWidget();

    QPoint parentPosGlobal = parent->mapToGlobal(parent->pos());
    QPoint newPos = parent->mapFromGlobal( QPoint( parentPosGlobal.x() + parent->width()/2 - newWidth/2, parentPosGlobal.y() + parent->height()/2 - newHeight/2 ) );

    this->setGeometry(
      newPos.x(),
      newPos.y(),
      newWidth,
      newHeight
   );

  }
  else {
    this->resize( newWidth, newHeight );
  }
}



