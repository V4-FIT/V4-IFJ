#include "test_token_list.h"

#define EXAMPLES_PATH "go_files/examples"

#define TK_MATCH(_tk)                      \
	do {                                   \
		token_t tk = tklist_front(tklist); \
		if (tk != NULL) {                  \
			EXPECT_EQ(tk->type, _tk);      \
			tklist_pop_front(tklist);      \
		}                                  \
	} while (0)

TEST_F(TokenListTestFile, DISABLED_example1) {
	SetUp(EXAMPLES_PATH, "hello.go");
	ASSERT_NE(stream, nullptr);
	TK_MATCH(TK_EOL);
	TK_MATCH(TK_EOL);
	TK_MATCH(TK_EOL);
	TK_MATCH(TK_KEYW_PACKAGE);
	TK_MATCH(TK_KEYW_MAIN);
	TK_MATCH(TK_EOL);
	TK_MATCH(TK_EOL);
	TK_MATCH(TK_KEYW_FUNC);
	TK_MATCH(TK_KEYW_MAIN);
	TK_MATCH(TK_L_PARENTHESIS);
	TK_MATCH(TK_R_PARENTHESIS);
	TK_MATCH(TK_L_CURLY);
	TK_MATCH(TK_EOL);
	TK_MATCH(TK_IDENTIFIER);
	TK_MATCH(TK_L_PARENTHESIS);
	TK_MATCH(TK_STR_LIT);
	TK_MATCH(TK_COMMA);
	TK_MATCH(TK_STR_LIT);
	TK_MATCH(TK_R_PARENTHESIS);
	TK_MATCH(TK_EOL);
	TK_MATCH(TK_R_CURLY);
	TK_MATCH(TK_EOL);
	TK_MATCH(TK_EOF);
	TK_MATCH(TK_EOF);
	TK_MATCH(TK_EOF);
}

TEST(token_list, basic) {
	struct token token;
	token.lexeme = "";
	tklist_t tklist = tklist_init();
	ASSERT_NE(tklist, nullptr);

	token.type = TK_EOL;
	tklist_push_back(tklist, &token);
	token.type = TK_COMMA;
	tklist_push_back(tklist, &token);

	token_t tk;
	tk = tklist_front(tklist);
	ASSERT_NE(tk, nullptr);
	ASSERT_EQ(tk->type, TK_EOL);
	tk = tklist_second(tklist);
	ASSERT_NE(tk, nullptr);
	ASSERT_EQ(tk->type, TK_COMMA);

	tklist_free(tklist);
}