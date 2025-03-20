/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window accepts data from employee and customer to create a new transaction.

*/

#include "boxofficetransactionaddform.h"
#include "ui_boxofficetransactionaddform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

BoxOfficeTransactionAddForm::BoxOfficeTransactionAddForm(QWidget *parent, Employee employee) :
    QMainWindow(parent),
    ui(new Ui::BoxOfficeTransactionAddForm)
{
    ui->setupUi(this);
    // Prevents user from resize window
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    seatSelectionCanceled = true;
    model = 0;
    isCustomerModelInitialized = false;
    customerModel = 0;

    boxOfficeTransaction.setEmployee(employee);
    boxOfficeTransaction.setMemberCard(MemberCard());
    // ID "1" means "No member card"
    boxOfficeTransaction.getMemberCardAddress().setId(1);

    ui->cbxAgeCategory->addItems({ "adult", "kid", "senior" });

    ui->dedShowDate->setDate(QDate(2020,11,16));

    ui->ledCardNumber->setPlaceholderText("9999 9999 9999 9999");
    // MyLineEdit slot when it is focussed
    connect (ui->ledCardNumber, SIGNAL (MyLineEdit::focussed()), this, SLOT(on_ledCardNumber_focussed(bool)));   

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load data!\n Error: " + conn.nativeErrorCode());
        close();
    }

    QSqlQuery query;

    query.prepare("SELECT * "
                  "FROM payment_type;");

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load payment types!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    // Populate comboBox "cbxPaymentType" and QList "paymentTypes"
    while(query.next())
    {
        paymentTypes.append({ query.value(0).toString(), query.value(1).toString() });
        ui->cbxPaymentType->addItem(query.value(1).toString());
    }

    conn.connClose();

    ui->cbxPaymentType->setCurrentText("Cash");
    ui->spbPointsUsed->setEnabled(false);
}

BoxOfficeTransactionAddForm::~BoxOfficeTransactionAddForm()
{
    // Make sure it was initialized
    if(customerModel != 0)
        delete customerModel;

    delete ui->tvwTable->model();
    delete model;

    for (int i = 0; i < ticketList.length(); i++) {
        ticketList[i].clearPointers();
    }

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Returns true if required fields are not blank
bool BoxOfficeTransactionAddForm::areFieldsNotBlank()
{
    QString message = "Please fill following fields:";

    // If chkMemberCard is enabled means that the customer has no member card. If it isn't enabled and no member card was provided
    // we ask to provide it.
    if(!ui->chkMemberCard->isEnabled() && boxOfficeTransaction.getMemberCard().getId() == 0)
        message += ", Member card Id";

    QString subStr(","); // String to replace.
    QString newStr(""); // Replacement string.
    message.replace(message.indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    // Returns true if all the required fields were filled up
    if(message == "Please fill following fields:")
        return true;
    else
        QMessageBox::warning(this, "Error", message + "!");

    return false;

}

// Returns true if fields had correct input
bool BoxOfficeTransactionAddForm::validateFields()
{
    QString message = "Invalid input:";

    int memberCardId = boxOfficeTransaction.getMemberCard().getId();
    QString cardNumber = ui->ledCardNumber->text();
    // Gets rid of blank spaces from card number.
    cardNumber.replace(QString(" "), QString(""));

    boxOfficeTransaction.setCardNumber(cardNumber);

    // memberCardId above 1 means customer has memberCard. If chkMemberCard is checked means the customer has no member card.
    // Returns error if user didn't input member card and didn't check chkMemberCard.
    if(!(memberCardId > 1 || ui->chkMemberCard->isChecked()))
        message += ", member card";
    // Returns error if user didn't input a valid card number when "card payment type" was specified
    if(!(ui->cbxPaymentType->currentText() == "Cash" || cardNumber.length() == 16))
        message += ", card number";
    // Returns error if user didn't select any seats
    if(seatSelectionCanceled)
        message += ", seats";

    QString subStr(","); // String to replace.
    QString newStr(""); // Replacement string.
    message.replace(message.indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    // Return true if user input valid data
    if(message == "Invalid input:")
        return true;
    else
        QMessageBox::warning(this, "Error", message + "!");

    return false;

}

// Gets total and a feedback message after the use of a coupon.
// @param QSqlQuery* query. Performs queries to DB
// @param int memberCardId.
// @param int paymentTypeId.
// @param couponId.
// @param QString* message. Feedback message from queries.
// @return bool true if feedback message was positive.
bool BoxOfficeTransactionAddForm::getTotalAfterCoupon(QSqlQuery* query, int memberCardId, int paymentTypeId, int couponId, QString* message)
{
    // Call stored procedure that get total of transaction after using coupon
    query->prepare("CALL sp_get_total_after_coupon(:member_card_id, :payment_type_id, :coupon_id, "
                  ":age_category_list, :showtime_id_list, :seat_id_list, @total, @message);");

    query->bindValue(":member_card_id", memberCardId);
    query->bindValue(":payment_type_id", paymentTypeId);
    query->bindValue(":coupon_id", couponId);
    query->bindValue(":age_category_list", ageCategoryString);
    query->bindValue(":showtime_id_list", showtimeIdString);
    query->bindValue(":seat_id_list", seatIdString);

    if(!query->exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't load coupon!\nError: " + query->lastError().nativeErrorCode());
        return false;
    }

    // Get total and feedback message
    query->prepare("SELECT @total, @message;");

    if(!query->exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't load coupon!\nError: " + query->lastError().nativeErrorCode());
        return false;
    }

    query->next();

    total = query->value(0).toDouble();
    *message = capitalize(query->value(1).toString());

    if(*message != "Coupon was used")
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", *message);
        return false;
    }

    return true;
}

// Finds applicable promotion to this box office transaction.
// @param QSqlQuery* query. Performs queries to DB
// @param int memberCardId.
// @param int paymentTypeId.
// @param QString* message. Feedback message from queries.
// @return bool true if feedback message was positive.
bool BoxOfficeTransactionAddForm::findApplicablePromotion(QSqlQuery* query, int memberCardId, int paymentTypeId, QString* message)
{
    // Call stored procedure  that finds promotions that can be applied to this transaction
    query->prepare("CALL sp_find_applicable_promotion(:member_card_id, :payment_type_id, :age_category_list, :showtime_id_list, :seat_id_list);");

    query->bindValue(":member_card_id", memberCardId);
    query->bindValue(":payment_type_id", paymentTypeId);
    query->bindValue(":age_category_list", ageCategoryString);
    query->bindValue(":showtime_id_list", showtimeIdString);
    query->bindValue(":seat_id_list", seatIdString);

    query->exec();

    query->prepare("SELECT * FROM applicable_promotion;");

    if(!query->exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't load applicable promotions!\nError: " + query->lastError().nativeErrorCode());
        return false;
    }

    // If there are applicable promotions
    if(query->next())
    {
        total = query->value(22).toDouble();

        if(query->value(1).toInt() != 0)
        {
            *message = capitalize(query->value(5).toString()) + " promotion used! (ID = " + query->value(1).toString() +")";


            PromotionType pt;
            pt.setId(query->value(2).toInt());
            pt.setName(query->value(5).toString());

            boxOfficeTransaction.setPromotion(Promotion(query->value(1).toInt(), pt, query->value(3).toDate(), query->value(4).toDate()));

        }
        else
        {
            *message = "No promotions applied.";
            boxOfficeTransaction.getPromotionAddress().setId(1);
        }

    }

    return true;
}

// Populate list with customer - showtime information
void BoxOfficeTransactionAddForm::populateLists()
{
    // Clears lists to avoid repeat of data
    clearLists();

    for(int row = 0; row < customerModel->rowCount(); row++)
    {
        int numberCustomers = customerModel->index(row , 0).data().toInt();
        QString ageCategory = customerModel->index(row , 1).data().toString();
        QString showtimeId = customerModel->index(row , 2).data().toString();

        // Get index in showtimeIdList of current showtime
        int index = showtimeIdList.indexOf(showtimeId);

        Movie movie(customerModel->index(row , 8).data().toInt(), customerModel->index(row , 3).data().toString(),
                    0, QDate(), "", "", "", "", QDate(), QDate(), Country(), QList<MovieGenre>());

        Theater theater(customerModel->index(row , 9).data().toInt(), customerModel->index(row , 7).data().toString(), TheaterType());

        Showtime showtime(customerModel->index(row , 2).data().toInt(), movie, customerModel->index(row , 4).data().toTime(),
                                           customerModel->index(row , 5).data().toTime(),  customerModel->index(row , 6).data().toDate(),
                                           theater);

        // Means it wasn't found. So just append it to the lists
        if(index == -1)
        {
            showtimeList.append(Showtime(showtime));

            // Separate groups of customers into individual entities
            for(int i = 0; i < numberCustomers; i++)
            {
                ageCategoryList.append(ageCategory);
                showtimeIdList.append(showtimeId);

                Ticket ticket;
                ticket.setAgeCategory(ageCategory);
                ticket.setShowtime(&showtime);
                ticketList.append(ticket);
            }
        }
        // Showtime was found. Means that it was previously added. We got the index of first occurence in list. We append it after index.
        //  So we will organize items in list ordered by showtime.
        else
        {
            // Index of first occurence of age category in ageCategoryList
            int ageCategoryIdx = -1;

            // We try to organize the lists ordered by showtime and then by ageCategory. To do that:
            // We go thru showtimeIdList starting from index of current showtime. If current showtimeId is the same that the one in showtimeIdList means
            // we are still in the same showtime. Then we ask if current ageCategory is the same that the one in ageCategoryList. If it does,
            // we save that index. Doing this we'll keep the age categories together in the same showtime.
            // If showtimeId wasn't the same as the one in showtimeIdList, means that we moved to a different showtime. So we don't need to compare
            // age categories since there are no age categories for that showtime yet.
            for(int i = index; i < showtimeIdList.length(); i++)
            {
                if(showtimeId == showtimeIdList[i])
                {
                    if(ageCategory == ageCategoryList[i])
                    {
                        ageCategoryIdx = i;
                        break;
                    }
                }
                else
                    break;
            }

            // Means age category wasn't found in that showtime. We just append next to the showtime index.
            if(ageCategoryIdx != -1)
                index = ageCategoryIdx;

            // Separate groups of customers into individual entities and insert them next to the showtime index.
            for(int i = 0; i < numberCustomers; i++)
            {
                ageCategoryList.insert(index, ageCategory);
                showtimeIdList.insert(index, showtimeId);

                Ticket ticket;
                ticket.setAgeCategory(ageCategory);
                ticket.setShowtime(&showtime);
                ticketList.insert(index, ticket);

                index++;
            }
        }
    }
}

// Clear list with customer - showtime information
void BoxOfficeTransactionAddForm::clearLists()
{
    showtimeList.clear();
    ageCategoryList.clear();
    showtimeIdList.clear();
    seatIdList.clear();
    seatList.clear();
    ticketList.clear();
}

// Prints receipt of box office transaction in pdf format
void BoxOfficeTransactionAddForm::printReceipt()
{
    QPdfWriter receipt("../receipt_" + QString::number(boxOfficeTransaction.getId()) + ".pdf");

    // Contains body of receipt
    QPainter receiptPainter(&receipt);

    // Controls the row where are at while printing receipt
    int row = 2;
    int receiptWidth = 3000;
    // Length of row
    int rectLenght = 250;

    int headerFontSize = 16;
    int totalFontSize = 14;
    int regularFontSize = 10;

    receiptPainter.setPen(Qt::black);
    receiptPainter.setFont(QFont("Arial", headerFontSize, QFont::Bold));

    receiptPainter.drawText(0, 0, receiptWidth, rectLenght, Qt::AlignCenter, "PORTFOLIO CINEMA");

    receiptPainter.setFont(QFont("Arial", regularFontSize));
    receiptPainter.drawText(0, rectLenght * row++, receiptWidth, rectLenght, Qt::AlignCenter, "5638 Country Lakes Dr");
    receiptPainter.drawText(0, rectLenght * row++, receiptWidth, rectLenght, Qt::AlignCenter, "Sarasota, FL 34243");
    receiptPainter.drawText(0, rectLenght * row++, receiptWidth, rectLenght, Qt::AlignCenter, "Employee: " +
                            boxOfficeTransaction.getEmployee().getFullName());
    receiptPainter.drawText(0, rectLenght * row, receiptWidth, rectLenght, Qt::AlignCenter, "Order # " +
                            QString::number(boxOfficeTransaction.getId()));

    row += 2;

    receiptPainter.drawText(0, rectLenght * row, receiptWidth, rectLenght, Qt::AlignCenter,
                            boxOfficeTransaction.getTransactionTime().toString("ddd MMMM dd, yyyy hh:mm:ss AP"));

    row += 2;
    int qtyWidth = 400;
    int descriptionWidth = 2000;
    int priceWidth = 600;

    receiptPainter.drawText(0, rectLenght * row, qtyWidth, rectLenght, Qt::AlignLeft, "Qty");
    receiptPainter.drawText(qtyWidth, rectLenght * row, descriptionWidth, rectLenght, Qt::AlignLeft, "Description");
    receiptPainter.drawText(qtyWidth + descriptionWidth, rectLenght * row++, priceWidth, rectLenght, Qt::AlignLeft, "Price");

    receiptPainter.setPen(QPen(Qt::black, 12, Qt::SolidLine, Qt::RoundCap));
    receiptPainter.drawLine(0, rectLenght * row, receiptWidth, rectLenght * row);

    receiptPainter.setFont(QFont("Arial", regularFontSize));

    // Number of people for each item in receipt
    int numberPeople = 1;
    QString ageCategory = ageCategoryList[0];
    QString showtimeId = showtimeIdList[0];
    double price = priceList[0];
    // Index of showtimeList where we are at
    int showtimeIdx = 0;

    // Loop thru list of customers who want to buy tickets
    for(int i = 1; i < showtimeIdList.length(); i++)
    {
        // If customer is from the same ageCategory and selected the same showtime, increase numberPeople by one and add cost of his ticket
        if(showtimeId == showtimeIdList[i] && ageCategory == ageCategoryList[i])
        {
            numberPeople++;
            price += priceList[i];
        }
        else
        {
            // Print item information
            receiptPainter.drawText(0, rectLenght * row, qtyWidth, rectLenght, Qt::AlignLeft, QString::number(numberPeople));
            receiptPainter.drawText(qtyWidth, rectLenght * row, descriptionWidth, rectLenght, Qt::AlignLeft, ageCategory);
            receiptPainter.drawText(qtyWidth, rectLenght * (row + 1), descriptionWidth, rectLenght, Qt::AlignLeft, showtimeList.at(showtimeIdx).getMovie().getTitle().left(16) + " " +
                                    QString::number(showtimeList.at(showtimeIdx).getId()));
            receiptPainter.drawText(qtyWidth + descriptionWidth, rectLenght * row, priceWidth, rectLenght, Qt::AlignLeft, "$" + QString::number(price, 'f', 2));

            // move to the next showtime
            if(showtimeId != showtimeIdList[i])
                showtimeIdx++;

            // reset numberPeople
            numberPeople = 1;
            // get information of customer
            ageCategory = ageCategoryList[i];
            showtimeId = showtimeIdList[i];
            // reset price and add cost of current ticket
            price = priceList[i];
            row += 2;
        }
    }

    receiptPainter.drawText(0, rectLenght * row, qtyWidth, rectLenght, Qt::AlignLeft, QString::number(numberPeople));
    receiptPainter.drawText(qtyWidth, rectLenght * row, descriptionWidth, rectLenght, Qt::AlignLeft, ageCategory);
    receiptPainter.drawText(qtyWidth, rectLenght * (row + 1), descriptionWidth, rectLenght, Qt::AlignLeft,
                            showtimeList.last().getMovie().getTitle().left(16) + " " + QString::number(showtimeList.last().getId()));
    receiptPainter.drawText(qtyWidth + descriptionWidth, rectLenght * row, priceWidth, rectLenght, Qt::AlignLeft, "$" + QString::number(price, 'f', 2));

    row += 2;

    receiptPainter.setPen(QPen(Qt::black, 12, Qt::SolidLine, Qt::RoundCap));
    receiptPainter.drawLine(0, rectLenght * row, receiptWidth, rectLenght * row);

    row++;

    receiptPainter.setFont(QFont("Arial", regularFontSize));

    receiptPainter.drawText(0, rectLenght * row, qtyWidth + descriptionWidth, rectLenght, Qt::AlignLeft, "- Promo used:");
    receiptPainter.drawText(qtyWidth + descriptionWidth, rectLenght * row++, priceWidth, rectLenght, Qt::AlignLeft, "$" + QString::number(discountedAmount, 'f', 2));

    receiptPainter.drawText(0, rectLenght * row, qtyWidth + descriptionWidth, rectLenght, Qt::AlignLeft, "- Points used:");
    receiptPainter.drawText(qtyWidth + descriptionWidth, rectLenght * row, priceWidth, rectLenght, Qt::AlignLeft, "$" + QString::number(discountedFromPoints, 'f', 2));

    row += 2;

    receiptPainter.setFont(QFont("Arial", totalFontSize, QFont::Bold));
    receiptPainter.drawText(0, rectLenght * row, receiptWidth, rectLenght, Qt::AlignCenter, "TOTAL SALE: $" + QString::number(total, 'f', 2));

    row += 2;

    receiptPainter.setFont(QFont("Arial", regularFontSize));

    QString paymentText = "Payment type: " + boxOfficeTransaction.getPaymentType().getName();
    // If customer paid with card, print last 4 digits of card number
    if(boxOfficeTransaction.getCardNumber() != "")
        paymentText += " " + boxOfficeTransaction.getCardNumber().mid(12);

    receiptPainter.drawText(0, rectLenght * row++, receiptWidth, rectLenght, Qt::AlignLeft, paymentText);

    QString memberCardText = "Member card: ";
    // If customer has a member card, print it.
    if(boxOfficeTransaction.getMemberCard().getId() > 1)
        memberCardText +=  QString::number(boxOfficeTransaction.getMemberCard().getId());
    else
        memberCardText += "N/A";

    receiptPainter.drawText(0, rectLenght * row++, receiptWidth, rectLenght, Qt::AlignLeft, memberCardText);

    QString promoText = "";
    // If promotion was used, print its information
    if(boxOfficeTransaction.getPromotion().getId() > 1)
        promoText +=  "Promotion: " + QString::number(boxOfficeTransaction.getPromotion().getId()) +
                " - " + boxOfficeTransaction.getPromotion().getPromotionType().getName();
    // Else if coupon was used, print its information
    else if(boxOfficeTransaction.getCoupon().getId() > 1)
        promoText +=  "Coupon: " + QString::number(boxOfficeTransaction.getCoupon().getId()) +
                " - " + boxOfficeTransaction.getCoupon().getPromotionType().getName();

    receiptPainter.drawText(0, rectLenght * row, receiptWidth, rectLenght, Qt::AlignLeft, promoText);

    receiptPainter.end();
}

// Prints tickets of box office transaction in pdf format
void BoxOfficeTransactionAddForm::printTickets()
{
    // Loop thru ticketList
    for(int i = 0; i < ticketList.length(); i++)
    {
        QPdfWriter ticket("../ticket_" + QString::number(ticketList[i].getId()) + ".pdf");

        // Contains body of ticket
        QPainter ticketPainter(&ticket);

        // Controls the row where are at while printing receipt
        int row = 2;
        int ticketWidth = 3000;
        // Length of row
        int rectLenght = 250;
        int seatRectWidth = 750;

        int headerFontSize = 16;
        int regularFontSize = 10;

        ticketPainter.setPen(Qt::black);
        ticketPainter.setFont(QFont("Arial", headerFontSize, QFont::Bold));

        ticketPainter.drawText(0, 0, ticketWidth, rectLenght, Qt::AlignCenter, "PORTFOLIO CINEMA");

        ticketPainter.setPen(Qt::white);
        ticketPainter.setFont(QFont("Arial", headerFontSize));

        ticketPainter.fillRect(0, rectLenght * row, ticketWidth, rectLenght, Qt::black);
        ticketPainter.drawText(0, rectLenght * row, ticketWidth, rectLenght, Qt::AlignCenter,
                               ticketList[i].getShowtime()->getMovie().getTitle().left(16));

        row += 2;

        ticketPainter.setPen(QPen(Qt::black, 12, Qt::DashLine, Qt::RoundCap));
        ticketPainter.drawLine(0, rectLenght * row, ticketWidth, rectLenght * row);

        row++;
        ticketPainter.setPen(Qt::black);

        ticketPainter.drawText(0, rectLenght * row++, ticketWidth, rectLenght, Qt::AlignCenter,
                                ticketList[i].getShowtime()->getShowDate().toString("ddd MMMM dd, yyyy"));
        ticketPainter.drawText(0, rectLenght * row++, ticketWidth, rectLenght, Qt::AlignCenter, "at " +
                                ticketList[i].getShowtime()->getStartAt().toString("hh:mm AP"));
        ticketPainter.drawText(0, rectLenght * row++, ticketWidth, rectLenght, Qt::AlignCenter,
                               ticketList[i].getShowtime()->getTheater().getName().left(16));

        ticketPainter.setPen(Qt::white);

        ticketPainter.fillRect(ticketWidth / 4, rectLenght * row, seatRectWidth, rectLenght, Qt::black);
        ticketPainter.drawText(ticketWidth / 4, rectLenght * row, ticketWidth / 4, rectLenght, Qt::AlignCenter,
                               QChar(ticketList[i].getSeat()->getSeatRow()));
        ticketPainter.fillRect(ticketWidth / 4 * 3, rectLenght * row, seatRectWidth, rectLenght, Qt::black);
        ticketPainter.drawText(ticketWidth / 4 * 3, rectLenght * row, ticketWidth / 4, rectLenght, Qt::AlignCenter,
                               ticketList[i].getSeat()->getSeatColumn());

        ticketPainter.setPen(Qt::black);

        ticketPainter.drawText(0, rectLenght * row, ticketWidth / 4, rectLenght, Qt::AlignLeft, "Row:");
        ticketPainter.drawText(ticketWidth / 2, rectLenght * row, ticketWidth / 4 * 3, rectLenght, Qt::AlignLeft, "Seat:");

        row += 2;

        ticketPainter.setPen(QPen(Qt::black, 12, Qt::DashLine, Qt::RoundCap));
        ticketPainter.drawLine(0, rectLenght * row, ticketWidth, rectLenght * row);

        row++;
        ticketPainter.setFont(QFont("Arial", regularFontSize));

        ticketPainter.drawText(0, rectLenght * row++, ticketWidth, rectLenght, Qt::AlignLeft,
                               "Age category: " + ticketList[i].getAgeCategory());
        ticketPainter.drawText(0, rectLenght * row++, ticketWidth, rectLenght, Qt::AlignLeft,
                               "Order #: " + QString::number(boxOfficeTransaction.getId()));
        ticketPainter.drawText(0, rectLenght * row++, ticketWidth, rectLenght, Qt::AlignLeft,
                               "Date / Time: ");
        ticketPainter.drawText(0, rectLenght * row++, ticketWidth, rectLenght, Qt::AlignLeft,
                               boxOfficeTransaction.getTransactionTime().toString("ddd MMMM dd, yyyy hh:mm:ss AP"));

        ticketPainter.end();
    }

}

// Capitalize a String
// @param const QString &str. String that will be capitalized
// @return QString capitalized
QString BoxOfficeTransactionAddForm::capitalize(const QString &str)
{
    QString tmp = str;
    // if you want to ensure all other letters are lowercase:
    //tmp = tmp.toLower();
    tmp[0] = str[0].toUpper();
    return tmp;
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void BoxOfficeTransactionAddForm::on_tvwTable_doubleClicked(const QModelIndex &index)
{
    // Store information of selected showtime
    selectedShowtime.setMovie(Movie(index.sibling(index.row(),6).data().toInt(), index.sibling(index.row(),1).data().toString(),
                               0, QDate(), "", "", "", "", QDate(), QDate(), Country(), QList<MovieGenre>()));
    selectedShowtime.setStartAt(index.sibling(index.row(),2).data().toTime());
    selectedShowtime.setEndAt(index.sibling(index.row(),3).data().toTime());
    selectedShowtime.setShowDate(index.sibling(index.row(),4).data().toDate());
    selectedShowtime.setTheater(Theater(index.sibling(index.row(),7).data().toInt(), index.sibling(index.row(),5).data().toString(),
                                        TheaterType()));

    // Display information of selected showtime
    ui->lblShowtime->setText(selectedShowtime.getShowDate().toString("yyyy-MM-dd") + " " + selectedShowtime.getStartAt().toString("hh:mm:ss")
                             + " - " +selectedShowtime.getMovie().getTitle());

    // Store index
    selectedRow = index;
}

// MyLineEdit slot when it is focussed
void BoxOfficeTransactionAddForm::on_ledCardNumber_focussed(bool hasFocus)
{
    if(hasFocus)
        ui->ledCardNumber->setInputMask("9999 9999 9999 9999");
}

// Complete box office transaction
void BoxOfficeTransactionAddForm::on_pbtContinue_clicked()
{
    discountedAmount = 0;

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't complete transaction!\n Error: " + conn.nativeErrorCode());
        return;
    }

    if(!conn.transaction())
    {
        QMessageBox::warning(this, "Error", "Couldn't complete transaction!\n Error: " + conn.nativeErrorCode());
        return;
    }

    QSqlQuery query;

    QString discountWay = "none";
    int discountWayId = 1;
    // If coupon was used
    if(boxOfficeTransaction.getCoupon().getId() > 1)
    {
        discountWay = "coupon";
        discountWayId = boxOfficeTransaction.getCoupon().getId();
    }
    else
        // If promotion was used
        if(boxOfficeTransaction.getPromotion().getId() > 1)
        {
            discountWay = "promotion";
            discountWayId = boxOfficeTransaction.getPromotion().getId();
        }

    // Stored procedure that makes a box office transaction
    QString queryText = "CALL sp_box_office_transaction(:member_card_id, :payment_type_id, :discount_way, :discount_way_id, %1, "
                        ":points_used, :employee_id, :age_category_list, :showtime_id_list, :seat_id_list, @message);";

    // If card was used give card number, else set NULL
    if(boxOfficeTransaction.getCardNumber().length() == 16)
        queryText = queryText.arg(":card_number");
    else
        queryText = queryText.arg("NULL");

        query.prepare(queryText);

        query.bindValue(":member_card_id", boxOfficeTransaction.getMemberCard().getId());
        query.bindValue(":payment_type_id", boxOfficeTransaction.getPaymentType().getId());
        query.bindValue(":discount_way", discountWay);
        query.bindValue(":discount_way_id", discountWayId);
        query.bindValue(":card_number", boxOfficeTransaction.getCardNumber());
        query.bindValue(":points_used", boxOfficeTransaction.getPointsUsed());
        query.bindValue(":employee_id", boxOfficeTransaction.getEmployee().getId());
        query.bindValue(":age_category_list", ageCategoryString);
        query.bindValue(":showtime_id_list", showtimeIdString);
        query.bindValue(":seat_id_list", seatIdString);

        if(!query.exec())
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "Couldn't complete transaction!\nError: " + query.lastError().nativeErrorCode());
            return;
        }

        // Get feedback message and ID of new box office transaction
        query.prepare("SELECT @message, @box_office_transaction_id;");

        if(!query.exec())
        {
            conn.rollback();
            conn.connClose();
            QMessageBox::warning(this, "Error", "Couldn't complete transaction!\nError: " + query.lastError().nativeErrorCode());
            return;
        }

        query.next();

        QString message = capitalize(query.value(0).toString());

        if(message != "Tickets created")
        {
            conn.rollback();
            conn.connClose();
            QMessageBox::warning(this, "Error", "Couldn't complete transaction!\nError: " + message);
            return;
        }

        boxOfficeTransaction.setId(query.value(1).toInt());

        // If coupon was used
        if(discountWay == "coupon")
        {
            query.prepare("SELECT * FROM tmp_coupon_used;");

            if(!query.exec())
            {
                conn.rollback();
                conn.connClose();
                QMessageBox::warning(this, "Error", "Couldn't load coupon!\nError: " + query.lastError().nativeErrorCode());
                return;
            }

            query.next();

            PromotionType pt;
            pt.setId(query.value(1).toInt());
            pt.setName(query.value(2).toString());

            boxOfficeTransaction.setCoupon(Coupon(query.value(0).toInt(), pt, query.value(3).toDate(), query.value(4).toDate(),
                                           query.value(5).toBool()));

            // Get discounted amount from coupon
            discountedAmount = query.value(6).toDouble();
        }

        // If promotion was used
        if(discountWay == "promotion")
        {
            query.prepare("SELECT * FROM tmp_promotion_used;");

            if(!query.exec())
            {
                conn.rollback();
                conn.connClose();
                QMessageBox::warning(this, "Error", "Couldn't load applicable promotions!\nError: " + query.lastError().nativeErrorCode());
                return;
            }

            query.next();

            PromotionType pt;
            pt.setId(query.value(1).toInt());
            pt.setName(query.value(2).toString());

            boxOfficeTransaction.setPromotion(Promotion(query.value(0).toInt(), pt, query.value(3).toDate(), query.value(4).toDate()));

            // Get discounted amount from promotion
            discountedAmount = query.value(5).toDouble();
        }

        // Get IDs from tickets created
        query.prepare("SELECT ticket_id "
                      "FROM ticket "
                      "WHERE box_office_transaction_id = :box_office_transaction_id;");
        query.bindValue(":box_office_transaction_id", boxOfficeTransaction.getId());

        if(!query.exec())
        {
            conn.rollback();
            conn.connClose();
            QMessageBox::warning(this, "Error", "Couldn't load tickets!\nError: " + query.lastError().nativeErrorCode());
            return;
        }

        int i = 0;
        while(query.next())
        {
            ticketList[i].setId(query.value(0).toInt());
            ticketList[i++].getBoxOfficeTransaction()->setId(boxOfficeTransaction.getId());
        }

        if(!conn.commit())
        {
            conn.rollback();
            conn.connClose();
            QMessageBox::warning(this, "Error", "Couldn't load tickets!\nError: " + query.lastError().nativeErrorCode());
            return;
        }

        conn.connClose();
        QMessageBox::information(this, "Success", message + " Order # " + QString::number (boxOfficeTransaction.getId()));

        printReceipt();
        printTickets();

        // If Employee is manager just close window
        if(boxOfficeTransaction.getEmployee().getId() < 3)
        {
            this->parentWidget()->close();
        }

        // If He is box office employee close window and show main menu
        else
        {
            this->parentWidget()->show();
            this->close();
        }

}

// Cancel transaction
void BoxOfficeTransactionAddForm::on_pbtCancel_clicked()
{
    // If Employee is manager just close window
    if(boxOfficeTransaction.getEmployee().getId() < 3)
    {
        this->parentWidget()->close();
    }

    // If He is box office employee close window and show main menu
    else
    {
        this->parentWidget()->show();
        this->close();
    }
}

// Add customers who want to watch a showtime
void BoxOfficeTransactionAddForm::on_pbtAddCustomers_clicked()
{
    if(ui->spbNumberCustomers->value() == 0)
    {
        QMessageBox::warning(this, "Error", "Number of customers must be at least 1!");
        return;
    }

    if(ui->lblShowtime->text() == "Not showtime selected...")
    {
        QMessageBox::warning(this, "Error", "Please select a showtime! (Double click the desired showtime)");
        return;
    }

    if(!isCustomerModelInitialized)
    {
        customerModel= new QStandardItemModel(0, numberColumns + 2);

        QStringList customerModelHeaders;
        customerModelHeaders.append({"No. Customers", "Age category"});
        customerModelHeaders.append(tableHeaders);

        customerModel->setHorizontalHeaderLabels(customerModelHeaders);

        ui->tvwCustomers->setModel(customerModel);

        ui->tvwCustomers->hideColumn(8);
        ui->tvwCustomers->hideColumn(9);

        // Prevent user from edit data from table
        ui->tvwCustomers->setEditTriggers(QAbstractItemView::NoEditTriggers);

        isCustomerModelInitialized = true;
    }

    QList<QStandardItem*> items;
    for(int i = 0; i < numberColumns + 2; i++)
        items.append(new QStandardItem());

    items[0]->setData(ui->spbNumberCustomers->value(), Qt::DisplayRole);
    items[1]->setData(ui->cbxAgeCategory->currentText(), Qt::DisplayRole);

    items[2]->setData(selectedRow.sibling(selectedRow.row(),0).data().toInt(), Qt::DisplayRole);
    items[3]->setData(selectedRow.sibling(selectedRow.row(),1).data().toString (), Qt::DisplayRole);
    items[4]->setData(selectedRow.sibling(selectedRow.row(),2).data().toTime(), Qt::DisplayRole);
    items[5]->setData(selectedRow.sibling(selectedRow.row(),3).data().toTime(), Qt::DisplayRole);

    items[6]->setData(selectedRow.sibling(selectedRow.row(),4).data().toDate(), Qt::DisplayRole);
    items[7]->setData(selectedRow.sibling(selectedRow.row(),5).data().toString (), Qt::DisplayRole);
    items[8]->setData(selectedRow.sibling(selectedRow.row(),6).data().toInt (), Qt::DisplayRole);
    items[9]->setData(selectedRow.sibling(selectedRow.row(),7).data().toInt (), Qt::DisplayRole);

    // Add No. Customers, Age category and showtime data to table
    customerModel->appendRow(items);
}

void BoxOfficeTransactionAddForm::on_chkMemberCard_stateChanged(int checked)
{
    // If chkMemberCard is checked means the customer doesn't have a member card, so we enable spbMemberCardId to accept the member card
    ui->spbMemberCardId->setEnabled(!checked);
    ui->lblPointsUsed->setText("Points used");
    ui->spbPointsUsed->setEnabled(false);
    ui->spbPointsUsed->setValue(0);

    // We set member card Id value as "1" which means "No member card"
    if(checked)
    {
        boxOfficeTransaction.setMemberCard(MemberCard());
        boxOfficeTransaction.getMemberCardAddress().setId(1);
        ui->pbtSelectSeats->setEnabled(true);
    }
    else
        // If customer doesn't have member card, enable pbtSelectSeats to select seats
        if(boxOfficeTransaction.getMemberCard().getId() > 1)
            ui->pbtSelectSeats->setEnabled(true);
    // The user has either to check chkMemberCard or input member card to enable pbtSelectSeats
    else
            ui->pbtSelectSeats->setEnabled(false);
}

void BoxOfficeTransactionAddForm::on_pbtLookUpMemberCard_clicked()
{
    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load member card!\n Error: " + conn.nativeErrorCode());
        // Reset fields to original status
        ui->chkMemberCard->setCheckState(Qt::Checked);
        ui->chkMemberCard->setCheckState(Qt::Unchecked);
        return;
    }

    QSqlQuery query;

    // Get customer and member card data
    query.prepare("SELECT m.member_card_id, m.points, c.first_name, c.middle_name, c.last_name "
                  "FROM customer c "
                  "INNER JOIN member_card m "
                  "ON c.member_card_id = m.member_card_id "
                  "WHERE c.member_card_id = :member_card_id");

    query.bindValue(":member_card_id", ui->spbMemberCardId->value());

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load member card!\nError: " + query.lastError().nativeErrorCode());
        // Reset fields to original status
        ui->chkMemberCard->setCheckState(Qt::Checked);
        ui->chkMemberCard->setCheckState(Qt::Unchecked);
        return;
    }

    if(!query.next())
    {
        QMessageBox::warning(this, "Error", "Couldn't find member card!");
        // Reset fields to original status
        ui->chkMemberCard->setCheckState(Qt::Checked);
        ui->chkMemberCard->setCheckState(Qt::Unchecked);
        return;
    }

    boxOfficeTransaction.setMemberCard(MemberCard(query.value(0).toInt(), query.value(1).toInt(), 0, QDate()));

    // Get full name of customer
    QString customer = query.value(2).toString() + " ";
    customer += query.value(3).toString() == "" ? query.value(4).toString()
                                                : query.value(3).toString() + " " + query.value(4).toString();

    conn.connClose();

    QMessageBox::information(this, "Success", "Customer found: " + customer + "!");

    ui->pbtSelectSeats->setEnabled(true);

    // Display the amount of points available
    ui->lblPointsUsed->setText("Points used (" + QString::number(boxOfficeTransaction.getMemberCard().getPoints())
                               + " points avail.)");
    // Maximum of spbPointsUsed will be the amount of points available
    ui->spbPointsUsed->setMaximum(boxOfficeTransaction.getMemberCard().getPoints());
    ui->spbPointsUsed->setEnabled(true);
}

// Display showtimes for a particular day
void BoxOfficeTransactionAddForm::on_dedShowDate_userDateChanged(const QDate &date)
{
    // clear tableHeaders to avoid repeat of headers
    tableHeaders.clear();

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load showtimes!\n Error: " + conn.nativeErrorCode());
        close();
    }

    QSqlQuery query;

    // Get showtimes for that day
    query.prepare("SELECT s.showtime_id AS ID, m.title AS Movie, s.start_at AS 'Start at', s.end_at AS 'End at', "
                  "s.show_date AS 'Show date', t.name AS 'Theater', m.movie_id, t.theater_id "
                  "FROM showtime s "
                  "INNER JOIN movie m "
                  "ON s.movie_id = m.movie_id "
                  "INNER JOIN theater t "
                  "ON s.theater_id = t.theater_id "
                  "WHERE s.show_date = :date "
                  "ORDER BY m.movie_id, s.start_at;");

    query.bindValue(":date", date.toString("yyyy-MM-dd"));

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load showtimes!\nError: " + query.lastError().nativeErrorCode());
        return;
    }

    numberColumns = query.record().count();


    delete ui->tvwTable->model();
    delete model;
    model= new QStandardItemModel(query.size(), numberColumns);
    for(int i = 0; i < numberColumns; i++)
        tableHeaders.append(query.record().fieldName(i));

    model->setHorizontalHeaderLabels(tableHeaders);

    int row =0;
    // Populate table with showtimes
    while(query.next())
    {
        QList<QStandardItem*> items;
        for(int i = 0; i < numberColumns; i++)
            items.append(new QStandardItem());

        items[0]->setData(query.value (0).toInt (), Qt::DisplayRole);
        items[1]->setData(query.value (1).toString (), Qt::DisplayRole);
        items[2]->setData(query.value (2).toTime(), Qt::DisplayRole);
        items[3]->setData(query.value (3).toTime(), Qt::DisplayRole);

        items[4]->setData(query.value (4).toDate(), Qt::DisplayRole);
        items[5]->setData(query.value (5).toString (), Qt::DisplayRole);
        items[6]->setData(query.value (6).toInt (), Qt::DisplayRole);
        items[7]->setData(query.value (7).toInt (), Qt::DisplayRole);

        // Add cells to row
        for(int column = 0; column < numberColumns; column++)
            model->setItem(row, column, items[column]);
        row++;
    }


    ui->tvwTable->setModel(model);

    // Model that filters data
    QSortFilterProxyModel *m=new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);

    ui->tvwTable->setModel(m);
    ui->tvwTable->setSortingEnabled(true);
    ui->tvwTable->sortByColumn(1, Qt::AscendingOrder);
    // Hide m.movie_id, t.theater_id
    ui->tvwTable->hideColumn(6);
    ui->tvwTable->hideColumn(7);
    // Prevent user from edit table
    ui->tvwTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvwTable->setSelectionBehavior(QAbstractItemView::SelectItems );
    // Prevent user from selecting multiple rows
    ui->tvwTable->setSelectionMode(QAbstractItemView::SingleSelection );

    conn.connClose();


}

void BoxOfficeTransactionAddForm::on_cbxPaymentType_currentIndexChanged(const QString &text)
{
    // If payment type is "Cash". Disable ledCardNumber and remove input mask so we can see placeholder
    if(text == "Cash")
    {
        ui->ledCardNumber->setEnabled(false);
        ui->ledCardNumber->setInputMask("");
        ui->ledCardNumber->setText("");
    }
    else
        ui->ledCardNumber->setEnabled(true);

}

// Delete selected row
void BoxOfficeTransactionAddForm::on_tvwCustomers_doubleClicked(const QModelIndex &index)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of row No. " + QString::number(index.row()+1) + "?" ,
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Delete QStandardItem pointers
    for(int column = 0; column <= customerModel->columnCount(); column++)
        delete customerModel->item(index.row(), column);

    customerModel->takeRow(index.row());
}

void BoxOfficeTransactionAddForm::on_pbtGetTotal_clicked()
{
    // Disable pbtContinue in case it was previously enabled and we run into a problem so It won't stay enabled.
    ui->pbtContinue->setEnabled(false);

    // If user didn't input correct data return
    if(!validateFields())
        return;

    // If chkMemberCard is checked means customer has no member card. So we set ID as "1" which means "No member card"
    if(ui->chkMemberCard->isChecked())
        boxOfficeTransaction.getMemberCardAddress().setId(1);

    int memberCardId = boxOfficeTransaction.getMemberCard().getId();


    int couponId = ui->spbCouponId->value();
    // If spbCouponId value is less than 2, means no coupon was used.
    if(ui->spbCouponId->value() < 2)
        boxOfficeTransaction.getCouponAddress().setId(1);
    else
        boxOfficeTransaction.getCouponAddress().setId(couponId);

    int paymentTypeId = paymentTypes[ui->cbxPaymentType->currentIndex()][0].toInt();
    boxOfficeTransaction.setPaymentType(PaymentType(paymentTypeId, ui->cbxPaymentType->currentText()));

    // Convert lists into QString which elements are separated by commas.
    ageCategoryString = ageCategoryList.join(",") + ",";
    showtimeIdString = showtimeIdList.join(",") + ",";
    seatIdString = seatIdList.join(",") + ",";

    // Get points used
    boxOfficeTransaction.setPointsUsed(ui->spbPointsUsed->value());

    // reset total
    total = 0;
    QString message = "";

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load applicable promotions / coupon!\n Error: " + conn.nativeErrorCode());
        return;
    }

    QSqlQuery query;

    // If coupon was used
    if(couponId != 0)
    {
        /*
        // Call stored procedure that get total of transaction after using coupon
        query.prepare("CALL sp_get_total_after_coupon(:member_card_id, :payment_type_id, :coupon_id, "
                      ":age_category_list, :showtime_id_list, :seat_id_list, @total, @message);");

        query.bindValue(":member_card_id", memberCardId);
        query.bindValue(":payment_type_id", paymentTypeId);
        query.bindValue(":coupon_id", couponId);
        query.bindValue(":age_category_list", ageCategoryString);
        query.bindValue(":showtime_id_list", showtimeIdString);
        query.bindValue(":seat_id_list", seatIdString);

        if(!query.exec())
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "Couldn't load coupon!\nError: " + query.lastError().nativeErrorCode());
            return;
        }

        // Get total and feedback message
        query.prepare("SELECT @total, @message;");

        if(!query.exec())
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "Couldn't load coupon!\nError: " + query.lastError().nativeErrorCode());
            return;
        }

        query.next();

        total = query.value(0).toDouble();
        message = capitalize(query.value(1).toString());

        if(message != "Coupon was used")
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", message);
            return;
        }
        */

        if(!getTotalAfterCoupon(&query, memberCardId, paymentTypeId, couponId, &message))
            return;

    }
    else
    {
        /*
        // Call stored procedure  that finds promotions that can be applied to this transaction
        query.prepare("CALL sp_find_applicable_promotion(:member_card_id, :payment_type_id, :age_category_list, :showtime_id_list, :seat_id_list);");

        query.bindValue(":member_card_id", memberCardId);
        query.bindValue(":payment_type_id", paymentTypeId);
        query.bindValue(":age_category_list", ageCategoryString);
        query.bindValue(":showtime_id_list", showtimeIdString);
        query.bindValue(":seat_id_list", seatIdString);

        query.exec();

        query.prepare("SELECT * FROM applicable_promotion;");

        if(!query.exec())
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "Couldn't load applicable promotions!\nError: " + query.lastError().nativeErrorCode());
            return;
        }

        // If there are applicable promotions
        if(query.next())
        {
            total = query.value(22).toDouble();

            if(query.value(1).toInt() != 0)
            {
                message = capitalize(query.value(5).toString()) + " promotion used! (ID = " + query.value(1).toString() +")";


                PromotionType pt;
                pt.setId(query.value(2).toInt());
                pt.setName(query.value(5).toString());

                boxOfficeTransaction.setPromotion(Promotion(query.value(1).toInt(), pt, query.value(3).toDate(), query.value(4).toDate()));

            }
            else
            {
                message = "No promotions applied.";
                boxOfficeTransaction.getPromotionAddress().setId(1);
            }

        }
        */

        if(!findApplicablePromotion(&query, memberCardId, paymentTypeId, &message))
            return;

    }

    // Get cost of each ticket
    query.prepare("SELECT total_price FROM customer_showtime_temp_table;");

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't get total!\nError: " + query.lastError().nativeErrorCode());
        return;
    }

    // Stores cost of tickets in priceList
    while(query.next())
        priceList.append(query.value(0).toDouble());

    conn.connClose();
    QMessageBox::information(this, "Success", message);

    // Calculate the discounted amount from the points used
    discountedFromPoints = ui->spbPointsUsed->value() / 100.0;
    total -= discountedFromPoints;
    ui->lblTotal->setText("$ " + QString::number(total, 'f', 2));
    ui->pbtContinue->setEnabled(true);

}

void BoxOfficeTransactionAddForm::on_pbtSelectSeats_clicked()
{
    // If customerModel wasn't initialized or it has no rows
    if(!isCustomerModelInitialized || customerModel->rowCount() == 0)
    {
        QMessageBox::warning(this, "Error", "Please add customers!");
        return;
    }

    // Populate list with customer - showtime information
    populateLists();

    int showtimeId = showtimeIdList.at(0).toInt();

    if(showtimeIdList.length() == 1)
    {
        // Open SeatSelectionForm to select seats
        SeatSelectionForm *form = new SeatSelectionForm(this, showtimeList[0], 1);
        connect (form, SIGNAL (is_pbtSelectSeats_clicked(bool, QList<Seat>)), this, SLOT(on_is_pbtSelectSeats_clicked(bool, QList<Seat>)));
        form->exec();

        delete form;
    }
    else
    {
        // Number of people for each showtime
        int numberPeople = 1;
        int showtimeIdx = 0;

        // Append another item to go thru all showtimes without throwing List exception
        showtimeIdList.append("-1");

        // Open a SeatSelectionForm for each showtime to select seats
        for(int i = 1; i < showtimeIdList.length(); i++)
        {
            // If next showtime is the same as the previous one increase numberPeople by one
            if(showtimeId == showtimeIdList[i].toInt())
            {
                numberPeople++;
            }
            else
            {
                // Open SeatSelectionForm to select seats
                SeatSelectionForm *form = new SeatSelectionForm(this, showtimeList[showtimeIdx++], numberPeople);
                connect (form, SIGNAL (is_pbtSelectSeats_clicked(bool, QList<Seat>)), this, SLOT(on_is_pbtSelectSeats_clicked(bool, QList<Seat>)));
                form->exec();
                delete form;

                // If pbtCancel was clicked in SeatSelectionForm, cancel seat selection process
                if(seatSelectionCanceled)
                    return;

                // Reset numberPeople
                numberPeople = 1;
                // Move to next showtime
                showtimeId = showtimeIdList[i].toInt();
            }
        }

        // Get rid of item that prevented exception
        showtimeIdList.takeLast();
    }
}

void BoxOfficeTransactionAddForm::on_is_pbtSelectSeats_clicked(bool clicked, QList<Seat> seatsSelected)
{
    if(clicked)
    {
        // Add selected seats to lists
        for(int i = 0; i < seatsSelected.length(); i++)
        {
            seatIdList.append(QString::number(seatsSelected[i].getId()));
            seatList.append(seatsSelected[i]);
            Seat* seat = &seatsSelected[i];
            ticketList[i].setSeat(seat);
        }

        seatSelectionCanceled = false;
    }
    else
        seatSelectionCanceled = true;
}
