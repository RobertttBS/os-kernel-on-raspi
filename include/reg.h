#ifndef REG_H
#define REG_H


#define SPSR_MASK_D                 (1 << 9)
#define SPSR_MASK_A                 (1 << 8)
#define SPSR_MASK_I                 (1 << 7)
#define SPSR_MASK_F                 (1 << 6)
#define SPSR_EL0t                   (0b0000 << 0) // Exception level [3:2] and selected Stack Pointer [0]. from which level we trap in to el2
#define SPSR_EL1t                   (0b0100 << 0)
#define SPSR_EL1h                   (0b0101 << 0)
#define SPSR_EL2t                   (0b1000 << 0)
#define SPSR_EL2h                   (0b1001 << 0)

#define SPSR_EL2_VALUE              (SPSR_MASK_D | SPSR_MASK_A | SPSR_MASK_I | SPSR_MASK_F | SPSR_EL1h) // 0x3c5

#endif