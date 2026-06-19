#include "rl/World/Unit/UnitDynamicTexture.h"
#include "rl/Base/SimplexNoise.h"
#include "rl/Base/Texture2.h"

#include <algorithm>
#include <vector>

namespace Rl::World {

using namespace Rl::Providers;

UnitDynamicTexture::ColorFreqKV UnitDynamicTexture::ColorFreqKV::operator++()
{
    ++freq;
    return *this;
}

UnitDynamicTexture::UnitDynamicTexture(Texture2& base, const Seed seed, DynamicOptions& options) :
    options(options), noiseGen{seed}, baseTexture(base), seed(seed)
{
}

std::vector<float> UnitDynamicTexture::GenNoiseValMap(const float scale) const
{
    std::vector<float> map;
    int width = baseTexture.GetWidth();
    int height = baseTexture.GetHeight();
    map.reserve(width * height);
    int channels = baseTexture.GetChannels();
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const size_t index = (y * width + x) * channels;
            float randX = static_cast<float>(x) * scale;
            float randY = static_cast<float>(y) * scale;
            float noise = noiseGen.eval(randX, randY);
            map[index] = noise;
        }
    }
    return map;
}

std::vector<int> UnitDynamicTexture::GetTargetColorMap()
{
    std::vector<int> clMap;
    uint8_t* data = baseTexture.GetData();
    int width = baseTexture.GetWidth();
    int height = baseTexture.GetHeight();
    int channels = baseTexture.GetChannels();
    constexpr int blockSize = 4;
    int blocksX = (width + blockSize - 1) / blockSize;
    int blocksY = (height + blockSize - 1) / blockSize;

    for (int blockY = 0; blockY < blocksY; ++blockY)
    {
        for (int blockX = 0; blockX < blocksX; ++blockX)
        {
            std::vector<ColorFreqKV> colorFreq;
            for (int y = 0; y < blockSize; ++y)
            {
                for (int x = 0; x < blockSize; ++x)
                {
                    int pixelX = blockX * blockSize + x;
                    int pixelY = blockY * blockSize + y;
                    if (pixelX >= width || pixelY >= height)
                        continue;
                    const int index = (pixelY * width + pixelX) * channels;
                    uint8_t r = data[index];
                    uint8_t g = data[index + 1];
                    uint8_t b = data[index + 2];
                    uint32_t colorKey = (r << 16) | (g << 8) | b;
                    ++colorFreq[colorKey];
                }
            }
            ProcessColorFreqMap(colorFreq, clMap);
        }
    }
    return clMap;
}
void UnitDynamicTexture::ProcessColorFreqMap(const std::vector<ColorFreqKV>& colorFreq,
                                             std::vector<int>& outMap)
{
    if (!colorFreq.empty())
    {
        const auto mostCommon =
            std::ranges::max_element(colorFreq,
                [](const auto& a, const auto& b)
                {
                    return a.key < b.freq;
                }
            );
        const int colorKey = mostCommon->key;
        outMap.push_back(colorKey);
        return;
    }
    outMap.push_back(0x00000000); // Default black
}

} // namespace Rl::World
