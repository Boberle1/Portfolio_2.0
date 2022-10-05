#pragma once
#include "CashAccount.h"
#include "Parser.h"
#include "wx/numformatter.h"
#include "wx/thread.h"
#include "wx/event.h"

constexpr auto STANDARD_DATE = "%m/%d/%G";
constexpr auto DATE_KEY = "%G-%m-%d";
wxDEFINE_EVENT(ThreadComplete, wxCommandEvent);
//wxCommandEvent event(ThreadComplete);
enum Action
{
	BUY = 0,
	SELL,
	UNDEFINED
};

enum Returns
{
	DAY = UNDEFINED + 1,
	WEEK,
	MONTH,
	QUARTER, 
	YEAR,
	ALL_TIME
};

enum _Sector
{
	BOND = ALL_TIME + 1,
	REIT,
	SMALLCAP,
	MIDCAP,
	LARGECAP,
	TECH,
	CRYPTO,
	FOREIGN
};

enum _PortfolioType
{
	STOCK = FOREIGN + 1,
	SECTOR,
	PORTFOLIO,
	LOT_PURCHASE,
	SOLD,
	OPTIONS
};

// Global function to check for valid date and is not a date in the future...
inline wxDateTime IsValidDate(wxString&, wxString);
unsigned int inline GetEndMonthDay(wxDateTime::Month, int);
wxDateTime::Month inline GetQuarterStartMonth(wxDateTime::Month);
wxDateTime::Month inline GetQuarterEndMonth(wxDateTime::Month);

// Global function to check if a ptr is null...
template <typename T>
bool IsNull(T*, wxString);


struct Day_Prices
{
	Day_Prices(double o, double h, double l, double c, wxDateTime t)
		: open(o), high(h), low(l), close(c), date(t) {}

	double open = 0.0;
	double high = 0.0;
	double low = 0.0;
	double close = 0.0;
	wxDateTime date;
};

class Return_node
{
public:
	Return_node() {}
	Return_node(double ocb, double dcb, double wcb, double qcb, double ycb, double m, double s, double divshares) 
		: original_cost_basis(ocb), day_cost_basis(dcb), week_cost_basis(wcb), 
		quarter_cost_basis(qcb), year_cost_basis(ycb), market_value(m), shares(s), div_shares(divshares)
	{
		this->Calibrate();
	}

	void SetNewValues(double ocb, double dcb, double wcb, double qcb, double ycb, double m, double s, double divshares, double divs)
	{
		this->original_cost_basis = ocb;
		this->day_cost_basis = dcb;
		this->week_cost_basis = wcb;
		this->quarter_cost_basis = qcb;
		this->year_cost_basis = ycb;
		this->market_value = m;
		this->shares = s;
		this->div_shares = divshares;
		this->dividends = divs;
		this->Calibrate();
	}

	void Calibrate()
	{
		// Percent returns...
		this->price_per_share = shares ? market_value / shares - 1 : 0.0;
		this->day_return = day_cost_basis ? market_value / day_cost_basis - 1 : 0.0;
		this->week_return = week_cost_basis ? market_value / week_cost_basis - 1 : 0.0;
		this->quarter_return = quarter_cost_basis ? market_value / quarter_cost_basis - 1 : 0.0;
		this->year_return = year_cost_basis ? market_value / year_cost_basis - 1 : 0.0;
		this->total_return = original_cost_basis ? market_value / original_cost_basis - 1 : 0.0;

		// $ Returns...
		this->day_return$ = market_value - day_cost_basis;
		this->week_return$ = market_value - week_cost_basis;
		this->quarter_return$ = market_value - quarter_cost_basis;
		this->year_return$ = market_value - year_cost_basis;
		this->total_return$ = market_value - original_cost_basis;
	}

	double GetDayReturn(bool times_100 = true)
	{
		if (times_100)
			return this->day_return * 100;

		return this->day_return;
	}

	double GetDayReturn$()
	{
		return this->day_return$;
	}

	double GetWeekReturn(bool times_100 = true)
	{
		if (times_100)
			return this->week_return * 100;

		return this->week_return;
	}

	double GetWeekReturn$()
	{
		return this->week_return$;
	}

	double GetQuarterReturn(bool times_100 = true)
	{
		if (times_100)
			return this->quarter_return * 100;

		return this->quarter_return;
	}

	double GetQuarterReturn$()
	{
		return this->quarter_return$;
	}

	double GetYearReturn(bool times_100 = true)
	{
		if (times_100)
			return this->year_return * 100;

		return this->year_return;
	}

	double GetYearReturn$()
	{
		return this->year_return$;
	}

	double GetTotalReturn(bool times_100)
	{
		if (times_100)
			return this->total_return * 100;

		return this->total_return;
	}

	double GetTotalReturn$()
	{
		return this->total_return$;
	}

	double original_cost_basis = 0.0;
	double day_cost_basis = 0.0;
	double week_cost_basis = 0.0;
	double quarter_cost_basis = 0.0;
	double year_cost_basis = 0.0;
	double market_value = 0.0;
	double shares = 0.0;
	double price_per_share = 0.0;
	double day_return = 0.0;
	double day_return$ = 0.0;
	double week_return = 0.0;
	double week_return$ = 0.0;
	double quarter_return = 0.0;
	double quarter_return$ = 0.0;
	double year_return = 0.0;
	double year_return$ = 0.0;
	double total_return = 0.0;
	double total_return$ = 0.0;

	double div_shares = 0.0;
	double dividends = 0.0;
};

struct StockViewerData
{
	StockViewerData(){}
	StockViewerData(wxString t, wxString e, wxString pd, double s, double pp, double p, double pc, double dg, double wg,
		double qg, double yg, double tg, double cb, double mv, double td)
		: ticker(t), earnings(e), purchase_date(pd), shares(s), purchase_price(pp), price(p), previous_close(pc), day_gain(dg),
		week_gain(wg), quarter_gain(qg), year_gain(yg), total_gain(tg), cost_basis(cb), market_value(mv), total_divs(td) {}
	wxString GetTicker() { return this->ticker; }
	wxString GetEarningsDate() { return this->earnings; }
	wxString GetPurchaseDate() { return this->purchase_date; }
	wxString GetShares() { return wxNumberFormatter::ToString(this->shares, 2); }
	wxString GetPurchasePrice() { return wxNumberFormatter::ToString(this->purchase_price, 2); }
	wxString GetPrice(){ return wxNumberFormatter::ToString(this->price, 2); }
	wxString GetPreviousClose(){ return wxNumberFormatter::ToString(this->previous_close, 2); }
	wxString GetDayGain(){ return wxNumberFormatter::ToString(this->day_gain * 100, 2) + "%"; }
	wxString GetWeekGain(){ return wxNumberFormatter::ToString(this->week_gain * 100, 2) + "%"; }
	wxString GetQuarterGain() { return wxNumberFormatter::ToString(this->quarter_gain * 100, 2) + "%"; }
	wxString GetYearGain(){ return wxNumberFormatter::ToString(this->year_gain * 100, 2) + "%"; }
	wxString GetTotalGain(){ return wxNumberFormatter::ToString(this->total_gain * 100, 2) + "%"; }
	wxString GetCostBasis(){ return wxNumberFormatter::ToString(this->cost_basis, 2); }
	wxString GetMarketValue(){ return wxNumberFormatter::ToString(this->market_value, 2); }
	wxString GetDividends(){ return wxNumberFormatter::ToString(this->total_divs, 2); }
	wxString ticker = "";
	wxString earnings = "";
	wxString purchase_date = "";
	double shares = 0.0;
	double purchase_price = 0.0;
	double price = 0.0;
	double previous_close = 0.0;
	double day_gain = 0.0;
	double week_gain = 0.0;
	double quarter_gain = 0.0;
	double year_gain = 0.0;
	double total_gain = 0.0;
	double cost_basis = 0.0;
	double market_value = 0.0;
	double total_divs = 0.0;
};

class stock_node
{
public:
	stock_node(long id, wxString ticker, wxDateTime date, double price_per_share, double shares, Action a, wxDateTime);
	void SetSibling(stock_node sn);
	void ShareDivsWithSibling(stock_node*);
	double GetCostBasis(wxDateTime*);
	double GetShares(wxDateTime*);
	double GetRealizedGain(wxDateTime*);
	bool IdMatch(long&);
	wxDateTime GetPurchaseDate();
	// Gets the purchase price for the quarter...
	double GetPurchasePrice(wxDateTime::Month, int, int);
	// Gets the purchase price for the year...
	double GetPurchasePrice(int, int);
	// gets the purchase price for the week
	double GetPurchasePrice(int, int, int);
	// Gets the purchase price for the day...
	double GetPurchasePrice(wxDateTime*);
	// Gets all time purchase price...
	double GetPurchasePrice();
	void HandleDivReInvest(Dividend&);
	void HandleDivReInvest(Dividend*);
	wxDateTime* GetLatestDivDate();
	wxDateTime* GetDividendDates(wxDateTime&);
	double GetDividends(wxDateTime*);
	double GetDividendShares(wxDateTime*);
private:
	double m_GetShares(wxDateTime*);
	void InsertDiv(Dividend& d);
	long m_id = 0;
	wxString m_ticker = "";
	wxDateTime m_date;
	wxDateTime m_reinvest_start;
	double m_price_per_share = 0.0;
	double m_shares = 0.0;
	double m_average_sold_price = 0.0;
	Action action = UNDEFINED;

	
	// Purchased stock_nodes can have multiple siblings because you can sell partial lots. Example, you sell two shares of 
	// a four share lot and later on sell the other two shares. That would create two siblings of the same lot number....
	wxVector<stock_node> sold;
	wxVector<Dividend> divs;
};

class Sector;
class Portfolio;
class StockNode;

// CallBackFunctions for StockNode to get info from Parser...
// Callback for parser to import Day_Prices...
void SetHistoricalDataCB(void*, double o, double h, double l, double c, wxDateTime d);
// CallBack for parser to import SummaryData...
void SetSummaryDataCB(void*, SummaryData);
// Callback for parser to import dividend...
void SetDividendsCB(void*, Dividend);

class StockThread : public wxThread
{
public:
	StockThread(wxThreadKind tk, StockNode*, long, wxDateTime, double, double);
	~StockThread();
	wxThread::ExitCode Entry();
	bool GetResult();
private:
	StockNode* m_parent = nullptr;
	long m_id = 0;
	wxDateTime m_date;
	double m_price = 0.0;
	double m_shares = 0.0;
	bool result = false;
};

class StockNode : public Return_node, public wxEvtHandler
{
public:
	StockNode(wxString, wxDateTime*, Portfolio*, Sector*, bool, wxString);
	StockNode(const StockNode&);
	~StockNode();
	bool Purchase(long id, wxString s, double price_per_share, double shares);
	bool InitiatePurchase(long id, wxDateTime, double price_per_share, double shares);
	bool Sell(long id, wxString s, double price_per_share, double shares);
	bool IdMatch(long);
	bool TickerMatch(wxString);
	wxString GetTicker();
	double GetShares();
	SummaryData GetSummaryData() { return this->current_Data; }
	wxString GetNextEarningsDate();
	bool IsActive();
	void Calibrate();
	void ClockChange();
	void SetHistoricalData(Day_Prices);
	void SetSummaryData(SummaryData);
	void SetDividends(Dividend);
	void OnThreadComplete(wxCommandEvent&);
	double GetDividends();
	StockViewerData* GetStockViewerData();

	// GetDividend Reinvestment shares...
	double GetDividendShares();

private:
	bool UpDate();
	bool Historical_prices_UpToDate();
	int m_GetStartWeekDay(wxDateTime*);
	wxDateTime GetLatestMarketOpenDate();
	double GetClosePrice(wxDateTime*);
	double GetPreviousClose(wxDateTime*);
	double GetEarliestWeekDayClose(wxDateTime*);
	double GetQuarterStartClose(wxDateTime*);
	wxDateTime GetQuarterStartDate(wxDateTime*);
	double GetYearStartClose(wxDateTime*);
	double GetDayCostBasis(wxDateTime*);
	double GetWeekCostBasis(wxDateTime*);
	double GetQuarterCostBasis(wxDateTime*);
	double GetYearCostBasis(wxDateTime*);
	double GetTotalCostBasis(wxDateTime*);
	double GetMarketValue(wxDateTime*);
	bool m_Purchase(long&, wxDateTime&, double&, double&);
	bool m_Sell(long&, wxDateTime&, double&, double&, stock_node*);
	stock_node* FindLot(long&);
	Parser* NewParser();
	void DeleteParser();
	void ShareDivstoLastPurchase();
	double m_GetShares(wxDateTime*);
	wxDateTime GetFirstDayOfTheWeekDate(wxDateTime*);
	wxDateTime* m_GetLatestCloseDate();
	wxDateTime* m_GetLatestDividendDate();
	wxDateTime GetInitalPurchaseDate();
	wxDateTime* m_FindDateInHistoricalPrices(wxDateTime&);
	wxDateTime* m_FindDateInHistoricaDivs(wxDateTime&);
	/*
	// This function will try to match the date in historical prices and return the close date if it does
	// if it doesnt find a match it will go back to the last date that is less than the date passed as a paramater
	// and return that close date...
	double m_GetClosePrice(wxDateTime*);
	double m_GetPreviousDayClosePrice(wxDateTime*);
	double m_GetLatestClosePrice();
	wxDateTime* m_GetLatestCloseDate();
	wxDateTime* m_GetLatestDividendDate();

	// This function will do the same as m_GetClosePrice but if it doesnt find a match it will go forwards by one
	// to the next date and return the close...
	double m_GetForwardClosePrice(wxDateTime*);

	double m_GetShares(wxDateTime*);
	double m_GetPurchasePrice(wxDateTime*);
	double m_GetPurchaseCostBasis(wxDateTime*);
	double m_GetMarketValue(wxDateTime*, Returns);
	double m_GetRealizedGain(wxDateTime*);
	double m_GetDayCostPricePerShare(wxDateTime*, wxDateTime*, Returns);
	double m_GetDayCostBasis(wxDateTime*, Returns);
	double m_GetPurchasePrice(Returns, stock_node*, wxDateTime*, wxDateTime*);
	double m_GetMarketPrice(Returns, wxDateTime*);
	double m_GetCostPrice(Returns, wxDateTime*);
	wxDateTime m_GetStartDate(Returns, wxDateTime*);
	wxDateTime GetInitalPurchaseDate();
	wxDateTime GetFirstDayOfTheWeekDate(wxDateTime*);
	int m_GetEndWeekDay(wxDateTime*);
	wxDateTime* m_FindDateInHistoricalPrices(wxDateTime&);
	wxDateTime* m_FindDateInHistoricaDivs(wxDateTime&);
	*/
private:
	wxVector<stock_node> bought;
	wxVector<Day_Prices> historical_prices;
//	wxVector<Dividend> divs;
	const wxString m_ticker = "";
	StockViewerData svd;
//	Return_node returns;
	SummaryData current_Data;
	const wxDateTime* m_parentclock = nullptr;
	wxDateTime inital_purchase;
	Parser* parser = nullptr;
	double m_beta = 0.0;
	int m_active = 0;
	int UpToDate = 0;
	Sector* m_parent = nullptr;
	Portfolio* m_grand_parent = nullptr;
	StockThread* thread = nullptr;
	bool DivReinvest = false;
	wxDateTime div_reinvest_start_date;
	stock_node* lastPurchase = nullptr;
	bool purchaseDateSwitch = false;
};

class Sector : public Return_node
{
public:
	Sector(_Sector, wxDateTime*, Portfolio*);
	~Sector();
	bool Purchase(long, wxString, wxString, double, double, bool, wxString);
	bool IsId(_Sector);
	int GetNumItems(_PortfolioType);
	wxVector<StockNode*> GetStockVector();
	
	//call back from for StockNode after thread completion...
	void ThreadComplete(StockNode*);
private:
	StockNode* GetStockNode(wxString);
	StockNode* CreateStockNode(long, wxString, bool, wxString);
	int GetStockSize();
	wxVector<StockNode> stocks;
	wxDateTime* m_parentclock = nullptr;
	_Sector id;
	Portfolio* m_parent = nullptr;
	int threads_running = 0;
};

class Portfolio : public Return_node
{
public:
	Portfolio(wxFrame*, double, wxDateTime*);
	bool Purchase(_Sector, wxString, wxString, double, double, bool, wxString);
	bool NewDepositSchedule(double, int, wxDateTime);
	int GetNumItems(_PortfolioType);
	const wxVector<StockNode*> GetStockNodeItems();

	// call back for sector after one of its stocks completes a thread...
	void ThreadComplete(StockNode*);
private:
	bool m_Purchase(long, _Sector, wxString, wxString, double, double, bool, wxString);
	long GetLot();
	Sector* GetSector(_Sector);
	Sector* CreateSector(_Sector);
	wxVector<Sector> sectors;
	wxVector<long> lot;
	wxFrame* m_parent = nullptr;
	wxDateTime* m_parentclock = nullptr;
	CashAccount cash;
	int threads_running = 0;

};