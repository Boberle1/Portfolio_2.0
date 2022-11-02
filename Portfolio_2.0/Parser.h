#pragma once
#include "webdata.h"
#include "DataStream.h"
//#include "wx/txtstrm.h"
//#include "wx/wfstream.h"
//#include "wx/wfstream.h"

constexpr int DayValue = 86400000;

//auto yahoofinancedate = [](wxLongLong Long)->wxLongLong { return Long / 1000; };

struct MonthNames
{
	wxString jan = "January";
	wxString feb = "February";
	wxString mar = "March";
	wxString apr = "April";
	wxString may = "May";
	wxString jun = "Jun";
	wxString jul = "July";
	wxString aug = "August";
	wxString sep = "September";
	wxString oct = "October";
	wxString nov = "November";
	wxString dec = "December";

	wxString _jan = "Jan";
	wxString _feb = "Feb";
	wxString _mar = "Mar";
	wxString _apr = "Apr";
	wxString _may = "May";
	wxString _jun = "Jun";
	wxString _jul = "Jul";
	wxString _aug = "Aug";
	wxString _sep = "Sep";
	wxString _oct = "Oct";
	wxString _nov = "Nov";
	wxString _dec = "Dec";

	wxDateTime::Month GetMonth(wxString);
};

struct month_day_pair
{
	month_day_pair(wxDateTime::Month m, int d) : month(m), day(d) {}
	wxDateTime::Month month = wxDateTime::Month::Inv_Month;
	int day = 0;
};

struct Holiday_Pair
{
	Holiday_Pair(int y) : year(y) {}
	void AddHoliday(month_day_pair mdp) { this->days.push_back(mdp); }
	wxVector<month_day_pair> days;
	int year = 0;
};

struct Holidays
{
	Holidays() {}
	wxVector<Holiday_Pair> holidays;
	void Retrieve();
	bool IsHoliday(wxDateTime&);
	Holiday_Pair* Find(int);
};

Holidays& GetHolidays();
void GetWorkDate(wxDateTime& T, bool B = false);
void GetForwardWorkDay(wxDateTime& T);

enum DataType
{
	ANY = 1,
	LONGNAME,
	DAYGAIN
};

class Parser
{
public:
	Parser(void* parent, wxString t, wxString pd, wxString ld, wxString ldd, void (*cb)(void* v, double o, double h, double l, double c, wxDateTime d), 
		void (*cbs)(void*, SummaryData), void (*cbd)(void*, Dividend), wxString enddate);
	Parser(wxString&);
	void UpDateAll();
	void UpDateSummaryData();
	wxString GetDescription(wxString);
	wxVector<DayGainersandLosers> GetStockGainers();
	wxVector<DayGainersandLosers> GetStockLosers();
	void PullFinVizOverview(wxString&, wxVector<SectorOverview>&);
	void PullFinVizPerformance(wxString&, wxVector<SectorPerformance>&);
	void UpDateHistoricalPrices();
	void UpDateDiv();
	void SetLastDateAndCallBack(wxString, void (*cb)(void* v, double o, double h, double l, double c, wxDateTime d));
	void SetSummaryDataCallBack(void (*cb)(void*, SummaryData));
	void SetLatestDividendDateAndCallBack(wxString, void (*cb)(void*, Dividend));
	SummaryData GetSummaryData();

private:
	
	bool ParsePrices(wxString& Data, size_t&);
	void ParseStockHistory(wxString&);
	void ParseDivHistory(wxString&);
	void GetLine(size_t& index, wxString& Data);
	void GetDivLine(size_t&, wxString&);
	wxString GetValue(size_t&, wxString&);
	void GetToEndline(size_t&, wxString&);

	int BeginParse(wxString&);

	void InsertDates(wxString& start, wxString& end);
	void UpDate(bool historical = true, bool div = true);
	bool PullWebData();
	bool PullWebData(wxString& url, wxString& data);
	void ParseSummaryData(wxString&);

	//CallBack for StockNode...
	void (*CallBack)(void* v, double o, double h, double l, double c, wxDateTime d) = nullptr;
	// CallBack for StockNode SummaryData...
	void (*CallBackSummary)(void*, SummaryData) = nullptr;
	// CallBack for StockNode Dividend...
	void (*CallBackDiv)(void*, Dividend) = nullptr;

//	template <typename Class, typename arg>
//	void (Class::*method)(SummaryData);
	// HISTORICAL URL EXTRACTION....
	wxString HUE(int, wxString&);

	wxString GetData(int&, wxString&, int);
	wxDateTime startDate;
	wxDateTime end_Date;
	wxDateTime PurchaseDate;
	wxDateTime LastDivDate;
	wxString lastdate = "";
	wxString purchaseDate = "";
	wxString lastDivDate = "";
	wxString m_enddate = "";
	wxString ticker = "";
	// TOKEN WORDS TO LOOK FOR...

	//Beginning Header...
	wxString BH = "<div id=\"quote-header-info\"";

	// HEADER ONE THAT INCLUDES LONGNAME...
	wxString H1 = "<h1 class";

	//	wxString BH = "data-test=\"quote-header";

		// BEGINNING OF LONGNAME AND TICKER ROW...
	wxString BOLAT = "<h1";

	// BEGIN CURRENT PRICE ROW(AND DAY GAIN)...
	wxString BCPR = "regularMarketPrice";
	wxString regularmarketprice = "regularMarketPrice";
	wxString regularmarketchange = "regularMarketChange";
	wxString tablestart = "<table";
	wxString href = "href=";
	wxString spanstart = "<span";
	wxString spanend = "</span>";
	wxString div = "<div";
	wxString tablebody = "<tbody>";
	wxString enddiv = "</div>";


		// PREVIOUS CLOSE ROW...
	wxString PCR = "PREV_CLOSE-value";

	// OPEN VALUE ROW...
	wxString OVR = "OPEN-value";

	// BETA ROW...
	wxString BR = "BETA_5Y-value";

	// EARNINGS VALUE...
	wxString EV = "EARNINGS_DATE-value";

	// BEGIN DATA...
	wxChar BD = '>';

	// END DATA...
	wxChar ED = '<';

	// GET TO APPLY BUTTON...
	wxString button = "Apply</span>";

	// DOWNLOAD HISTORICAL PRICES...
//	wxString href = "download=\"CWH.csv";

	wxString URL = "";

	SummaryData sumdata;
	//	wxString TableFind = "<table class=\"W(100%) M(0)\" data-test=\"historical-prices\">";
	wxString TableFind = "data-test=\"historical-prices\">";
	wxString RowStart = "<tr";
	wxString DataStart = "<td";

	// DATA EXTRACTION POINT...
	wxString DEP = "<span";

	// END DATA EXTRACTION POINT...
	wxString EDEP = "</span";
	wxString BodyStart = "<tbody>";
	wxString DataEnd = "</td>";
	wxString body_end = "</tbody>";
	wxString table_end = "</table>";
	wxString row_end = "</tr>";
	wxString start_table_head = "<thead>";
	wxString end_table_head = "</thead>";
	wxString Linkbutton = "<span data-reactid=\"28\">Components</span>";
	wxString paragragh_start = "<p";
	wxString paragragh_end = "</p";
	wxString Description = "<span>Description</span>";

	wxDateTime Deincrement;
	bool ValidData = true;

	wxString Normal = "https://query1.finance.yahoo.com/v7/finance/download/TICKER?period1=BEGINDATE&period2=ENDDATE&interval=1d&events=history&includeAdjustedClose=true";
	wxString QURL = "https://finance.yahoo.com/quote/%5ETICKER?p=%5ETICKER";
	wxString nasdaq_list = "https://datahub.io/core/nasdaq-listings/r/nasdaq-listed.csv";
	wxString sp500_list = "https://datahub.io/core/s-and-p-500-companies/r/constituents.csv";

//	wxVector<SectorPerformance> secper;

	// US inflation data...
	wxString inflation_URL = "https://datahub.io/core/cpi-us/r/cpiai.csv";

	// HISTORICAL DATA URL
	wxString HDURL = "";
	wxString HDDURL = "";

	wxString ulllr = "https://finviz.com/elite.ashx";

	void* m_parent = nullptr;
};


