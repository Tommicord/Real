#include "rl/World/Unit/UnitGrass.h"
#include "rl/World/Unit/Unit.h"

namespace Rl::World {

UnitGrass::UnitGrass() noexcept : AbstractUnit(this)
{
}

bool UnitGrass::InGrowState()
{
    return true;
}

void UnitGrass::Grow()
{
}

// Explicit template instantiation
template AbstractUnit::AbstractUnit(UnitGrass*) noexcept;

} // namespace Rl::World
