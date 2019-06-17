#pragma once

#include "xlsxdocument.h"

#include "MySQLSimpleDriver.hpp"

using Query = std::shared_ptr<querryResult>;

class exportXLSX
{
public:

    mysqldriver * db;

	enum precision
	{
		LOW = 1,
		STANDARD = 2,
		MEDIUM = 3,
		HIGH = 4
	};

    exportXLSX(mysqldriver * _db) :db{_db} {}

	~exportXLSX() = default;

	void process(const QString pathToFile, precision p = STANDARD);

private:

	void __procLOW(QXlsx::Document& doc);

	void __procSTAND(QXlsx::Document& doc);

	void __procMED(QXlsx::Document& doc);

	void __procHIGH(QXlsx::Document& doc);

};
