export module Rl.Player.PlayerController;

import Rl.Player;
import Rl.Base.InputReceiver;

namespace Rl::Player
{

/* Controls player movement from input */
export class PlayerController final : public Input::IInputObserver
{
public:
  /* Constructs controller with player reference */
  explicit PlayerController(IPlayer& player) noexcept;

  /* Handles keyboard input for player movement */
  void OnKeyEvent(const Input::KeyEvent& event) override;

  /* Handles mouse button input for player actions */
  void OnMouseButtonEvent(const Input::MouseButtonEvent& event) override;

  /* Handles mouse movement for player interaction */
  void OnMouseMoveEvent(const Input::MouseMoveEvent& event) override;

  /* Handles mouse scroll for player actions */
  void OnMouseScrollEvent(const Input::MouseScrollEvent& event) override;

  /* Updates player state from input */
  void Update() const;

private:
  IPlayer& player;
  float    moveSpeed{10.0f};
  bool     moveForward{false};
  bool     moveBackward{false};
  bool     moveLeft{false};
  bool     moveRight{false};
  bool     moveUp{false};
  bool     moveDown{false};
};

}