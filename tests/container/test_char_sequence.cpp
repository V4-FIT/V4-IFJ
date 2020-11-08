#include <gtest/gtest.h>

extern "C" {
#include "char_sequence.h"
}

#define LONG_STRING_SIZE 0xDEAD

TEST(char_sequence, initialization) {
	charseq_t charseq = charseq_init();
	ASSERT_NE(charseq, nullptr);
	ASSERT_NE(charseq_data(charseq), nullptr);

	EXPECT_EQ(charseq_len(charseq), 0);
	EXPECT_STREQ(charseq_data(charseq), "");

	charseq_free(charseq);
}

TEST(char_sequence, content_equality) {
	charseq_t charseq = charseq_init();
	ASSERT_NE(charseq, nullptr);
	ASSERT_NE(charseq_data(charseq), nullptr);

	charseq_push_back(charseq, 'o');
	charseq_push_back(charseq, 'k');

	EXPECT_STREQ(charseq_data(charseq), "ok");

	charseq_free(charseq);
}

TEST(char_sequence, length_equality) {
	charseq_t charseq = charseq_init();
	ASSERT_NE(charseq, nullptr);
	ASSERT_NE(charseq_data(charseq), nullptr);

	charseq_push_back(charseq, 'o');
	charseq_push_back(charseq, 'k');

	EXPECT_EQ(charseq_len(charseq), strlen(charseq_data(charseq)));

	charseq_free(charseq);
}

TEST(char_sequence, long_string) {
	charseq_t charseq = charseq_init();
	ASSERT_NE(charseq, nullptr);
	ASSERT_NE(charseq_data(charseq), nullptr);

	for (int i = 0; i < LONG_STRING_SIZE; i++) {
		charseq_push_back(charseq, 'o');
	}

	EXPECT_EQ(charseq_len(charseq), LONG_STRING_SIZE);
	EXPECT_EQ(charseq_len(charseq), strlen(charseq_data(charseq)));

	charseq_free(charseq);
}
