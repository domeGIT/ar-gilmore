#include "first_order_logic.h"


FormulaPtr ptr(const Formula& f) { return std::make_shared<Formula>(f);};

template<typename T>
bool is(const FormulaPtr& f){return std::holds_alternative<T>(*f);};

template<typename T>
T as(const FormulaPtr& f) { return std::get<T>(*f); }

template<typename T>
bool is(const TermPtr& t){return std::holds_alternative<T>(*t);};
template<typename T>
T as(const TermPtr& t){return std::get<T>(*t);};
TermPtr ptr(const Term& f) { return std::make_shared<Term>(f);};




template<typename T1, typename T2>
std::string uniqueVariable(T1 f, T2 g) {
    static unsigned uniqueCounter = 0;
    std::string uniqueVar;
    do {
        uniqueVar = "u" + std::to_string(++uniqueCounter);
    } while(containsVariable(f, uniqueVar,false) || containsVariable(g, uniqueVar,false));
    return uniqueVar;
};





bool checkSignature(TermPtr term,const Signature& s)
{
	if(is<VariableTerm>(term))
		return true;
	if(is<FunctionTerm>(term))
	{
		auto it = s.fun.find(as<FunctionTerm>(term).symbol);
		if(it==s.fun.end() || it->second!=as<FunctionTerm>(term).args.size())//ako ne pripada || druga arnost
		return false;	
		
		for(const auto& subterm: as<FunctionTerm>(term).args)
			if(!checkSignature(subterm,s))
				return false;
		return true;
	}
};

bool checkSignature(FormulaPtr f, Signature& s){


	if(is<True>(f) || is<False>(f))
	return true;
	if(is<Atom>(f))
	{
		auto it = s.rel.find(as<Atom>(f).name);
		if(it==s.rel.end() || it->second!=as<Atom>(f).args.size())//ako ne pripada || druga arnost
		return false;	
		
		for(const auto& subterm: as<Atom>(f).args)
			if(!checkSignature(subterm,s))
				return false;
		return true;
	}
	if(is<Not>(f))
	{
		return checkSignature(as<Not>(f).subformula,s);
	}
	if(is<Binary>(f))
	{	
		auto l=as<Binary>(f).left;
		auto r=as<Binary>(f).right;	
	
		return checkSignature(l,s) && checkSignature(r,s);
	}
	if(is<Quantifier>(f))
	{
	
	return checkSignature(as<Quantifier>(f).subformula,s);
	}
};

unsigned evaluate(TermPtr term,LStructure& l,Valuation& val)
{
	if(is<VariableTerm>(term))
	return val.at(as<VariableTerm>(term).symbol);
	if(is<FunctionTerm>(term))
	{
	std::vector<unsigned> args;
	for(auto& arg: as<FunctionTerm>(term).args)
		args.push_back(evaluate(arg,l,val));
	return l.functions.at(as<FunctionTerm>(term).symbol)(args);
	
};	
}

bool evaluate(FormulaPtr f,LStructure& l,Valuation& val){
	if(is<True>(f))
	return true;
	if(is<False>(f))
	return false;
	if(is<Atom>(f))
	{
	std::vector<unsigned> args;
	for(auto& arg: as<Atom>(f).args)
		args.push_back(evaluate(arg,l,val));
	return l.relations.at(as<Atom>(f).name)(args);
	}
	if(is<Not>(f))
	return !evaluate(as<Not>(f).subformula,l,val);
	if(is<Binary>(f))
	{
	auto lr=as<Binary>(f).left;
	auto rr= as<Binary>(f).right;

	if(as<Binary>(f).type==Binary::And)
	return evaluate(lr,l,val) && evaluate(rr,l,val);
	if(as<Binary>(f).type==Binary::Or)
	return evaluate(lr,l,val) || evaluate(rr,l,val);
	if(as<Binary>(f).type==Binary::Imp)
	return !evaluate(lr,l,val) || evaluate(rr,l,val);
	if(as<Binary>(f).type==Binary::Eq)
	return (!evaluate(lr,l,val) || evaluate(rr,l,val)) && (evaluate(lr,l,val) || !evaluate(rr,l,val));
	}
	if(is<Quantifier>(f))
	{
		std::string var=as<Quantifier>(f).var;
		FormulaPtr subformula=as<Quantifier>(f).subformula;
		if(as<Quantifier>(f).type==Quantifier::All)
		{
			Valuation valQ(val);
			for(auto value: l.domain)
			{
			valQ[var]=value;
			if(!evaluate(subformula,l,valQ))
			return false;
			}
			
			return true;
		}
		if(as<Quantifier>(f).type==Quantifier::Exist)
		{
			Valuation valQ(val);
			for(auto value: l.domain)
			{
				valQ[var]=value;
				if(evaluate(subformula,l,valQ))
				return true;
			}
			
			return false;
		}
	}
};

void print(TermPtr term)
{
	bool zagrade=true;
	if(is<VariableTerm>(term))
	std::cout<<as<VariableTerm>(term).symbol;
	if(is<FunctionTerm>(term))
	{
	if(as<FunctionTerm>(term).args.size()==0)
	zagrade=false;
	std::cout<<as<FunctionTerm>(term).symbol;
	if(zagrade)
	std::cout<<"(";
	int i=0;
	for(auto subterm: as<FunctionTerm>(term).args)
	{
	if(i!=0)
	std::cout<<",";
	print(subterm);
	i++;
	}
	if(zagrade)
	std::cout<<")";
	}
};

std::string termToString(TermPtr term)
{	std::string s="";
	bool zagrade=true;
	if(is<VariableTerm>(term))
	s=s+as<VariableTerm>(term).symbol;
	if(is<FunctionTerm>(term))
	{
	if(as<FunctionTerm>(term).args.size()==0)
	zagrade=false;
	s=s+as<FunctionTerm>(term).symbol;
	if(zagrade)
	s=s+"(";
	int i=0;
	for(auto subterm: as<FunctionTerm>(term).args)
	{
	if(i!=0)
	s=s+",";
	s+=termToString(subterm);
	i++;
	}
	if(zagrade)
	s=s+")";
	}
	return s;
};

void getVariables(TermPtr term,std::set<std::string>& vars)
{
 if(is<VariableTerm>(term))
	vars.insert(as<VariableTerm>(term).symbol);
if(is<FunctionTerm>(term))
	for(auto& sub: as<FunctionTerm>(term).args)
	getVariables(sub,vars);
};

void getVariables(FormulaPtr f,std::set<std::string>& vars, bool free)
{
	if(is<True>(f))
	return ;
	if(is<False>(f))
	return ;
	if(is<Atom>(f))
	{
		for(const auto& sub: as<Atom>(f).args)
			getVariables(sub,vars);
	}
	if(is<Not>(f))
	return getVariables(as<Not>(f).subformula,vars,free);
	if(is<Binary>(f))
	{
	getVariables(as<Binary>(f).left,vars,free);
	getVariables(as<Binary>(f).right,vars,free);
	}
	if(is<Quantifier>(f))
	{
		getVariables(as<Quantifier>(f).subformula, vars, free);
		if(free)
		vars.erase(as<Quantifier>(f).var);
		
	}
};

bool containsVariable(TermPtr term,std::string v)
{
std::set<std::string> skup;
getVariables(term,skup);
return skup.find(v)!=skup.end();
};

bool containsVariable(FormulaPtr f,std::string& v,bool free)
{
std::set<std::string> skup;
getVariables(f,skup,free);
return skup.find(v)!=skup.end();
};

TermPtr substitute(TermPtr term, const std::string &v, TermPtr t)
{
	if(is<VariableTerm>(term))
	{	
	return v==as<VariableTerm>(term).symbol? t : term; 
	}
	if(is<FunctionTerm>(term))
	{	
	std::vector<TermPtr> args;
	for(auto& subterm: as<FunctionTerm>(term).args)
	{
		args.push_back(substitute(subterm,v,t));
	//	print(substitute(subterm,v,t));
	} 
	return ptr(FunctionTerm{as<FunctionTerm>(term).symbol,args});
	}
}

FormulaPtr substitute(FormulaPtr f, const std::string &v, TermPtr t) 
{
	if(is<True>(f))
	return f ;
	if(is<False>(f))
	return f;
	if(is<Atom>(f))
	{
		std::vector<TermPtr> args;
	for(auto subterm: as<Atom>(f).args)
	{
		args.push_back(substitute(subterm,v,t));
	} 
	return ptr(Atom{as<Atom>(f).name,args});
	}
	if(is<Not>(f))
	return ptr(Not{substitute(as<Not>(f).subformula,v,t)});
	if(is<Binary>(f))
	{
	return ptr(Binary{as<Binary>(f).type,substitute(as<Binary>(f).left,v,t),substitute(as<Binary>(f).right,v,t),});
	}
	if(is<Quantifier>(f))
	{
		if(as<Quantifier>(f).var==v)
			return f;
	
		if(containsVariable(t,as<Quantifier>(f).var))
		{
			unsigned uniqueCounter = 0;
            std::string uniqueVar;
			do {
                uniqueVar = "u" + std::to_string(++uniqueCounter);
            } while(containsVariable(f, uniqueVar, false) || containsVariable(t, uniqueVar));
			
			TermPtr varTerm=ptr(VariableTerm{uniqueVar});
			FormulaPtr subformula = substitute(as<Quantifier>(f).subformula, as<Quantifier>(f).var, varTerm);
			return ptr(Quantifier{as<Quantifier>(f).type,uniqueVar,substitute(subformula,v,t)});
		}
			return ptr(Quantifier{as<Quantifier>(f).type,as<Quantifier>(f).var,substitute(as<Quantifier>(f).subformula,v,t)});
	}

}

void print(FormulaPtr f)
{
	if(is<True>(f))
	std::cout<<"T";
	if(is<False>(f))
	std::cout<<"!T";
	if(is<Atom>(f))
	{
	bool zagrade=true;
	if(as<Atom>(f).args.size()==0)
	zagrade=false;
	std::cout<<as<Atom>(f).name;
	if(zagrade)
	std::cout<<"(";
	int i=0;
	for(auto subterm: as<Atom>(f).args)
	{
	if(i!=0)
	std::cout<<",";
	print(subterm);
	i++;
	}
	if(zagrade)
	std::cout<<")";
	}
	if(is<Not>(f)){
	std::cout<<"~";
	print(as<Not>(f).subformula);}
	if(is<Binary>(f))
	{
		std::cout << "( ";
	print(as<Binary>(f).left);
	if(as<Binary>(f).type==Binary::And)
	std::cout<<"&";
	if(as<Binary>(f).type==Binary::Or)
	std::cout<<"|";
	if(as<Binary>(f).type==Binary::Imp)
	std::cout<<"-->";
	if(as<Binary>(f).type==Binary::Eq)
	std::cout<<"<->";
	print(as<Binary>(f).right);
	std::cout << ") ";
	}
	if(is<Quantifier>(f))
	{
	if(as<Quantifier>(f).type==Quantifier::All)
	{
		std::cout<<"(A"<<as<Quantifier>(f).var<<")";
		print(as<Quantifier>(f).subformula);
	}
	if(as<Quantifier>(f).type==Quantifier::Exist)
	{
		std::cout<<"(E"<<as<Quantifier>(f).var<<")";
		print(as<Quantifier>(f).subformula);
	}
	}
};

FormulaPtr simplify(FormulaPtr f)
{
	if(is<True>(f) || is<False>(f) || is<Atom>(f))
	return f;
	if(is<Not>(f))
	{
	FormulaPtr simplified=simplify(as<Not>(f).subformula);
	if(is<True>(simplified))
	return ptr(False{});
	if(is<False>(simplified))
	return ptr(True{});
	return ptr(Not{simplified});
	}
	if(is<Binary>(f))
	{
	FormulaPtr l=as<Binary>(f).left;
	FormulaPtr r=as<Binary>(f).right;

		if(as<Binary>(f).type==Binary::And)
		{
			if(is<False>(simplify(l)) || is<False>(simplify(r))) 
			return ptr(False{});
			if(is<True>(simplify(l)))
			return simplify(r);
			if(is<True>(simplify(r)))
			return simplify(l);
			return ptr(Binary{Binary::And,simplify(l),simplify(r)});
		}
		if(as<Binary>(f).type==Binary::Or)
		{
			if(is<True>(simplify(l)) || is<True>(simplify(r))) 
			return ptr(True{});
			if(is<False>(simplify(l)))
			return simplify(r);
			if(is<False>(simplify(r)))
			return simplify(l);
			return ptr(Binary{Binary::Or,simplify(l),simplify(r)});
		}
		if(as<Binary>(f).type==Binary::Imp)
		{
			if(is<False>(simplify(l)))
			return ptr(True{});
			if(is<True>(simplify(r)))
			return ptr(True{});
			if(is<True>(simplify(l)))
			return simplify(r);
			if(is<False>(simplify(r)))
			return simplify(ptr(Not{simplify(l)}));
			return ptr(Binary{Binary::Imp,simplify(l),simplify(r)});
		}
		if(as<Binary>(f).type==Binary::Eq)
		{
			if(is<True>(simplify(l)))
			return simplify(r);
			if(is<True>(simplify(r)))
			return simplify(l);
			if(is<False>(simplify(l)))
			return simplify(ptr(Not{simplify(r)}));
			if(is<False>(simplify(r)))
			return simplify(ptr(Not{simplify(l)}));
			return ptr(Binary{Binary::Eq,simplify(l),simplify(r)});
		}	
	}
	if(is<Quantifier>(f))
	{
	FormulaPtr subformula = simplify(as<Quantifier>(f).subformula);
	std::string s=as<Quantifier>(f).var;
    if(!containsVariable(subformula, s, true))
        return subformula;
    return ptr(Quantifier{as<Quantifier>(f).type,s,subformula});
	}

};

FormulaPtr nnfNot(FormulaPtr f)
{	
	if(is<True>(f))
		return ptr(Not{});
	if(is<False>(f))
		return ptr(True{});
	if(is<Atom>(f))
        return ptr(Not{f});
	if(is<Not>(f))
        return nnf(as<Not>(f).subformula);
	if(is<Binary>(f))
	{
    auto b = as<Binary>(f);
    if(b.type == Binary::And)
        return ptr(Binary{Binary::Or, nnfNot(b.left), nnfNot(b.right)});
    if(b.type == Binary::Or)
        return ptr(Binary{Binary::And, nnfNot(b.left), nnfNot(b.right)});
    if(b.type == Binary::Imp)
        return ptr(Binary{Binary::And, nnf(b.left), nnfNot(b.right)});
    if(b.type == Binary::Eq)
        return ptr(Binary{
            Binary::Or,
            ptr(Binary{Binary::And, nnf(b.left), nnfNot(b.right)}),
            ptr(Binary{Binary::And, nnfNot(b.left), nnf(b.right)})
        });
	}

	if(is<Quantifier>(f))
	if(as<Quantifier>(f).type==Quantifier::All)
			return ptr(Quantifier{Quantifier::Exist,as<Quantifier>(f).var,nnfNot(as<Quantifier>(f).subformula)});
	else
			return ptr(Quantifier{Quantifier::All,as<Quantifier>(f).var,nnfNot(as<Quantifier>(f).subformula)});
	
}


FormulaPtr nnf(FormulaPtr f)
{
	if(is<True>(f) || is<False>(f) || is<Atom>(f))
	return f;
	if(is<Not>(f))
	return nnfNot(as<Not>(f).subformula);
	if(is<Binary>(f))
	{
		if(as<Binary>(f).type==Binary::And)
		{
		return ptr(Binary{Binary::And,nnf(as<Binary>(f).left),nnf(as<Binary>(f).right)});
		}
		if(as<Binary>(f).type==Binary::Or)
		{
		return ptr(Binary{Binary::Or,nnf(as<Binary>(f).left),nnf(as<Binary>(f).right)});
		}
		if(as<Binary>(f).type==Binary::Imp)
		{
		return ptr(Binary{Binary::Or,nnfNot(as<Binary>(f).left),nnf(as<Binary>(f).right)});
		}
		if(as<Binary>(f).type==Binary::Eq)
		{
		return ptr(Binary{Binary::And,ptr(Binary{Binary::Or,nnfNot(as<Binary>(f).left),nnf(as<Binary>(f).right)}),ptr(Binary{Binary::Or,nnf(as<Binary>(f).left),nnfNot(as<Binary>(f).right)})});
		}
	}
	if(is<Quantifier>(f))
		return ptr(Quantifier{as<Quantifier>(f).type,as<Quantifier>(f).var,nnf(as<Quantifier>(f).subformula)});
};

FormulaPtr pullQuantifiers(FormulaPtr f)
{
	if(!is<Binary>(f))
	return f;
	
	Binary::Type type=as<Binary>(f).type;
	FormulaPtr l=as<Binary>(f).left;
	FormulaPtr r=as<Binary>(f).right;
	
	if(type==Binary::And && is<Quantifier>(l) && as<Quantifier>(l).type==Quantifier::All && is<Quantifier>(r) && as<Quantifier>(r).type==Quantifier::All)
	{

		if(as<Quantifier>(l).var==as<Quantifier>(r).var)
			return ptr(Quantifier{Quantifier::All,as<Quantifier>(l).var,pullQuantifiers(ptr(Binary{Binary::And,as<Quantifier>(l).subformula,as<Quantifier>(r).subformula}))});
		std::string uniqueVar=uniqueVariable(l,r);		
		TermPtr varTerm=ptr(VariableTerm{uniqueVar});
		return ptr(Quantifier{Quantifier::All,uniqueVar,pullQuantifiers(ptr(Binary{Binary::And,substitute(as<Quantifier>(l).subformula,as<Quantifier>(l).var,varTerm),substitute(as<Quantifier>(r).subformula,as<Quantifier>(r).var,varTerm)}))});
	}
	if(type==Binary::Or && is<Quantifier>(l) && as<Quantifier>(l).type==Quantifier::Exist && is<Quantifier>(r) && as<Quantifier>(r).type==Quantifier::Exist)
	{
		if(as<Quantifier>(l).var==as<Quantifier>(r).var)
			return ptr(Quantifier{Quantifier::Exist,as<Quantifier>(l).var,pullQuantifiers(ptr(Binary{Binary::Or,as<Quantifier>(l).subformula,as<Quantifier>(r).subformula}))});
		std::string uniqueVar=uniqueVariable(l,r);	
		TermPtr varTerm=ptr(VariableTerm{uniqueVar});
		return ptr(Quantifier{Quantifier::Exist,uniqueVar,pullQuantifiers(ptr(Binary{Binary::Or,substitute(as<Quantifier>(l).subformula,as<Quantifier>(l).var,varTerm),substitute(as<Quantifier>(r).subformula,as<Quantifier>(r).var,varTerm)}))});
	}
	if(is<Quantifier>(l))
	{
		
		std::string var=as<Quantifier>(l).var;
		if(!containsVariable(r,var,false))
			return ptr(Quantifier{as<Quantifier>(l).type,as<Quantifier>(l).var,pullQuantifiers(ptr(Binary{type,as<Quantifier>(l).subformula,r}))});
		else{
			std::string uniqueVar=uniqueVariable(as<Quantifier>(l).subformula,r);
			TermPtr varTerm=ptr(VariableTerm{uniqueVar});
			//print(substitute(as<Quantifier>(l).subformula,as<Quantifier>(l).var,varTerm));
			return ptr(Quantifier{as<Quantifier>(l).type,uniqueVar,pullQuantifiers(ptr(Binary{type,substitute(as<Quantifier>(l).subformula,as<Quantifier>(l).var,varTerm),r}))});
		}
	}
	if(is<Quantifier>(r))
	{
		std::string var=as<Quantifier>(r).var;
		if(!containsVariable(l,var,false))
			return ptr(Quantifier{as<Quantifier>(r).type,as<Quantifier>(r).var,pullQuantifiers(ptr(Binary{type,l,as<Quantifier>(r).subformula}))});
		else{
			std::string uniqueVar=uniqueVariable(l,as<Quantifier>(r).subformula);
			TermPtr varTerm=ptr(VariableTerm{uniqueVar});
			return ptr(Quantifier{as<Quantifier>(r).type,uniqueVar,pullQuantifiers(ptr(Binary{type,l,substitute(as<Quantifier>(r).subformula,as<Quantifier>(r).var,varTerm)}))});
		}
	}
	return f;
};

FormulaPtr prenex(FormulaPtr f)
{
	if(is<Binary>(f))
	{
		return pullQuantifiers(ptr(Binary{as<Binary>(f).type,prenex(as<Binary>(f).left),prenex(as<Binary>(f).right)}));
	}
	else if(is<Quantifier>(f))
	{
		return ptr(Quantifier{as<Quantifier>(f).type,as<Quantifier>(f).var,prenex(as<Quantifier>(f).subformula)});
	}
	else
	{
	return f;
	}
};

FormulaPtr skolem(FormulaPtr f, Signature& s, std::set<std::string>& vars) {
    if(!is<Quantifier>(f))
        return f;

    if(as<Quantifier>(f).type==Quantifier::All) {
        vars.insert(as<Quantifier>(f).var);
        return ptr(Quantifier{Quantifier::All,as<Quantifier>(f).var, skolem(as<Quantifier>(f).subformula, s, vars)});
    }

    std::string uniqueFun = s.getUniqueSymbol(vars.size());
    std::vector<TermPtr> args;
    for(const auto& var : vars)
        args.push_back(ptr(VariableTerm{var}));
    TermPtr uniqueTerm = ptr(FunctionTerm{uniqueFun, args});
    return skolem(substitute(as<Quantifier>(f).subformula, as<Quantifier>(f).var, uniqueTerm), s, vars);
}

FormulaPtr skolem(FormulaPtr formula, Signature& s) {
    std::set<std::string> vars;
    return skolem(formula, s, vars);
}

std::string atomToString(FormulaPtr f)
{
	if(!is<Atom>(f))
	return "";
	std::string s="";
	
	s=s+ as<Atom>(f).name;
	bool zagrade=false;
	if(as<Atom>(f).args.size()==0)
	return s;
	if(as<Atom>(f).args.size()!=0)
	{
		s=s+"(";
		zagrade=true;
	}
	int i=0;
	for(TermPtr x: as<Atom>(f).args)
	{
	if(i!=0)
	s=s+",";
	s=s+termToString(x);
	i++;
	}
	if(zagrade)
	s=s+")";
	return s;
}

void print(const NormalForm& f) {
    for(const auto& clause : f) {
        std::cout << "[ ";
        for (const auto &literal: clause)
            std::cout << (literal.pos ? "" : "~") << literal.name << " ";
        std::cout << "]";
    }
    std::cout << std::endl;
}

NormalForm cross(const NormalForm& l, const NormalForm& r) {
    NormalForm result;
    for(const auto& lc : l)
        for(const auto& rc : r)
            result.push_back(concat(lc, rc));
    return result;

}

NormalForm dnf(const FormulaPtr& f) {
    if(is<True>(f))
        return {};
    if(is<False>(f))
        return {{}};
    if(is<Atom>(f))
        {return {{Literal{true, atomToString(f)}}};};
    if(is<Not>(f))
		{return {{Literal{false, atomToString(as<Not>(f).subformula)}}};};
    auto b = as<Binary>(f);
    if(b.type == Binary::Or)
        return concat(dnf(b.left), dnf(b.right));
    if(b.type == Binary::And)
        return cross(dnf(b.left), dnf(b.right));
    return NormalForm{};
}


FormulaPtr removeUniversal(FormulaPtr f)
{
	if(!is<Quantifier>(f))
		return f;
	else
	return removeUniversal(as<Quantifier>(f).subformula);
}




