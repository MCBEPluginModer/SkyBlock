#include <ll/api/plugin/NativePlugin.h>

#include "Plugin.h"

#include "ll/api/Logger.h"
#include "ll/api/service/Bedrock.h"

#include "mc/deps/core/mce/Color.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/DimensionConversionData.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/biome/VanillaBiomeNames.h"
#include "mc/world/level/biome/registry/BiomeRegistry.h"
#include "mc/world/level/chunk/VanillaLevelChunkUpgrade.h"
#include "mc/world/level/dimension/NetherDimension.h"
#include "mc/world/level/dimension/OverworldDimension.h"
#include "mc/world/level/dimension/VanillaDimensions.h"
#include "mc/world/level/dimension/end/TheEndDimension.h"
#include "mc/world/level/levelgen/VoidGenerator.h"
#include "mc/world/level/levelgen/flat/FlatWorldGenerator.h"
#include "mc/world/level/levelgen/v1/NetherGenerator.h"
#include "mc/world/level/levelgen/v1/OverworldGenerator2d.h"
#include "mc/world/level/levelgen/v1/TheEndGenerator.h"

#include "magic_enum.hpp"

bool IslandGenerator::postProcess(ChunkViewSource& chunkViewSource)
{
    //TODO Написать генерацию острова и сундука на нем с деревом
    return true;
}

namespace ll::dimension {

static ll::Logger loggerMoreDim("SkyBlock");

SkyBlock::SkyBlock(std::string const& name, DimensionFactoryInfo const& info)
: Dimension(info.level, info.dimId, {-64, 320}, info.scheduler, name) {
    loggerMoreDim.debug(__FUNCTION__ " dimension name:{}", name);
    mDefaultBrightness.sky = Brightness::MAX;
    generatorType          = *magic_enum::enum_cast<GeneratorType>((std::string_view)info.data["generatorType"]);
    seed                   = info.data["seed"];
    mSeaLevel                = 50;
    mHasWeather              = true;
    mDimensionBrightnessRamp = std::make_unique<OverworldBrightnessRamp>();
    mDimensionBrightnessRamp->buildBrightnessRamp();
}

CompoundTag SkyBlock::generateNewData(uint seed, GeneratorType generatorType) {
    CompoundTag result;
    result["seed"]          = seed;
    result["generatorType"] = magic_enum::enum_name(generatorType);
    return result;
}

void SkyBlock::init() {
    loggerMoreDim.debug(__FUNCTION__);
    setSkylight(false);
    Dimension::init();
}

std::unique_ptr<WorldGenerator> SkyBlock::createGenerator() {
    loggerMoreDim.debug(__FUNCTION__);
    auto& level     = getLevel();
    auto& levelData = level.getLevelData();
    auto  biome     = level.getBiomeRegistry().lookupByName(levelData.getBiomeOverride());

    auto generator    = std::make_unique<VoidGenerator>(*this);
    generator->mBiome = level.getBiomeRegistry().lookupByHash(VanillaBiomeNames::Ocean);
    return std::move(generator);
    
}

void SkyBlock::upgradeLevelChunk(ChunkSource& cs, LevelChunk& lc, LevelChunk& generatedChunk) {
    loggerMoreDim.debug(__FUNCTION__);
    auto blockSource = BlockSource(getLevel(), *this, cs, false, true, false);
    VanillaLevelChunkUpgrade::_upgradeLevelChunkViaMetaData(lc, generatedChunk, blockSource);
    VanillaLevelChunkUpgrade::_upgradeLevelChunkLegacy(lc, blockSource);
}

void SkyBlock::fixWallChunk(ChunkSource& cs, LevelChunk& lc) {
    loggerMoreDim.debug(__FUNCTION__);
    auto blockSource = BlockSource(getLevel(), *this, cs, false, true, false);
    VanillaLevelChunkUpgrade::fixWallChunk(lc, blockSource);
}

bool SkyBlock::levelChunkNeedsUpgrade(LevelChunk const& lc) const {
    loggerMoreDim.debug(__FUNCTION__);
    return VanillaLevelChunkUpgrade::levelChunkNeedsUpgrade(lc);
}
void SkyBlock::_upgradeOldLimboEntity(CompoundTag& tag, ::LimboEntitiesVersion vers) {
    loggerMoreDim.debug(__FUNCTION__);
    auto isTemplate = getLevel().getLevelData().isFromWorldTemplate();
    return VanillaLevelChunkUpgrade::upgradeOldLimboEntity(tag, vers, isTemplate);
}

Vec3 SkyBlock::translatePosAcrossDimension(Vec3 const& fromPos, DimensionType fromId) const {
    loggerMoreDim.debug(__FUNCTION__);
    Vec3 topos;
    VanillaDimensions::convertPointBetweenDimensions(
        fromPos,
        topos,
        fromId,
        mId,
        getLevel().getDimensionConversionData()
    );
    constexpr auto clampVal = 32000000.0f - 128.0f;

    topos.x = std::clamp(topos.x, -clampVal, clampVal);
    topos.z = std::clamp(topos.z, -clampVal, clampVal);

    return topos;
}

short SkyBlock::getCloudHeight() const { return 256; }

bool SkyBlock::hasPrecipitationFog() const { return false; }

std::unique_ptr<ChunkSource>
SkyBlock::_wrapStorageForVersionCompatibility(std::unique_ptr<ChunkSource> cs, ::StorageVersion /*ver*/) {
    loggerMoreDim.debug(__FUNCTION__);
    return cs;
}

mce::Color SkyBlock::getBrightnessDependentFogColor(mce::Color const& color, float brightness) const {
    loggerMoreDim.debug(__FUNCTION__);
    float temp   = (brightness * 0.94f) + 0.06f;
    float temp2  = (brightness * 0.8f) + 0.09f;
    auto  result = color;
    result.r     = color.r * temp;
    result.g     = color.g * temp;
    result.b     = color.b * temp2;
    return result;
};

std::unique_ptr<StructureFeatureRegistry> SkyBlock::makeStructureFeatures(
    bool                   isLegacy,
    BaseGameVersion const& baseGameVersion,
    Experiments const&     experiments
) {
    loggerMoreDim.debug(__FUNCTION__);
    return OverworldDimension::makeStructureFeatures(seed, isLegacy, baseGameVersion, experiments);
};
} // namespace ll::dimension


namespace plugin {

Plugin::Plugin(ll::plugin::NativePlugin& self) : mSelf(self) {
    mSelf.getLogger().info("loading...");

    // Code for loading the plugin goes here.
}

bool Plugin::enable() {
    mSelf.getLogger().info("enabling...");

    // Code for enabling the plugin goes here.

    return true;
}

bool Plugin::disable() {
    mSelf.getLogger().info("disabling...");

    // Code for disabling the plugin goes here.

    return true;
}

} // namespace plugin
