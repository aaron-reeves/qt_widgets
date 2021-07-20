/*
cmultiprogressdialog.ui/h/cpp
-----------------------------
Begin: 2021-07-19
Author: Aaron Reeves <aaron@aaronreeves.com>
---------------------------------------------------
Copyright (C) 2021 Aaron Reeves

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation, version 3.
*/

#include "cmultiprogressdialog.h"
#include "ui_cmultiprogressdialog.h"

CMultiProgressDialog::CMultiProgressDialog( QWidget *parent ) : QDialog (parent ), ui( new Ui::CMultiProgressDialog ) {
  ui->setupUi( this );

  cancelClickedPtr = &ui->progress->cancelClicked;

  connect( ui->progress, SIGNAL( cancelButtonClicked() ), this, SIGNAL( cancelButtonClicked() ) );
  connect( ui->progress, SIGNAL( okButtonClicked() ), this, SIGNAL( okButtonClicked() ) );

  connect( ui->progress, SIGNAL( okButtonClicked() ), this, SLOT( close() ) );
}


CMultiProgressDialog::~CMultiProgressDialog() {
  delete ui;
}


// Create a progress bar for each step name.  Progress of each step can be tracked independently
void CMultiProgressDialog::setup( const QStringList& stepNames ) {
  ui->progress->setup( stepNames );
}


void CMultiProgressDialog::setup( const QStringList& stepNames, const QStringList& captions ) {
  ui->progress->setup( stepNames, captions );
}


int CMultiProgressDialog::nBars() const {
  return ui->progress->nBars();
}


void CMultiProgressDialog::start() {
  ui->progress->start();
}


void CMultiProgressDialog::setCaption( QString caption ) {
  ui->progress->setCaption( caption );
}


void CMultiProgressDialog::setCaption( int idx, QString caption ) {
  ui->progress->setCaption( idx, caption );
}


void CMultiProgressDialog::setCaption( QString stepName, QString caption ) {
  ui->progress->setCaption( stepName, caption);
}


void CMultiProgressDialog::start( int idx, int nSteps, QString caption ) {
  ui->progress->start( idx, nSteps, caption );
}


void CMultiProgressDialog::start( QString key, int nSteps, QString caption ) {
  ui->progress->start( key, nSteps, caption );
}


void CMultiProgressDialog::setNStepsToBeCompleted( int idx, int nSteps ) {
  ui->progress->setNStepsToBeCompleted( idx, nSteps );
}


void CMultiProgressDialog::setNStepsToBeCompleted( QString stepName, int nSteps ) {
  ui->progress->setNStepsToBeCompleted( stepName, nSteps );
}


void CMultiProgressDialog::setStepComplete( int idx, int step ) {
  ui->progress->setStepComplete( idx, step );
}

void CMultiProgressDialog::setStepComplete( QString stepName, int step ) {
  ui->progress->setStepComplete( stepName, step );
}


void CMultiProgressDialog::incrementSteps( int idx ) {
  ui->progress->incrementSteps( idx );
}


void CMultiProgressDialog::incrementSteps( QString stepName ) {
  ui->progress->incrementSteps( stepName );
}


void CMultiProgressDialog::setProgressComplete( int idx, int result )  {
  ui->progress->setProgressComplete( idx, result );
}


void CMultiProgressDialog::setProgressComplete( QString stepName, int result )  {
  ui->progress->setProgressComplete( stepName, result );
}


void CMultiProgressDialog::setAllProgressComplete( int result ) {
  ui->progress->setAllProgressComplete( result );
}


void CMultiProgressDialog::clearMessages() {
  ui->progress->clearMessages();
}


void CMultiProgressDialog::appendMessage( QString msg ) {
  ui->progress->appendMessage( msg );
}
