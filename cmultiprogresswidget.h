/*
cmultiprogresswidget.ui/h/cpp
-----------------------------
Begin: 2019-06-18
Author: Aaron Reeves <aaron@aaronreeves.com>
---------------------------------------------------
Copyright (C) 2019 - 2021 Aaron Reeves

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation, version 3.
*/

#ifndef CMULTIPROGRESSWIDGET_H
#define CMULTIPROGRESSWIDGET_H

#include <QWidget>
#include <QMutex>

#include "cprogresswidget.h"

namespace Ui {
  class CMultiProgressWidget;
}

class CMultiProgressWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CMultiProgressWidget( QWidget *parent = nullptr );

    // stepNames are useful shorthand for referring to individual progress bars on this widget.
    // They aren't displayed to the user, but can be used by the programmer to make life simpler.
    explicit CMultiProgressWidget( const QStringList& stepNames, QWidget *parent = nullptr );
    ~CMultiProgressWidget();

    // Create a progress bar for each step name.  Progress of each step can be tracked independently
    void setup( const QStringList& stepNames );
    void setup( const QStringList& stepNames, const QStringList& captions );

    int nBars() const { return _progressWidgetsList.count(); }

    // If the user clicks cancel, the value of this public variable is changed.
    // Threads or other objects can check it, and depending on its value, they might
    // shop processing, etc.
    bool cancelClicked;

    // By default, the OK button on the widget is not used or visible.
    // By contrast, it *is* used by default on CMultiprogressDialog.
    void setUseOKButton( const bool val ) { _useOKButton = val; }

  public slots:
    // Call this before calling any other functions for the widget to operate correctly.
    // Sets all individual progress bars to their starting positions.
    void start();

    // The master caption at the top of the widget
    void setCaption( QString caption );

    // Captions associated with individual progress bars, which are referred to either by index or by name.
    void setCaption( int idx, QString caption );
    void setCaption( QString stepName, QString caption );    

    // Start an individual progress bar at 0, while simultaneously informing it of how many total steps it will ultimately have
    // and setting the caption for the individual progress bar.
    void start( int idx, int nSteps, QString caption );
    void start( QString key, int nSteps, QString caption );

    // Tell an individual progress bar how many total steps it has, but don't start it at 0.
    // Useful in conjunction with start() (without parameters) above.
    void setNStepsToBeCompleted( int idx, int nSteps );
    void setNStepsToBeCompleted( QString stepName, int nSteps );

    // Tell an individual progress bar how far it's made it.
    void setStepComplete( int idx, int step );
    void setStepComplete( QString stepName, int step );

    // Tell an individual progress bar to increment the number of steps completed.
    void incrementSteps( int idx );
    void incrementSteps( QString stepName );

    // Tell an individual progress bar that it is finished.
    void setProgressComplete( int idx, int result );
    void setProgressComplete( QString stepName, int result );

    // Tell the widget that everything is complete. Result will indicate success, termination, or other error (see ReturnCodes)
    void setAllProgressComplete( int result );

    void clearMessages();
    void appendMessage( QString msg );
    void copyMessages();

  signals:
    void cancelButtonClicked();
    void okButtonClicked();

  protected:
    QString _operation;
    QString _mainCaption;

    QMutex _mutex;

    bool _useOKButton;

  private slots:
    void setProcessingCanceled();

  private:
    Ui::CMultiProgressWidget *ui;

    QList<CProgressWidget*> _progressWidgetsList;
    QHash<QString, CProgressWidget*> _progressWidgetsHash;
};

#endif // CMULTIPROGRESSWIDGET_H
