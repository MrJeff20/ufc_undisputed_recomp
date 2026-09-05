#include "ufc_image.h"

#include <cstdlib>
#include <filesystem>
#include <memory>
#include <string_view>

#include <rex/filesystem.h>
#include <rex/rex_app.h>
#include <rex/runtime.h>
#include <rex/system/gpu_plugin.h>
#include <rex/ui/windowed_app.h>

namespace {

class UFC3RexApp final : public rex::ReXApp {
 public:
  explicit UFC3RexApp(rex::ui::WindowedAppContext& ctx)
      : rex::ReXApp(ctx, "ufc3", UFC3PPCImageConfig,
                    "[game_data_root]") {
    AddPositionalOption("game_data_root");
  }

  static std::unique_ptr<rex::ui::WindowedApp> Create(
      rex::ui::WindowedAppContext& ctx) {
    return std::make_unique<UFC3RexApp>(ctx);
  }

 protected:
  void OnConfigurePaths(rex::PathConfig& paths) override {
    if (auto arg = GetArgument("game_data_root")) {
      paths.game_data_root = std::filesystem::path(*arg);
      return;
    }

    paths.game_data_root = std::filesystem::path(
        "D:\\Codificacion\\UFC Undisputed3\\Xbox360");
  }

  void OnPreSetup(rex::RuntimeConfig& config) override {
    const char* backend = std::getenv("UFC3_GPU_BACKEND");
    config.graphics = rex::system::LoadGpuPlugin(
        "xenos", backend && *backend ? std::string_view(backend) : "any");
    config.gpu_plugin.clear();
  }
};

}  // namespace

REX_DEFINE_APP(ufc3, UFC3RexApp::Create)
