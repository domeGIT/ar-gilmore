#include "interpretation.h"
#include "first_order_logic.h"
#include "herbrand.h"
#include "gilmore.h"


int main()
{

 TermPtr x=ptr(VariableTerm{"x"});

 TermPtr y=ptr(VariableTerm{"y"});


 TermPtr z = ptr(VariableTerm{ "z" });
//----------------------------------------brijac------------------------------------------------------------------------------------------------
FormulaPtr brije1 = ptr(Quantifier{Quantifier::Exist,"y",ptr(Quantifier{Quantifier::All,"x",ptr(Atom{"brije",{y,x}})})});
FormulaPtr brije2 = ptr(Atom{"brije",{x,x}});
FormulaPtr trije2 = ptr(Atom{"brije",{y,x}});
FormulaPtr trije4 =ptr(Quantifier{Quantifier::Exist,"y",ptr(Quantifier{Quantifier::All,"x",ptr(Binary{Binary::Eq,trije2,ptr(Not{brije2})})})});

FormulaPtr brije=ptr(Binary{Binary::Eq,brije1,ptr(Not{brije2})});
Signature s_brije;
s_brije.rel["brije"]=2;
gilmore(s_brije,ptr(Not{trije4}));

//---------------------------------------------------------------------------------------------------------------------------------------------
FormulaPtr zadatak1=ptr(Atom{"p",{{x}}});
FormulaPtr zadatak2=ptr(Quantifier{Quantifier::All,"y",ptr(Atom{"p",{{y}}})});
FormulaPtr zadatak3=ptr(Binary{Binary::Imp,zadatak1,zadatak2});
FormulaPtr zadatak4=ptr(Quantifier{Quantifier::Exist,"x",zadatak3});
Signature s_zadatak;
s_zadatak.rel["p"]=1;
gilmore(s_zadatak,zadatak4);

//---------------------------------------------------------------------------------------------------------------------------------------------
FormulaPtr f1 = ptr(Binary{ Binary::Or, ptr(Atom{ "F",{x,y}}), ptr(Atom{ "F",{y,x}}) });
FormulaPtr f1_1 = ptr(Quantifier{ Quantifier::Exist, "y", f1 });
FormulaPtr f1_2 = ptr(Quantifier{ Quantifier::All, "x", f1_1 });

FormulaPtr f2 = ptr(Binary{ Binary::Imp, ptr(Atom{ "F",{x,y}}), ptr(Atom{ "F",{y,y}}) });
FormulaPtr f2_1 = ptr(Quantifier{ Quantifier::All, "y", f2 });
FormulaPtr f2_2 = ptr(Quantifier{ Quantifier::All, "x", f2_1 });
FormulaPtr f3 = ptr(Quantifier{ Quantifier::Exist, "z", ptr(Atom{ "F",{z,z}}) });

FormulaPtr f4 = ptr(Binary{ Binary::And,f1_2,f2_2 });
FormulaPtr f5 = ptr(Binary{ Binary::Imp,f4,f3 });


Signature a;
a.rel["F"] = 2;

gilmore(a, f5);

	return 0;
}