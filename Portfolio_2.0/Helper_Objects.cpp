#include "Helper_Objects.h"


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

// SectorStock functions...
SectorStock::SectorStock(wxString& _ticker, wxString& _longname, wxString& _price, wxString& _change, 
	wxString& _percent_change, wxString& _reg_mar_vol, wxString& _avg_vol, wxString& _market_cap)
	: ticker(_ticker), longname(_longname), price(_price), change(_change), percent_change(_percent_change),
	reg_mar_vol(_reg_mar_vol), avg_vol(_avg_vol), market_cap(_market_cap)
{

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

wxDateTime::Month inline G_GetQuarterStartMonth(wxDateTime::Month m)
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


// Generic functions for getting different day_prices...
double G_GetClosePrice(wxDateTime* T, wxVector<Day_Prices>& vec)
{
	if (!T)
	{
		wxMessageBox("wxDateTime* is null in G_GetClosePrice!");
		return 0.0;
	}

	GetWorkDate(*T);

	wxString T_date;
	wxString date;
	for (auto& v : vec)
	{
		T_date = T->Format(STANDARD_DATE);
		date = v.date.Format(STANDARD_DATE);
		if (v.date == *T)
			return v.close;
	}

	if (vec.rbegin() != vec.rend())
		return vec.rbegin()->close;

	return 0.0;
}

double G_GetPreviousClose(wxDateTime* T, wxVector<Day_Prices>& vec)
{
	if (!T)
	{
		if (vec.rbegin() == vec.rend())
			return 0.0;
		auto it = vec.rbegin();
		++it;
		if (it == vec.rend())
			return 0.0;
		return it->close;
	}

	wxDateTime temp = *T;
	GetWorkDate(temp);
	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (vec[i].date == temp)
		{
			if (i > 0)
				return vec[--i].close;
			else
				return 0.0;
		}
	}

	wxMessageBox("Date not found in G_GetPreviousClose!");
	return 0.0;
}

int inline GetStartWeekDay(wxDateTime* T)
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

wxDateTime GetLatestMarketOpenDate()
{
	wxDateTime T = wxDateTime::Today();
	GetWorkDate(T);
	return T;
}

wxDateTime G_GetFirstDayOfTheWeekDate(wxDateTime* date)
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

double GetEarliestWeekDayClose(wxDateTime* T, wxDateTime* purchaseDate, wxVector<Day_Prices>& vec)
{
	wxDateTime date = wxDateTime::Today();
	if (T)
		date = *T;

	date = G_GetFirstDayOfTheWeekDate(&date);

	if (date < *purchaseDate)
		return G_GetClosePrice(purchaseDate, vec);
	else
		return G_GetClosePrice(&date, vec);
}

double G_GetQuarterStartClose(wxDateTime* T, wxDateTime* purchaseDate, wxVector<Day_Prices>& vec)
{
	wxDateTime date = wxDateTime::Today();
	wxString s_date = date.Format(STANDARD_DATE);
	wxString p_date = purchaseDate->Format(STANDARD_DATE);
	if (T)
		date = *T;
	wxString a_s_date = date.Format(STANDARD_DATE);
	date = G_GetQuarterStartDate(&date);
	wxString af_date = date.Format(STANDARD_DATE);
	if (date < *purchaseDate)
		return G_GetClosePrice(purchaseDate, vec);
	else
		return G_GetClosePrice(&date, vec);
}

wxDateTime G_GetQuarterStartDate(wxDateTime* T)
{
	wxDateTime date = wxDateTime::Today();
	if (T)
		date = *T;

	date = wxDateTime(1, G_GetQuarterStartMonth(date.GetMonth()), date.GetYear());
	GetForwardWorkDay(date);
	
	return date;
}

double GetYearStartClose(wxDateTime* T, wxDateTime* purchaseDate, wxVector<Day_Prices>& vec)
{
	wxDateTime date = wxDateTime::Today();
	if (T)
		date = *T;

	date = wxDateTime(2, wxDateTime::Month::Jan, date.GetYear());
	GetForwardWorkDay(date);

	if (date < *purchaseDate)
		return G_GetClosePrice(purchaseDate, vec);
	else
		return G_GetClosePrice(&date, vec);
}

wxDateTime GetFirstDayOfTheWeekDate(wxDateTime* date)
{
	if (date->GetWeekDay() == wxDateTime::WeekDay::Mon)
		return *date;

	wxDateTime T = *date;
	while (T.GetWeekDay() != wxDateTime::WeekDay::Mon)
	{
		T = T - wxDateSpan(0, 0, 0, 1);
	}

	return T;
}

wxDateTime GetLatestCloseDate(wxVector<Day_Prices>& vec)
{
	return vec.rbegin() != vec.rend() ? vec.rbegin()->date : wxDateTime(wxInvalidDateTime);
}

double GetDeviation(int days, wxVector<Day_Prices>&  vec)
{
	int size = days;
	auto it = vec.rbegin();
	if (it == vec.rend())
		return 0.0;

	double total = 0.0, standardDiv = 0.0;
	while (it != vec.rend() && days)
	{
		--days;
		total += it->close;
		++it;
	}

	if (days != 0)
		size -= days;

	double mean = total / size;

	days = size;
	it = vec.rbegin();
	while (it != vec.rend() && days)
	{
		--days;
		standardDiv += std::pow((it->close - mean), 2);
		++it;
	}

	return std::sqrt(standardDiv / (size - 1));
}