#pragma once

#include <ll/api/plugin/NativePlugin.h>
#include <ll/api/dimension/CustomDimensionManager.h>
#include <ll/api/dimension/SimpleCustomDimension.h>

namespace ll
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
