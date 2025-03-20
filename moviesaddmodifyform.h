/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows to add new or update existing movie.

*/

#ifndef MOVIESADDMODIFYFORM_H
#define MOVIESADDMODIFYFORM_H

#include <QDialog>

#include <QListWidgetItem>
#include <QFileDialog>
#include <qmessagebox.h>
#include <qlist.h>

#include "login.h"
#include "movie.h"
#include "moviegenre.h"

namespace Ui {
class MoviesAddModifyForm;
}

class MoviesAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    // Constructor for adding new movie.
    explicit MoviesAddModifyForm(QWidget *parent, QList<int> countryIDList, QStringList countryList,
                                 QList<int> genreIDList, QStringList genreList);
    // Constructor for updating existing movie.
    explicit MoviesAddModifyForm(QWidget *parent, QList<int> countryIDList, QStringList countryList, QList<int> genreIDList,
                                 QStringList genreList, Movie oldMovie);
    ~MoviesAddModifyForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Initialize some features of Dialog.
    // @param QList<int> countryIDList. List with Ids of countries.
    // @param QStringList countryList.
    // @param QList<int> genreIDList. List with Ids of genres.
    void initializeForm(QList<int> countryIDList, QStringList countryList, QList<int> genreIDList);

    // Checks if required fields were filled out.
    // @param QList<MovieGenre> genreList.
    // @return bool. True if all required fields were filled out
    bool areFieldsNotBlank(QList<MovieGenre> genreList);

    // Insert new Movie into DB.
    // @param Movie newMovie. Movie that's gonna be inserted.
    // @return bool. True if movie was successfully inserted.
    bool addMovie(Movie newMovie);
    // Update existing movie in the DB.
    // @param Movie newMovie. Movie with updated data.
    // @return bool. True if movie was successfully updated.
    bool updateMovie(Movie newMovie);

private slots:

    // Open QFileDialog to select Trailer file.
    void on_pbtTrailer_clicked();
    // Open QFileDialog to select Poster file.
    void on_pbtPoster_clicked();

    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();

    // Executes when user checks / unchecks item from "lwdGenres". Highlights checked items.
    // @param QListWidgetItem *item. Item that was checked / unchecked.
    void highlightChecked(QListWidgetItem* item);

signals:

    // Is emitted to say if push button "pbtOk" was clicked.
    // @param bool clicked. True if pbtOk was clicked. False if it wasn't
    void is_pbtOK_clicked(bool clicked);

private:
    Ui::MoviesAddModifyForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Says if query type is Insert or Update.
    QString queryType;

    // List with Ids of countries
    QList<int> countryIDList;
    // List with Ids of genres
    QList<int> genreIDList;

    // Original data of movie
    Movie oldMovie;

    // Original genre index list that movie had. The indexes are from QListWidget "lwdGenres".
    QList<int> oldCheckedGenreIdxList;
    // Updated genre index list that movie will have. The indexes are from QListWidget "lwdGenres".
    QList<int> newCheckedGenreIdxList;
};

#endif // MOVIESADDMODIFYFORM_H
