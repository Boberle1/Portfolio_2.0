#pragma once
//#include "wx/wx.h"
#include "DataStream.h"

class DepositSchedule
{
public:
	DepositSchedule() {}
	DepositSchedule(double d, int schedule, wxDateTime start);
	DepositSchedule(double d, int schedule, wxDateTime start, wxDateTime end);
	double RunDepositSchedule();
	bool IsOK();
	void Save(DataStream&);
	void Retrieve(DataStream&);
private:
	double m_deposit = 0.0;
	bool OK = true;
	wxDateTime m_startDate;
	wxDateTime m_endDate;

	// number of days between deposits...
	wxDateTime m_nextDepositDate;
	wxDateSpan m_days;
	int m_numberOfDeposits = 0;
};

class CashAccount
{
public:
	CashAccount();
	bool Purchase(long, double, wxDateTime);
	bool RequestPurchase(long, double);
	bool Deposit(wxDateTime, double);
	bool Withdrawl(wxDateTime, double);
	bool NewDepositSchedule(double, int, wxDateTime);
	bool NewDepositSchedule(double, int, wxDateTime, wxDateTime);

	// One thing to note is if the Pair::amount is zero then the divs index will be erased with that ticker
	// because is assumes it is not needed being that the dividend amount is zero..
	// So do not send a pair with amount zero if you dont want that index erased from the sector...
	bool ReplaceDividends(Pair);

	bool AddDividends(Pair);
	void UpdateCash();
	const double GetFreeCash() const;
	const double* GetFreeCashPtr() const;
	void Save();
	void Retrieve();
private:
	bool m_CheckDate(wxDateTime&, wxString);
	bool m_ValidDate(wxDateTime&, wxString);
	bool m_CheckCashAmount(double&, wxString);
	bool m_CheckDuplicateID(long&, wxString);
	bool m_NewDepositSchedule(double&, int&, wxDateTime&);
	bool m_NewDepositSchedule(double&, int&, wxDateTime&, wxDateTime&);
	double m_UpdateCash();
	double m_RunDepositSchedule();
	double m_AddDeposits();
	double m_AddWithdrawls();
	double m_AddPurchases();
	void m_Purchase(wxDateTime&, double&, long&);
	void m_Withdrawl(wxDateTime&, double&);
	void m_Deposit(wxDateTime&, double&);
	void m_RemoveCash(double&);
	void m_AddCash(double&);
	void m_ZeroCash();
	bool m_ReplaceDividends(Pair&);
	bool m_AddDividends(Pair&);
	double m_AddDividends();
	bool m_EraseDividends(Pair&);
	Pair* m_FindDiv(wxString&);
	
	double m_cash = 0.0;
	wxVector<pair> purchase;
	wxVector<deposit_pair> deposit;
	wxVector<withdrawl_pair> withdrawl;
	wxVector<DepositSchedule> schedule;
	wxVector<Pair> divs;
};

