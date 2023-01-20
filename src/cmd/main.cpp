#include <ast.hpp>
#include <fmt/format.h>
#include <fstream>
#include <parser.hpp>

using namespace mcc::literals;

// int main(int argc, char **argv) {
//   auto regex = "{' '} ~ 'sus'"_rx;

//   // auto regex = "'#' !{'\n'}*"_rx;
//   fmt::print("{}\n", regex);

// //   auto expr = R"("
// // #define new(n)\
// // malloc(n)
// // ")";

//   auto expr = R"("Hello world\"" //the next expression)";
//   auto match = regex.match(expr);
//   fmt::print("/* {} => {} */", match.expr(), match.view());
// }

// void dump_tokens(mcc::Lexer &lexer) {
//   auto [expr, trait] = lexer.tokenize();
//   fmt::print("{:>30} : {}\n", expr, mcc::trait_name(trait));

//   if (trait != mcc::trait::End) {
//     dump_tokens(lexer);
//   }
// }

// int main(int argc, char **argv) {
//   if (argc < 2) {
//     fmt::print("mcc - micro c compiler\n");
//     return 0;
//   }

//   try {
//     std::fstream fstream{argv[1]};
//     if (!fstream) {
//       throw mcc::Exception{"fs exception", "can't read file from '{}'", argv[1]};
//     }
//     std::string src{
//       std::istreambuf_iterator<char>(fstream),
//       std::istreambuf_iterator<char>(),
//     };

//     mcc::Lexer lexer{src, mcc::syntax_ansi()};
//     dump_tokens(lexer);
//   } catch (const mcc::Exception &exception) {
//     fmt::print(stderr, "Mcc {} {}\n", exception.name(), exception.what());
//   } catch (const std::exception &exception) {
//     fmt::print(stderr, "{}\n", exception.what());
//   }
// }

namespace mcc {
constexpr std::string_view SOURCE = R"(
#include "builtin.h"
#include "config.h"
#include "refs.h"

static const char var_usage[] = " git var (-l | <variable>) ";

static const char *editor(int flag)
{
	return git_editor();
}

static const char *sequence_editor(int flag)
{
	return git_sequence_editor();
}

static const char *pager(int flag)
{
	const char *pgm = git_pager(1);

	if (!pgm)
		pgm = "cat";
	return pgm;
}

static const char *default_branch(int flag)
{
	return git_default_branch_name(1);
}

struct git_var {
	const char *name;
	const char *(*read)(int);
};
static struct git_var git_vars[] = {
	{ "GIT_COMMITTER_IDENT", git_committer_info },
	{ "GIT_AUTHOR_IDENT",   git_author_info },
	{ "GIT_EDITOR", editor },
	{ "GIT_SEQUENCE_EDITOR", sequence_editor },
	{ "GIT_PAGER", pager },
	{ "GIT_DEFAULT_BRANCH", default_branch },
	{ "", NULL },
};

static void list_vars(void)
{
	struct git_var *ptr;
	const char *val;

	for (ptr = git_vars; ptr->read; ptr++)
		if ((val = ptr->read(0)))
			printf("%s=%s\n", ptr->name, val);
}

static const struct git_var *get_git_var(const char *var)
{
	struct git_var *ptr;
	for (ptr = git_vars; ptr->read; ptr++) {
		if (strcmp(var, ptr->name) == 0) {
			return ptr;
		}
	}
	return NULL;
}

static int show_config(const char *var, const char *value, void *cb)
{
	if (value)
		printf("%s=%s\n", var, value);
	else
		printf("%s\n", var);
	return git_default_config(var, value, cb);
}

int cmd_var(int argc, const char **argv, const char *prefix)
{
	const struct git_var *git_var;
	const char *val;

	if (argc != 2)
		usage(var_usage);

	if (strcmp(argv[1], "-l") == 0) {
		git_config(show_config, NULL);
		list_vars();
		return 0;
	}
	git_config(git_default_config, NULL);

	git_var = get_git_var(argv[1]);
	if (!git_var)
		usage(var_usage);

	val = git_var->read(IDENT_STRICT);
	if (!val)
		return 1;

	printf("%s\n", val);

	return 0;
}
)";

void parse_source() {
  Parser parser{Lexer(mcc::SOURCE, syntax_ansi())};
  Ast &ast = parser.parse();
}

}  // namespace mcc

int main() {
  mcc::parse_source();
}
