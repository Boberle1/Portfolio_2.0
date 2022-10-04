#pragma once
#include "wx/wx.h"

class DepositSchedule
{
public:
	DepositSchedule(double d, int schedule, wxDateTime start);
	DepositSchedule(double d, int schedule, wxDateTime start, wxDateTime end);
	double RunDepositSchedule();
	bool IsOK();
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
	CashAccount(double cash);
	bool Purchase(long, double, wxDateTime);
	bool RequestPurchase(long, double);
	bool Deposit(wxDateTime, double);
	bool Withdrawl(wxDateTime, double);
	bool NewDepositSchedule(double, int, wxDateTime);
	bool NewDepositSchedule(double, int, wxDateTime, wxDateTime);
	void UpdateCash();
	double GetFreeCash();
	const double* GetFreeCashPtr();
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

	struct pair
	{
		pair(long l, double d, wxDateTime t) : id(l), value(d), date(t) {}
		long id = 0;
		double value = 0.0;
		wxDateTime date;
	};
	struct deposit_pair
	{
		deposit_pair(wxDateTime d, double v) : T(d), value(v) {}
		wxDateTime T;
		double value = 0;
	};
	struct withdrawl_pair
	{
		withdrawl_pair(wxDateTime d, double v) : date(d), value(v) {}
		wxDateTime date;
		double value = 0.0;
	};

	double m_cash = 0.0;
	wxVector<pair> purchase;
	wxVector<deposit_pair> deposit;
	wxVector<withdrawl_pair> withdrawl;
	wxVector<DepositSchedule> schedule;
};

