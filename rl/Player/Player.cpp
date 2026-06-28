import Rl.Player;

namespace Rl::Player
{

std::unique_ptr<IPlayer> PlayerProvider::player = nullptr;

IPlayer& PlayerProvider::GetInstance()
{
  if (!player)
  {
    player = std::make_unique<IPlayer>();
  }
  return *player;
}

} // namespace Rl::Player
