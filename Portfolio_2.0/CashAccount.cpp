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

void DepositSchedule::FillClientVector(wxVector<deposit_pair>& vec)
{
	wxDateTime current(wxDateTime::Today());
	wxDateTime temp = this->m_startDate;

	for (temp; temp <= current; temp = temp + this->m_days)
	{
		size_t i = 0;
		while (vec.size() > i && temp <= vec[i].T)
			++i;

		if (!i)
			vec.push_back(deposit_pair(temp, this->m_deposit));
		else
			vec.insert(&vec[i], deposit_pair(temp, this->m_deposit));
	}
}

bool DepositSchedule::IsOK()
{
	return this->OK;
}

void DepositSchedule::Save(DataStream& ds)
{
	int invalid = -1;
	int valid = 1;
	ds.WritewxDateTime(this->m_startDate);

	if (this->m_endDate.IsValid())
	{
		ds.WriteData(valid);
		ds.WritewxDateTime(this->m_endDate);
	}
	else
		ds.WriteData(invalid);

	if (this->m_nextDepositDate.IsValid())
	{
		ds.WriteData(valid);
		ds.WritewxDateTime(this->m_nextDepositDate);
	}
	else
		ds.WriteData(invalid);

	int days = this->m_days.GetDays();
	ds.WriteData(this->m_deposit);
	ds.WriteData(this->m_numberOfDeposits);
	ds.WriteData(days);
	ds.WriteBool(this->OK);
}

void DepositSchedule::Retrieve(DataStream& ds)
{
	int valid = 1;
	int invalid = -1;
	int test;
	ds.ReadwxDateTime(this->m_startDate);

	ds.ReadData(test);
	if (test == valid)
		ds.ReadwxDateTime(this->m_endDate);

	ds.ReadData(test);
	if (test == valid)
		ds.ReadwxDateTime(this->m_nextDepositDate);

	int days;
	ds.ReadData(this->m_deposit);
	ds.ReadData(this->m_numberOfDeposits);
	ds.ReadData(days);
	ds.ReadBool(this->OK);

	this->m_days = wxDateSpan(0, 0, 0, days);
}



// public CashAccount Methods...

CashAccount::CashAccount() 
{
	
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

bool CashAccount::CompleteSale(Pair p, wxDateTime date, double proceeds, long lot)
{
	pair* P = this->m_FindPurchase(lot);
	if (!P)
	{
		wxFAIL_MSG("Could not find lot in CashAccount::AddSellInfo!");
		return false;
	}

	return this->m_CompleteSale(p, date, proceeds, lot);
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

bool CashAccount::ReplaceDividends(Pair p)
{
	if (!p.amount)
		return this->m_EraseDividends(p);

	return this->m_ReplaceDividends(p);
}

bool CashAccount::AddDividends(Pair p)
{
	return this->m_AddDividends(p);
}

void CashAccount::UpdateCash()
{
	this->m_ZeroCash();
	this->m_cash = this->m_UpdateCash();
}

const double CashAccount::GetFreeCash() const
{
	return this->m_cash;
}

const double* CashAccount::GetFreeCashPtr() const
{
	return &this->m_cash;
}

wxVector<deposit_pair> CashAccount::GetDepositVector()
{
	wxVector<deposit_pair> vec;
	for (auto& v : this->deposit)
		vec.push_back(v);

	for (auto& v : this->schedule)
		v.FillClientVector(vec);

	return vec;
}

void CashAccount::Save()
{
	DataStream ds("SavedFiles/CashAccount.bin", wxPosixPermissions::wxS_DEFAULT, wxFile::OpenMode::write);
	if (!ds.IsOK())
		return;

	ds.WriteDepositVector(this->deposit);
	ds.WriteWithdrawlVector(this->withdrawl);
	ds.Write_pairVector(this->purchase);
	ds.Write_pairVector(this->sell);
	ds.WriteCashDivVector(this->divs);

	size_t size = this->schedule.size();
	ds.WriteData(size);
	for (size_t i = 0; i < size; ++i)
		this->schedule[i].Save(ds);
}

void CashAccount::Retrieve()
{
	DataStream ds("SavedFiles/CashAccount.bin", wxPosixPermissions::wxS_DEFAULT, wxFile::OpenMode::read);
	if (!ds.IsOK())
		return;

	ds.ReadDepositVector(this->deposit);
	ds.ReadWithdrawlVector(this->withdrawl);
	ds.Read_pairVector(this->purchase);
	ds.Read_pairVector(this->sell);
	ds.ReadCashDivVector(this->divs);

	size_t size;
	ds.ReadData(size);
	for (size_t i = 0; i < size; ++i)
	{
		this->schedule.push_back(DepositSchedule());
		this->schedule[this->schedule.size() - 1].Retrieve(ds);
	}

	this->UpdateCash();
}

// private CashAccount Methods...
pair* CashAccount::m_FindPurchase(long& lot)
{
	for (auto& v : this->purchase)
	{
		if (v.id == lot)
			return &v;
	}

	return NULL;
}

bool CashAccount::m_CompleteSale(Pair& p, wxDateTime& date, double& proceeds, long& lot)
{
	this->sell.push_back(pair(lot, proceeds, date));
	if (p.amount)
		this->m_ReplaceDividends(p);
	this->UpdateCash();
	return true;
}

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
	temp += this->m_AddSales();
	temp += this->m_AddDividends();

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
		temp += -(this->withdrawl[i].value);

	return temp;
}

double CashAccount::m_AddPurchases()
{
	double temp = 0.0;
	for (size_t i = 0; i < this->purchase.size(); ++i)
		temp += -(purchase[i].value);

	return temp;
}

double CashAccount::m_AddSales()
{
	double temp = 0.0;
	for (auto& v : this->sell)
		temp += v.value;

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

bool CashAccount::m_ReplaceDividends(Pair& p)
{
	Pair* ptr = this->m_FindDiv(p.ticker);
	if (!ptr)
	{
		wxFAIL_MSG("Can't find Pair* in CashAccount::m_ReplaceDividends! Returning false.");
		return false;
	}

	ptr->amount = p.amount;
	this->UpdateCash();
	return true;
}

bool CashAccount::m_AddDividends(Pair& p)
{
	auto index = this->m_FindDiv(p.ticker);
	if(!index)
		this->divs.push_back(p);

	index->amount = p.amount;
	this->UpdateCash();
	return true;
}

double CashAccount::m_AddDividends()
{
	double d = 0.0;
	for (auto& v : this->divs)
		d += v.amount;

	return d;
}

bool CashAccount::m_EraseDividends(Pair& p)
{
	Pair* ptr = this->m_FindDiv(p.ticker);
	if (!ptr)
	{
		wxFAIL_MSG("Ticker: " + p.ticker + " does not exist in divs vector in CashAccount::m_EraseDividends! Returning false");
		return false;
	}

	this->divs.erase(ptr);
	return true;
}

Pair* CashAccount::m_FindDiv(wxString& s)
{
	for (auto& v : this->divs)
	{
		if (v.ticker == s)
			return &v;
	}

	return NULL;
}
