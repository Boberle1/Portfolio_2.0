#include "Parser.h"

auto yahoofinancedate = [](wxLongLong Long)->wxLongLong { return Long / 1000; };

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

// HOLIDAYS STRUCT...
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

Holidays holidays;
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

Parser::Parser(wxString& data, bool historical)
{
	BeginParse(data);
}

Parser::Parser(wxString& Data, void* sh, void (*foo)(void* sh, wxDateTime&, double&, double&, double&, double&), void(*foo2)(wxDateTime&, bool B), wxString firstdate)
{
	BeginParse(Data);
	ParseTable(Data, sh, foo, foo2, firstdate);
}

Parser::Parser(void* parent, wxString t, wxString pd, wxString ld, wxString ldd, void (*cb)(void* v, double o, double h, double l, double c, wxDateTime d), 
	void (*cbs)(void* v, SummaryData), void (*cbd)(void* v, Dividend), wxString enddate) : m_parent(parent), ticker(t), purchaseDate(pd), lastdate(ld), lastDivDate(ldd),
	CallBack(cb), CallBackSummary(cbs), CallBackDiv(cbd), m_enddate(enddate)
{
	this->PurchaseDate.ParseDate(this->purchaseDate);
	this->startDate.ParseDate(this->lastdate);
	this->LastDivDate.ParseDate(this->lastDivDate);
	this->end_Date.ParseDate(this->m_enddate);
}

void Parser::UpDateAll()
{
	this->UpDateSummaryData();
	this->UpDate(true, true);
}

void Parser::UpDateSummaryData()
{
	this->PullWebData();
}

void Parser::UpDateHistoricalPrices()
{
	this->UpDate(true, false);
}

void Parser::UpDateDiv()
{
	this->UpDate(false, true);
}

wxString Parser::RetrieveHistoricalURL()
{
	return URL;
}

void Parser::SetLastDateAndCallBack(wxString date, void (*cb)(void* v, double o, double h, double l, double c, wxDateTime d))
{
	this->CallBack = cb;
	this->lastdate = date;
}

void Parser::SetTickerAndPurchaseDate(wxString t, wxString pd)
{
	this->ticker = t;
	this->purchaseDate = pd;
}

void Parser::SetSummaryDataCallBack(void (*cb)(void* v, SummaryData))
{
	this->CallBackSummary = cb;
}

void Parser::SetLatestDividendDateAndCallBack(wxString div, void (*cb)(void* v, Dividend))
{
	this->lastDivDate = div;
	this->CallBackDiv = cb;
}

/*
bool Parser::ParsePrices(wxString& Data, size_t& index)
{
	auto end = Day.rbegin();
	if (end == Day.rend())
		return false;

	wxString test = end->first.Format("%G-%m-%d");
	int find = Data.find(end->first.Format("%G-%m-%d"));
	if (find == -1)
	{
		// CHECK TO SEE IF WE HAVE A BAD DATE...
		auto it = Day.begin();
		auto& iter = it;
		for (it; it != Day.end(); ++it)
			iter = it;

		if (end->first > wxDateTime::Today().GetValue())
		{
			wxMessageBox("Bad date in Daymap in StockHistory::ParsePrices. Date is: " + iter->first.Format("%m/%d/%G") + " Date will be removed.");
			Day.erase(iter);
			auto next = Day.rbegin();
			if (next == Day.rend())
			{
				wxMessageBox("Day map in Stock_History::ParsePrices is empty!");
				return false;
			}
			find = Data.find(next->first.Format("%G-%m-%d"));
			if (find == -1)
			{
				wxMessageBox("Could not find date in Stock_History::ParsePrices! Date is: " + next->first.Format("%m/%d/%G"));
				Day.erase(next->first);
				return false;
			}
		}
	}

	index = find;
	GetToEndline(index, Data);
	++index;

	if (index >= Data.size())
		return true;

	for (index; index < Data.size(); ++index)
		GetLine(index, Data);

	for (auto& it : dayholder)
		Day.insert(it);

	dayholder.clear();

	return true;
}
*/

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

/*
void Parser::ParseStockHistory(wxString& Data)
{
	wxString key = "Date,Open,High,Low,Close,Adj Close,Volume\n";
	int size = key.size();
	int i = Data.find(key);
	if (i == -1)
	{
		i = Data.find("Date,Open,High,Low,Close,Adj Close,Volume");
		size = key.size();
	}

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

	if (Day.size())
	{
		bool Notfound = false;
		int counter = 0;
		auto it = dayholder.find(Day.rbegin()->first);
		auto iter = Day.rbegin();
		auto& holder = iter;
		if (it == dayholder.end())
		{
			wxMessageBox("dayholder map could not find date in Stock_History::ParseStockHistory! And ticker is: " + ticker + 
				"! Date trying to find is: " + Day.rbegin()->first.Format("%m/%d/%G"));
			Notfound = true;
			do {
				++iter;
				if (iter == Day.rend() || counter > 30)
				{
					wxMessageBox("Could not find any matching dates in dayholder after iterating through 30 days in Days map!");
					return;
				}
				++counter;
				it = dayholder.find(iter->first);
			} while (it == dayholder.end());
		}

		++it;
		for (it; it != dayholder.end(); ++it)
			Day.insert_or_assign(it->first, it->second);

		if (Notfound)
			Day.erase(holder->first);
	}

	else
	{
		for (auto it = dayholder.find(PurchaseDate); it != dayholder.end(); ++it)
			Day.insert_or_assign(it->first, it->second);
	}
}
*/
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

	if (!data.ToLong(&Volume))
		return;

	this->CallBack(this->m_parent, Open, High, Low, Close, Date);
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

	this->CallBackDiv(this->m_parent, Dividend(Date, div));
}

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

	/*
	index = Data.find("<tbody", index + 6);
	if (index == -1)
	{
		wxMessageBox("Cant find body start!");
		return;
	}
	*/
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

void Parser::GetHistoricalURL(wxString& Data)
{
	int index = Data.find(href, 0);
	if (index == -1)
	{
		wxMessageBox("href was not found in Parser::GetHistorcalURL()");
		return;
	}

	URL = HUE(index, Data);
}

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

void Parser::InsertDates(wxString& start, wxString& end)
{
	wxString hdwd = Normal;
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

void Parser::UpDate(bool historical, bool div)
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

bool Parser::PullWebData()
{
	wxString Data = "";
	wxString URL = "https://finance.yahoo.com/quote/TICKER?p=TICKER&.tsrc=fin-srch";
	URL.Replace("TICKER", this->ticker);
	webdata web;
	web.setpages(1);
	web.seturl(URL);
	if (!web.getwebdata(Data))
		return false;

	this->ParseSummaryData(Data);
	return true;
}

void Parser::ParseSummaryData(wxString& data)
{
	BeginParse(data);

	this->sumdata.date = wxDateTime::Today();
	this->sumdata.time = wxDateTime::Now();
	this->CallBackSummary(this->m_parent, this->GetSummaryData());
}
