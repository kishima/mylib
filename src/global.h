/*! @file
  @brief
  Manage global objects.

  <pre>
  Copyright (C) 2015-2018 Kyushu Institute of Technology.
  Copyright (C) 2015-2018 Shimane IT Open-innovation Center.

  This file is distributed under BSD 3-Clause License.

  </pre>
*/

#ifndef MRBC_SRC_GLOBAL_H_
#define MRBC_SRC_GLOBAL_H_

#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif


void mrbc_init_global(void);
void global_object_add(mrbc_sym sym_id, mrbc_value v);
void const_object_add(mrbc_sym sym_id, mrbc_object *obj);
mrbc_value global_object_get(mrbc_sym sym_id);
mrbc_object const_object_get(mrbc_sym sym_id);
void mrbc_global_clear_vm_id(void);


#ifdef __cplusplus
}
#endif
#endif
