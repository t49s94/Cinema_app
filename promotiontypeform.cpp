/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays promotion types stored in DB. Allows look up, add new, delete or update existing promotion type.

*/

#include "promotiontypeform.h"
#include "ui_promotiontypeform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

PromotionTypeForm::PromotionTypeForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PromotionTypeForm)
{
    ui->setupUi(this);
    // Prevents user from resizing Window
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;

    ui->ledName->setPlaceholderText("Any");
    ui->tedDescription->setPlaceholderText("Any");

    populateComboBoxes();

    QStringList days =
    {
        "Any", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday", "Sunday"
    };
    ui->lwdDays->addItems(days);

    // Add items to lwdDays and make them checakble
    QListWidgetItem* item = 0;
    for(int i = 0; i < ui->lwdDays->count(); ++i){
        item = ui->lwdDays->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }

    // Executes highlightChecked slot when item in lwdDays changes state.
    QObject::connect(ui->lwdDays, SIGNAL(itemChanged(QListWidgetItem*)),
                         this, SLOT(highlightChecked(QListWidgetItem*)));

    ui->lwdDays->item(0)->setCheckState(Qt::Checked);
    ui->lwdDays->item(0)->setBackgroundColor(QColor("#ffffb2"));

}

PromotionTypeForm::~PromotionTypeForm()
{
    delete ui->tvwTable->model();
    if(model != 0)
        delete model;

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Populate combo boxes: cbxAgeCategory, cbxCustomerType, cbxTheaterType, cbxPaymentType.
void PromotionTypeForm::populateComboBoxes()
{
    QStringList ageCategoryList = { "Any", "Adult", "Kid", "Senior" };
    ui->cbxAgeCategory->addItems(ageCategoryList);

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load load data!\n Error: " + conn.nativeErrorCode());
        close();
    }

    QSqlQuery query;

    query.prepare("SELECT * "
                  "FROM customer_type;");

    customerTypesList.append({"1", "Any"});
    ui->cbxCustomerType->addItem("Any");

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't load customer types!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    while(query.next())
    {
        customerTypesList.append({query.value(0).toString(), query.value(1).toString()});
        ui->cbxCustomerType->addItem(query.value(1).toString());
    }

    query.prepare("SELECT * "
                  "FROM theater_type;");

    theaterTypesList.append({"1", "Any"});
    ui->cbxTheaterType->addItem("Any");

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't load theater types!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    while(query.next())
    {
        theaterTypesList.append({query.value(0).toString(), query.value(1).toString()});
        ui->cbxTheaterType->addItem(query.value(1).toString());
    }

    query.prepare("SELECT * "
                  "FROM payment_type;");

    paymentTypesList.append({"1", "Any"});
    ui->cbxPaymentType->addItem("Any");

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't load payment types!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    while(query.next())
    {
        paymentTypesList.append({query.value(0).toString(), query.value(1).toString()});
        ui->cbxPaymentType->addItem(query.value(1).toString());
    }

    conn.connClose();
}

// Load table with information about promotion types.
// @return bool. True if table was successfully loaded.
bool PromotionTypeForm::loadTable()
{
    delete ui->tvwTable->model();
    delete model;
    model = new QSqlQueryModel();

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load promotion types!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;
    QString queryText;

    queryText = "SELECT pt.promotion_type_id AS ID, pt.name AS Name, pt.discount AS Discount, pt.description AS Description, "
                "pt.age_category AS 'Age category', ct.name AS 'Customer type', tt.name AS 'Theater type', pm.name AS 'Payment Type', "
                "pt.monday AS Mo, pt.tuesday AS Tu, pt.wednesday AS We, pt.thursday AS Th, "
                "pt.friday AS Fr, pt.saturday AS Sa, pt.sunday AS Su, "
                "ct.customer_type_id, tt.theater_type_id, pm.payment_type_id "
                "FROM promotion_type pt "
                "INNER JOIN customer_type ct "
                "ON pt.customer_type_id = ct.customer_type_id "
                "INNER JOIN theater_type tt "
                "ON pt.theater_type_id = tt.theater_type_id "
                "INNER JOIN payment_type pm "
                "ON pt.payment_type_id = pm.payment_type_id "
                "WHERE 1 = 1 ";

    //----------------------------------------------------------------------------------------------------------------------------
    // Append updated fields to query text.

    QString name = "%" + ui->ledName->text() + "%";
    queryText += name != "%%"? " AND pt.name LIKE :name" : "";

    QString discount = QString::number(ui->spbDiscount->value());
    queryText += discount != "0"? " AND pt.discount = :discount" : "";

    QString description = "%" + ui->tedDescription->toPlainText() + "%";
    queryText += description != "%%"? " AND pt.description LIKE :description" : "";

    QString ageCategory = ui->cbxAgeCategory->currentText();
    queryText += ageCategory != "Any"? " AND pt.age_category = :age_category" : "";

    QString costumerType = ui->cbxCustomerType->currentText();
    queryText += costumerType != "Any"? " AND ct.name = :customer_type" : "";

    QString theaterType = ui->cbxTheaterType->currentText();
    queryText += theaterType != "Any"? " AND tt.name = :theater_type" : "";

    QString paymentType = ui->cbxPaymentType->currentText();
    queryText += paymentType != "Any"? " AND pm.name = :payment_type" : "";

    QVector<bool> daysChecked = QVector<bool>(7);
    daysChecked[0] = ui->lwdDays->item(1)->checkState();
    daysChecked[1] = ui->lwdDays->item(2)->checkState();
    daysChecked[2] = ui->lwdDays->item(3)->checkState();
    daysChecked[3] = ui->lwdDays->item(4)->checkState();
    daysChecked[4] = ui->lwdDays->item(5)->checkState();
    daysChecked[5] = ui->lwdDays->item(6)->checkState();
    daysChecked[6] = ui->lwdDays->item(7)->checkState();

    if(!ui->lwdDays->item(0)->checkState())
    {
        queryText += " AND pt.monday = :monday";
        queryText += " AND pt.tuesday = :tuesday";
        queryText += " AND pt.wednesday = :wednesday";
        queryText += " AND pt.thursday = :thursday";
        queryText += " AND pt.friday = :friday";
        queryText += " AND pt.saturday = :saturday";
        queryText += " AND pt.sunday = :sunday";

    }

    query.prepare( queryText + " ORDER BY pt.promotion_type_id;");

    query.bindValue(":name", name);
    query.bindValue(":discount", discount);
    query.bindValue(":description", description);
    query.bindValue(":age_category", ageCategory);
    query.bindValue(":customer_type", costumerType);
    query.bindValue(":theater_type", theaterType);
    query.bindValue(":payment_type", paymentType);
    query.bindValue(":monday", daysChecked[0]);
    query.bindValue(":tuesday", daysChecked[1]);
    query.bindValue(":wednesday", daysChecked[2]);
    query.bindValue(":thursday", daysChecked[3]);
    query.bindValue(":friday", daysChecked[4]);
    query.bindValue(":saturday", daysChecked[5]);
    query.bindValue(":sunday", daysChecked[6]);


    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't promotion types!\nError: " + query.lastError().nativeErrorCode());
        return false;
    }

    model->setQuery(query);

    // Model that filters data in table
    QSortFilterProxyModel *m=new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);

    ui->tvwTable->setModel(m);
    ui->tvwTable->setSortingEnabled(true);
    ui->tvwTable->sortByColumn(0, Qt::AscendingOrder);
    // Hides ct.customer_type_id, tt.theater_type_id, pm.payment_type_id
    ui->tvwTable->hideColumn(15);
    ui->tvwTable->hideColumn(16);
    ui->tvwTable->hideColumn(17);

    conn.connClose();
    return true;

}

// Resets fields to default value in form
void PromotionTypeForm::resetFields()
{
    ui->ledName->setText("");
    ui->spbDiscount->setValue(0);
    ui->tedDescription->setText("");
    ui->cbxAgeCategory->setCurrentIndex(0);
    ui->cbxTheaterType->setCurrentIndex(0);
    ui->cbxPaymentType->setCurrentIndex(0);
    ui->lwdDays->item(0)->setCheckState(Qt::Checked);
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void PromotionTypeForm::on_pbtLoadTable_clicked()
{
    if(loadTable())
    {
        ui->pbtAddNew->setEnabled(true);
        ui->pbtDelete->setEnabled(true);
        ui->pbtModify->setEnabled(true);
    }
    else
    {
        ui->pbtAddNew->setEnabled(false);
        ui->pbtDelete->setEnabled(false);
        ui->pbtModify->setEnabled(false);
    }
}

void PromotionTypeForm::on_pbtClearFields_clicked()
{
    resetFields();
}

void PromotionTypeForm::on_pbtAddNew_clicked()
{
    PromotionTypeAddModifyForm *addForm = new PromotionTypeAddModifyForm(this, customerTypesList, theaterTypesList, paymentTypesList);

    connect (addForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    addForm->exec();
}

// Deletes selected promotion types from table
void PromotionTypeForm::on_pbtDelete_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of promotion type(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get selected rows from table
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // List that contains Ids of promotion types to be deleted
    QVector<int> ids(select.count());

   if(select.count() == 0)
   {
       QMessageBox::warning(this, "Error", "No rows selected!");
       return;
   }

   // Populate QVector<int> ids
   for(int i=0; i< select.count(); i++)
   {
       QModelIndex index = select.at(i);
       int row = index.row();
       ids[i] = index.sibling(row,0).data().toInt();
   }

   // Says if there was an error during deletion of promotion types
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete promotion type(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete promotion type(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Deletes promotion types
   for(int i=0; i< select.count(); i++)
   {
       QSqlQuery query;

       query.prepare("DELETE FROM promotion_type "
                     "WHERE promotion_type_id = :promotion_type_id;");
       query.bindValue(":promotion_type_id", ids[i]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete promotion type(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }

   }

   if(!error)
   {
       conn.commit();
       conn.connClose();
       QMessageBox::information(this, "Success", "Promotion type(s) deleted!");
       loadTable();
   }
}

// Update selected promotion types from table
void PromotionTypeForm::on_pbtModify_clicked()
{
    if(ui->tvwTable->selectionModel()->selectedRows().count() == 0)
    {
        QMessageBox::warning(this, "Error", "No row was selected!");
        return;
    }
    else if(ui->tvwTable->selectionModel()->selectedRows().count() > 1)
    {
        QMessageBox::warning(this, "Error", "You can only modify a row at the time!");
        return;
    }

    QModelIndex index=ui->tvwTable->selectionModel()->currentIndex();

    PromotionType pt;
    pt.setId(index.sibling(index.row(),0).data().toInt());
    pt.setName(index.sibling(index.row(),1).data().toString());
    pt.setDiscount(index.sibling(index.row(),2).data().toInt());
    pt.setDescription(index.sibling(index.row(),3).data().toString());
    pt.setAgeCategory(index.sibling(index.row(),4).data().toString());
    pt.setCustomerType(CustomerType(index.sibling(index.row(),15).data().toInt(),
                                    index.sibling(index.row(),5).data().toString(), 0, 0));
    pt.setTheaterType(TheaterType(index.sibling(index.row(),16).data().toInt(),
                                   index.sibling(index.row(),6).data().toString(), 0, 0));
    pt.setPaymentType(PaymentType(index.sibling(index.row(),17).data().toInt(),
                                   index.sibling(index.row(),7).data().toString()));
    pt.setMonday(index.sibling(index.row(),8).data().toBool());
    pt.setTuesday(index.sibling(index.row(),9).data().toBool());
    pt.setWednesday(index.sibling(index.row(),10).data().toBool());
    pt.setThursday(index.sibling(index.row(),11).data().toBool());
    pt.setFriday(index.sibling(index.row(),12).data().toBool());
    pt.setSaturday(index.sibling(index.row(),13).data().toBool());
    pt.setSunday(index.sibling(index.row(),14).data().toBool());

    PromotionTypeAddModifyForm *modifyForm = new PromotionTypeAddModifyForm(this, customerTypesList, theaterTypesList, paymentTypesList, pt);

    connect (modifyForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    modifyForm->exec();

}

// Executes when item in lwdDays is checked / unchecked. Highlights checked items.
// @param QListWidgetItem *item. Item that was checked / unchecked.
void PromotionTypeForm::highlightChecked(QListWidgetItem *item)
{
    // Temporary disconnects because since we change state of item inside this slot, signal itemChanged will be emitted and slot will be
    // called again and again creating an infinite loop.
    QObject::disconnect(ui->lwdDays, SIGNAL(itemChanged(QListWidgetItem*)),
                        this, SLOT(highlightChecked(QListWidgetItem*)));

    // When item "Any" is checked, all the other items will be unchecked leaving item "Any" as the only one checked.
    if(item->text() == "Any")
    {

        item->setBackgroundColor(QColor("#ffffb2"));

        for(int i = 1; i < ui->lwdDays->count(); i++)
        {
            ui->lwdDays->item(i)->setBackgroundColor(QColor("#ffffff"));
            ui->lwdDays->item(i)->setCheckState(Qt::Unchecked);
        }

    }
    // Make sure that item "Any" is unchecked and check the item from parameter.
    else
    {
        ui->lwdDays->item(0)->setBackgroundColor(QColor("#ffffff"));
        ui->lwdDays->item(0)->setCheckState(Qt::Unchecked);

        if(item->checkState() == Qt::Checked)
            item->setBackgroundColor(QColor("#ffffb2"));
        else
            item->setBackgroundColor(QColor("#ffffff"));

    }

    // Reconnect signal - slot.
    QObject::connect(ui->lwdDays, SIGNAL(itemChanged(QListWidgetItem*)),
                         this, SLOT(highlightChecked(QListWidgetItem*)));
}

// Executed when signal is_pbtOK_clicked is emitted. Says if push button "pbtOk" from PromotionTypeAddModifyForm was clicked.
// @param bool clicked. True if pbtOk was clicked.
void PromotionTypeForm::on_is_pbtOK_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}
