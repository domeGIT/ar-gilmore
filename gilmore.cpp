#include "herbrand.h"
#include "gilmore.h"
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<map>
#include<set>
#include <variant>
#include<unordered_set>

void gilmore(Signature& s,FormulaPtr f)
{
	FormulaPtr newFormula=removeUniversal(skolem(prenex(nnf(ptr(Not{f}))),s));
	Herbrand h=generate(s,newFormula);
	
	std::set<std::string> vars_set;
	getVariables(newFormula,vars_set,false);
	std::vector<std::string> vars;
	std::copy(vars_set.cbegin(),vars_set.cend(),::std::back_inserter(vars));
	unsigned numVars=vars.size();
	for(int i=0;i<4;i++)
	{
	
		std::vector<std::vector<TermPtr>> variations;
		std::vector<TermPtr> current;
		generatePermutationsWithRepetition(h.universe,numVars,current,variations);
		unsigned variation_length=variations.size();		
		FormulaPtr f1=newFormula;
		for(unsigned i = 0; i < numVars; i++)
		{
			
            f1=substitute(f1,vars[i],variations[0][i]);
		
		}
		for(unsigned i = 1; i < variation_length; i++){
			FormulaPtr fi=newFormula;
			for(unsigned j = 0; j < numVars; j++)
				fi=substitute(fi,vars[j],variations[i][j]);
			f1=ptr(Binary{Binary::And,f1,fi});

		
		}
		NormalForm f_dnf=dnf(f1);
		int num_false=0;
		
		for(Clause clause: f_dnf){
		
			std::unordered_set<std::string> pos;
			std::unordered_set<std::string> neg;	
			
			
			for(Literal l: clause)
			{
				if(l.pos==false)
				{
					if(pos.find(l.name)!=pos.end())
					{	
						num_false++;
						break;
					}	
					else
					{
						neg.insert(l.name);
					}
				}
				else if(l.pos==true)
				{
					if(neg.find(l.name)!=neg.end())
					{
						num_false++;
						break;
					}	
					else
					{
						pos.insert(l.name);
					}
				}
			}
			


			pos.clear();
			neg.clear();
			
		}
		if(num_false==f_dnf.size())
		{	
			print(f);
			std::cout<<" je valjana\n";
			return;
		}
		
		
			h=nextLevel(h);
		
		
	}
		
	std::cout<<" prevazisli maximalni broj iteracija\n";
}