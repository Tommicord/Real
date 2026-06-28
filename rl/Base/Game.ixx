export module Rl.Base.Game;

import Rl.Base.InputReceiver;
import Rl.Base.Binding;
import Rl.Client.State.UnitState;

import <GLFW/glfw3.h>;
import <optional>;
import <vector>;
import <memory>;
import <vulkan/vulkan.hpp>;

namespace Rl::Game
{

/* The GLFWwindow alias type */
export using Window = GLFWwindow;

/* The MainBinding alias type */
export using Context = MainBinding;

/* The UserInput alias type */
export using Input = Input::UserInput;

/* Width of the window */
export constinit unsigned int width = 1800;

/* Height of the window */
export constinit unsigned int height = 900;

/* The unique input receiver in the Game */
constinit Input::UserInput& input = Input::UserInput::GetInstance();

export class Game
{

  std::unique_ptr<Providers::UnitModel> unitModel;

  public:
  void         Run();
  void         CleanupGraphics();
  void         CleanupResources();
  void         InitGraphics();
  void         InitWindow();
  void         UpdateModels();
  static Game& GetInstance();
  MainBinding& GetVulkanContext();
  ~Game();

  private:
  Game();
  Window* window;
  Input&  input;
  Context binding;

  void CreateInstance();
  void CreateSurface();
  void CreateUnitModel();
  void CreateResources();
  void PickPhysicalDevice();
  void CreateLogicalDevice();
  void CreateSwapChain();
  void CreateImageViews();
  void CreateRenderPass();
  void CreateFramebuffers();
  void CreateCommandPool();
  void CreateCommandBuffers();
  void CreatePipelineLayout();
  void CreateSyncObjects();
  void DrawModels();
  void Draw();

  [[nodiscard]]
  MainBinding::QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;

  [[nodiscard]]
  bool IsDeviceSuitable(VkPhysicalDevice device) const;

  [[nodiscard]]
  bool CheckValidationLayerSupport() const;

  [[nodiscard]]
  std::vector<const char*> GetRequiredExtensions() const;
};

} // namespace Rl::Game
