QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    boxemployeeform.cpp \
    boxofficetransaction.cpp \
    boxofficetransactionaddform.cpp \
    boxofficetransactionsform.cpp \
    castmember.cpp \
    castmembersaddmodifyform.cpp \
    castmembersform.cpp \
    castposition.cpp \
    castpositionsaddmodifyform.cpp \
    castpositionsform.cpp \
    country.cpp \
    coupon.cpp \
    couponaddmodifyform.cpp \
    couponsform.cpp \
    customer.cpp \
    customersaddmodifyform.cpp \
    customersform.cpp \
    customertype.cpp \
    customertypeform.cpp \
    customertypesaddmodifyform.cpp \
    customlineedit.cpp \
    dragwidget.cpp \
    employee.cpp \
    employeeaddmodifyform.cpp \
    employeeposition.cpp \
    employeepositionsaddmodifyform.cpp \
    employeesform.cpp \
    employeetypesform.cpp \
    generalmanagerform.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    membercard.cpp \
    movie.cpp \
    moviecastaddmodifyform.cpp \
    moviecastform.cpp \
    moviegenre.cpp \
    moviegenresaddmodifyform.cpp \
    moviegenresform.cpp \
    moviesaddmodifyform.cpp \
    moviesform.cpp \
    mylineedit.cpp \
    paymenttype.cpp \
    paymenttypeaddmodifyform.cpp \
    paymenttypesform.cpp \
    promotion.cpp \
    promotionaddmodifyform.cpp \
    promotionsform.cpp \
    promotiontype.cpp \
    promotiontypeaddmodifyform.cpp \
    promotiontypeform.cpp \
    qlistwidgetitemidx.cpp \
    seat.cpp \
    seatselectionform.cpp \
    seatwidget.cpp \
    showtime.cpp \
    showtimeaddmodifyform.cpp \
    showtimesform.cpp \
    theater.cpp \
    theateraddmodifyform.cpp \
    theatersform.cpp \
    theatertype.cpp \
    theatertypeaddmodifyform.cpp \
    theatertypesform.cpp \
    theaterwidget.cpp \
    ticket.cpp

HEADERS += \
    SmartPtr.h \
    boxemployeeform.h \
    boxofficetransaction.h \
    boxofficetransactionaddform.h \
    boxofficetransactionsform.h \
    castmember.h \
    castmembersaddmodifyform.h \
    castmembersform.h \
    castposition.h \
    castpositionsaddmodifyform.h \
    castpositionsform.h \
    country.h \
    coupon.h \
    couponaddmodifyform.h \
    couponsform.h \
    customer.h \
    customersaddmodifyform.h \
    customersform.h \
    customertype.h \
    customertypeform.h \
    customertypesaddmodifyform.h \
    customlineedit.h \
    dragwidget.h \
    employee.h \
    employeeaddmodifyform.h \
    employeeposition.h \
    employeepositionsaddmodifyform.h \
    employeesform.h \
    employeetypesform.h \
    generalmanagerform.h \
    login.h \
    mainwindow.h \
    membercard.h \
    movie.h \
    moviecastaddmodifyform.h \
    moviecastform.h \
    moviegenre.h \
    moviegenresaddmodifyform.h \
    moviegenresform.h \
    moviesaddmodifyform.h \
    moviesform.h \
    mylineedit.h \
    paymenttype.h \
    paymenttypeaddmodifyform.h \
    paymenttypesform.h \
    promotion.h \
    promotionaddmodifyform.h \
    promotionsform.h \
    promotiontype.h \
    promotiontypeaddmodifyform.h \
    promotiontypeform.h \
    qlistwidgetitemidx.h \
    seat.h \
    seatselectionform.h \
    seatwidget.h \
    showtime.h \
    showtimeaddmodifyform.h \
    showtimesform.h \
    theater.h \
    theateraddmodifyform.h \
    theatersform.h \
    theatertype.h \
    theatertypeaddmodifyform.h \
    theatertypesform.h \
    theaterwidget.h \
    ticket.h

FORMS += \
    boxemployeeform.ui \
    boxofficetransactionaddform.ui \
    boxofficetransactionsform.ui \
    castmembersaddmodifyform.ui \
    castmembersform.ui \
    castpositionsaddmodifyform.ui \
    castpositionsform.ui \
    couponaddmodifyform.ui \
    couponsform.ui \
    customersaddmodifyform.ui \
    customersform.ui \
    customertypeform.ui \
    customertypesaddmodifyform.ui \
    employeeaddmodifyform.ui \
    employeepositionsaddmodifyform.ui \
    employeesform.ui \
    employeetypesform.ui \
    generalmanagerform.ui \
    mainwindow.ui \
    moviecastaddmodifyform.ui \
    moviecastform.ui \
    moviegenresaddmodifyform.ui \
    moviegenresform.ui \
    moviesaddmodifyform.ui \
    moviesform.ui \
    paymenttypeaddmodifyform.ui \
    paymenttypesform.ui \
    promotionaddmodifyform.ui \
    promotionsform.ui \
    promotiontypeaddmodifyform.ui \
    promotiontypeform.ui \
    seatselectionform.ui \
    showtimeaddmodifyform.ui \
    showtimesform.ui \
    theateraddmodifyform.ui \
    theatersform.ui \
    theatertypeaddmodifyform.ui \
    theatertypesform.ui

QMAKE_CXXFLAGS += -std=gnu++14

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32: LIBS += -L$$PWD/./ -llibmysql

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

RESOURCES += \
    MyResources.qrc

DISTFILES += \
    available handicap seat.png \
    available seat.png \
    occupied handicap seat.png \
    occupied seat.png \
    selected handicap seat.png \
    selected seat.png

