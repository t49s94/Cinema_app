/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window accepts data from employee and customer to create a new transaction.

*/

#ifndef BOXOFFICETRANSACTIONADDFORM_H
#define BOXOFFICETRANSACTIONADDFORM_H

#include <QMainWindow>

#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <QStandardItemModel>
#include <QPdfWriter>
#include <QPainter>

#include "login.h"
#include "employee.h"
#include "showtime.h"
#include "boxofficetransaction.h"
#include "membercard.h"
#include "seatselectionform.h"
#include "ticket.h"

namespace Ui {
class BoxOfficeTransactionAddForm;
}

class BoxOfficeTransactionAddForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit BoxOfficeTransactionAddForm(QWidget *parent, Employee employee);
    ~BoxOfficeTransactionAddForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Checks if required fields were filled out.
    // @return bool. True if all required fields were filled out
    bool areFieldsNotBlank();
    // Checks if fields have correct data
    // @return bool. True if fields have correct data
    bool validateFields();

    // Gets total and a feedback message after the use of a coupon.
    // @param QSqlQuery* query. Performs queries to DB
    // @param int memberCardId.
    // @param int paymentTypeId.
    // @param int couponId.
    // @param QString* message. Feedback message from queries.
    // @return bool true if feedback message was positive.
    bool getTotalAfterCoupon(QSqlQuery* query, int memberCardId, int paymentTypeId, int couponId, QString* message);
    // Finds applicable promotion to this box office transaction.
    // @param QSqlQuery* query. Performs queries to DB
    // @param int memberCardId.
    // @param int paymentTypeId.
    // @param QString* message. Feedback message from queries.
    // @return bool true if feedback message was positive.
    bool findApplicablePromotion(QSqlQuery* query, int memberCardId, int paymentTypeId, QString* message);

    // Populate list with customer - showtime information
    void populateLists();
    // Clear list with customer - showtime information
    void clearLists();

    // Prints receipt of box office transaction in pdf format
    void printReceipt();
    // Prints tickets of box office transaction in pdf format
    void printTickets();

    // Capitalize a String
    // @param const QString &str. String that will be capitalized
    // @return QString capitalized
    QString capitalize(const QString &str);

private:
    Ui::BoxOfficeTransactionAddForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;

    BoxOfficeTransaction boxOfficeTransaction;

    // List of payment types and IDs
    QList<QList<QString>> paymentTypes;

    // Model that contains information about showtimes
    QStandardItemModel* model;

    // Model that contains information about customers purchasing tickets
    QStandardItemModel* customerModel;
    bool isCustomerModelInitialized;
    // Table headers for Showtime table
    QStringList tableHeaders;
    // Number of columns that showtime table has
    int numberColumns;

    // Stores information of the selected Showtime
    QModelIndex selectedRow;
    // Stores information of the selected Showtime
    Showtime selectedShowtime;

    // Showtimes that customers selected
    QList<Showtime> showtimeList;
    // IDs of the Showtimes that custoners selected
    QStringList showtimeIdList;
    // Age categories which the customers are
    QStringList ageCategoryList;
    // Seats which customers selected
    QList<Seat> seatList;
    // IDS of the seats that customers selected
    QStringList seatIdList;  

    // Strings that represent the list of age categories, showtimes and seats selected by customers.
    // We send them in the query for the DB.
    QString ageCategoryString;
    QString showtimeIdString;
    QString seatIdString;

    // Total of transaction
    double total;
    // Amount that was discounted from total of transaction depending on the promotion / coupon used
    double discountedAmount;
    // Amount discounted from total depending on the number of points used
    double discountedFromPoints;

    // True if employee canceled the seat selection process
    bool seatSelectionCanceled;

    // Cost of each ticket
    QList<double> priceList;

    // Tickets made
    QList<Ticket> ticketList;

private slots:

    // Complete box office transaction
    void on_pbtContinue_clicked();
    // Cancel transaction
    void on_pbtCancel_clicked();
    // Add customers who want to watch a showtime
    void on_pbtAddCustomers_clicked();
    void on_pbtLookUpMemberCard_clicked();
    void on_pbtGetTotal_clicked();
    void on_pbtSelectSeats_clicked();

    void on_tvwTable_doubleClicked(const QModelIndex &index);
    // Delete selected row
    void on_tvwCustomers_doubleClicked(const QModelIndex &index);

    // MyLineEdit slot when it is focussed
    void on_ledCardNumber_focussed(bool hasFocus);

    void on_chkMemberCard_stateChanged(int checked);

    // Display showtimes for a particular day
    void on_dedShowDate_userDateChanged(const QDate &date);

    void on_cbxPaymentType_currentIndexChanged(const QString &text);

    // Slot when pushButton "pbtSelectSeats" from SeatSelectionForm is clicked
    void on_is_pbtSelectSeats_clicked(bool clicked, QList<Seat> seatsSelected);
};

#endif // BOXOFFICETRANSACTIONADDFORM_H
