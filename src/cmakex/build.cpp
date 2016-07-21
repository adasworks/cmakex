#include "build.h"

#include <adasworks/sx/check.h>

#include "cmakex_utils.h"
#include "misc_utils.h"
#include "out_err_messages.h"
#include "print.h"

namespace cmakex {

void build(string_par binary_dir, const pkg_desc_t& request, string_par config)
{
    // create binary dir
    cmakex_config_t cfg(binary_dir);

    auto pkg_bin_dir = pkg_bin_dir_helper(cfg, request, config);
    auto pkg_clone_dir = cfg.pkg_clone_dir(request.name);
    string source_dir = pkg_clone_dir;

    if (!request.b.source_dir.empty())
        source_dir += "/" + request.b.source_dir;

    vector<string> args;
    args.emplace_back(string("-H") + source_dir);
    args.emplace_back(string("-B") + pkg_bin_dir);

    args.insert(args.end(), BEGINEND(request.b.cmake_args));

    args.emplace_back(
        stringf("-DCMAKE_INSTALL_PREFIX=%s", cfg.pkg_install_dir(request.name).c_str()));
    args.emplace_back(stringf("-DCMAKE_PREFIX_PATH=%s", cfg.deps_install_dir().c_str()));

    log_info("Configuring '%s'-%s", request.name.c_str(), config.c_str());

    log_exec("cmake", args);
    {
        OutErrMessagesBuilder oeb(pipe_capture, pipe_capture);
        int r = exec_process("cmake", args, oeb.stdout_callback(), oeb.stderr_callback());
        auto oem = oeb.move_result();

        save_log_from_oem(
            oem, cfg.cmakex_log_dir(),
            stringf("%s-%s-configure%s", request.name.c_str(), config.c_str(), k_log_extension));

        if (r != EXIT_SUCCESS)
            throwf("CMake configure step failed, result: %d.", r);
    }
    args.assign({"--build", pkg_bin_dir.c_str(), "--config", config.c_str()});

    // todo add build_args
    // todo add --clean-first if it's needed but not yet added
    // todo add native tool args
    // todo clear install dir if --clean-first

    log_info("Building '%s'-%s", request.name.c_str(), config.c_str());

    log_exec("cmake", args);
    {
        OutErrMessagesBuilder oeb(pipe_capture, pipe_capture);
        int r = exec_process("cmake", args, oeb.stdout_callback(), oeb.stderr_callback());
        auto oem = oeb.move_result();

        save_log_from_oem(oem, cfg.cmakex_log_dir(), stringf("%s-%s-build%s", request.name.c_str(),
                                                             config.c_str(), k_log_extension));

        if (r != EXIT_SUCCESS)
            throwf("CMake build step failed, result: %d.", r);
    }

    args.assign(
        {"--build", pkg_bin_dir.c_str(), "--config", config.c_str(), "--target", "install"});

    // todo add build_args
    // todo add --clean-first if it's needed but not yet added
    // todo add native tool args

    log_info("Installing '%s'-%s", request.name.c_str(), config.c_str());

    log_exec("cmake", args);
    {
        OutErrMessagesBuilder oeb(pipe_capture, pipe_capture);
        int r = exec_process("cmake", args, oeb.stdout_callback(), oeb.stderr_callback());
        auto oem = oeb.move_result();

        save_log_from_oem(
            oem, cfg.cmakex_log_dir(),
            stringf("%s-%s-install%s", request.name.c_str(), config.c_str(), k_log_extension));

        if (r != EXIT_SUCCESS)
            throwf("CMake install step failed, result: %d.", r);
    }
}
}
