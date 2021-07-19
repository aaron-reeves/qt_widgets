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


#ifndef CPROGRESSWIDGET_H
#define CPROGRESSWIDGET_H

#include <QWidget>

namespace Ui {
  class CProgressWidget;
  }

class CProgressWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CProgressWidget(QWidget *parent = nullptr);
    ~CProgressWidget();

    static QString defaultStyle();
    static QString errorStyle();
    static QString terminatedStyle();

    void setCaption( const QString& str );

    void restart();
    void start( const int nSteps, const QString& caption = QString() );
    void setComplete( const int result );

    void setValue( const int val );
    int value() const;
    void setMaximum( const int val );
    int maximum() const;
    void setMinimum( const int val );
    int minimum() const;

  protected:
    void setCaption();

    QString _caption;
    QString _addlCaption;

  private:
    Ui::CProgressWidget *ui;

};

#endif // CPROGRESSWIDGET_H
