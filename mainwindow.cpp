#include "ui_mainwindow.h"

#include <fstream>
#include <string>

#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>

#include "mainwindow.h"
#include "team.h"
#include "examprop.h"
#include "advancedsearch.h"
#include "selectexam.h"
#include "advancedpupilsearch.h"

using Query = std::shared_ptr<querryResult>;

#define cout qDebug()

std::map<num_type, QString> MainWindow::sPrivillages{};

std::map<num_type, QString> MainWindow::sSubjects{};

std::map<num_type, QString> MainWindow::sRegions{};

QString MainWindow::LIMIT_RESULTS =  " LIMIT 50;";

num_type MainWindow::privStr2Id(const QString & src) noexcept
{
    for(auto var : sPrivillages) if(src == var.second) return var.first;
    return 0;
}

num_type MainWindow::subStr2Id(const QString & src) noexcept
{
    for(auto var : sSubjects) if(src == var.second) return var.first;
    return 0;
}

num_type MainWindow::regionStr2Id(const QString & src) noexcept
{
    for(auto var : sRegions) if(src == var.second) return var.first;
    return 0;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showFullScreen();

    __loadEverythink();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resolve(std::queue<QString> & dst, const num_type src) noexcept
{
    for(auto var : sPrivillages)
        if(__divadable(src, var.first)) dst.push(var.second);
}

num_type MainWindow::process(const std::list<num_type> & src) noexcept
{
    size_t toReturn = 1;
    for(auto var : src)
        toReturn *= var;

    return toReturn;
}

void MainWindow::getPath(QLineEdit * dst)
{
    const QString path = QFileDialog::getOpenFileUrl(this, "Wybierz plik CSV do importu", QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)).toString();
    if(path == QString()) return;
    dst->setText(path);
}

void MainWindow::loadEgzTab(Query src) noexcept
{
    ui->EgzTabRes->clear();
    ui->EgzTabRes->setRowCount(0);
    ui->EgzTabRes->setColumnCount(3);
    ui->EgzTabRes->setHorizontalHeaderLabels(QStringList({"ID Egzaminu", "Nazwa Przedmiotu", "Data Egzaminu"}));

    for(const auto& var : src->data())
    {
        QString temp = var[2];
        temp.truncate(10);
        const int row = ui->EgzTabRes->rowCount();
        ui->EgzTabRes->insertRow(row);
        ui->EgzTabRes->setItem(row, 0, new QTableWidgetItem(var[0]));
        ui->EgzTabRes->setItem(row, 1, new QTableWidgetItem(sSubjects.at(var[1].toUInt())));
        ui->EgzTabRes->setItem(row, 2, new QTableWidgetItem( temp ));
    }
    src->clear();
}

void MainWindow::loadZespTab(Query src) noexcept
{
    ui->ZespTabRes->clear();
    ui->ZespTabRes->setRowCount(0);
    ui->ZespTabRes->setColumnCount(3);
    ui->ZespTabRes->setHorizontalHeaderLabels(QStringList({"ID Egzaminu", "Okręg", "Ilość Osób w Zespole"}));

    for(const auto& var : src->data())
    {
        const int row = ui->ZespTabRes->rowCount();
        ui->ZespTabRes->insertRow(row);
        ui->ZespTabRes->setItem(row, 0, new QTableWidgetItem(QString::number(var[0].toUInt())));
        ui->ZespTabRes->setItem(row, 1, new QTableWidgetItem(sRegions.at(var[1].toUInt())));
        ui->ZespTabRes->setItem(row, 2, new QTableWidgetItem(var[2]));
    }

    src->clear();
}

void MainWindow::loadPracTab(Query src) noexcept
{
    ui->PracTabRes->clear();
    ui->PracTabRes->setRowCount(0);
    ui->PracTabRes->setColumnCount(5);
    ui->PracTabRes->setHorizontalHeaderLabels(QStringList({"ID Pracownika", "Imię", "Nazwisko", "Okręg", "Uprawnienia"}));

    for(const auto& var : src->data())
    {
        const int row = ui->PracTabRes->rowCount();
        ui->PracTabRes->insertRow(row);
        ui->PracTabRes->setItem(row, 0, new QTableWidgetItem(var[0]));
        ui->PracTabRes->setItem(row, 1, new QTableWidgetItem(var[1]));
        ui->PracTabRes->setItem(row, 2, new QTableWidgetItem(var[2]));
        ui->PracTabRes->setItem(row, 3, new QTableWidgetItem(sRegions.at(var[3].toUInt())));
        ui->PracTabRes->setItem(row, 4, new QTableWidgetItem(var[4]));
    }

    src->clear();
}

void MainWindow::loadTerTab(Query src) noexcept
{
    ui->TerTabRes->clear();
    ui->TerTabRes->setRowCount(0);
    ui->TerTabRes->setColumnCount(10);
    ui->TerTabRes->setHorizontalHeaderLabels(QStringList({"PESEL", "Przedmiot 1", "Przedmiot 2", "Przedmiot 3","Przedmiot 4","Przedmiot 5", "Przedmiot 6", "Przedmiot 7", "Przedmiot 8", "Przedmiot 9"}));

    for(const auto& var : src->data())
    {
        const int row = ui->TerTabRes->rowCount();
        ui->TerTabRes->insertRow(row);

        std::queue<QString> huehue;
        const num_type subjects = var[1].toULongLong();
        resolve(huehue, subjects);

        ui->TerTabRes->setItem(row, 0, new QTableWidgetItem(var[0]));

        int col = 0;
        while (!huehue.empty())
        {
            col++;
            ui->TerTabRes->setItem(row, col, new QTableWidgetItem(huehue.front()));
            huehue.pop();
        }
    }

    src->clear();
}

void MainWindow::loadZdaTab(Query src) noexcept
{
    ui->ZdaTabRes->clear();
    ui->ZdaTabRes->setRowCount(0);
    ui->ZdaTabRes->setColumnCount(5);
    ui->ZdaTabRes->setHorizontalHeaderLabels(QStringList({"PESEL", "Imię", "Nazwisko", "Okręg", "Zdawane Przedmioty"}));

    for(const auto& var : src->data())
    {
        const int row = ui->ZdaTabRes->rowCount();
        ui->ZdaTabRes->insertRow(row);
        ui->ZdaTabRes->setItem(row, 0, new QTableWidgetItem(var[0]));
        ui->ZdaTabRes->setItem(row, 1, new QTableWidgetItem(var[1]));
        ui->ZdaTabRes->setItem(row, 2, new QTableWidgetItem(var[2]));
        ui->ZdaTabRes->setItem(row, 3, new QTableWidgetItem(sRegions.at(var[3].toUInt())));
        ui->ZdaTabRes->setItem(row, 4, new QTableWidgetItem(var[4]));
    }

    src->clear();
}

void MainWindow::loadAllDefaults() noexcept
{
    //Wypełnianie Tabel
    db->open();

    Query Egz{db->exec(loadEgzQuer+LIMIT_RESULTS)};
    Query Zes{db->exec(loadZesQuer+LIMIT_RESULTS)};
    Query Prac{db->exec(loadPracQuer+LIMIT_RESULTS)};
    Query Ter{db->exec(loadTerQuer+LIMIT_RESULTS)};
    Query Zda{db->exec(loadZdaQuer+LIMIT_RESULTS)};

    db->close();

    //Ładowanie Egzaminów
    loadEgzTab(Egz);

    //Ładowanie zespołów
    loadZespTab(Zes);

    //Ładowanie pracowników
    loadPracTab(Prac);

    //Ładowanie Terminarza
    loadTerTab(Ter);

    //Ładowanie Zdających
    loadZdaTab(Zda);
}

QString MainWindow::extract(const QString & src) const noexcept
{
    bool first = false;
    QString tmp = "";

    for(auto var : src)
    {
        if(var == '\'')
        {
            if(first) return tmp;
            first = true;
            continue;
        }

        if(first) tmp += var;
    }
}

bool MainWindow:: __divadable(const num_type thisOne, const num_type byThisOne) noexcept
{
    return (thisOne % byThisOne == 0) ? true : false;
}

void MainWindow::__EgzProps()
{
    if(ui->EgzTabRes->selectedItems().empty()) return;
    db->open();
    Query q1{db->exec(ExamProp::pracQuer + ui->EgzTabRes->selectedItems().front()->text()+";")};
    Query q2{db->exec(ExamProp::regioQuert + ui->EgzTabRes->selectedItems().front()->text()+";")};
    db->close();

    ExamProp tmp(ui->EgzTabRes->selectedItems().front()->text().toUInt(), q1, q2, this);
    tmp.show();
    tmp.topLevelWidget();
    tmp.exec();
}

void MainWindow::__ZdaSetModState(const bool state) noexcept
{
    if(ui->ZdaTabRes->currentRow() < 0 && state) return;

    //{"PESEL", "Imię", "Nazwisko", "Okręg", "Zdawane Przedmioty"}

    if(state)
    {
        ui->ZdaHas->setText("");
        ui->label_19->setToolTip("Jeżeli pozostawisz puste, hasło pozostanie bez zmian");
        ui->ZdaPESEL->setText(ui->ZdaTabRes->selectedItems()[0]->text());
        ui->ZdaImi->setText(ui->ZdaTabRes->selectedItems()[1]->text());
        ui->ZdaNaz->setText(ui->ZdaTabRes->selectedItems()[2]->text());
        ui->ZdaCombOkr->setCurrentIndex(ui->ZdaCombOkr->findText(ui->ZdaTabRes->selectedItems()[3]->text()));
        ui->ZdaPrzed->setText(ui->ZdaTabRes->selectedItems()[4]->text());

        ui->ZdaTabRes->setEnabled(false);
        ui->ZdaButDod->setEnabled(false);
        ui->ZdaButImport->setEnabled(false);
        ui->ZdaButUsu->setEnabled(false);
        ui->ZdaButSzu->setEnabled(false);
        ui->ZdaButPathSelect->setEnabled(false);
        ui->pushButton->setEnabled(false);
        ui->ZdaButMod->setText("Zatwierdź");
    }
    else
    {
        db->open();
        Query{db->exec( "UPDATE uczniowie SET imie='"+ui->ZdaImi->text()+"', nazwisko='"
                        + ui->ZdaNaz->text()+"', idOkregu="+QString::number(regionStr2Id(ui->ZdaCombOkr->currentText()))
                        + ", zdawanePrzedmioty="+ui->ZdaPrzed->text()+", pesel="+ui->ZdaPESEL->text()+ ((ui->ZdaHas->text() == "") ? "" : ", password=SHA2('"+ui->ZdaHas->text()+"', 256) ")
                        + " WHERE pesel=" + ui->ZdaTabRes->selectedItems().front()->text() + ";")};
        /*cout << ( "UPDATE pracownicy SET imie='"+ui->PracImi->text()+"', nazwisko='"
                  + ui->PracNaz->text()+"', idOkregu="+QString::number(regionStr2Id(ui->PracCombOkr->currentText()))
                  + ", uprawnienia="+ui->PracUpr->text()+", idpracownicy="+ui->PracLeg->text()+ ((ui->PracHas->text() == "") ? "" : " SHA2('"+ui->PracHas->text()+"', 256) ")
                  + " WHERE idpracownicy=" + ui->PracTabRes->selectedItems().front()->text() + ";");*/

        loadZdaTab(Query{db->exec( loadZdaQuer + LIMIT_RESULTS )});
        db->close();

        ui->ZdaPESEL->setText("");
        ui->ZdaHas->setText("");
        ui->ZdaImi->setText("");
        ui->ZdaNaz->setText("");
        ui->ZdaPrzed->setText("1");
        ui->ZdaCombOkr->setCurrentIndex(0);
        ui->label_19->setToolTip("");
        ui->ZdaTabRes->setEnabled(true);
        ui->ZdaButDod->setEnabled(true);
        ui->ZdaButImport->setEnabled(true);
        ui->ZdaButUsu->setEnabled(true);
        ui->ZdaButSzu->setEnabled(true);
        ui->ZdaButPathSelect->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->ZdaButMod->setText("Modyfikuj");
    }
}

void MainWindow::__PracSetModState(const bool state) noexcept
{
    if(ui->PracTabRes->currentRow() < 0 && state) return;

    //"SELECT idpracownicy, imie, nazwisko, idOkregu, uprawnienia FROM pracownicy "

    if(state)
    {
        ui->PracHas->setText("");
        ui->label_13->setToolTip("Jeżeli pozostawisz puste, hasło pozostanie bez zmian");
        ui->PracLeg->setText(ui->PracTabRes->selectedItems()[0]->text());
        ui->PracImi->setText(ui->PracTabRes->selectedItems()[1]->text());
        ui->PracNaz->setText(ui->PracTabRes->selectedItems()[2]->text());
        ui->PracUpr->setText(ui->PracTabRes->selectedItems()[4]->text());
        ui->PracCombOkr->setCurrentIndex(ui->PracCombOkr->findText(ui->PracTabRes->selectedItems()[3]->text()));

        ui->PracTabRes->setEnabled(false);
        ui->PracButDod->setEnabled(false);
        ui->PracButImp->setEnabled(false);
        ui->PracButUsu->setEnabled(false);
        ui->PracButZna->setEnabled(false);
        ui->PracButPathSelect->setEnabled(false);
        ui->PracButMod->setText("Zatwierdź");
    }
    else
    {
        db->open();
        Query{db->exec( "UPDATE pracownicy SET imie='"+ui->PracImi->text()+"', nazwisko='"
                        + ui->PracNaz->text()+"', idOkregu="+QString::number(regionStr2Id(ui->PracCombOkr->currentText()))
                        + ", uprawnienia="+ui->PracUpr->text()+", idpracownicy="+ui->PracLeg->text()+ ((ui->PracHas->text() == "") ? "" : ", password=SHA2('"+ui->PracHas->text()+"', 256) ")
                        + " WHERE idpracownicy=" + ui->PracTabRes->selectedItems().front()->text() + ";")};

        loadPracTab(Query{db->exec(loadPracQuer+LIMIT_RESULTS)});
        db->close();

        ui->PracLeg->setText("");
        ui->PracHas->setText("");
        ui->PracImi->setText("");
        ui->PracNaz->setText("");
        ui->PracUpr->setText("1");
        ui->PracCombOkr->setCurrentIndex(0);
        ui->label_13->setToolTip("");
        ui->PracTabRes->setEnabled(true);
        ui->PracButDod->setEnabled(true);
        ui->PracButImp->setEnabled(true);
        ui->PracButUsu->setEnabled(true);
        ui->PracButZna->setEnabled(true);
        ui->PracButPathSelect->setEnabled(true);
        ui->PracButMod->setText("Modyfikuj");
    }
}

void MainWindow::__ZdaProps()
{
    //	{"PESEL", "Imię", "Nazwisko", "Okręg", "Zdawane Przedmioty"}
    if(ui->ZdaTabRes->currentRow() < 0) return;

    auto data = ui->ZdaTabRes->selectedItems();

    QString dane = "Podstawowe Dane: \n\n";
    dane+="\tPESEL: "+data[0]->text()+"\n";
    dane+="\tImię: "+data[1]->text()+"\n";
    dane+="\tNazwisko: "+data[2]->text()+"\n";
    dane+="\tOkręg: "+data[3]->text()+"\n\n";
    dane+="Zdawane Przedmioty: \n\n";
    std::queue<QString> tmp;
    resolve(tmp, data[4]->text().toULongLong());
    if(tmp.size() > 1) tmp.pop();
    while(!tmp.empty()) { dane+="\t"+tmp.front()+"\n"; tmp.pop(); }

    QMessageBox(QMessageBox::Icon::NoIcon, "Informacje", dane).exec();
}

void MainWindow::__loadEverythink()
{
    const std::string path{ QDir::toNativeSeparators((QStandardPaths::writableLocation(QStandardPaths::HomeLocation))+"/settings.ini").toStdString() };
    std::ifstream input(path);
    if(!input.good())
    {
        input.close();
        QMessageBox a(QMessageBox::Icon::Critical, "Błąd", "Brak pliku `settings.ini` w katalogu domowym, utworzyć?", QMessageBox::Yes | QMessageBox::No);
        if(a.exec() == QMessageBox::Yes)
        {
            std::ofstream output(path);

            output<<"#current"<<std::endl;

            output<<"host=''"<<std::endl;
            output<<"port=''"<<std::endl;
            output<<"dbName=''"<<std::endl;
            output<<"user=''"<<std::endl;
            output<<"password=''"<<std::endl;

            output<<std::endl;
            output<<std::endl;

            output<<"#default - do not delete!!!!!!!"<<std::endl;
            output<<"#host='www.db4free.net'"<<std::endl;
            output<<"#port='3306'"<<std::endl;
            output<<"#dbName='cy8h8p1of1'"<<std::endl;
            output<<"#user='cy8h8p1of1'"<<std::endl;
            output<<"#password='0JpOagXzku'";

            output.close();
        }
    }
    std::string line, ihost, iuser, ipass, idbname;
    unsigned short iport;

    while(std::getline(input, line))
    {
        if(line[0] == '#') continue;
        QString tmp(line.c_str());
        QString t = extract(tmp);
        if(tmp.indexOf("host='") != -1)
        {
            ihost = t.toStdString();
            ui->UstSerwAdr->setText(t);
        }
        else if(tmp.indexOf("port='") != -1)
        {
            iport = t.toUShort();
            ui->UstPort->setText(t);
        }
        else if(tmp.indexOf("dbName='") != -1)
        {
            idbname = t.toStdString();
            ui->UstDBName->setText(t);
        }
        else if(tmp.indexOf("user='") != -1)
        {
            iuser = t.toStdString();
            ui->UstUzyt->setText(t);
        }
        else if(tmp.indexOf("password='") != -1)
        {
            ipass = t.toStdString();
            ui->UstHas->setText(t);

        }else continue;
    }
    input.close();

    db = new mysqldriver(ihost, iuser, ipass, idbname, iport);

    if(!__testDB())
    {
        QMessageBox(QMessageBox::Icon::Critical, "Błąd krytyczny", "Nie udało się ustanowić połączenia z bazą danych!!!\n\nPrzejdź do ustawień (ostatnie zakładka), sprawdź parametry połączenia i połącz ponownie!").exec();
        ui->tabWidget->setCurrentIndex(5);
        ui->EgzTab->setEnabled(false);
        ui->ZesTab->setEnabled(false);
        ui->PracTab->setEnabled(false);
        ui->tab->setEnabled(false);
        ui->ZdaTab->setEnabled(false);
        return;
    }

    if(db->open())
    {
        std::unique_ptr<querryResult> q{db->exec("SELECT idprzedmiot, nazwa FROM przedmiot;")};
        db->close();

        for(const auto & var : q->data()) { sSubjects[QString(var[0]).toUInt()] = QString(var[1]); }

        db->open();
        q.reset(db->exec("SELECT idstanowiska, nazwa FROM stanowiska;"));
        db->close();

        for(const auto & var : q->data()) { sPrivillages[QString(var[0]).toUInt()] = QString(var[1]); }

        db->open();
        q.reset(db->exec("SELECT idOkregu, nazwaOkregu FROM okregi;"));
        db->close();

        for(const auto & var : q->data()) { sRegions[QString(var[0]).toUInt()] = QString(var[1]); }

    }else db->close();

    //Dodawanie przedmiotów
    ui->EgzCombPrzed->clear();

    for(const auto & var : sSubjects) ui->EgzCombPrzed->addItem(var.second);

    //Dodawanie okręgów
    ui->PracCombOkr->clear();
    ui->ZdaCombOkr->clear();

    for(const auto & var : sRegions)
    {
        ui->PracCombOkr->addItem(var.second);
        ui->ZdaCombOkr->addItem(var.second);
    }

    loadAllDefaults();

    //Ładowanie Szczegółów
    //Kalendarz, Ilość Zakończonych Egzaminów

    if(!db->open()) try{ throw std::bad_function_call(); }catch(...) { db->close(); throw; }

    Query Zda{db->exec("SELECT MIN(data) FROM egzamin;")};
    Query Ter{db->exec("SELECT COUNT(*)/(SELECT COUNT(*) FROM egzamin) FROM egzamin WHERE data<CURRENT_DATE;")};

    db->close();

    //Konwersja std::string -> QDate
    ui->EgzDat->setDate(str2date(Zda->data().front().front()).date());

    ui->EgzBar->setValue(static_cast<int>(Ter->data().front().front().toDouble() * 100.0));

    engine = new exportXLSX(db);
    ui->WynName->setText("raport_"+QDateTime::currentDateTime().toString("yyyy_MM_dd")+"_LVL"+QString::number(ui->WynDetail->value()));
    ui->WynPathView->setText(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));

    ui->EgzTab->setEnabled(true);
    ui->ZesTab->setEnabled(true);
    ui->PracTab->setEnabled(true);
    ui->tab->setEnabled(true);
    ui->ZdaTab->setEnabled(true);

}

void MainWindow::__updateLIMIT_RESULTS(const QString & src) noexcept
{
    if(src == "0")
    {
        LIMIT_RESULTS = " ;";
        return;
    }
    LIMIT_RESULTS = " LIMIT "+src+";";
}

bool MainWindow::__testDB() noexcept
{
    if(!db->open()) return  false;
    Query t1{db->exec("SHOW TABLES;")};
    db->close();

    int wyn = 0;

    for(const auto& var : t1->data())
    {
        if(
                var[0] == "egzamin" ||
                var[0] == "oceny" ||
                var[0] == "okregi" ||
                var[0] == "pracownicy" ||
                var[0] == "przedmiot" ||
                var[0] == "stanowiska" ||
                var[0] == "uczniowie" ||
                var[0] == "zespoly"
                ) wyn++;
    }

    return wyn >= 8;

}


void MainWindow::on_PracUprCreator_clicked()
{
    Permissions dial(ui->PracUpr->text().toULongLong(),false, this);
    dial.show();
    dial.topLevelWidget();
    dial.exec();
    ui->PracUpr->setText(QString::number(dial.result));
}

void MainWindow::on_ZdaButPrzed_clicked()
{
    Permissions dial(ui->ZdaPrzed->text().toULongLong(), true, this);
    dial.show();
    dial.topLevelWidget();
    dial.exec();
    ui->ZdaPrzed->setText(QString::number(dial.result));
}

void MainWindow::on_EgzButZespWyb_clicked()
{
    Team tmp(db, &__mNewTeamBuffor, __mLastSearchTeam, this);
    tmp.show();
    tmp.topLevelWidget();
    tmp.exec();

    __mNewTeamBuffor = tmp.curr;

    QString t = "";

    for(auto var : __mNewTeamBuffor) t+=QString::number(var)+" ";

    __mLastSearchTeam = tmp.__last;

    ui->EgzZesp->setText(t);
}

void MainWindow::on_PomButKalk_clicked()
{
    std::queue<QString> tmp;
    resolve(tmp, static_cast<num_type>(ui->PomSpinKalk->text().toULongLong()));
    QString wyn = "";
    if(tmp.size() > 1) tmp.pop();

    while (!tmp.empty())
    {
        wyn+=tmp.front() + "\n";
        tmp.pop();
    }

    QMessageBox a(QMessageBox::Icon::Information, "Obliczone Uprawnienia", wyn);
    a.show();
    a.topLevelWidget();
    a.exec();
}

void MainWindow::on_EgzPathSelect_clicked()
{
    getPath(ui->EgzPathView);
}

void MainWindow::on_ZespPathSelect_clicked()
{
    getPath(ui->ZespPathView);
}

void MainWindow::on_PracButPathSelect_clicked()
{
    getPath(ui->PracPathView);
}

void MainWindow::on_ZdaButPathSelect_clicked()
{
    getPath(ui->ZdaPathView);
}

void MainWindow::on_EgzButImp_clicked()
{
    if(ui->EgzPathView->text() == "") return;
    CSVImporter(CSVImporter::Egzaminy, ui->EgzPathView->text(), db).exec();
}

void MainWindow::on_ZespButImp_clicked()
{
    if(ui->ZespPathView->text() == "") return;
    CSVImporter(CSVImporter::Zespoly, ui->ZespPathView->text(), db).exec();
}

void MainWindow::on_PracButImp_clicked()
{
    if(ui->PracPathView->text() == "") return;
    CSVImporter(CSVImporter::Pracownicy, ui->PracPathView->text(), db).exec();
}

void MainWindow::on_ZdaButImport_clicked()
{
    if(ui->ZdaPathView->text() == "") return;
    CSVImporter(CSVImporter::Zdajacy, ui->ZdaPathView->text(), db).exec();
}

void MainWindow::on_EgzButDod_clicked()
{
    //Przetestować
    QString quer = "SELECT MAX(idegzamin) FROM egzamin;";

    db->open();

    Query q { db->exec(quer) };
    db->close();

    size_t max = q->data().front().front().toUInt();
    q->clear();
    max++;

    quer = "INSERT INTO egzamin VALUES("+QString::number(max)+", "+QString::number(subStr2Id(ui->EgzCombPrzed->currentText()))+", '"+ui->EgzDat->date().toString("yyyy-MM-dd")+"');";

    db->open();
    try
    {
        q.reset(db->exec(quer));
    }
    catch(const std::invalid_argument& e)
    {
        QMessageBox(QMessageBox::Icon::Information, "Błąd", "Nie można dodać pozycji").exec();
        return;
    }
    q->clear();

    for(auto var : __mNewTeamBuffor)
    {
        Query tmp { db->exec("INSERT INTO zespoly VALUES("+QString::number(var)+", "+QString::number(max)+");") };
    }

    db->close();

    __mNewTeamBuffor.clear();
    __mLastSearchTeam = "";
    ui->EgzZesp->setText("");
    if(ui->EgzCombPrzed->currentIndex() + 1 != ui->EgzCombPrzed->count()) ui->EgzCombPrzed->setCurrentIndex(ui->EgzCombPrzed->currentIndex() + 1);
}

void MainWindow::on_spinBox_valueChanged(const QString &arg1)
{
    __updateLIMIT_RESULTS(arg1);
}

void MainWindow::on_spinBox_editingFinished()
{
    loadAllDefaults();
}

void MainWindow::on_EgzButSzu_clicked()
{
    QString q = loadEgzQuer + " JOIN przedmiot ON idprzedmiot=idprzedmiotu JOIN zespoly ON id_egzamin=idegzamin "
            + "WHERE data='"+ui->EgzDat->date().toString("yyyy-MM-dd") + "' "
            + "AND idprzedmiot%" +QString::number(subStr2Id(ui->EgzCombPrzed->currentText()))+"=0 ";

    if(!__mNewTeamBuffor.empty())
    {
        for(auto var : __mNewTeamBuffor)
        {
            q+=" AND idpracownika="+QString::number(var)+" ";
        }
    }

    q+=LIMIT_RESULTS;

    db->open();
    loadEgzTab(Query{db->exec(q)});
    db->close();

}

void MainWindow::on_EgzButUsu_clicked()
{
    if(ui->EgzTabRes->selectedItems().empty()) return;
    QString id = ui->EgzTabRes->selectedItems().front()->text();
    db->open();
    Query{db->exec("DELETE FROM egzamin WHERE idegzamin="+id+";")};
    loadEgzTab(Query{db->exec(loadEgzQuer+LIMIT_RESULTS)});
    db->close();

    __mNewTeamBuffor.clear();
    __mLastSearchTeam = "";
    ui->EgzZesp->setText("");
    ui->EgzCombPrzed->setCurrentIndex(0);

}

void MainWindow::on_EgzButInf_clicked()
{
    __EgzProps();
}

void MainWindow::on_EgzTabRes_itemDoubleClicked(QTableWidgetItem *item)
{
    __EgzProps();
}

void MainWindow::on_ZespButDod_clicked()
{
    db->open();
    Query{db->exec("INSERT INTO zespoly VALUES("+ui->ZespSpinEgz->text()+", "+ui->ZespSpinPrac->text()+");")};
    loadZespTab(Query{db->exec(loadZesQuer+LIMIT_RESULTS)});
    db->close();

    ui->ZespSpinEgz->setValue(0);
    ui->ZespSpinPrac->setValue(0);
}

void MainWindow::on_ZespButUsu_clicked()
{
    db->open();
    Query{db->exec("DELETE FROM zespoly WHERE idpracownika="+ui->ZespSpinPrac->text()+" AND id_egzamin="+ui->ZespSpinEgz->text()+";")};
    loadZespTab(Query{db->exec(loadZesQuer+LIMIT_RESULTS)});
    db->close();

    ui->ZespSpinEgz->setValue(0);
    ui->ZespSpinPrac->setValue(0);
}

void MainWindow::on_ZespButSzu_clicked()
{
    db->open();
    loadZespTab(Query{db->exec(loadZesQuer+" HAVING id_egzamin LIKE '%"+ui->ZespSpinEgz->text()+"%' "+LIMIT_RESULTS)});
    db->close();
}

void MainWindow::on_ZespButPraWyb_clicked()
{
    std::list<size_t> x;
    advancedSearch tmp{db, x, "SELECT idpracownicy FROM pracownicy WHERE ", true};
    tmp.show();
    tmp.exec();

    if(tmp.everythingGoesGoog)
    {
        db->open();
        Query q{db->exec(tmp.res)};
        db->close();
        ui->ZespSpinPrac->setValue(q->data().front().front().toInt());
    }
}

void MainWindow::on_ZespButEgzWyb_clicked()
{
    SelectExam tmp(db, this);
    tmp.show();
    tmp.topLevelWidget();
    tmp.exec();

    if(tmp.IDresult == 0) return;

    ui->ZespSpinEgz->setValue(static_cast<int>(tmp.IDresult));
}

void MainWindow::on_PracButDod_clicked()
{
    if(
            ui->PracLeg->text() == "" ||
            ui->PracImi->text() == "" ||
            ui->PracNaz->text() == "" ||
            ui->PracUpr->text() == "" ||
            ui->PracHas->text() == ""
            )
    {
        QMessageBox(QMessageBox::Icon::Warning, "Błąd", "Proszę Uzupełnić Wszystkie Dane!").exec();
        return;
    }

    QString querry = "INSERT INTO pracownicy VALUES("+ui->PracLeg->text()
            + ", '"+ui->PracImi->text() + "' "
            + ", '"+ui->PracNaz->text() + "' "
            + ", "+QString::number(regionStr2Id(ui->PracCombOkr->currentText()))
            + ", "+ui->PracUpr->text()
            + ", SHA2('"+ui->PracHas->text()+"', 256));";

    db->open();
    Query{db->exec(querry)};
    db->close();

    ui->PracLeg->setText("");
    ui->PracImi->setText("");
    ui->PracNaz->setText("");
    ui->PracUpr->setText("");
    ui->PracHas->setText("");
}

void MainWindow::on_PracHas_textChanged(const QString &arg1)
{
    for(const auto& var : arg1)
    {
        if(!var.isLetterOrNumber())
        {
            QMessageBox(QMessageBox::Icon::Critical, "Błąd Formatu","Użyto Niedozwolonego znaku.\nDozwolone Tylko litery i liczby.").exec();
            QString tmp = "";
            for(auto i : arg1) if(i.isLetterOrNumber()) tmp+=i;
            ui->PracHas->setText(tmp);
            return;
        }
    }
}

void MainWindow::on_PracChck_stateChanged(int arg1)
{
    ui->PracHas->setEchoMode(ui->PracChck->isChecked() ? QLineEdit::EchoMode::Password : QLineEdit::EchoMode::Normal);
}

void MainWindow::on_PracButZna_clicked()
{
    //loadPracQuer = "SELECT idpracownicy, imie, nazwisko, idOkregu, uprawnienia FROM pracownicy ";
    QString querry = loadPracQuer + " WHERE idpracownicy LIKE '%"+ui->PracLeg->text()+"%'"
            + " AND imie LIKE '%"+ui->PracImi->text()+"%' AND nazwisko LIKE '%"+ui->PracNaz->text()+"%'"
            + " AND idOkregu="+QString::number(regionStr2Id(ui->PracCombOkr->currentText()))
            + " AND uprawnienia%"+ui->PracUpr->text()+"=0 "
            + LIMIT_RESULTS;

    db->open();
    loadPracTab(Query{db->exec(querry)});
    db->close();

}

void MainWindow::on_PracButUsu_clicked()
{
    if(ui->PracTabRes->currentRow() < 0) return;

    db->open();
    Query{db->exec("DELETE FROM pracownicy WHERE idpracownicy="+ui->PracTabRes->selectedItems().front()->text()+";")};
    loadPracTab(Query{db->exec(loadPracQuer + LIMIT_RESULTS)});
    db->close();
}

void MainWindow::on_PracButMod_clicked()
{
    __mActPracModState = !__mActPracModState;
    __PracSetModState(__mActPracModState);
}

void MainWindow::on_TerButPESEL_clicked()
{
    advancedPupilSearch tmp(db, this);
    tmp.show();
    tmp.topLevelWidget();
    tmp.exec();

    if(tmp.ok)
    {
        db->open();
        Query q{db->exec("SELECT pesel FROM uczniowie "+tmp.Result)};
        db->close();
        ui->TerPESEL->setText(q->data().front().front());
    }

}

void MainWindow::on_TerButDod_clicked()
{
    num_type add=0;

    db->open();

    Query q1{db->exec("SELECT idprzedmiot FROM przedmiot JOIN egzamin ON idprzedmiot=idprzedmiotu WHERE idegzamin="+ui->TerEgzID->text()+";")};

    q1.reset(db->exec("SELECT zdawanePrzedmioty FROM uczniowie WHERE pesel='"+ui->TerPESEL->text()+"';"));
    Query q{db->exec("SELECT COUNT(*) FROM uczniowie WHERE zdawanePrzedmioty%"+q1->data().front().front()+"=0 AND pesel='"+ui->TerPESEL->text()+"';")};
    db->close();

    if(q->data().front().front().toInt() != 0)
    {
        QMessageBox(QMessageBox::Icon::Warning, "Błąd", "Ta osoba zdaje już ten przedmiot!").exec();
        return;
    }

    num_type przedm = q1->data().front().front().toULongLong();
    przedm = przedm*add;

    db->open();
    Query{db->exec("UPDATE uczniowie SET zdawanePrzedmioty="+QString::number(przedm)+" WHERE pesel='"+ui->TerPESEL->text()+"';")};
    loadTerTab(Query{db->exec(loadTerQuer+LIMIT_RESULTS)});
    db->close();
}

void MainWindow::on_TerButSzu_clicked()
{
    db->open();
    Query q1{db->exec("SELECT idprzedmiot FROM przedmiot JOIN egzamin ON idprzedmiot=idprzedmiotu WHERE idegzamin="+ui->TerEgzID->text()+";")};
    loadTerTab(Query{db->exec(loadTerQuer+" WHERE pesel LIKE '%"+ui->TerPESEL->text()+"%' AND zdawanePrzedmioty%"+q1->data().front().front()+"=0 "+LIMIT_RESULTS)});
    db->close();
}

void MainWindow::on_TerButEgz_clicked()
{
    SelectExam tmp(db, this);
    tmp.show();
    tmp.topLevelWidget();
    tmp.exec();

    if(tmp.IDresult == 0) return;

    ui->TerEgzID->setValue(tmp.IDresult);
}

void MainWindow::on_TerButUsu_clicked()
{
    num_type add=0;

    db->open();

    Query q1{db->exec("SELECT idprzedmiot FROM przedmiot JOIN egzamin ON idprzedmiot=idprzedmiotu WHERE idegzamin="+ui->TerEgzID->text()+";")};
    Query q{db->exec("SELECT COUNT(*) FROM uczniowie WHERE zdawanePrzedmioty%"+q1->data().front().front()+"=0 AND pesel='"+ui->TerPESEL->text()+"';")};
    q1.reset(db->exec("SELECT zdawanePrzedmioty FROM uczniowie WHERE pesel='"+ui->TerPESEL->text()+"';"));

    db->close();

    if(q->data().front().front().toInt() == 0)
    {
        QMessageBox(QMessageBox::Icon::Warning, "Błąd", "Ta osoba nie zdaje tego przedmiotu!").exec();
        return;
    }

    num_type przedm = q1->data().front().front().toULongLong();
    przedm = przedm/add;

    db->open();
    Query{db->exec("UPDATE uczniowie SET zdawanePrzedmioty="+QString::number(przedm)+" WHERE pesel='"+ui->TerPESEL->text()+"';")};
    loadTerTab(Query{db->exec(loadTerQuer+LIMIT_RESULTS)});
    db->close();
}

void MainWindow::on_ZdaChck_stateChanged(int arg1)
{
    ui->ZdaHas->setEchoMode((ui->ZdaChck->isChecked() ? QLineEdit::EchoMode::Password : QLineEdit::EchoMode::Normal));
}

void MainWindow::on_ZdaHas_textChanged(const QString &arg1)
{
    for(auto var : arg1)
    {
        if(!var.isLetterOrNumber())
        {
            QMessageBox(QMessageBox::Icon::Critical, "Błąd Formatu","Użyto Niedozwolonego znaku.\nDozwolone Tylko litery i liczby.").exec();
            QString tmp = "";
            for(auto i : arg1) if(i.isLetterOrNumber()) tmp+=i;
            ui->ZdaHas->setText(tmp);
            return;
        }
    }
}

void MainWindow::on_ZdaButDod_clicked()
{
    if(
            ui->ZdaImi->text() == "" ||
            ui->ZdaPESEL->text().length() != 11 ||
            ui->ZdaNaz->text() == "" ||
            ui->ZdaHas->text() == "" ||
            ui->ZdaPrzed->text() == ""
            )
    {
        QMessageBox(QMessageBox::Icon::Critical, "Błąd", "Wprowadzone dane są niekompletne lub nieprawidłowe").exec();
        return;
    }

    QString querry = "INSERT INTO uczniowie VALUES('"+ui->ZdaPESEL->text()+"', '"+ui->ZdaImi->text()+"', '"+ui->ZdaNaz->text()+"', "+QString::number(regionStr2Id(ui->ZdaCombOkr->currentText())) + ", "+ui->ZdaPrzed->text()+", SHA2('"+ui->ZdaHas->text()+"', 256));";

    //cout <<querry;

    db->open();
    Query{db->exec(querry)};
    loadTerTab(Query{db->exec(loadTerQuer+LIMIT_RESULTS)});
    db->close();

    ui->ZdaHas->setText("");
    ui->ZdaImi->setText("");
    ui->ZdaNaz->setText("");
    ui->ZdaCombOkr->setCurrentText("0");
    ui->ZdaPESEL->setText("");
    ui->ZdaPrzed->setText("1");
}

void MainWindow::on_ZdaButSzu_clicked()
{
    //const QString loadZdaQuer = "SELECT pesel, imie, nazwisko, idOkregu, zdawanePrzedmioty FROM uczniowie";
    QString quer = loadZdaQuer + " WHERE pesel LIKE '%"+ui->ZdaPESEL->text()+"%' AND imie LIKE '%"+ui->ZdaImi->text()+"%' AND "+
            "nazwisko LIKE '%"+ui->ZdaNaz->text()+"%' AND "+
            "idOkregu="+QString::number(regionStr2Id(ui->ZdaCombOkr->currentText()))+" AND zdawanePrzedmioty%"+ui->ZdaPrzed->text()+"=0 "+LIMIT_RESULTS;

    db->open();
    loadZdaTab(Query{db->exec(quer)});
    db->close();
}

void MainWindow::on_pushButton_clicked()
{
    __ZdaProps();
}

void MainWindow::on_ZdaTabRes_itemDoubleClicked(QTableWidgetItem *item)
{
    __ZdaProps();
}

void MainWindow::on_ZdaButUsu_clicked()
{
    if(ui->ZdaTabRes->currentRow() < 0) return;

    QMessageBox a(QMessageBox::Icon::Warning, "Ostrzeżenie", "Usuwając Zdającego usuwasz jednoscześnie wszystkie jego oceny!", QMessageBox::StandardButton::Ok | QMessageBox::StandardButton::Cancel);
    a.exec();

    if(a.result() == QMessageBox::StandardButton::Cancel) return;

    db->open();
    Query{db->exec("DELETE FROM oceny WHERE iducznia='"+ui->ZdaTabRes->selectedItems().front()->text()+"';")};
    Query{db->exec("DELETE FROM uczniowie WHERE pesel='"+ui->ZdaTabRes->selectedItems().front()->text()+"';")};
    loadZdaTab(Query{db->exec(loadZdaQuer+LIMIT_RESULTS)});
    loadTerTab(Query{db->exec(loadTerQuer + LIMIT_RESULTS)});
    db->close();
}

void MainWindow::on_ZdaButMod_clicked()
{
    __mActZdaModState = !__mActZdaModState;
    __ZdaSetModState(__mActZdaModState);
}

void MainWindow::on_WynButGen_clicked()
{

    if(ui->WynPathView->text() == "" || ui->WynName->text() == "")
    {
        QMessageBox(QMessageBox::Icon::Critical, "Niepoprawne Dane", "Wprowadzono niepoprawne dane (ścieżka lub nazwa pliku)").exec();
        return;
    }

    QString path = ui->WynPathView->text();
    if(path == QString()) return;

    path+="/"+ui->WynName->text()+".xlsx";

    path= QDir::toNativeSeparators(path);

    if(ui->WynDetail->value() == 4)
    {
        QMessageBox a(QMessageBox::Icon::Critical, "Ostrzeżenie", "Uwaga generowany dokument będzie zawierać dane wrażliwe (PESEL oraz wyniki).\nCzy posiadasz autoryzację oraz jesteś w stanie zapewnić odpowiednie warunki do jego przechowywania, aby dane były bezpieczne?", QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);
        a.exec();
        if(a.result() == QMessageBox::StandardButton::No) return;
    }

    engine->process(path, static_cast<exportXLSX::precision>(ui->WynDetail->value()));
    QMessageBox(QMessageBox::Icon::Information, "Ukończono", "Wygenerowano raport w lokalizacji: \n\n\t"+ui->WynPathView->text()).exec();
    if(ui->WynDetail->value() == 4) QMessageBox(QMessageBox::Icon::Information, "Zalecenie", "Zaleca się otwarcie tego arkusza w programie Excel 2010 lub nowszym!");
}

void MainWindow::on_WynButPathSelect_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this);
    if(path == QString()) return;

    ui->WynPathView->setText(path);
}

void MainWindow::on_UstZap_clicked()
{

    std::ofstream output(QDir::toNativeSeparators((QStandardPaths::writableLocation(QStandardPaths::HomeLocation))+"/settings.ini").toStdString());

    output<<"#current"<<std::endl;

    output<<"host='"+ui->UstSerwAdr->text().toStdString()+"'"<<std::endl;
    output<<"port='"+ui->UstPort->text().toStdString()+"'"<<std::endl;
    output<<"dbName='"+ui->UstDBName->text().toStdString()+"'"<<std::endl;
    output<<"user='"+ui->UstUzyt->text().toStdString()+"'"<<std::endl;
    output<<"password='"+ui->UstHas->text().toStdString()+"'"<<std::endl;

    output<<std::endl;
    output<<std::endl;

    output<<"#default - do not delete!!!!!!!"<<std::endl;
    output<<"#host='www.db4free.net'"<<std::endl;
    output<<"#port='3306'"<<std::endl;
    output<<"#dbName='cy8h8p1of1'"<<std::endl;
    output<<"#user='cy8h8p1of1'"<<std::endl;
    output<<"#password='0JpOagXzku'";

    output.close();

    __loadEverythink();
}
