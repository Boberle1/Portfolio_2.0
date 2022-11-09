#pragma once
#include "CashAccount.h"
#include "Parser.h"
#include "wx/numformatter.h"
#include "wx/thread.h"
#include "wx/event.h"

constexpr auto STANDARD_DATE = "%m/%d/%G";
constexpr auto STANDARD_TIME = "%H:%M:%S";
constexpr auto DATE_KEY = "%G-%m-%d";

constexpr auto _BONDS = "Bonds";
constexpr auto _LARGECAP = "Large Cap";
constexpr auto _MIDCAP = "Mid Cap";
constexpr auto _SMALLCAP = "Small Cap";
constexpr auto _REIT = "REIT";
constexpr auto _FOREIGN = "Foreign";
constexpr auto _TECH = "Tech";
constexpr auto _CRYPTO = "CRYPTO";

wxDEFINE_EVENT(ThreadComplete, wxCommandEvent);


// Global function to check for valid date and is not a date in the future...
inline wxDateTime IsValidDate(wxString&, wxString);
unsigned int inline GetEndMonthDay(wxDateTime::Month, int);
wxDateTime::Month inline GetQuarterStartMonth(wxDateTime::Month);
wxDateTime::Month inline GetQuarterEndMonth(wxDateTime::Month);
wxString inline GetSectorName(_Sector);

_Sector inline GetStringToSector(wxString s)
{
	if (s == _BONDS)
		return _Sector::BOND;
	if (s == _LARGECAP)
		return _Sector::LARGECAP;
	if (s == _MIDCAP)
		return _Sector::MIDCAP;
	if (s == _SMALLCAP)
		return _Sector::SMALLCAP;
	if (s == _FOREIGN)
		return _Sector::FOREIGN;
	if (s == _TECH)
		return _Sector::TECH;
	if (s == _CRYPTO)
		return _Sector::CRYPTO;

	wxMessageBox("Error string passed to GetStringToSector does not match any sectors!");
	return _Sector::SECTOR_INVALID;
}

// Global function to check if a ptr is null...
template <typename T>
bool IsNull(T* t, wxString msg)
{
	if (!t)
	{
		wxFAIL_MSG(msg);
		return true;
	}

	return false;
}

// Simple class for information on an item like subsector, sector or stock...
class ItemInfo
{
public:
	ItemInfo(double&, double&);
	ItemInfo() {}
	ItemInfo(double&, double&, SectorPerformance&);
	void _SetAllData(double, double, SectorPerformance&);
	void _SetOverViewData(double, double);
	void _SetPerformance(SectorPerformance&);
	void _SetPerformance(SectorPerformance&, double&, double&, double&);
	void _SetTimeStamp(wxDateTime&);
	void _Write(DataStream&);
	void _Read(DataStream&);
	bool IsUpToDate();
	wxString GetDayReturnString() const { return wxNumberFormatter::ToString(this->day_return, 2) + '%'; } 
	wxString GetWeekReturnString() const { return wxNumberFormatter::ToString(this->week_return, 2) + '%'; }
	wxString GetMonthReturnString() const { return wxNumberFormatter::ToString(this->month_return, 2) + '%'; }
	wxString GetQuarterReturnString() const { return wxNumberFormatter::ToString(this->quarter_return, 2) + '%'; }
	wxString GetDayhalfyearString() const { return wxNumberFormatter::ToString(this->half_year_return, 2) + '%'; }
	wxString GetYTDReturnString() const { return wxNumberFormatter::ToString(this->YTD_return, 2) + '%'; }
	wxString GetYearReturnString() const { return wxNumberFormatter::ToString(this->year_return, 2) + '%'; }
	wxString GetMarketCapString() const { return wxNumberFormatter::ToString(this->market_cap, 5); }
private:
	void Populate(SectorData&);
	void AddDataToVec();
	SectorData* IsDuplicateDate();
	void Replace(SectorData*);
	void PushBack();
protected:
	double price = 0.0;
	double day_return = 0.0;
	double week_return = 0.0;
	double month_return = 0.0;
	double quarter_return = 0.0;
	double half_year_return = 0.0;
	double YTD_return = 0.0;
	double year_return = 0.0;
	double market_cap = 0.0;
	wxDateTime date;
	wxVector<SectorData> vec;
};

class SectorClass;
class ParentSector;
class SubSector;

// This is the child to ParentSector...
class SubSector : public ItemInfo
{
public:
	SubSector(ParentSector*);
	SubSector(ParentSector*, DataStream&);
	SubSector(ParentSector*, _Sub_Sector, wxString&, double&, double&);
	SubSector(ParentSector*, _Sub_Sector, wxString&, double&, double&, SectorPerformance&);
	void SetAllData(wxString&, double&, double&, SectorPerformance&);
	void SetOverViewData(wxString&, double&, double&);
	void SetPerformance(SectorPerformance&);
	void SetParent(ParentSector*);
	_Sub_Sector GetSubSecID();
	void Read(DataStream&);
	void Write(DataStream&);
	wxString GetSectorName() const { return this->sectorname; }
private:
	ParentSector* m_parent = nullptr;
	_Sub_Sector ID = _Sub_Sector::SUB_SECTOR_INVALID;
	wxString sectorname = "";
};

// This is the parent to SubSector...
class ParentSector : public ItemInfo
{
public:
	ParentSector(SectorClass*);
	ParentSector(SectorClass*, DataStream&);
	ParentSector(SectorClass*, _Sector, wxString, wxString);
	void SetAllData(wxString&, double&, double&, SectorPerformance&);
	bool SetSubSectorOverviewData(_Sub_Sector, wxString&, double&, double&);
	void SetSubSectorPerformance(SectorPerformance&);
	void SetSubSecData(wxString&, double&, double&, SectorPerformance&);
	_Sector GetSectorId();
	void Read(DataStream&);
	void Write(DataStream&);
	wxVector<SubSector>* GetSubSector() { return &this->subsecs; }
	wxString GetSectorName() { return this->sectorName; }
private:
	void m_GetSubSectorData();
	void m_GetSubSectorPerformance();
	wxString m_GetNameToInsert();
	wxString sectorName = "";
	wxString market_cap = "";
	_Sector ID = _Sector::SECTOR_INVALID;
	wxVector<SubSector> subsecs;
	wxString url = "";
	wxString subsecurl = "https://finviz.com/grp_export.ashx?g=industry&sg=INSERTION&v=TYPE&o=name";
	SectorClass* m_parent = nullptr;
};

// class defining all the sectors and subsectors...
class SectorClass
{
public:
	SectorClass();
	void ActivateData();
	wxString GetSectorString(_Sector);
	wxString GetSubSectorString(_Sector, _Sub_Sector);
	_Sector GetSectorID(wxString);
	_Sub_Sector GetSubSectorID(_Sector, wxString);
	const wxVector<_Sub_Sector>* GetSubSector(_Sector);
	const wxVector<wxString>* GetSubSectorString(_Sector);
	const wxVector<_Sector>* GetSector();
	const wxVector<wxString>* GetSectorString();
	ParentSector* GetSector(_Sector);
	wxVector<SubSector>* GetSubSectorVec(_Sector);
	bool Read();
	void Save();
private:
	wxString m_GetSubSector(wxVector<_Sub_Sector>&, wxVector<wxString>&, _Sub_Sector);
	_Sub_Sector m_GetSubSectorID(wxVector<wxString>&, wxVector<_Sub_Sector>&, wxString&);
	wxVector<_Sub_Sector>& m_GetSubSectorVec(_Sector);
	wxVector<wxString>& m_GetSubSectorStringVec(_Sector);
	wxString m_GetURL(_Sector);
	void m_SetData(SectorOverview&, SectorPerformance&);
	void m_FetchSectorData();
	bool IsUpToDate();
private:
	wxVector<_Sector> sec;
	wxVector<wxString> _sec;
	wxVector<_Sub_Sector> communication_services;
	wxVector<wxString> _communication_services;
	wxVector<_Sub_Sector> consumer_discretionary;
	wxVector<wxString> _consumer_discretionary;
	wxVector<_Sub_Sector> consumer_staples;
	wxVector<wxString> _consumer_staples;
	wxVector<_Sub_Sector> energy;
	wxVector<wxString> _energy;
	wxVector<_Sub_Sector> financials;
	wxVector<wxString> _financials;
	wxVector<_Sub_Sector> health_care;
	wxVector<wxString> _health_care;
	wxVector<_Sub_Sector> industrials;
	wxVector<wxString> _industrials;
	wxVector<_Sub_Sector> information_technology;
	wxVector<wxString> _information_technology;
	wxVector<_Sub_Sector> materials;
	wxVector<wxString> _materials;
	wxVector<_Sub_Sector> real_estate;
	wxVector<wxString> _real_estate;
	wxVector<_Sub_Sector> utilities;
	wxVector<wxString> _utilities;
	wxVector<_Sub_Sector> emptyint;
	wxVector<wxString> emptystring;

	wxVector<ParentSector> parentsector;
	wxVector<wxString> urls;

	// url for getting the data for the sectors, and in turn the sectors will get the data for all thier subsectors...
	wxString sectorurl = "https://finviz.com/grp_export.ashx?g=sector&v=INSERTION&o=name";
	
	// Helper items for quick access to items..
	ParentSector* sectorPtr = nullptr;
	SubSector* subsectorPtr = nullptr;
	wxVector<SubSector>* vecsubPtr = nullptr;
};

// global function for getting the static instance of SectorClass...
SectorClass& GetSectorClass();

class Return_node
{
public:
	Return_node(double* p, double* s, double* a) : PortfolioMarketValue(p), SectorMarketValue(s), StockMarketValue(a) {}
	Return_node(double ocb, double dcb, double wcb, double qcb, double ycb, double m, double s, double divshares) 
		: original_cost_basis(ocb), day_cost_basis(dcb), week_cost_basis(wcb), 
		quarter_cost_basis(qcb), year_cost_basis(ycb), market_value(m), shares(s), div_shares(divshares)
	{
		this->ReturnCalibarate();
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
		this->ReturnCalibarate();
	}

	void AddNewValues(const Return_node* rn)
	{
		this->original_cost_basis += rn->original_cost_basis;
		this->day_cost_basis += rn->day_cost_basis;
		this->week_cost_basis += rn->week_cost_basis;
		this->quarter_cost_basis += rn->quarter_cost_basis;
		this->year_cost_basis += rn->year_cost_basis;
		this->market_value += rn->market_value;
		this->shares += rn->shares;
		this->div_shares += rn->div_shares;
		this->dividends += rn->dividends;
	}

	void ReturnCalibarate()
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

		// Percent Relative To Parents...
		this->PortfolioPerc = this->PortfolioMarketValue && *this->PortfolioMarketValue ? this->market_value / *this->PortfolioMarketValue : 0.0;
		this->SectorPerc = this->SectorMarketValue && *this->SectorMarketValue ? this->market_value / *this->SectorMarketValue : 0.0;
		this->AssetPerc = this->StockMarketValue && *this->StockMarketValue ? this->market_value / *this->StockMarketValue : 0.0;
	}

	void Reset()
	{
		*this = Return_node(this->PortfolioMarketValue, this->SectorMarketValue, this->StockMarketValue);
	}

	void CalcRatios()
	{
		// Percent Relative To Parents...
		this->PortfolioPerc = this->PortfolioMarketValue && *this->PortfolioMarketValue ? this->market_value / *this->PortfolioMarketValue : 0.0;
		this->SectorPerc = this->SectorMarketValue && *this->SectorMarketValue ? this->market_value / *this->SectorMarketValue : 0.0;
		this->AssetPerc = this->StockMarketValue && *this->StockMarketValue ? this->market_value / *this->StockMarketValue : 0.0;
	}

	Return_node* GetReturnNode() { return this; }

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

	double AssetPerc = 0.0;
	double SectorPerc = 0.0;
	double PortfolioPerc = 0.0;

	double* PortfolioMarketValue = nullptr;
	double* SectorMarketValue = nullptr;
	double* StockMarketValue = nullptr;

	double _52weekDeviation = 0.0;
	double _90dayDeviation = 0.0;
	double _30dayDeviation = 0.0;
};

struct StockViewerData
{
	StockViewerData(){}
	StockViewerData(Return_node* r, wxString t, wxString e, wxString pd, double s, double pp, double p, double pc, double dg, double wg,
		double qg, double yg, double tg, double cb, double mv, double td, double cash = 0.0)
		: m_parent(r), ticker(t), earnings(e), purchase_date(pd), shares(s), purchase_price(pp), price(p), previous_close(pc), day_gain(dg),
		week_gain(wg), quarter_gain(qg), year_gain(yg), total_gain(tg), cost_basis(cb), market_value(mv), total_divs(td), m_cash(cash) {}
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
	wxString GetTotalGain(){ return wxNumberFormatter::ToString(this->total_gain * 100, 2); }
	wxString GetPortfolioPerc() { return wxNumberFormatter::ToString(this->m_parent->PortfolioPerc * 100, 2) + "%"; }
	wxString GetCostBasis(){ return wxNumberFormatter::ToString(this->cost_basis, 2); }
	wxString GetMarketValue(){ return wxNumberFormatter::ToString(this->market_value, 2); }
	wxString GetDividends(){ return wxNumberFormatter::ToString(this->total_divs, 2); }
	wxString GetTotalValue() { return wxNumberFormatter::ToString(this->market_value + this->m_cash, 2); }

	// Cash return methods...
	wxString GetDayReturn$() { return wxNumberFormatter::ToString(m_parent->GetDayReturn$(), 2); }
	wxString GetWeekReturn$() { return wxNumberFormatter::ToString(m_parent->GetWeekReturn$(), 2); }
	wxString GetQuarterReturn$() { return wxNumberFormatter::ToString(m_parent->GetQuarterReturn$(), 2); }
	wxString GetYearReturn$() { return wxNumberFormatter::ToString(m_parent->GetYearReturn$(), 2); }
	wxString GetTotalReturn$() { return wxNumberFormatter::ToString(m_parent->GetTotalReturn$(), 2); }
	wxString Get52Week_Deviation() { return wxNumberFormatter::ToString(m_parent->_52weekDeviation, 2); }
	wxString Get90Day_Deviation() { return wxNumberFormatter::ToString(m_parent->_90dayDeviation, 2); }
	wxString Get30Day_Deviation() { return wxNumberFormatter::ToString(m_parent->_30dayDeviation, 2); }

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
	double portfolio_perc = 0.0;
	double sector_perc = 0.0;
	double m_cash = 0.0;
	Return_node* m_parent = nullptr;
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
	long GetLotNumber() { return this->m_id; }
	wxString GetTicker();
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
	void GetDividendVec(wxVector<Dividend>&);
	bool IsPendingDivReInvest();
	bool SetReInvestShares(double&);
	void Save(DataStream&);
	void Retrieve(DataStream&);
private:
	void FillClientDividendVec(wxVector<Dividend>&);
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
	StockNode(wxString, wxDateTime*, Portfolio*, Sector*);
	StockNode(const StockNode&);
	~StockNode();
	bool Purchase(long id, wxString s, double price_per_share, double shares);
	bool InitiatePurchase(long id, wxDateTime, double price_per_share, double shares);
	bool Sell(long& id, wxDateTime&, double& price_per_share, double& shares);
	bool IdMatch(long);
	bool TickerMatch(wxString);
	wxString GetTicker();
	double GetShares();
	SummaryData GetSummaryData() { return this->current_Data; }
	wxString GetNextEarningsDate();
	bool IsActive();
	void Calibrate(bool force = false);
	void ClockChange();
	void SetHistoricalData(Day_Prices);
	void SetSummaryData(SummaryData);
	void SetDividends(Dividend);
	bool SetReInvestShares(double&);
	void OnThreadComplete(wxCommandEvent&);
	double GetDividends();
	wxVector<Dividend> GetDividendVec();
	StockViewerData* GetStockViewerData();
	bool PendingReInvestment();
	void Save();
	void Retrieve();

	// GetDividend Reinvestment shares...
	double GetDividendShares(bool latest = false);
	wxVector<stock_node*> GetChildren();

private:
	bool DistributeReInvestmentShares(double&);
	bool UpDate(bool force_update = false);
	bool Historical_prices_UpToDate();
	double GetDeviation(int);
	Day_Prices* GetHistoricalIndexIter();
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
private:
	wxVector<stock_node> bought;
	wxVector<Day_Prices> historical_prices;
	const wxString m_ticker = "";
	StockViewerData svd;
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
	bool Sell(long&, wxDateTime&, double&, double&);
	bool AddReInvestShares(double&);
	bool IsId(_Sector);
	bool IsChild(wxString&);
	bool IsChild(long&);
	int GetNumItems(_PortfolioType);
	wxVector<StockNode*> GetStockVector();
	wxVector<stock_node*> GetAllLotData();
	wxVector<stock_node*> GetLotData();
	Pair GetPair();
	void Calibrate(bool datechange = false);
	void CalcRatiosOfChildren();
	StockViewerData* GetStockViewerData();
	void Save();
	void Retrieve();
	void Update();
	_Sector GetID() { return this->id; }
	
	//call back from for StockNode after thread completion...
	void ThreadComplete(StockNode*);
private:
	StockNode* GetStockNode(wxString);
	StockNode* GetStockNode(long&);
	StockNode* CreateStockNode(long, wxString, bool, wxString);
//	wxString GetSectorName();
	int GetStockSize();
	wxVector<StockNode> stocks;
	wxDateTime* m_parentclock = nullptr;
	_Sector id;
	wxString Sector_Name = "";
	Portfolio* m_parent = nullptr;
	int threads_running = 0;
	StockNode* sn = NULL;
	StockViewerData svd;
};

class Portfolio : public Return_node
{
public:
	Portfolio(wxFrame*, double, wxDateTime*);
	bool Purchase(_Sector, wxString, wxString, double, double, bool, wxString);
	bool DoesTickerExist(wxString&);
	bool RequestSell(wxString&);
	bool Sell(long&, wxDateTime&, double&, double&);
	bool AddReInvestShares(double&);
	wxVector<stock_node*> GetLotData();
	bool NewDepositSchedule(double, int, wxDateTime);
	int GetNumItems(_PortfolioType);
	const wxVector<StockNode*> GetStockNodeItems();
	void Calibrate(bool datechange = false);
	StockViewerData* GetStockViewerData();
	void Save();
	void Retrieve();
	double GetFreeCash();
	wxVector<DayGainersandLosers> GetDayGainers();
	wxVector<DayGainersandLosers> GetDayLosers();
	SummaryData QuoteLookup(wxString);
	bool AddDeposit(wxDateTime&, double&);
	bool AddWithdrawl(wxDateTime&, double&);
	void DateChange();
	void Update();

	// call back for sector after one of its stocks completes a thread...
	void ThreadComplete(StockNode*);
private:
	bool m_Purchase(long, _Sector, wxString, wxString, double, double, bool, wxString);
	long GetLot();
	void DeleteLot(long&);
	Sector* GetSector(_Sector);
	Sector* GetSector(wxString&);
	Sector* GetSector(long&);
	Sector* CreateSector(_Sector);
	wxVector<Sector> sectors;
	wxVector<long> lot;
	wxVector<DayGainersandLosers> gainers;
	wxVector<DayGainersandLosers> losers;
	wxFrame* m_parent = nullptr;
	wxDateTime* m_parentclock = nullptr;
	Sector* sec = NULL;
	CashAccount cash;
	int threads_running = 0;
	StockViewerData svd;
	StockNode nasdaq;
	StockNode dow_jones;
	wxString url = "https://datahub.io/core/nyse-other-listings/r/0.html";
};