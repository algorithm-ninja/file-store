#include "file_manager.hpp"
#include <libgen.h>
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " file_root file ... file dest\n" << std::endl;
        return 1;
    }
    std::string file_root = argv[1];
    std::string dest = argv[argc-1];
    for (int i=2; i<argc-1; i++) {
        std::string src = argv[i];
        std::string real_dest = dest;
        if (argc > 4) {
            real_dest += "/";
            real_dest += basename(argv[i]);
        }
        try {
            copy_file_to_storage(file_root, real_dest, src);
        } catch (OSError& e) {
            std::cerr << "Error copying " << src << ": " << e.what() << std::endl;
        }
    }
}
