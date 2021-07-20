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

#ifndef CMULTIPROGRESSDIALOG_H
#define CMULTIPROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
  class CMultiProgressDialog;
  }

class CMultiProgressDialog : public QDialog {
  Q_OBJECT

  public:
    explicit CMultiProgressDialog(QWidget *parent = nullptr);
    ~CMultiProgressDialog();

    // Create a progress bar for each step name.  Progress of each step can be tracked independently
    void setup( const QStringList& stepNames );
    void setup( const QStringList& stepNames, const QStringList& captions );

    int nBars() const;

    bool* cancelClickedPtr;

  public slots:
    // Call this before calling any other functions for the widget to operate correctly.
    // Sets all individual progress bars to their starting positions.
    void start();

    // The master caption at the top of the widget
    void setCaption( QString caption );

    // Captions associated with individual progress bars, which are referred to either by index or by name.
    void setCaption( int idx, QString caption );
    void setCaption( QString stepName, QString caption );

    // Start an individual progress bar, informing it of how many total steps it will ultimately have.
    void start( int idx, int nSteps, QString caption );
    void start( QString key, int nSteps, QString caption );

    // Tell an individual progress bar how many total steps it has.
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

    // Tell the widget that everything is complete. Result will indicate success, termination, or error (see ReturnCodes).
    void setAllProgressComplete( int result );

    void clearMessages();
    void appendMessage( QString msg );

  signals:
    void cancelButtonClicked();
    void okButtonClicked();

  protected:
    void closeEvent( QCloseEvent* e ) override;
    bool _workInProgress;

  private:
    Ui::CMultiProgressDialog *ui;
};

#endif // CMULTIPROGRESSDIALOG_H
