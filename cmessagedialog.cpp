#include "cmessagedialog.h"
#include "ui_cmessagedialog.h"

#include <QDebug>
#include <QMessageBox>

CMessageDialog::CMessageDialog(QWidget *parent) : QDialog(parent), ui(new Ui::CMessageDialog) {
  initializeUi( QMessageBox::NoIcon );
}


CMessageDialog::CMessageDialog( const QString& title, const QString& text, QMessageBox::Icon iconStyle, QWidget *parent ) : QDialog(parent), ui(new Ui::CMessageDialog) {
  initializeUi( iconStyle );

  setWindowTitle( title );
  setCaption( text );
  clear();
}


CMessageDialog::~CMessageDialog() {
  delete ui;
}


void CMessageDialog::initializeUi( QMessageBox::Icon iconStyle ) {
  ui->setupUi(this);
  ui->label_2->setFrameShape( QFrame::NoFrame );

  QMessageBox* mb = new QMessageBox();

  QStyle *style = mb ? mb->style() : QApplication::style();

  QIcon tmpIcon;

  switch( iconStyle ) {
    case QMessageBox::NoIcon:
      // Do nothing
      break;
    case QMessageBox::Information:
      tmpIcon = style->standardIcon( QStyle::SP_MessageBoxInformation );
      break;
    case QMessageBox::Warning:
      tmpIcon = style->standardIcon( QStyle::SP_MessageBoxWarning );
      break;
    case QMessageBox::Critical:
      tmpIcon = style->standardIcon( QStyle::SP_MessageBoxCritical );
      break;
    case QMessageBox::Question:
      tmpIcon = style->standardIcon( QStyle::SP_MessageBoxQuestion );
      break;
  }

  if( tmpIcon.isNull() )
    ui->label_2->setGeometry( ui->label_2->x(), ui->label_2->y(), 0, 0 );
  else {
    int iconSize = style->pixelMetric( QStyle::PM_MessageBoxIconSize, 0, mb );
    ui->label_2->setGeometry( ui->label_2->x(), ui->label_2->y(), iconSize, iconSize );
    ui->label_2->setPixmap( tmpIcon.pixmap( iconSize, iconSize ) );
  }

  delete mb;
}


void CMessageDialog::setCaption( const QString& caption ) {
  ui->label->setText( caption );
}

void CMessageDialog::clear() {
  ui->plainTextEdit->clear();
}

void CMessageDialog::append( const QString& str ) {
  ui->plainTextEdit->appendPlainText( str );
}

void CMessageDialog::setText( const QString& text ) {
  ui->plainTextEdit->setPlainText( text );
}

QString CMessageDialog::caption() {
  return ui->label->text();
}

QString CMessageDialog::text() {
  return ui->plainTextEdit->toPlainText();
}
