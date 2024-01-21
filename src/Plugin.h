#pragma once

#include <ll/api/plugin/NativePlugin.h>
#include <ll/api/dimension/CustomDimensionManager.h>
#include <ll/api/dimension/SimpleCustomDimension.h>
#include <mc/world/level/dimension/Dimension.h>
#include <mc/world/level/dimension/NetherBrightnessRamp.h>
#include <mc/world/level/dimension/OverworldBrightnessRamp.h>
#include <mc/world/level/levelgen/structure/StructureFeatureRegistry.h>
#include <ll/api/utils/RandomUtils.h>

#include <mc/common/wrapper/FeatureRefTraits.h>
#include <mc/common/wrapper/WeakRefT.h>
#include <mc/deps/core/debug/log/ContentLog.h>
#include <mc/deps/core/string/HashedString.h>
#include <mc/deps/core/utility/buffer_span.h>
#include <mc/deps/json/Reader.h>
#include <mc/deps/json/Value.h>
#include <mc/deps/json/Writer.h>
#include <mc/util/Random.h>
#include <mc/util/random/XoroshiroPositionalRandomFactory.h>
#include <mc/world/level/BlockPalette.h>
#include <mc/world/level/BlockSource.h>
#include <mc/world/level/ChunkBlockPos.h>
#include <mc/world/level/ChunkPos.h>
#include <mc/world/level/Level.h>
#include <mc/world/level/biome/Biome.h>
#include <mc/world/level/biome/registry/BiomeRegistry.h>
#include <mc/world/level/biome/source/BiomeArea.h>
#include <mc/world/level/biome/source/FixedBiomeSource.h>
#include <mc/world/level/block/Block.h>
#include <mc/world/level/block/BlockVolume.h>
#include <mc/world/level/block/registry/BlockTypeRegistry.h>
#include <mc/world/level/block/utils/VanillaBlockTypeIds.h>
#include <mc/world/level/chunk/ChunkViewSource.h>
#include <mc/world/level/chunk/LevelChunk.h>
#include <mc/world/level/dimension/Dimension.h>
#include <mc/world/level/dimension/OverworldBrightnessRamp.h>
#include <mc/world/level/levelgen/WorldGenerator.h>
#include <mc/world/level/levelgen/feature/BambooFeature.h>
#include <mc/world/level/levelgen/feature/FlowerFeature.h>
#include <mc/world/level/levelgen/feature/LakeFeature.h>
#include <mc/world/level/levelgen/feature/registry/FeatureRegistry.h>
#include <mc/world/level/levelgen/flat/FlatWorldGenerator.h>
#include <mc/world/level/levelgen/flat/FlatWorldGeneratorOptions.h>
#include <mc/world/level/levelgen/structure/StructureFeatureRegistry.h>
#include <mc/world/level/levelgen/structure/StructureFeatureType.h>
#include <mc/world/level/levelgen/v1/Aquifer.h>
#include <mc/world/level/levelgen/v1/Ove
#include <mc/world/level/levelgen/v1/OverworldGenerator2d.h>
#include <mc/world/level/levelgen/v1/OverworldGeneratorMultinoise.h>
#include <mc/world/level/levelgen/v1/WorldGenCache.h>
#include <mc/world/level/storage/LevelData.h>
#include <mc/wmc/world/level/levelgen/VoidGenerator.h>

class IslandGenerator : public VoidGenerator
{
public:
   IslandGenerator(Dimension& dimension) : VoidGenerator(dimension) {}
   ~ IslandGenerator() {}
   virtual bool postProcess(ChunkViewSource& chunkViewSource);
};

namespace ll::dimension
{
class SkyBlock  : public Dimension
{
public:
    LLNDAPI SkyBlock(std::string const& name, DimensionFactoryInfo const& info);

    LLNDAPI static CompoundTag generateNewData(uint seed = 123, GeneratorType generatorType = GeneratorType::Void);
    
    void init() override;

    std::unique_ptr<WorldGenerator> createGenerator() override;

    void upgradeLevelChunk(ChunkSource& chunkSource, LevelChunk& oldLc, LevelChunk& newLc) override;

    void fixWallChunk(ChunkSource& cs, LevelChunk& lc) override;

    bool levelChunkNeedsUpgrade(LevelChunk const& lc) const override;

    void _upgradeOldLimboEntity(CompoundTag& tag, ::LimboEntitiesVersion vers) override;

    Vec3 translatePosAcrossDimension(Vec3 const& pos, DimensionType did) const override;

    std::unique_ptr<ChunkSource>
    _wrapStorageForVersionCompatibility(std::unique_ptr<ChunkSource> cs, ::StorageVersion ver) override;


    mce::Color getBrightnessDependentFogColor(mce::Color const& color, float brightness) const override;

    short getCloudHeight() const override;

    bool hasPrecipitationFog() const override;

    std::unique_ptr<StructureFeatureRegistry>
    makeStructureFeatures(bool isLegacy, BaseGameVersion const& baseGameVersion, Experiments const& experiments);
};
}

namespace plugin {

class Plugin {
public:
    explicit Plugin(ll::plugin::NativePlugin& self);

    Plugin(Plugin&&)                 = delete;
    Plugin(const Plugin&)            = delete;
    Plugin& operator=(Plugin&&)      = delete;
    Plugin& operator=(const Plugin&) = delete;

    ~Plugin() = default;

    /// @return True if the plugin is enabled successfully.
    bool enable();

    /// @return True if the plugin is disabled successfully.
    bool disable();

private:
    ll::plugin::NativePlugin& mSelf;
};

} // namespace plugin
