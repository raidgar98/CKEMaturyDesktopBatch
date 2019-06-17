#pragma once

#include <QDialog>
#include <QListWidgetItem>

using num_type = unsigned long long int;

namespace Ui {
class Permissions;
}

class Permissions : public QDialog
{
	Q_OBJECT

public:

	explicit Permissions(const num_type src, const bool _student = false, QWidget *parent = nullptr);
	~Permissions();

	bool student = false;
	num_type result = 0;
	const num_type input;

private slots:

	void on_avaiableList_itemDoubleClicked(QListWidgetItem *item);

	void on_addButt_clicked();

	void on_currentList_itemDoubleClicked(QListWidgetItem *item);

	void on_remButt_clicked();

	void on_positiveFinishedButton_clicked();

	void on_negativeFinishButton_clicked();

private:

	Ui::Permissions *ui;

	void __add(QListWidgetItem * item);

	void __remove(QListWidgetItem * item);
};
