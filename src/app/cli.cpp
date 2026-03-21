#include "src/app/cli.h"
#include <iostream>

namespace fx {

CliResult parse_args(int argc, char* argv[]) {
    if (argc < 2) {
        return {
            .success = false,
            .exit_code = 1,
            .directory_path = {},
            .error_message = "Error: No directory path provided. Usage: fx_movies <directory_path>"
        };
    }

    std::filesystem::path path(argv[1]);

    if (!std::filesystem::exists(path)) {
        return {
            .success = false,
            .exit_code = 2,
            .directory_path = {},
            .error_message = "Error: Path does not exist: " + path.string()
        };
    }

    if (!std::filesystem::is_directory(path)) {
        return {
            .success = false,
            .exit_code = 2,
            .directory_path = {},
            .error_message = "Error: Path is not a directory: " + path.string()
        };
    }

    return {
        .success = true,
        .exit_code = 0,
        .directory_path = std::filesystem::canonical(path),
        .error_message = {}
    };
}

} // namespace fx
