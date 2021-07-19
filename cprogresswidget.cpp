/*
cprogresswidget.h/cpp/ui
------------------------
Begin: 2021/07/05
Author: Aaron Reeves <aaron.reeves@sruc.ac.uk>
--------------------------------------------------
Copyright (C) 2021 Epidemiology Research Unit, Scotland's Rural College (SRUC)

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/


#include "cprogresswidget.h"
#include "ui_cprogresswidget.h"

#include <ar_general_purpose/returncodes.h>

CProgressWidget::CProgressWidget(QWidget *parent) : QWidget(parent), ui(new Ui::CProgressWidget) {
  ui->setupUi(this);
}


CProgressWidget::~CProgressWidget() {
  delete ui;
}


QString CProgressWidget::defaultStyle() {
  return QString(
    " QProgressBar:horizontal {         "
    " border-top: 1px solid gray;       "
    " border-left: 1px solid gray;      "
    " border-bottom: 1px solid white;   "
    " border-right: 1px solid white;    "
    " border-radius: 0px;               "
    " background: rgb( 212, 208, 200 ); "
    " padding: 2px;                     "
    " text-align: right;                "
    " margin-right: 12ex;               "
    " height: 15px;                     "
    " }                                 "
    " QProgressBar::chunk:horizontal {  "
    " background: rgb( 30, 86, 234 );   "
    " margin-right: 2px; /* space */    "
    " width: 8px;                       "
    " }                                 "
  );
}


QString CProgressWidget::errorStyle() {
  return QString(
    " QProgressBar:horizontal {         "
    " border-top: 1px solid gray;       "
    " border-left: 1px solid gray;      "
    " border-bottom: 1px solid white;   "
    " border-right: 1px solid white;    "
    " border-radius: 0px;               "
    " background: rgb( 212, 208, 200 ); "
    " padding: 2px;                     "
    " text-align: right;                "
    " margin-right: 12ex;               "
    " height: 15px;                     "
    " }                                 "
    " QProgressBar::chunk:horizontal {  "
    " background: rgb( 253, 179, 3 );   "
    " margin-right: 2px; /* space */    "
    " width: 8px;                       "
    " }                                 "
  );
}


QString CProgressWidget::terminatedStyle() {
  return QString(
    " QProgressBar:horizontal {         "
    " border-top: 1px solid gray;       "
    " border-left: 1px solid gray;      "
    " border-bottom: 1px solid white;   "
    " border-right: 1px solid white;    "
    " border-radius: 0px;               "
    " background: rgb( 212, 208, 200 ); "
    " padding: 2px;                     "
    " text-align: right;                "
    " margin-right: 12ex;               "
    " height: 15px;                     "
    " }                                 "
    " QProgressBar::chunk:horizontal {  "
    " background: rgb( 200, 0, 0 );     "
    " margin-right: 2px; /* space */    "
    " width: 8px;                       "
    " }                                 "
  );
}


void CProgressWidget::start( const int nSteps, const QString& caption /* = QString() */ ) {
  ui->progressBar->setMaximum( nSteps );
  ui->progressBar->setValue( 0 );

  if( !caption.isEmpty() ) {
    _caption = caption;
  }

  _addlCaption = QStringLiteral( "0 of %1" ).arg( nSteps );

  setCaption();
  QCoreApplication::processEvents();
}


void CProgressWidget::setValue( const int val ) {
  _addlCaption = QString( "%1 of %2" ).arg( val ).arg( ui->progressBar->maximum() );
  setCaption();
  ui->progressBar->setValue( val );
  QCoreApplication::processEvents();
}


int CProgressWidget::value() const {
  return ui->progressBar->value();
}


void CProgressWidget::setMaximum( const int val ) {
  ui->progressBar->setMaximum( val );
  QCoreApplication::processEvents();
}


int CProgressWidget::maximum() const {
  return ui->progressBar->maximum();
}


void CProgressWidget::setMinimum( const int val ) {
  ui->progressBar->setMinimum( val );
  QCoreApplication::processEvents();
}


int CProgressWidget::minimum() const {
  return ui->progressBar->minimum();
}


void CProgressWidget::setCaption( const QString& str ) {
  _caption = str;
  _addlCaption = "";
  setCaption();
}


void CProgressWidget::setCaption() {
  if( _caption.isEmpty() ) {
    ui->label->setText( _addlCaption );
  }
  else {
    ui->label->setText( QStringLiteral( "%1: %2" ).arg( _caption, _addlCaption ) );
  }
  QCoreApplication::processEvents();
}


void CProgressWidget::restart() {
  ui->progressBar->setStyleSheet( CProgressWidget::defaultStyle() );
  _addlCaption = QStringLiteral( "Starting..." );

  ui->progressBar->setMaximum( 1 );
  ui->progressBar->setValue( 0 );

  setCaption();
}


void CProgressWidget::setComplete( const int result ) {

  if( ReturnCode::SUCCESS == result ) {
    ui->progressBar->setStyleSheet( CProgressWidget::defaultStyle() );
    _addlCaption = QStringLiteral( "Complete" );
  }
  else if( result & ReturnCode::PROCESSING_INTERRUPTED ) {
    ui->progressBar->setStyleSheet( CProgressWidget::terminatedStyle() );
    _addlCaption = QStringLiteral( "Terminated" );
  }
  else {
    ui->progressBar->setStyleSheet( CProgressWidget::errorStyle() );
    _addlCaption = QStringLiteral( "Complete with errors" );
  }

  if( 0 == ui->progressBar->value() ) {
    ui->progressBar->setMaximum(1);
  }
  ui->progressBar->setValue( ui->progressBar->maximum() );


  setCaption();
}
