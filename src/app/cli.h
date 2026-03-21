#pragma once

#include <filesystem>
#include <string>

namespace fx {

struct CliResult {
    bool success = false;
    int exit_code = 0;
    std::filesystem::path directory_path;
    std::string error_message;
};

CliResult parse_args(int argc, char* argv[]);

} // namespace fx
