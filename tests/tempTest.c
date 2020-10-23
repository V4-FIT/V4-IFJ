#include <criterion/criterion.h>
#include <stdlib.h>

// The purpose of this test is to be able to
// build tests while we add actual tests

Test( tempTest, empty )
{
    cr_assert_null( NULL );
}
