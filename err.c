/*
 * A n t l r  S e t s / E r r o r  F i l e  H e a d e r
 *
 * Generated from: plumber.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-2001
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"

#include <string>
#include <iostream>
using namespace std;

// struct to store information about tokens
typedef struct {
  string kind;
  string text;
} Attrib;

// function to fill token information (predeclaration)
void zzcr_attr(Attrib *attr, int type, char *text);

// fields for AST nodes
#define AST_FIELDS string kind; string text;
#include "ast.h"

// macro to create a new AST node (and function predeclaration)
#define zzcr_ast(as,attr,ttype,textt) as=createASTnode(attr,ttype,textt)
AST* createASTnode(Attrib* attr, int ttype, char *textt);
#define zzSET_SIZE 8
#include "antlr.h"
#include "ast.h"
#include "tokens.h"
#include "dlgdef.h"
#include "err.h"

ANTLRChar *zztokens[36]={
	/* 00 */	"Invalid",
	/* 01 */	"@",
	/* 02 */	"NUM",
	/* 03 */	"LP",
	/* 04 */	"RP",
	/* 05 */	"COMMA",
	/* 06 */	"ADD",
	/* 07 */	"SUB",
	/* 08 */	"MULT",
	/* 09 */	"LT",
	/* 10 */	"GT",
	/* 11 */	"EQ",
	/* 12 */	"OR",
	/* 13 */	"AND",
	/* 14 */	"NOT",
	/* 15 */	"SET",
	/* 16 */	"WHILE",
	/* 17 */	"ENDWHILE",
	/* 18 */	"TUBEVECTOR",
	/* 19 */	"OF",
	/* 20 */	"CONNECTOR",
	/* 21 */	"TUBE",
	/* 22 */	"LENGTH",
	/* 23 */	"DIAMETER",
	/* 24 */	"SPLIT",
	/* 25 */	"MERGE",
	/* 26 */	"PUSH",
	/* 27 */	"POP",
	/* 28 */	"FULL",
	/* 29 */	"EMPTY",
	/* 30 */	"WRITE",
	/* 31 */	"CHECK",
	/* 32 */	"COMMENT",
	/* 33 */	"SPACE",
	/* 34 */	"ID",
	/* 35 */	"OTHERS"
};
SetWordType zzerr1[8] = {0x8,0x0,0xc1,0xcc, 0x4,0x0,0x0,0x0};
SetWordType zzerr2[8] = {0x0,0x0,0x34,0x2, 0x4,0x0,0x0,0x0};
SetWordType zzerr3[8] = {0x8,0x0,0x0,0x0, 0x4,0x0,0x0,0x0};
SetWordType setwd1[36] = {0x0,0x0,0x0,0xe5,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0xe1,0xe2,0x0,0x0,0x0,0x0,0xe9,
	0xe9,0x0,0x0,0xf1,0xf1,0x0,0x0,0xe1,
	0xe1,0x0,0x0,0xe5,0x0};
SetWordType zzerr4[8] = {0x0,0x0,0x0,0x2, 0x4,0x0,0x0,0x0};
SetWordType zzerr5[8] = {0x0,0x0,0x0,0xc, 0x0,0x0,0x0,0x0};
SetWordType zzerr6[8] = {0x0,0x0,0xc0,0x0, 0x0,0x0,0x0,0x0};
SetWordType setwd2[36] = {0x0,0x0,0x0,0xff,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0xff,0xff,0x0,0x0,0x0,0x0,0xff,
	0xff,0x0,0x0,0xff,0xff,0x0,0x0,0xff,
	0xff,0x0,0x0,0xff,0x0};
SetWordType zzerr7[8] = {0x0,0x0,0x0,0x30, 0x0,0x0,0x0,0x0};
SetWordType zzerr8[8] = {0x0,0x0,0xc0,0x0, 0x4,0x0,0x0,0x0};
SetWordType zzerr9[8] = {0xc,0x40,0xc0,0x30, 0x0,0x0,0x0,0x0};
SetWordType setwd3[36] = {0x0,0x0,0x81,0xf7,0x63,0x0,0x1,
	0x1,0x1,0x1,0x1,0x1,0x43,0x3,0x0,
	0x0,0x77,0x77,0x0,0x0,0x0,0x0,0xff,
	0xff,0x0,0x0,0x77,0x77,0x80,0x80,0x77,
	0x77,0x0,0x0,0x77,0x0};
SetWordType zzerr10[8] = {0xc,0x0,0xc0,0x30, 0x0,0x0,0x0,0x0};
SetWordType zzerr11[8] = {0x0,0xe,0x0,0x0, 0x0,0x0,0x0,0x0};
SetWordType zzerr12[8] = {0xc0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0};
SetWordType setwd4[36] = {0x0,0x0,0xc2,0xd9,0xd9,0x0,0xa0,
	0xa0,0x0,0xc0,0xc0,0xc0,0xd9,0xd9,0x0,
	0x0,0xd9,0xd9,0x0,0x0,0x0,0x0,0xdb,
	0xdb,0x0,0x0,0xd9,0xd9,0x4,0x4,0xd9,
	0xd9,0x0,0x0,0xd9,0x0};
SetWordType zzerr13[8] = {0x4,0x0,0xc0,0x0, 0x0,0x0,0x0,0x0};
SetWordType setwd5[36] = {0x0,0x0,0x2,0x2,0x2,0x0,0x2,
	0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x0,
	0x0,0x2,0x2,0x0,0x0,0x0,0x0,0x3,
	0x3,0x0,0x0,0x2,0x2,0x0,0x0,0x2,
	0x2,0x0,0x0,0x2,0x0};