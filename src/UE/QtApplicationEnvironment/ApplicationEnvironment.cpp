#include <ApplicationEnvironment.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>
#include "Messages.hpp"

namespace ue
{

namespace
{

std::string getPhoneNumberPrefix(PhoneNumber const& phoneNumber)
{
    return " [phone:" + to_string(phoneNumber) + "]";
}

std::string logFilename(PhoneNumber phoneNumber)
{
    // Check if the environment variable DO_NOT_LOG_TO_FILE is set to "1" and if so
    // return a null device path to avoid automatic creation of log file by a subsequent
    // logger initialization
    // This is to prevent pollution of filesystem with log files
    const char *doNotLogEnv = std::getenv("DO_NOT_LOG_TO_FILE");
    if (doNotLogEnv && std::string(doNotLogEnv) == "1")
    {
        // Using std::cerr to indicate that logging to file is disabled, explicitly without
        // relying on the logger itself
        std::cerr << "Note: DO_NOT_LOG_TO_FILE is set to 1, no log file will be created."
                  << std::endl;

#ifdef _WIN32
        return "NUL";
#else
        return "/dev/null";
#endif
    }
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto localNow = localtime(&now);
    char timeBuff[20];
    strftime(timeBuff, sizeof(timeBuff), "%Y%m%d%H%M%S", localNow);

    std::ostringstream os;
    os << "ue" << phoneNumber << "_syslog_" << timeBuff << ".txt";
    return os.str();
}

} // namespace

ApplicationEnvironment::ApplicationEnvironment(int& argc, char* argv[])
    : configuration(ApplicationEnvironment::readConfiguration(argc, argv)),
      myPhoneNumber(PhoneNumber{configuration->getNumber<decltype(PhoneNumber::value)>("phone", 123)}),
      logFile(logFilename(myPhoneNumber)),
      loggerBase(logFile),
      logger(loggerBase, getPhoneNumberPrefix(myPhoneNumber)),
      qApplication(argc, argv),
      gui(logger),
      transport(*configuration, logger)
{
}

ue::IUeGui& ApplicationEnvironment::getUeGui()
{
    return gui;
}

ue::ITransport& ApplicationEnvironment::getTransportToBts()
{
    return transport;
}

ILogger &ApplicationEnvironment::getLogger()
{
    return logger;
}

void ApplicationEnvironment::startMessageLoop()
{
    gui.start();
    qApplication.exec();
}

std::unique_ptr<common::MultiLineConfig> ApplicationEnvironment::readConfiguration(int argc, char *argv[])
{
    auto commandLineConfig = std::make_unique<common::MultiLineConfig>(argc - 1, argv + 1);

    std::string configFile = commandLineConfig->getString("config", "config");

    try
    {
        std::ifstream configStream;
        configStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        configStream.open(configFile);

        common::MultiLineConfig fileConfig(configStream);
        commandLineConfig->insertFrom(fileConfig);
    }
    catch (...)
    {
        std::clog << "Note: config file: \"" << configFile << "\" is not present or reading failure.\n\t((only command line arguments are used))" << std::endl;
    }
    return commandLineConfig;
}

PhoneNumber ApplicationEnvironment::getMyPhoneNumber() const
{
    return myPhoneNumber;
}

std::int32_t ApplicationEnvironment::getProperty(std::string const& name, std::int32_t defaultValue) const
{
    return configuration->getNumber<std::int32_t>(name, defaultValue);
}

}
