#include "advancedpupilsearch.h"
#include "ui_advancedpupilsearch.h"
#include "mainwindow.h"

advancedPupilSearch::advancedPupilSearch(mysqldriver * _db, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::advancedPupilSearch),
	db{_db}
{
	ui->setupUi(this);

	for(auto var : MainWindow::sRegions) ui->okreg->addItem(var.second);

}

advancedPupilSearch::~advancedPupilSearch()
{
	delete ui;
}

void advancedPupilSearch::on_pesel_textChanged(const QString &arg1)
{
	__refresh();
}

void advancedPupilSearch::on_imie_textChanged(const QString &arg1)
{
	__refresh();
}

void advancedPupilSearch::on_nazwisko_textChanged(const QString &arg1)
{
	__refresh();
}

void advancedPupilSearch::on_okreg_currentTextChanged(const QString &arg1)
{
	__refresh();
}

void advancedPupilSearch::on_przedm_textChanged(const QString &arg1)
{
	__refresh();
}

void advancedPupilSearch::__refresh()
{
	QString querry = "SELECT COUNT(*) FROM uczniowie ";

	Result = " WHERE pesel LIKE '%"+ui->pesel->text()+"%' "
			+ "AND imie LIKE '%"+ui->imie->text()+"%' AND nazwisko LIKE '%"+ui->nazwisko->text()+"%' "
			+ "AND idOkregu=" + QString::number(MainWindow::regionStr2Id(ui->okreg->currentText())) + " AND "
			+ "zdawanePrzedmioty%"+ui->przedm->text()+"=0;";

	db->open();
    Query q{db->exec(querry+ Result)};
	db->close();

    int count = q->data().front().front().toInt();

	ui->results->setText(QString::number(count));

	ui->okButton->setEnabled(count == 1);

}

void advancedPupilSearch::on_okButton_clicked()
{
	ok = true;
	this->close();
}

void advancedPupilSearch::on_cancelButton_clicked()
{
	Result = "";
	this->close();
}

void advancedPupilSearch::on_pushButton_clicked()
{
	Permissions tmp(ui->przedm->text().toULongLong(), true, this);
	tmp.show();
	tmp.topLevelWidget();
	tmp.exec();

	if(tmp.result != ui->przedm->text().toULongLong())
	{
		ui->przedm->setText(QString::number(tmp.result));
	}
}
