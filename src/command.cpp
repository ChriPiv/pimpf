#include <cstdint>
#include "command.h"

namespace pimpfvm {

Instruction IntToInstr (uint32_t Param) {

    Instruction RetVal;

    union {
        uint32_t A;
        struct {
            uint8_t B1, B2, B3, B4;
        } B;
    };
    A = Param;
    RetVal.Com = (Command)B.B4;
    RetVal.Param1 = B.B3;
    RetVal.Param2 = B.B2;
    RetVal.Param3 = B.B1;

    return RetVal;

}

uint32_t InstrToInt (Instruction Param) {

    union {
        uint32_t A;
        struct {
            uint8_t B1, B2, B3, B4;
        } B;
    };

    B.B4 = (uint8_t) Param.Com;
    B.B3 = Param.Param1;
    B.B2 = Param.Param2;
    B.B1 = Param.Param3;
    
    return A;
}

}
