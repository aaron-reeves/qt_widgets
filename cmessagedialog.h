/*
cmessagedialog.h/cpp
--------------------
Begin: 2008/02/18
Author: Aaron Reeves <aaron.reeves@naadsm.org>
--------------------------------------------------
Copyright (C) 2008 - 2019 Aaron Reeves

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/

#ifndef CMESSAGEDIALOG_H
#define CMESSAGEDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QtWidgets>

namespace Ui {
class CMessageDialog;
}

/*
 * Typical usage:
 * CMessageDialog dlg( this );
 * dlg.setWindowTitle( QStringLiteral("My window title") );
 * dlg.setCaption( QStringLiteral("My message caption:") );
 * dlg.setText( QStringLiteral("My lengthy message goes in here.") );
 * dlg.exec();
*/

class CMessageDialog : public QDialog {
  Q_OBJECT

  public:
    explicit CMessageDialog( QWidget *parent = nullptr );
    explicit CMessageDialog( const QString& title, const QString& caption, QMessageBox::Icon iconStyle = QMessageBox::NoIcon, QWidget *parent = nullptr );
    ~CMessageDialog();

    void setCaption( const QString& text );
    void setLineWrapMode( QPlainTextEdit::LineWrapMode mode );

    void clear();
    void setText( const QString& body );
    void setText( const QStringList& list );
    void setTextFromFile( const QString& filename );
    void setDimensions( const int newWidth, const int newHeight );

    QString caption();
    QString text();

    bool error() const { return _error; }
    QString errorMessage() const { return _errMsg; }

  public slots:
    void append( const QString& str );

  protected slots:
    void copyToClipboard();
    void save();

  protected:
    void initializeUi( QMessageBox::Icon iconStyle );

  private:
    Ui::CMessageDialog *ui;

    bool _error;
    QString _errMsg;
};

#endif // CMESSAGEDIALOG_H
