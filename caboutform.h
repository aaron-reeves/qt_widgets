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

#ifndef CABOUTFORM_H
#define CABOUTFORM_H

#include <QDialog>

namespace Ui {
  class CAboutForm;
}

/*
  A basic form for showing an "About" window/message in a grapical application.
*/
class CAboutForm : public QDialog {
  Q_OBJECT

  public:
    explicit CAboutForm( QWidget *parent = nullptr );
    ~CAboutForm();

    void setPixmap( const QString& filename );
    void setEmailAddress( const QString& address );
    void setBriefDescr( const QString& descr );
    void setCopyright( const QString& copyright );
    void setAuthor( const QString& author );
    void setWebsite( QString website );

    // Author field is visible by default
    void hideAuthor();
    void showAuthor();

    // Email field is visible by default
    void hideEmail();
    void showEmail();

    // Website field is hidden by default
    void hideWebsite();
    void showWebsite();

  protected slots:
    void sendEmail();
    void visitWebsite();
    void showLicense();

  protected:
    QString _emailAddress;
    QString _website;

  private:
    Ui::CAboutForm *ui;
};

#endif // CABOUTFORM_H
