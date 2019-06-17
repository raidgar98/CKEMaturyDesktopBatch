#pragma once
#include <QDialog>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSql>
#include <QtSql/QSqlResult>

#include <memory>
#include "MySQLSimpleDriver.hpp"

using Query = std::shared_ptr<querryResult>;

class Team;

namespace Ui {
class advancedSearch;
}

class advancedSearch : public QDialog
{
	Q_OBJECT

public:

    explicit advancedSearch(mysqldriver * _db, const std::list<size_t>& src, const QString& whereSQLstatement, bool ok1 = false, QDialog *parent = nullptr);

	~advancedSearch();

	void _update();

    mysqldriver * db;

	QString lastQuer = "";

	QString res;

	bool okok = false;

	bool everythingGoesGoog = false;

	const std::list<size_t>& curr;

	QString finalQuery = "";

private slots:

	void on_permissionSearch_textChanged(const QString &arg1);

	void on_idSearch_textChanged(const QString &arg1);

	void on_nameSearch_textChanged(const QString &arg1);

	void on_surnameSearch_textChanged(const QString &arg1);

	void on_regionSearch_currentTextChanged(const QString &arg1);

	void on_searchButton_clicked();

	void on_resultCount_valueChanged(int arg1);

private:

	Ui::advancedSearch *ui;
};
