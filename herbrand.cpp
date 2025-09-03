#include "interpretation.h"
#include "first_order_logic.h"
#include "herbrand.h"


void generatePermutationsWithRepetition(const std::set<TermPtr>& inputSet, int r, std::vector<TermPtr>& current, std::vector<std::vector<TermPtr>>& result) {
    if (current.size() == r) {
        result.push_back(current); 
        return;
    }

    
    for (auto it = inputSet.begin(); it != inputSet.end(); ++it) {
        current.push_back(*it); // Add the element
        generatePermutationsWithRepetition(inputSet, r, current, result); // Recurse
        current.pop_back();     // Remove the element (backtrack)
    }       
    
}




Herbrand generate(Signature& s,FormulaPtr f)
{
	Herbrand h;
	h.signature=s;	
	h.formula=f;
	h.functions={};
	std::vector<std::string> args;
	for(auto it: s.fun)
	h.functions.insert(it.first);

    for(auto funk : h.functions)
        if(s.fun[funk]==0)
            h.universe.insert(ptr(FunctionTerm{funk,{}}));
	
	 if(h.universe.empty()) {
        h.constant = s.getUniqueSymbol(0);
        h.universe.insert(ptr(FunctionTerm{h.constant,{}}));
    }

	return h;
};


Herbrand nextLevel(Herbrand h)
{
	std::vector<std::vector<TermPtr>> variations;
	std::set<TermPtr> help;
	help.insert(h.universe.cbegin(),h.universe.cend());
	h.universe.clear();

	if(h.constant!="")
	h.universe.insert(ptr(FunctionTerm{h.constant,{}}));
	
	for(auto funk: h.functions)
	{
		variations.clear();
		
		unsigned arity=h.signature.fun[funk];
		std::vector<TermPtr> current;
		
		generatePermutationsWithRepetition(help,arity,current,variations);
		
		
		 for(auto v : variations){
            h.universe.insert(ptr(FunctionTerm{funk,v}));
			}
	}
	

	return h;
}

