#include <string>
#include <vector>
#include "Unit.h"
#include "rl/World/UnitResourceName.h"

namespace Rl::World {

void AbstractUnit::InitRegistryUnits()
{

registry.Register(1, {{"block", "grass"}}, new );

}

}
