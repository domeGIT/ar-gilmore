#ifndef HERBRAND_H
#define HERBRAND_H
#include<set>
#include "interpretation.h"
#include "first_order_logic.h"

struct Herbrand
{
Signature signature;
FormulaPtr formula;
std::set<std::string> functions;
std::string constant;
std::set<TermPtr> universe;
};
void generatePermutationsWithRepetition(const std::set<TermPtr>& inputSet, int r, std::vector<TermPtr>& current, std::vector<std::vector<TermPtr>>& result);


Herbrand generate(Signature& s,FormulaPtr f);
Herbrand nextLevel(Herbrand h);



#endif