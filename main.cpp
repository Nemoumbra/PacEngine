#include <iostream>

#include "src/context/pac_context.h"


int main() {
    PacGlobalRuntime runtime{0x200, 0x200};
    PacContext ctx{0x48, 0x48, 1};
}
