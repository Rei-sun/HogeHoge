#ifndef _H_BASEMODULE_
#define _H_BASEMODULE_

#include "ModuleDefinition.h"

class BaseModule {
protected:
  ModuleID module_id;
  uint8_t module_num;

  BaseModule(ModuleID _module_id, uint8_t _module_num)
    : module_id(_module_id), module_num(_module_num) {
  }
  BaseModule() = delete;
};

#endif