/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This is the main window for the general manager and manager. Allows access to all tables in DB for looking up, add, delete and update records.

*/

#include "generalmanagerform.h"
#include "ui_generalmanagerform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

GeneralManagerForm::GeneralManagerForm(QWidget *parent, Employee employee) :
    QMainWindow(parent),
    ui(new Ui::GeneralManagerForm)
{
    ui->setupUi(this);
    // Prevents user from resizing window
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setCentralWidget(ui->mdiArea);
    this->employee = employee;
}

GeneralManagerForm::~GeneralManagerForm()
{
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Load a subwindow for this window.
// @param QWidget *widget. Window that will be shown.
void GeneralManagerForm::loadSubWindow(QWidget *widget)
{
    auto window  = ui->mdiArea->addSubWindow(widget);
    window->setWindowTitle(widget->windowTitle());
    window->setWindowIcon(widget->windowIcon());
    window->setFixedSize(widget->width(),widget->height());
    window->show();
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void GeneralManagerForm::on_actionCast_members_triggered()
{
    loadSubWindow(new CastMembersForm(this));
}

void GeneralManagerForm::on_actionCast_positions_triggered()
{
    loadSubWindow(new CastPositionsForm(this));
}

void GeneralManagerForm::on_actionMovie_genres_triggered()
{
    loadSubWindow(new MovieGenresForm(this));
}

void GeneralManagerForm::on_actionMovies_triggered()
{
    loadSubWindow(new MoviesForm(this));
}

void GeneralManagerForm::on_actionCast_triggered()
{
    loadSubWindow(new MovieCastForm(this));
}

void GeneralManagerForm::on_actionCustomer_types_triggered()
{
    loadSubWindow(new CustomerTypeForm(this));
}

void GeneralManagerForm::on_actionCustomers_triggered()
{
    loadSubWindow(new CustomersForm(this));
}

void GeneralManagerForm::on_actionEmployee_positions_triggered()
{
    loadSubWindow(new EmployeeTypesForm(this));
}

void GeneralManagerForm::on_actionEmployees_triggered()
{
    loadSubWindow(new EmployeesForm(this));
}

void GeneralManagerForm::on_actionTheater_types_triggered()
{
    loadSubWindow(new TheaterTypesForm(this));
}

void GeneralManagerForm::on_actionPayment_types_triggered()
{
    loadSubWindow(new PaymentTypesForm(this));
}

void GeneralManagerForm::on_actionPromotion_types_triggered()
{
    loadSubWindow(new PromotionTypeForm(this));
}

void GeneralManagerForm::on_actionCoupons_triggered()
{
    loadSubWindow(new CouponsForm(this));
}

void GeneralManagerForm::on_actionPromotions_triggered()
{
    loadSubWindow(new PromotionsForm(this));
}

void GeneralManagerForm::on_actionTheaters_triggered()
{
    loadSubWindow(new TheatersForm(this));
}

void GeneralManagerForm::on_actionShowtimes_triggered()
{
    loadSubWindow(new ShowtimesForm(this));
}

void GeneralManagerForm::on_actionAll_transactions_triggered()
{
    loadSubWindow(new BoxOfficeTransactionsForm(this));
}

void GeneralManagerForm::on_actionNew_transaction_triggered()
{
    loadSubWindow(new BoxOfficeTransactionAddForm(this, employee));

}

void GeneralManagerForm::on_actionLog_out_triggered()
{
    // Asks if user wants to log out. Return if not.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Log out", "Do you want to log out?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    this->parentWidget()->show();
    this->close();
}
