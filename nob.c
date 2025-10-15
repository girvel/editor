#include <assert.h>
#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_FOLDER ".build/"
#define SOURCES_N 1
const char* source_files[SOURCES_N] = {
    "main.c",
};

bool compile_all(Nob_Cmd *cmd, Nob_File_Paths *outfiles);
bool link_(Nob_Cmd *cmd, Nob_File_Paths targets);

bool run(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "nob.h");

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return false;

    Nob_Cmd cmd = {0};
    Nob_File_Paths outfiles = {0};

    if (!compile_all(&cmd, &outfiles)) return false;
    if (!link_(&cmd, outfiles)) return false;

    nob_cmd_append(&cmd, "./"BUILD_FOLDER"editor");
    if (!nob_cmd_run(&cmd)) return false;

    return true;
}

int main(int argc, char **argv) {
    return !run(argc, argv);
}

// needs manual deallocation
char *src_to_obj(const char *path) {
    Nob_String_Builder _builder = {0};
    _builder.count = 0;

    nob_sb_append_cstr(&_builder, BUILD_FOLDER);
    nob_sb_append_cstr(&_builder, path);

    assert(_builder.items[_builder.count - 2] == '.');
    assert(_builder.items[_builder.count - 1] == 'c');
    _builder.items[_builder.count - 1] = 'o';

    nob_sb_append_null(&_builder);
    return _builder.items;
}

bool compile(Nob_Cmd *cmd, const char *source_file, char **outfile) {
    *outfile = src_to_obj(source_file);

    nob_cc(cmd);
    nob_cc_flags(cmd);
    nob_cmd_append(cmd, "-c");
    nob_cc_inputs(cmd, source_file);
    nob_cc_output(cmd, *outfile);

    return nob_cmd_run(cmd);
}

bool compile_all(Nob_Cmd *cmd, Nob_File_Paths *outfiles) {
    bool ok = true;
    for (size_t i = 0; i < SOURCES_N; i++) {
        char *outfile;
        ok &= compile(cmd, source_files[i], &outfile);
        nob_da_append(outfiles, outfile);
    }

    nob_cc(cmd);
    nob_cc_flags(cmd);
    nob_cmd_append(cmd, "-c", "-x", "c", "-DNOB_IMPLEMENTATION");
    nob_cc_inputs(cmd, "nob.h");
    nob_cc_output(cmd, BUILD_FOLDER"nob.o");
    nob_da_append(outfiles, BUILD_FOLDER"nob.o");

    ok &= nob_cmd_run(cmd);

    return ok;
}

bool link_(Nob_Cmd *cmd, Nob_File_Paths targets) {
    nob_cc(cmd);
    nob_da_foreach(const char *, e, &targets) {
        nob_da_append(cmd, *e);
    }

    nob_cc_output(cmd, BUILD_FOLDER"editor");
    return nob_cmd_run(cmd);
}
