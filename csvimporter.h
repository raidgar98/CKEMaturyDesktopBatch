#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <memory>

#include <QString>

#include "MySQLSimpleDriver.hpp"

using Query = std::shared_ptr<querryResult>;

class CSVImporter
{
public:

	enum Tryb
	{
		Egzaminy = 0,
		Zespoly,
		Zdajacy,
		Pracownicy
	};

    CSVImporter(const Tryb tryb, const QString& pathToFile, mysqldriver * _db)
		:__mType{tryb}, __mPath{pathToFile}, db{_db} {}

	~CSVImporter()
	{
		db = nullptr;
	}

	void exec();

	void operator()() noexcept
	{
		exec();
	}

private:

	const Tryb __mType;
	const QString __mPath;
    mysqldriver * db = nullptr;

	void __sendSQL(const std::list<QString>& src) noexcept;

	void __resolve(const std::string& src, std::list<QString>& dst) const noexcept;

};
