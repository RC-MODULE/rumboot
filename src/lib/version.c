#include <rumboot/version.h>
#include <platform/bootheader.h>

static const struct rumboot_version_info version = {
    .platform = RUMBOOT_PLATFORM,
    .gitversion = RUMBOOT_VERSION,
    .chip_id = RUMBOOT_PLATFORM_CHIPID,
    .chip_rev = RUMBOOT_PLATFORM_CHIPREV
};

const struct rumboot_version_info *rumboot_version_info()
{
    return &version;
}