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

#ifndef CMULTIPROGRESSWIDGET_H
#define CMULTIPROGRESSWIDGET_H

#include <QWidget>

#include "cprogresswidget.h"

namespace Ui {
  class CMultiProgressWidget;
}

class CMultiProgressWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CMultiProgressWidget( QWidget *parent = nullptr );
    explicit CMultiProgressWidget( const QStringList& stepNames, QWidget *parent = nullptr );
    ~CMultiProgressWidget();

    void setup( const QStringList& stepNames );

    int nBars() const { return _progressWidgetsList.count(); }

    bool cancelClicked;

  public slots:
    void setCaption( QString caption );

    void setCaption( int idx, QString caption );
    void setCaption( QString key, QString caption );

    void start();

    void start( int idx, int nSteps, QString caption );
    void start( QString key, int nSteps, QString caption );

    void setNSteps( int idx, int nSteps );
    void setNSteps( QString key, int nSteps );

    void setStepComplete( int idx, int step );
    void setStepComplete( QString key, int step );

    void progressComplete( int idx );
    void progressComplete( QString key );

    void progressComplete();

    void setTerminated();

    void clearMessages();
    void appendMessage( QString msg );

  protected slots:
    void setProcessingCanceled( bool dummy );

  protected:
    void connectSlots();
    void disconnectSlots();

    QString _operation;

  private:
    Ui::CMultiProgressWidget *ui;

    QList<CProgressWidget*> _progressWidgetsList;
    QHash<QString, CProgressWidget*> _progressWidgetsHash;
};

#endif // CMULTIPROGRESSWIDGET_H
