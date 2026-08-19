#include "config.h"

#include "defines.h"

//------------
// Private values
//------------

const std::string Config::ProgramName = PROGRAM_NAME;
const std::string Config::ProgramVersion = PROGRAM_VERSION;

const std::string Config::LogPrefix = PROGRAM_NAME;

const std::string Config::LogFile = "tcpserver.log";

const std::string Config::IniFile = "tcpserver.ini";

//------------
// Public values
//------------

/**
 * 
 * @return The program name
 */
const std::string& Config::GetProgramName()
{
    return Config::ProgramName;
}

/**
 * 
 * @return The program version
 */
const std::string& Config::GetProgramVersion()
{
    return Config::ProgramVersion;
}

/**
 * 
 * @return The Log prefix of the application
 */
const std::string& Config::GetLogPrefix()
{
    return Config::LogPrefix;
}

/**
 * 
 * @return The config file name
 */
const std::string& Config::GetLogFile()
{
    return Config::LogFile;
}

/**
 * 
 * @return The ini file name
 */
const std::string& Config::GetIniFile()
{
    return Config::IniFile;
}