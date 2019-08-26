/**
 *       @file  DIS25.cpp
 *      @brief
 *
 *    @details
 *
 *     @author  Yslab Corp. Kenji Otomune (otomune@yslab.co.jp)
 *
 *   @internal
 *     Created  2018/12/28
 * ===================================================================
 */

#include <dandy/drivers/spi_flash/is25/DIS25.hpp>


struct D__EraseInfo
{
    uint8_t cmd;
    bd_size_t size;
};

const D__EraseInfo D__eraseInfos[] = {
    { D_IS25_CMD_4BER64, D_IS25_ERASE_SIZE_64KB},
    { D_IS25_CMD_4BER32, D_IS25_ERASE_SIZE_32KB},
    { D_IS25_CMD_4SER, D_IS25_ERASE_SIZE_4KB}
};

static bool D__IsAligned(bd_addr_t address, bd_addr_t align)
{
    X_ASSERT(align > 0);
    const bool aligned = (address & (align - 1)) == 0;

    return aligned;
}

DIS25::DIS25(PinName mosi, PinName miso, PinName sclk, PinName cs, int frequency)
    : m_spi(mosi, miso, sclk)
    , m_cs(cs)
    , m_memorySize(0)
    , m_deviceType(D_IS25_TYPE_UNKNOWN)
{
    this->SPICSHigh();
    m_spi.frequency(frequency);
}

DIS25::~DIS25()
{
    this->deinit();
}

void DIS25::chipErase()
{
    this->WriteEnable();
    this->DoCommandNoAddress(D_IS25_CMD_CER, NULL, 0, NULL, 0);
    this->WaitForCommandCompletion();
}

int DIS25::init()
{
    const uint8_t IS25_ids[][3] = {
        { 0x9D, 0x60, 0x16 }, /* IS25LP032A */
        { 0x9D, 0x60, 0x17 }, /* IS25LP064A */
        { 0x9D, 0x60, 0x18 }, /* IS25LP128A */
        { 0x9D, 0x60, 0x19 }, /* IS25LP256D */
        { 0x9D, 0x70, 0x19 }, /* IS25WP256D */
        { 0x9D, 0x60, 0x1A }, /* IS25LP512M */
        { 0x9D, 0x70, 0x1A }, /* IS25WP512M */
    };

    uint8_t deviceID[3] = { 0 };

    this->DoCommandNoAddress(D_IS25_CMD_RDJDID, NULL, 0, deviceID, sizeof(deviceID));

    DIS25DeviceType deviceType = D_IS25_TYPE_UNKNOWN;
    for (int i = 0; i < X_COUNT_OF_ROW(IS25_ids); i++)
    {
        if (memcmp(deviceID, IS25_ids[i], sizeof(deviceID)) == 0)
        {
            X_ASSERT(i < D_IS25_TYPE_END);
            deviceType = static_cast<DIS25DeviceType>(i);
            break;
        }
    }

    if (deviceType == D_IS25_TYPE_UNKNOWN)
    {
        X_LOG_WARN(("DIS25", "Invalid ids {%02X,%02X,%02X}", deviceID[0], deviceID[1], deviceID[2]));
        return BD_ERROR_DEVICE_ERROR;
    }

    switch (deviceType)
    {
        case D_IS25_TYPE_LP032A:
            m_memorySize = D_IS25xx32_SIZE;
            break;
        case D_IS25_TYPE_LP064A:
            m_memorySize = D_IS25xx64_SIZE;
            break;
        case D_IS25_TYPE_LP128A:
            m_memorySize = D_IS25xx128_SIZE;
            break;
        case D_IS25_TYPE_LP256D:
        case D_IS25_TYPE_WP256D:
            m_memorySize = D_IS25xx256_SIZE;
            break;
        case D_IS25_TYPE_LP512M:
        case D_IS25_TYPE_WP512M:
            m_memorySize = D_IS25xx512_SIZE;
            break;
        X_ABORT_DEFAULT;
    }
    m_deviceType = deviceType;
    return BD_ERROR_OK;
}

int DIS25::deinit()
{
    this->WRDI_writeDisable();
    return BD_ERROR_OK;
}

int DIS25::read(void* dst, bd_addr_t address, bd_size_t size)
{
    X_ASSERT(this->is_valid_read(address, size));

    this->DoCommand(D_IS25_CMD_4NORD, address, NULL, 0, dst, size);

    return BD_ERROR_OK;
}

int DIS25::program(const void* src, bd_addr_t address, bd_size_t size)
{
    X_ASSERT(this->is_valid_program(address, size));

    const char* p = static_cast<const char*>(src);
    const int maxSizeOfOneTime = 256; /* 1回のPPコマンドで書き込み可能な最大バイト数 */

    while (size)
    {
        this->WriteEnable();

        const int toProgram = size > maxSizeOfOneTime ? maxSizeOfOneTime : size;
        this->DoCommand(D_IS25_CMD_4PP, address, p, toProgram, NULL, 0);
        p += toProgram;
        address += toProgram;
        size -= toProgram;

        this->WaitForCommandCompletion();
    }
    return BD_ERROR_OK;
}

int DIS25::erase(bd_addr_t address, bd_size_t size)
{
    X_ASSERT(this->is_valid_erase(address, size));

    while (size)
    {
        const D__EraseInfo* info = NULL;
        for (int i = 0; i < D_COUNT_OF(D__eraseInfos); i++)
        {
            if ((size >= D__eraseInfos[i].size) &&
                (D__IsAligned(address, D__eraseInfos[i].size)))
            {
                info = &D__eraseInfos[i];
                break;
            }
        }
        X_ASSERT(info);

        this->WriteEnable();
        this->xER_sectorOrBlockErase(info->cmd, address);
        address += info->size;
        size -= info->size;
        this->WaitForCommandCompletion();
    }

    return BD_ERROR_OK;
}

bd_size_t DIS25::get_read_size() const
{
    return 1;
}

bd_size_t DIS25::get_program_size() const
{
    return 1;
}

bd_size_t DIS25::get_erase_size() const
{
    return D_IS25_ERASE_SIZE_4KB;
}


bd_size_t DIS25::size() const
{
    return m_memorySize;
}

DIS25DeviceType DIS25::getDeviceType() const
{
    return m_deviceType;
}

void DIS25::DoCommand(uint8_t cmd, bd_addr_t address, const void* tx, int txSize, void* rx, int rxSize)
{
    const uint8_t preTx[5] = {
        cmd,
        static_cast<uint8_t>((address >> 24) & 0xFF),
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

void DIS25::DoCommandNoAddress(uint8_t cmd, const void* tx, int txSize, void* rx, int rxSize)
{
    this->SPICSLow();
    this->SPIExchange(&cmd, 1, NULL, 0);
    if (tx || rx)
        this->SPIExchange(tx, txSize, rx, rxSize);
    this->SPICSHigh();
}

void DIS25::WaitForCommandCompletion()
{
    for (;;)
    {
        const uint8_t status = this->RDSP_readStatusRegister();
        if (!(status & D_IS25_SR_BIT_WIP))
            break;
    }
}

void DIS25::WREN_writeEnable()
{
    this->DoCommandNoAddress(D_IS25_CMD_WREN, NULL, 0, NULL, 0);
}

void DIS25::WRDI_writeDisable()
{
    this->DoCommandNoAddress(D_IS25_CMD_WRDI, NULL, 0, NULL, 0);
}

void DIS25::WriteEnable()
{
    this->WREN_writeEnable();
    const uint8_t statusRegister = this->RDSP_readStatusRegister();
    X_ASSERT(statusRegister & D_IS25_SR_BIT_WEL);
}

void DIS25::xER_sectorOrBlockErase(uint8_t eraseCommand, bd_addr_t address)
{
    this->DoCommand(eraseCommand, address, NULL, 0, NULL, 0);
}

void DIS25::WRSR_writeStatusRegister(uint8_t statusRegister)
{
    this->DoCommandNoAddress(D_IS25_CMD_WRSR, &statusRegister, 1, NULL, 0);
}

uint8_t DIS25::RDSP_readStatusRegister()
{
    uint8_t statusRegister;
    this->DoCommandNoAddress(D_IS25_CMD_RDSR, NULL, 0, &statusRegister, 1);

    return statusRegister;
}

void DIS25::SPICSHigh()
{
    m_cs.write(1);
}

void DIS25::SPICSLow()
{
    m_cs.write(0);
}

void DIS25::SPIExchange(const void* tx, int txSize, void* rx, int rxSize)
{
    m_spi.write((const char*)tx, txSize, (char*)rx, rxSize);
}
