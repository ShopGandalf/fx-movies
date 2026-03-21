#include "src/app/cli.h"
#include "src/app/application.h"
#include "src/asset/asset_scanner.h"
#include <iostream>

int main(int argc, char* argv[]) {
    auto cli_result = fx::parse_args(argc, argv);
    if (!cli_result.success) {
        std::cerr << cli_result.error_message << std::endl;
        return cli_result.exit_code;
    }

    auto assets = fx::scan_directory(cli_result.directory_path);

    std::string title = "fx-movies - " + cli_result.directory_path.string();

    fx::Application app;
    if (!app.init(title)) {
        std::cerr << "Error: Failed to initialize application window" << std::endl;
        return 1;
    }

    app.set_assets(std::move(assets));
    app.run();
    app.shutdown();

    return 0;
}
