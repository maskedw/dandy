/**
 *       @file  DOsMail.hpp
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
 * Copyright (c) 2006-2017 ARM Limited
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

#ifndef dandy_DOsMail_hpp_
#define dandy_DOsMail_hpp_


#include <dandy/rtos/DOsQueue.hpp>
#include <dandy/rtos/DOsMemoryPool.hpp>


/** mbed::Mailクラスの改造版です
 *
 *  mbed::Mailはキューサイズをtemplate引数で静的に指定する必要がありま
 *  すが、この改造版では動的にサイズ指定ができます。
 */
template<typename T>
class DOsMail : private mbed::NonCopyable<DOsMail<T> > {
public:
    DOsMail(uint32_t queue_sz)
        : _queue(queue_sz)
        , _pool(queue_sz)
    {
    }

    /** Check if the mail queue is empty
     *
     * @return True if the mail queue is empty, false if not
     *
     * @note You may call this function from ISR context.
     */
    bool empty() const {
        return _queue.empty();
    }

    /** Check if the mail queue is full
     *
     * @return True if the mail queue is full, false if not
     *
     * @note You may call this function from ISR context.
     */
    bool full() const {
        return _queue.full();
    }

    /** Allocate a memory block of type T
      @param   millisec  timeout value or 0 in case of no time-out. (default: 0).
      @return  pointer to memory block that can be filled with mail or NULL in case error.

      @note You may call this function from ISR context if the millisec parameter is set to 0.
    */
    T* alloc(uint32_t millisec=0) {
        return _pool.alloc();
    }

    /** Allocate a memory block of type T and set memory block to zero.
      @param   millisec  timeout value or 0 in case of no time-out.  (default: 0).
      @return  pointer to memory block that can be filled with mail or NULL in case error.

      @note You may call this function from ISR context if the millisec parameter is set to 0.
    */
    T* calloc(uint32_t millisec=0) {
        return _pool.calloc();
    }

    /** Put a mail in the queue.
      @param   mptr  memory block previously allocated with Mail::alloc or Mail::calloc.
      @return  status code that indicates the execution status of the function.

      @note You may call this function from ISR context.
    */
    osStatus put(T *mptr) {
        return _queue.put(mptr);
    }

    /** Get a mail from a queue.
      @param   millisec  timeout value or 0 in case of no time-out. (default: osWaitForever).
      @return  event that contains mail information or error code.

      @note You may call this function from ISR context if the millisec parameter is set to 0.
    */
    osEvent get(uint32_t millisec=osWaitForever) {
        osEvent evt = _queue.get(millisec);
        if (evt.status == osEventMessage) {
            evt.status = osEventMail;
        }
        return evt;
    }

    /** Free a memory block from a mail.
      @param   mptr  pointer to the memory block that was obtained with Mail::get.
      @return  status code that indicates the execution status of the function.

      @note You may call this function from ISR context.
    */
    osStatus free(T *mptr) {
        return _pool.free(mptr);
    }

private:
    DOsQueue<T> _queue;
    DOsMemoryPool<T> _pool;
};

#endif /* end of include guard: dandy_DOsMail_hpp_ */
