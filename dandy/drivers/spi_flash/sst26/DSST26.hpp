/**
 *       @file  DSST26.hpp
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

#ifndef dandy_DSST26_hpp_
#define dandy_DSST26_hpp_


#include <dandy/core/DCore.hpp>
#include <dandy/drivers/spi_flash/DSST26_def.h>


enum DSST26DeviceType
{
    D_SST26_TYPE_VF016,
    D_SST26_TYPE_VF032,
    D_SST26_TYPE_VF064,
    D_SST26_TYPE_END,
    D_SST26_TYPE_UNKNOWN = D_SST26_TYPE_END,
};

class DSST26: public BlockDevice
{
public:
    DSST26(PinName mosi, PinName miso, PinName sclk, PinName cs, int frequency);
    virtual ~DSST26() override;
    virtual const char* get_type() const { return "DSST26"; }
    virtual int init() override;
    virtual int deinit() override;
    virtual int read(void* dst, bd_addr_t address, bd_size_t size) override;
    virtual int program(const void* src, bd_addr_t address, bd_size_t size) override;
    virtual int erase(bd_addr_t address, bd_size_t size) override;
    virtual bd_size_t get_read_size() const override;
    virtual bd_size_t get_program_size() const override;
    virtual bd_size_t get_erase_size() const override;
    virtual bd_size_t size() const override;

    DSST26DeviceType getDeviceType() const;
    bool chipErase();
    uint8_t readStatusRegister();
    uint8_t readConfigurationRegister();

private:
    void DoCommand(uint8_t cmd, bd_addr_t address, const void* tx, int txSize, void* rx, int rxSize);
    void DoCommandNoAddress(uint8_t cmd, const void* tx, int txSize, void* rx, int rxSize);
    void WriteEnable();
    void WriteDisable();
    void WaitForCommandCompletion();
    void UnlockGlobalBlockProtection();
    bd_size_t GetBlockEraseSize(bd_addr_t addr) const;
    void EraseSectorOrBlock(uint8_t eraseCommand, bd_addr_t address);

    void SPICSHigh();
    void SPICSLow();
    void SPIExchange(const void* tx, int txSize, void* rx, int rxSize);

    SPI m_spi;
    DigitalOut m_cs;
    size_t m_memorySize;
    DSST26DeviceType m_deviceType;
};


#endif /* end of include guard: dandy_DSST26_hpp_ */
