#pragma once

#include "rl/Base/StateDrawable.h"

namespace Rl::Providers {

template<class T, class D>
class StateModel
{
public:
    /* Initializer of a state model */
    explicit StateModel(Game::VulkanContext context) {}

    /* Default destructor for child classes */
    virtual ~StateModel() = default;

    /* Gets the stored object */
    virtual T& GetObject() = 0;

    /* Gets the stored camera */
    virtual StateResource& GetResource() = 0;

    /* Gets the stored camera */
    virtual D& GetDrawable() = 0;

    /* Gets the stored camera */
    virtual StateDrawableVulkan& GetVulkanState() = 0;
};

}