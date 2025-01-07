#include <iostream>
#include <vector>
#include <filesystem>
#include <optional>
#include <fstream>
#include <string>

#include "../src/context/pac_context.h"
#include "../src/controllers/default_controller.h"
#include "../src/controllers/custom/script_controller.h"

namespace fs = std::filesystem;

std::optional<std::vector<char>> read_file(const fs::path& path) {
    std::ifstream source(path, std::ios::binary);
    if (!source) {
        std::cout << "Error! Cannot access " << path << "\n";
        return {};
    }

    auto start = std::istreambuf_iterator<char>(source);
    auto end = std::istreambuf_iterator<char>();

    std::vector <char> source_data(start, end);
    return source_data;
}

fs::path ask_path() {
    std::cout << "Enter the PAC filename\n";

    std::string line;
    std::getline(std::cin, line);

    fs::path path = line;
    return path;
}


int main() {
    PacGlobalRuntime runtime{0x200, 0x200};
    PacContext ctx{0x48, 0x48, 1};
    DefaultController sec_0x0;
    ScriptController sec_0x10;


    ctx.set_runtime(&runtime);
    ctx.register_controller(&sec_0x0);
    ctx.register_controller(&sec_0x10);

    const auto path = ask_path();
    auto pac_data = read_file(path);

    if (!pac_data.has_value()) {
        std::cout << "Failed to read the file!";
        return 1;
    }

    ctx.set_starting_pc(pac_data.value().data());

    ctx.run(1);
}
