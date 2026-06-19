#pragma once

#include "rl/Base/Game.h"

namespace Rl::Providers {

struct StateResource
{
    virtual ~StateResource() = default;
};

class StateDrawableVulkan {};

template<class T = void, class U = void>
class StateDrawable
{
public:
    using state_res_value_type = T;
    using state_vk_value_type = U;
    StateDrawable() = default;
    StateDrawable(const StateDrawable& other) = delete;
    StateDrawable& operator=(const StateDrawable& other) = delete;
    virtual ~StateDrawable() = default;
    virtual void OnDraw(const CameraStateResource& resource, CameraStateDrawableVulkan& vk,
                        Game::VulkanContext& context) = 0;
    virtual void OnUpdate(T& resource,
                          U& vk,
                          Game::Game::Context& context) = 0;
    virtual void OnCreate(T& resources,
                          U& vk,
                          Game::Game::Context& context) = 0;
    virtual void OnDestroy(T& resources,
                           U& vk,
                           Game::Game::Context& context) = 0;
    virtual void OnPause() = 0;
    virtual void OnResume() = 0;
};

} // namespace Rl::Providers
