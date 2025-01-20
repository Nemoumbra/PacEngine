#include <iostream>
#include <vector>
#include <filesystem>
#include <optional>
#include <fstream>
#include <string>

#include "../src/context/pac_context.h"
#include "../src/controllers/default_controller.h"
#include "../src/controllers/custom/script_controller.h"

#include "../src/debugger/debugger.h"

namespace fs = std::filesystem;


fs::path ask_path() {
    std::cout << "Enter the PAC filename\n";

    std::string line;
    std::getline(std::cin, line);

    fs::path path = line;
    return path;
}

std::optional<std::vector<uint8_t>> read_file(const fs::path& path) {
    std::ifstream source(path, std::ios::binary);
    if (!source) {
        std::cout << "Error! Cannot access " << path << "\n";
        return {};
    }

    auto start = std::istreambuf_iterator<char>(source);
    auto end = std::istreambuf_iterator<char>();

    std::vector<uint8_t> source_data(start, end);
    return source_data;
}

std::optional<std::vector<uint8_t>> get_example_file() {
    std::vector<uint8_t> data = {
            0x00, 0x00, 0x00, 0x00, 0x25, 0x00, 0x02, 0x00, 0x10, 0x00, 0x00, 0x00, 0x25, 0x99, 0x99, 0x99,
            0x25, 0x00, 0x03, 0x00, 0x30, 0x00, 0x00, 0x00, 0x25, 0x10, 0x06, 0x00, 0x04, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x25, 0x00, 0x02, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x25, 0x10, 0x01, 0x00, 0x25, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x25, 0x00, 0x01, 0x00,
    };
    return data;
}

int main() {
    PacGlobalRuntime runtime{0x200, 0x200};
    PacContext ctx{0x48, 0x48, 1};

    DefaultController sec_0x0;
    ScriptController sec_0x10{&ctx};


    ctx.set_runtime(&runtime);
    ctx.register_controller(&sec_0x0);
    ctx.register_controller(&sec_0x10);
    ctx.set_logging_settings(0xffffffff);

    // const auto path = ask_path();
    // auto pac_data = read_file(path);
    auto pac_data = get_example_file();

    if (!pac_data.has_value()) {
        std::cout << "Failed to read the file!";
        return 1;
    }
    auto& pac_bytes = pac_data.value();
    auto pac_start = reinterpret_cast<char*>(pac_bytes.data());
    auto pac_size = pac_bytes.size();

    constexpr uint32_t virtual_address = 0x08804000;
    Debugger::remapper.RegisterFile(pac_start, pac_size, virtual_address);

    ctx.set_starting_pc(pac_start);

    ctx.run(1);
    std::cout << "Exited!\n";

    auto locals = ctx.get_locals();
    auto reg_0x0 = static_cast<uint32_t>(locals->get_int_reg(0));
    std::cout << std::format("Local reg 0x0: expected 0x{:x}, found 0x{:x}", virtual_address, reg_0x0);
}
