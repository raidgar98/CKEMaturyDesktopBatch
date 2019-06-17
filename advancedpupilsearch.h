#pragma once

#include <QDialog>

#include <memory>

#include "MySQLSimpleDriver.hpp"

using Query = std::shared_ptr<querryResult>;

namespace Ui {
class advancedPupilSearch;
}

class advancedPupilSearch : public QDialog
{
	Q_OBJECT

public:
    explicit advancedPupilSearch(mysqldriver * _db, QWidget *parent = nullptr);
	~advancedPupilSearch();

	QString Result = "";
	bool ok = false;

private slots:

	void on_pesel_textChanged(const QString &arg1);

	void on_imie_textChanged(const QString &arg1);

	void on_nazwisko_textChanged(const QString &arg1);

	void on_okreg_currentTextChanged(const QString &arg1);

	void on_przedm_textChanged(const QString &arg1);

	void on_okButton_clicked();

	void on_cancelButton_clicked();

	void on_pushButton_clicked();

private:

	Ui::advancedPupilSearch *ui;

    mysqldriver * db;

	void __refresh();
};
