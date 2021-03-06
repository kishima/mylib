/*! @file
  @brief
  mruby bytecode executor.

  <pre>
  Copyright (C) 2015-2017 Kyushu Institute of Technology.
  Copyright (C) 2015-2017 Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.

  Fetch mruby VM bytecodes, decode and execute.

  </pre>
*/

#ifndef MRBC_SRC_VM_H_
#define MRBC_SRC_VM_H_

#include <stdint.h>
#include "vm_config.h"
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif


//================================================================
/*!@brief
  IREP Internal REPresentation
*/
typedef struct IREP {
  uint16_t nlocals;		//!< # of local variables
  uint16_t nregs;		//!< # of register variables
  uint16_t rlen;		//!< # of child IREP blocks
  uint16_t ilen;		//!< # of irep
  uint16_t plen;		//!< # of pool

  uint8_t     *code;		//!< ISEQ (code) BLOCK
  mrbc_object  **pools;          //!< array of POOL objects pointer.
  uint8_t     *ptr_to_sym;
  struct IREP **reps;		//!< array of child IREP's pointer.

} mrbc_irep;
typedef struct IREP mrb_irep;


//================================================================
/*!@brief
  Call information
*/
typedef struct CALLINFO {
  struct CALLINFO *prev;
  mrbc_irep *pc_irep;
  uint16_t  pc;
  mrbc_value *current_regs;
  mrbc_class *target_class;
  uint8_t   n_args;     // num of args
} mrbc_callinfo;
typedef struct CALLINFO mrb_callinfo;


//================================================================
/*!@brief
  Virtual Machine
*/
typedef struct VM {
  mrbc_irep *irep;

  uint8_t        vm_id; // vm_id : 1..n
  const uint8_t *mrb;   // bytecode

  mrbc_irep *pc_irep;    // PC
  uint16_t  pc;         // PC

  //  uint16_t     reg_top;
  mrbc_value    regs[MAX_REGS_SIZE];
  mrbc_value   *current_regs;
  mrbc_callinfo *callinfo_tail;

  mrbc_class *target_class;

  int32_t error_code;

  volatile int8_t flag_preemption;
  int8_t flag_need_memfree;
} mrbc_vm;
typedef struct VM mrb_vm;


const char *mrbc_get_irep_symbol(const uint8_t *p, int n);
const char *mrbc_get_callee_name(struct VM *vm);
void mrbc_push_callinfo(struct VM *vm, int n_args);
void mrbc_pop_callinfo(struct VM *vm);
mrbc_vm *mrbc_vm_open(struct VM *vm_arg);
void mrbc_vm_close(struct VM *vm);
void mrbc_vm_begin(struct VM *vm);
void mrbc_vm_end(struct VM *vm);
int mrbc_vm_run(struct VM *vm);


//================================================================
/*!@brief
  Get 32bit value from memory big endian.

  @param  s	Pointer of memory.
  @return	32bit unsigned value.
*/
inline static uint32_t bin_to_uint32( const void *s )
{
#if MRBC_REQUIRE_32BIT_ALIGNMENT
  uint8_t *p = (uint8_t *)s;
  uint32_t x = *p++;
  x <<= 8;
  x |= *p++;
  x <<= 8;
  x |= *p++;
  x <<= 8;
  x |= *p;
  return x;
#else
  uint32_t x = *((uint32_t *)s);
  return (x << 24) | ((x & 0xff00) << 8) | ((x >> 8) & 0xff00) | (x >> 24);
#endif
}


//================================================================
/*!@brief
  Get 16bit value from memory big endian.

  @param  s	Pointer of memory.
  @return	16bit unsigned value.
*/
inline static uint16_t bin_to_uint16( const void *s )
{
#if MRBC_REQUIRE_32BIT_ALIGNMENT
  uint8_t *p = (uint8_t *)s;
  uint16_t x = *p++ << 8;
  x |= *p;
  return x;
#else
  uint16_t x = *((uint16_t *)s);
  return (x << 8) | (x >> 8);
#endif
}

/*!@brief
  Set 16bit big endian value from memory.

  @param  s Input value.
  @param  bin Pointer of memory.
  @return sizeof(uint16_t).
*/
inline static void uint16_to_bin(uint16_t s, uint8_t *bin)
{
  *bin++ = (s >> 8) & 0xff;
  *bin   = s & 0xff;
}

/*!@brief
  Set 32bit big endian value from memory.

  @param  l Input value.
  @param  bin Pointer of memory.
  @return sizeof(uint32_t).
*/
static inline void uint32_to_bin(uint32_t l, uint8_t *bin)
{
  *bin++ = (l >> 24) & 0xff;
  *bin++ = (l >> 16) & 0xff;
  *bin++ = (l >> 8) & 0xff;
  *bin   = l & 0xff;
}

#ifdef __cplusplus
}
#endif
#endif
