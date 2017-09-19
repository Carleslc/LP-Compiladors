#ifndef tokens_h
#define tokens_h
/* tokens.h -- List of labelled tokens and stuff
 *
 * Generated from: plumber.g
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * ANTLR Version 1.33MR33
 */
#define zzEOF_TOKEN 1
#define NUM 2
#define LP 3
#define RP 4
#define COMMA 5
#define ADD 6
#define SUB 7
#define MULT 8
#define LT 9
#define GT 10
#define EQ 11
#define OR 12
#define AND 13
#define NOT 14
#define SET 15
#define WHILE 16
#define ENDWHILE 17
#define TUBEVECTOR 18
#define OF 19
#define CONNECTOR 20
#define TUBE 21
#define LENGTH 22
#define DIAMETER 23
#define SPLIT 24
#define MERGE 25
#define PUSH 26
#define POP 27
#define FULL 28
#define EMPTY 29
#define WRITE 30
#define CHECK 31
#define COMMENT 32
#define SPACE 33
#define ID 34
#define OTHERS 35

#ifdef __USE_PROTOS
void plumber(AST**_root);
#else
extern void plumber();
#endif

#ifdef __USE_PROTOS
void ops(AST**_root);
#else
extern void ops();
#endif

#ifdef __USE_PROTOS
void assign(AST**_root);
#else
extern void assign();
#endif

#ifdef __USE_PROTOS
void split(AST**_root);
#else
extern void split();
#endif

#ifdef __USE_PROTOS
void split_eval(AST**_root);
#else
extern void split_eval();
#endif

#ifdef __USE_PROTOS
void merge(AST**_root);
#else
extern void merge();
#endif

#ifdef __USE_PROTOS
void merge_param(AST**_root);
#else
extern void merge_param();
#endif

#ifdef __USE_PROTOS
void vector_ops(AST**_root);
#else
extern void vector_ops();
#endif

#ifdef __USE_PROTOS
void loop(AST**_root);
#else
extern void loop();
#endif

#ifdef __USE_PROTOS
void tube(AST**_root);
#else
extern void tube();
#endif

#ifdef __USE_PROTOS
void connector(AST**_root);
#else
extern void connector();
#endif

#ifdef __USE_PROTOS
void vector(AST**_root);
#else
extern void vector();
#endif

#ifdef __USE_PROTOS
void parametric_accessor(AST**_root);
#else
extern void parametric_accessor();
#endif

#ifdef __USE_PROTOS
void vector_accessor(AST**_root);
#else
extern void vector_accessor();
#endif

#ifdef __USE_PROTOS
void check(AST**_root);
#else
extern void check();
#endif

#ifdef __USE_PROTOS
void write(AST**_root);
#else
extern void write();
#endif

#ifdef __USE_PROTOS
void bool_expr(AST**_root);
#else
extern void bool_expr();
#endif

#ifdef __USE_PROTOS
void bool_term(AST**_root);
#else
extern void bool_term();
#endif

#ifdef __USE_PROTOS
void bool_term_not(AST**_root);
#else
extern void bool_term_not();
#endif

#ifdef __USE_PROTOS
void bool_atom(AST**_root);
#else
extern void bool_atom();
#endif

#ifdef __USE_PROTOS
void bool_term_rel(AST**_root);
#else
extern void bool_term_rel();
#endif

#ifdef __USE_PROTOS
void num_expr(AST**_root);
#else
extern void num_expr();
#endif

#ifdef __USE_PROTOS
void num_term(AST**_root);
#else
extern void num_term();
#endif

#ifdef __USE_PROTOS
void num_atom(AST**_root);
#else
extern void num_atom();
#endif

#endif
extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType zzerr3[];
extern SetWordType setwd1[];
extern SetWordType zzerr4[];
extern SetWordType zzerr5[];
extern SetWordType zzerr6[];
extern SetWordType setwd2[];
extern SetWordType zzerr7[];
extern SetWordType zzerr8[];
extern SetWordType zzerr9[];
extern SetWordType setwd3[];
extern SetWordType zzerr10[];
extern SetWordType zzerr11[];
extern SetWordType zzerr12[];
extern SetWordType setwd4[];
extern SetWordType zzerr13[];
extern SetWordType setwd5[];
