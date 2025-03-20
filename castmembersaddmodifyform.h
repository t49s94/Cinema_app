#ifndef CASTMEMBERSADDMODIFYFORM_H
#define CASTMEMBERSADDMODIFYFORM_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This window adds or update a selected cast member.

*/

#include <QDialog>
#include <QListWidgetItem>
#include <qmessagebox.h>

#include "login.h"
#include "castmember.h"
#include "castposition.h"

namespace Ui {
class CastMembersAddModifyForm;
}

class CastMembersAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //------------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructors

    // For adding a new cast member
    explicit CastMembersAddModifyForm(QWidget *parent, QList<int> countryIDList, QStringList countryList,
                                      QList<int> positionIDList, QStringList positionList);
    // For updating an existing cast member
    explicit CastMembersAddModifyForm(QWidget *parent, QList<int> countryIDList, QStringList countryList,
                                      QList<int> positionIDList, QStringList positionList, CastMember oldMember);
    ~CastMembersAddModifyForm();

    //------------------------------------------------------------------------------------------------------------------------------
    // Functions

    void initializeForm(QList<int>* countryIDList, QStringList* countryList,  QList<int>* positionIDList);

    // Add new cast member
    // @returns bool true if the insert of cast member was a success
    bool addCastMember(QSqlQuery* query, CastMember* newMember);
    // Update an exisiting cast member
    // @returns bool true if the update of cast member was a success
    bool updateCastMember(QSqlQuery* query, CastMember* newMember, QList<int> *newCheckedPositionIdxList);

    // Returns true if fields required fields are not blank
    // @returns bool that says if required fields are blank or not
    bool areFieldsNotBlank();

public slots:
    void highlightChecked(QListWidgetItem* item);

private:
    Ui::CastMembersAddModifyForm *ui;

    //------------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    QString queryType;
    QList<int> positionIDList;
    QList<int> countryIDList;
    CastMember oldMember;
    // List of indexes of cast positions that cast member had originally
    QList<int> oldCheckedPositionIdxList;

signals:
    void is_pbtOK_clicked(bool clicked);

private slots:
    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();
};

#endif // CASTMEMBERSADDMODIFYFORM_H
