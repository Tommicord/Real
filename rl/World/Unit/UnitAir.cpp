import Rl.World.Unit.UnitAir;
import Rl.World.Unit;

namespace Rl::World
{

UnitAir::UnitAir() noexcept :
    IUnit(IUnitIdentifiable::GetClassId()), IUnitIdentifiable()
{
}

template class IUnit<UnitAir>;

} // namespace Rl::World
