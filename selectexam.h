#pragma once

#include <QDialog>
#include <QListWidgetItem>

#include <memory>

#include "MySQLSimpleDriver.hpp"


using Query = std::shared_ptr<querryResult>;

namespace Ui {
class SelectExam;
}

class SelectExam : public QDialog
{
	Q_OBJECT

public:

	size_t IDresult = 0;

    mysqldriver * db;

    explicit SelectExam(mysqldriver * _db, QWidget *parent = nullptr);

	~SelectExam();

private slots:

	void on_subjects_currentIndexChanged(const QString &arg1);

	void on_ok_clicked();

	void on_cancel_clicked();

	void on_results_itemDoubleClicked(QListWidgetItem *item);

private:

	Ui::SelectExam *ui;

	void __ready();
};
