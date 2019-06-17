#include "MySQLSimpleDriver.hpp"

std::ostream *mysqldriver::__mLogDestination = nullptr;

mysqldriver::mysqldriver(const std::string &iHost,
                         const std::string &iUsername,
                         const std::string &iPassword,
                         const std::string &iDataBaseName,
                         const portNumber iPort,
                         std::ostream *iLogOutput,
                         bool *connectionEstablished) noexcept
    : __mHostName(iHost), __mUsername(iUsername),
      __mPassword(iPassword), __mDataBaseName(iDataBaseName),
      __mPort{iPort}
{
    if (iLogOutput != nullptr)
        __mLogDestination = iLogOutput;
    __log("Trying to initialize connection with database...");
    bool iFlag = false;
    //__mConnection.reset(std::make_unique<MYSQL>());
    if (mysql_real_connect(__mConnection, iHost.c_str(), iUsername.c_str(), iPassword.c_str(), iDataBaseName.c_str(), iPort, NULL, 0) == NULL)
    {
        __log("Connection failed.", logType::ERROR);
    }
    else
    {
        __log("Connection established.");
        iFlag = true;
    }

    __log("Killing test connection");
    mysql_close(__mConnection);

    __log("Setting flags");
    if (connectionEstablished != nullptr)
        (*connectionEstablished) = iFlag;

    __log("Finishing object construction");
    iLogOutput = nullptr;
    connectionEstablished = nullptr;
    return;
}

mysqldriver::~mysqldriver()
{
    if (__mConnection != nullptr)
    {

        if (__mIsConnected)
        {
            close();
        }

        __log("Object has been destroyed.");
        __mLogDestination = nullptr;
    }
}

void mysqldriver::__log(const std::string &src, const logType type) noexcept
{
    if (__mLogDestination == nullptr)
        return;
    std::chrono::system_clock::time_point iCurrTime = std::chrono::system_clock::now();
    const std::time_t iTempTimeVarriable = std::chrono::system_clock::to_time_t(iCurrTime);
    std::string iLogPrefix{std::ctime(&iTempTimeVarriable)};
    auto it = iLogPrefix.end();
    it--;
    iLogPrefix = iLogPrefix.replace(it, iLogPrefix.end(), "");

    if (type == logType::INFO)
        iLogPrefix = "[INFO] " + iLogPrefix;
    else if (type == logType::WARNING)
        iLogPrefix = "[WARNING] " + iLogPrefix;
    else
        iLogPrefix = "[ERROR] " + iLogPrefix;

    iLogPrefix += ": " + src;
    (*__mLogDestination) << iLogPrefix
                         << std::endl;
}

bool mysqldriver::open()
{
    try
    {
        __possiableRead();
    }
    catch (...)
    {
        return false;
    }

    __mConnection = mysql_init(NULL);
    if (__mConnection == NULL || mysql_real_connect(
                __mConnection, __mHostName.c_str(),
                __mUsername.c_str(), __mPassword.c_str(),
                __mDataBaseName.c_str(), __mPort, NULL, 0) == NULL)
    {
        __log("Database occured error during establishing connection. Connection Failed.", logType::ERROR);
        try
        {
            throw mySqlException("Database occured error during establishing connection. Connection Failed.");
        }
        catch (...)
        {
            return  false;
        };
    }
    else
        __mIsConnected = true;

    return true;
}

querryResult *mysqldriver::exec(const QString &src1)
{
    const std::string src = src1.toStdString();
    try
    {
        __canModify();
    }
    catch (...)
    {
        __log("Tries execute querry, when database not opened", logType::ERROR);
        throw;
    }

    __log("Executing querry: " + src, logType::INFO);

    if (mysql_query(__mConnection, src.c_str()))
    {
        __log("Something goes wrong. Check your querry and database connection", logType::ERROR);
        try
        {
            throw   std::invalid_argument("Something goes wrong. Check your querry and database connection.\nQuerry: "+src1.toLatin1());
        }
        catch (...)
        {
            throw;
        }
    }

    return new querryResult(mysql_store_result(__mConnection));
}

void mysqldriver::close() noexcept
{
    if (!__mIsConnected)
        return;
    try
    {
        mysql_close(__mConnection);
    }
    catch (...)
    {
        __log("Something goes wrong, but database is closed.", logType::WARNING);
    }

    __mIsConnected = false;
}

bool mysqldriver::testConnection() noexcept
{
    try
    {
        open();
    }
    catch (const std::exception &e)
    {
        __log("Connection test failed.", logType::WARNING);
        return false;
    }

    close();
    return true;
}

std::string mysqldriver::HostName() const noexcept
{
    return __mHostName;
}

std::string mysqldriver::Username() const noexcept
{
    return __mUsername;
}

std::string mysqldriver::Password() const noexcept
{
    return __mPassword;
}

std::string mysqldriver::DataBase() const noexcept
{
    return __mDataBaseName;
}

portNumber mysqldriver::Port() const noexcept
{
    return __mPort;
}

/*
std::ostream const * mysqldriver::LogDestination() const
{
        if(__mLogDestination == nullptr)
        {
                try{ throw std::bad_function_call();
        }catch(...) { throw; }

        return static_cast<std::ostream const *>(__mLogDestination.get());
}

void mysqldriver::LogDestination(std::ostream * src)
{
        __mLogDestination = std::make_shared<std::ostream>(src);
}
*/

void mysqldriver::HostName(const std::string &src)
{
    try
    {
        __possiableRead();
        if (__mIsConnected == true)
            try
        {
            throw std::bad_function_call();
        }
        catch (...)
        {
            throw;
        }
    }
    catch (...)
    {
        throw;
    }

    if (src == "" || src.find(" ") >= 0)
        try
    {
        __log("Hostname can't consist of spaces", logType::ERROR);
        throw std::invalid_argument("Hostname can't consist of spaces");
    }
    catch (const std::invalid_argument &e)
    {
        throw;
    }

    __mHostName = src;
    __log("Hostname has been set to: " + src);
}

void mysqldriver::Username(const std::string &src)
{
    try
    {
        __possiableRead();
        if (__mIsConnected == true)
            try
        {
            throw std::bad_function_call();
        }
        catch (...)
        {
            throw;
        }
    }
    catch (...)
    {
        throw;
    }

    if (src == "" || src.find(" ") >= 0)
        try
    {
        __log("Username can't consist of spaces", logType::ERROR);
        throw std::invalid_argument("Username can't consist of spaces");
    }
    catch (const std::invalid_argument &e)
    {
        throw;
    }

    __mUsername = src;
    __log("Username has been set to: " + src);
}

void mysqldriver::Password(const std::string &src)
{
    try
    {
        __possiableRead();
        if (__mIsConnected == true)
            try
        {
            throw std::bad_function_call();
        }
        catch (...)
        {
            throw;
        }
    }
    catch (...)
    {
        throw;
    }

    if (src == "" || src.find(" ") >= 0)
        try
    {
        __log("Password can't consist of spaces", logType::ERROR);
        throw std::invalid_argument("Password can't consist of spaces");
    }
    catch (const std::invalid_argument &e)
    {
        throw;
    }

    __mPassword = src;
    __log("Password has been set to: " + src);
}

void mysqldriver::DataBase(const std::string &src)
{
    try
    {
        __possiableRead();
        if (__mIsConnected == true)
            try
        {
            throw std::bad_function_call();
        }
        catch (...)
        {
            throw;
        }
    }
    catch (...)
    {
        throw;
    }

    if (src == "" || src.find(" ") >= 0)
        try
    {
        __log("Database name can't consist of spaces", logType::ERROR);
        throw std::invalid_argument("Database name can't consist of spaces");
    }
    catch (const std::invalid_argument &e)
    {
        throw;
    }

    __mDataBaseName = src;
    __log("Database name has been set to: " + src);
}

void mysqldriver::Port(const portNumber src)
{
    try
    {
        __possiableRead();
        if (__mIsConnected == true)
            try
        {
            throw std::bad_function_call();
        }
        catch (...)
        {
            throw;
        }
    }
    catch (...)
    {
        throw;
    }

    if (src == 0)
        try
    {
        __log("Port has to be in range: < 1; " + std::to_string(std::numeric_limits<portNumber>::max()), logType::ERROR);
        throw std::invalid_argument("Port has to be in range: < 1; " + std::to_string(std::numeric_limits<portNumber>::max()));
    }
    catch (const std::invalid_argument &e)
    {
        throw;
    }

    __mPort = src;
    __log("Port has been set to: " + std::to_string(src));
}

void mysqldriver::__canModify() const
{
    try
    {
        __possiableRead();
    }
    catch (...)
    {
        throw;
    }
    if (__mIsConnected == false)
        try
    {
        throw std::bad_function_call();
    }
    catch (...)
    {
        throw;
    }
}

querryResult::querryResult(MYSQL_RES *src)
{
    if (src == NULL || src == nullptr)
    {
        mysqldriver::__log("Empty querry result.", mysqldriver::logType::WARNING);
        return;
    }

    size_t num_fields = mysql_num_fields(src);
    mysqldriver::__log("Amount of columns: " + std::to_string(num_fields));
    mysqldriver::__log("Resolving headers... ");
    while (MYSQL_FIELD *field = mysql_fetch_field(src))
    {
        std::string tmp{field->name};
        __mHeaders.push_back(tmp.c_str());
        mysqldriver::__log(__mHeaders.back().toStdString());
    }

    while (MYSQL_ROW row = mysql_fetch_row(src))
    {
        __mData.push_back(std::vector<QString>());
        for (int i = 0; i < num_fields; i++)
            __mData.back().push_back(row[i]);
    }

    mysql_free_result(src);

    mysqldriver::__log("Finishing: Setting last varriables");
    amountOfColumns = __mHeaders.size();
    amountOfRows = __mData.size();

    mysqldriver::__log("Finished");
}

/*
bool querryResult::next(std::list<std::string>* dst)
{
        if (__mData.size() != 0)
        {
                if(__mData.size() != amountOfRows) __mData.pop();
                mysqldriver::__log("Gathering next element from result");
                dst = __mData.front();
                return true;
        }
        else return false;
}
*/

void querryResult::clear() noexcept
{
    __mHeaders.clear();

    amountOfColumns = 0;
    amountOfRows = 0;
}

size_t querryResult::size() const noexcept { return amountOfRows; }

const std::list<QString> &querryResult::Headers() const
{
    return __mHeaders;
}

void mysqldriver::__possiableRead() const
{
    if (__mConnection == nullptr)
        try
    {
        throw std::bad_function_call();
    }
    catch (...)
    {
        throw;
    }
}
