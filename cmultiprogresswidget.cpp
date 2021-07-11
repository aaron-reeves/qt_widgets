/*
cmultiprogresswidget.ui/h/cpp
-----------------------------
Begin: 2019-06-18
Author: Aaron Reeves <aaron@aaronreeves.com>
---------------------------------------------------
Copyright (C) 2019- 2021 Aaron Reeves

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation, version 3.
*/

#include "cmultiprogresswidget.h"
#include "ui_cmultiprogresswidget.h"

#include <ar_general_purpose/arcommon.h>

CMultiProgressWidget::CMultiProgressWidget( QWidget *parent ) : QWidget(parent), ui(new Ui::CMultiProgressWidget) {
  ui->setupUi(this);
  ui->btnCancel->setEnabled( false );
  connect( ui->btnCancel, SIGNAL( clicked(bool) ), this, SLOT( setProcessingCanceled(bool) ) );
}


CMultiProgressWidget::CMultiProgressWidget( const QStringList& stepNames, QWidget *parent ) : QWidget(parent), ui(new Ui::CMultiProgressWidget) {
  ui->setupUi(this);
  connect( ui->btnCancel, SIGNAL( clicked(bool) ), this, SLOT( setProcessingCanceled(bool) ) );
  setup( stepNames );
}


CMultiProgressWidget::~CMultiProgressWidget() {
  delete ui;
}


void CMultiProgressWidget::setProcessingCanceled( bool dummy ) {
  Q_UNUSED( dummy );
  cancelClicked = true;
  ui->btnCancel->setEnabled( false );
}


void CMultiProgressWidget::setup( const QStringList& stepNames ) {
  while( !_progressWidgetsList.isEmpty() ) {
    delete _progressWidgetsList.takeFirst();
  }

  if( nullptr != ui->frame->layout() ) {
    delete ui->frame->layout();
  }

  QVBoxLayout* layout = new QVBoxLayout();

  for( int i = 0; i < stepNames.count(); ++i ) {
    CProgressWidget* wid = new CProgressWidget();
    _progressWidgetsList.append( wid );
    _progressWidgetsHash.insert( stepNames.at(i), wid );
    layout->addWidget( wid );
  }
  ui->frame->setLayout( layout );

  this->start();
}


void CMultiProgressWidget::setCaption( QString caption ) {
  ui->lblCaption->setText( caption );
}


void CMultiProgressWidget::start() {
  for( int i = 0; i < _progressWidgetsList.count(); ++i ) {
    _progressWidgetsList.at(i)->restart();
  }

  cancelClicked = false;

  this->clearMessages();

  ui->btnCancel->setEnabled( true );
}


void CMultiProgressWidget::start( int idx, int nSteps, QString caption ) {
  Q_ASSERT( idx < _progressWidgetsList.count() );
  _progressWidgetsList.at(idx)->start( nSteps, caption );
}
void CMultiProgressWidget::start( QString key, int nSteps, QString caption ) {
  Q_ASSERT( _progressWidgetsHash.contains( key ) );
  _progressWidgetsHash.value(key)->start( nSteps, caption );
}


void CMultiProgressWidget::setCaption(int idx, QString caption ) {
  Q_ASSERT( idx < _progressWidgetsList.count() );
  _progressWidgetsList.at(idx)->setCaption( caption );
}
void CMultiProgressWidget::setCaption( QString key, QString caption ) {
  Q_ASSERT( _progressWidgetsHash.contains( key ) );
  _progressWidgetsHash.value(key)->setCaption( caption );
}


void CMultiProgressWidget::setNSteps( int idx, int nSteps ) {
  Q_ASSERT( idx < _progressWidgetsList.count() );
  _progressWidgetsList.at(idx)->setMaximum( nSteps );
}
void CMultiProgressWidget::setNSteps( QString key, int nSteps ) {
  Q_ASSERT( _progressWidgetsHash.contains( key ) );
  _progressWidgetsHash.value(key)->setMaximum( nSteps );
}


void CMultiProgressWidget::setStepComplete( int idx, int step ) {
  Q_ASSERT( idx < _progressWidgetsList.count() );
  _progressWidgetsList.at(idx)->setValue( step );
}
void CMultiProgressWidget::setStepComplete( QString key, int step ) {
  Q_ASSERT( _progressWidgetsHash.contains( key ) );
  _progressWidgetsHash.value(key)->setValue( step );
}


void CMultiProgressWidget::progressComplete( int idx ) {
  Q_ASSERT( idx < _progressWidgetsList.count() );
  _progressWidgetsList.at(idx)->setComplete();
}
void CMultiProgressWidget::progressComplete( QString key ) {
  Q_ASSERT( _progressWidgetsHash.contains( key ) );
  _progressWidgetsHash.value(key)->setComplete();
}


void CMultiProgressWidget::progressComplete() {
  ui->lblCaption->setText( QStringLiteral( "Processing complete." ) );

  for( int i = 0; i < _progressWidgetsList.count(); ++i ) {
    _progressWidgetsList.at(i)->setComplete();
  }

  ui->btnCancel->setEnabled( false );
}


void CMultiProgressWidget::setTerminated() {
  ui->lblCaption->setText( QStringLiteral( "Processing terminated." ) );

  for( int i = 0; i < _progressWidgetsList.count(); ++i ) {
    _progressWidgetsList.at(i)->setTerminated();
  }

  ui->btnCancel->setEnabled( false );
}


void CMultiProgressWidget::clearMessages() {
  ui->pteMessages->clear();
}


void CMultiProgressWidget::appendMessage( const QString msg ) {
  ui->pteMessages->appendPlainText( msg );
}


//void CMultiProgressWidget::setRunner( CRunner* runner ) {
//  _runner = runner;
//  connectSlots();
//}


//void CMultiProgressWidget::connectSlots() {
////  if( nullptr != _runner ) {
////    connect( _runner, SIGNAL( processingStarted() ), this, SLOT( processingStarted() ) );
////    connect( _runner, SIGNAL( setNFiles( int )    ), this, SLOT( setNFiles( int )    ) );
////    connect( _runner, SIGNAL( setNSheets( int )   ), this, SLOT( setNSheets( int )   ) );

////    connect( _runner, SIGNAL( fileProcessingStarted() ), this, SLOT( fileProcessingStarted() ) );

////    connect( _runner, SIGNAL( operationStart( QString, int ) ), this, SLOT( operationStart( QString, int ) ) );
////    connect( _runner, SIGNAL( operationProgress( int )       ), this, SLOT( operationProgress( int )       ) );
////    connect( _runner, SIGNAL( operationComplete()            ), this, SLOT( operationComplete()            ) );
////    connect( _runner, SIGNAL( lastOperationComplete()        ), this, SLOT( lastOperationComplete()        ) );

////    connect( _runner, SIGNAL( fileProcessingComplete() ), this, SLOT( fileProcessingComplete() ) );

////    connect( _runner, SIGNAL( processingComplete()   ), this, SLOT( processingComplete()   ) );
////    connect( _runner, SIGNAL( processingTerminated() ), this, SLOT( processingTerminated() ) );
////  }

//  connect( &appLog, SIGNAL( messageLogged( QString ) ), this, SLOT( appendMessage( QString ) ) );
//}


//void CMultiProgressWidget::disconnectSlots() {
////  if( nullptr != _runner ) {
////    disconnect( _runner, SIGNAL( processingStarted() ), this, SLOT( processingStarted() ) );
////    disconnect( _runner, SIGNAL( setNFiles( int )    ), this, SLOT( setNFiles( int )    ) );
////    disconnect( _runner, SIGNAL( setNSheets( int )   ), this, SLOT( setNSheets( int )   ) );

////    disconnect( _runner, SIGNAL( fileProcessingStarted() ), this, SLOT( fileProcessingStarted() ) );

////    disconnect( _runner, SIGNAL( operationStart( QString, int ) ), this, SLOT( operationStart( QString, int ) ) );
////    disconnect( _runner, SIGNAL( operationProgress( int )       ), this, SLOT( operationProgress( int )       ) );
////    disconnect( _runner, SIGNAL( operationComplete()            ), this, SLOT( operationComplete()            ) );
////    disconnect( _runner, SIGNAL( lastOperationComplete()        ), this, SLOT( lastOperationComplete()        ) );

////    disconnect( _runner, SIGNAL( fileProcessingComplete() ), this, SLOT( fileProcessingComplete() ) );

////    disconnect( _runner, SIGNAL( processingComplete()   ), this, SLOT( processingComplete()    ) );
////    disconnect( _runner, SIGNAL( processingTerminated() ), this, SLOT( processingTerminated()  ) );
////  }

//  disconnect( &appLog, SIGNAL( messageLogged( QString ) ), this, SLOT( appendMessage( QString ) ) );
//}

