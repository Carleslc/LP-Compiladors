/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 *
 *   ../pccts/bin/antlr -gt plumber.g
 *
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
#define GENAST

#include "ast.h"

#define zzSET_SIZE 8
#include "antlr.h"
#include "tokens.h"
#include "dlgdef.h"
#include "mode.h"

/* MR23 In order to remove calls to PURIFY use the antlr -nopurify option */

#ifndef PCCTS_PURIFY
#define PCCTS_PURIFY(r,s) memset((char *) &(r),'\0',(s));
#endif

#include "ast.c"
zzASTgvars

ANTLR_INFO

#include <cstdlib>
#include <cmath>
#include <map>
#include <stack>

struct Tube {
  string id;
  int length, diameter;
  Tube() {}
  Tube(string i, int l, int d) : id(i), length(l), diameter(d) {}
  Tube(string i, const Tube& copy) : id(i), length(copy.length), diameter(copy.diameter) {}
  
    void print() const {
    cout << id << ":\tTUBE " << length << " " << diameter << endl;
  }
};

struct Connector {
  string id;
  int diameter;
  Connector() {}
  Connector(string i, int d) : id(i), diameter(d) {}
  Connector(string i, const Connector& copy) : id(i), diameter(copy.diameter) {}
  
    void print() const {
    cout << id << ":\tCONNECTOR " << diameter << endl;
  }
};

struct TubeVector {
  string id;
  int capacity;
  stack<Tube> internal;
  
    TubeVector() {}
  TubeVector(string i, int c) : id(i), capacity(c) {}
  TubeVector(string i, const TubeVector& copy) : id(i), capacity(copy.capacity) {
    internal = copy.internal;
  }
  
    bool empty() const { return internal.empty(); }
  bool full() const { return internal.size() == capacity; }
  
    bool pop(Tube& tube, string& error) {
    if (empty()) {
      error = "Cannot pop from " + id + " because vector is empty!";
      return false;
    }
    tube = internal.top();
    internal.pop();
    return true;
  }
  
    bool push(Tube from, string& error) {
    if (full()) {
      error = "Cannot push " + from.id + " to " + id + " because vector is full!";
      return false;
    }
    internal.push(from);
    return true;
  }
  
    void print() const {
    cout << id << ":\tTUBEVECTOR OF " << capacity;
    if (empty()) cout << " (EMPTY)" << endl;
    else {
      if (full()) cout << " (FULL)" << endl;
      else cout << " (CURRENT SIZE " << internal.size() << ")" << endl;
      stack<Tube> copy = this->internal;
      int i = internal.size() - 1;
      print_recursive(copy, i);
    }
  }
  
    private:
  void print_recursive(stack<Tube>& copy, int i) const {
    if (!copy.empty()) {
      const Tube& tube = copy.top();
      copy.pop();
      print_recursive(copy, i - 1);
      cout << id << "[" << i << "]:\t";
      tube.print();
    }
  }
};

map<string, Tube> tubes;
map<string, Connector> connectors;
map<string, TubeVector > vectors;

// function to fill token information
void zzcr_attr(Attrib *attr, int type, char *text) {
  if (type == NUM) {
    attr->kind = "int";
    attr->text = text;
  }
  else if (type == ID) {
    attr->kind = "ID";
    attr->text = text;
  }
  else {
    attr->kind = text;
    attr->text = "";
  }
}

// function to create a new AST node
AST* createASTnode(Attrib* attr, int type, char* text) {
  AST* as = new AST;
  as->kind = attr->kind; 
  as->text = attr->text;
  as->right = NULL; 
  as->down = NULL;
  return as;
}

/// create a new "list" AST node with one element
AST* createASTlist(AST *child) {
  AST *as=new AST;
  as->kind="list";
  as->right=NULL;
  as->down=child;
  return as;
}

/// get nth child of a tree. Count starts at 0.
/// if no such child, returns NULL
AST* child(AST *a,int n) {
  AST *c=a->down;
  for (int i=0; c!=NULL && i<n; i++) c=c->right;
  return c;
} 

/// print AST, recursively, with indentation
void ASTPrintIndent(AST *a,string s)
{
  if (a==NULL) return;
  
  if (a->kind != "ID" and a->kind != "int")cout<<a->kind;
  if (a->text!="") cout<<a->text;
  cout<<endl;
  
  AST *i = a->down;
  while (i!=NULL && i->right!=NULL) {
    cout<<s+"  \\__";
    ASTPrintIndent(i,s+"  |"+string(i->kind.size()+i->text.size(),' '));
    i=i->right;
  }
  
  if (i!=NULL) {
    cout<<s+"  \\__";
    ASTPrintIndent(i,s+"   "+string(i->kind.size()+i->text.size(),' '));
    i=i->right;
  }
}

/// print AST
void ASTPrint(AST *a)
{
  while (a!=NULL) {
    cout<<" ";
    ASTPrintIndent(a,"");
    a=a->right;
  }
}

bool failed(const string& error) {
  return error != "";
}

bool check(const string& error) {
  if (failed(error)) cout << error << endl;
}

bool exists_tube(const string& id) {
  return tubes.count(id) > 0;
}

bool exists_connector(const string& id) {
  return connectors.count(id) > 0;
}

bool exists_vector(const string& id) {
  return vectors.count(id) > 0;
}

int length(const string& id, string& error) {
  if (exists_tube(id)) return tubes[id].length;
  else error = id + " not exists!";
}

int diameter(const string& id, string& error) {
  if (exists_tube(id)) return tubes[id].diameter;
  else if (exists_connector(id)) return connectors[id].diameter;
  else error = id + " not exists!";
}

bool empty(const string& id, string& error) {
  if (exists_vector(id)) {
    return vectors[id].empty();
  } else if (exists_tube(id) or exists_connector(id)) {
    error = id + " is not a tube vector!";
  } else error = id + " not exists!";
}

bool full(const string& id, string& error) {
  if (exists_vector(id)) {
    return vectors[id].full();
  } else if (exists_tube(id) or exists_connector(id)) {
    error = id + " is not a tube vector!";
  } else error = id + " not exists!";
}

void push(const string& from_id, const string& tube_id, string& error) {
  if (exists_vector(from_id)) {
    if (exists_tube(tube_id)) {
      if (vectors[from_id].push(tubes[tube_id], error)) {
        tubes.erase(tube_id);
      }
    }
    else if (exists_connector(tube_id) or exists_vector(tube_id)) {
      error = tube_id + " is not a tube!";
    } else error = tube_id + " not exists!";
  } else if (exists_tube(from_id) or exists_connector(from_id)) {
    error = from_id + " is not a tube vector!";
  } else error = from_id + " not exists!";
}

void pop(const string& from_id, const string& to_id, string& error) {
  if (exists_vector(from_id)) {
    Tube top;
    if (vectors[from_id].pop(top, error)) {
      top.id = to_id;
      tubes[to_id] = top;
    }
  } else if (exists_tube(from_id) or exists_connector(from_id)) {
    error = from_id + " is not a tube vector!";
  } else error = from_id + " not exists!";
}

int evaluate_num(AST *a, string& error) {
  if (a == NULL) return 0;
  if (a->kind == "+") return evaluate_num(child(a, 0), error) + evaluate_num(child(a, 1), error);
  if (a->kind == "-") return evaluate_num(child(a, 0), error) - evaluate_num(child(a, 1), error);
  if (a->kind == "*") return evaluate_num(child(a, 0), error) * evaluate_num(child(a, 1), error);
  if (a->kind == "LENGTH") return length(child(a, 0)->text, error);
  if (a->kind == "DIAMETER") return diameter(child(a, 0)->text, error);
  return atoi(a->text.c_str());
}

bool evaluate_bool(AST *a, string& error) {
  if (a == NULL) return false;
  if (a->kind == ">") return evaluate_num(child(a, 0), error) > evaluate_num(child(a, 1), error);
  if (a->kind == "<") return evaluate_num(child(a, 0), error) < evaluate_num(child(a, 1), error);
  if (a->kind == "==") return evaluate_num(child(a, 0), error) == evaluate_num(child(a, 1), error);
  if (a->kind == "NOT") return not evaluate_bool(child(a, 0), error);
  if (a->kind == "AND") return evaluate_bool(child(a, 0), error) and evaluate_bool(child(a, 1), error);
  if (a->kind == "OR") return evaluate_bool(child(a, 0), error) or evaluate_bool(child(a, 1), error);
  if (a->kind == "FULL") return full(child(a,0)->text, error);
  if (a->kind == "EMPTY") return empty(child(a,0)->text, error);
}

Tube merge(const string& id, const Tube& first, const Connector& connector, const Tube& second) {
  tubes.erase(first.id);
  tubes.erase(second.id);
  connectors.erase(connector.id);
  return tubes[id] = Tube(id, first.length + second.length, connector.diameter);
}

Tube merge(AST *a, const string& id, string& error) {
  if (a->kind == "MERGE") {
    string connector_id = child(a,1)->text;
    if (exists_connector(connector_id)) {
      Connector connector = connectors[connector_id];
      Tube first = merge(child(a,0), id, error);
      if (not failed(error)) {
        if (connector.diameter == first.diameter) {
          Tube second = merge(child(a,2), id, error);
          if (not failed(error)) {
            if (connector.diameter == second.diameter) {
              return merge(id, first, connector, second);
            }
            else error = "Unmatch diameter between " + connector.id + " and " + second.id;
          }
        }
        else error = "Unmatch diameter between " + connector.id + " and " + first.id;
      }
    }
    else if (exists_tube(id) or exists_vector(id)) {
      error = id + " is not a connector!";
    } else error = id + " not exists!";
  }
  else {
    string tube_id = a->text;
    if (exists_tube(tube_id)) return tubes[tube_id];
    else if (exists_connector(tube_id) or exists_vector(tube_id)) {
      error = tube_id + " is not a tube!";
    } else error = tube_id + " not exists!";
  }
  return Tube();
}

void split(const string& from_id, const string& id1, const string& id2, string& error) {
  if (exists_tube(from_id)) {
    Tube from = tubes[from_id];
    int splitted_length1 = from.length/2;
    if (splitted_length1 == 0) error = from_id + " cannot be splitted because it has length 1";
    else {
      int splitted_length2 = splitted_length1;
      if (from.length%2 != 0) splitted_length2++;
      int splitted_diameter = from.diameter;
      tubes.erase(from_id);
      tubes[id1] = Tube(id1, splitted_length1, splitted_diameter);
      tubes[id2] = Tube(id2, splitted_length2, splitted_diameter);
    }
  }
  else error = from_id + " not exists!";
}

bool is_split(AST *a) {
  AST* third = child(a,2);
  return third != NULL and third->kind == "SPLIT";
}

void set(const string& id, AST *a, string& error) {
  if (a->kind == "TUBE") {
    int length = evaluate_num(child(a,0), error);
    if (not failed(error)) {
      int diameter = evaluate_num(child(a,1), error);
      if (not failed(error)) tubes[id] = Tube(id, length, diameter);
    }
  }
  else if (a->kind == "CONNECTOR") {
    int diameter = evaluate_num(child(a,0), error);
    if (not failed(error)) connectors[id] = Connector(id, diameter);
  }
  else if (a->kind == "TUBEVECTOR") {
    int capacity = evaluate_num(child(a,0), error);
    if (not failed(error)) vectors[id] = TubeVector(id, capacity);
  }
  else if (a->kind == "MERGE") {
    Tube old;
    bool to_replace = exists_tube(id);
    if (to_replace) old = tubes[id];
    merge(a, id, error);
    if (failed(error)) {
      tubes.erase(id); // clear intermediate data
      if (to_replace) tubes[id] = old;
    }
  }
  else {
    string from_id = a->text;
    if (exists_tube(from_id)) tubes[id] = Tube(id, tubes[from_id]);
    else if (exists_connector(from_id)) connectors[id] = Connector(id, connectors[from_id]);
    else if (exists_vector(from_id)) vectors[id] = TubeVector(id, vectors[from_id]);
    else error = from_id + " not exists!";
  }
}

void print(const string& msg, string& error) {
  if (not failed(error)) cout << msg << endl;
}

void print(int accessor, string& error) {
  if (not failed(error)) cout << accessor << endl;
}

void show_status() {
  cout << "-- END STATUS --" << endl;
  for (map<string, Tube>::const_iterator it = tubes.begin(); it != tubes.end(); it++) {
    const Tube& tube = it->second;
    tube.print();
  }
  for (map<string, Connector>::const_iterator it = connectors.begin(); it != connectors.end(); it++) {
    const Connector& connector = it->second;
    connector.print();
  }
  for (map<string, TubeVector>::const_iterator it = vectors.begin(); it != vectors.end(); it++) {
    const TubeVector& vector = it->second;
    vector.print();
  }
}

void write(AST *a, string& error) {
  if (a->kind == "ID") {
    string id = a->text;
    if (exists_tube(id)) tubes[id].print();
    else if (exists_connector(id)) connectors[id].print();
    else if (exists_vector(id)) vectors[id].print();
    else error = id + " not exists!";
  }
  else print(evaluate_num(a, error), error);
}

void execute(AST *a) {
  if (a == NULL) return;
  string error = "";
  if (a->kind == "=") {
    if (is_split(a)) split(child(child(a,2),0)->text, child(a,0)->text, child(a,1)->text, error);
    else set(child(a,0)->text, child(a,1), error);
  }
  else if (a->kind == "WHILE") {
    AST *bool_expr = child(a,0);
    AST *execution = child(a,1)->down;
    while (evaluate_bool(bool_expr, error) and not failed(error)) execute(execution);
  }
  else if (a->kind == "LENGTH") print(length(child(a,0)->text, error), error);
  else if (a->kind == "DIAMETER") print(diameter(child(a,0)->text, error), error);
  else if (a->kind == "PUSH") push(child(a,0)->text, child(a,1)->text, error);
  else if (a->kind == "POP") pop(child(a,0)->text, child(a,1)->text, error);
  else if (a->kind == "WRITE") write(child(a,0), error);
  else if (a->kind == "CHECK") print((evaluate_bool(child(a,0), error) ? "true" : "false"), error);
  check(error);
  execute(a->right);
}

int main() {
  AST *root = NULL;
  ANTLR(plumber(&root), stdin);
  ASTPrint(root);
  execute(root->down);
  show_status();
}

void
#ifdef __USE_PROTOS
plumber(AST**_root)
#else
plumber(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd1[LA(1)]&0x1) ) {
      ops(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x2);
  }
}

void
#ifdef __USE_PROTOS
ops(AST**_root)
#else
ops(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (setwd1[LA(1)]&0x4) ) {
    assign(zzSTR); zzlink(_root, &_sibling, &_tail);
  }
  else {
    if ( (LA(1)==WHILE) ) {
      loop(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (setwd1[LA(1)]&0x8) ) {
        parametric_accessor(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (setwd1[LA(1)]&0x10) ) {
          vector_ops(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {
          if ( (LA(1)==CHECK) ) {
            check(zzSTR); zzlink(_root, &_sibling, &_tail);
          }
          else {
            if ( (LA(1)==WRITE) ) {
              write(zzSTR); zzlink(_root, &_sibling, &_tail);
            }
            else {zzFAIL(1,zzerr1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
        }
      }
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x20);
  }
}

void
#ifdef __USE_PROTOS
assign(AST**_root)
#else
assign(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==LP) ) {
    split(zzSTR); zzlink(_root, &_sibling, &_tail);
  }
  else {
    if ( (LA(1)==ID) ) {
      {
        zzBLOCK(zztasp2);
        zzMake0;
        {
        zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
        zzmatch(SET); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        {
          zzBLOCK(zztasp3);
          zzMake0;
          {
          if ( (LA(1)==ID) ) {
            zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {
            if ( (LA(1)==TUBE) ) {
              tube(zzSTR); zzlink(_root, &_sibling, &_tail);
            }
            else {
              if ( (LA(1)==CONNECTOR) ) {
                connector(zzSTR); zzlink(_root, &_sibling, &_tail);
              }
              else {
                if ( (LA(1)==TUBEVECTOR) ) {
                  vector(zzSTR); zzlink(_root, &_sibling, &_tail);
                }
                else {
                  if ( (LA(1)==MERGE) ) {
                    merge(zzSTR); zzlink(_root, &_sibling, &_tail);
                  }
                  else {zzFAIL(1,zzerr2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                }
              }
            }
          }
          zzEXIT(zztasp3);
          }
        }
        zzEXIT(zztasp2);
        }
      }
    }
    else {zzFAIL(1,zzerr3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x40);
  }
}

void
#ifdef __USE_PROTOS
split(AST**_root)
#else
split(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(LP);  zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(COMMA);  zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(RP);  zzCONSUME;
  zzmatch(SET); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  split_eval(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x80);
  }
}

void
#ifdef __USE_PROTOS
split_eval(AST**_root)
#else
split_eval(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(SPLIT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x1);
  }
}

void
#ifdef __USE_PROTOS
merge(AST**_root)
#else
merge(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(MERGE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  merge_param(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  merge_param(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x2);
  }
}

void
#ifdef __USE_PROTOS
merge_param(AST**_root)
#else
merge_param(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==MERGE) ) {
    merge(zzSTR); zzlink(_root, &_sibling, &_tail);
  }
  else {
    if ( (LA(1)==ID) ) {
      zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {zzFAIL(1,zzerr4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x4);
  }
}

void
#ifdef __USE_PROTOS
vector_ops(AST**_root)
#else
vector_ops(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==PUSH) ) {
      zzmatch(PUSH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==POP) ) {
        zzmatch(POP); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {zzFAIL(1,zzerr5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x8);
  }
}

void
#ifdef __USE_PROTOS
loop(AST**_root)
#else
loop(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(WHILE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LP);  zzCONSUME;
  bool_expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RP);  zzCONSUME;
  plumber(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(ENDWHILE);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x10);
  }
}

void
#ifdef __USE_PROTOS
tube(AST**_root)
#else
tube(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(TUBE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  num_expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  num_expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x20);
  }
}

void
#ifdef __USE_PROTOS
connector(AST**_root)
#else
connector(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(CONNECTOR); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  num_expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x40);
  }
}

void
#ifdef __USE_PROTOS
vector(AST**_root)
#else
vector(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(TUBEVECTOR); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(OF);  zzCONSUME;
  num_expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x80);
  }
}

void
#ifdef __USE_PROTOS
parametric_accessor(AST**_root)
#else
parametric_accessor(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==LENGTH) ) {
      zzmatch(LENGTH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==DIAMETER) ) {
        zzmatch(DIAMETER); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {zzFAIL(1,zzerr6,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzmatch(LP);  zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(RP);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x1);
  }
}

void
#ifdef __USE_PROTOS
vector_accessor(AST**_root)
#else
vector_accessor(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==FULL) ) {
      zzmatch(FULL); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==EMPTY) ) {
        zzmatch(EMPTY); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {zzFAIL(1,zzerr7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzmatch(LP);  zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(RP);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x2);
  }
}

void
#ifdef __USE_PROTOS
check(AST**_root)
#else
check(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(CHECK); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  bool_expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x4);
  }
}

void
#ifdef __USE_PROTOS
write(AST**_root)
#else
write(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(WRITE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==ID) ) {
      zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (setwd3[LA(1)]&0x8) ) {
        parametric_accessor(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {zzFAIL(1,zzerr8,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x10);
  }
}

void
#ifdef __USE_PROTOS
bool_expr(AST**_root)
#else
bool_expr(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  bool_term(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==OR) ) {
      zzmatch(OR); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      bool_term(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x20);
  }
}

void
#ifdef __USE_PROTOS
bool_term(AST**_root)
#else
bool_term(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  bool_term_not(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==AND) ) {
      zzmatch(AND); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      bool_term_not(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x40);
  }
}

void
#ifdef __USE_PROTOS
bool_term_not(AST**_root)
#else
bool_term_not(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==NOT) ) {
      zzmatch(NOT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (setwd3[LA(1)]&0x80) ) {
      }
      else {zzFAIL(1,zzerr9,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  bool_atom(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x1);
  }
}

void
#ifdef __USE_PROTOS
bool_atom(AST**_root)
#else
bool_atom(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==LP) ) {
    zzmatch(LP);  zzCONSUME;
    bool_expr(zzSTR); zzlink(_root, &_sibling, &_tail);
    zzmatch(RP);  zzCONSUME;
  }
  else {
    if ( (setwd4[LA(1)]&0x2) ) {
      bool_term_rel(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (setwd4[LA(1)]&0x4) ) {
        vector_accessor(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {zzFAIL(1,zzerr10,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x8);
  }
}

void
#ifdef __USE_PROTOS
bool_term_rel(AST**_root)
#else
bool_term_rel(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  num_expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==LT) ) {
      zzmatch(LT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==GT) ) {
        zzmatch(GT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {
        if ( (LA(1)==EQ) ) {
          zzmatch(EQ); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {zzFAIL(1,zzerr11,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
    zzEXIT(zztasp2);
    }
  }
  num_expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x10);
  }
}

void
#ifdef __USE_PROTOS
num_expr(AST**_root)
#else
num_expr(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  num_term(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd4[LA(1)]&0x20) ) {
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        if ( (LA(1)==ADD) ) {
          zzmatch(ADD); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==SUB) ) {
            zzmatch(SUB); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr12,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzEXIT(zztasp3);
        }
      }
      num_term(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x40);
  }
}

void
#ifdef __USE_PROTOS
num_term(AST**_root)
#else
num_term(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  num_atom(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==MULT) ) {
      zzmatch(MULT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      num_atom(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x80);
  }
}

void
#ifdef __USE_PROTOS
num_atom(AST**_root)
#else
num_atom(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==NUM) ) {
    zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  }
  else {
    if ( (setwd5[LA(1)]&0x1) ) {
      parametric_accessor(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {zzFAIL(1,zzerr13,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd5, 0x2);
  }
}
