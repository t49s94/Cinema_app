/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows sing in to the applications

*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Prevents user from resizing window
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    QImage image(":/Images/make movie logo transparent.png");
    QPixmap pixmap(QPixmap::fromImage(image));
    ui->lblLogo->setPixmap(pixmap.scaled(ui->lblLogo->width(), ui->lblLogo->height(),Qt::KeepAspectRatio));

}

MainWindow::~MainWindow()
{
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Loads a window and hides this one.
// @param QWidget *window. Window that is gonna be shown
void MainWindow::loadWindow(QWidget *window)
{
    QMessageBox::information(this, "Welcome", "Access granted!");
    window->show();
    this->hide();
    ui->ldtUsername->setText("");
    ui->ldtPassword->setText("");
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void MainWindow::on_pbtLogin_clicked()
{
    Login conn;

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't connect to the server!\n Error: " + conn.nativeErrorCode());
        return;
    }

    username = ui->ldtUsername->text();
    password = ui->ldtPassword->text();
    // Encrypt password with Sha521 algorithm
    password = QString("%1").arg(QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha512).toHex()));

    QSqlQuery query;
    query.prepare("SELECT employee_id, first_name, middle_name, last_name, employee_position_id "
                  "FROM employee "
                  "WHERE username = :username AND password = :password;");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.exec();

    if(!query.next())
    {
        QMessageBox::warning(this, "Access denied", "Username and/or password incorrect!");
        return;
    }

    Employee employee;
    employee.setId(query.value(0).toInt());
    employee.setFirstName(query.value(1).toString());
    employee.setMiddleName(query.value(2).toString());
    employee.setLastName(query.value(3).toString());
    employee.setPosition(EmployeePosition(query.value(4).toInt(), ""));

    switch(query.value(4).toInt())
    {
    case 1:
        loadWindow(new GeneralManagerForm(this, employee));
        break;

    case 2:
        loadWindow(new GeneralManagerForm(this, employee));
        break;

    case 3:
        loadWindow(new BoxEmployeeForm(this, employee));
        break;

    default:
        QMessageBox::warning(this, "Access denied", "You have no authorization to use this program!");

    }

    conn.connClose();
}
