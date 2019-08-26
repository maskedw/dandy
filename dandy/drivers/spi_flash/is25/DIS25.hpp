/**
 *       @file  DIS25.hpp
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

#ifndef dandy_DIS25_hpp_
#define dandy_DIS25_hpp_


#include <dandy/core/DCore.hpp>
#include <dandy/drivers/spi_flash/is25/DIS25_def.h>


enum DIS25DeviceType
{
    D_IS25_TYPE_LP032A, /* IS25LP032A (4MB)  */
    D_IS25_TYPE_LP064A, /* IS25LP064A (8MB)  */
    D_IS25_TYPE_LP128A, /* IS25LP128A (16MB) */
    D_IS25_TYPE_LP256D, /* IS25LP256D (32MB) */
    D_IS25_TYPE_WP256D, /* IS25WP256D (32MB) */
    D_IS25_TYPE_LP512M, /* IS25LP512M (64MB) */
    D_IS25_TYPE_WP512M, /* IS25WP512M (64MB) */
    D_IS25_TYPE_END,
    D_IS25_TYPE_UNKNOWN = D_IS25_TYPE_END,
};

class DIS25: public BlockDevice
{
public:
    DIS25(PinName mosi, PinName miso, PinName sclk, PinName cs, int frequency);
    virtual ~DIS25() override;
    virtual const char* get_type() const { return "DIS25"; }
    virtual int init() override;
    virtual int deinit() override;
    virtual int read(void* dst, bd_addr_t address, bd_size_t size) override;
    virtual int program(const void* src, bd_addr_t address, bd_size_t size) override;
    virtual int erase(bd_addr_t address, bd_size_t size) override;
    virtual bd_size_t get_read_size() const override;
    virtual bd_size_t get_program_size() const override;
    virtual bd_size_t get_erase_size() const override;
    virtual bd_size_t size() const override;

    DIS25DeviceType getDeviceType() const;
    void chipErase();
private:
    void DoCommand(uint8_t cmd, bd_addr_t address, const void* tx, int txSize, void* rx, int rxSize);
    void DoCommandNoAddress(uint8_t cmd, const void* tx, int txSize, void* rx, int rxSize);
    void WriteEnable();
    void WaitForCommandCompletion();

    void SPICSLow();
    void SPICSHigh();
    void SPIExchange(const void* tx, int txSize, void* rx, int rxSize);

    void WREN_writeEnable();
    void WRDI_writeDisable();
    void xER_sectorOrBlockErase(uint8_t eraseCommand, bd_addr_t address);
    void WRSR_writeStatusRegister(uint8_t statusRegister);
    uint8_t RDSP_readStatusRegister();

    SPI m_spi;
    DigitalOut m_cs;
    uint32_t m_memorySize;
    DIS25DeviceType m_deviceType;
};


#endif /* end of include guard: dandy_DIS25_hpp_ */
