#include "CashAccount.h"

DepositSchedule::DepositSchedule(double d, int schedule, wxDateTime start) : m_deposit(d), m_days(0,0,0,schedule), m_startDate(start), m_endDate(wxInvalidDateTime)
{
	if (!m_startDate.IsValid())
	{
		wxFAIL_MSG("startdate param in DepositSchedule::DepositSchedule is an invalid date!");
		this->OK = false;
	}
	if (schedule < 0)
	{
		wxFAIL_MSG("schedule param in DespositSchedule::DespositSchedule is less than zero!");
		this->OK = false;
	}
}

DepositSchedule::DepositSchedule(double d, int schedule, wxDateTime start, wxDateTime end) : m_deposit(d), m_days(0,0,0,schedule), m_startDate(start), m_endDate(end)
{
	if (!this->m_startDate.IsValid())
	{
		wxFAIL_MSG("startdate param in DepositSchedule::DepositSchedule is an invalid date!");
		this->OK = false;
	}
	if (!this->m_endDate.IsValid())
	{
		wxFAIL_MSG("enddate param in DepositSchedule::DepositSchedule is an invalid date!");
		this->OK = false;
	}
	if (schedule < 0)
	{
		wxFAIL_MSG("schedule param in DespositSchedule::DespositSchedule is less than zero!");
		this->OK = false;
	}
}

double DepositSchedule::RunDepositSchedule()
{
	if (!this->IsOK())
		return 0.0;
	
	wxString date = "";
	wxDateTime current = wxDateTime::Today();

	// check to see if start date is today or in the past. If not then no reason to run the schedule so return 0.0;
	if (this->m_startDate > current)
		return 0.0;

	double deposit = this->m_deposit * this->m_numberOfDeposits;
	wxDateTime temp = this->m_nextDepositDate.IsValid() ? this->m_nextDepositDate : this->m_startDate;

	for (temp; temp <= current; temp = temp + this->m_days)
	{
		deposit += this->m_deposit;
		this->m_numberOfDeposits++;
	}

	this->m_nextDepositDate = temp;
	return deposit;
}

bool DepositSchedule::IsOK()
{
	return this->OK;
}



// public CashAccount Methods...

CashAccount::CashAccount(double cash) 
{
	wxDateTime date(1, wxDateTime::Month::Jan, 2000);
	this->Deposit(date, cash);
}

bool CashAccount::Purchase(long l, double d, wxDateTime t)
{
	// check that date is valid and not in the future....
	if (!this->m_CheckDate(t, "Withdrawl date paramater was invalid or in the future in CashAccount::Purchase!"))
		return false;

	// check to make sure purchase id is not the same as a previous one...
	if (this->m_CheckDuplicateID(l, "Id passed as paramater to CashAccount::Purchase is the same as a previous purchase!"))
		return false;

	// check that withdrawl amount is not more than cash available to withdrawl...
	if (!this->m_CheckCashAmount(d, "Purchase amount exceeds available cash in CashAccount::Purchase!"))
		return false;

	//purchase will go through...
	this->m_Purchase(t, d, l);
	return true;
}

bool CashAccount::RequestPurchase(long l, double d)
{
	// check to make sure purchase id is not the same as a previous one...
	if (this->m_CheckDuplicateID(l, "Id passed as paramater to CashAccount::Purchase is the same as a previous purchase!"))
		return false;

	// check that withdrawl amount is not more than cash available to withdrawl...
	if (!this->m_CheckCashAmount(d, "Purchase amount exceeds available cash in CashAccount::Purchase!"))
		return false;

	return true;
}

bool CashAccount::Deposit(wxDateTime t, double d)
{
	// check that date is valid and not in the future....
	if (!this->m_CheckDate(t, "Withdrawl date paramater was invalid or in the future in CashAccount::Deposit!"))
		return false;

	// execute deposit...
	this->m_Deposit(t, d);
	return true;
}

bool CashAccount::Withdrawl(wxDateTime t, double d)
{
	// check that date is valid and not in the future....
	if (!this->m_CheckDate(t, "Withdrawl date paramater was invalid or in the future in CashAccount::Withdrawl!"))
		return false;

	// check that withdrawl amount is not more than cash available to withdrawl...
	if (!this->m_CheckCashAmount(d, "Withdrawl amount exceeds available cash in CashAccount::Withdrawl!"))
		return false;

	// execute withdrawl...
	this->m_Withdrawl(t, d);
	return true;
}

bool CashAccount::NewDepositSchedule(double d, int days, wxDateTime start)
{
	if (!this->m_ValidDate(start, "Date passed to CashAccount::NewDepositSchedule is invalid date!"))
		return false;

	if (d == 0)
	{
		wxString s_d = "";
		s_d << d;
		wxFAIL_MSG("deposit value passed to CashAccount::NewDepositSchedule is invalid number. Deposit is: " + s_d);
		return false;
	}

	if (days == 0 || days < 0)
	{
		wxString s_days = "";
		s_days << days;
		wxFAIL_MSG("days passed to CashAccount::NewDepositSchedule is invalid number. Days is: " + s_days);
		return false;
	}

	//Execute New Deposit Schedule...
	return this->m_NewDepositSchedule(d, days, start);
}

bool CashAccount::NewDepositSchedule(double d, int days, wxDateTime start, wxDateTime end)
{
	// check dates to make sure they are valid...
	if (!this->m_ValidDate(start, "Startdate passed to CashAccount::NewDepositSchedule is invalid date!"))
		return false;

	if (this->m_ValidDate(end, "Enddate passed to CashAccount::NewDepositSchedule is invalid date!"))
		return false;

	// check to make sure deposit is not zero...
	if (d == 0)
	{
		wxString s_d = "";
		s_d << d;
		wxFAIL_MSG("deposit value passed to CashAccount::NewDepositSchedule is invalid number. Deposit is: " + s_d);
		return false;
	}

	// check to make sure days are not zero or negative...
	if (days == 0 || days < 0)
	{
		wxString s_days = "";
		s_days << days;
		wxFAIL_MSG("days passed to CashAccount::NewDepositSchedule is invalid number. Days is: " + s_days);
		return false;
	}

	//Execute New Deposit Schedule...
	return this->m_NewDepositSchedule(d, days, start, end);
}

void CashAccount::UpdateCash()
{
	this->m_ZeroCash();
	this->m_cash = this->m_UpdateCash();
}

double CashAccount::GetFreeCash()
{
	return this->m_cash;
}

const double* CashAccount::GetFreeCashPtr()
{
	return &this->m_cash;
}

// private CashAccount Methods...
bool CashAccount::m_CheckDate(wxDateTime& T, wxString s)
{
	if (T.IsValid())
	{
		if (T > wxDateTime::Today())
		{
			wxFAIL_MSG(s);
			return false;
		}
	}
	else
	{
		wxFAIL_MSG(s);
		return false;
	}

	return true;
}

bool CashAccount::m_ValidDate(wxDateTime& date, wxString s)
{
	if (!date.IsValid())
	{
		wxFAIL_MSG(s);
		return false;
	}

	return true;
}

bool CashAccount::m_CheckCashAmount(double& d, wxString s)
{
	if (this->m_cash < d)
	{
		wxFAIL_MSG(s);
		return false;
	}

	return true;
}

bool CashAccount::m_CheckDuplicateID(long& l, wxString s)
{
	for (size_t i = 0; i < this->purchase.size(); ++i)
	{
		if (this->purchase[i].id == l)
		{
			wxFAIL_MSG(s);
			return true;
		}
	}

	return false;
}

bool CashAccount::m_NewDepositSchedule(double& d, int& days, wxDateTime& start)
{
	this->schedule.push_back(DepositSchedule(d, days, start));
	return true;
}

double CashAccount::m_UpdateCash()
{
	double temp = 0.0;
	temp += this->m_RunDepositSchedule();
	temp += this->m_AddDeposits();
	temp += this->m_AddWithdrawls();
	temp += this->m_AddPurchases();

	return temp;
}

bool CashAccount::m_NewDepositSchedule(double& d, int& days, wxDateTime& start, wxDateTime& end)
{
	this->schedule.push_back(DepositSchedule(d, days, start, end));
	return true;
}

double CashAccount::m_RunDepositSchedule()
{
	double temp = 0.0;
	for (size_t i = 0; i < this->schedule.size(); ++i)
		temp += schedule[i].RunDepositSchedule();

	return temp;
}

double CashAccount::m_AddDeposits()
{
	double temp = 0.0;
	for (size_t i = 0; i < this->deposit.size(); ++i)
		temp += this->deposit[i].value;

	return temp;
}

double CashAccount::m_AddWithdrawls()
{
	double temp = 0.0;
	for (size_t i = 0; i < this->withdrawl.size(); ++i)
		temp += this->withdrawl[i].value;

	return temp;
}

double CashAccount::m_AddPurchases()
{
	double temp = 0.0;
	for (size_t i = 0; i < this->purchase.size(); ++i)
		temp += purchase[i].value;

	return temp;
}

void CashAccount::m_Purchase(wxDateTime& t, double& d, long& l)
{
	this->purchase.push_back(pair(l, d, t));
	this->m_RemoveCash(d);
}

void CashAccount::m_Withdrawl(wxDateTime& t, double& d)
{
	this->withdrawl.push_back(withdrawl_pair(t, d));
	this->m_RemoveCash(d);
}

void CashAccount::m_Deposit(wxDateTime& t, double& d)
{
	this->deposit.push_back(deposit_pair(t, d));
	this->m_AddCash(d);
}

void CashAccount::m_RemoveCash(double& d)
{
	this->m_cash -= d;
}

void CashAccount::m_AddCash(double& d)
{
	this->m_cash += d;
}

void CashAccount::m_ZeroCash()
{
	this->m_cash = 0.0;
}
