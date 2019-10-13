/*
caboutform.h/cpp
----------------
Begin: 2019-05-06
Author: Aaron Reeves <aaron.reeves@sruc.ac.uk>
---------------------------------------------------
Copyright (C) 2019 Epidemiology Research Unit, Scotland's Rural College (SRUC)

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/

#include "caboutform.h"
#include "ui_caboutform.h"

#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QtCore>
#include <QMessageBox>

#include "cmessagedialog.h"

CAboutForm::CAboutForm(QWidget *parent) :  QDialog(parent), ui(new Ui::CAboutForm) {
  ui->setupUi( this );

  QString appVersion = QString( APP_VERSION );
  QStringList bits = appVersion.split( '.' );

  QDate buildDate = QDate::fromString( bits.last(), "yyyyMMdd" );

  bits.removeLast();
  QString versString = QString( "%1 %2" ).arg( APP_NAME ).arg( bits.join( '.' ) );

  this->setWindowTitle( versString );
  ui->lblAppName->setText( versString );
  ui->lblBuildDate->setText( QString( "Build date: %1" ).arg( buildDate.toString( "MMM dd, yyyy") ) );

  ui->lblPixmap->setStyleSheet( QString() );

  connect( ui->btnClose, SIGNAL( clicked() ), this, SLOT( close() ) );
  connect( ui->btnLicense, SIGNAL( clicked() ), this, SLOT( showLicense() ) );

  connect( ui->lblEmail, SIGNAL( clicked() ), this, SLOT( sendEmail() ) );
}


CAboutForm::~CAboutForm() {
  delete ui;
}


void CAboutForm::setPixmap( const QString& filename ) {
  ui->lblPixmap->setPixmap( QPixmap( filename ) );
}


void CAboutForm::setEmailAddress( const QString& address ) {
  _emailAddress = address;
  ui->lblEmail->setText( _emailAddress );
}


void CAboutForm::setBriefDescr( const QString& descr ) {
  ui->lblBriefDescr->setText( descr );
}


void CAboutForm::setCopyright( const QString& copyright ) {
  ui->lblCopyright->setText( copyright );
}


void CAboutForm::setAuthor( const QString& author ) {
  ui->lblAuthor->setText( QString( "Author: %1" ).arg( author ) );
}


void CAboutForm::sendEmail() {
  if( !QDesktopServices::openUrl( QUrl( QString( "mailto:%1" ).arg( _emailAddress ) ) ) ) {
    QMessageBox mbx( this );
    mbx.setText( "Email could not be sent. Please ensure that your email application is installed and properly configured to handle links." );
    mbx.setIcon( QMessageBox::Warning );
    mbx.setWindowIcon( this->windowIcon() );
    mbx.exec();
  }
}


void CAboutForm::showLicense() {
  CMessageDialog dlg( "License", "This application is released under the terms of the following license:", QMessageBox::NoIcon, this );
  dlg.setDimensions( 550, 400 );
  dlg.setTextFromFile( ":/license/LICENSE.txt" );
  dlg.exec();
}


