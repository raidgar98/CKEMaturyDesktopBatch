#include "selectexam.h"
#include "ui_selectexam.h"
#include "mainwindow.h"



SelectExam::SelectExam(mysqldriver * _db, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SelectExam),
	db{_db}
{
	ui->setupUi(this);	

	for(auto var : MainWindow::sSubjects) ui->subjects->addItem(var.second);

	ui->subjects->setCurrentIndex(1);
}

SelectExam::~SelectExam()
{
	delete ui;
}

void SelectExam::on_subjects_currentIndexChanged(const QString &arg1)
{
	db->open();
    Query q{db->exec("SELECT idegzamin FROM egzamin JOIN przedmiot ON idprzedmiot=idprzedmiotu WHERE nazwa='"+arg1+"';")};
	db->close();

	ui->results->clear();

    for(const auto& var : q->data()) ui->results->addItem(QString::number(var[0].toUInt()));
}

void SelectExam::on_ok_clicked()
{
	__ready();
}

void SelectExam::on_cancel_clicked()
{
	IDresult = 0;
	this->close();
}

void SelectExam::__ready()
{

	if(ui->results->currentRow() < 0) return;

	IDresult = ui->results->currentItem()->text().toUInt();

	this->close();
}

void SelectExam::on_results_itemDoubleClicked(QListWidgetItem *item)
{
	__ready();
}
