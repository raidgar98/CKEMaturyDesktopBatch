#include "advancedsearch.h"
#include "ui_advancedsearch.h"
#include "mainwindow.h"

#include <QMessageBox>
#include <QDebug>

#define cout qDebug()

advancedSearch::advancedSearch(mysqldriver * _db, const std::list<size_t>& src, const QString& whereSQLstatement, bool ok1, QDialog *parent) :
	curr{src},
	QDialog(parent),
	ui(new Ui::advancedSearch),
	finalQuery{whereSQLstatement},
	db{_db},
	okok(ok1)
{
	ui->setupUi(this);

	ui->regionSearch->addItem("");

	for(auto var : MainWindow::sRegions)
		ui->regionSearch->addItem(var.second);


	if(ok1) ui->searchButton->setEnabled(false);
}

advancedSearch::~advancedSearch()
{
	db = nullptr;
	delete ui;
}

void advancedSearch::_update()
{
	lastQuer = " idpracownicy LIKE '%"+ui->idSearch->text() + "%' " +
			"AND imie LIKE '%"+ui->nameSearch->text() + "%' " +
			"AND nazwisko LIKE '%"+ui->surnameSearch->text() + "%' " +
			"AND idOkregu LIKE '%"+QString::number(MainWindow::regionStr2Id(ui->regionSearch->currentText())) +
			"%' AND uprawnienia%"+ui->permissionSearch->text()+"=0 ";

	for(auto var : curr) lastQuer+="AND idpracownicy!="+QString::number(var)+" ";

	lastQuer+=";";

	QString quer = "SELECT COUNT(*) FROM "+
                   QString(db->DataBase().c_str()) + ".pracownicy WHERE " + lastQuer;

	db->open();
    Query q{db->exec(quer)};
	db->close();

    ui->resultCount->setValue(q->data().front().front().toInt());
}

void advancedSearch::on_permissionSearch_textChanged(const QString &arg1)
{
	_update();
}

void advancedSearch::on_idSearch_textChanged(const QString &arg1)
{
	_update();
}

void advancedSearch::on_nameSearch_textChanged(const QString &arg1)
{
	_update();
}

void advancedSearch::on_surnameSearch_textChanged(const QString &arg1)
{
	_update();
}

void advancedSearch::on_regionSearch_currentTextChanged(const QString &arg1)
{
	_update();
}

void advancedSearch::on_searchButton_clicked()
{
	db->open();
    Query prepare{db->exec(finalQuery+lastQuer)};
	db->close();

    res = finalQuery+lastQuer;

	everythingGoesGoog = true;

	this->close();
}

void advancedSearch::on_resultCount_valueChanged(int arg1)
{
	if(!okok) return;
	if(arg1 == 1) ui->searchButton->setEnabled(true); else ui->searchButton->setEnabled(false);
}
