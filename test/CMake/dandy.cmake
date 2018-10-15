include_directories(
    ..
)
set(dandy_sources
    ../dandy/core/DRef.cpp
    ../dandy/core/DObject.cpp
    ../dandy/core/DObjectStorage.cpp
    ../dandy/core/utils/DFileUtils.cpp
    ../dandy/drivers/i2c/DI2C.cpp
    ../dandy/drivers/i2c/DMbedI2C.cpp
    # ../dandy/core/DStringUtils.cpp
    # ../dandy/core/DFileUtils.cpp
    # ../dandy/core/DMemStream.cpp
    # ../dandy/core/DNullStream.cpp
    # ../dandy/shell/DReadLine.c
    ../dandy/shell/commands/d_i2cdetect.cpp
)
