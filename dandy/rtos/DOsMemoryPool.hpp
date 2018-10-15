/**
 *       @file  DQueue.hpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2018/05/31
 * ===================================================================
 */


/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef dandy_DMemoryPoll_hpp_
#define dandy_DMemoryPoll_hpp_


#include <dandy/core/DCore.hpp>
#include "cmsis_os2.h"
#include "mbed_rtos_storage.h"
#include "mbed_rtos1_types.h"


/** mbed::MemoryPollクラスの改造版です
 *
 *  mbed::MemoryPoolはプールサイズをtemplate引数で静的に指定する必要がありますが
 *  、この改造版では動的にサイズ指定ができます。
 */
template<typename T>
class DOsMemoryPool : private mbed::NonCopyable<DOsMemoryPool<T> > {
public:
    /** Create and Initialize a memory pool.
     *
     * @note You cannot call this function from ISR context.
    */
    DOsMemoryPool(uint32_t pool_sz) {
        X_ASSERT((pool_sz > 0) && "Invalid memory pool size. Must be greater than 0.");
        /* osMemoryPoolNew requires that pool block size is a multiple of 4 bytes. */
        const uint32_t mem_sz = ((sizeof(T) + 3) & ~3) * pool_sz;
        _pool_mem = D_NEW(char[mem_sz]);
        X_ASSERT(_pool_mem);

        memset(_pool_mem, 0, mem_sz);
        memset(&_obj_mem, 0, sizeof(_obj_mem));
        osMemoryPoolAttr_t attr = { 0 };
        attr.mp_mem = _pool_mem;
        attr.mp_size = mem_sz;
        attr.cb_mem = &_obj_mem;
        attr.cb_size = sizeof(_obj_mem);
        _id = osMemoryPoolNew(pool_sz, sizeof(T), &attr);
        X_ASSERT(_id);
    }

    /** Destroy a memory pool
     *
     * @note You cannot call this function from ISR context.
    */
    ~DOsMemoryPool() {
        osMemoryPoolDelete(_id);
        D_DELETE_ARRAY(_pool_mem);
    }

    /** Allocate a memory block of type T from a memory pool.
      @return  address of the allocated memory block or NULL in case of no memory available.

      @note You may call this function from ISR context.
    */
    T* alloc(void) {
        return (T*)osMemoryPoolAlloc(_id, 0);
    }

    /** Allocate a memory block of type T from a memory pool and set memory block to zero.
      @return  address of the allocated memory block or NULL in case of no memory available.

      @note You may call this function from ISR context.
    */
    T* calloc(void) {
        T *item = (T*)osMemoryPoolAlloc(_id, 0);
        if (item != NULL) {
            memset(item, 0, sizeof(T));
        }
        return item;
    }

    /** Free a memory block.
      @param   block  address of the allocated memory block to be freed.
      @return         osOK on successful deallocation, osErrorParameter if given memory block id
                      is NULL or invalid, or osErrorResource if given memory block is in an
                      invalid memory pool state.

      @note You may call this function from ISR context.
    */
    osStatus free(T *block) {
        return osMemoryPoolFree(_id, (void*)block);
    }

private:
    osMemoryPoolId_t             _id;
    char*                        _pool_mem;
    mbed_rtos_storage_mem_pool_t _obj_mem;
};

#endif /* end of include guard: dandy_DMemoryPoll_hpp_ */
