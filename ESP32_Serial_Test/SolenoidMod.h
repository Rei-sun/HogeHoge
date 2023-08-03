#pragma once

#include "ModuleDefinition.h"
#include "BaseModule.h"

#include "Solenoid.h"

class SolenoidMod : public BaseModule {
public:
  typedef struct {
    unsigned state_1: 1;
    unsigned state_2: 1;
    unsigned state_3: 1;
    unsigned state_4: 1;
    unsigned state_5: 1;
    unsigned state_6: 1;
    unsigned state_7: 1;
    unsigned state_8: 1;
  }  in_state_struct;
  
  typedef union {
    uint8_t all;
    in_state_struct bits;
  } in_state;

  // device output
  in_state solenoid_state;

  Solenoid solenoid_1;
  Solenoid solenoid_2;
  Solenoid solenoid_3;

  SolenoidMod(uint8_t _module_num, uint8_t _pin_1, uint8_t _pin_2, uint8_t _pin_3):
    BaseModule(ModuleID::SolenoidModule, _module_num), 
    solenoid_state{0},
    solenoid_1(_pin_1),
    solenoid_2(_pin_2),
    solenoid_3(_pin_3)
  {

  }

  void Begin() {
    solenoid_1.Begin();
    solenoid_2.Begin();
    solenoid_3.Begin();
  }

  void UpdateAll() {
    solenoid_1.SetState(solenoid_state.bits.state_1);
    solenoid_2.SetState(solenoid_state.bits.state_2);
    solenoid_3.SetState(solenoid_state.bits.state_3);
  }
};