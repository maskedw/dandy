/**
 *       @file  DNullOutputBlockDevice.hpp
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

#ifndef dandy_DNullOutputBlockDevice_hpp_
#define dandy_DNullOutputBlockDevice_hpp_


#include <dandy/core/DCore.hpp>


class DNullOutputBlockDevice: public BlockDevice
{
public:
    virtual ~DNullOutputBlockDevice() override {}
    virtual const char* get_type() const { return "DNullOutputBlockDevice"; }
    virtual int init() override { return 0; }
    virtual int deinit() override { return 0; }

    virtual int read(void* dst, bd_addr_t address, bd_size_t size) override { return -1; }
    virtual int program(const void* src, bd_addr_t address, bd_size_t size) override { return 0; }

    virtual bd_size_t get_read_size() const override { return 1; }
    virtual bd_size_t get_program_size() const override { return 1; }
    virtual bd_size_t size() const override { return UINT32_MAX; }
};


#endif /* end of include guard: dandy_DNullOutputBlockDevice_hpp_ */
