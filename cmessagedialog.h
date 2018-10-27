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

#ifndef CMESSAGEDIALOG_H
#define CMESSAGEDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class CMessageDialog;
}

class CMessageDialog : public QDialog {
  Q_OBJECT

  public:
    explicit CMessageDialog(QWidget *parent = 0);
    explicit CMessageDialog( const QString& title, const QString& text, QMessageBox::Icon iconStyle = QMessageBox::NoIcon, QWidget *parent = 0 );
    ~CMessageDialog();

    void setCaption( const QString& text );

    void clear();
    void append( const QString& str );
    void setText( const QString& body );

    QString caption();
    QString text();

  protected slots:
    void copyToClipboard();
    void save();

  protected:
    void initializeUi( QMessageBox::Icon iconStyle );

  private:
    Ui::CMessageDialog *ui;
};

#endif // CMESSAGEDIALOG_H
