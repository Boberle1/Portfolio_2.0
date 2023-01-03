#include "Parser.h"

auto yahoofinancedate = [](wxLongLong Long)->wxLongLong { return Long / 1000; };

wxString RemoveCommas(wxString S)
{
	int index = S.find(',');
	while (index != -1)
	{
		S = S.Remove(index, 1);
		index = S.find(',');
	}

	if (index != -1)
		S = S.Remove(index, 1);

	return S;
}

/*
wxDateTime::Month MonthNames::GetMonth(wxString mon)
{
	if (mon == this->jan || mon == this->_jan)
		return wxDateTime::Month::Jan;
	if (mon == this->feb || mon == this->_feb)
		return wxDateTime::Month::Feb;
	if (mon == this->mar || mon == this->_mar)
		return wxDateTime::Month::Mar;
	if (mon == this->apr || mon == this->_apr)
		return wxDateTime::Month::Apr;
	if (mon == this->may || mon == this->_may)
		return wxDateTime::Month::May;
	if (mon == this->jun || mon == this->_jun)
		return wxDateTime::Month::Jun;
	if (mon == this->jul || mon == this->_jul)
		return wxDateTime::Month::Jul;
	if (mon == this->aug || mon == this->_aug)
		return wxDateTime::Month::Aug;
	if (mon == this->sep || mon == this->_sep)
		return wxDateTime::Month::Sep;
	if (mon == this->oct || mon == this->_oct)
		return wxDateTime::Month::Oct;
	if (mon == this->nov || mon == this->_nov)
		return wxDateTime::Month::Nov;
	if (mon == this->dec || mon == this->_dec)
		return wxDateTime::Month::Dec;

	wxFAIL_MSG("wxString: " + mon + " passed to MonthNames::GetMonth did not match any months. Returning invalid month!");
	return wxDateTime::Month::Inv_Month;
}

 HOLIDAYS STRUCT...
void Holidays::Retrieve()
{
	wxString Filename = "SavedFiles/Holidays.txt";
	wxFile file;
	if (!file.Exists(Filename))
	{
		wxMessageBox(Filename + " in Holidays::Retrieve does not exist!");
		return;
	}

	if (!file.Open(Filename, wxFile::read))
	{
		wxMessageBox(Filename + " in Holidays::Retrieve could not be opened!");
		return;
	}

	wxFileInputStream i(file);
	wxTextInputStream in(i);

	wxString year = "Year";
	wxString value;
	wxString mon;
	int day;
	int Year;
	MonthNames mn;
	in >> value;
	while (!file.Eof())
	{
		in >> Year;
		in >> mon;
		Holiday_Pair hp(Year);
		while (mon != year && !file.Eof())
		{
			in >> day;
			hp.AddHoliday(month_day_pair(mn.GetMonth(mon), day));
			in >> mon;
		}
		holidays.push_back(hp);
	}
}

bool Holidays::IsHoliday(wxDateTime& T)
{
	if (!T.IsValid())
	{
		wxMessageBox("Date passed to Holidays::IsHoliday is not a valid date.");
		return false;
	}

	Holiday_Pair* it = this->Find(T.GetYear());
	if (!it)
	{
		wxMessageBox("Could not find year in Holidays::IsHoliday!");
		return false;
	}

	for (auto v : it->days)
	{
		if (v.month == T.GetMonth() && v.day == T.GetDay())
			return true;
	}

	return false;
}

Holiday_Pair* Holidays::Find(int y)
{
	for (size_t i = 0; i < this->holidays.size(); ++i)
	{
		if (this->holidays[i].year == y)
			return &this->holidays[i];
	}

	return nullptr;
}

// global Holidays instance
static Holidays holidays;

Holidays& GetHolidays()
{
	return holidays;
}

void GetWorkDate(wxDateTime& T, bool B)
{
	if (!T.IsValid())
		wxMessageBox("wxDateTime Param in GetWorkDay() is Invalid Date!");

	if (B)
	{
		do {
			T = wxDateTime(T.GetValue() - DayValue);
		} while (!T.IsWorkDay());
		return;
	}

	if (T.IsWorkDay() && !holidays.IsHoliday(T))
		return;

	T = wxDateTime(T.GetValue() - DayValue);
	GetWorkDate(T, B);
	return;
}

void GetForwardWorkDay(wxDateTime& T)
{
	if (!T.IsValid())
		wxMessageBox("wxDateTime Param in GetForwardWorkDay() is Invalid Date!");

	if (T.IsWorkDay() && !holidays.IsHoliday(T))
		return;

	T = wxDateTime(T.GetValue() + DayValue);
	GetForwardWorkDay(T);
	return;
}

bool IsMarketOpen()
{
	wxDateTime today(wxDateTime::Now());
	if (today.IsWorkDay() && !holidays.IsHoliday(today))
	{
		if (_MarketOpen <= today && _MarketClose > today)
			return true;
	}

	return false;
}
*/
/*
Parser::Parser(wxString& data, bool historical)
{
	BeginParse(data);
}

Parser::Parser(wxString& Data, void* sh, void (*foo)(void* sh, wxDateTime&, double&, double&, double&, double&), void(*foo2)(wxDateTime&, bool B), wxString firstdate)
{
	BeginParse(Data);
	ParseTable(Data, sh, foo, foo2, firstdate);
}
*/

Parser::Parser(void* parent, wxString t, wxString pd, wxString ld, wxString ldd, void (*cb)(void* v, double o, double h, double l, double c, wxDateTime d, _PortfolioType _type), 
	void (*cbs)(void* v, SummaryData, _PortfolioType), void (*cbd)(void* v, Dividend), wxString enddate) : m_parent(parent), ticker(t), purchaseDate(pd), lastdate(ld), lastDivDate(ldd),
	CallBack(cb), CallBackSummary(cbs), CallBackDiv(cbd), m_enddate(enddate)
{
	this->PurchaseDate.ParseDate(this->purchaseDate);
	this->startDate.ParseDate(this->lastdate);
	this->LastDivDate.ParseDate(this->lastDivDate);
	this->end_Date.ParseDate(this->m_enddate);
}

Parser::Parser(void* parent, wxString t, wxString pd, wxString ld, void(*cb)(void* v, double o, double h, double l, double c, wxDateTime d, _PortfolioType _type)
	, _PortfolioType _type, void (*cbs)(void*, SummaryData, _PortfolioType))
	: m_parent(parent), ticker(t), purchaseDate(pd), lastdate(ld), CallBack(cb), type(_type), CallBackSummary(cbs)
{
	this->PurchaseDate.ParseDate(this->purchaseDate);
	this->startDate.ParseDate(this->lastdate);
}

Parser::Parser(void* parent, _Sector S) : m_parent(parent), sector_type(S)
{
	wxString url = this->idustrie_stocks_url;
	wxString key = "";
	switch (this->sector_type)
	{
	case _Sector::COMMUNICATION_SERVICES: key = ms_cOMMUNICATION_SERVICES; break;
	case _Sector::CONSUMER_DESCRETIONARY: key = ms_cONSUMER_DESCRETIONARY; break;
	case _Sector::CONSUMER_STAPLES: key = ms_cONSUMER_STAPLES; break;
	case _Sector::ENERGY: key = ms_eNERGY; break;
	case _Sector::FINANCIALS: key = ms_fINANCIAL; break;
	case _Sector::HEALTH_CARE: key = ms_hEALTH_CARE; break;
	case _Sector::INDUSTRIALS: key = ms_iNDUSTRIALS; break;
	case _Sector::INFORMATION_TECHNOLOGY: key = ms_tECHNOLOGY; break;
	case _Sector::MATERIALS: key = ms_bASIC_MATERIALS; break;
	case _Sector::REAL_ESTATE: key = ms_rEAL_ESTATE; break;
	case _Sector::UTILITIES: key = ms_uTILITIES; break;
	default: wxFAIL_MSG("_Sector passed to Parser constructor did not match anything in the switch statement!"); return;
	}

	wxString data = "";
	url.Replace("TICKER", key);
	this->PullWebData(url, data);
}

Parser::Parser(wxString& data)
{
	if (!data.size())
		return;

	this->ticker = data;
	this->UpDateSummaryData();
}

SummaryData Parser::PullIndexQuote(int selection)
{
	wxString url = "";
	switch (selection)
	{
	case DOW_JONES: url = this->DOW_QUOTE_URL; break;
	case NASDAQ: url = this->QQQ_QUOTE_URL; break;
	case SP_500: url = this->SP500_QUOTE_URL; break;
	}

	wxString data = "";
	this->PullWebData(url, data);

	this->ParseSummaryData(data);

	return this->GetSummaryData();
}

void Parser::UpDateAll()
{
	this->UpDateSummaryData();
	this->UpDate(true, true);
}

void Parser::Test()
{
	wxString data = "";
	wxString link = "https://query1.finance.yahoo.com/v7/finance/download/%5EDJI?period1=1639177638&period2=1670713638&interval=1d&events=history&includeAdjustedClose=true";
	this->PullWebData(link, data);
}

void Parser::UpDateSummaryData(bool indices)
{
	this->PullWebData(indices);
}

void Parser::GetSectorNameForStock(wxString& ticker, wxString& sector, wxString& industry)
{
	wxString url = this->url_Sectorname;
	url.Replace("TICKER", ticker);

	wxString data = "";
	this->PullWebData(url, data);
	wxString temp = "";

	wxString sectorspan = "Sector(s)</span>";
	wxString start = "<span";
	wxString _industry = "Industry";
	int index = this->FindItem(sectorspan, data);
	if (index != -1)
	{
		index = this->FindItem(start, data, ++index);
		if (index == -1)
			return;
		++index;
		sector = this->GetData(index, data, 1);
		index = this->FindItem(_industry, data, ++index);
		if (index == -1)
			return;
		++index;
		index = this->FindItem(start, data, index);
		if (index == -1)
			return;
		++index;
		industry = this->GetData(index, data, 1);
	}
}

wxString Parser::GetDescription(wxString t)
{
	wxString url = "https://finance.yahoo.com/quote/TICKER/profile?p=TICKER";
	wxString data = "";
	url.Replace("TICKER", t);
	webdata web;
	web.seturl(url);
	web.setpages(1);
	web.getwebdata(data);

	int index = data.find(this->Description);
	if (index == -1)
		return "";
	index = data.find(this->paragragh_start, index);
	if (index == -1)
		return "";
	
	return "\t" + this->GetData(index, data, 1);
}

wxVector<DayGainersandLosers> Parser::GetStockGainers()
{
	wxString url = "https://finance.yahoo.com/gainers";
	/*
	find <table
	then 
	find <tr
	then 
	find href=
	then getdata

	find <td
	then
	getdata

	find <td
	then 
	find regularMarketPrice
	then
	getdata

	find <td
	then
	find regularMarketChange
	then
	getdata
	*/
	wxVector<DayGainersandLosers> vec;
	DayGainersandLosers day;

	int index = 0;
	wxString data = "";
	if (!this->PullWebData(url, data))
		return vec;

	index = data.find(this->tablestart);
	if (index == -1)
		return vec;

	for (size_t i = 0; i < 20; ++i)
	{
		index = data.find(this->RowStart, index);
		if (index == -1)
			return vec;

		index = data.find(this->href, index);
		if (index == -1)
			return vec;

		day.ticker = this->GetData(index, data, 1);

		index = data.find(this->DataStart, index);
		if (index == -1)
			return vec;

		day.longname = this->GetData(index, data, 1);

		index = data.find(this->DataStart, index);
		if (index == -1)
			return vec;

		index = data.find(this->regularmarketprice, index);
		if (index == -1)
			return vec;

		day.marketprice = this->GetData(index, data, 1);

		index = data.find(this->DataStart, index);
		if (index == -1)
			return vec;

		index = data.find(this->spanstart, index);
		if (index == -1)
			return vec;

		day.change = this->GetData(index, data, 1);

		day.ToDoubles();
		vec.push_back(day);
	}

	return vec;
}

wxVector<DayGainersandLosers> Parser::GetStockLosers()
{
	wxString url = "https://finance.yahoo.com/losers";
	wxVector<DayGainersandLosers> vec;
	DayGainersandLosers day;

	int index = 0;
	wxString data = "";
	if (!this->PullWebData(url, data))
		return vec;

	index = data.find(this->tablestart);
	if (index == -1)
		return vec;

	for (size_t i = 0; i < 20; ++i)
	{
		index = data.find(this->RowStart, index);
		if (index == -1)
			return vec;

		index = data.find(this->href, index);
		if (index == -1)
			return vec;

		day.ticker = this->GetData(index, data, 1);

		index = data.find(this->DataStart, index);
		if (index == -1)
			return vec;

		day.longname = this->GetData(index, data, 1);

		index = data.find(this->DataStart, index);
		if (index == -1)
			return vec;

		index = data.find(this->regularmarketprice, index);
		if (index == -1)
			return vec;

		day.marketprice = this->GetData(index, data, 1);

		index = data.find(this->DataStart, index);
		if (index == -1)
			return vec;

		index = data.find(this->spanstart, index);
		if (index == -1)
			return vec;

		day.change = this->GetData(index, data, 1);

		day.ToDoubles();
		vec.push_back(day);
	}

	return vec;
}

void Parser::PullFinVizOverview(wxString& url, wxVector<SectorOverview>& v)
{
	wxString data = "";
	
	if (!this->PullWebData(url, data))
		return;

	size_t index = 0;
	wxString d = "0.0";
	// throw the header row away...
	this->GetToEndline(index, data);
	++index;

	while (index < data.size())
	{
		SectorOverview so;

		// throw away first value;
		this->GetValue(index, data);
		++index;
		so.name = this->GetValue(index, data);
		if (so.name == "\"Furnishings" || so.name == "\"Health Care")
		{
			so.name += ',';
			so.name += this->GetValue(++index, data);
		}
		++index;
		so._numOfstocks = this->GetValue(index, data);
		++index;
		so._market_cap = this->GetValue(index, data);
		++index;
		so._dividend = this->GetValue(index, data);
		if (!so._dividend.size())
			so._dividend = "0.0";
		++index;
		so._PtoE = this->GetValue(index, data);
		++index;
		so._forward_PtoE = this->GetValue(index, data);
		++index;
		so._PEG = this->GetValue(index, data);
		++index;
		so._float_short = this->GetValue(index, data);
		++index;
		so._change = this->GetValue(index, data);
		++index;
		so._volume = this->GetValue(index, data);
		++index;
		if (index > data.size())
			return;
		so.ToDouble();
		v.push_back(so);
	}
}

void Parser::PullFinVizPerformance(wxString& url, wxVector<SectorPerformance>& v)
{
	wxString data = "";

	if (!this->PullWebData(url, data))
		return;

	size_t index = 0;
	this->GetToEndline(index, data);
	++index;

	while (true)
	{
		SectorPerformance sp;

		// throw away first value;
		this->GetValue(index, data);
		++index;
		sp.name = this->GetValue(index, data);
		if (sp.name == "\"Furnishings" || sp.name == "\"Health Care")
		{
			sp.name += ',';
			sp.name += this->GetValue(++index, data);
		}
		++index;
		sp._week = this->GetValue(index, data);
		++index;
		sp._month = this->GetValue(index, data);
		++index;
		sp._quarter = this->GetValue(index, data);
		++index;
		sp._half = this->GetValue(index, data);
		++index;
		sp._year = this->GetValue(index, data);
		++index;
		sp._yearToDate = this->GetValue(index, data);
		++index;

		// throw the rest away, we already have it...
		this->GetToEndline(index, data);
		++index;
		if (index > data.size())
			return;
		sp.ToDouble();
		v.push_back(sp);
	}
}

void Parser::PullSectorStocks(_Sector s, wxVector<SectorStock>& v)
{
	wxString sec = "";
	wxString sectorname = "";
	switch (s)
	{
	case COMMUNICATION_SERVICES: sec = ms_cOMMUNICATION_SERVICES; sectorname = _COMMUNICATION_SERVICES; break;
	case CONSUMER_DESCRETIONARY: sec = ms_cONSUMER_DESCRETIONARY; sectorname = _CONSUMER_DESCRETIONARY; break;
	case CONSUMER_STAPLES:		 sec = ms_cONSUMER_STAPLES; sectorname = _CONSUMER_STAPLES; break;
	case ENERGY:				 sec = ms_eNERGY; sectorname = _ENERGY; break;
	case FINANCIALS:			 sec = ms_fINANCIAL; sectorname = _FINANCIALS; break;
	case HEALTH_CARE:			 sec = ms_hEALTH_CARE; sectorname = _HEALTH_CARE; break;
	case INDUSTRIALS:			 sec = ms_iNDUSTRIALS; sectorname = _INDUSTRIALS; break;
	case INFORMATION_TECHNOLOGY: sec = ms_tECHNOLOGY; sectorname = _INFORMATION_TECHNOLOGY; break;
	case MATERIALS:				 sec = ms_bASIC_MATERIALS; sectorname = _MATERIALS; break;
	case REAL_ESTATE:			 sec = ms_rEAL_ESTATE; sectorname = _REAL_ESTATE; break;
	case UTILITIES:				 sec = ms_uTILITIES; sectorname = _UTILITIES; break;
	default:
		wxFAIL_MSG("switch failed in Parser::PullSectorStocks! Returning!"); return;
	}
	
	int start = 0;
	int rows = 0;
	int total = 0;
	long lstart = 0;
	long lrows = 0;
	long ltotal = 0;
	int first = 0;
	wxString _start = "";
	wxString _rows = "";
	wxString _total = "";
	wxString result = "";

	wxString offset_count = "0";
	int N_offset_count = 0;
	wxString count = "100";

	wxString url = this->idustrie_stocks_url;
	url.Replace("TICKER", sec);
	url.Replace("OFFSETCOUNT", "0");
	url.Replace("COUNT", "100");
	wxString data = "";
	this->PullWebData(url, data);

	wxString match = "<span>Matching <span>Stocks";
	int index = this->FindItem(match, data);
	if (index == -1)
		return;

	index += match.size();
	index = this->FindItem("<span>", data, index);
	if (index == -1)
		return;

	result = this->GetData(index, data, 1);

	for (size_t i = 0; i < result.size(); ++i)
	{
		if (isdigit(result[i]))
		{
			switch (first)
			{
			case 0: _start += result[i]; break;
			case 1: _rows  += result[i]; break;
			case 2: _total += result[i]; break;
			}
		}
		else
		{
			if (_start.size() && !first)
				first++;
			else if (_rows.size() && first == 1)
				first++;
		}
	}

	_start.ToLong(&lstart);
	_rows.ToLong(&lrows);
	_total.ToLong(&ltotal);

	start = lstart;
	rows = lrows;
	total = ltotal;

	do
	{
		if (N_offset_count)
		{
			wxString match = "<span>Matching <span>Stocks";
			index = this->FindItem(match, data);
			if (index == -1)
				return;

			index += match.size();
			index = this->FindItem("<span>", data, index);
			if (index == -1)
				return;

			result = this->GetData(index, data, 1);
		}

		this->PullSectorStocks(index, 1, rows, total, v, data, s, sectorname);
		data = "";
		url = this->industrie_stocks_url_2;
		start += 100;

		N_offset_count += 100;
		if (N_offset_count > total)
			break;
		offset_count = "";
		offset_count << N_offset_count;

		rows = start + 100 < total ? 100 : total - start + 1;
		url.Replace("TICKER", sec);
		url.Replace("OFFSETCOUNT", offset_count);
		url.Replace("COUNT", "100");
		this->PullWebData(url, data);
	} while (N_offset_count < total);
}

void Parser::UpDateHistoricalPrices(bool indices)
{
	this->UpDate(true, false, indices);
}

void Parser::UpDateDiv()
{
	this->UpDate(false, true);
}

void Parser::SetLastDateAndCallBack(wxString date, void (*cb)(void* v, double o, double h, double l, double c, wxDateTime d, _PortfolioType))
{
	this->CallBack = cb;
	this->lastdate = date;
}

void Parser::SetSummaryDataCallBack(void (*cb)(void* v, SummaryData, _PortfolioType))
{
	this->CallBackSummary = cb;
}

void Parser::SetLatestDividendDateAndCallBack(wxString div, void (*cb)(void* v, Dividend))
{
	this->lastDivDate = div;
	this->CallBackDiv = cb;
}

bool Parser::PullSectorStocks(int index, int start, int rows, int end, wxVector<SectorStock>& vec, wxString& data, _Sector S, wxString& sectorname)
{
	int begin = start;
	wxString ticker = "", longname = "", price = "", 
		change = "", change_perc = "", volume = "", avg_volume = "", market_cap = "";

	int endofrow = 0;
	int inttemp = index;
	wxString match = "";
	for (begin; begin <= rows; ++begin)
	{
		inttemp = index;
		index = this->FindItem("data-id=\"portfolio-checkbox", data, index);
		if (index == -1)
		{
			//wxFAIL_MSG("Could not find data-id=\"portfolio-checkbox in Parser::PullSectorStocks!");
			return false;
		}

		inttemp = index;
		index = this->FindItem("title=\"", data, index);
		if (index == -1)
		{
			wxFAIL_MSG("Could not find title= in Parser::PullSectorStocks!");
			return false;
		}

		ticker = this->GetData(index, data, 1);
		endofrow = this->FindItem("</tr>", data, index);
		inttemp = index;
		match = "\"Name\"";
		index = this->FindItem(match, data, index);
		if (index == -1)
		{
			wxString stringtemp = data.Mid(inttemp, 3000);
			wxFAIL_MSG("Could not find name in Parser::PullSectorStocks!");
			stringtemp = "";
			return false;
		}

		index += match.size();
		longname = this->GetData(index, data, 1);

		inttemp = index;
		wxString stringtemp1 = data.Mid(inttemp, 3000);
		match = "data-field=\"regularMarketPrice\"";
		index = this->FindItem(match, data, index);
		if (index == -1)
		{
			wxFAIL_MSG("Could not find regularMarketPrice in Parser::PullSectorStocks!");
			stringtemp1 = "";
			return false;
		}

		index += match.size();
		price = this->GetData(index, data, 1);

		inttemp = index;
		match = "data-field=\"regularMarketChange\"";
//		wxString stringtemp2 = data.Mid(inttemp, 3000);
		index = this->FindItem(match, data, index);
		if (index == -1)
		{
			wxFAIL_MSG("Could not find regularMarketChange in Parser::PullSectorStocks!");
//			stringtemp2 = "";
			return false;
		}

		inttemp = index;
		match = "class=\"C";
//		wxString stringtemp3 = data.Mid(inttemp, 3000);
		index = this->FindItem(match, data, index);

		// check to see if it skipped a row...
		if (index > endofrow)
		{
			match = "class=\"\"";
			index = this->FindItem(match, data, inttemp);
		}

		if (index == -1)
		{
			wxFAIL_MSG("Could not find class=C in Parser::PullSectorStocks!");
//			stringtemp3 = "";
			return false;
		}

		index += match.size();
		change = this->GetData(index, data, 1);

		inttemp = index;
		match = "data-field=\"regularMarketChangePercent\"";
//		wxString stringtemp4 = data.Mid(inttemp, 3000);
		index = this->FindItem(match, data, index);
		if (index == -1)
		{
			wxFAIL_MSG("Could not find regularMarketChangePercent in Parser::PullSectorStocks!");
//			stringtemp4 = "";
			return false;
		}

		inttemp = index;
		match = "class=\"C";
//		wxString stringtemp5 = data.Mid(inttemp, 3000);
		index = this->FindItem(match, data, index);

		// check to see if it skipped a row...
		if (index > endofrow)
		{
			match = "class=\"\"";
			index = this->FindItem(match, data, inttemp);
		}

		if (index == -1)
		{
			wxFAIL_MSG("Could not find class=C in Parser::PullSectorStocks!");
//			stringtemp5 = "";
			return false;
		}

		index += match.size();
		change_perc = this->GetData(index, data, 1);

		inttemp = index;
//		wxString stringtemp6 = data.Mid(inttemp, 3000);
		match = "regularMarketVolume";
		index = this->FindItem("regularMarketVolume", data, index);
		if (index == -1)
		{
			wxFAIL_MSG("Could not find regularMarketVolume in Parser::PullSectorStocks!");
//			stringtemp6 = "";
			return false;
		}

		index += match.size();
		volume = this->GetData(index, data, 1);

		inttemp = index;
		match = "Avg Vol";
//		wxString stringtemp7 = data.Mid(inttemp, 3000);
		index = this->FindItem(match, data, index);
		if (index == -1)
		{
			wxString stringtemp = data.Mid(inttemp, 3000);
			wxFAIL_MSG("Could not find Avg Vol in Parser::PullSectorStocks!");
//			stringtemp = "";
			return false;
		}

		index += match.size();
		avg_volume = this->GetData(index, data, 1);

		inttemp = index;
		int temp = index;
		match = "marketCap";
		wxString secondmatch = "aria-label=\"Market Cap\"><span";
//		wxString stringtemp8 = data.Mid(inttemp, 3000);
		index = this->FindItem(match, data, index);
		if (index == -1)
		{
			wxString stringtemp = data.Mid(inttemp, 3000);
			wxFAIL_MSG("Could not find marketCap in Parser::PullSectorStocks!");
			stringtemp = "";
			return false;
		}
		else if(index > endofrow)
		{
			index = this->FindItem(secondmatch, data, temp);
			market_cap = "N/A";
			if (index == -1)
				index = temp;
		}

		if (market_cap != "N/A")
		{
			index += match.size();
			market_cap = this->GetData(index, data, 1);
		}

		vec.push_back(SectorStock(S, sectorname, ticker, longname, price, change, change_perc, volume, avg_volume, market_cap));
		market_cap = "";
	}
	return true;
}

bool Parser::ParsePrices(wxString& Data, size_t& index)
{
	wxString date = this->lastdate.size() ? this->lastdate : this->purchaseDate;
	int find = Data.find(date);
	if (find == -1)
		return false;

	index = find;
//	GetToEndline(index, Data);
//	++index;

	if (index > Data.size())
		return true;

	for (index; index < Data.size(); ++index)
		GetLine(index, Data);

	return true;
}

void Parser::ParseStockHistory(wxString& Data)
{
	wxString key = "Date,Open,High,Low,Close,Adj Close,Volume\n";
	int size = key.size();
	int i = Data.find(key);

	if (i == -1)
	{
		wxMessageBox("Could not find index of date open close ect.");
		return;
	}

	size_t index = i + size;
	if (ParsePrices(Data, index))
		return;

	for (index; index < Data.size(); ++index)
		GetLine(index, Data);
}

void Parser::GetLine(size_t& index, wxString& Data)
{
	if (!(Data.size() > index))
		return;

	wxDateTime Date;
	double Open = 0.0;
	double High = 0.0;
	double Low = 0.0;
	double Close = 0.0;
	double Adjclose = 0.0;
	long Volume = 0;

	int i = index;
	wxString date = "";
	wxString open = "";
	wxString high = "";
	wxString low = "";
	wxString close = "";
	wxString adjclose = "";
	wxString volume = "";
	int commas = 0;

	// NUMBER OF COMMAS THAT SHOULD BE FOUND...
	int noc = 6;

	char period = '.';
	char dash = '-';
	char comma = ',';
	char endline = '\n';
	char tab = '\r';
	char c = ' ';
	wxString data = "";

	while (Data.size() > index && Data[index] != endline)
	{
		if (Data[index] == comma)
			++commas;

		++index;
	}

	if (commas != noc)
		return;

	commas = 0;

	index = i;
	while (Data.size() > index && Data[index] != endline)
	{
		c = Data[index];
		if (c == comma)
		{
			++commas;
			if (commas == 1)
			{
				if (!Date.ParseDate(data))
				{
					wxMessageBox("Date not parsed in Stock_History::GetLine! data is: " + data + " and ticker is: " + ticker);
					GetToEndline(index, Data);
					return;
				}
				else if (Date < this->startDate)
				{
					this->GetToEndline(index, Data);
					return;
				}
				data = "";
				++index;
				continue;
			}
			else
			{
				switch (commas)
				{
				case 2:
				{
					if (!data.ToDouble(&Open))
					{
						GetToEndline(index, Data);
						return;
					}
					data = "";
					++index;
					continue;
				}
				case 3:
				{
					if (!data.ToDouble(&High))
					{
						GetToEndline(index, Data);
						return;
					}
					data = "";
					++index;
					continue;
				}
				case 4:
				{
					if (!data.ToDouble(&Low))
					{
						GetToEndline(index, Data);
						return;
					}
					data = "";
					++index;
					continue;
				}
				case 5:
				{
					if (!data.ToDouble(&Close))
					{
						GetToEndline(index, Data);
						return;
					}
					data = "";
					++index;
					continue;
				}
				case 6:
				{
					if (!data.ToDouble(&Adjclose))
					{
						GetToEndline(index, Data);
						return;
					}
					data = "";
					++index;
					continue;
				}
				}
				++index;
				continue;
			}
		}

		if (c != period && c != comma && c != dash && c != tab)
		{
			if (isdigit(c))
				data += Data[index];
			else
			{
				GetToEndline(index, Data);
				return;
			}
		}
		else if (c == tab)
		{
			++index;
			continue;
		}
		else if (c != comma && data.size() && isdigit(data[data.size() - 1]))
			data += Data[index];
		else
		{
			GetToEndline(index, Data);
			return;
		}
		++index;
	}

//	if (!data.ToLongLong(&Volume))
//		return;

	this->CallBack(this->m_parent, Open, High, Low, Close, Date, this->type);
}

void Parser::GetToEndline(size_t& index, wxString& Data)
{
	while (Data.size() > index && Data[index] != '\n')
		++index;
}

void Parser::ParseDivHistory(wxString& Data)
{
	wxString key = "Date,Dividend";
	int i = Data.find(key);
	if (i == -1)
	{
		wxMessageBox("Could not find Date,Dividend in Stock_History::ParseDivHistory()!");
		return;
	}
	i += key.size();

	int temp = Data.find(this->lastDivDate);
	if (temp != -1 && !(temp == 0 && lastDivDate.IsEmpty()))
		i = temp;

	for (size_t index = i + key.size(); index < Data.size(); ++index)
		GetDivLine(index, Data);
}

void Parser::GetDivLine(size_t& index, wxString& Data)
{
	int i = index;
	char period = '.';
	char comma = ',';
	char dash = '-';
	char endline = '\n';
	wxString data = "";
	double div = 0.0;
	wxDateTime Date;

	int commas = 0;
	// NUMBER OF COMMAS THAT SHOULD BE FOUND...
	int noc = 1;

	while (Data.size() > index && Data[index] != endline)
	{
		if (Data[index] == comma)
			++commas;

		++index;
	}

	if (commas != noc)
		return;

	index = i;
	char c = ' ';
	commas = 0;
	while (Data.size() > index && Data[index] != endline)
	{
		c = Data[index];
		if (c == comma)
		{
			if (!Date.ParseDate(data))
			{
				GetToEndline(index, Data);
				return;
			}
			else if ((this->PurchaseDate.IsValid() && Date < this->PurchaseDate) || (this->LastDivDate.IsValid() && Date < this->LastDivDate))
			{
				GetToEndline(index, Data);
				return;
			}
			data = "";
			++index;
			continue;
		}

		if (isdigit(c) || c == dash || c == period)
			data += Data[index];
		else
		{
			GetToEndline(index, Data);
			return;
		}
		++index;
	}

	if (!data.ToDouble(&div))
	{
		GetToEndline(index, Data);
		return;
	}

	this->CallBackDiv(this->m_parent, Dividend(0, Date, div));
}

wxString Parser::GetValue(size_t& index, wxString& data)
{
	wxString value = "";
	while (index < data.size() && data[index] != '\n' && data[index] != ',')
		value += data[index++];

	return value;
}

/*
void Parser::ParseTable(wxString& Data, void* sh, void (*foo)(void*, wxDateTime&, double&, double&, double&, double&), void(*foo2)(wxDateTime&, bool B), wxString firstdate)
{
	int index = Data.find(firstdate);
	if (index == -1)
	{
		wxMessageBox("Cant find start date!");
		return;
	}
	return;
	index = Data.find(Linkbutton);
	if (index == -1)
	{
		wxMessageBox("Cant find end table head!");
		return;
	}

	index = Data.find("<tbody", index);
	if (index == -1)
	{
		wxMessageBox("Cant find body start!");
		return;
	}

	int endindex = Data.find(body_end, index);
	if (endindex == -1)
	{
		wxMessageBox("Cant find body end!");
		return;
	}

	wxString Rows = Data.Mid(index, endindex - index + body_end.size() + 20);
	index = Rows.find(RowStart);
	if (index == -1)
	{
		wxMessageBox("Cant find row start!");
		return;
	}

	index = Rows.find(firstdate);
	if (index == -1)
	{
		wxMessageBox("Cant find start date!");
		return;
	}

	endindex = Rows.find(row_end, index);
	if (endindex == -1)
	{
		wxMessageBox("Cant find row end!");
		return;
	}

	wxString Row = Rows.Mid(index - 5, endindex - index + row_end.size());
	int mainloop = 0;
	int subloop = 0;
	wxDateTime date;
	double open = 0.0;
	double low = 0.0;
	double high = 0.0;
	double close = 0.0;
	double adjclose = 0.0;
	long volume = 0.0;
	index = Row.find(DEP);
	Deincrement.ParseDate(firstdate);
	index = 0;

	do
	{

		while (index != -1 && subloop < 20)
		{
			ValidData = true;
			wxString value = GetData(index, Row, 1);
			Switch(subloop, date, open, high, low, close, adjclose, volume, value);
			index = Row.find(DataStart, index);
			index = Row.find(DEP, index);
			++subloop;
		}

		if (subloop == 0)
			break;

		if (ValidData)
			foo(sh, date, open, high, low, close);

		do {
			foo2(Deincrement, true);
			wxString start = Deincrement.Format("%b %d, %G");
			index = Rows.find(start);
			++subloop;
		} while (index == -1 && subloop < 4);

		if (index == -1)
			break;

		subloop = 0;
		index = index - 2;

		endindex = Rows.find(row_end, index);
		if (endindex == -1)
		{
			wxMessageBox("Cant find row end!");
			break;
		}

		Row = Rows.Mid(index, endindex - index + 20);
		if (index != -1)
			index = 0;
		++mainloop;

	} while (index != -1 && mainloop < 3000);

	Deincrement = wxDateTime();
}


void Parser::Switch(int& i, wxDateTime& T, double& o, double& h, double& l, double& c, double& ac, long& v, wxString& value)
{
	int index = value.find(',');
	while (index != -1)
	{
		value = value.Remove(index, 1);
		index = value.find(',');
	}

	if (index != -1)
		value = value.Remove(index, 1);
	switch (i)
	{
	case 0:
	{
		if (!T.ParseDate(value))
		{
			wxMessageBox("Could not parse date in Parser::Switch! String value is: " + value);
			ValidData = false;
			return;
		}
		if (!Deincrement.IsValid())
			Deincrement = T;
		return;
	}
	case 1:
	{
		if (!value.ToDouble(&o))
		{
			wxMessageBox("ToDouble failed in Parser::Switch! String value is: " + value);
			ValidData = false;
		}
		return;
	}
	case 2:
	{
		if (!value.ToDouble(&h))
		{
			wxMessageBox("ToDouble failed in Parser::Switch! String value is: " + value);
			ValidData = false;
		}
		return;
	}
	case 3:
	{
		if (!value.ToDouble(&l))
		{
			wxMessageBox("ToDouble failed in Parser::Switch! String value is: " + value);
			ValidData = false;
		}
		return;
	}
	case 4:
	{
		if (!value.ToDouble(&c))
		{
			wxMessageBox("ToDouble failed in Parser::Switch! String value is: " + value);
			ValidData = false;
		}
		return;
	}
	case 6:
	{
		if (!value.ToDouble(&ac))
		{
			wxMessageBox("ToDouble failed in Parser::Switch! String value is: " + value);
			ValidData = false;
		}
		return;
	}
	case 7:
	{
		if (!value.ToLong(&v))
			wxMessageBox("ToLong failed in Parser::Switch! String value is: " + value);
		return;
	}
	}
}
*/

wxString Parser::HUE(int index, wxString& Data)
{
	char quote = '\"';
	wxString url = "";
	while (index > 0 && Data[index] != quote)
		--index;

	--index;
	while (index > 0 && Data[index] != quote)
	{
		url += Data[index];
		--index;
	}

	wxString searchword = "amp;";
	int pos = url.find(searchword);
	while (pos != -1)
	{
		url = url.Remove(pos, searchword.size());
		pos = url.find(searchword);
	}

	return url;
}

SummaryData Parser::GetSummaryData()
{
	sumdata.curr_price = RemoveCommas(sumdata.curr_price);
	sumdata.open = RemoveCommas(sumdata.open);
	sumdata.prev_close = RemoveCommas(sumdata.prev_close);
	sumdata.ToDoubles();
	return sumdata;
}

int Parser::BeginParse(wxString& data)
{
	int index = data.find(BH, 0);
	if (index == -1)
		return index;

	index = data.find(BOLAT, index);
	if (index == -1)
		return index;

	index += BOLAT.size();
	sumdata.Longname = GetData(index, data, DataType::LONGNAME);
	if (sumdata.Longname == "NotFound")
		return -1;

	index = data.find(BCPR, index);
	if (index == -1)
		return index;

	index += BCPR.size();
	sumdata.curr_price = GetData(index, data, DataType::LONGNAME);
	if (sumdata.curr_price == "NotFound")
		return -1;

	index = data.find(PCR, index);
	if (index == -1)
		return index;

	index += PCR.size();
	sumdata.prev_close = GetData(index, data, DataType::LONGNAME);
	if (sumdata.prev_close == "NotFound")
		return -1;

	index = data.find(OVR, index);
	if (index == -1)
		return index;

	index += OVR.size();
	sumdata.open = GetData(index, data, DataType::LONGNAME);
	if (sumdata.open == "NotFound")
		return -1;

	index = data.find(BR, index);
	if (index == -1)
		return index;

	index += BR.size();
	sumdata.beta = GetData(index, data, DataType::LONGNAME);
	if (sumdata.beta == "NotFound")
		return -1;

	index = data.find(EV, index);
	if (index == -1)
		return index;

	index += EV.size();
	sumdata.earningsdate = GetData(index, data, DataType::LONGNAME);
	if (sumdata.earningsdate == "NotFound")
		return -1;

	return 0;
}

wxString Parser::GetData(int& index, wxString& data, int type)
{
	wxString DATA = "";

	index = data.find(BD, index);
	if (index == -1)
		return "NotFound";

	if (data[index + 1] == ED)
	{
		index = data.find(BD, index + 1);
		if (index == -1)
			return "NotFound";
	}

	int holder = data.find(ED, index);
	DATA = data.Mid(index + 1, --holder - index);
	return DATA;
}

void Parser::InsertDates(wxString& start, wxString& end, bool indices)
{
	wxString hdwd = indices ? this->indices_url : Normal;
	wxString bd = "BEGINDATE";
	wxString ed = "ENDDATE";
	wxString t = "TICKER";
	wxString h = "history";
	wxString d = "div";

	size_t success1 = hdwd.Replace(t, ticker);
	size_t success2 = hdwd.Replace(bd, start);
	size_t success3 = hdwd.Replace(ed, end);

	if (success1 && success2 && success3)
		HDURL = hdwd;
	else
		wxMessageBox("DATE key word was not successfully replaced in historicaldata url!");

	hdwd.Replace(h, d);
	HDDURL = hdwd;
}

void Parser::UpDate(bool historical, bool div, bool indices)
{
	wxDateTime today;
	if (!this->end_Date.IsValid())
		today = wxDateTime(wxDateTime::Today().GetDay(), wxDateTime::Today().GetMonth(), wxDateTime::Today().GetYear(), 19);
	else
		today = wxDateTime(this->end_Date.GetDay(), this->end_Date.GetMonth(), this->end_Date.GetYear(), 19);
	GetWorkDate(today);

	int year = today.GetYear();
//	int years = 1;
	wxString date = this->lastdate.size() ? this->lastdate : this->purchaseDate;
	this->startDate.ParseDate(date);
	if (this->startDate.IsValid())
		year = this->startDate.GetYear();
	else
	{
		wxFAIL_MSG("this->startDate in Parser::Update failed to ParseDate! wxString parsed is: " + date);
		return;
	}

//	if (year != today.GetYear())
//		years = today.GetYear() - year;

	wxString start = "";
	wxString end = "";
	wxDateTime begin(1, wxDateTime::Month::Jan, year, 18);
	start << yahoofinancedate(begin.GetValue());
	end << yahoofinancedate(today.GetValue());
	InsertDates(start, end, indices);

	wxString Data = "";
	webdata web;
	web.setpages(1);
	web.seturl(HDURL);
	if (historical)
	{
		for (int i = 0; i < 10; ++i)
		{
			if (web.getwebdata(Data))
			{
				ParseStockHistory(Data);
				break;
			}

			Data = "";
			end = "";
			int day = DayValue * (i + 1);
			end << yahoofinancedate(today.GetValue() - day);
			InsertDates(start, end);
			web.setpages(1);
			web.seturl(HDURL);
		}
	}
	if (div)
	{
		Data = "";
		web.setpages(1);
		web.seturl(HDDURL);
		if (web.getwebdata(Data))
			ParseDivHistory(Data);
		else
			wxMessageBox("Could not get webdata for Stock_History::UpDate historical Dividends! URL is: " + HDDURL);
	}

	/*
	for (size_t index = 0; index <= years; ++index)
	{
		year += index;

		wxString start = "";
		wxString end = "";
		wxDateTime begin(1, wxDateTime::Month::Jan, year, 18);
		start << yahoofinancedate(begin.GetValue());
		end << yahoofinancedate(today.GetValue());
		InsertDates(start, end);

		wxString Data = "";
		webdata web;
		web.setpages(1);
		web.seturl(HDURL);
		if (historical)
		{
			for (int i = 0; i < 10; ++i)
			{
				if (web.getwebdata(Data))
				{
					ParseStockHistory(Data);
					break;
				}

				Data = "";
				end = "";
				int day = DayValue * (i + 1);
				end << yahoofinancedate(today.GetValue() - day);
				InsertDates(start, end);
				web.setpages(1);
				web.seturl(HDURL);
			}
		}
		if (div)
		{
			Data = "";
			web.setpages(1);
			web.seturl(HDDURL);
			if (web.getwebdata(Data))
				ParseDivHistory(Data);
			else
				wxMessageBox("Could not get webdata for Stock_History::UpDate historical Dividends! URL is: " + HDDURL);
		}
	}
	*/
}

bool Parser::PullWebData(bool indices)
{
	wxString Data = "";
	wxString URL = "https://finance.yahoo.com/quote/TICKER?p=TICKER&.tsrc=fin-srch";
	if (indices)
		URL = "https://finance.yahoo.com/quote/%5ETICKER?p=%5ETICKER";
	URL.Replace("TICKER", this->ticker);
	webdata web;
	web.setpages(1);
	web.seturl(URL);
	if (!web.getwebdata(Data))
		return false;
	wxString result = "Found.Redirecting to /lookup?s=" + ticker;
	if (result == Data)
		return false;

	this->ParseSummaryData(Data);
	return true;
}

bool Parser::PullWebData(wxString& url, wxString& data)
{
	webdata web;
	web.seturl(url);
	web.setpages(1);
	return web.getwebdata(data);
}

int Parser::FindItem(wxString item, wxString& data, int index)
{
	return data.find(item, index);
}

void Parser::ParseSummaryData(wxString& data)
{
	BeginParse(data);

	this->sumdata.date = wxDateTime::Today();
	this->sumdata.time = wxDateTime::Now();
	if (this->CallBackSummary)
		this->CallBackSummary(this->m_parent, this->GetSummaryData(), type);
}
