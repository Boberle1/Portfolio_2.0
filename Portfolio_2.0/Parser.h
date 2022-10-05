#pragma once
#include "webdata.h"
#include "wx/txtstrm.h"
#include "wx/wfstream.h"
#include "wx/wfstream.h"


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
	Holidays() { this->Retrieve(); }
	wxVector<Holiday_Pair> holidays;
	void Retrieve();
	bool IsHoliday(wxDateTime&);
	Holiday_Pair* Find(int);
};

void GetWorkDate(wxDateTime& T, bool B = false);
void GetForwardWorkDay(wxDateTime& T);

enum DataType
{
	ANY = 1,
	LONGNAME,
	DAYGAIN
};

struct Dividend
{
	Dividend(wxDateTime ex, double d) : ex_Div(ex), div(d) {}
	Dividend(wxDateTime ex, double d, bool reinvest, double reinvest_shares) : ex_Div(ex), div(d), DivReinvest(reinvest), re_invest_shares(reinvest_shares) {}
	bool IsPendingReInvestment()
	{
		if (DivReinvest && !re_invest_shares)
			return true;

		return false;
	}
	void SetReinvestmentShares(double d)
	{
		this->re_invest_shares = d;
	}
	void SetDivReinvestOn()
	{
		this->DivReinvest = true;
	}
	double div = 0.0;
	wxDateTime ex_Div;
	bool DivReinvest = false;
	double re_invest_shares = 0.0;
};
struct SummaryData
{
	wxString Longname = "NotFound";
	wxString curr_price = "NotFound";
	wxString prev_close = "NotFound";
	wxString open = "NotFound";
	wxString beta = "NotFound";
	wxString earningsdate = "NotFound";

	// Date data was pulled...
	wxDateTime date;
	
	// Time it was pulled...
	wxDateTime time;
	double marketprice = 0.0;
	double previousclose = 0.0;
	double Open = 0.0;
	double Beta = 0.0;

	void ToDoubles()
	{
		if (!curr_price.ToDouble(&marketprice))
		{
			wxMessageBox("curr_price.ToDouble failed in SummaryData::ToDoubles! wxString is: " + curr_price + " Longname is: " + Longname);
			marketprice = 0.0;
		}
		if (!prev_close.ToDouble(&previousclose))
		{
			wxMessageBox("prev_close.ToDouble failed in SummaryData::ToDoubles! wxString is: " + prev_close + " Longname is: " + Longname);
			previousclose = 0.0;
		}
		if (!open.ToDouble(&Open))
		{
			wxMessageBox("open.ToDouble failed in SummaryData::ToDoubles! wxString is: " + open + " Longname is: " + Longname);
			Open = 0.0;
		}
		if (!beta.ToDouble(&Beta))
		{
			wxMessageBox("beta.ToDouble failed in SummaryData::ToDoubles! wxString is: " + beta + " Longname is: " + Longname);
			Beta = 0.0;
		}
	}
	/*
	void Save(wxTextOutputStream& out)
	{
		out << Longname << '\n';
		out << "CurrentPrice" << " " << curr_price << '\n';
		out << "PreviousClose" << " " << prev_close << '\n';
		out << "Open" << " " << open << '\n';
		out << "Beta" << " " << beta << '\n';
		out << earningsdate << '\n';
	}

	void Retrieve(wxTextInputStream& in)
	{
		wxString temp = "";
		Longname = in.ReadLine();
		in >> temp >> curr_price;
		in >> temp >> prev_close;
		in >> temp >> open;
		in >> temp >> beta;
		earningsdate = in.ReadLine();

		ToDoubles();
	}
	*/
};

class Parser
{
public:
	Parser(wxString&, bool);
	Parser(wxString&, void*, void (*foo)(void*, wxDateTime&, double&, double&, double&, double&), void(*foo2)(wxDateTime&, bool B), wxString);
	Parser(void* parent, wxString t, wxString pd, wxString ld, wxString ldd, void (*cb)(void* v, double o, double h, double l, double c, wxDateTime d), 
		void (*cbs)(void*, SummaryData), void (*cbd)(void*, Dividend), wxString enddate);
	void UpDateAll();
	void UpDateSummaryData();
	void UpDateHistoricalPrices();
	void UpDateDiv();
	void SetLastDateAndCallBack(wxString, void (*cb)(void* v, double o, double h, double l, double c, wxDateTime d));
	void SetTickerAndPurchaseDate(wxString, wxString);
	void SetSummaryDataCallBack(void (*cb)(void*, SummaryData));
	void SetLatestDividendDateAndCallBack(wxString, void (*cb)(void*, Dividend));
	SummaryData GetSummaryData();

	wxString RetrieveHistoricalURL();
private:
	bool ParsePrices(wxString& Data, size_t&);
	void ParseStockHistory(wxString&);
	void ParseDivHistory(wxString&);
	void GetLine(size_t& index, wxString& Data);
	void GetDivLine(size_t&, wxString&);
	void GetToEndline(size_t&, wxString&);
	void ParseTable(wxString&, void* sh, void (*foo)(void*, wxDateTime&, double&, double&, double&, double&), void(*foo2)(wxDateTime&, bool B), wxString);
	void Switch(int&, wxDateTime&, double&, double&, double&, double&, double&, long&, wxString&);

	int BeginParse(wxString&);

	void GetHistoricalURL(wxString&);
	void InsertDates(wxString& start, wxString& end);
	void UpDate(bool historical = true, bool div = true);
	bool PullWebData();
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
	//	wxString BCPR = "<span class=\"Trsdu(0.3s) Fw(b) Fz(36px) Mb(-4px) D(ib)";


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
	wxString href = "download=\"CWH.csv";

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

	wxDateTime Deincrement;
	bool ValidData = true;

	wxString Normal = "https://query1.finance.yahoo.com/v7/finance/download/TICKER?period1=BEGINDATE&period2=ENDDATE&interval=1d&events=history&includeAdjustedClose=true";
	wxString QURL = "https://finance.yahoo.com/quote/%5ETICKER?p=%5ETICKER";

	// HISTORICAL DATA URL
	wxString HDURL = "";
	wxString HDDURL = "";

	void* m_parent = nullptr;
};


