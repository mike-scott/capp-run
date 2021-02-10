#include <boost/filesystem.hpp>
#include <iostream>
#include <unistd.h>

#include "CLI11.hpp"

#include "oci-hooks.h"

int main(int argc, char **argv) {
  CLI::App app{"capp-run"};

  std::string app_name(boost::filesystem::current_path().filename().string());
  app.add_option("-n,--name", app_name, "Compose application name", true);

  std::string app_dir("./");
  app.add_option("-d,--app-dir", app_dir, "Compose application directory",
                 true);

  auto &create = *app.add_subcommand("createRuntime", "OCI createRuntime hook");
  auto &teardown = *app.add_subcommand("poststop", "OCI poststop hook");

  app.require_subcommand(1);
  CLI11_PARSE(app, argc, argv);

  if (chdir(app_dir.c_str()) != 0) {
    perror("Unable to change into app directory");
    return EXIT_FAILURE;
  }

  try {
    if (create) {
      oci_createRuntime(app_name);
    } else if (teardown) {
      oci_poststop(app_name);
    }
  } catch (const std::exception &ex) {
    std::cerr << ex.what();
    return EXIT_FAILURE;
  }

  return 0;
}
