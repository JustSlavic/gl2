#include <gtest/gtest.h>

#include <defines.h>

DEFINE_BIT_MASK(Flags,
    ONE   = 0x1,
    TWO   = 0x2,
    THREE = 0x4,
    FOUR  = 0x8,
    FIVE  = 0x10,
);

TEST(Bit_Masks, Size_Set_Get_Unset) {
    Flags flags;

    ASSERT_EQ(sizeof(flags), 4);

    flags.set(Flags::ONE);
    flags.set(Flags::THREE);
    flags.set(Flags::FOUR);

    ASSERT_EQ(flags.value, 0b1101);
    
    ASSERT_EQ(flags.get(Flags::ONE), true);
    ASSERT_EQ(flags.get(Flags::TWO), false);
    ASSERT_EQ(flags.get(Flags::THREE), true);
    ASSERT_EQ(flags.get(Flags::FOUR), true);
    ASSERT_EQ(flags.get(Flags::FIVE), false);

    flags.unset(Flags::THREE);

    ASSERT_EQ(flags.value, 0b1001);

    ASSERT_EQ(flags.get(Flags::ONE), true);
    ASSERT_EQ(flags.get(Flags::TWO), false);
    ASSERT_EQ(flags.get(Flags::THREE), false);
    ASSERT_EQ(flags.get(Flags::FOUR), true);
    ASSERT_EQ(flags.get(Flags::FIVE), false);
}
