#ifndef BANK_COMPONENT_H__
#define BANK_COMPONENT_H__

struct BankComponent
{
	BankComponent() { }
	BankComponent(const int& income, const int& gold) : income(income), gold(gold) { }
	int income;
	int gold;
};


#endif