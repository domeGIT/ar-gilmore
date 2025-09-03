#ifndef FIRST_ORDER_LOGIC_H
#define FIRST_ORDER_LOGIC_H
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<map>
#include<set>
#include <variant>
#include<unordered_set>
#include<memory>
#include "interpretation.h"
struct True;
struct False;
struct Atom;
struct Not;
struct Binary;
struct VariableTerm;
struct FunctionTerm;
struct Quantifier;

using Formula=std::variant<True,False,Atom,Not,Binary,Quantifier>;
using FormulaPtr=std::shared_ptr<Formula>;

using Term=std::variant<VariableTerm,FunctionTerm>;
using TermPtr=std::shared_ptr<Term>;
struct VariableTerm{std::string symbol;};
struct FunctionTerm{std::string symbol;std::vector<TermPtr> args;};

//----------------------------------------------------------------------
struct False {};
struct True {};
struct Atom{ std::string name; std::vector<TermPtr> args;};
struct Not {FormulaPtr subformula;};
struct Binary{
	enum Type {And, Or,Imp,Eq} type;
	FormulaPtr left,right;
};
struct Quantifier{
	enum Type { All, Exist} type;
	std::string var;
	FormulaPtr subformula;
 };


struct Literal {
    bool pos;
    std::string name;
};
using Clause = std::vector<Literal>;
using NormalForm = std::vector<Clause>;
//------------------------------------------------------------------------------

FormulaPtr ptr(const Formula& f);

template<typename T>
bool is(const FormulaPtr& f);

template<typename T>
T as(const FormulaPtr& f);

template<typename T>
bool is(const TermPtr& t);
template<typename T>
T as(const TermPtr& t);
TermPtr ptr(const Term& f);

void print(FormulaPtr f);
void print(TermPtr term);

bool checkSignature(TermPtr term,const Signature& s);
bool checkSignature(FormulaPtr f, Signature& s);

unsigned evaluate(TermPtr term,LStructure& l,Valuation& val);
bool evaluate(FormulaPtr f,LStructure& l,Valuation& val);

std::string termToString(TermPtr term);

void getVariables(TermPtr term,std::set<std::string>& vars);
void getVariables(FormulaPtr f,std::set<std::string>& vars, bool free);
bool containsVariable(TermPtr term,std::string v);
bool containsVariable(FormulaPtr f,std::string& v,bool free);

TermPtr substitute(TermPtr term, const std::string &v, TermPtr t);
FormulaPtr substitute(FormulaPtr f, const std::string &v, TermPtr t);


FormulaPtr simplify(FormulaPtr f);

FormulaPtr nnf(FormulaPtr f);
FormulaPtr nnfNot(FormulaPtr f);


template<typename T1, typename T2>
std::string uniqueVariable(T1 f, T2 g);

FormulaPtr pullQuantifiers(FormulaPtr f);
FormulaPtr prenex(FormulaPtr f);
FormulaPtr skolem(FormulaPtr f, Signature& s, std::set<std::string>& vars);
FormulaPtr skolem(FormulaPtr formula, Signature& s);

std::string atomToString(FormulaPtr f);

void print(const NormalForm& f);

template<typename List>
List concat(const List& l, const List& r) {
    List result;
    std::copy(begin(l), end(l), std::back_inserter(result));
    std::copy(begin(r), end(r), std::back_inserter(result));
    return result;
}

//NormalForm concat(const NormalForm& a, const NormalForm& b);

NormalForm cross(const NormalForm& l, const NormalForm& r);


NormalForm dnf(const FormulaPtr& f); 
FormulaPtr removeUniversal(FormulaPtr f);



#endif
