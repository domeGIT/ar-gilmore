#ifndef INTERPRETATION_H
#define INTERPRETATION_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <any>
#include <functional>

//sadrzi funkcije,relacije i njihove arnosti
struct Signature{
 std::map<std::string, unsigned> rel;
 std::map<std::string, unsigned> fun;
 std::string getUniqueSymbol(unsigned arity) {
        static unsigned uniqueCounter = 0;
        std::string uniqueSymbol;
        do {
            uniqueSymbol = "f" + std::to_string(++uniqueCounter);
        } while(fun.find(uniqueSymbol) != fun.end());
        fun[uniqueSymbol] = arity;
        return uniqueSymbol;
    }

};

using Domain= std::set<unsigned>;
using Valuation=std::map<std::string,unsigned>;

using Function = std::function<unsigned(const std::vector<unsigned>&)>;
using Relation = std:: function<bool(const std::vector<unsigned>&)>;

struct LStructure
{
 Signature signature;
    Domain domain;
    std::map<std::string, Function> functions;
    std::map<std::string, Relation> relations;
};
#endif
