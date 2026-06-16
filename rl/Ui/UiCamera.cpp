#include "rl/Base/CameraProvider.h"
#include "rl/Base/StateDrawable.h"
#include "rl/Base/ShaderFactory.h"

using namespace Rl::Providers;

namespace Rl::Ui {

CameraStateDrawable::CameraStateDrawable(const Camera& camera) {
    resources = std::make_shared<CameraStateDrawableResources>(camera);
}

}
