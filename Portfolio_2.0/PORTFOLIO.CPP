#include "Portfolio.h"

inline wxDateTime IsValidDate(wxString& date, wxString msg)
{
	wxDateTime temp;
	if (!temp.ParseDate(date))
	{
		wxFAIL_MSG(msg);
		return wxDateTime(wxInvalidDateTime);
	}
	if (wxDateTime::Today() < temp)
	{
		wxFAIL_MSG(msg);
		return wxDateTime(wxInvalidDateTime);
	}

	return temp;
}

unsigned int inline GetEndMonthDay(wxDateTime::Month m, int year)
{
	wxDateTime LastDay(1, m, year);
	int lastworkday;

	switch (m) {
	case 1: {
		if (LastDay.IsLeapYear())
			lastworkday = 29;
		else
			lastworkday = 28;
		break;
	}
	case 3:
	case 5:
	case 8:
	case 10: lastworkday = 30;
		break;
	default: lastworkday = 31;
	}

	return lastworkday;
}

wxDateTime::Month inline GetQuarterStartMonth(wxDateTime::Month m)
{
	switch (m)
	{
	case wxDateTime::Month::Jan:
	case wxDateTime::Month::Feb:
	case wxDateTime::Month::Mar: return wxDateTime::Month::Jan;
	case wxDateTime::Month::Apr:
	case wxDateTime::Month::May:
	case wxDateTime::Month::Jun: return wxDateTime::Month::Apr;
	case wxDateTime::Month::Jul:
	case wxDateTime::Month::Aug:
	case wxDateTime::Month::Sep: return wxDateTime::Month::Jul;
	case wxDateTime::Month::Oct:
	case wxDateTime::Month::Nov:
	case wxDateTime::Month::Dec: return wxDateTime::Month::Oct;
	}

	wxString month = "";
	wxFAIL_MSG("wxDateTime::Month passed to GetQuarterStartMonth did not match any months in the switch statment. Month passed to it is: " + month);
	return wxDateTime::Month::Inv_Month;
}

wxDateTime::Month inline GetQuarterEndMonth(wxDateTime::Month m)
{
	switch (m)
	{
	case wxDateTime::Month::Jan:
	case wxDateTime::Month::Feb:
	case wxDateTime::Month::Mar: return wxDateTime::Month::Mar;
	case wxDateTime::Month::Apr:
	case wxDateTime::Month::May:
	case wxDateTime::Month::Jun: return wxDateTime::Month::Jun;
	case wxDateTime::Month::Jul:
	case wxDateTime::Month::Aug:
	case wxDateTime::Month::Sep: return wxDateTime::Month::Sep;
	case wxDateTime::Month::Oct:
	case wxDateTime::Month::Nov:
	case wxDateTime::Month::Dec: return wxDateTime::Month::Dec;
	}

	wxString month = "";
	wxFAIL_MSG("wxDateTime::Month passed to GetQuarterEndMonth did not match any months in the switch statment. Month passed to it is: " + month);
	return wxDateTime::Month::Inv_Month;
}

template <typename T>
bool IsNull(T* t, wxString msg)
{
	if (!t)
	{
		wxFAIL_MSG(msg);
		return false;
	}

	return true;
}

// stock_node functions...
stock_node::stock_node(long id, wxString ticker, wxDateTime date, double price_per_share, double shares, Action a, wxDateTime reinvest)
	: m_id(id), m_ticker(ticker), m_date(date), m_price_per_share(price_per_share), m_shares(shares), action(a), m_reinvest_start(reinvest)
{

}

void stock_node::SetSibling(stock_node sn)
{
	size_t i = 0;
	while (i < this->sold.size() && this->sold[i].GetPurchaseDate() < sn.GetPurchaseDate())
		++i;
	if (i == this->sold.size() || i == 0)
		this->sold.push_back(sn);
	else
		this->sold.insert(&this->sold[i], sn);
}

void stock_node::ShareDivsWithSibling(stock_node* sn)
{
	if (this == sn)
		return;
	for (size_t i = 0; i < this->divs.size(); ++i)
	{
		if (this->divs[i].ex_Div > sn->GetPurchaseDate())
			sn->HandleDivReInvest(&this->divs[i]);
	}
}

double stock_node::GetCostBasis(wxDateTime* T)
{
	if (!T)
	{
		wxFAIL_MSG("wxDateTime* passed to stock_node::AveragePrice is nullptr!");
		return 0.0;
	}

	double shares = this->m_GetShares(T);
	return shares * m_price_per_share;
}

double stock_node::GetShares(wxDateTime* T)
{
	return this->m_GetShares(T);
}

double stock_node::GetRealizedGain(wxDateTime* T)
{
	double gain = 0.0;
	for (size_t i = 0; i < this->sold.size(); ++i)
	{
		if (sold[i].m_date <= *T)
			gain += sold[i].GetCostBasis(T);
	}

	return gain;
}

bool stock_node::IdMatch(long& id)
{
	if (this->m_id == id)
		return true;

	return false;
}

wxDateTime stock_node::GetPurchaseDate()
{
	return this->m_date;
}

double stock_node::GetPurchasePrice(wxDateTime::Month start, int dayofyear, int year)
{
	if (this->m_date.GetYear() != year)
		return 0.0;

	if (this->m_date.GetMonth() >= start && this->m_date.GetDayOfYear() <= dayofyear)
	{
		return this->m_price_per_share;
	}
	else
		return 0.0;
}

double stock_node::GetPurchasePrice(int year, int dayofyear)
{
	if (this->m_date.GetYear() != year)
		return 0.0;

	if(this->m_date.GetDayOfYear() <= dayofyear)
		return this->m_price_per_share;

	return 0.0;
}

double stock_node::GetPurchasePrice(int monday, int friday, int year)
{
	if (this->m_date.GetYear() != year)
		return 0.0;

	if (this->m_date.GetDayOfYear() >= monday && this->m_date.GetDayOfYear() <= friday)
		return this->m_price_per_share;

	return 0.0;
}

double stock_node::GetPurchasePrice(wxDateTime* date)
{
	if (*date == this->m_date)
		return this->m_price_per_share;

	return 0.0;
}

void stock_node::HandleDivReInvest(Dividend& d)
{
	if (this->GetPurchaseDate() < d.ex_Div && this->GetShares(&d.ex_Div))
	{
		if (this->m_reinvest_start.IsValid())
		{
			if (this->m_reinvest_start <= d.ex_Div)
				d.SetDivReinvestOn();
			else
			{
				int span = this->m_reinvest_start.GetDayOfYear() - (d.ex_Div + wxDateSpan(0, 0, 0, 1)).GetDayOfYear();
				if (!span || span < 31)
					d.SetDivReinvestOn();
			}

		}
		this->InsertDiv(d);
	}
}

void stock_node::HandleDivReInvest(Dividend* d)
{
	if (this->GetPurchaseDate() < d->ex_Div && this->GetShares(&d->ex_Div))
	{
		if (this->m_reinvest_start.IsValid())
		{
			if (this->m_reinvest_start <= d->ex_Div)
				d->SetDivReinvestOn();
			else
			{
				int span = this->m_reinvest_start.GetDayOfYear() - (d->ex_Div + wxDateSpan(0, 0, 0, 1)).GetDayOfYear();
				if (!span || span < 31)
					d->SetDivReinvestOn();
			}

		}
		this->InsertDiv(*d);
	}
}

wxDateTime* stock_node::GetLatestDivDate()
{
	wxDateTime* latest = nullptr;
	for (size_t i = 0; i < this->divs.size(); ++i)
	{
		if ((latest && !latest->IsValid()) || !latest)
			latest = &this->divs[i].ex_Div;
		else
		{
			if (*latest < this->divs[i].ex_Div)
				latest = &this->divs[i].ex_Div;
		}
	}

	return latest;
}

wxDateTime* stock_node::GetDividendDates(wxDateTime& date)
{
	if (!date.IsValid())
		return nullptr;

	for (size_t i = 0; i < this->divs.size(); ++i)
	{
		if (date == this->divs[i].ex_Div)
			return &this->divs[i].ex_Div;
	}

	return nullptr;
}

double stock_node::GetDividends(wxDateTime* date)
{
	double d = 0.0;
	for (size_t i = 0; i < this->divs.size(); ++i)
	{
		if (!divs[i].DivReinvest && *date >= this->divs[i].ex_Div)
			d += this->divs[i].div * this->GetShares(&this->divs[i].ex_Div);
	}

	return d;
}

double stock_node::GetDividendShares(wxDateTime* date)
{
	double d = 0.0;
	for (size_t i = 0; i < this->divs.size(); ++i)
	{
		if (divs[i].DivReinvest && *date <= this->divs[i].ex_Div)
			d += this->divs[i].re_invest_shares;
	}

	return d;
}

void stock_node::InsertDiv(Dividend& d)
{
	size_t i = 0;
	while (i < this->divs.size() && this->divs[i].ex_Div < d.ex_Div)
		++i;
	if (i == this->divs.size() || i == 0)
		this->divs.push_back(d);
	else
	{
		// check for duplicate
		if (this->divs[i].ex_Div != d.ex_Div)
			this->divs.insert(&this->divs[i], d);
//		else
//			wxMessageBox("stock_node::InsertDiv tried to insert a duplicate dividend!");
	}
}

double stock_node::GetPurchasePrice()
{
	return this->m_price_per_share;
}

double stock_node::m_GetShares(wxDateTime* T)
{
	double shares = this->m_shares;
	for (size_t i = 0; i < this->sold.size(); ++i)
	{
		if (sold[i].m_date <= *T)
			shares -= sold[i].m_shares;
	}

	return shares;
}

// CallBacks
void SetHistoricalDataCB(void* v, double o, double h, double l, double c, wxDateTime d)
{
	static_cast<StockNode*>(v)->SetHistoricalData(Day_Prices(o, h, l, c, d));
}

void SetSummaryDataCB(void* v, SummaryData sd)
{
	static_cast<StockNode*>(v)->SetSummaryData(sd);
}

void SetDividendsCB(void* v, Dividend d)
{
	static_cast<StockNode*>(v)->SetDividends(d);
}

// thread callback....
bool ExecutePurchase(StockNode* S, long id, wxDateTime date, double price, double shares)
{
	return S->InitiatePurchase(id, date, price, shares);
}

// StockThread functions...
StockThread::StockThread(wxThreadKind tk, StockNode* S, long id, wxDateTime date, double price, double shares) 
	: wxThread(tk), m_parent(S), m_id(id),  m_date(date), m_price(price), m_shares(shares)
{
	
}

wxThread::ExitCode StockThread::Entry()
{
	if (TestDestroy())
		return (wxThread::ExitCode)0;

	result = ExecutePurchase(this->m_parent, this->m_id, this->m_date, this->m_price, this->m_shares);

	wxThreadEvent evt(ThreadComplete, 0);
	wxPostEvent(this->m_parent, evt);

	return (wxThread::ExitCode)0;
}

StockThread::~StockThread()
{

}

bool StockThread::GetResult()
{
	return this->result;
}

// StockNode functions...
StockNode::StockNode(wxString t, wxDateTime* p, Portfolio* P, Sector* S, bool reinvest, wxString T) 
	: m_ticker(t), m_parentclock(p), m_grand_parent(P), m_parent(S), DivReinvest(reinvest)
{
	wxString s_date = this->m_parentclock->Format(STANDARD_DATE);
	this->Bind(ThreadComplete, &StockNode::OnThreadComplete, this);
	if (this->DivReinvest)
	{
		wxDateTime div_date = IsValidDate(T, "Div reinvest date is not valid in StockNode constructor!");
		this->div_reinvest_start_date = div_date;
	}
}

StockNode::StockNode(const StockNode& sn) : m_ticker(sn.m_ticker)
{
	this->historical_prices = sn.historical_prices;
	this->bought = sn.bought;
	this->m_parentclock = sn.m_parentclock;
	this->inital_purchase = sn.inital_purchase;
	this->m_parent = sn.m_parent;
	this->m_grand_parent = sn.m_grand_parent;
	this->SetSummaryData(sn.current_Data);
	this->DivReinvest = sn.DivReinvest;
	this->div_reinvest_start_date = sn.div_reinvest_start_date;
//	this->Calibrate();
}

StockNode::~StockNode()
{
	this->DeleteParser();
}

bool StockNode::Purchase(long id, wxString s, double price_per_share, double shares)
{
	wxDateTime date = IsValidDate(s, "Invalid or future date in StockNode::Purchase!");
	if (!date.IsValid())
		return false;

	if (shares == 0.0)
	{
		wxFAIL_MSG("Shares passed to StockNode::Purchase is 0! Purchase failed!");
		return false;
	}

	return this->InitiatePurchase(id, date, price_per_share, shares);
}

bool StockNode::InitiatePurchase(long id, wxDateTime date, double price_per_share, double shares)
{
	if (this->m_Purchase(id, date, price_per_share, shares))
	{
		if (this->bought.size() > 1)
		{
			if (this->inital_purchase.IsValid() && !this->m_FindDateInHistoricalPrices(this->inital_purchase))
				this->parser = this->NewParser();
			else if(this->purchaseDateSwitch)
				this->parser = this->NewParser();
		}
		else
			this->parser = this->NewParser();
	}

	if (this->parser)
	{
		if (this->UpDate())
			this->Calibrate();
		this->m_active = 1;
	}

	if (this->bought.size() > 1)
		this->ShareDivstoLastPurchase();

	this->DeleteParser();
	return true;
}

bool StockNode::Sell(long id, wxString s, double price_per_share, double shares)
{
	stock_node* sn = this->FindLot(id);
	if (!sn)
	{
		wxString lot = "";
		lot << id;
		wxFAIL_MSG("Could not find lot number: " + lot + " in StockNode::Sell!");
		return false;
	}

	wxDateTime date = IsValidDate(s, "Invalid or future date in StockNode::Sell!");
	if (!date.IsValid())
		return false;

	return this->m_Sell(id, date, price_per_share, shares, sn);
}

bool StockNode::IdMatch(long l)
{
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		if (this->bought[i].IdMatch(l))
			return true;
	}

	return false;
}

wxString StockNode::GetTicker()
{
	return this->m_ticker;
}

double StockNode::GetShares()
{
	wxDateTime T = *this->m_parentclock;
	return this->div_shares + this->shares;
	return this->m_GetShares(&T);
}

bool StockNode::TickerMatch(wxString ticker)
{
	if (this->m_ticker == ticker)
		return true;

	return false;
}

wxString StockNode::GetNextEarningsDate()
{
	return this->current_Data.earningsdate;
}

bool StockNode::IsActive()
{
	return this->m_active;
}

void StockNode::Calibrate()
{
	wxDateTime T = *this->m_parentclock;
	double daycostbasis = this->GetDayCostBasis(&T);
	double weekcostbasis = this->GetWeekCostBasis(&T);
	double quartercostbasis = this->GetQuarterCostBasis(&T);
	double yearcostbasis = this->GetYearCostBasis(&T);
	wxDateTime initial = this->GetInitalPurchaseDate();
	double originalcostbasis = this->GetTotalCostBasis(&T);
	double marketvalue = this->GetMarketValue(&T);
	T = *this->m_parentclock;
	double _shares = this->m_GetShares(&T);
	double divshares = this->GetDividendShares();
	double _dividends = this->GetDividends();

	this->SetNewValues(originalcostbasis, daycostbasis, weekcostbasis, quartercostbasis, yearcostbasis, marketvalue, _shares, divshares, _dividends);
	this->UpToDate = 1;

	this->svd = StockViewerData(this->m_ticker, this->current_Data.earningsdate, this->inital_purchase.Format(STANDARD_DATE),
		_shares, originalcostbasis / _shares, this->current_Data.marketprice, this->current_Data.previousclose,
		this->day_return, this->week_return, this->quarter_return, this->year_return, this->total_return, originalcostbasis,
		marketvalue, this->GetDividends());
}

void StockNode::ClockChange()
{
	UpToDate = 0;
}

// StockNode private functions...

bool StockNode::UpDate()
{
	Holidays holiday;
	wxDateTime today = wxDateTime::Today();
	if (this->current_Data.date != today)
	{
		this->parser->UpDateAll();
		return true;
	}
	this->parser->UpDateHistoricalPrices();
	this->parser->UpDateDiv();
	return true;
}

bool StockNode::Historical_prices_UpToDate()
{
	size_t size = this->historical_prices.size();
	if (!size)
		return false;

	wxDateTime latest = this->GetLatestMarketOpenDate();
	if (this->historical_prices.rbegin()->date == latest)
		return true;

	return false;
}

int StockNode::m_GetStartWeekDay(wxDateTime* T)
{
	if (!T)
	{
		wxFAIL_MSG("wxDateTime* passed to StockNode::m_GetStartWeekDay is nullptr!");
		return 0;
	}

	if (!T->IsValid())
	{
		wxFAIL_MSG("wxDateTime* passed to StockNode::m_GetStartWeekDay is Invalid Date!");
		return 0;
	}

	int day = T->GetDay();
	int weekday = T->GetWeekDay();
	switch (weekday)
	{
	case wxDateTime::WeekDay::Mon: return T->GetDayOfYear();
	case wxDateTime::WeekDay::Tue: return T->GetDayOfYear() - 1;
	case wxDateTime::WeekDay::Wed: return T->GetDayOfYear() - 2;
	case wxDateTime::WeekDay::Thu: return T->GetDayOfYear() - 3;
	case wxDateTime::WeekDay::Fri: return T->GetDayOfYear() - 4;
	case wxDateTime::WeekDay::Sat: return T->GetDayOfYear() - 5;
	case wxDateTime::WeekDay::Sun: return 0;
	}

	wxString wd = "";
	wd << weekday;
	wxFAIL_MSG("Weekday: " + wd + " matched no days in the switch statement in StockNode::m_GetStartWeekDay!");
	return 0;
}

wxDateTime StockNode::GetLatestMarketOpenDate()
{
	wxDateTime T = wxDateTime::Today();
	GetWorkDate(T);
	return T;
}

double StockNode::GetClosePrice(wxDateTime* T)
{
	if (!T)
	{
		wxMessageBox("wxDateTime* is null in StockNode::GetClosePrice!");
		return 0.0;
	}
	wxString enddate = this->historical_prices.rbegin()->date.Format(STANDARD_DATE);
	for (size_t i = 0; i < this->historical_prices.size(); ++i)
	{
		wxString Tclose = T->Format(STANDARD_DATE);
		wxString close = this->historical_prices[i].date.Format(STANDARD_DATE);
		double closep = this->historical_prices[i].close;
		if (this->historical_prices[i].date == *T)
			return this->historical_prices[i].close;
	}

	return 0.0;
}

double StockNode::GetPreviousClose(wxDateTime* T)
{
	if (!T)
	{
		if (this->historical_prices.rend() == this->historical_prices.rend())
			return 0.0;
		auto it = this->historical_prices.rbegin();
		++it;
		if (it == this->historical_prices.rend())
			return 0.0;
		return it->close;
	}

	for (size_t i = 0; i < this->historical_prices.size(); ++i)
	{
		if (this->historical_prices[i].date == *T)
		{
			if (i > 0)
				return this->historical_prices[--i].close;
			else
				return 0.0;
		}
	}

	wxMessageBox("Date not found in StockNode::GetPreviousClose!");
	return 0.0;
}

double StockNode::GetEarliestWeekDayClose(wxDateTime* T)
{
	wxDateTime date = wxDateTime::Today();
	if (T)
		date = *T;

	date = this->GetFirstDayOfTheWeekDate(&date);

	return this->GetClosePrice(&date);
}

double StockNode::GetQuarterStartClose(wxDateTime* T)
{
	wxDateTime date = wxDateTime::Today();
	if (T)
		date = *T;

	date = this->GetQuarterStartDate(&date);
	
	return this->GetClosePrice(&date);
}

wxDateTime StockNode::GetQuarterStartDate(wxDateTime* T)
{
	wxDateTime date = wxDateTime::Today();
	if (T)
		date = *T;
	wxString before = date.Format(STANDARD_DATE);
	date = wxDateTime(1, GetQuarterStartMonth(date.GetMonth()), date.GetYear());
	GetForwardWorkDay(date);
	wxString after = date.Format(STANDARD_DATE);
	return date;
}

double StockNode::GetYearStartClose(wxDateTime* T)
{
	wxDateTime date = wxDateTime::Today();
	if (T)
		date = *T;

	date = wxDateTime(2, wxDateTime::Month::Jan, date.GetYear());
	GetForwardWorkDay(date);
	wxString year = date.Format(STANDARD_DATE);
	return this->GetClosePrice(&date);
}

double StockNode::GetDayCostBasis(wxDateTime* T)
{
	double cost_basis = 0.0;
	double price = this->GetPreviousClose(T);
	double temp = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		temp = this->bought[i].GetPurchasePrice(T);
		if (!temp)
			cost_basis += price * this->bought[i].GetShares(T);
		else
			cost_basis += temp * this->bought[i].GetShares(T);
	}

	return cost_basis;
//	case DAY: return sn->GetPurchasePrice(begin_date);
//	case WEEK: return sn->GetPurchasePrice(this->m_GetStartWeekDay(begin_date), this->m_GetEndWeekDay(begin_date), begin_date->GetYear());
//	case QUARTER: return sn->GetPurchasePrice(GetQuarterStartMonth(begin_date->GetMonth()), begin_date->GetYear());
//	case YEAR: return sn->GetPurchasePrice(begin_date->GetYear());
//	case ALL_TIME: return sn->GetPurchasePrice();
}

double StockNode::GetWeekCostBasis(wxDateTime* T)
{
	if (!T)
	{
		wxMessageBox("wxDateTime* in StockNode::GetWeekCostBasis is nullptr!");
		return 0.0;
	}

	wxDateTime copy = *T;
	double cost_basis = 0.0;
	double price = this->GetEarliestWeekDayClose(&copy);
	double temp = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		temp = this->bought[i].GetPurchasePrice(this->m_GetStartWeekDay(&copy), copy.GetDayOfYear(), copy.GetYear());
		if (!temp)
			cost_basis += price * this->bought[i].GetShares(&copy);
		else
			cost_basis += temp * this->bought[i].GetShares(&copy);
	}

	return cost_basis;
}
double StockNode::GetQuarterCostBasis(wxDateTime* T)
{
	if (!T)
	{
		wxMessageBox("wxDateTime* in StockNode::GetQuarterCostBasis is nullptr!");
		return 0.0;
	}

	wxDateTime copy = *T;
	double cost_basis = 0.0;
	double price = this->GetQuarterStartClose(&copy);
	double temp = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		temp = this->bought[i].GetPurchasePrice(GetQuarterStartMonth(copy.GetMonth()), copy.GetDayOfYear(), copy.GetYear());
		if (!temp)
			cost_basis += price * this->bought[i].GetShares(&copy);
		else
			cost_basis += temp * this->bought[i].GetShares(&copy);
	}

	return cost_basis;
}

double StockNode::GetYearCostBasis(wxDateTime* T)
{
	if (!T)
	{
		wxMessageBox("wxDateTime* in StockNode::GetYearCostBasis is nullptr!");
		return 0.0;
	}
	
	wxDateTime copy = *T;
	double cost_basis = 0.0;
	double price = this->GetYearStartClose(&copy);
	double temp = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		temp = this->bought[i].GetPurchasePrice(copy.GetYear(), copy.GetDayOfYear());
		if (!temp)
			cost_basis += price * this->bought[i].GetShares(&copy);
		else
			cost_basis += temp * this->bought[i].GetShares(&copy);
	}

	return cost_basis;
}

double StockNode::GetTotalCostBasis(wxDateTime* T)
{
	if (!T)
	{
		wxMessageBox("wxDateTime* in StockNode::GetTotalCostBasis is nullptr!");
		return 0.0;
	}

	wxDateTime copy = *T;
	double cost_basis = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
		cost_basis += this->bought[i].GetPurchasePrice() * this->bought[i].GetShares(&copy);

	return cost_basis;
}

double StockNode::GetMarketValue(wxDateTime* T)
{
	if (!T)
	{
		wxMessageBox("wxDateTime* in StockNode::GetMarketValue is nullptr!");
		return 0.0;
	}

	if (*T == wxDateTime::Today())
		return this->current_Data.marketprice * this->m_GetShares(T) + this->GetDividendShares();

	wxDateTime copy = *T;
	GetWorkDate(copy);
	return this->GetClosePrice(&copy) * this->m_GetShares(T) + this->GetDividendShares();
}

bool StockNode::m_Purchase(long& id, wxDateTime& date, double& price_per_share, double& shares)
{
	wxDateTime r_date(wxInvalidDateTime);
	if (this->DivReinvest)
		r_date = this->div_reinvest_start_date;
	this->bought.push_back(stock_node(id, this->m_ticker, date, price_per_share, shares, BUY, r_date));
	this->lastPurchase = &this->bought[this->bought.size() - 1];

	if (!this->inital_purchase.IsValid())
		this->inital_purchase = date;
	else
	{
		if (this->inital_purchase > date)
		{
			this->inital_purchase = date;
			this->purchaseDateSwitch = true;
		}
	}
	return true;
}

bool StockNode::m_Sell(long& id, wxDateTime& date, double& price_per_share, double& shares, stock_node* sn)
{
	wxDateTime r_date(wxInvalidDateTime);
	if (this->DivReinvest)
		r_date = this->div_reinvest_start_date;
	sn->SetSibling(stock_node(id, this->m_ticker, date, price_per_share, shares, SELL, r_date));
	return true;
}

stock_node* StockNode::FindLot(long& id)
{
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		if (bought[i].IdMatch(id))
			return &bought[i];
	}

	return nullptr;
}

Parser* StockNode::NewParser()
{
	this->DeleteParser();
	wxDateTime* latest_close_date = this->m_GetLatestCloseDate();
	wxDateTime* divdate = this->m_GetLatestDividendDate();
	wxString close_date_string = latest_close_date && !this->purchaseDateSwitch ? latest_close_date->Format(DATE_KEY) : "";
	wxString div_date = divdate ? divdate->Format(DATE_KEY) : "";
	wxString enddate = this->historical_prices.size() && this->purchaseDateSwitch ? this->historical_prices[0].date.Format(DATE_KEY) : "";
	return new Parser(this, this->m_ticker, this->GetInitalPurchaseDate().Format(DATE_KEY), close_date_string, 
		div_date, SetHistoricalDataCB, SetSummaryDataCB, SetDividendsCB, enddate);
}

void StockNode::DeleteParser()
{
	if (this->parser)
		delete this->parser;

	this->parser = nullptr;
}

void StockNode::ShareDivstoLastPurchase()
{
	if(this->lastPurchase)
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		this->bought[i].ShareDivsWithSibling(this->lastPurchase);
	}

	this->purchaseDateSwitch = false;
}

double StockNode::m_GetShares(wxDateTime* date)
{
	double shares = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
		shares += this->bought[i].GetShares(date);

	return shares;
}

wxDateTime StockNode::GetFirstDayOfTheWeekDate(wxDateTime* date)
{
	wxString s_date = date->Format(STANDARD_DATE);
	if (date->GetWeekDay() == wxDateTime::WeekDay::Mon)
		return *date;

	wxDateTime T = *date;
	while (T.GetWeekDay() != wxDateTime::WeekDay::Mon)
	{
		T = T - wxDateSpan(0, 0, 0, 1);
	}

	s_date = T.Format(STANDARD_DATE);
	return T;
}

wxDateTime* StockNode::m_GetLatestCloseDate()
{
	return this->historical_prices.rbegin() != this->historical_prices.rend() ? &this->historical_prices.rbegin()->date : nullptr;
}

wxDateTime* StockNode::m_GetLatestDividendDate()
{
	wxDateTime* latest = nullptr;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		wxDateTime* temp = this->bought[i].GetLatestDivDate();
		if ((latest && !latest->IsValid()) || !latest)
			latest = temp;
		else
		{
			if (temp && temp->IsValid())
			{
				if (*latest < *temp)
					latest = temp;
			}
		}
	}
	return latest;
}

wxDateTime StockNode::GetInitalPurchaseDate()
{
	wxDateTime date(wxInvalidDateTime);
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		if (!date.IsValid() || date > this->bought[i].GetPurchaseDate())
			date = this->bought[i].GetPurchaseDate();
	}

	return date;
}

wxDateTime* StockNode::m_FindDateInHistoricalPrices(wxDateTime& date)
{
	for (size_t i = 0; i < this->historical_prices.size(); ++i)
	{
		if (this->historical_prices[i].date == date)
			return &this->historical_prices[i].date;
	}

	return nullptr;
}

wxDateTime* StockNode::m_FindDateInHistoricaDivs(wxDateTime& date)
{
	wxDateTime* div_date = nullptr;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		div_date = this->bought[i].GetDividendDates(date);
		if (div_date)
			return div_date;
	}

	return nullptr;
}

void StockNode::SetSummaryData(SummaryData sd)
{
	this->UpToDate = 0;
	this->m_beta = this->current_Data.Beta;
	this->current_Data = sd;
	if (sd.beta == "NotFound")
		this->current_Data.Beta = this->m_beta;
}

void StockNode::SetDividends(Dividend d)
{
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		if (this->bought[i].GetPurchaseDate() < d.ex_Div)
			this->bought[i].HandleDivReInvest(d);
	}
}

void StockNode::OnThreadComplete(wxCommandEvent&)
{
	this->m_parent->ThreadComplete(this);
}

double StockNode::GetDividends()
{
	wxDateTime T = *this->m_parentclock;
	double d = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		d += this->bought[i].GetDividends(&T);
	}

	return d;
}

StockViewerData* StockNode::GetStockViewerData()
{
	return &this->svd;
}

double StockNode::GetDividendShares()
{
	wxDateTime T = *this->m_parentclock;
	double d = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
		d += this->bought[i].GetDividendShares(&T);

	return d;
}

void StockNode::SetHistoricalData(Day_Prices day)
{
	this->UpToDate = 0;
	size_t i = 0;
	while (i < this->historical_prices.size() && this->historical_prices[i].date < day.date)
		++i;
	if ((i == this->historical_prices.size()) || (i == 0 && !this->historical_prices.size()))
		this->historical_prices.push_back(day);
	else
		this->historical_prices.insert(&this->historical_prices[i], day);
}
/*
double StockNode::m_GetClosePrice(wxDateTime* date)
{
	wxString s_date = date->Format(STANDARD_DATE);
	for (size_t i = 0; i < this->historical_prices.size(); ++i)
	{
		wxString fuckdate2 = this->historical_prices[i].date.Format(STANDARD_DATE);
		if (this->historical_prices[i].date == *date)
			return this->historical_prices[i].close;
	}
	return 0.0;
}
double StockNode::m_GetPreviousDayClosePrice(wxDateTime* date)
{
	for (size_t i = 0; i < this->historical_prices.size(); ++i)
	{
		if (this->historical_prices[i].date == *date && i > 0)
			return this->historical_prices[i - 1].close;
	}

	return 0.0;
}

double StockNode::m_GetLatestClosePrice()
{
	size_t size = this->historical_prices.size();
	if (!size)
		return 0.0;

	return this->historical_prices[size - 1].close;
}

double StockNode::m_GetForwardClosePrice(wxDateTime* T)
{
	wxString date = T->Format(STANDARD_DATE);
	for (size_t i = 0; i < this->historical_prices.size(); ++i)
	{
		if (this->historical_prices[i].date == *T)
			return this->historical_prices[i].close;

		if (this->historical_prices[i].date > *T)
				return this->historical_prices[i].close;
	}
	wxString something = date;
	return 0.0;
}

double StockNode::m_GetPurchasePrice(wxDateTime* date)
{
	wxDateTime T = *this->m_parentclock;
	double value = 0.0;
	double shares = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		wxDateTime purchasedate = this->bought[i].GetPurchaseDate();
		shares += this->bought[i].GetShares(&T);
		value = this->bought[i].GetPurchasePrice() * this->bought[i].GetShares(&T);
	}

	if (shares)
		return value / shares;

	return 0.0;
}

double StockNode::m_GetPurchaseCostBasis(wxDateTime* date)
{
	wxDateTime T = *this->m_parentclock;
	double value = 0.0;
	double shares = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		value += this->bought[i].GetPurchasePrice() * this->bought[i].GetShares(&T);
	}

	return value;
}

double StockNode::m_GetMarketValue(wxDateTime* date, Returns r)
{
	double marketPrice = this->m_GetMarketPrice(r, date);
	double shares = this->m_GetShares(date) + this->GetDividendShares();
	double d = this->m_GetMarketPrice(r, date) * shares;
	return d;
}

double StockNode::m_GetRealizedGain(wxDateTime* date)
{
	double realized_gain = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
		realized_gain += this->bought[i].GetRealizedGain(date);

	return realized_gain;
}

double StockNode::m_GetDayCostPricePerShare(wxDateTime* date, wxDateTime* enddate, Returns r)
{
	if (this->bought.size())
		return 0.0;
	
	wxDateTime startdate = this->m_GetStartDate(r, date);
	double _price = this->m_GetCostPrice(r, date);
	double _shares = 0.0;
	double temp = 0.0;
	double value = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		double loopshares = 0.0;
		_shares += loopshares = this->bought[i].GetShares(enddate);
		temp = this->m_GetPurchasePrice(r, &bought[i], date, enddate);
		if (!temp)
			value += _price * loopshares;
		else
			value += temp * loopshares;
	}
	
	double price_per_share = _shares ? value / _shares : 0.0;
	return price_per_share;
}

double StockNode::m_GetDayCostBasis(wxDateTime* begin_date, Returns r)
{
	wxDateTime T = *this->m_parentclock;
	wxDateTime startdate = this->m_GetStartDate(r, begin_date);
	wxString fuckyou = startdate.Format(STANDARD_DATE);
	int fuckyouday = startdate.GetWeekDay();
	double _price = this->m_GetCostPrice(r, &startdate);
	double temp = 0.0;
	double cost_basis = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		temp = this->m_GetPurchasePrice(r, &bought[i], begin_date, &T);
		if (!temp)
			cost_basis += _price * this->bought[i].GetShares(&T);
		else
			cost_basis += temp * this->bought[i].GetShares(&T);
	}

	return cost_basis;
}

double StockNode::m_GetPurchasePrice(Returns r, stock_node* sn, wxDateTime* begin_date, wxDateTime* end_date)
{
	switch (r)
	{
	case DAY: return sn->GetPurchasePrice(begin_date);
	case WEEK: return sn->GetPurchasePrice(this->m_GetStartWeekDay(begin_date), this->m_GetEndWeekDay(begin_date), begin_date->GetYear());
	case QUARTER: return sn->GetPurchasePrice(GetQuarterStartMonth(begin_date->GetMonth()), begin_date->GetYear());
	case YEAR: return sn->GetPurchasePrice(begin_date->GetYear());
	case ALL_TIME: return sn->GetPurchasePrice();
	}

	wxString R = "";
	R << r;
	wxFAIL_MSG("Return enum passed to StockNode::m_GetPurchasePrice did not match in the switchstatment! Return enum is: " + R);
	return 0.0;
}

double StockNode::m_GetMarketPrice(Returns r, wxDateTime* date)
{
	switch (r)
	{
	case DAY: return this->m_GetForwardClosePrice(date);
	case WEEK: return this->m_GetClosePrice(date);
	case QUARTER: return this->m_GetClosePrice(date);
	case YEAR: return this->m_GetClosePrice(date);
	case ALL_TIME: return this->m_GetLatestClosePrice();
	}

	wxString R = "";
	R << r;
	wxFAIL_MSG("Return enum passed to StockNode::m_GetMarketPrice did not match in the switchstatment! Return enum is: " + R);
	return 0.0;
}

double StockNode::m_GetCostPrice(Returns r, wxDateTime* date)
{
	switch (r)
	{
	case DAY: return this->m_GetClosePrice(date);
	case WEEK: return this->m_GetForwardClosePrice(date);
	case QUARTER: return this->m_GetForwardClosePrice(date);
	case YEAR: return this->m_GetForwardClosePrice(date);
	case ALL_TIME: return this->m_GetForwardClosePrice(date);
	}

	wxString R = "";
	R << r;
	wxFAIL_MSG("Return enum passed to StockNode::m_GetCostPrice did not match in the switchstatment! Return enum is: " + R);
	return 0.0;
}

wxDateTime StockNode::m_GetStartDate(Returns r, wxDateTime* date)
{
	switch (r)
	{
	case Returns::DAY: 
	{
		// if date lands on saturday or a sunday, we need to call GetWorkDate and then subtract another day from it.
		// by calling GetWorkDate again but with true as the second param it will comback with the previous day that was not a holiday
		if (date->GetWeekDay() == wxDateTime::Sun || date->GetWeekDay() == wxDateTime::Sat)
		{
			GetWorkDate(*date);
			GetWorkDate(*date, true);
		}
		else if (date->GetWeekDay() == wxDateTime::Mon)
			GetWorkDate(*date, true);
		else
			GetWorkDate(*date);
		wxString Date = date->Format(STANDARD_DATE);
		return *date;//*date - wxDateSpan(0, 0, 0, 1);
	}
	case Returns::WEEK: return this->GetFirstDayOfTheWeekDate(date);
	case Returns::QUARTER: return wxDateTime(1, GetQuarterStartMonth(date->GetMonth()), date->GetYear());
	case Returns::YEAR: return wxDateTime(2, wxDateTime::Month::Jan, date->GetYear());
	case Returns::ALL_TIME: return this->GetInitalPurchaseDate();
	}

	wxString R = "";
	R << r;
	wxFAIL_MSG("Return enum passed to StockNode::m_GetStartDate did not match in the switchstatment! Return enum is: " + R);
	return wxDateTime(wxInvalidDateTime);
}

int StockNode::m_GetEndWeekDay(wxDateTime* T)
{
	if (!T)
	{
		wxFAIL_MSG("wxDateTime* passed to StockNode::m_GetEndWeekDay is nullptr!");
		return 0;
	}

	if (!T->IsValid())
	{
		wxFAIL_MSG("wxDateTime* passed to StockNode::m_GetEndWeekDay is Invalid Date!");
		return 0;
	}

	int day = T->GetDay();
	int weekday = T->GetWeekDay();
	int lastmonthday = T->GetLastMonthDay(T->GetMonth(), T->GetYear()).GetDay();
	switch (weekday)
	{
	case wxDateTime::WeekDay::Mon: return T->GetDayOfYear() + 4;
	case wxDateTime::WeekDay::Tue: return T->GetDayOfYear() + 3;
	case wxDateTime::WeekDay::Wed: return T->GetDayOfYear() + 2;
	case wxDateTime::WeekDay::Thu: return T->GetDayOfYear() + 1;
	case wxDateTime::WeekDay::Fri: return T->GetDayOfYear();
	case wxDateTime::WeekDay::Sat: return T->GetDayOfYear() - 1;
	case wxDateTime::WeekDay::Sun: return 0;
	}

	wxString wd = "";
	wd << weekday;
	wxFAIL_MSG("Weekday: " + wd + " matched no days in the switch statement in StockNode::m_GetEndWeekDay!");
	return 0;
}
*/
// Sector Functions...

Sector::Sector(_Sector s, wxDateTime* date, Portfolio* P) : id(s), m_parentclock(date), m_parent(P)
{
	
}

Sector::~Sector()
{

}

bool Sector::Purchase(long lot, wxString ticker, wxString date, double price, double shares, bool div_re, wxString T)
{
	StockNode* S = this->GetStockNode(ticker);
	if (!S)
		S = this->CreateStockNode(lot, ticker, div_re, T);

	if (!S->Purchase(lot, date, price, shares))
	{
		if (S->IsActive())
			this->stocks.erase(S);
		return false;
	}

	this->threads_running++;
	return true;
}

bool Sector::IsId(_Sector s)
{
	if (this->id == s)
		return true;

	return false;
}

wxVector<StockNode*> Sector::GetStockVector()
{
	wxVector<StockNode*> sn;
	for (size_t i = 0; i < this->stocks.size(); ++i)
		sn.push_back(&this->stocks[i]);

	return sn;
}

void Sector::ThreadComplete(StockNode* sn)
{
	this->threads_running--;
	this->m_parent->ThreadComplete(sn);
}

int Sector::GetNumItems(_PortfolioType p)
{
	switch (p)
	{
	case _PortfolioType::STOCK: return this->GetStockSize();
	}

	return 0;
}

// Sector private functions...

StockNode* Sector::GetStockNode(wxString ticker)
{
	for (size_t i = 0; i < this->stocks.size(); ++i)
	{
		if (this->stocks[i].TickerMatch(ticker))
			return &this->stocks[i];
	}

	return nullptr;
}

StockNode* Sector::CreateStockNode(long l, wxString ticker, bool div_re, wxString T)
{
	this->stocks.push_back(StockNode(ticker, this->m_parentclock, this->m_parent, this, div_re, T));
	return &this->stocks[this->stocks.size() - 1];
}

int Sector::GetStockSize()
{
	return this->stocks.size();
}

// Portfolio Functions...

Portfolio::Portfolio(wxFrame* f, double money, wxDateTime* m) : m_parent(f), cash(money), m_parentclock(m)
{

}

bool Portfolio::Purchase(_Sector s, wxString ticker, wxString purchasedate, double price, double shares, bool div_re, wxString T)
{
	long lotnumber = this->GetLot();
	wxDateTime date;
	date.ParseDate(purchasedate);
	if (!this->cash.RequestPurchase(lotnumber, price * shares))
		return false;

	if (!this->m_Purchase(lotnumber, s, ticker, purchasedate, price, shares, div_re, T))
		return false;

	this->threads_running++;
	return true;
}

bool Portfolio::NewDepositSchedule(double d, int i, wxDateTime date)
{
	bool result = this->cash.NewDepositSchedule(d, i, date);
	if (result)
		this->cash.UpdateCash();

	return result;
}

int Portfolio::GetNumItems(_PortfolioType p)
{
	int num = 0;
	for (size_t i = 0; i < this->sectors.size(); ++i)
		num += this->sectors[i].GetNumItems(p);

	return num;	
}

const wxVector<StockNode*> Portfolio::GetStockNodeItems()
{
	wxVector<StockNode*> sn;
	wxVector<StockNode*> returnvec;
	for (size_t i = 0; i < this->sectors.size(); ++i)
	{
		sn = this->sectors[i].GetStockVector();
		for (size_t j = 0; j < sn.size(); ++j)
			returnvec.push_back(sn[j]);
	}

	return returnvec;
}

void Portfolio::ThreadComplete(StockNode* sn)
{
	this->threads_running--;
	if (!this->threads_running)
		wxPostEvent(this->m_parent, wxThreadEvent());
	
}

// Portfolio private functions...

long Portfolio::GetLot()
{
	long increment = 0;
	if (!lot.size())
	{
		lot.push_back((long)1);
		return (long)1;
	}

	for (size_t i = 0; i < this->lot.size(); ++i)
	{
		if (lot[i] != increment)
		{
			lot.insert(&lot[i], increment);
			return increment;
		}
		++increment;
	}

	lot.push_back(increment);
	return increment;
}

bool Portfolio::m_Purchase(long lotnumber, _Sector s, wxString ticker, wxString purchasedate, double price, double shares, bool div_re, wxString T)
{
	Sector* S = this->GetSector(s);
	if (!S)
	{
		S = this->CreateSector(s);
	}

	if (!S->Purchase(lotnumber, ticker, purchasedate, price, shares, div_re, T))
		return false;

	wxDateTime date;
	date.ParseDate(purchasedate);
	return this->cash.Purchase(lotnumber, price * shares, date);
}

Sector* Portfolio::GetSector(_Sector s)
{
	for (size_t i = 0; i < this->sectors.size(); ++i)
	{
		if (this->sectors[i].IsId(s))
			return &this->sectors[i];
	}

	return nullptr;
}

Sector* Portfolio::CreateSector(_Sector s)
{
	this->sectors.push_back(Sector(s, this->m_parentclock, this));
	return &this->sectors[this->sectors.size() - 1];
}