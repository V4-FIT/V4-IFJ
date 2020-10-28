#include "criterion/criterion.h"

#include "char_sequence.h"

#define LONG_STRING_SIZE 0xDEAD

Test( char_sequence, initialization ) {
	charseq_t charseq = charseq_init();
	cr_assert_not_null(charseq);
	cr_assert_not_null(charseq_get(charseq));

	cr_expect_eq(charseq_len(charseq), 0);
	cr_expect_str_empty(charseq_get(charseq));

	charseq_free(charseq);
}

Test( char_sequence, content_equality ) {
	charseq_t charseq = charseq_init();
	cr_assert_not_null(charseq);
	cr_assert_not_null(charseq_get(charseq));

	charseq_add(charseq, 'o');
	charseq_add(charseq, 'k');

	cr_expect_str_not_empty(charseq_get(charseq));
	cr_expect_str_eq(charseq_get(charseq), "ok");

	charseq_free(charseq);
}

Test( char_sequence, length_equality ) {
	charseq_t charseq = charseq_init();
	cr_assert_not_null(charseq);
	cr_assert_not_null(charseq_get(charseq));

	charseq_add(charseq, 'o');
	charseq_add(charseq, 'k');

	cr_expect_eq(charseq_len(charseq), strlen(charseq_get(charseq)));

	charseq_free(charseq);
}

Test( char_sequence, long_string ) {
	charseq_t charseq = charseq_init();
	cr_assert_not_null(charseq);
	cr_assert_not_null(charseq_get(charseq));

	for (int i = 0; i < LONG_STRING_SIZE; i++) {
		charseq_add(charseq, 'o');
	}

	cr_expect_eq(charseq_len(charseq), LONG_STRING_SIZE);
	cr_expect_eq(charseq_len(charseq), strlen(charseq_get(charseq)));

	charseq_free(charseq);
}
