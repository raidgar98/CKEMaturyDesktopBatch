#ifndef TEAM_H
#define TEAM_H

#include <QDialog>
#include <QListWidgetItem>

#include <memory>

#include "MySQLSimpleDriver.hpp"

using Query = std::shared_ptr<querryResult>;

namespace Ui {
class Team;
}

class Team : public QDialog
{
	Q_OBJECT


public:
	QString __last = "";

    explicit Team(mysqldriver * _db, std::list<size_t>* iList = nullptr, QString iLastSearch = "", QWidget *parent = nullptr);

	~Team();

	size_t resultSubject = -1;
	QString resultRegion = "";

	std::list<size_t> curr;

	static QString finalQuerryTemplate;

    mysqldriver * db;

    void fillList(Query q, QListWidget * toList = nullptr) noexcept;

	bool ok = false;

private slots:

	void on_fastSearch_textChanged(const QString &arg1);

	void on_advanceSearch_clicked();

	void on_avaiableList_itemDoubleClicked(QListWidgetItem *item);

	void on_pushButton_clicked();

	void on_currenList_itemDoubleClicked(QListWidgetItem *item);

	void on_pushButton_2_clicked();

	void on_okButton_clicked();

private:

	void __add(QListWidgetItem * src);

	void __remove(QListWidgetItem * src);

	Ui::Team *ui;
};

#endif // TEAM_H
