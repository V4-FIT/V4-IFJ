/**
 * @file rules.h
 * @author Adrián Kálazi, Kevin Lackó, Julie Gyselová
 * @brief Grammar rules for parser
 * @date 2020-12-08
 *
 */

#ifndef IFJ_RULES_H
#define IFJ_RULES_H

#include "parser.h"

/**
 * @brief Root rule - define builtin functions,...
 *
 * @param parser
 * @return int
 */
int rule_root(parser_t parser);

/**
 * @brief Rule EOL_OPT -> EOL | eps
 *
 * @param parser
 * @return int
 */
int rule_eol_opt_n(parser_t parser);

#endif // !IFJ_RULES_H
