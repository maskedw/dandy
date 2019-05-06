/**
 *       @file  DBlockDeviceMapper.cpp
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
 * Copyright (c) 2010-2011 mbed.org, MIT License
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


#include <dandy/core/DBlockDeviceMapper.hpp>

static inline bool D__IsWithIn(bd_addr_t x, bd_addr_t begin, bd_addr_t end)
{
    return ((begin <= x) && (x < end));
}

void DBlockDeviceMapper::addMap(BlockDevice* device, bd_addr_t virtualBegin, bd_addr_t physicalBegin, bd_size_t size)
{
    Map map;
    map.device = device;
    map.vbegin = virtualBegin;
    map.pbegin = physicalBegin;
    map.size = size;

    D_FOREACH(MappingTable::iterator, ite, this->m_mappingTable)
    {
        X_ASSERT(!D__IsWithIn(map.vbegin, ite->vbegin, ite->vbegin + ite->size));
        X_ASSERT(!D__IsWithIn(map.vbegin + map.size, ite->vbegin, ite->vbegin + ite->size));
    }

    this->m_mappingTable.push_back(map);
}


void DBlockDeviceMapper::removeMap(BlockDevice* device)
{
    D_FOREACH(MappingTable::iterator, ite, this->m_mappingTable)
    {
        if (ite->device == device)
        {
            this->m_mappingTable.erase(ite);
            break;
        }
    }
}


BlockDevice* DBlockDeviceMapper::findDevice(bd_addr_t virtualAddress, bd_addr_t* physicalAddressOut) const
{
    BlockDevice* device = nullptr;
    D_FOREACH(MappingTable::const_iterator, ite, this->m_mappingTable)
    {
        if (D__IsWithIn(virtualAddress, ite->vbegin, ite->vbegin + ite->size))
        {
            const bd_addr_t offset = virtualAddress - ite->vbegin;
            device = ite->device;
            *physicalAddressOut = ite->pbegin + offset;
            break;
        }
    }

    return device;
}
