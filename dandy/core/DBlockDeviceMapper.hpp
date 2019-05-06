/**
 *       @file  DBlockDeviceMapper.hpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2019/01/21
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


#ifndef dandy_DBlockDeviceMapper_hpp_
#define dandy_DBlockDeviceMapper_hpp_


#include <dandy/core/DCore.hpp>
#include <list>


class DBlockDeviceMapper
{
public:

    void addMap(BlockDevice* device, bd_addr_t virtualBegin, bd_addr_t physicalBegin, bd_size_t size);
    void removeMap(BlockDevice* device);
    BlockDevice* findDevice(bd_addr_t virtualAddress, bd_addr_t* physicalAddressOut) const;

private:
    struct Map
    {
        BlockDevice* device;
        bd_addr_t vbegin;
        bd_addr_t pbegin;
        bd_size_t size;
    };

    typedef std::list<Map>  MappingTable;

    MappingTable    m_mappingTable;
};


#endif // gengine_DBlockDeviceMapper_hpp_
