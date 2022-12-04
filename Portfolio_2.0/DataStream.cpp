#include "DataStream.h"

void SetStaticTextColor(wxStaticText& T, wxColour green, wxColour red)
{
	wxString t = T.GetLabelText();
	if (t.find('-') != -1)
		T.SetForegroundColour(red);
	else
		T.SetForegroundColour(green);
}

bool IsStringPositive(wxString& s)
{
	int index = s.find('-');
	return index == -1 ? true : false;
}

int IsStringPNZ(wxString& s)
{
	double d;
	wxString temp = s;

	int index = temp.find('$');
	if (index != -1)
		temp = temp.Mid(1);

	index = temp.find('%');
	if (index != -1)
		temp = temp.Mid(0, index);

	if (!temp.ToDouble(&d))
	{
		wxMessageBox("Todouble failed in IsStringPNZ! String is: " + temp);
		return 0;
	}

	if (d > 0)
		return 1;
	if (d < 0)
		return -1;

	return 0;
}

bool IswxDateEqual(wxDateTime& d1, wxDateTime& d2)
{
	if (!d1.IsValid() || !d2.IsValid())
	{
		wxFAIL_MSG("wxDateTime passed to IswxDateTimeEqual is invalid date!");
		return false;
	}

	int year1 = d1.GetYear();
	int year2 = d2.GetYear();
	wxDateTime::Month m1 = d1.GetMonth();
	wxDateTime::Month m2 = d2.GetMonth();
	int day1 = d1.GetDay();
	int day2 = d2.GetDay();

	if (d1.GetYear() != d2.GetYear())
		return false;
	if (d1.GetMonth() != d2.GetMonth())
		return false;
	if (d1.GetDay() != d2.GetDay())
		return false;

	return true;
}

DataStream::DataStream(wxString filename, wxPosixPermissions per, wxFile::OpenMode op) : mode(op)
{
	if (op == wxFile::OpenMode::write || op == wxFile::OpenMode::write_append)
	{
		if (!this->file.Exists(filename))
		{
			if (!this->file.Create(filename, true, per))
			{
				wxMessageBox("Failed to creat file: " + filename);
				this->ok = false;
				return;
			}
			this->file.Close();
		}
		this->tempfile = this->NewTempFile();
		if (!this->tempfile->Open(filename))
		{
			wxMessageBox("Failed to open: " + filename + " by tempfile!");
			this->ok = false;
			this->DeleteTempFile();
			return;
		}

		this->ok = true;
		return;
	}

	if (this->file.Exists(filename))
	{
		if (!this->file.Open(filename, wxFile::read))
		{
			wxMessageBox("Failed to open: " + filename + " to read!");
			this->ok = false;
			return;
		}
	}
	else
	{
		wxMessageBox(filename + " does not exist! Nothing can be read from it!");
		this->ok = false;
		return;
	}

	this->ok = true;
}

DataStream::~DataStream()
{
	if (this->tempfile)
	{
		if (!this->commit)
			this->tempfile->Commit();
		this->DeleteTempFile();
	}
	else
		this->file.Close();
}

bool DataStream::IsOK()
{
	return this->ok;
}

wxFile& DataStream::GetFileHandle()
{
	return this->file;
}

void DataStream::WriteLong(long& l)
{
	int temp = l;
	this->WriteData(temp);
}

void DataStream::ReadLong(long& l)
{
	int temp;
	this->ReadData(temp);
	l = temp;
}

void DataStream::Write(wxString& str)
{
	if (!this->tempfile)
		return;
	wxScopedCharBuffer utf8 = str.utf8_str();
	size_t length = utf8.length();
	bool tmepfirst = tempfile->Write(reinterpret_cast<char*>(&length), sizeof(length));
	bool tmepsecond = tempfile->Write(utf8, utf8.length());
}

void DataStream::Read(wxString& str)
{
	int length;
	this->ReadData(length);
	char* s = new char[length + 1];
	s[length] = '\0';
	this->RetrieveChar(s, length);
	str << s;
	delete[] s;
}

void DataStream::ReadwxDateTime(wxDateTime& d)
{
	wxString format;
	this->Read(format);

	if (!d.ParseDate(format))
		wxFAIL_MSG("ParseFormat failed in DataStream::ReadwxDateTime! value was: " + format);
}

void DataStream::WritewxDateTime(wxDateTime& d)
{
	if (!d.IsValid())
	{
		wxFAIL_MSG("wxDateTime in DataStream::Read is invalid datetime!");
		return;
	}

	wxString format = d.Format("%m/%d/%G");
	this->Write(format);
}

void DataStream::ReadTimeTm(wxDateTime::Tm& tm)
{
	int year;
	int month;
	int yday;
	int day;
	int hour;
	int min;
	int sec;
	int msec;

	this->ReadData(year);
	this->ReadData(month);
	this->ReadData(yday);
	this->ReadData(day);
	this->ReadData(hour);
	this->ReadData(min);
	this->ReadData(sec);
	this->ReadData(msec);

	tm.year = year;
	tm.mon = wxDateTime::Month(month);
	tm.yday = yday;
	tm.mday = day;
	tm.hour = hour;
	tm.min = min;
	tm.sec = sec;
	tm.msec = msec;
}

void DataStream::WriteTimeTm(wxDateTime::Tm& tm)
{
	int year = tm.year;
	int month = tm.mon;
	int yday = tm.yday;
	int day = tm.mday;
	int hour = tm.hour;
	int min = tm.min;
	int sec = tm.sec;
	int msec = tm.msec;

	this->WriteData(year);
	this->WriteData(month);
	this->WriteData(yday);
	this->WriteData(day);
	this->WriteData(hour);
	this->WriteData(min);
	this->WriteData(sec);
	this->WriteData(msec);
}

void DataStream::WriteBool(bool& b)
{
	int i = b ? 1 : 0;
	this->WriteData(i);
}

void DataStream::ReadBool(bool& b)
{
	int i;
	this->ReadData(i);
	b = i;
}

void DataStream::WriteDividendVector(wxVector<Dividend>& v)
{
	size_t size = v.size();
	this->WriteData(size);
	size_t valid = 0;
	for (size_t i = 0; i < v.size(); ++i)
	{
		this->WritewxDateTime(v[i].ex_Div);
		valid = v[i].payment_date.IsValid() ? 1 : 0;
		this->WriteData(valid);
		if (valid)
			this->WritewxDateTime(v[i].payment_date);
		this->WriteLong(v[i].id);
		this->WriteData(v[i].div);
		this->WriteBool(v[i].DivReinvest);
		this->WriteData(v[i].re_invest_shares);
		this->WriteBool(v[i].linked);
	}
}

void DataStream::ReadDividendVector(wxVector<Dividend>& v)
{
	size_t size;
	this->ReadData(size);
	size_t valid = 0;
	for (size_t i = 0; i < size; ++i)
	{
		Dividend d(0, wxDateTime(), 0.0);
		this->ReadwxDateTime(d.ex_Div);
		this->ReadData(valid);
		if (valid)
			this->ReadwxDateTime(d.payment_date);
		this->ReadLong(d.id);
		this->ReadData(d.div);
		this->ReadBool(d.DivReinvest);
		this->ReadData(d.re_invest_shares);
		this->ReadBool(d.linked);
		v.push_back(d);
	}
}

void DataStream::WriteSummaryData(SummaryData& sd)
{
	wxDateTime::Tm tm = sd.time.GetTm();
	this->WritewxDateTime(sd.date);
	this->WriteTimeTm(tm);
	this->Write(sd.earningsdate);
	this->Write(sd.Longname);
	this->WriteData(sd.Beta);
	this->WriteData(sd.marketprice);
	this->WriteData(sd.Open);
	this->WriteData(sd.previousclose);
}

void DataStream::ReadSummaryData(SummaryData& sd)
{
	wxString temp = "";
	wxDateTime::Tm tm;
	this->ReadwxDateTime(sd.date);
	this->ReadTimeTm(tm);
	this->Read(temp);
	if (temp != NA)
		sd.earningsdate = temp;
	temp = "";
	this->Read(temp);
	if (temp != NA)
		sd.Longname = temp;
	this->ReadData(sd.Beta);
	this->ReadData(sd.marketprice);
	this->ReadData(sd.Open);
	this->ReadData(sd.previousclose);
	sd.time = wxDateTime(tm);
}

void DataStream::WriteDayPrices(Day_Prices& ds)
{
	this->WritewxDateTime(ds.date);
	this->WriteData(ds.open);
	this->WriteData(ds.high);
	this->WriteData(ds.low);
	this->WriteData(ds.close);
}

void DataStream::ReadDayPrices(Day_Prices& ds)
{
	this->ReadwxDateTime(ds.date);
	this->ReadData(ds.open);
	this->ReadData(ds.high);
	this->ReadData(ds.low);
	this->ReadData(ds.close);
	double d = 180.66999799999999;
	double dd = 426.97000100000002;
	if (ds.close == d)
		ds.close = 131.67;
	if (ds.close == dd)
		ds.close = 377.97;
}

void DataStream::WriteDayPricesVector(wxVector<Day_Prices>& v)
{
	size_t size = v.size();
	this->WriteData(size);
	for (size_t i = 0; i < size; ++i)
		this->WriteDayPrices(v[i]);
}

void DataStream::ReadDayPricesVector(wxVector<Day_Prices>& v)
{
	size_t size;
	this->ReadData(size);
	for (size_t i = 0; i < size; ++i)
	{
		v.push_back(Day_Prices(0.0, 0.0, 0.0, 0.0, wxDateTime(wxInvalidDateTime)));
		this->ReadDayPrices(v[v.size() - 1]);
	}
}

void DataStream::WriteDepositVector(wxVector<deposit_pair>& v)
{
	size_t size = v.size();
	this->WriteData(size);
	for (size_t i = 0; i < v.size(); ++i)
	{
		this->WritewxDateTime(v[i].T);
		this->WriteData(v[i].value);
	}
}

void DataStream::WriteWithdrawlVector(wxVector<withdrawl_pair>& v)
{
	size_t size = v.size();
	this->WriteData(size);
	for (size_t i = 0; i < v.size(); ++i)
	{
		this->WritewxDateTime(v[i].date);
		this->WriteData(v[i].value);
	}
}

void DataStream::Write_pairVector(wxVector<pair>& v)
{
	size_t size = v.size();
	this->WriteData(size);
	for (size_t i = 0; i < v.size(); ++i)
	{
		this->WritewxDateTime(v[i].date);
		this->WriteData(v[i].value);
		this->WriteLong(v[i].id);
	}
}

void DataStream::ReadDepositVector(wxVector<deposit_pair>& v)
{
	size_t size;
	this->ReadData(size);
	for (size_t i = 0; i < size; ++i)
	{
		deposit_pair dp(wxDateTime(wxInvalidDateTime), 0.0);
		this->ReadwxDateTime(dp.T);
		this->ReadData(dp.value);
		v.push_back(dp);
	}
}

void DataStream::ReadWithdrawlVector(wxVector<withdrawl_pair>& v)
{
	size_t size;
	this->ReadData(size);
	for (size_t i = 0; i < size; ++i)
	{
		withdrawl_pair dp(wxDateTime(wxInvalidDateTime), 0.0);
		this->ReadwxDateTime(dp.date);
		this->ReadData(dp.value);
		v.push_back(dp);
	}
}

void DataStream::Read_pairVector(wxVector<pair>& v)
{
	size_t size;
	this->ReadData(size);
	for (size_t i = 0; i < size; ++i)
	{
		pair p(0, 0.0, wxDateTime());
		this->ReadwxDateTime(p.date);
		this->ReadData(p.value);
		this->ReadLong(p.id);
		v.push_back(p);
	}
}

void DataStream::WriteCashDivVector(wxVector<Pair>& v)
{
	size_t size = v.size();
	this->WriteData(size);
	for (size_t i = 0; i < size; ++i)
		this->WritePair(v[i]);
}

void DataStream::ReadCashDivVector(wxVector<Pair>& v)
{
	size_t size;
	this->ReadData(size);
	for (size_t i = 0; i < size; ++i)
	{
		Pair p("", 0.0);
		this->ReadPair(p);
		v.push_back(p);
	}
}

void DataStream::ReadStringwxVector(wxVector<wxString>& v)
{
	size_t size;
	this->ReadData(size);
	for (size_t i = 0; i < size; ++i)
	{
		v.push_back(wxString());
		this->Read(v[v.size() - 1]);
	}
}

void DataStream::WriteStringwxVector(wxVector<wxString>& v)
{
	size_t size = v.size();
	this->WriteData(size);
	for (size_t i = 0; i < size; ++i)
		this->Write(v[i]);
}

void DataStream::WriteLongwxVector(wxVector<long>& v)
{
	size_t size = v.size();
	this->WriteData(size);
	for (size_t i = 0; i < size; ++i)
		this->WriteLong(v[i]);
}

void DataStream::ReadLongwxVector(wxVector<long>& v)
{
	size_t size;
	this->ReadData(size);
	for (size_t i = 0; i < size; ++i)
	{
		v.push_back(long());
		this->ReadLong(v[v.size() - 1]);
	}
}

void DataStream::WriteSectorDataVector(wxVector<SectorData>& v)
{
	size_t size = v.size();
	this->WriteData(size);
	for (auto& i : v)
	{
		this->WriteData(i.price);
		this->WriteData(i.day_return);
		this->WriteData(i.week_return);
		this->WriteData(i.month_return);
		this->WriteData(i.quarter_return);
		this->WriteData(i.half_year_return);
		this->WriteData(i.YTD_return);
		this->WriteData(i.year_return);
		this->WriteData(i.market_cap);
		wxDateTime::Tm tm = i.date.GetTm();
		this->WriteTimeTm(tm);
	}
}

void DataStream::ReadSectorDataVector(wxVector<SectorData>& v)
{
	size_t size;
	this->ReadData(size);
	for (size_t i = 0; i < size; ++i)
	{
		v.push_back(SectorData());
		auto& it = *v.rbegin();

		this->ReadData(it.price);
		this->ReadData(it.day_return);
		this->ReadData(it.week_return);
		this->ReadData(it.month_return);
		this->ReadData(it.quarter_return);
		this->ReadData(it.half_year_return);
		this->ReadData(it.YTD_return);
		this->ReadData(it.year_return);
		this->ReadData(it.market_cap);
		wxDateTime::Tm tm;
		this->ReadTimeTm(tm);
		it.date = wxDateTime(tm);
	}
}

void DataStream::WritePair(Pair& p)
{
	this->Write(p.ticker);
	this->WriteData(p.amount);
}

void DataStream::ReadPair(Pair& p)
{
	this->Read(p.ticker);
	this->ReadData(p.amount);
}

void DataStream::CommitMyFile()
{
	if (!this->tempfile)
		return;
	this->tempfile->Commit();
	this->commit = true;
}

wxTempFile* DataStream::NewTempFile()
{
	return new wxTempFile();
}

void DataStream::DeleteTempFile()
{
	if (this->tempfile)
		delete this->tempfile;

	tempfile = nullptr;
}

void DataStream::RetrieveChar(char* c, int length)
{
	ssize_t size = file.Read(c, sizeof(char) * length);
	if (wxInvalidOffset == size)
		int Catch = 0;
}