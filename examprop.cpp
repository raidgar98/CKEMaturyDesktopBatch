#include "examprop.h"
#include "ui_examprop.h"
#include "mainwindow.h"

#include <QDebug>

const QString ExamProp::pracQuer = "SELECT idPracownicy, imie, nazwisko, idOkregu FROM pracownicy JOIN zespoly ON idpracownika=idpracownicy WHERE id_egzamin=";
const QString ExamProp::regioQuert = "SELECT DISTINCT idOkregu FROM zespoly JOIN pracownicy ON idpracownika=idpracownicy WHERE id_egzamin=";

ExamProp::ExamProp(size_t id, Query prac, Query regio, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ExamProp)
{
	ui->setupUi(this);
	ui->ExID->setText(QString::number(id));

    for(const auto& var : prac->data() ) ui->ExPrac->addItem(var[0]+": "+var[1]+" "+var[2]+" z "+MainWindow::sRegions[var[3].toUInt()]);

    for(const auto& var : regio->data() )
        ui->ExRegions->addItem(MainWindow::sRegions[var[0].toUInt()]);

	ui->ExPracCount->setText(QString::number(ui->ExPrac->count()));
	ui->ExRegionsCount->setText(QString::number(ui->ExRegions->count()));

}

ExamProp::~ExamProp()
{
	delete ui;
}

void ExamProp::on_pushButton_clicked()
{
	this->close();
}
