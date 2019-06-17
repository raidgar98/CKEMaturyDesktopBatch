#include "permissions.h"
#include "ui_permissions.h"
#include "mainwindow.h"

Permissions::Permissions(const num_type src, const bool _student, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Permissions),
	input{src},
	student{_student},
	result{src}
{
	ui->setupUi(this);

	std::queue<QString> que;

	std::map<num_type, QString> defs(MainWindow::sPrivillages);

	MainWindow::resolve(que, src);

	std::list<QString> lis;

	while(!que.empty())
	{
		lis.push_back(que.front());
		que.pop();
	}

	for(auto var : lis)
	{
		for(auto tar : defs)
		{
			if(var == tar.second)
				defs.erase(tar.first);
		}
		ui->currentList->addItem(new QListWidgetItem(var));
	}

	for(auto var : defs)
		if(!student || var.first < 1000) ui->avaiableList->addItem(new QListWidgetItem(var.second));

	ui->currentCode->setText(QString::number(src));
	ui->newCode->setText(QString::number(src));

}

Permissions::~Permissions()
{
	delete ui;
}

//Add
void Permissions::on_avaiableList_itemDoubleClicked(QListWidgetItem *item)
{
	__add(item);
}

//Add
void Permissions::on_addButt_clicked()
{
	__add(ui->avaiableList->currentItem());
}

//Erease
void Permissions::on_currentList_itemDoubleClicked(QListWidgetItem *item)
{
	__remove(item);
}

//Erease
void Permissions::on_remButt_clicked()
{
	__remove(ui->currentList->currentItem());
}

//Ok
void Permissions::on_positiveFinishedButton_clicked()
{
	this->close();
}

//Cancel
void Permissions::on_negativeFinishButton_clicked()
{
	result = input;
	this->close();
}

void Permissions::__add(QListWidgetItem * item)
{
	QListWidgetItem * temp = new QListWidgetItem(*item);
	ui->currentList->addItem(temp);
	delete item;

	for(auto var : MainWindow::sPrivillages)
		if(var.second == temp->text()) { result*=var.first; break; }

	ui->newCode->setText(QString::number(result));
	temp = nullptr;
}

void Permissions::__remove(QListWidgetItem * item)
{
	QListWidgetItem * temp = new QListWidgetItem(*item);
	ui->avaiableList->addItem(temp);
	delete item;

	for(auto var : MainWindow::sPrivillages)
		if(var.second == temp->text()) { result /= var.first; break; }

	ui->newCode->setText(QString::number(result));
	temp = nullptr;
}
