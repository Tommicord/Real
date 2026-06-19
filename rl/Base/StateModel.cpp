#include "rl/Base/StateModel.h"

namespace Rl::Providers {

template <class T, class D>
void StateModel<T, D>::DrawFromStateModel(Game::VulkanContext& context) const
{
    GetDrawable().OnDraw(context);
}


template <class T, class D>
void StateModel<T, D>::UpdateFromStateModel(Game::VulkanContext& context) const
{
    GetObject().Update();
    GetDrawable().OnUpdate(context);
}

} // namespace Rl::Providers