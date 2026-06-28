export module Rl.Player;

import Rl.Player.Camera;
import Rl.Base.IUpdatable;

import <memory>;

namespace Rl::Player
{

export class IPlayer : public Providers::IUpdatable
{
public:
  /* The 64-bit coordinate type (1 meter = 1000)
   * To avoid floating-point math errors
   */
  using Coord = long long;

  /* Floating-point double precision type of the Coord */
  using FpCoord = double;

  /* The current X, Y, Z player coordinates */
  Coord cX, cY, cZ;

  /* The unique, current camera player */
  std::unique_ptr<IPlayerCamera> camera;

  /* Gets coordinate floating-point version of coordinates */
  [[nodiscard]]
  constexpr FpCoord GetFp(const Coord coord) const
  {
    return static_cast<FpCoord>((coord * 4294967L) >> 32); // Divide by 1000
  }

  /* Gets horizontal coordinate floating-point version of coordinates */
  [[nodiscard]]
  constexpr FpCoord GetXfp() const
  {
    return GetFp(cX);
  }

  /* Gets vertical coordinate floating-point version of coordinates */
  [[nodiscard]]
  constexpr FpCoord GetYfp() const
  {
    return GetFp(cY);
  }

  /* Gets depth coordinate floating-point version of coordinates */
  [[nodiscard]]
  constexpr FpCoord GetZfp() const
  {
    return GetFp(cZ);
  }

  IPlayer() noexcept
  {
    cX = cY = 0L;
    cZ = 5000L; // 5.0

    camera = std::make_unique<IPlayerCamera>();
    camera->SetAspectRatio(16.0f / 9.0f);
    // For now this
    camera->SetEyePosition(
      IPlayerCamera::Eye { GetXfp(), GetYfp(), GetZfp() }
    );
  }
  IPlayer& operator=(const IPlayer& player) = delete;
  IPlayer& operator=(const IPlayer&& player) = delete;
};

/* Provides a unique current player instance for the Game instance */
export class PlayerProvider
{
public:
  /* Gets the singleton player instance */
  [[nodiscard]]
  static IPlayer& GetInstance();
  
  PlayerProvider(const PlayerProvider&)            = delete;
  PlayerProvider& operator=(const PlayerProvider&) = delete;
  PlayerProvider(PlayerProvider&&)                 = delete;
  PlayerProvider& operator=(PlayerProvider&&)      = delete;

private:
  PlayerProvider() = default;
  ~PlayerProvider() = default;
  
  static std::unique_ptr<IPlayer> player;
};

} // namespace Rl::Player
