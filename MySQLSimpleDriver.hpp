#ifndef CARD_H
#define CARD_H
#pragma once

#include <mysql/mysql.h>
#include <mysql/my_global.h>

#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <stdexcept>
#include <limits>
#include <list>
#include <queue>
#include <vector>

#include <QDateTime>
#include <QTimeZone>
#include <boost/tokenizer.hpp>

using portNumber = uint16_t;
using tokenizer = boost::tokenizer<boost::char_separator<char>>;

class querryResult final
{
private:
    std::list<std::vector<QString>> __mData;
    std::list<QString> __mHeaders;
    size_t amountOfColumns = 0, amountOfRows = 0;

public:
    querryResult(MYSQL_RES *src);
    querryResult(const querryResult &src) = delete;
    querryResult(querryResult &&src) = delete;

    //bool next(std::list<std::string>* dst);
    const std::list<std::vector<QString>>& data() { return __mData; }

    void clear() noexcept;

    size_t size() const noexcept;

    const std::list<QString> &Headers() const;

};

class mysqldriver final
{
private:
    MYSQL *__mConnection = mysql_init(NULL);
    bool __mIsConnected = false;

    std::string __mHostName, __mUsername, __mPassword, __mDataBaseName;
    portNumber __mPort;

    static std::ostream *__mLogDestination;

    enum logType
    {
        INFO,
        WARNING,
        ERROR
    };

public:
    mysqldriver(
        const std::string &iHost,
        const std::string &iUsername,
        const std::string &iPassword,
        const std::string &iDataBaseName,
        const portNumber iPort = 3306,
        std::ostream *iLogOutput = nullptr,
        bool *connectionEstablished = nullptr) noexcept;

    ~mysqldriver();

    bool open();
    querryResult *exec(const QString &src1);
    void close() noexcept;
    bool testConnection() noexcept;

    //Getters
    std::string HostName() const noexcept;
    std::string Username() const noexcept;
    std::string Password() const noexcept;
    std::string DataBase() const noexcept;
    portNumber Port() const noexcept;
    //std::ostream const * LogDestination() const;

    //Setters
    void HostName(const std::string &src);
    void Username(const std::string &src);
    void Password(const std::string &src);
    void DataBase(const std::string &src);
    void Port(const portNumber src);
    //void LogDestination(std::ostream * src);

private:
    static void __log(const std::string &src, const logType type = logType::INFO) noexcept;
    void __canModify() const;
    void __possiableRead() const;

    friend class querryResult;
};

class mySqlException : public std::exception
{
    std::string msg_;

public:
    mySqlException() : msg_{""} {};
    mySqlException(const mySqlException &) = default;
    mySqlException(mySqlException &&) = default;

    explicit mySqlException(const std::string &msg) : msg_(msg) {}

    virtual ~mySqlException() throw() {}

    virtual const char *what() const throw()
    {
        return msg_.c_str();
    }
};

inline QDateTime str2date(const QString& src1)
{    
    std::string src = src1.toStdString();
    const boost::char_separator<char> sep{R"( :-)"};
    tokenizer values(src, sep);
    auto it = values.begin();
    return ((src1.length() > 11) ? QDateTime(
                QDate(
                    QString((*it).c_str()).toInt(),
                    QString((*(it++)).c_str()).toInt(),
                    QString((*(it++)).c_str()).toInt()
                    ),
                QTime(
                    QString((*(it++)).c_str()).toInt(),
                    QString((*(it++)).c_str()).toInt(),
                    QString((*(it++)).c_str()).toInt()
                    ),
                    QTimeZone::systemTimeZone()
                ) : QDateTime(
                                       QDate(
                                           QString((*it).c_str()).toInt(),
                                           QString((*(it++)).c_str()).toInt(),
                                           QString((*(it++)).c_str()).toInt()
                                           )));
}

#endif
