#include <assert.h>
#define NOB_IMPLEMENTATION
#include "nob.h"

#define MUST(X) do { if (!X) return false; } while (0)

#define BUILD_FOLDER ".build/"

bool compile(Nob_Cmd *cmd);

bool run(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "nob.h");

    MUST(nob_mkdir_if_not_exists(BUILD_FOLDER));

    Nob_Cmd cmd = {0};

    MUST(compile(&cmd));

    nob_cmd_append(&cmd, "./"BUILD_FOLDER"main");
    MUST(nob_cmd_run(&cmd));

    return true;
}

int main(int argc, char **argv) {
    return !run(argc, argv);
}

bool compile(Nob_Cmd *cmd) {
    nob_cc(cmd);
    nob_cc_flags(cmd);
    nob_cmd_append(cmd, "-c");
    nob_cc_inputs(cmd, "main.c");
    nob_cc_output(cmd, BUILD_FOLDER"main.o");

    MUST(nob_cmd_run(cmd));

    // nob_cc(cmd);
    // nob_cc_flags(cmd);
    // nob_cmd_append(cmd, "-c", "-x", "c", "-DNOB_IMPLEMENTATION");
    // nob_cc_inputs(cmd, "nob.h");
    // nob_cc_output(cmd, BUILD_FOLDER"nob.o");

    // MUST(nob_cmd_run(cmd));

    nob_cc(cmd);
    nob_cc_inputs(cmd, BUILD_FOLDER"main.o");
    nob_cc_output(cmd, BUILD_FOLDER"main");
    nob_cmd_append(cmd);

    MUST(nob_cmd_run(cmd));

    return true;
}
