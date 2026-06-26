import Rl.World.Unit.UnitGrass;
import Rl.World.Unit;

namespace Rl::World
{

UnitGrass::UnitGrass() noexcept :
    IUnit(IUnitIdentifiable<UnitGrass>::GetClassId()), IUnitGrowable(), IUnitIdentifiable<UnitGrass>()
{
}

bool UnitGrass::InGrowState()
{
  return true;
}

void UnitGrass::Grow()
{
}
template class IUnit<UnitGrass>;

} // namespace Rl::World
