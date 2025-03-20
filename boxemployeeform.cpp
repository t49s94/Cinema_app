/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This Window is the main menu for the Box office employees. Allows them to create new transactions for customers who wants
to watch movie(s).

*/

#include "boxemployeeform.h"
#include "ui_boxemployeeform.h"

BoxEmployeeForm::BoxEmployeeForm(QWidget *parent, Employee employee) :
    QMainWindow(parent),
    ui(new Ui::BoxEmployeeForm)
{
    ui->setupUi(this);
    // Prevent the user to resize the window
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    this->employee = employee;

    ui->lblTitle->setText("WELCOME " + employee.getFullName());
    ui->lblTimeLoggedIn->setText(QTime::currentTime().toString("hh:mm:ss AP"));

    QImage image(":/Images/make movie logo transparent.png");
    QPixmap pixmap(QPixmap::fromImage(image));
    // Resize the image to the label dimensions.
    ui->lblLogo->setPixmap(pixmap.scaled(ui->lblLogo->width(), ui->lblLogo->height(),Qt::KeepAspectRatio));
}

BoxEmployeeForm::~BoxEmployeeForm()
{
    delete ui;
}

// Create an instance of BoxOfficeTransactionAddForm for a new transaction.
void BoxEmployeeForm::on_pbtNewTransaction_clicked()
{
    BoxOfficeTransactionAddForm *f = new BoxOfficeTransactionAddForm(this, employee);
    f->show();
    this->hide();
}

// Logs out employee.
void BoxEmployeeForm::on_pbtLogOut_clicked()
{
    // Ask if the user really wants to log out.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Log out", "Do you want to log out?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    this->parentWidget()->show();
    this->close();
}
