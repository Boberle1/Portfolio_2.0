#pragma once
#include "wx/wx.h"
#include "wx/txtstrm.h"
#include "wx/wfstream.h"
#include "wx/wfstream.h"
#include "Enums.h"

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

// Simple struct for sectorstock data...
struct SectorStock
{
	SectorStock(_Sector, wxString&, wxString&, wxString&, wxString&, wxString&, wxString&, wxString&, wxString&, wxString&);

	wxString ticker = "";
	wxString longname = "";
	wxString sectorname = "";
	wxString price = "";
	wxString change = "";
	wxString percent_change = "";
	wxString reg_mar_vol = "";
	wxString avg_vol = "";
	wxString market_cap = "";

	double d_price = 0.0;
	double d_previous_close = 0.0;
	double d_change = 0.0;
	double d_change_perc = 0.0;
	double d_vol = 0.0;
	double d_avg_vol = 0.0;
	int Market_cap = 0.0;

	_Sector sec;
	wxVector<Day_Prices> days;
};

Holidays& GetHolidays();
void GetWorkDate(wxDateTime& T, bool B = false);
void GetForwardWorkDay(wxDateTime& T);
bool IsMarketOpen();

wxDateTime::Month inline G_GetQuarterStartMonth(wxDateTime::Month);
wxDateTime::Month inline GetQuarterEndMonth(wxDateTime::Month);

double G_GetClosePrice(wxDateTime*, wxVector<Day_Prices>&);
double G_GetPreviousClose(wxDateTime*, wxVector<Day_Prices>&);
int inline GetStartWeekDay(wxDateTime*);
wxDateTime GetLatestMarketOpenDate();
wxDateTime G_GetFirstDayOfTheWeekDate(wxDateTime*);
double GetEarliestWeekDayClose(wxDateTime*, wxDateTime*, wxVector<Day_Prices>&);
double G_GetQuarterStartClose(wxDateTime*, wxDateTime*, wxVector<Day_Prices>&);
wxDateTime G_GetQuarterStartDate(wxDateTime*);
double GetYearStartClose(wxDateTime*, wxDateTime*, wxVector<Day_Prices>&);
wxDateTime GetFirstDayOfTheWeekDate(wxDateTime*);
wxDateTime GetLatestCloseDate(wxVector<Day_Prices>&);
double GetDeviation(int, wxVector<Day_Prices>&);

