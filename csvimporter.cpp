#include "csvimporter.h"

void CSVImporter::exec()
{
	std::ifstream input(__mPath.toStdString());
	std::string line;
	std::list<QString> processedData;

	while(std::getline(input, line))
	{
		__resolve(line, processedData);
		__sendSQL(processedData);
	}

	input.close();
}

void CSVImporter::__sendSQL(const std::list<QString> & src) noexcept
{
	QString prep = "INSERT INTO ";
	switch(static_cast<quint8>(__mType))
	{
		case Egzaminy: prep+="egzamin(idprzedmiotu, data)"; break;
		case Zespoly: prep+="zespoly"; break;
		case Zdajacy: prep+="uczniowie"; break;
		case Pracownicy: prep+="pracownicy"; break;
	}
	prep+=" VALUES(";
	for(auto var : src)
	{
		prep += "'" + var + "'";
		if(var != src.back()) prep+=", ";
	}
	prep+=");";

	db->open();
    Query{db->exec(prep)};
	db->close();
}

void CSVImporter::__resolve(const std::string & src, std::list<QString> & dst) const noexcept
{
	dst.clear();
	dst.push_back("");

	for(auto var : src)
	{
		if(var == ';') { dst.push_back("");	continue; }
		dst.back()+=var;
	}
}
