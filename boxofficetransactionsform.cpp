#include "boxofficetransactionsform.h"
#include "ui_boxofficetransactionsform.h"

BoxOfficeTransactionsForm::BoxOfficeTransactionsForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BoxOfficeTransactionsForm)
{
    ui->setupUi(this);
    // Prevents user from resize window
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    ui->ledCardNumber->setPlaceholderText("9999 9999 9999 9999");

    ui->dteTransactionTimeTo->setDateTime(QDateTime::currentDateTime());

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load data!\n Error: " + conn.nativeErrorCode());
        close();
    }

    QSqlQuery query;

    query.prepare("SELECT * "
                  "FROM payment_type;");

    paymentTypes.append({"0", "Any"});
    ui->cbxPaymentType->addItem("Any");

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load payment types!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    while(query.next())
    {
        paymentTypes.append({ query.value(0).toString(), query.value(1).toString() });
        ui->cbxPaymentType->addItem(query.value(1).toString());
    }

    query.prepare("SELECT employee_id, first_name, middle_name, last_name "
                  "FROM employee;");

    employees.append({"0", "Any"});
    ui->cbxEmployee->addItem("Any");

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load employees!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    while(query.next())
    {
        QString fullName = query.value(1).toString() + " ";
        fullName += query.value(2).toString() == "" ? query.value(3).toString() :
                                                      query.value(2).toString() + " " + query.value(3).toString();
        employees.append({ query.value(0).toString(), fullName });
        ui->cbxEmployee->addItem(fullName);
    }

    conn.connClose();

    // Connect signal to slot on_ledCardNumber_focussed for when user focussed MyLineEdit.
    connect (ui->ledCardNumber, SIGNAL (MyLineEdit::focussed()), this, SLOT(on_ledCardNumber_focussed(bool)));
}

BoxOfficeTransactionsForm::~BoxOfficeTransactionsForm()
{
    delete ui;
}

// Load table with box office transactions data
bool BoxOfficeTransactionsForm::loadTable()
{
    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load showtimes!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;
    QString queryText;

    queryText = "SELECT bo.box_office_transaction_id AS ID, cu.customer_id AS 'Customer ID', cu.first_name AS 'Customer first name', cu.middle_name AS 'Customer middle name', "
                "cu.last_name AS 'Customer last name', cu.member_card_id AS 'Member card ID', pt.name AS 'Payment type', bo.coupon_id AS 'Coupon ID', "
                "bo.promotion_id AS 'Promotion ID', bo.card_number AS 'Card number', bo.points_used AS 'Points used', bo.number_of_tickets AS 'No. Tickets', "
                "bo.total AS Total, em.employee_id AS 'Employee ID', em.first_name AS 'Employee first name', em.middle_name AS 'Employee middle name', "
                "em.last_name AS 'Employee last name', bo.transaction_time AS 'Transaction time', pt.payment_type_id AS 'Payment type ID' "
                "FROM box_office_transaction bo "
                "INNER JOIN payment_type pt "
                "ON bo.payment_type_id = pt.payment_type_id "
                "INNER JOIN employee em "
                "ON bo.employee_id = em.employee_id "
                "LEFT JOIN customer cu "
                "ON bo.member_card_id = cu.member_card_id "
                "WHERE 1 = 1 ";

    //------------------------------------------------------------------------------------------------------------------------------
    // Add "ADD" statements to query depending on the fields the user filled

    QString memberCardId = QString::number(ui->spbMemberCardId->value());
    queryText += memberCardId != "0"? " AND cu.member_card_id = :member_card_id" : "";

    QString paymentType = ui->cbxPaymentType->currentText();
    queryText += paymentType != "Any"? " AND pt.name = :payment_type" : "";

    QString couponId = QString::number(ui->spbCouponId->value());
    queryText += couponId != "0"? " AND bo.coupon_id = :coupon_id" : "";

    QString promotionId = QString::number(ui->spbPromotionId->value());
    queryText += promotionId != "0"? " AND bo.promotion_id = :promotion_id" : "";

    QString cardNumber = ui->ledCardNumber->text();
    cardNumber.replace(QString(" "), QString(""));
    queryText += cardNumber != ""? " AND bo.card_number = :card_number" : "";

    QString pointsUsedFrom = QString::number(ui->spbPointsUsedFrom->value());
    queryText += pointsUsedFrom != "0"? " AND bo.points_used >= :points_used_from" : "";

    QString pointsUsedTo = QString::number(ui->spbPointsUsedTo->value());
    queryText += pointsUsedTo != "0"? " AND bo.points_used <= :points_used_to" : "";

    QString numberTickets = QString::number(ui->spbNumberTickets->value());
    queryText += numberTickets != "0"? " AND bo.number_of_tickets = :number_of_tickets" : "";

    double totalFrom = ui->dspTotalFrom->value();
    queryText += totalFrom != 0? " AND bo.total >= :total_from" : "";

    double totalTo = ui->dspTotalTo->value();
    queryText += totalTo != 0? " AND bo.total <= :total_to" : "";

    QString employeeId = employees.at(ui->cbxEmployee->currentIndex()).at(0);
    queryText += employeeId != "0"? " AND em.employee_id = :employee_id" : "";

    QString transactionTimeFrom = ui->dteTransactionTimeFrom->dateTime().toString("yyyy-M-d hh:mm:ss");
    queryText += ui->chkTransactionTimeFrom->isChecked()? " AND bo.transaction_time >= :transaction_time_from" : "";

    QString transactionTimeTo = ui->dteTransactionTimeTo->dateTime().toString("yyyy-M-d hh:mm:ss");
    queryText += ui->chkTransactionTimeTo->isChecked()? " AND bo.transaction_time <= :transaction_time_to" : "";

    query.prepare( queryText + " ORDER BY bo.box_office_transaction_id;");

    query.bindValue(":member_card_id", memberCardId);
    query.bindValue(":payment_type", paymentType);
    query.bindValue(":coupon_id", couponId);
    query.bindValue(":promotion_id", promotionId);
    query.bindValue(":card_number", cardNumber);
    query.bindValue(":points_used_from", pointsUsedFrom);
    query.bindValue(":points_used_to", pointsUsedTo);
    query.bindValue(":number_of_tickets", numberTickets);
    query.bindValue(":total_from", totalFrom);
    query.bindValue(":total_to", totalTo);
    query.bindValue(":employee_id", employeeId);
    query.bindValue(":transaction_time_from", transactionTimeFrom);
    query.bindValue(":transaction_time_to", transactionTimeTo);

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load showtimes!\nError: " + query.lastError().nativeErrorCode());
        return false;
    }

    createTableModel(&query);

    conn.connClose();
    return true;

}

// Create model with box office transactions data
void BoxOfficeTransactionsForm::createTableModel(QSqlQuery* query)
{
    // Detelete model pointer
    delete ui->tvwTable->model();
    QStandardItemModel* model= new QStandardItemModel(query->size(), query->record().count());

    // We get headers from query
    QStringList tableHeaders;
    for(int i = 0; i < query->record().count(); i++)
        tableHeaders.append(query->record().fieldName(i));

    model->setHorizontalHeaderLabels(tableHeaders);

    int row =0;
    // Add row to table
    while(query->next())
    {
        QList<QStandardItem*> items;
        for(int i = 0; i < query->record().count(); i++)
            items.append(new QStandardItem());

        items[0]->setData(query->value (0).toInt(), Qt::DisplayRole);
        items[1]->setData(query->value (1).toInt(), Qt::DisplayRole);
        items[2]->setData(query->value (2).toString(), Qt::DisplayRole);
        items[3]->setData(query->value (3).toString(), Qt::DisplayRole);

        items[4]->setData(query->value (4).toString(), Qt::DisplayRole);
        items[5]->setData(query->value (5).toInt(), Qt::DisplayRole);
        items[6]->setData(query->value (6).toString(), Qt::DisplayRole);
        items[7]->setData(query->value (7).toInt(), Qt::DisplayRole);

        items[8]->setData(query->value (8).toInt(), Qt::DisplayRole);
        items[9]->setData(query->value (9).toString(), Qt::DisplayRole);
        items[10]->setData(query->value (10).toInt(), Qt::DisplayRole);
        items[11]->setData(query->value (11).toInt(), Qt::DisplayRole);

        items[12]->setData(query->value (12).toDouble(), Qt::DisplayRole);
        items[13]->setData(query->value (13).toInt(), Qt::DisplayRole);
        items[14]->setData(query->value (14).toString(), Qt::DisplayRole);
        items[15]->setData(query->value (15).toString(), Qt::DisplayRole);

        items[16]->setData(query->value (16).toString(), Qt::DisplayRole);
        items[17]->setData(query->value (17).toDateTime(), Qt::DisplayRole);
        items[18]->setData(query->value (18).toInt(), Qt::DisplayRole);

        for(int column = 0; column < query->record().count(); column++)
            model->setItem(row, column, items[column]);

        row++;
    }

    ui->tvwTable->setModel(model);

    //Filter model to filter data
    QSortFilterProxyModel *m=new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);
    ui->tvwTable->setModel(m);
    ui->tvwTable->setSortingEnabled(true);
    ui->tvwTable->sortByColumn(0, Qt::AscendingOrder);
    // Hide pt.payment_type_id
    ui->tvwTable->hideColumn(18);
}

// Reset fields to default value
void BoxOfficeTransactionsForm::resetFields()
{
    ui->spbMemberCardId->setValue(0);
    ui->cbxPaymentType->setCurrentIndex(0);
    ui->spbCouponId->setValue(0);
    ui->spbPromotionId->setValue(0);
    ui->ledCardNumber->setText("");
    // delete mask to see placeholder
    ui->ledCardNumber->setInputMask("");
    ui->spbPointsUsedFrom->setValue(0);
    ui->spbPointsUsedTo->setValue(0);
    ui->spbNumberTickets->setValue(0);
    ui->dspTotalFrom->setValue(0);
    ui->dspTotalTo->setValue(0);
    ui->cbxEmployee->setCurrentIndex(0);
    ui->dteTransactionTimeFrom->setDateTime(QDateTime(QDate(2000, 1, 1), QTime(12, 0, 0, 0)));
    ui->dteTransactionTimeTo->setDateTime(QDateTime::currentDateTime());
    ui->chkTransactionTimeFrom->setChecked(Qt::Unchecked);
    ui->chkTransactionTimeTo->setChecked(Qt::Unchecked);
}

void BoxOfficeTransactionsForm::on_pbtLoadTable_clicked()
{
    // If table was loaded, enable delete button
    if(loadTable())
        ui->pbtDelete->setEnabled(true);
    else
        ui->pbtDelete->setEnabled(false);
}

void BoxOfficeTransactionsForm::on_pbtClearFields_clicked()
{
    resetFields();
}

void BoxOfficeTransactionsForm::on_pbtDelete_clicked()
{
    // Ask if user wants to procced with deletion or records. If not, return.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of box office transaction(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

   // Get rows selected by user
   QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();

   if(select.count() == 0)
   {
       QMessageBox::warning(this, "Error", "No rows selected!");
       return;
   }

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete box office transaction(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete box office transaction(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   QSqlQuery query;

   // Delete all selected rows
   for(int i=0; i< select.count(); i++)
   {
       QModelIndex index = select.at(i);
       int row = index.row();

       query.prepare("DELETE FROM box_office_transaction "
                     "WHERE box_office_transaction_id = :box_office_transaction_id;");
       query.bindValue(":box_office_transaction_id", index.sibling(row,0).data().toInt());

       if(!query.exec())
       {
           conn.connClose();
           QMessageBox::warning(this, "Error", "Couldn't delete box office transaction(s)!\nError: " + query.lastError().nativeErrorCode());
           return;
       }

   }

   if(!conn.commit())
   {
       conn.connClose();
       QMessageBox::warning(this, "Error", "Couldn't delete box office transaction(s)!\nError: " + query.lastError().nativeErrorCode());
       return;
   }

   conn.connClose();
   QMessageBox::information(this, "Success", "Box office transaction(s) deleted!");
   loadTable();
}

void BoxOfficeTransactionsForm::on_chkTransactionTimeFrom_stateChanged(int checked)
{
    ui->dteTransactionTimeFrom->setEnabled(checked);
}

void BoxOfficeTransactionsForm::on_chkTransactionTimeTo_stateChanged(int checked)
{
    ui->dteTransactionTimeTo->setEnabled(checked);
}

// When MyLineEdit is focussed, set mask.
void BoxOfficeTransactionsForm::on_ledCardNumber_focussed(bool hasFocus)
{
    if(hasFocus)
        ui->ledCardNumber->setInputMask("9999 9999 9999 9999");
}

