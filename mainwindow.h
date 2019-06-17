#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QDebug>

#include "MySQLSimpleDriver.hpp"

#include <map>
#include <list>
#include <queue>

#include "permissions.h"
#include "csvimporter.h"
#include "exportxlsx.h"

#define cout qDebug()

//CY8H8P1of1
//0JpOagXzku

using num_type = unsigned long long int;

namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	static QString LIMIT_RESULTS;

	static std::map<num_type, QString> sPrivillages;
	static num_type privStr2Id(const QString& src) noexcept;

	static std::map<num_type, QString> sSubjects;
	static num_type subStr2Id(const QString& src) noexcept;

	static std::map<num_type, QString> sRegions;
	static num_type regionStr2Id(const QString& src) noexcept;

	const QString loadEgzQuer = "SELECT DISTINCT idegzamin, idprzedmiotu, data FROM egzamin ";

	const QString loadZesQuer = "SELECT id_egzamin, idOkregu, COUNT(idpracownika) FROM zespoly z JOIN pracownicy p ON p.idpracownicy=z.idpracownika GROUP BY id_egzamin, idOkregu ";

	const QString loadPracQuer = "SELECT idpracownicy, imie, nazwisko, idOkregu, uprawnienia FROM pracownicy ";

	const QString loadTerQuer = "SELECT PESEL, zdawanePrzedmioty FROM uczniowie ";

	const QString loadZdaQuer = "SELECT pesel, imie, nazwisko, idOkregu, zdawanePrzedmioty FROM uczniowie";

    mysqldriver * db = nullptr;

	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow();

	static void resolve(std::queue<QString>& dst, const num_type src) noexcept;

	static num_type process(const std::list<num_type>& src) noexcept;

	void getPath(QLineEdit * dst);

    void loadEgzTab(Query src) noexcept;

    void loadZespTab(Query src) noexcept;

    void loadPracTab(Query src) noexcept;

    void loadTerTab(Query src) noexcept;

    void loadZdaTab(Query src) noexcept;

	void loadAllDefaults() noexcept;

	QString extract(const QString& src) const noexcept;

private slots:

	void on_PracUprCreator_clicked();

	void on_ZdaButPrzed_clicked();

	void on_EgzButZespWyb_clicked();

	void on_PomButKalk_clicked();

	void on_EgzPathSelect_clicked();

	void on_ZespPathSelect_clicked();

	void on_PracButPathSelect_clicked();

	void on_ZdaButPathSelect_clicked();

	void on_EgzButImp_clicked();

	void on_ZespButImp_clicked();

	void on_PracButImp_clicked();

	void on_ZdaButImport_clicked();

	void on_EgzButDod_clicked();

	void on_spinBox_valueChanged(const QString &arg1);

	void on_spinBox_editingFinished();

	void on_EgzButSzu_clicked();

	void on_EgzButUsu_clicked();

	void on_EgzButInf_clicked();

	void on_EgzTabRes_itemDoubleClicked(QTableWidgetItem *item);

	void on_ZespButDod_clicked();

	void on_ZespButUsu_clicked();

	void on_ZespButSzu_clicked();

	void on_ZespButPraWyb_clicked();

	void on_ZespButEgzWyb_clicked();

	void on_PracButDod_clicked();

	void on_PracHas_textChanged(const QString &arg1);

	void on_PracChck_stateChanged(int arg1);

	void on_PracButZna_clicked();

	void on_PracButUsu_clicked();

	void on_PracButMod_clicked();

	void on_TerButPESEL_clicked();

	void on_TerButDod_clicked();

	void on_TerButSzu_clicked();

	void on_TerButEgz_clicked();

	void on_TerButUsu_clicked();

	void on_ZdaChck_stateChanged(int arg1);

	void on_ZdaHas_textChanged(const QString &arg1);

	void on_ZdaButDod_clicked();

	void on_ZdaButSzu_clicked();

	void on_pushButton_clicked();

	void on_ZdaTabRes_itemDoubleClicked(QTableWidgetItem *item);

	void on_ZdaButUsu_clicked();

	void on_ZdaButMod_clicked();

	void on_WynButGen_clicked();

	void on_WynButPathSelect_clicked();

	void on_UstZap_clicked();

private:

	Ui::MainWindow *ui;

	static bool __divadable(const num_type thisOne, const num_type byThisOne) noexcept;

	std::list<size_t> __mNewTeamBuffor;

	QString __mLastSearchTeam = "";

	void __EgzProps();

	void __PracSetModState(const bool state) noexcept;

	void __ZdaSetModState(const bool state) noexcept;

	bool __mActPracModState = false;

	bool __mActZdaModState = false;

	void __ZdaProps();

	void __loadEverythink();

	exportXLSX * engine;

	void __updateLIMIT_RESULTS(const QString& src) noexcept;

	bool __testDB() noexcept;

};
