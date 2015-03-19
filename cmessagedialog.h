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

  protected:
    void initializeUi( QMessageBox::Icon iconStyle );

  private:
    Ui::CMessageDialog *ui;
};

#endif // CMESSAGEDIALOG_H