#include "toml_handler.h"

#include "file_functions.h"
#include "defines.h"

#include <toml++/toml.hpp>

TomlHandler::TomlHandler()
{
    
}

/**
 * Toml testing with Tomlplusplus
 *
 * https://github.com/marzer/tomlplusplus
 */
void TomlHandler::TestTomlFile()
{
    FileFunctions &fileFunctions = FileFunctions::getInstance();
    toml::v3::ex::parse_result config;

    std::string configFile = fileFunctions.GetWorkingDirectory() + "/config.toml";    

    try
    {
        // First, make sure the file exists.
        if (!fileFunctions.FileExists(configFile))
        {
            log_output("Error: Toml file doesn't exist.");
            return;
        }

        config = toml::parse_file(configFile);
    }
    catch (toml::v3::ex::parse_error)
    {
        log_output("Error: toml failed to parse file");
        return;
    }

    // get key-value pairs
    std::string_view library_name = config["library"]["name"].value_or("");
    std::string_view library_author = config["library"]["authors"][0].value_or("");
    int64_t depends_on_cpp_version = config["dependencies"]["cpp"].value_or(0);

    // modify the data
    config.insert_or_assign("alternatives", toml::array{
                                                "cpptoml",
                                                "toml11",
                                                "Boost.TOML"});

    // use a visitor to iterate over heterogenous data
    config.for_each([](auto &key, auto &value)
                    {
                        std::cout << value << "\n";

                        // if constexpr (toml::is_string<decltype(value)>)
                        // do_something_with_string_values(value);
                        // });
                    });

    // you can also iterate more 'traditionally' using a ranged-for
    // for (auto &&[k, v] : config)
    // {
    //     // ...
    // }

    // re-serialize as TOML
    // std::cout << config << "\n";

    // re-serialize as JSON
    // std::cout << toml::json_formatter{config} << "\n";

    // re-serialize as YAML
    // std::cout << toml::yaml_formatter{config} << "\n";
}