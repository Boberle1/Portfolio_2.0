#pragma once

#include "wx/wx.h"
#include "wx/txtstrm.h"
#include "wx/wfstream.h"
#include "wx/wfstream.h"
#include "Enums.h"

auto constexpr NA = "NotFound";

void SetStaticTextColor(wxStaticText&, wxColour, wxColour);
bool IsStringPositive(wxString&);

// Check if string that represents number or double is a negative, positive or zero...
int IsStringPNZ(wxString&);

// Function that compares two wxDateTimes and discards the time,
// So if they are the same year, month, day, regardless of time this will return true
// Otherwise false...
bool IswxDateEqual(wxDateTime&, wxDateTime&);

struct Stocklist
{
	wxString ticker = "";
	wxString company_name = "";
	double last_sale = 0.0;
	double net_change = 0.0;
	double percent_change = 0.0;
	double market_cap = 0.0;
	wxString country = "";
	int ipo_year = 0;
	double volume = 0.0;
	wxString sector = "";
	wxString industry = "";
};

struct Dividend
{
	Dividend(long l, wxDateTime ex, double d) : id(l), ex_Div(ex), div(d) {}
	Dividend(long l, wxDateTime ex, double d, bool reinvest, double reinvest_shares) : id(l), ex_Div(ex), div(d), DivReinvest(reinvest), re_invest_shares(reinvest_shares) {}
	bool IsPendingReInvestment()
	{
		if (DivReinvest && !re_invest_shares)
		{
			if (!this->payment_date.IsValid())
				return true;
			if (this->payment_date > wxDateTime::Today())
				return false;
			
			return true;
		}

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
	void AddReInvestmentShares(double& shares)
	{
		this->re_invest_shares = shares;
	}
	void AddPaymentDate(wxDateTime& date)
	{
		this->payment_date = date;
	}

	long id;
	double div = 0.0;
	wxDateTime ex_Div;
	wxDateTime payment_date;
	bool DivReinvest = false;
	double re_invest_shares = 0.0;
	bool linked = false;
};

struct SummaryData
{
	wxString Longname = "NotFound";
	wxString curr_price = "NotFound";
	wxString prev_close = "NotFound";
	wxString open = "NotFound";
	wxString beta = "NotFound";
	wxString earningsdate = "NotFound";
	wxString description = "";

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
		//	wxMessageBox("curr_price.ToDouble failed in SummaryData::ToDoubles! wxString is: " + curr_price + " Longname is: " + Longname);
			marketprice = 0.0;
		}
		if (!prev_close.ToDouble(&previousclose))
		{
		//	wxMessageBox("prev_close.ToDouble failed in SummaryData::ToDoubles! wxString is: " + prev_close + " Longname is: " + Longname);
			previousclose = 0.0;
		}
		if (!open.ToDouble(&Open))
		{
		//	wxMessageBox("open.ToDouble failed in SummaryData::ToDoubles! wxString is: " + open + " Longname is: " + Longname);
			Open = 0.0;
		}
		if (!beta.ToDouble(&Beta))
		{
		//	wxMessageBox("beta.ToDouble failed in SummaryData::ToDoubles! wxString is: " + beta + " Longname is: " + Longname);
			Beta = 0.0;
		}
	}
};

struct DayGainersandLosers
{
	wxString ticker = "";
	wxString longname = "";
	wxString change = "";
	wxString marketprice = "";
	double _change = 0.0;
	double _marketprice = 0.0;
	double _gain_loss = 0.0;
	double _previouclose = 0.0;
	void ToDoubles()
	{

		if (change.find('+') != -1)
			change = change.Mid(1);
		if (marketprice.find('+') != -1 || marketprice.find('-') != -1)
			marketprice = marketprice.Mid(1);

		if (!this->change.ToDouble(&this->_change))
			wxMessageBox("ToDouble failed on change in DayGainersandLosers::ToDoubles; string was: " + this->change);

		if (!this->marketprice.ToDouble(&this->_marketprice))
			wxMessageBox("ToDouble failed on change in DayGainersandLosers::ToDoubles; string was: " + this->marketprice);

		if (_change < 0)
			this->_previouclose = this->_marketprice + -(this->_change);
		else
			this->_previouclose = this->_marketprice - this->_change;

		this->_gain_loss = this->_previouclose ? this->_marketprice / this->_previouclose - 1 : 0.0;
	}
};

// Simple struct to Write and Read SectorData to and from disc for ItemInfo..
// SectorClass encaplsulates this so ParentSector and SubSector and ItemInfo are the users of this struct
struct SectorData
{
	SectorData() {}
	SectorData(double& p, double& dr, double& wr, double& mr, double& qr, double& hyr, double& ytdr, double& yr, double& mc, wxDateTime& d)
		: price(p), day_return(dr), week_return(wr), month_return(mr), quarter_return(qr), half_year_return(hyr), YTD_return(ytdr),
		year_return(yr), market_cap(mc), date(d) {}

	double price = 0.0;
	double day_return = 0.0;
	double week_return = 0.0;
	double month_return = 0.0;;
	double quarter_return = 0.0;
	double half_year_return = 0.0;
	double YTD_return = 0.0;
	double year_return = 0.0;
	double market_cap = 0.0;
	wxDateTime date;
};

// Simple struct for Parser to gather overviewdata from the web to give to SectorClass...
struct SectorOverview
{
	SectorOverview(wxString n, wxString st, wxString mc, wxString d, wxString pe, wxString fpe, wxString peg, wxString fs, wxString c, wxString v)
		: name(n), _numOfstocks(st), _market_cap(mc), _dividend(d), _PtoE(pe), _forward_PtoE(fpe), _PEG(peg), _float_short(fs), _change(c), _volume(v)
	{
		this->ToDouble();
	}
	SectorOverview() {}
	wxString name = "";
	wxString _numOfstocks = "";
	wxString _market_cap = "";
	wxString _dividend = "";
	wxString _PtoE = "";
	wxString _forward_PtoE = "";
	wxString _PEG = "";
	wxString _float_short = "";
	wxString _change = "";
	wxString _volume = "";
	void ToDouble()
	{
		if (this->name == "Specialty Retail")
			int Catch = 0;

		if (name.find("\"") != -1)
			name = name.Mid(1, name.size() - 2);

		if (_volume.find('\r') != -1)
			_volume = _volume.Mid(0, _volume.size() - 1);

		if (_dividend.find('%') != -1)
			_dividend = _dividend.Mid(0, _dividend.size() - 1);

		if (_float_short.find('%') != -1)
			_float_short = _float_short.Mid(0, _float_short.size() - 1);

		if (_change.find('%') != -1)
			_change = _change.Mid(0, _change.size() - 1);

		if (!_market_cap.ToDouble(&market_cap))
			wxMessageBox("SectorOverview::ToDouble failed on _market_cap! _market_cap was: " + _market_cap);
		if (!_dividend.ToDouble(&dividend))
			wxMessageBox("SectorOverview::ToDouble failed on _dividend! _dividend was: " + _dividend);
		if (!_float_short.ToDouble(&float_short))
			wxMessageBox("SectorOverview::ToDouble failed on _float_short! _float_short was: " + _float_short);
		if (!_change.ToDouble(&change))
			wxMessageBox("SectorOverview::ToDouble failed on _change! _change was: " + _change);

		market_cap = market_cap * 1000000;
		_market_cap << market_cap;
	}

	double market_cap = 0.0;
	double dividend = 0.0;
	double PtoE = 0.0;
	double forward_PtoE = 0.0;
	double PEG = 0.0;
	double float_short = 0.0;
	double change = 0.0;
};

// Simple struct for Parser to gather Performance Data from the web to give to SectorClass...
struct SectorPerformance
{
	SectorPerformance(wxString n, wxString w, wxString m, wxString q, wxString h, wxString y, wxString ytd)
		: name(n), _week(w), _month(m), _quarter(q), _half(h), _year(y), _yearToDate(ytd), stamp(wxDateTime::Now())
	{
		this->ToDouble();
	}
	SectorPerformance() : stamp(wxDateTime::Now()) {}
	wxString name = "";
	wxString _week = "";
	wxString _month = "";
	wxString _quarter = "";
	wxString _half = "";
	wxString _year = "";
	wxString _yearToDate = "";
	void ToDouble()
	{
		if (name.find("\"") != -1)
			name = name.Mid(1, name.size() - 2);

		if (_week.find('%') != -1)
			_week = _week.Mid(0, _week.size() - 1);

		if (_month.find('%') != -1)
			_month = _month.Mid(0, _month.size() - 1);

		if (_quarter.find('%') != -1)
			_quarter = _quarter.Mid(0, _quarter.size() - 1);

		if (_half.find('%') != -1)
			_half = _half.Mid(0, _half.size() - 1);

		if (_year.find('%') != -1)
			_year = _year.Mid(0, _year.size() - 1);

		if (_yearToDate.find('%') != -1)
			_yearToDate = _yearToDate.Mid(0, _yearToDate.size() - 1);

		if (!_week.ToDouble(&week))
			wxMessageBox("SectorPerformance::ToDouble failed on _week! _market_cap was: " + _week);
		if (!_month.ToDouble(&month))
			wxMessageBox("SectorPerformance::ToDouble failed on _month! _dividend was: " + _month);
		if (!_quarter.ToDouble(&quarter))
			wxMessageBox("SectorPerformance::ToDouble failed on _quarter! _float_short was: " + _quarter);
		if (!_half.ToDouble(&half))
			wxMessageBox("SectorPerformance::ToDouble failed on _half! _float_short was: " + _half);
		if (!_year.ToDouble(&year))
			wxMessageBox("SectorPerformance::ToDouble failed on _year! _change was: " + _year);
		if (!_yearToDate.ToDouble(&yearToDate))
			wxMessageBox("SectorPerformance::ToDouble failed on _yearToDate! _change was: " + _yearToDate);
	}

	double week = 0.0;
	double month = 0.0;
	double quarter = 0.0;
	double half = 0.0;
	double year = 0.0;
	double yearToDate = 0.0;
	wxDateTime stamp;
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

struct pair
{
	pair(long l, double d, wxDateTime t) : id(l), value(d), date(t) {}
	long id = 0;
	double value = 0.0;
	wxDateTime date;
};

struct Pair
{
	Pair(wxString s, double d) : ticker(s), amount(d) {}
	Pair(wxString& s, double& d) : ticker(s), amount(d) {}
	wxString ticker = "";
	double amount = 0.0;
};

struct deposit_pair
{
	deposit_pair(wxDateTime d, double v) : T(d), value(v) {}
	wxDateTime T;
	double value = 0;
};

struct withdrawl_pair
{
	withdrawl_pair(wxDateTime d, double v) : date(d), value(v) {}
	wxDateTime date;
	double value = 0.0;
};

class DataStream
{
public:
	DataStream(wxString, wxPosixPermissions, wxFile::OpenMode);
	~DataStream();
	bool IsOK();
	wxFile& GetFileHandle();
	template <typename T>
	void WriteData(T&);
	template <typename T>
	void ReadData(T&);
	void WriteLong(long&);
	void ReadLong(long&);
	void Write(wxString&);
	void Read(wxString&);
	void ReadwxDateTime(wxDateTime&);
	void WritewxDateTime(wxDateTime&);
	void ReadTimeTm(wxDateTime::Tm&);
	void WriteTimeTm(wxDateTime::Tm&);
	template <typename T>
	void WritewxVector(wxVector<T>&);
	template <typename T>
	void ReadwxVector(wxVector<T>&);
	void WriteBool(bool&);
	void ReadBool(bool&);
	void WriteDividendVector(wxVector<Dividend>&);
	void ReadDividendVector(wxVector<Dividend>&);
	void WriteSummaryData(SummaryData&);
	void ReadSummaryData(SummaryData&);
	void WriteDayPrices(Day_Prices&);
	void ReadDayPrices(Day_Prices&);
	void WriteDayPricesVector(wxVector<Day_Prices>&);
	void ReadDayPricesVector(wxVector<Day_Prices>&);
	void WriteDepositVector(wxVector<deposit_pair>&);
	void WriteWithdrawlVector(wxVector<withdrawl_pair>&);
	void Write_pairVector(wxVector<pair>&);
	void ReadDepositVector(wxVector<deposit_pair>&);
	void ReadWithdrawlVector(wxVector<withdrawl_pair>&);
	void Read_pairVector(wxVector<pair>&);
	void WriteCashDivVector(wxVector<Pair>&);
	void ReadCashDivVector(wxVector<Pair>&);
	void ReadStringwxVector(wxVector<wxString>&);
	void WriteStringwxVector(wxVector<wxString>&);
	void WriteLongwxVector(wxVector<long>&);
	void ReadLongwxVector(wxVector<long>&);
	void WriteSectorDataVector(wxVector<SectorData>&);
	void ReadSectorDataVector(wxVector<SectorData>&);
	void WritePair(Pair&);
	void ReadPair(Pair&);
	void CommitMyFile();
private:
	void RetrieveChar(char*, int);
	wxTempFile* NewTempFile();
	void DeleteTempFile();
private:
	wxFile file;
	wxTempFile* tempfile = nullptr;
	bool ok = false;
	int mode = wxFile::read;
	bool commit = false;
};

template <typename T>
void DataStream::WriteData(T& t)
{
	if (!this->tempfile)
		return;
	this->tempfile->Write(reinterpret_cast<char*>(&t), sizeof(t));
}

template <typename T>
void DataStream::ReadData(T& t)
{
	file.Read(reinterpret_cast<char*>(&t), sizeof(t));
}

template <typename T>
void DataStream::WritewxVector(wxVector<T>& v)
{
	size_t length = v.size();
	this->WriteData(length);

	for (size_t i = 0; i < length; ++i)
		this->WriteData(v[i]);
}

template <typename T>
void DataStream::ReadwxVector(wxVector<T>& v)
{
	int length;
	this->ReadData(length);

	for (int i = 0; i < length; ++i)
	{
		T t;
		this->ReadData(t);
		v.push_back(t);
	}
}