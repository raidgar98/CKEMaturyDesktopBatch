#include "team.h"
#include "ui_team.h"
#include "mainwindow.h"
#include <QDebug>
#include "advancedsearch.h"
#include <list>

#define cout qDebug()

QString Team::finalQuerryTemplate = "SELECT idpracownicy, nazwisko, idOkregu, uprawnienia FROM ";

Team::Team(mysqldriver * _db, std::list<size_t>* iList, QString iLastSearch, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Team),
	db{_db},
	__last(iLastSearch)
{
	ui->setupUi(this);
	if(iList != nullptr && !(iList->empty()))
	{
		curr = *iList;
        finalQuerryTemplate+= QString(db->DataBase().c_str()) + ".pracownicy WHERE ";
		QString quer = finalQuerryTemplate + " 0=1 ";
		for(auto var : curr)
		{
			quer += "OR idpracownicy="+QString::number(var)+" ";
		}
		quer+=";";
		cout <<quer;
		db->open();
        Query que{db->exec(quer)};
		db->close();
		fillList(que, ui->currenList);
	}
	ui->fastSearch->setText(__last);
}

Team::~Team()
{
	db = nullptr;
	delete ui;
}

void Team::fillList(Query q, QListWidget * toList) noexcept
{
	if(toList == nullptr) toList = ui->avaiableList;

	toList->clear();

    for(const auto& var : q->data())
	{
        toList->addItem(new QListWidgetItem(var[0]+": " + var[1]+" z " + MainWindow::sRegions[var[2].toUInt()]+". "
                                            +var[3] ));

		std::queue<QString> temp;
        MainWindow::resolve(temp,var[3].toULongLong());
		QString t = "";
		while(!temp.empty())
		{
			t+=temp.front() + "\n";
			temp.pop();
		}
		toList->itemAt(toList->count(), 0)->setToolTip(t);
	}

	ui->pushButton->setEnabled(true);
	ui->pushButton_2->setEnabled(true);
}

void Team::__add(QListWidgetItem * src)
{
	QListWidgetItem * temp = new QListWidgetItem(*src);

	QString tmp = "";
	for(auto var : src->text()) if(var == ':') break; else tmp+=var;
	curr.push_back(tmp.toUInt());

	delete src;
	src = nullptr;
	ui->currenList->addItem(temp);
	temp = nullptr;
}

void Team::__remove(QListWidgetItem * src)
{
	QString tmp = "";
	for(auto var : src->text()) if(var == ':') break; else tmp+=var;
	curr.remove(tmp.toUInt());

	delete src;
	src = nullptr;
	QString temp = ui->fastSearch->text();
	QString _xd = __last;
	ui->fastSearch->setText("1");
	ui->fastSearch->setText(_xd);
}

void Team::on_fastSearch_textChanged(const QString &arg1)
{
	__last = arg1;
	QString tt = "SELECT COUNT(*) FROM pracownicy WHERE idpracownicy LIKE '%"+arg1+"%' ";

	for(auto var : curr) tt+="AND idpracownicy!="+QString::number(var)+" ";

	tt+=";";

	db->open();
    Query q{db->exec(tt)};
	db->close();
    if(q->data().front().front().toInt() > MainWindow::LIMIT_RESULTS)
	{
		ui->avaiableList->clear();
		ui->avaiableList->addItem("Za dużo wynków.");
		return;
	}

    q->clear();

    tt= finalQuerryTemplate + QString(db->DataBase().c_str()) +  ".pracownicy WHERE idpracownicy LIKE '%"+arg1+"%' ";

	for(auto var : curr) tt+="AND idpracownicy!="+QString::number(var)+" ";

	tt+=";";

	db->open();
    q.reset(db->exec(tt));
	db->close();

	fillList(q);
}

void Team::on_advanceSearch_clicked()
{
    advancedSearch tmp(db, curr, Team::finalQuerryTemplate + QString(db->DataBase().c_str()) +  ".pracownicy WHERE ", 0, this);
	tmp.show();
	tmp.topLevelWidget();
	tmp.exec();

	db->open();
    Query q{db->exec(tmp.res)};
	db->close();

	fillList(q);
}

//Add
void Team::on_avaiableList_itemDoubleClicked(QListWidgetItem *item)
{
	__add(item);
}

//Add
void Team::on_pushButton_clicked()
{
	__add(ui->avaiableList->currentItem());
}

//Rem
void Team::on_currenList_itemDoubleClicked(QListWidgetItem *item)
{
	__remove(item);
}

//Rem
void Team::on_pushButton_2_clicked()
{
	__remove(ui->currenList->currentItem());
}

void Team::on_okButton_clicked()
{
	ok = true;
	this->close();
}
