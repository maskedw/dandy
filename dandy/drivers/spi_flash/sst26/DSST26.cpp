/**
 *       @file  DSST26.cpp
 *      @brief
 *
 *    @details
 *    Microchip  Serial SPI/SQI™ NOR Flash Devices
 *    https://www.microchip.com/design-centers/memory/serial-parallel-flash/superflash-technology-features/serial-flash
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2019/06/04
 * ===================================================================
 */

/*
 * License: MIT license
 * Copyright (c) <2019> <MaskedW [maskedw00@gmail.com]>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <dandy/drivers/spi_flash/DSST26.hpp>


static const char TAG[] = "DSST26";


DSST26::DSST26(PinName mosi, PinName miso, PinName sclk, PinName cs, int frequency)
    : m_spi(mosi, miso, sclk)
    , m_cs(cs)
    , m_memorySize(0)
    , m_deviceType(D_SST26_TYPE_UNKNOWN)
{
    this->SPICSHigh();
    m_spi.frequency(frequency);
}

DSST26::~DSST26()
{
    this->deinit();
}

int DSST26::init()
{
    const uint8_t SST26_ids[][3] = {
        { 0xBF, 0x26, 0x01 }, /* SST26VF016 */
        { 0xBF, 0x26, 0x02 }, /* SST26VF032 */
        { 0xBF, 0x26, 0x43 }, /* SST26VF064 */
    };

    uint8_t deviceID[3] = { 0 };

    this->DoCommandNoAddress(D_SST26_CMD_JEDEC_ID, NULL, 0, deviceID, sizeof(deviceID));

    DSST26DeviceType deviceType = D_SST26_TYPE_UNKNOWN;
    for (int i = 0; i < X_COUNT_OF_ROW(SST26_ids); i++)
    {
        if (memcmp(deviceID, SST26_ids[i], sizeof(deviceID)) == 0)
        {
            X_ASSERT(i < D_SST26_TYPE_END);
            deviceType = static_cast<DSST26DeviceType>(i);
            break;
        }
    }

    if (deviceType == D_SST26_TYPE_UNKNOWN)
    {
        X_LOG_WARN(("DSST26", "Invalid ids {%02X,%02X,%02X}", deviceID[0], deviceID[1], deviceID[2]));
        return false;
    }

    switch (deviceType)
    {
        case D_SST26_TYPE_VF016:
            m_memorySize = D_SST26XX016_SIZE;
            break;
        case D_SST26_TYPE_VF032:
            m_memorySize = D_SST26XX032_SIZE;
            break;
        case D_SST26_TYPE_VF064:
            m_memorySize = D_SST26XX064_SIZE;
            break;
        X_ABORT_DEFAULT;
    }
    m_deviceType = deviceType;

    this->UnlockGlobalBlockProtection();

    return true;
}

int DSST26::deinit()
{
    this->WriteDisable();
    return 0;
}

int DSST26::read(void* dst, bd_addr_t address, bd_size_t size)
{
    X_ASSERT(this->is_valid_read(address, size));

    this->DoCommand(D_SST26_CMD_Read, address, NULL, 0, dst, size);

    return 0;
}

int DSST26::program(const void* src, bd_addr_t address, bd_size_t size)
{
    X_ASSERT(this->is_valid_program(address, size));

    const char* p = static_cast<const char*>(src);
    const int maxSizeOfOneTime = 256; /* 1回のPPコマンドで書き込み可能な最大バイト数 */

    while (size)
    {
        this->WriteEnable();

        const int toProgram = size > maxSizeOfOneTime ? maxSizeOfOneTime : size;
        this->DoCommand(D_SST26_CMD_PP, address, p, toProgram, NULL, 0);
        p += toProgram;
        address += toProgram;
        size -= toProgram;

        this->WaitForCommandCompletion();
    }
    return 0;
}

int DSST26::erase(bd_addr_t address, bd_size_t size)
{
    X_ASSERT(this->is_valid_erase(address, size));

    while (size)
    {
        const bd_size_t blockEraseSize = this->GetBlockEraseSize(address);
        uint8_t eraseCommand;
        bd_size_t eraseSize;

        if ((size >= blockEraseSize) && ((address % blockEraseSize) == 0))
        {
            eraseCommand = D_SST26_CMD_BE;
            eraseSize = blockEraseSize;
        }
        else
        {
            eraseCommand = D_SST26_CMD_SE;
            eraseSize = D_SST26_ERASE_SIZE_4KB;
        }

        this->EraseSectorOrBlock(eraseCommand, address);
        address += eraseSize;
        size -= eraseSize;
    }

    return 0;
}

bd_size_t DSST26::get_read_size() const
{
    if (!m_memorySize)
        return 0;

    return 1;
}

bd_size_t DSST26::get_program_size() const
{
    if (!m_memorySize)
        return 0;

    return 1;
}

bd_size_t DSST26::get_erase_size() const
{
    if (!m_memorySize)
        return 0;

    return D_SST26_ERASE_SIZE_4KB;
}

bd_size_t DSST26::size() const
{
    return m_memorySize;
}

DSST26DeviceType DSST26::getDeviceType() const
{
    return m_deviceType;
}

bool DSST26::chipErase()
{
    this->WriteEnable();
    this->DoCommandNoAddress(D_SST26_CMD_CE, NULL, 0, NULL, 0);
    this->WaitForCommandCompletion();

    return true;
}

uint8_t DSST26::readStatusRegister()
{
    uint8_t statusRegister;
    this->DoCommandNoAddress(D_SST26_CMD_RDSR, NULL, 0, &statusRegister, 1);

    return statusRegister;
}

uint8_t DSST26::readConfigurationRegister()
{
    uint8_t configurationRegister;
    this->DoCommandNoAddress(D_SST26_CMD_RDCR, NULL, 0, &configurationRegister, 1);

    return configurationRegister;
}

void DSST26::DoCommand(uint8_t cmd, bd_addr_t address, const void* tx, int txSize, void* rx, int rxSize)
{
    const uint8_t preTx[4] = {
        cmd,
        static_cast<uint8_t>((address >> 16) & 0xFF),
        static_cast<uint8_t>((address >> 8)  & 0xFF),
        static_cast<uint8_t>((address) & 0xFF)
    };

    this->SPICSLow();
    this->SPIExchange(preTx, sizeof(preTx), NULL, 0);
    if (tx || rx)
        this->SPIExchange(tx, txSize, rx, rxSize);
    this->SPICSHigh();
}

void DSST26::DoCommandNoAddress(uint8_t cmd, const void* tx, int txSize, void* rx, int rxSize)
{
    this->SPICSLow();
    this->SPIExchange(&cmd, 1, NULL, 0);
    if (tx || rx)
        this->SPIExchange(tx, txSize, rx, rxSize);
    this->SPICSHigh();
}

void DSST26::WriteEnable()
{
    this->DoCommandNoAddress(D_SST26_CMD_WREN, NULL, 0, NULL, 0);
    const uint8_t statusRegister = this->readStatusRegister();
    X_ASSERT(statusRegister & D_SST26_SR_BIT_WEL);
}

void DSST26::WriteDisable()
{
    this->DoCommandNoAddress(D_SST26_CMD_WRDI, NULL, 0, NULL, 0);
    const uint8_t statusRegister = this->readStatusRegister();
    X_ASSERT(!(statusRegister & D_SST26_SR_BIT_WEL));
}

void DSST26::WaitForCommandCompletion()
{
    for (;;)
    {
        const uint8_t status = this->readStatusRegister();
        if (!(status & D_SST26_SR_BIT_BUSY))
            break;
    }
}

void DSST26::UnlockGlobalBlockProtection()
{
    /* パワーオンリセット後はデフォルトで書き込み保護されているので、ULBPRコマン
     * ドでロックを解除しないと消去も書き込みもできない。
     *
     * see [DataSheet PDF p7 4.1 Device Protection]
     * SST26VF064B/064BA offer a flexible memory protec-
     * tion scheme that allows the protection state of each
     * individual block to be controlled separately. In addition,
     * the Write-Protection Lock-Down register prevents any
     * change of the lock status during device operation. To
     * avoid inadvertent writes during power-up, the device is
     * write-protected by default after a power-on reset cycle.
     * A Global Block-Protection Unlock command offers a
     * single command cycle that unlocks the entire memory
     * array for faster manufacturing throughput.
     */
    this->WriteEnable();
    this->DoCommandNoAddress(D_SST26_CMD_ULBPR, NULL, 0, NULL, 0);
    this->WaitForCommandCompletion();
}

bd_size_t DSST26::GetBlockEraseSize(bd_addr_t addr) const
{
    /* see [DataSheet PDF p5 Memory Organization]
     */
    X_ASSERT(addr < m_memorySize);
    if (addr < 32 * 1024)
    {
        return 8 * 1024;
    }
    else if (addr < (64 * 1024))
    {
        return 32 * 1024;
    }
    else if (x_is_uwithin(addr, m_memorySize - (64 * 1024), m_memorySize - (32 * 1024)))
    {
        return 32 * 1024;
    }
    else if (x_is_uwithin(addr, m_memorySize - (32 * 1024), m_memorySize))
    {
        return 8 * 1024;
    }

    return 64 * 1024;
}

void DSST26::EraseSectorOrBlock(uint8_t eraseCommand, bd_addr_t address)
{
    this->WriteEnable();
    this->DoCommand(eraseCommand, address, NULL, 0, NULL, 0);
    this->WaitForCommandCompletion();
}

void DSST26::SPICSHigh()
{
    m_cs.write(1);
}

void DSST26::SPICSLow()
{
    m_cs.write(0);
}

void DSST26::SPIExchange(const void* tx, int txSize, void* rx, int rxSize)
{
    m_spi.write((const char*)tx, txSize, (char*)rx, rxSize);
}
