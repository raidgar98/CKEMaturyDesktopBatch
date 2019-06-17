#include "exportxlsx.h"
#include "mainwindow.h"

//Osób zdało
//SELECT (SELECT COUNT(DISTINCT pesel) FROM uczniowie) - COUNT(DISTINCT iducznia) FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu WHERE przedmiot.nazwa LIKE '%odstawo%' AND wynik<30;

//Osób niezdało
//SELECT COUNT(DISTINCT iducznia) FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu WHERE przedmiot.nazwa LIKE '%odstawo%' AND wynik<30;

//Średnia z każdego okręgu
//SELECT okregi.nazwaOkregu, AVG(wynik) FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia JOIN okregi ON okregi.idOkregu=uczniowie.idOkregu GROUP BY nazwaOkregu;

//Średnia z każdego przedmiotu
//SELECT przedmiot.nazwa, AVG(wynik) FROM oceny JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu GROUP BY przedmiot.nazwa;

//Średnia z każdego przemdiotu w każdym okręgu
//SELECT okregi.nazwaOkregu, przedmiot.nazwa, AVG(wynik), MIN(wynik), MAX(wynik) FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia JOIN okregi ON okregi.idOkregu=uczniowie.idOkregu JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu  GROUP BY nazwaOkregu, przedmiot.nazwa;

//Najlepsze/Najgorsze wyniki w każdym okręgu
//SELECT okregi.nazwaOkregu, MAX/MIN(wynik) FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia JOIN okregi ON okregi.idOkregu=uczniowie.idOkregu GROUP BY nazwaOkregu;

//Najlpsze/Najgorsze wyniki z każdego przedmiotu
//SELECT przedmiot.nazwa, MAX/MIN(wynik) FROM oceny JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu GROUP BY przedmiot.nazwa;

//Najlepsze/Najgorsze wyniki z każdego przedmiotu w każdym okręgu
//SELECT okregi.nazwaOkregu, przedmiot.nazwa, MAX/MIN(wynik) FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia JOIN okregi ON okregi.idOkregu=uczniowie.idOkregu JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu  GROUP BY nazwaOkregu, przedmiot.nazwa;

//Osób niezdało z danego przedmiotu w danym województwie
//SELECT okregi.nazwaOkregu, przedmiot.nazwa, COUNT(DISTINCT pesel) FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia JOIN okregi ON okregi.idOkregu=uczniowie.idOkregu JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu WHERE wynik<30 GROUP BY nazwaOkregu, przedmiot.nazwa HAVING przedmiot.nazwa LIKE '%odstawo%';

//Ilość osób w danym województwie
//SELECT nazwaOkregu, COUNT(DISTINCT pesel) FROM uczniowie JOIN okregi ON uczniowie.idOkregu=okregi.idOkregu GROUP BY nazwaOkregu;

//Ilość Zdających z każdego przedmiotu
//SELECT przedmiot.nazwa, COUNT(DISTINCT iducznia) FROM oceny JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu GROUP BY przedmiot.nazwa ;


void exportXLSX::process(const QString pathToFile, exportXLSX::precision p)
{
	QXlsx::Document document;
	document.addSheet("Raport");

	if( p == LOW) __procLOW(document); else
	if( p == STANDARD)  __procSTAND(document); else
	if( p == MEDIUM) __procMED(document); else __procHIGH(document);

	document.saveAs(pathToFile);
}

void exportXLSX::__procLOW(QXlsx::Document & doc)
{
	db->open();

    Query zdalo{db->exec("SELECT (SELECT COUNT(DISTINCT pesel) FROM uczniowie) - COUNT(DISTINCT iducznia) FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu WHERE przedmiot.nazwa LIKE '%odstawo%' AND wynik<30;")};
    Query nieZdalo{db->exec("SELECT COUNT(DISTINCT iducznia) FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu WHERE przedmiot.nazwa LIKE '%odstawo%' AND wynik<30;")};
    Query maxWyn{db->exec("SELECT MAX(wynik) FROM oceny;")};
    Query minWyn{db->exec("SELECT MIN(wynik) FROM oceny;")};
    Query avgWyn{db->exec("SELECT AVG(wynik) FROM oceny;")};

	db->close();

	QXlsx::Format header;
	header.setFontBold(true);

	QXlsx::Format avg;
	avg.setNumberFormat("#0.00");

	doc.write(1, 1, "Opis", header);
	doc.write(2, 1, "Ilość osób, które zdała", header);
	doc.write(3, 1, "Ilość osób, które nie zdała", header);
	doc.write(4, 1, "Najlepszy wynik", header);
	doc.write(5, 1, "Najgorszy wynik", header);
	doc.write(6, 1, "Średni wynik", header);

	doc.write(1, 2, "Wartość", header);
    doc.write(2, 2, zdalo->data().front().front().toUInt());
    doc.write(3, 2, nieZdalo->data().front().front().toUInt());
    doc.write(4, 2, maxWyn->data().front().front().toInt());
    doc.write(5, 2, minWyn->data().front().front().toUInt());
    doc.write(6, 2, avgWyn->data().front().front().toDouble(), avg);
}

void exportXLSX::__procSTAND(QXlsx::Document & doc)
{
	doc.renameSheet(doc.sheetNames().first(), "Regiony");

	db->open();
    Query q{db->exec("SELECT uczniowie.idOkregu, MIN(wynik), MAX(wynik), AVG(wynik), COUNT(DISTINCT uczniowie.pesel)  FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia GROUP BY uczniowie.idOkregu;")};
	db->close();

	QXlsx::Format summary;
	summary.setFontBold(true);

	QXlsx::Format avg;
	avg.setNumberFormat("#0.00");

	doc.write(1, 1, "Nazwa Okręgu", summary);
	doc.write(1, 2, "Najniższy Wynik", summary);
	doc.write(1, 3, "Najwyższy Wynik", summary);
	doc.write(1, 4, "Średni Wynik", summary);
	doc.write(1, 5, "Ilość Zdających", summary);

	int row = 2;

    for( const auto& var : q->data())
	{
        doc.write(row, 1, MainWindow::sRegions[var[0].toULongLong()]);
        doc.write(row, 2, var[1].toUInt());
        doc.write(row, 3, var[2].toUInt());
        doc.write(row, 4, var[3].toDouble(), avg);
        doc.write(row, 5, var[4].toUInt());


        row++;
	}

	doc.write(row, 1, "Podsumowanie", summary);

	summary.setNumberFormat(avg.numberFormat());

	doc.write(row, 2, "=MIN(B2:B"+QString::number(row-1)+")", summary);
	doc.write(row, 3, "=MAX(C2:C"+QString::number(row-1)+")", summary);
	doc.write(row, 4, "=ŚREDNIA(D2:D"+QString::number(row-1)+")", summary);

	doc.addSheet("Przedmioty");
	doc.selectSheet("Przedmioty");

	row = 2;
    q->clear();

	db->open();
    q.reset(db->exec("SELECT idprzedmiotu, MIN(wynik), MAX(wynik), AVG(wynik), COUNT(DISTINCT iducznia) FROM oceny GROUP BY idprzedmiotu;"));
    Query zdalo{db->exec("SELECT (SELECT COUNT(DISTINCT pesel) FROM uczniowie) - COUNT(DISTINCT iducznia) FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu WHERE przedmiot.nazwa LIKE '%odstawo%' AND wynik<30;")};
    Query nieZdalo{db->exec("SELECT COUNT(DISTINCT iducznia) FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu WHERE przedmiot.nazwa LIKE '%odstawo%' AND wynik<30;")};
	db->close();

	summary.setNumberFormat("@");

	doc.write(1, 1, "Nazwa Przedmiotu", summary);
	doc.write(1, 2, "Najniższy Wynik", summary);
	doc.write(1, 3, "Najwyższy Wynik", summary);
	doc.write(1, 4, "Średni Wynik", summary);
	doc.write(1, 5, "Ilość Zdających", summary);

    for( const auto& var : q->data())
	{
        doc.write(row, 1, MainWindow::sSubjects[var[0].toULongLong()]);
        doc.write(row, 2, var[1].toUInt());
        doc.write(row, 3, var[2].toUInt());
        doc.write(row, 4, var[3].toDouble(), avg);
        doc.write(row, 5, var[4].toUInt());

		row++;
	}

	doc.write(row, 1, "Podsumowanie", summary);

	summary.setNumberFormat(avg.numberFormat());

	doc.write(row, 2, "=MIN(B2:B"+QString::number(row-1)+")", summary);
	doc.write(row, 3, "=MAX(C2:C"+QString::number(row-1)+")", summary);
	doc.write(row, 4, "=ŚREDNIA(D2:D"+QString::number(row-1)+")", summary);

	row+=3;

	doc.write(row, 1, "Ogółem zdało: ");
    doc.write(row, 2, zdalo->data().front().front().toUInt(), summary);

	row++;

	doc.write(row, 1, "Ogółem nie zdało: ");
    doc.write(row, 2, nieZdalo->data().front().front().toUInt(), summary);

	doc.selectSheet(doc.sheetNames().first());

}

void exportXLSX::__procMED(QXlsx::Document & doc)
{

	db->open();
    Query q{db->exec("SELECT uczniowie.idOkregu, przedmiot.idprzedmiot, AVG(wynik), MIN(wynik), MAX(wynik) FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu  GROUP BY uczniowie.idOkregu, przedmiot.idprzedmiot;")};
    Query zdalo{db->exec("SELECT (SELECT COUNT(DISTINCT pesel) FROM uczniowie) - COUNT(DISTINCT iducznia) FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu WHERE przedmiot.nazwa LIKE '%odstawo%' AND wynik<30;")};
    Query nieZdalo{db->exec("SELECT COUNT(DISTINCT iducznia) FROM oceny JOIN uczniowie ON uczniowie.pesel=oceny.iducznia JOIN przedmiot ON przedmiot.idprzedmiot=oceny.idprzedmiotu WHERE przedmiot.nazwa LIKE '%odstawo%' AND wynik<30;")};
	db->close();

	QXlsx::Format summary;
	summary.setFontBold(true);

	doc.write(1, 1, "Nazwa Okręgu", summary);
	doc.write(1, 2, "Nazwa Przedmiotu", summary);
	doc.write(1, 3, "Najniższy Wynik", summary);
	doc.write(1, 4, "Najwyższy Wynik", summary);
	doc.write(1, 5, "Średni Wynik", summary);

	int row = 2;

	QXlsx::Format avg;
	avg.setNumberFormat("#0.00");

    for(const auto& var : q->data())
	{
        doc.write(row, 1, MainWindow::sRegions[var[0].toULongLong()]);
        doc.write(row, 2, MainWindow::sSubjects[var[1].toULongLong()]);
        doc.write(row, 3, var[3].toUInt());
        doc.write(row, 4, var[4].toUInt());
        doc.write(row, 5, var[2].toDouble(), avg);

		row++;
	}

	doc.mergeCells(QXlsx::CellRange(row, 1, row, 2), summary);
	doc.write(row, 1, "Podsumowanie: ");

	summary.setNumberFormat("#0.00");

	doc.write(row, 3, "=MIN(C2:C"+QString::number(row-1)+")", summary);
	doc.write(row, 4, "=MAX(D2:D"+QString::number(row-1)+")", summary);
	doc.write(row, 5, "=ŚREDNIA(E2:E"+QString::number(row-1)+")", summary);

	row+=3;

	doc.write(row, 1, "Ogółem zdało: ");
    doc.write(row, 2, zdalo->data().front().front().toUInt(), summary);

	row++;

	doc.write(row, 1, "Ogółem nie zdało: ");
    doc.write(row, 2, nieZdalo->data().front().front().toUInt(), summary);

}

void exportXLSX::__procHIGH(QXlsx::Document & doc)
{

	db->open();
    Query q{db->exec("SELECT o.iducznia, (SELECT COUNT(*) FROM oceny o1 WHERE o1.iducznia=o.iducznia ) ,idOkregu, idprzedmiotu, ANY_VALUE(wynik) FROM oceny o JOIN uczniowie ON pesel=iducznia GROUP BY iducznia, idprzedmiotu ORDER BY iducznia ASC;")};

	QXlsx::Format header;
	header.setFontBold(true);

	QXlsx::Format merged;
	merged.setHorizontalAlignment(QXlsx::Format::HorizontalAlignment::AlignHCenter);
	merged.setVerticalAlignment(QXlsx::Format::VerticalAlignment::AlignVCenter);
	merged.setNumberFormat("@");

	QXlsx::Format nums;
	nums.setNumberFormat("#0.00");

	const QString sheet1 = doc.sheetNames().front();
	const QString sheet2 = "dataAnalizeFriendly";

	doc.write(1, 1, "PESEL zdającego", header);
	doc.write(1, 2, "Okręg zdającego", header);
	doc.write(1, 3, "Przedmiot zdawany", header);
	doc.write(1, 4, "Wynik [%]", header);

	doc.addSheet(sheet2);

	doc.write(1, 1, "PESEL zdającego", header);
	doc.write(1, 2, "Okręg zdającego", header);
	doc.write(1, 3, "Przedmiot zdawany", header);
	doc.write(1, 4, "Wynik [%]", header);

	doc.selectSheet(sheet1);

	int row = 2;
	QString pes = "";

    for(const auto& var : q->data())
	{
		doc.selectSheet(sheet1);

        if(var[0] != pes)
		{
            int counter = var[1].toInt();

			doc.mergeCells(QXlsx::CellRange(row, 1, row+counter - 1, 1));
			doc.mergeCells(QXlsx::CellRange(row, 2, row+counter - 1, 2));

            doc.write(row, 1, var[0], merged);
            doc.write(row, 2, MainWindow::sRegions[var[2].toULongLong()], merged);
		}

        doc.write(row, 3, MainWindow::sSubjects[var[3].toULongLong()]);
        doc.write(row, 4, var[4].toInt(), nums);

		doc.selectSheet(sheet2);

        doc.write(row, 1, var[0]);
        doc.write(row, 2, MainWindow::sRegions[var[2].toULongLong()]);
        doc.write(row, 3, MainWindow::sSubjects[var[3].toULongLong()]);
        doc.write(row, 4, var[4].toInt(), nums);

		row++;

	}

	doc.selectSheet(sheet1);

}
