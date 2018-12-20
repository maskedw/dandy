set(rootdir ${CMAKE_CURRENT_LIST_DIR})

include_directories(
    ${rootdir}
    ${rootdir}/dandy_config
    ${rootdir}/dandy_external/EASTL/include
    ${rootdir}/dandy_external/EASTL/test/packages/EABase/include/Common
)

add_definitions(
    -DEASTL_USER_CONFIG_HEADER="eastl_dandy_config.h"
)

set(dandy_sources
    ${rootdir}/dandy/core/DRef.cpp
    ${rootdir}/dandy/core/DObject.cpp
    ${rootdir}/dandy/core/DObjectStorage.cpp
    ${rootdir}/dandy/core/utils/DFILEUtils.cpp
    ${rootdir}/dandy/core/utils/DStringUtils.cpp
    ${rootdir}/dandy/core/stream/DStream.cpp
    ${rootdir}/dandy/core/stream/DNullStream.cpp
    ${rootdir}/dandy/core/stream/DMemoryInputStream.cpp
    ${rootdir}/dandy/core/stream/DMemoryOutputStream.cpp
    ${rootdir}/dandy/core/stream/DFILEStream.cpp
    ${rootdir}/dandy/drivers/i2c/DI2C.cpp
    ${rootdir}/dandy/drivers/i2c/DMbedI2C.cpp
)
