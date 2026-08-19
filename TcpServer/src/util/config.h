
#pragma once

#include <iostream>

class Config 
{
public:
    static const std::string& GetProgramName();
    static const std::string& GetProgramVersion();

    static const std::string& GetLogFile();
    static const std::string& GetLogPrefix();

    static const std::string& GetIniFile();

private:
    
    static const std::string ProgramName;
    static const std::string ProgramVersion;

    static const std::string LogFile;
    static const std::string LogPrefix;

    static const std::string IniFile;
};