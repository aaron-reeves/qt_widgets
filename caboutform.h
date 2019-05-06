#ifndef CABOUTFORM_H
#define CABOUTFORM_H

#include <QDialog>

namespace Ui {
  class CAboutForm;
}

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

  protected slots:
    void sendEmail();
    void showLicense();

  protected:
    QString _emailAddress;

  private:
    Ui::CAboutForm *ui;
};

#endif // CABOUTFORM_H
