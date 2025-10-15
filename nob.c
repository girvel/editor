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

// Nob_String_Builder _builder = {0};
// char *src_to_obj(char *path) {
//     _builder.count = 0;
// 
//     nob_sb_append_cstr(&_builder, BUILD_FOLDER);
//     nob_sb_append_cstr(&_builder, path);
// 
//     assert(_builder.items[_builder.count - 2] == '.');
//     assert(_builder.items[_builder.count - 1] == 'c');
//     _builder.items[_builder.count - 1] = 'o';
// 
//     nob_sb_append_null(&_builder);
//     return _builder.items;
// }
// 
// bool compile(Nob_Cmd *cmd, char *source_file, char **outfile) {
//     *outfile = src_to_obj(source_file);
// 
//     nob_cc(cmd);
//     nob_cc_flags(cmd);
//     nob_cmd_append(cmd, "-c");
//     nob_cc_inputs(cmd, source_file);
//     nob_cc_output(cmd, *outfile);
// 
//     MUST(nob_cmd_run(cmd));
//     return true;
// }

bool compile(Nob_Cmd *cmd) {
    nob_cc(cmd);
    nob_cc_flags(cmd);
    nob_cmd_append(cmd, "-c");
    nob_cc_inputs(cmd, "main.c");
    nob_cc_output(cmd, BUILD_FOLDER"main.o");

    MUST(nob_cmd_run(cmd));

    nob_cc(cmd);
    nob_cc_flags(cmd);
    nob_cmd_append(cmd, "-c", "-x", "c", "-DNOB_IMPLEMENTATION");
    nob_cc_inputs(cmd, "nob.h");
    nob_cc_output(cmd, BUILD_FOLDER"nob.o");

    MUST(nob_cmd_run(cmd));

    nob_cc(cmd);
    nob_cc_inputs(cmd, BUILD_FOLDER"main.o", BUILD_FOLDER"nob.o");
    nob_cc_output(cmd, BUILD_FOLDER"main");
    nob_cmd_append(cmd);

    MUST(nob_cmd_run(cmd));

    return true;
}
