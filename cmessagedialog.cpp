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

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QClipboard>

CMessageDialog::CMessageDialog(QWidget *parent) : QDialog(parent), ui(new Ui::CMessageDialog) {
  initializeUi( QMessageBox::NoIcon );
}


CMessageDialog::CMessageDialog( const QString& title, const QString& text, QMessageBox::Icon iconStyle, QWidget *parent ) : QDialog(parent), ui(new Ui::CMessageDialog) {
  initializeUi( iconStyle );

  setWindowTitle( title );
  setCaption( text );
  clear();
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
    int iconSize = style->pixelMetric( QStyle::PM_MessageBoxIconSize, 0, mb );
    ui->label_2->setGeometry( ui->label_2->x(), ui->label_2->y(), iconSize, iconSize );
    ui->label_2->setPixmap( tmpIcon.pixmap( iconSize, iconSize ) );
  }

  delete mb;
}


void CMessageDialog::setCaption( const QString& caption ) {
  ui->label->setText( caption );
}


void CMessageDialog::clear() {
  ui->plainTextEdit->clear();
}


void CMessageDialog::append( const QString& str ) {
  ui->plainTextEdit->appendPlainText( str );
}


void CMessageDialog::setText( const QString& text ) {
  ui->plainTextEdit->setPlainText( text );
}


QString CMessageDialog::caption() {
  return ui->label->text();
}


QString CMessageDialog::text() {
  return ui->plainTextEdit->toPlainText();
}


void CMessageDialog::copyToClipboard() {
  QApplication::clipboard()->setText( ui->plainTextEdit->toPlainText() );
  QMessageBox::information( this, "Message copied", "Contents of this message have been copied to the clipboard." );
}


void CMessageDialog::save() {
  QString s = QFileDialog::getSaveFileName(
    this,
    "Save message as file",
    QDir::currentPath(),
    "Plain-text files (*.txt);; All files (*.*)"
  );

  if( !s.isEmpty() ) {
    QFile file( s );
    if( !file.open( QFile::WriteOnly | QFile::Truncate ) )
      QMessageBox::warning( this, "Cannot write file", QString( "Cannot open error log for writing: %1" ).arg( s ) );
    else {
      QTextStream out( &file );
      out << ui->plainTextEdit->toPlainText();
      out << flush;
      file.close();
    }
  }
}
