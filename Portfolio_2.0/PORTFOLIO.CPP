#include "Portfolio.h"

inline wxDateTime IsValidDate(wxString& date, wxString msg)
{
	wxDateTime temp;
	if (!temp.ParseDate(date))
	{
		wxFAIL_MSG(msg);
		return wxDateTime(wxInvalidDateTime);
	}
	if (wxDateTime::Today() < temp)
	{
		wxFAIL_MSG(msg);
		return wxDateTime(wxInvalidDateTime);
	}

	return temp;
}

unsigned int inline GetEndMonthDay(wxDateTime::Month m, int year)
{
	wxDateTime LastDay(1, m, year);
	int lastworkday;

	switch (m) {
	case 1: {
		if (LastDay.IsLeapYear())
			lastworkday = 29;
		else
			lastworkday = 28;
		break;
	}
	case 3:
	case 5:
	case 8:
	case 10: lastworkday = 30;
		break;
	default: lastworkday = 31;
	}

	return lastworkday;
}

wxDateTime::Month inline GetQuarterStartMonth(wxDateTime::Month m)
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
/*
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
*/
/*
wxString inline GetSectorName(_Sector sect)
{
	switch (sect)
	{
	case _Sector::BOND: return "Bonds";
	case _Sector::LARGECAP: return "Large Cap";
	case _Sector::MIDCAP: return "Mid Cap";
	case _Sector::SMALLCAP: return "Small Cap";
	case _Sector::FOREIGN: return "Foreign";
	case _Sector::TECH: return "Tech";
	case _Sector::REIT: return "REIT";
	case _Sector::CRYPTO: return "CRYPTO";
	default: return "";
	}
}
*/

// ItemInfo functions...
ItemInfo::ItemInfo(double& marketcap, double& daygain) : market_cap(marketcap), day_return(daygain)
{

}

ItemInfo::ItemInfo(double& marketcap, double& daygain, SectorPerformance& sp) : market_cap(marketcap), day_return(daygain)
{
	this->_SetPerformance(sp);
	this->AddDataToVec();
}

void ItemInfo::_SetAllData(double m_cap, double daygain, SectorPerformance& sp)
{
	this->_SetOverViewData(m_cap, daygain);
	this->_SetPerformance(sp);
	this->AddDataToVec();
}

void ItemInfo::_SetOverViewData(double m_cap, double daygain)
{
	this->market_cap = m_cap;
	this->day_return = daygain;
}

void ItemInfo::_SetPerformance(SectorPerformance& sp)
{
	this->week_return = sp.week;
	this->month_return = sp.month;
	this->quarter_return = sp.quarter;
	this->half_year_return = sp.half;
	this->year_return = sp.year;
	this->YTD_return = sp.yearToDate;
	this->date = sp.stamp;
}

void ItemInfo::_SetPerformance(SectorPerformance& sp, double& day, double& m_cap, double& m_price)
{
	this->week_return = sp.week;
	this->month_return = sp.month;
	this->quarter_return = sp.quarter;
	this->half_year_return = sp.half;
	this->year_return = sp.year;
	this->YTD_return = sp.yearToDate;
	this->date = sp.stamp;
	this->day_return = day;
	this->market_cap = m_cap;
	this->price = m_price;
}

void ItemInfo::_SetTimeStamp(wxDateTime& d)
{
	this->date = d;
}

void ItemInfo::_Write(DataStream& ds)
{
	ds.WriteSectorDataVector(this->vec);
}

void ItemInfo::_Read(DataStream& ds)
{
	ds.ReadSectorDataVector(this->vec);
	if (this->vec.rbegin() != this->vec.rend())
		this->Populate(*this->vec.rbegin());
}

bool ItemInfo::IsUpToDate()
{
	wxDateTime today = wxDateTime::Now();
	if (this->vec.rbegin() == this->vec.rend())
		return false;

	wxDateTime lastpull = this->vec.rbegin()->date;
	if (!lastpull.IsValid())
	{
		wxFAIL_MSG("lastpull in ItemInfo::IsUpToDate is invalid wxDateTime!");
		this->vec.erase(&*this->vec.rbegin());
		return false;
	}

	if (!IswxDateEqual(today, lastpull))
		return false;

	if ((_MarketClose.GetValue() - lastpull.GetValue()) >= 0)
		return false;

	return true;
}

void ItemInfo::Populate(SectorData& sd)
{
	this->price = sd.price;
	this->day_return = sd.day_return;
	this->week_return = sd.week_return;
	this->month_return = sd.month_return;
	this->quarter_return = sd.quarter_return;
	this->half_year_return = sd.half_year_return;
	this->year_return = sd.year_return;
	this->YTD_return = sd.YTD_return;
	this->date = sd.date;
}

void ItemInfo::AddDataToVec()
{
	if (!this->vec.size())
	{
		this->PushBack();
		return;
	}

	SectorData* data = this->IsDuplicateDate();
	if (data)
	{
		if (data->date < this->date)
			this->Replace(data);
	}
	else
		this->PushBack();
}

SectorData* ItemInfo::IsDuplicateDate()
{
	for (auto& v : this->vec)
	{
		if (IswxDateEqual(v.date, this->date))
			return &v;
	}

	return NULL;
}

void ItemInfo::Replace(SectorData* sd)
{
	*sd = SectorData(this->price, this->day_return, this->week_return, this->month_return,
			this->quarter_return, this->half_year_return, this->YTD_return, this->year_return, this->market_cap, this->date);
}

void ItemInfo::PushBack()
{
	this->vec.push_back(SectorData(this->price, this->day_return, this->week_return, this->month_return,
		this->quarter_return, this->half_year_return, this->YTD_return, this->year_return, this->market_cap, this->date));
}

// SubSector class functions...
SubSector::SubSector(ParentSector* p) : ItemInfo(), m_parent(p)
{

}

SubSector::SubSector(ParentSector* parent, DataStream& ds) : ItemInfo(), m_parent(parent)
{
	this->Read(ds);
}

SubSector::SubSector(ParentSector* p, _Sub_Sector ss, wxString& name, double& m_cap, double& daygain) : ItemInfo(m_cap, daygain),
	m_parent(p), ID(ss), sectorname(name)
{

}

SubSector::SubSector(ParentSector* p, _Sub_Sector ss, wxString& name, double& m_cap, double& day_gain, SectorPerformance& sp) 
	: ItemInfo(m_cap, day_gain, sp), m_parent(p), ID(ss), sectorname(name)
{

}

void SubSector::SetAllData(wxString& _name, double& m_cap, double& daygain, SectorPerformance& sp)
{
	this->sectorname = _name;
	this->_SetAllData(m_cap, daygain, sp);
}

void SubSector::SetOverViewData(wxString& _name, double& m_cap, double& daygain)
{
	this->sectorname = _name;
	this->_SetOverViewData(m_cap, daygain);
}

void SubSector::SetPerformance(SectorPerformance& sp)
{
	this->_SetPerformance(sp);
}

void SubSector::SetParent(ParentSector* p)
{
	this->m_parent = p;
}

_Sub_Sector SubSector::GetSubSecID()
{
	return this->ID;
}

void SubSector::Read(DataStream& ds)
{
	ds.Read(this->sectorname);
	ds.ReadData(this->ID);
	this->_Read(ds);
}

void SubSector::Write(DataStream& ds)
{
	ds.Write(this->sectorname);
	ds.WriteData(ID);
	this->_Write(ds);
}

// ParentSector class functions...
ParentSector::ParentSector(SectorClass* parent) : m_parent(parent)
{

}

ParentSector::ParentSector(SectorClass* parent, DataStream& ds) : m_parent(parent)
{
	this->Read(ds);
}

ParentSector::ParentSector(SectorClass* parent, _Sector secid, wxString URL, wxString name) : 
	m_parent(parent), ID(secid), url(URL), sectorName(name)
{
	this->url.Replace(_PERFORMANCE, _INSERTION);
	this->m_GetSubSectorData();
}

void ParentSector::SetAllData(wxString& _name, double& m_cap, double& change, SectorPerformance& sp)
{
	this->sectorName = _name;
	this->_SetAllData(m_cap, change, sp);
}

bool ParentSector::SetSubSectorOverviewData(_Sub_Sector s, wxString& _name, double& m_cap, double& daygain)
{
	for (size_t i = 0; i < this->subsecs.size(); ++i)
	{
		if (subsecs[i].GetSubSecID() == s)
		{
			subsecs[i].SetOverViewData(_name, m_cap, daygain);
			return true;
		}
	}

	return false;
}

void ParentSector::SetSubSectorPerformance(SectorPerformance& sp)
{
	wxDateTime stamp = wxDateTime::Today();
	for (size_t i = 0; i < this->subsecs.size(); ++i)
	{
		if (this->subsecs[i].GetSubSecID() == this->m_parent->GetSubSectorID(this->ID, sp.name))
		{
			this->subsecs[i].SetPerformance(sp);
			return;
		}
	}

	wxFAIL_MSG("ParentSector::SetSubSectorPerformance failed in finding a match in subsecs");
}

void ParentSector::SetSubSecData(wxString& name, double& m_cap, double& daychange, SectorPerformance& sp)
{
	for (auto& v : this->subsecs)
	{
		if (v.GetSubSecID() == this->m_parent->GetSubSectorID(this->ID, sp.name))
		{
			v.SetAllData(name, m_cap, daychange, sp);
			return;
		}
	}

	wxFAIL_MSG("ParentSector::SetSubSecData failed in finding a match in subsecs");
}

_Sector ParentSector::GetSectorId()
{
	return this->ID;
}

void ParentSector::Read(DataStream& ds)
{
	ds.Read(this->sectorName);
	ds.Read(this->url);
	ds.ReadData(this->ID);
	this->_Read(ds);

	size_t size;
	ds.ReadData(size);
	for (size_t i = 0; i < size; ++i)
	{
		this->subsecs.push_back(SubSector(this));
		this->subsecs.rbegin()->Read(ds);
	}	
}

void ParentSector::Write(DataStream& ds)
{
	ds.Write(this->sectorName);
	ds.Write(this->url);
	ds.WriteData(this->ID);
	this->_Write(ds);

	size_t size = this->subsecs.size();
	ds.WriteData(size);
	for (auto& v : this->subsecs)
		v.Write(ds);
}

void ParentSector::m_GetSubSectorData()
{
	wxString temp = this->subsecurl;
	wxString empty = "";
	temp.Replace(_INSERTION, this->m_GetNameToInsert());
	temp.Replace(_INSERT_TYPE, _OVERVIEW);
	Parser parser(empty);
	wxVector<SectorOverview> v;
	parser.PullFinVizOverview(temp, v);

	temp = this->subsecurl;
	temp.Replace(_INSERTION, this->m_GetNameToInsert());
	temp.Replace(_INSERT_TYPE, _PERFORMANCE);
	wxVector<SectorPerformance> v2;
	parser.PullFinVizPerformance(temp, v2);

	for (size_t i = 0; i < v.size(); ++i)
		this->subsecs.push_back(SubSector(this, this->m_parent->GetSubSectorID(this->ID, v[i].name), v[i].name, v[i].market_cap, v[i].change, v2[i]));
}

void ParentSector::m_GetSubSectorPerformance()
{
	wxString temp = this->subsecurl;
	wxString empty = "";
	temp.Replace(_INSERTION, this->m_GetNameToInsert());
	temp.Replace(_INSERT_TYPE, _PERFORMANCE);
	Parser parser(empty);
	wxVector<SectorPerformance> v;
	parser.PullFinVizPerformance(temp, v);

	for (size_t i = 0; i < v.size(); ++i)
		this->SetSubSectorPerformance(v[i]);
}

wxString ParentSector::m_GetNameToInsert()
{
	switch (this->ID)
	{
	case _Sector::COMMUNICATION_SERVICES: return _cOMMUNICATION_SERVICES;
	case _Sector::CONSUMER_DESCRETIONARY: return _cONSUMER_DESCRETIONARY;
	case _Sector::CONSUMER_STAPLES: return _cONSUMER_STAPLES;
	case _Sector::ENERGY: return _eNERGY;
	case _Sector::FINANCIALS: return _fINANCIAL;
	case _Sector::HEALTH_CARE: return _hEALTH_CARE;
	case _Sector::INDUSTRIALS: return _iNDUSTRIALS;
	case _Sector::INFORMATION_TECHNOLOGY: return _tECHNOLOGY;
	case _Sector::UTILITIES: return _uTILITIES;
	case _Sector::MATERIALS: return _bASIC_MATERIALS;
	case _Sector::REAL_ESTATE: return _rEAL_ESTATE;
	default:
	{
		wxMessageBox("Sector id does not match anything in switch statement in SubSector::m_GetNameToInsert");
		return "";
	}
	}
}

// SectorClass functions...
SectorClass::SectorClass()
{

}

void SectorClass::ActivateData()
{
	this->sec.push_back(BOND);
	this->_sec.push_back(_BONDS);
//	this->sec.push_back(REIT);
//	this->_sec.push_back(_REIT);
	this->sec.push_back(SMALLCAP);
	this->_sec.push_back(_SMALLCAP);
	this->sec.push_back(MIDCAP);
	this->_sec.push_back(_MIDCAP);
	this->sec.push_back(LARGECAP);
	this->_sec.push_back(_LARGECAP);
//	this->sec.push_back(TECH);
//	this->_sec.push_back(_TECH);
	this->sec.push_back(CRYPTO);
	this->_sec.push_back(_CRYPTO);
	this->sec.push_back(FOREIGN);
	this->_sec.push_back(_FOREIGN);
	this->sec.push_back(COMMUNICATION_SERVICES);
	this->_sec.push_back(_COMMUNICATION_SERVICES);
	this->sec.push_back(CONSUMER_DESCRETIONARY);
	this->_sec.push_back(_CONSUMER_DESCRETIONARY_ALTERNATE);
	this->sec.push_back(CONSUMER_STAPLES);
	this->_sec.push_back(_CONSUMER_STAPLES_ALTERNATE);
	this->sec.push_back(ENERGY);
	this->_sec.push_back(_ENERGY);
	this->sec.push_back(FINANCIALS);
	this->_sec.push_back(_FINANCIALS);
	this->sec.push_back(HEALTH_CARE);
	this->_sec.push_back(_HEALTH_CARE);
	this->sec.push_back(INDUSTRIALS);
	this->_sec.push_back(_INDUSTRIALS);
	this->sec.push_back(INFORMATION_TECHNOLOGY);
	this->_sec.push_back(_INFORMATION_TECHNOLOGY);
	this->sec.push_back(MATERIALS);
	this->_sec.push_back(_MATERIALS);
	this->sec.push_back(REAL_ESTATE);
	this->_sec.push_back(_REAL_ESTATE);
	this->sec.push_back(UTILITIES);
	this->_sec.push_back(_UTILITIES);
	this->sec.push_back(SECTOR_INVALID);
	this->_sec.push_back(_SECTOR_INVALID);

	//NEW
	this->communication_services.push_back(ETF);
	this->_communication_services.push_back(_ETF);

	this->communication_services.push_back(ADVERTISING_AGENCIES);
	this->_communication_services.push_back(_ADVERTISING_AGENCIES);
	this->communication_services.push_back(BRODCASTING);
	this->_communication_services.push_back(_BRODCASTING);
	this->communication_services.push_back(ELECTRONIC_GAMING_AND_MULTIMEDIA);
	this->_communication_services.push_back(_ELECTRONIC_GAMING_AND_MULTIMEDIA);
	this->communication_services.push_back(ENTERTAINMENT);
	this->_communication_services.push_back(_ENTERTAINMENT);
	this->communication_services.push_back(INTERNET_CONTENT_AND_CREATION);
	this->_communication_services.push_back(_INTERNET_CONTENT_AND_CREATION);
	this->communication_services.push_back(PUBLISHING);
	this->_communication_services.push_back(_PUBLISHING);
	this->communication_services.push_back(TELECOM_SERVICES);
	this->_communication_services.push_back(_TELECOM_SERVICES);

	//NEW
	this->consumer_discretionary.push_back(ETF);
	this->_consumer_discretionary.push_back(_ETF);

	this->consumer_discretionary.push_back(APPAREL_MANUFACTURING);
	this->_consumer_discretionary.push_back(_APPAREL_MANUFACTURING);
	this->consumer_discretionary.push_back(APPAREL_RETAIL);
	this->_consumer_discretionary.push_back(_APPAREL_RETAIL);
	this->consumer_discretionary.push_back(AUTO_TRUCK_AND_DEALERSHIPS);
	this->_consumer_discretionary.push_back(_AUTO_TRUCK_AND_DEALERSHIPS);
	this->consumer_discretionary.push_back(AUTO_MANUFACTURERS);
	this->_consumer_discretionary.push_back(_AUTO_MANUFACTURERS);
	this->consumer_discretionary.push_back(AUTO_PARTS);
	this->_consumer_discretionary.push_back(_AUTO_PARTS);
	this->consumer_discretionary.push_back(DEPARTMENT_STORES);
	this->_consumer_discretionary.push_back(_DEPARTMENT_STORES);
	this->consumer_discretionary.push_back(FOOTWEAR_AND_ACCESSORIES);
	this->_consumer_discretionary.push_back(_FOOTWEAR_AND_ACCESSORIES);
	this->consumer_discretionary.push_back(FURNISHIN_FIXTURES_AND_APPLIANCES);
	this->_consumer_discretionary.push_back(_FURNISHIN_FIXTURES_AND_APPLIANCES);
	this->consumer_discretionary.push_back(GAMBLING);
	this->_consumer_discretionary.push_back(_GAMBLING);
	this->consumer_discretionary.push_back(HOME_IMPROVMENT_RETAIL);
	this->_consumer_discretionary.push_back(_HOME_IMPROVMENT_RETAIL);
	this->consumer_discretionary.push_back(INTERNET_RETAIL);
	this->_consumer_discretionary.push_back(_INTERNET_RETAIL);
	this->consumer_discretionary.push_back(LEISURE);
	this->_consumer_discretionary.push_back(_LEISURE);
	this->consumer_discretionary.push_back(LODGING);
	this->_consumer_discretionary.push_back(_LODGING);
	this->consumer_discretionary.push_back(LUXURY_GOODS);
	this->_consumer_discretionary.push_back(_LUXURY_GOODS);
	this->consumer_discretionary.push_back(PACKAGING_AND_CONTAINERS);
	this->_consumer_discretionary.push_back(_PACKAGING_AND_CONTAINERS);
	this->consumer_discretionary.push_back(PERSONAL_SERVICES);
	this->_consumer_discretionary.push_back(_PERSONAL_SERVICES);
	this->consumer_discretionary.push_back(RECREATIONAL_VEHICLES);
	this->_consumer_discretionary.push_back(_RECREATIONAL_VEHICLES);
	this->consumer_discretionary.push_back(RESIDENTIAL_CONSTRUCTION);
	this->_consumer_discretionary.push_back(_RESIDENTIAL_CONSTRUCTION);
	this->consumer_discretionary.push_back(RESORTS_AND_CASINOS);
	this->_consumer_discretionary.push_back(_RESORTS_AND_CASINOS);
	this->consumer_discretionary.push_back(RESTAURANTS);
	this->_consumer_discretionary.push_back(_RESTAURANTS);
	this->consumer_discretionary.push_back(SPECIALTY_RETAIL);
	this->_consumer_discretionary.push_back(_SPECIALTY_RETAIL);
	this->consumer_discretionary.push_back(TEXTILE_MANUFACTURING);
	this->_consumer_discretionary.push_back(_TEXTILE_MANUFACTURING);
	this->consumer_discretionary.push_back(TRAVEL_SERVICES);
	this->_consumer_discretionary.push_back(_TRAVEL_SERVICES);

	//NEW
	this->consumer_staples.push_back(ETF);
	this->_consumer_staples.push_back(_ETF);

	this->consumer_staples.push_back(BEVERAGES_BREWERS);
	this->_consumer_staples.push_back(_BEVERAGES_BREWERS);
	this->consumer_staples.push_back(BEVERAGES_NON_ALCOHOLIC);
	this->_consumer_staples.push_back(_BEVERAGES_NON_ALCOHOLIC);
	this->consumer_staples.push_back(BEVERAGES_WINERIES_AND_DESTILLERIES);
	this->_consumer_staples.push_back(_BEVERAGES_WINERIES_AND_DESTILLERIES);
	this->consumer_staples.push_back(CONFECTIONERS);
	this->_consumer_staples.push_back(_CONFECTIONERS);
	this->consumer_staples.push_back(DISCOUNT_STORES);
	this->_consumer_staples.push_back(_DISCOUNT_STORES);
	this->consumer_staples.push_back(EDUCATION_AND_TRAINING_SERVICES);
	this->_consumer_staples.push_back(_EDUCATION_AND_TRAINING_SERVICES);
	this->consumer_staples.push_back(FARM_PRODUCTS);
	this->_consumer_staples.push_back(_FARM_PRODUCTS);
	this->consumer_staples.push_back(FOOD_DISTRIBUTION);
	this->_consumer_staples.push_back(_FOOD_DISTRIBUTION);
	this->consumer_staples.push_back(GROCERY_STORES);
	this->_consumer_staples.push_back(_GROCERY_STORES);
	this->consumer_staples.push_back(HOUSEHOLD_AND_PERSONAL_PRODUCTS);
	this->_consumer_staples.push_back(_HOUSEHOLD_AND_PERSONAL_PRODUCTS);
	this->consumer_staples.push_back(PACKEDGED_FOODS);
	this->_consumer_staples.push_back(_PACKEDGED_FOODS);
	this->consumer_staples.push_back(TOBACCO);
	this->_consumer_staples.push_back(_TOBACCO);

	//NEW
	this->energy.push_back(ETF);
	this->_energy.push_back(_ETF);

	this->energy.push_back(OIL_AND_GAS_DRILLING);
	this->_energy.push_back(_OIL_AND_GAS_DRILLING);
	this->energy.push_back(OIL_AND_GAS_E_AND_P);
	this->_energy.push_back(_OIL_AND_GAS_E_AND_P);
	this->energy.push_back(OIL_AND_GAS_EQUIPMENT_AND_SERVICES);
	this->_energy.push_back(_OIL_AND_GAS_EQUIPMENT_AND_SERVICES);
	this->energy.push_back(OIL_AND_GAS_INTEGRATED);
	this->_energy.push_back(_OIL_AND_GAS_INTEGRATED);
	this->energy.push_back(OIL_AND_GAS_MIDSTREAM);
	this->_energy.push_back(_OIL_AND_GAS_MIDSTREAM);
	this->energy.push_back(OIL_AND_GAS_REFINING_AND_MARKETING);
	this->_energy.push_back(_OIL_AND_GAS_REFINING_AND_MARKETING);
	this->energy.push_back(THERMAL_COAL);
	this->_energy.push_back(_THERMAL_COAL);
	this->energy.push_back(URANIUM);
	this->_energy.push_back(_URANIUM);

	//NEW
	this->financials.push_back(ETF);
	this->_financials.push_back(_ETF);

	this->financials.push_back(ASSET_MANAGEMENT);
	this->_financials.push_back(_ASSET_MANAGEMENT);
	this->financials.push_back(BANKS_DIVERSIFIED);
	this->_financials.push_back(_BANKS_DIVERSIFIED);
	this->financials.push_back(BANKS_REGIONAL);
	this->_financials.push_back(_BANKS_REGIONAL);
	this->financials.push_back(CAPITAL_MARKETS);
	this->_financials.push_back(_CAPITAL_MARKETS);
	this->financials.push_back(CREDIT_SERVICES);
	this->_financials.push_back(_CREDIT_SERVICES);
	this->financials.push_back(FINANCIAL_CONGLOMERATES);
	this->_financials.push_back(_FINANCIAL_CONGLOMERATES);
	this->financials.push_back(FINANCIAL_DATA_AND_STOCK_EXCHANGES);
	this->_financials.push_back(_FINANCIAL_DATA_AND_STOCK_EXCHANGES);
	this->financials.push_back(INSURANCE_DIVERSIFIED);
	this->_financials.push_back(_INSURANCE_DIVERSIFIED);
	this->financials.push_back(INSURANCE_LIFE);
	this->_financials.push_back(_INSURANCE_LIFE);
	this->financials.push_back(INSURANCE_PROPERTY_AND_CASUALTY);
	this->_financials.push_back(_INSURANCE_PROPERTY_AND_CASUALTY);
	this->financials.push_back(INSURANCE_REINSURANCE);
	this->_financials.push_back(_INSURANCE_REINSURANCE);
	this->financials.push_back(INSURANCE_SPECIALTY);
	this->_financials.push_back(_INSURANCE_SPECIALTY);
	this->financials.push_back(INSURANCE_BROKERS);
	this->_financials.push_back(_INSURANCE_BROKERS);
	this->financials.push_back(MORTGAGE_FINANCE);
	this->_financials.push_back(_MORTGAGE_FINANCE);
	this->financials.push_back(SHELL_COMPANIES);
	this->_financials.push_back(_SHELL_COMPANIES);

	//NEW
	this->health_care.push_back(ETF);
	this->_health_care.push_back(_ETF);

	this->health_care.push_back(BIOTECHNOLOGY);
	this->_health_care.push_back(_BIOTECHNOLOGY);
	this->health_care.push_back(DIAGNOSTICS_AND_RESEARCH);
	this->_health_care.push_back(_DIAGNOSTICS_AND_RESEARCH);
	this->health_care.push_back(DRUG_MANUFACTURES_GENERAL);
	this->_health_care.push_back(_DRUG_MANUFACTURES_GENERAL);
	this->health_care.push_back(DRUG_MANUFACTURES_SPECIALTY_AND_GENERIC);
	this->_health_care.push_back(_DRUG_MANUFACTURES_SPECIALTY_AND_GENERIC);
	this->health_care.push_back(HEALTH_INFORMATION_AND_SERVICES);
	this->_health_care.push_back(_HEALTH_INFORMATION_AND_SERVICES);
	this->health_care.push_back(HEALTH_CARE_PLANS);
	this->_health_care.push_back(_HEALTH_CARE_PLANS);
	this->health_care.push_back(MEDICAL_CARE_FACILITIES);
	this->_health_care.push_back(_MEDICAL_CARE_FACILITIES);
	this->health_care.push_back(MEDICAL_DEVICES);
	this->_health_care.push_back(_MEDICAL_DEVICES);
	this->health_care.push_back(MEDICAL_DISTRIBUTION);
	this->_health_care.push_back(_MEDICAL_DISTRIBUTION);
	this->health_care.push_back(MEDICAL_INSTRUMENTS_AND_SUPPLIES);
	this->_health_care.push_back(_MEDICAL_INSTRUMENTS_AND_SUPPLIES);
	this->health_care.push_back(PHARMECUTICALS_RETAILERS);
	this->_health_care.push_back(_PHARMECUTICALS_RETAILERS);

	//NEW
	this->industrials.push_back(ETF);
	this->_industrials.push_back(_ETF);

	this->industrials.push_back(AEROSPACE_AND_DEFENSE);
	this->_industrials.push_back(_AEROSPACE_AND_DEFENSE);
	this->industrials.push_back(AIRPORTS_AND_SERVICES);
	this->_industrials.push_back(_AIRPORTS_AND_SERVICES);
	this->industrials.push_back(AIRLINES);
	this->_industrials.push_back(_AIRLINES);
	this->industrials.push_back(BUILDING_PRODUCTS_AND_EQUIPMENT);
	this->_industrials.push_back(_BUILDING_PRODUCTS_ANDE_EQUIPMENT);
	this->industrials.push_back(BUSINESS_EQUIPMENT_AND_SUPPLIES);
	this->_industrials.push_back(_BUSINESS_EQUIPMENT_AND_SUPPLIES);
	this->industrials.push_back(CONGLOMERATES);
	this->_industrials.push_back(_CONGLOMERATES);
	this->industrials.push_back(CONSULTING_SERVICES);
	this->_industrials.push_back(_CONSULTING_SERVICES);
	this->industrials.push_back(ELECTRICAL_EQUIPMENT_AND_PARTS);
	this->_industrials.push_back(_ELECTRICAL_EQUIPMENT_AND_PARTS);
	this->industrials.push_back(ENGINEERING_AND_CONSTRUCTION);
	this->_industrials.push_back(_ENGINEERING_AND_CONSTRUCTION);
	this->industrials.push_back(INDUSTRIAL_DISTRIBUTION);
	this->_industrials.push_back(_INDUSTRIAL_DISTRIBUTION);
	this->industrials.push_back(FARM_AND_HEAVY_CONSTRUCTION_MACHINERY);
	this->_industrials.push_back(_FARM_AND_HEAVY_CONSTRUCTION_MACHINERY);
	this->industrials.push_back(INTEGRATED_FREIGHT_AND_LOGISTICS);
	this->_industrials.push_back(_INTEGRATED_FREIGHT_AND_LOGISTICS);
	this->industrials.push_back(MARINE_SHIPPING);
	this->_industrials.push_back(_MARINE_SHIPPING);
	this->industrials.push_back(METAL_FABRICATION);
	this->_industrials.push_back(_METAL_FABRICATION);
	this->industrials.push_back(POLLUTION_AND_TREATMENT_CONTROLS);
	this->_industrials.push_back(_POLLUTION_AND_TREATMENT_CONTROLS);
	this->industrials.push_back(RAILROADS);
	this->_industrials.push_back(_RAILROADS);
	this->industrials.push_back(RENTAL_AND_LEASING_SERVICES);
	this->_industrials.push_back(_RENTAL_AND_LEASING_SERVICES);
	this->industrials.push_back(SECURITY_AND_PROTECTION_SERVICES);
	this->_industrials.push_back(_SECURITY_AND_PROTECTION_SERVICES);
	this->industrials.push_back(SPECIALTY_BUSINESS_SERVICES);
	this->_industrials.push_back(_SPECIALTY_BUSINESS_SERVICES);
	this->industrials.push_back(SPECIALTY_INDUSTRIAL_MACHINERY);
	this->_industrials.push_back(_SPECIALTY_INDUSTRIAL_MACHINERY);
	this->industrials.push_back(STAFFING_AND_EMPLOYMENT_SERVICES);
	this->_industrials.push_back(_STAFFING_AND_EMPLOYMENT_SERVICES);
	this->industrials.push_back(TOOLS_AND_ACCESSORIES);
	this->_industrials.push_back(_TOOLS_AND_ACCESSORIES);
	this->industrials.push_back(TRUCKING);
	this->_industrials.push_back(_TRUCKING);
	this->industrials.push_back(WASTE_MANAGEMENT);
	this->_industrials.push_back(_WASTE_MANAGEMENT);

	//NEW
	this->information_technology.push_back(ETF);
	this->_information_technology.push_back(_ETF);

	this->information_technology.push_back(COMMUNICATIONS_EQUIPMENT);
	this->_information_technology.push_back(_COMMUNICATIONS_EQUIPMENT);
	this->information_technology.push_back(COMPUTER_HARDWARE);
	this->_information_technology.push_back(_COMPUTER_HARDWARE);
	this->information_technology.push_back(CONSUMER_ELECTRONICS);
	this->_information_technology.push_back(_CONSUMER_ELECTRONICS);
	this->information_technology.push_back(ELECTRONIC_COMPONENTS);
	this->_information_technology.push_back(_ELECTRONIC_COMPONENTS);
	this->information_technology.push_back(ELECTRONICS_AND_COMPUTER_DISTRIBUTION);
	this->_information_technology.push_back(_ELECTRONICS_AND_COMPUTER_DISTRIBUTION);
	this->information_technology.push_back(INFORMATION_TECHNOLOGY_SERVICES);
	this->_information_technology.push_back(_INFORMATION_TECHNOLOGY_SERVICES);
	this->information_technology.push_back(SCIENTIFIC_AND_TECHNICAL_INSTRUMENTS);
	this->_information_technology.push_back(_SCIENTIFIC_AND_TECHNICAL_INSTRUMENTS);
	this->information_technology.push_back(SEMICONDUCTOR_EQUIPMENT_AND_MATERIALS);
	this->_information_technology.push_back(_SEMICONDUCTOR_EQUIPMENT_AND_MATERIALS);
	this->information_technology.push_back(SEMICONDUCTORS);
	this->_information_technology.push_back(_SEMICONDUCTORS);
	this->information_technology.push_back(SOFTWARE_APPLICATION);
	this->_information_technology.push_back(_SOFTWARE_APPLICATION);
	this->information_technology.push_back(SOFTWARE_INFRASTRUCTURE);
	this->_information_technology.push_back(_SOFTWARE_INFRASTRUCTURE);
	this->information_technology.push_back(SOLAR);
	this->_information_technology.push_back(_SOLAR);

	//NEW
	this->materials.push_back(ETF);
	this->_materials.push_back(_ETF);

	this->materials.push_back(AGRICULTURE_INPUTS);
	this->_materials.push_back(_AGRICULTURE_INPUTS);
	this->materials.push_back(ALUMINUM);
	this->_materials.push_back(_ALUMINUM);
	this->materials.push_back(BUILDING_MATERIALS);
	this->_materials.push_back(_BUILDING_MATERIALS);
	this->materials.push_back(CHEMICALS);
	this->_materials.push_back(_CHEMICALS);
	this->materials.push_back(COKING_COAL);
	this->_materials.push_back(_COKING_COAL);
	this->materials.push_back(COPPER);
	this->_materials.push_back(_COPPER);
	this->materials.push_back(GOLD);
	this->_materials.push_back(_GOLD);
	this->materials.push_back(LUMBER_AND_WOOD_PRODUCTION);
	this->_materials.push_back(_LUMBER_AND_WOOD_PRODUCTION);
	this->materials.push_back(OTHER_INDUSTRIAL_METALS_AND_MINING);
	this->_materials.push_back(_OTHER_INDUSTRIAL_METALS_AND_MINING);
	this->materials.push_back(OTHER_PRECIOUS_METALS_AND_MINING);
	this->_materials.push_back(_OTHER_PRECIOUS_METALS_AND_MINING);
	this->materials.push_back(PAPER_AND_PAPER_PRODUCTS);
	this->_materials.push_back(_PAPER_AND_PAPER_PRODUCTS);
	this->materials.push_back(SILVER);
	this->_materials.push_back(_SILVER);
	this->materials.push_back(SPECIALTY_CHEMICALS);
	this->_materials.push_back(_SPECIALTY_CHEMICALS);
	this->materials.push_back(STEEL);
	this->_materials.push_back(_STEEL);

	//NEW
	this->real_estate.push_back(ETF);
	this->_real_estate.push_back(_ETF);

	this->real_estate.push_back(REAL_ESTATE_DEVELOPMENT);
	this->_real_estate.push_back(_REAL_ESTATE_DEVELOPMENT);
	this->real_estate.push_back(REAL_ESTATE_DIVERSIFIED);
	this->_real_estate.push_back(_REAL_ESTATE_DIVERSIFIED);
	this->real_estate.push_back(REAL_ESTATE_SERVICES);
	this->_real_estate.push_back(_REAL_ESTATE_SERVICES);
	this->real_estate.push_back(REIT_DIVERSIFIED);
	this->_real_estate.push_back(_REIT_DIVERSIFIED);
	this->real_estate.push_back(REIT_HEALTH_CARE_FACILITIES);
	this->_real_estate.push_back(_REIT_HEALTH_CARE_FACILITIES);
	this->real_estate.push_back(REIT_HOTEL_AND_MOTEL);
	this->_real_estate.push_back(_REIT_HOTEL_AND_MOTEL);
	this->real_estate.push_back(REIT_INDUSTRIAL);
	this->_real_estate.push_back(_REIT_INDUSTRIAL);
	this->real_estate.push_back(REIT_MORTGAGE);
	this->_real_estate.push_back(_REIT_MORTGAGE);
	this->real_estate.push_back(REIT_OFFICE);
	this->_real_estate.push_back(_REIT_OFFICE);
	this->real_estate.push_back(REIT_RESIDENTIAL);
	this->_real_estate.push_back(_REIT_RESIDENTIAL);
	this->real_estate.push_back(REIT_RETAIL);
	this->_real_estate.push_back(_REIT_RETAIL);
	this->real_estate.push_back(REAL_ESTATE_DIVERSIFIED);
	this->_real_estate.push_back(_REIT_SPECIALTY);

	//NEW
	this->utilities.push_back(ETF);
	this->_utilities.push_back(_ETF);

	this->utilities.push_back(UTILITIES_DIVERSIFIED);
	this->_utilities.push_back(_UTILITIES_DIVERSIFIED);
	this->utilities.push_back(UTILITIES_INDEPENDENT_POWER_PRODUCERS);
	this->_utilities.push_back(_UTILITIES_INDEPENDENT_POWER_PRODUCERS);
	this->utilities.push_back(UTITLITIES_REGULATED_ELECTRIC);
	this->_utilities.push_back(_UTITLITIES_REGULATED_ELECTRIC);
	this->utilities.push_back(UTITLITIES_REGULATED_GAS);
	this->_utilities.push_back(_UTITLITIES_REGULATED_GAS);
	this->utilities.push_back(UTITLITIES_REGULATED_WATER);
	this->_utilities.push_back(_UTITLITIES_REGULATED_WATER);
	this->utilities.push_back(UTITLITIES_RENEWABLE);
	this->_utilities.push_back(_UTITLITIES_RENEWABLE);

	this->urls.push_back("https://finviz.com/grp_export.ashx?g=industry&sg=communicationservices&v=140&o=name");
	this->urls.push_back("https://finviz.com/grp_export.ashx?g=industry&sg=consumercyclical&v=140&o=name");
	this->urls.push_back("https://finviz.com/grp_export.ashx?g=industry&sg=consumerdefensive&v=140&o=name");
	this->urls.push_back("https://finviz.com/grp_export.ashx?g=industry&sg=energy&v=140&o=name");
	this->urls.push_back("https://finviz.com/grp_export.ashx?g=industry&sg=financial&v=140&o=name");
	this->urls.push_back("https://finviz.com/grp_export.ashx?g=industry&sg=healthcare&v=140&o=name");
	this->urls.push_back("https://finviz.com/grp_export.ashx?g=industry&sg=industrials&v=140&o=name");
	this->urls.push_back("https://finviz.com/grp_export.ashx?g=industry&sg=technology&v=140&o=name");
	this->urls.push_back("https://finviz.com/grp_export.ashx?g=industry&sg=basicmaterials&v=140&o=name");
	this->urls.push_back("https://finviz.com/grp_export.ashx?g=industry&sg=realestate&v=140&o=name");
	this->urls.push_back("https://finviz.com/grp_export.ashx?g=industry&sg=utilities&v=140&o=name");

	if (!this->Read())
	{
		for (size_t i = 8; i < this->sec.size() - 1; ++i)
			this->parentsector.push_back(ParentSector(this, this->sec[i], this->m_GetURL(this->sec[i]), this->_sec[i]));
	}

	this->m_FetchSectorData();
}

wxString SectorClass::GetSectorString(_Sector s)
{
	for (size_t i = 0; i < this->sec.size(); ++i)
	{
		if (this->sec[i] == s)
			return this->_sec[i];
	}

	wxMessageBox("Could not find a match in SectorClass::GetSectorString! Returning Invalid Sector!");
	return _SECTOR_INVALID;
}

wxString SectorClass::GetSubSectorString(_Sector s, _Sub_Sector sub)
{
	switch(s)
	{
	case _Sector::BOND: return _ETF;
	case _Sector::SMALLCAP: return _ETF;
	case _Sector::MIDCAP: return _ETF;
	case _Sector::LARGECAP: return _ETF;
//	case _Sector::TECH: return _ETF;
	case _Sector::FOREIGN: return _ETF;
	case _Sector::CRYPTO: return _ETF;
	case _Sector::COMMUNICATION_SERVICES: return this->m_GetSubSector(this->communication_services, this->_communication_services, sub);
	case _Sector::CONSUMER_DESCRETIONARY: return this->m_GetSubSector(this->consumer_discretionary, this->_consumer_discretionary, sub);
	case _Sector::CONSUMER_STAPLES: return this->m_GetSubSector(this->consumer_staples, this->_consumer_staples, sub);
	case _Sector::ENERGY: return this->m_GetSubSector(this->energy, this->_energy, sub);
	case _Sector::FINANCIALS: return this->m_GetSubSector(this->financials, this->_financials, sub);
	case _Sector::HEALTH_CARE: return this->m_GetSubSector(this->health_care, this->_health_care, sub);
	case _Sector::INDUSTRIALS: return this->m_GetSubSector(this->industrials, this->_industrials, sub);
	case _Sector::INFORMATION_TECHNOLOGY: return this->m_GetSubSector(this->information_technology, this->_information_technology, sub);
	case _Sector::MATERIALS: return this->m_GetSubSector(this->materials, this->_materials, sub);
	case _Sector::REAL_ESTATE: return this->m_GetSubSector(this->real_estate, this->_real_estate, sub);
//	case _Sector::REIT: return _ETF;
	case _Sector::SECTOR_INVALID: return _SECTOR_INVALID;
	case _Sector::UTILITIES: return this->m_GetSubSector(this->utilities, this->_utilities, sub);
	}

	wxFAIL_MSG("Switch failed in SectorClass::GetSubSector! Returning empty string");
	return "";
}

_Sector SectorClass::GetSectorID(wxString s)
{
	for (size_t i = 0; i < this->_sec.size(); ++i)
	{
		if (this->_sec[i] == s)
			return this->sec[i];
	}

	wxFAIL_MSG("wxString sent to SectorClass::GetSectorID has not match! String was: " + s);
	return SECTOR_INVALID;
}

_Sub_Sector SectorClass::GetSubSectorID(_Sector id, wxString sub)
{
	switch (id)
	{
	case _Sector::BOND: return ETF;
	case _Sector::SMALLCAP: return ETF;
	case _Sector::MIDCAP: return ETF;
	case _Sector::LARGECAP: return ETF;
//	case _Sector::TECH: return ETF;
	case _Sector::FOREIGN: return ETF;
	case _Sector::CRYPTO: return ETF;
	case _Sector::COMMUNICATION_SERVICES: return this->m_GetSubSectorID(this->_communication_services, this->communication_services, sub);
	case _Sector::CONSUMER_DESCRETIONARY: return this->m_GetSubSectorID(this->_consumer_discretionary, this->consumer_discretionary, sub);
	case _Sector::CONSUMER_STAPLES: return this->m_GetSubSectorID(this->_consumer_staples, this->consumer_staples, sub);
	case _Sector::ENERGY: return this->m_GetSubSectorID(this->_energy, this->energy, sub);
	case _Sector::FINANCIALS: return this->m_GetSubSectorID(this->_financials, this->financials, sub);
	case _Sector::HEALTH_CARE: return this->m_GetSubSectorID(this->_health_care, this->health_care, sub);
	case _Sector::INDUSTRIALS: return this->m_GetSubSectorID(this->_industrials, this->industrials, sub);
	case _Sector::INFORMATION_TECHNOLOGY: return this->m_GetSubSectorID(this->_information_technology, this->information_technology, sub);
	case _Sector::MATERIALS: return this->m_GetSubSectorID(this->_materials, this->materials, sub);
	case _Sector::REAL_ESTATE: return this->m_GetSubSectorID(this->_real_estate, this->real_estate, sub);
//	case _Sector::REIT: return ETF;
	case _Sector::SECTOR_INVALID: return SUB_SECTOR_INVALID;
	case _Sector::UTILITIES: return this->m_GetSubSectorID(this->_utilities, this->utilities, sub);
	}

	wxFAIL_MSG("Switch failed in SectorClass::m_GetSubSectorID! Returning SUB_SECTOR_INVALID");
	return SUB_SECTOR_INVALID;
}

const wxVector<_Sub_Sector>* SectorClass::GetSubSector(_Sector s)
{
	return &this->m_GetSubSectorVec(s);
}

const wxVector<wxString>* SectorClass::GetSubSectorString(_Sector s)
{
	return &this->m_GetSubSectorStringVec(s);
}

const wxVector<_Sector>* SectorClass::GetSector()
{
	return &this->sec;
}

const wxVector<wxString>* SectorClass::GetSectorString()
{
	return &this->_sec;
}

ParentSector* SectorClass::GetSector(_Sector s)
{
	for (size_t i = 0; i < this->parentsector.size(); ++i)
	{
		if (this->parentsector[i].GetSectorId() == s)
			return &this->parentsector[i];
	}

	return nullptr;
}

wxVector<SubSector>* SectorClass::GetSubSectorVec(_Sector s)
{
	ParentSector* ps = this->GetSector(s);
	if (ps)
		return ps->GetSubSector();

	return NULL;
}

bool SectorClass::Read()
{
	DataStream ds("SavedFiles/SectorClass.bin", wxPosixPermissions::wxS_DEFAULT, wxFile::OpenMode::read);
	if (!ds.IsOK())
		return false;

	size_t size;
	ds.ReadData(size);
	for (size_t i = 0; i < size; ++i)
		this->parentsector.push_back(ParentSector(this, ds));

	return true;
}

void SectorClass::Save()
{
	DataStream ds("SavedFiles/SectorClass.bin", wxPosixPermissions::wxS_DEFAULT, wxFile::OpenMode::write);
	if (!ds.IsOK())
	{
		wxFAIL_MSG("Could not write to: SavedFiles/SectorClass.bin! In SectorClass::Read");
		return;
	}

	size_t size = this->parentsector.size();
	ds.WriteData(size);
	for (auto& v : this->parentsector)
		v.Write(ds);
}

wxString SectorClass::m_GetSubSector(wxVector<_Sub_Sector>& v, wxVector<wxString>& sv, _Sub_Sector item)
{
	for (size_t i = 0; i < v.size(); ++i)
	{
		if (v[i] == item)
			return sv[i];
	}

	wxFAIL_MSG("vector failed in SectorClass::m_GetSubSector private function! Returning empty string");
	return "";
}

_Sub_Sector SectorClass::m_GetSubSectorID(wxVector<wxString>& v, wxVector<_Sub_Sector>& sub, wxString& s)
{
	for (size_t i = 0; i < v.size(); ++i)
	{
		if (v[i] == s)
			return sub[i];
	}

	wxFAIL_MSG("vector failed in SectorClass::m_GetSubSector private function! Returning SECTOR_INVALID string is: " + s);
	return SUB_SECTOR_INVALID;
}

wxVector<_Sub_Sector>& SectorClass::m_GetSubSectorVec(_Sector s)
{
	switch (s)
	{
	case _Sector::COMMUNICATION_SERVICES: return this->communication_services;
	case _Sector::CONSUMER_DESCRETIONARY: return this->consumer_discretionary;
	case _Sector::CONSUMER_STAPLES: return this->consumer_staples;
	case _Sector::ENERGY: return this->energy;
	case _Sector::FINANCIALS: return this->financials;
	case _Sector::HEALTH_CARE: return this->health_care;
	case _Sector::INDUSTRIALS: return this->industrials;
	case _Sector::INFORMATION_TECHNOLOGY: return this->information_technology;
	case _Sector::MATERIALS: return this->materials;
	case _Sector::REAL_ESTATE: return this->real_estate;
	case _Sector::UTILITIES: return this->utilities;
    // returning empty vector...
	default: return emptyint;
	}
}

wxVector<wxString>& SectorClass::m_GetSubSectorStringVec(_Sector s)
{
	switch (s)
	{
	case _Sector::COMMUNICATION_SERVICES: return this->_communication_services;
	case _Sector::CONSUMER_DESCRETIONARY: return this->_consumer_discretionary;
	case _Sector::CONSUMER_STAPLES: return this->_consumer_staples;
	case _Sector::ENERGY: return this->_energy;
	case _Sector::FINANCIALS: return this->_financials;
	case _Sector::HEALTH_CARE: return this->_health_care;
	case _Sector::INDUSTRIALS: return this->_industrials;
	case _Sector::INFORMATION_TECHNOLOGY: return this->_information_technology;
	case _Sector::MATERIALS: return this->_materials;
	case _Sector::REAL_ESTATE: return this->_real_estate;
	case _Sector::UTILITIES: return this->_utilities;
		// returning empty vector...
	default:
	{
		wxFAIL_MSG("No _Sector matches in SectorClass::m_GetSubSectorStringVec!");
		return emptystring;
	}
	}
}

wxString SectorClass::m_GetURL(_Sector s)
{
	switch (s)
	{
	case _Sector::COMMUNICATION_SERVICES: return this->urls[0];
	case _Sector::CONSUMER_DESCRETIONARY: return this->urls[1];
	case _Sector::CONSUMER_STAPLES: return this->urls[2];
	case _Sector::ENERGY: return this->urls[3];
	case _Sector::FINANCIALS: return this->urls[4];
	case _Sector::HEALTH_CARE: return this->urls[5];
	case _Sector::INDUSTRIALS: return this->urls[6];
	case _Sector::INFORMATION_TECHNOLOGY: return this->urls[7];
	case _Sector::MATERIALS: return this->urls[8];
	case _Sector::REAL_ESTATE: return this->urls[9];
	case _Sector::UTILITIES: return this->urls[10];
		// returning empty string...
	default: return "";
	}
}

void SectorClass::m_SetData(SectorOverview& so, SectorPerformance& sp)
{
	_Sector s = this->GetSectorID(sp.name);
	for (size_t i = 0; i < this->parentsector.size(); ++i)
	{
		if (this->parentsector[i].GetSectorId() == s)
		{
			this->parentsector[i].SetAllData(so.name, so.market_cap, so.change, sp);
			return;
		}
	}

	wxFAIL_MSG("SectorClass::m_SetOverViewData failed to match in parentsector!");
}

void SectorClass::m_FetchSectorData()
{
	// We need to check if data is up to date...
	if (this->IsUpToDate())
		return;
	
	wxVector<SectorOverview> v;
	wxVector<SectorPerformance> v2;
	wxString empty = "";
	Parser parser(empty);

	wxString url = this->sectorurl;
	url.Replace(_INSERTION, _OVERVIEW);
	parser.PullFinVizOverview(url, v);

	wxString url2 = this->sectorurl;
	url2.Replace(_INSERTION, _PERFORMANCE);
	parser.PullFinVizPerformance(url2, v2);

	for (size_t i = 0; i < v.size(); ++i)
		this->m_SetData(v[i], v2[i]);
}

bool SectorClass::IsUpToDate()
{
	if (this->parentsector.begin() != this->parentsector.end())
		return this->parentsector.begin()->IsUpToDate();

	else
	{
		wxFAIL_MSG("In SectorClass::IsUpToDate this->parentsector vec is empty!!");
		return false;
	}
}

static SectorClass sc;
SectorClass& GetSectorClass()
{
	return sc;
}

wxString StockViewerData::GetWeekGain()
{
	return wxNumberFormatter::ToString(this->week_gain * 100, 2) + "%";
}

// stock_node functions...
stock_node::stock_node(long id, wxString ticker, wxDateTime date, double price_per_share, double shares, Action a, wxDateTime reinvest)
	: m_id(id), m_ticker(ticker), m_date(date), m_price_per_share(price_per_share), m_shares(shares), action(a), m_reinvest_start(reinvest), active(1)
{
	
}

void stock_node::SetSibling(stock_node sn)
{
	size_t i = 0;
	while (i < this->sold.size() && this->sold[i].GetPurchaseDate() < sn.GetPurchaseDate())
		++i;
	if (i == this->sold.size() || i == 0)
		this->sold.push_back(sn);
	else
		this->sold.insert(&this->sold[i], sn);

	this->active = this->GetShares(NULL) ? true : false;
}

void stock_node::ShareDivsWithSibling(stock_node* sn)
{
	if (this == sn)
		return;
	for (auto& v : this->divs)
	{
		if (v.ex_Div > sn->GetPurchaseDate())
			sn->HandleDivReInvest(&v);
	}
}

double stock_node::GetCostBasis(wxDateTime* T)
{
	if (!T)
	{
		wxFAIL_MSG("wxDateTime* passed to stock_node::GetCostBasis is nullptr!");
		return 0.0;
	}

	double shares = this->m_GetShares(T);
	return shares * m_price_per_share;
}

double stock_node::GetShares(wxDateTime* T)
{
	return this->m_GetShares(T);
}

double stock_node::GetRealizedGain(wxDateTime* T)
{
	double gain = 0.0;
	for (size_t i = 0; i < this->sold.size(); ++i)
	{
		if (sold[i].m_date <= *T)
			gain += sold[i].GetCostBasis(T);
	}

	return gain;
}

bool stock_node::IdMatch(long& id)
{
	if (this->m_id == id)
		return true;

	return false;
}

wxString stock_node::GetTicker()
{
	return this->m_ticker;
}

wxDateTime stock_node::GetPurchaseDate()
{
	return this->m_date;
}

double stock_node::GetPurchasePrice(wxDateTime::Month start, int dayofyear, int year)
{
	if (this->m_date.GetYear() != year)
		return 0.0;

	if (this->m_date.GetMonth() >= start && this->m_date.GetDayOfYear() <= dayofyear)
	{
		return this->m_price_per_share;
	}
	else
		return 0.0;
}

double stock_node::GetPurchasePrice(int year, int dayofyear)
{
	if (this->m_date.GetYear() != year)
		return 0.0;

	if(this->m_date.GetDayOfYear() <= dayofyear)
		return this->m_price_per_share;

	return 0.0;
}

double stock_node::GetPurchasePrice(int monday, int friday, int year)
{
	if (this->m_date.GetYear() != year)
		return 0.0;

	if (this->m_date.GetDayOfYear() >= monday && this->m_date.GetDayOfYear() <= friday)
		return this->m_price_per_share;

	return 0.0;
}

double stock_node::GetPurchasePrice(wxDateTime* date)
{
	if (*date == this->m_date)
		return this->m_price_per_share;

	return 0.0;
}

void stock_node::AddDivSharesToDiv(Dividend& d)
{
	for (auto& v : this->divs)
	{
		if (v.ex_Div == d.ex_Div && d.DivReinvest)
		{
			d.re_invest_shares += v.re_invest_shares;
			return;
		}
	}
}

void stock_node::HandleDivReInvest(Dividend& d)
{
	if (this->GetPurchaseDate() < d.ex_Div && this->GetShares(&d.ex_Div))
	{
		if (this->m_reinvest_start.IsValid())
		{
			if (this->m_reinvest_start <= d.ex_Div)
				d.SetDivReinvestOn();
			else
			{
				int span = this->m_reinvest_start.GetDayOfYear() - (d.ex_Div + wxDateSpan(0, 0, 0, 1)).GetDayOfYear();
				if (!span || span < 31)
					d.SetDivReinvestOn();
			}

		}
		this->InsertDiv(d);
	}
}

void stock_node::HandleDivReInvest(Dividend* d)
{
	if (this->GetPurchaseDate() < d->ex_Div && this->GetShares(&d->ex_Div))
	{
		if (this->m_reinvest_start.IsValid())
		{
			if (this->m_reinvest_start <= d->ex_Div)
				d->SetDivReinvestOn();
			else
			{
				int span = this->m_reinvest_start.GetDayOfYear() - (d->ex_Div + wxDateSpan(0, 0, 0, 1)).GetDayOfYear();
				if (!span || span < 31)
					d->SetDivReinvestOn();
			}

		}
		this->InsertDiv(*d);
	}
}

void stock_node::HandlePaymentDate(Dividend& dividend, wxDateTime& date)
{
	if (this->GetPurchaseDate() < dividend.ex_Div && this->GetShares(&dividend.ex_Div))
	{
		for (auto& v : this->divs)
		{
			if (v.ex_Div == dividend.ex_Div)
			{
				v.payment_date = date;
				return;
			}
		}
	}
}

wxDateTime* stock_node::GetLatestDivDate()
{
	wxDateTime* latest = nullptr;
	for (size_t i = 0; i < this->divs.size(); ++i)
	{
		if ((latest && !latest->IsValid()) || !latest)
			latest = &this->divs[i].ex_Div;
		else
		{
			if (*latest < this->divs[i].ex_Div)
				latest = &this->divs[i].ex_Div;
		}
	}

	return latest;
}

wxDateTime* stock_node::GetDividendDates(wxDateTime& date)
{
	if (!date.IsValid())
		return nullptr;

	for (size_t i = 0; i < this->divs.size(); ++i)
	{
		if (date == this->divs[i].ex_Div)
			return &this->divs[i].ex_Div;
	}

	return nullptr;
}

double stock_node::GetDividends(wxDateTime* date)
{
	double d = 0.0;
	for (size_t i = 0; i < this->divs.size(); ++i)
	{
		if (!divs[i].DivReinvest && *date >= this->divs[i].ex_Div)
			d += this->divs[i].div * this->GetShares(&this->divs[i].ex_Div);
	}

	return d;
}

double stock_node::GetDividendShares(wxDateTime* date)
{
	double d = 0.0;
	for (size_t i = 0; i < this->divs.size(); ++i)
	{
		if (divs[i].DivReinvest && *date >= this->divs[i].ex_Div)
			d += this->divs[i].re_invest_shares;
	}

	return d;
}

void stock_node::GetDividendVec(wxVector<Dividend>& v)
{
	this->FillClientDividendVec(v);
}

bool stock_node::IsPendingDivReInvest()
{
	for (auto& v : this->divs)
	{
		if (v.DivReinvest && v.IsPendingReInvestment())
			return true;
	}

	return false;
}

bool stock_node::SetReInvestShares(double& s)
{
	for (auto& v : this->divs)
	{
		if (v.DivReinvest && v.IsPendingReInvestment())
		{
			v.AddReInvestmentShares(s);
			return true;
		}
	}

	return false;
}

bool stock_node::SetReInvestShares(Dividend& d, double& s)
{
	for (auto& v : this->divs)
	{
		if (v.DivReinvest && v.IsPendingReInvestment() && d.ex_Div == v.ex_Div)
		{
			if (!v.payment_date.IsValid())
			{
				v.AddReInvestmentShares(s);
				v.payment_date = wxDateTime::Today();
			}
			else
			{
				v.AddReInvestmentShares(s);
				return true;
			}
		}
	}

	return false;
}

void stock_node::SetSiblingRatio(double ratio)
{
	this->ratio_of_siblings = ratio;
}

double stock_node::GetSiblingRatio()
{
	return this->ratio_of_siblings;
}

void stock_node::Save(DataStream& ds)
{
	int valid = 1;
	int invalid = -1;
	ds.WritewxDateTime(this->m_date);

	if (this->m_reinvest_start.IsValid())
	{
		ds.WriteData(valid);
		ds.WritewxDateTime(this->m_reinvest_start);
	}
	else
		ds.WriteData(invalid);

//	ds.Write(this->m_ticker);
	ds.WriteLong(this->m_id);
	ds.WriteData(this->m_price_per_share);
	ds.WriteData(this->m_shares);
	ds.WriteData(this->m_average_sold_price);
	ds.WriteData(this->action);
	ds.WriteDividendVector(this->divs);

	size_t size = this->sold.size();
	ds.WriteData(size);
	for (size_t i = 0; i < size; ++i)
		this->sold[i].Save(ds);
}

void stock_node::Retrieve(DataStream& ds)
{
	int valid = 1;
	int invalid = -1;
	int temp;
	ds.ReadwxDateTime(this->m_date);
	
	ds.ReadData(temp);
	if (temp == valid)
		ds.ReadwxDateTime(this->m_reinvest_start);

//	ds.Read(this->m_ticker);
	ds.ReadLong(this->m_id);
	ds.ReadData(this->m_price_per_share);
	ds.ReadData(this->m_shares);
	ds.ReadData(this->m_average_sold_price);
	ds.ReadData(this->action);
	ds.ReadDividendVector(this->divs);

	size_t size;
	ds.ReadData(size);
	for (size_t i = 0; i < size; ++i)
	{
		this->sold.push_back(stock_node(0, this->m_ticker, wxDateTime(wxInvalidDateTime), 0.0, 0.0, Action::SELL, wxDateTime(wxInvalidDateTime)));
		this->sold[this->sold.size() - 1].Retrieve(ds);
	}
}

void stock_node::InsertDiv(Dividend& d)
{
	size_t i = 0;
	while (i < this->divs.size() && this->divs[i].ex_Div < d.ex_Div)
		++i;
	if (i == this->divs.size() || i == 0)
	{
		this->divs.push_back(d);
		this->divs[divs.size() - 1].id = this->m_id;
	}
	else
	{
		// check for duplicate
		if (this->divs[i].ex_Div != d.ex_Div)
			this->divs.insert(&this->divs[i], d)->id = this->m_id;
	}
}

double stock_node::GetPurchasePrice()
{
	return this->m_price_per_share;
}

void stock_node::_FillClientDividendVec(wxVector<Dividend>& vec)
{
	for (auto& v : this->divs)
	{
		bool duplicate = false;
		for (auto& vv : vec)
		{
			if (vv.ex_Div == v.ex_Div)
			{
				duplicate = true;
				break;
			}
		}
		if (!duplicate)
			vec.push_back(v);
	}
}

void stock_node::FillClientDividendVec(wxVector<Dividend>& v)
{
	for (size_t i = 0; i < this->divs.size(); ++i)
	{
		if (!this->divs[i].DivReinvest)
			v.push_back(this->divs[i]);
	}
}

double stock_node::m_GetShares(wxDateTime* T)
{
	double shares = 0.0;
	if (!T || *T >= this->m_date)
		shares = this->m_shares;
	for (size_t i = 0; i < this->sold.size(); ++i)
	{
		if (!T || sold[i].m_date <= *T)
			shares -= sold[i].m_shares;
	}

	return shares;
}

// CallBacks
void SetHistoricalDataCB(void* v, double o, double h, double l, double c, wxDateTime d, _PortfolioType type)
{
	if (type == STOCK)
		static_cast<StockNode*>(v)->SetHistoricalData(Day_Prices(o, h, l, c, d));
	else if(type == INDICES)
		static_cast<Indices*>(v)->SetHistoricalData(Day_Prices(o, h, l, c, d));
}

void SetSummaryDataCB(void* v, SummaryData sd, _PortfolioType type)
{
	if (type == STOCK)
		static_cast<StockNode*>(v)->SetSummaryData(sd);
	else if (type == INDICES)
		static_cast<Indices*>(v)->SetSummaryData(sd);
}

void SetDividendsCB(void* v, Dividend d)
{
	static_cast<StockNode*>(v)->SetDividends(d);
}

// thread callback....
bool ExecutePurchase(StockNode* S, long id, wxDateTime date, double price, double shares)
{
	return S->InitiatePurchase(id, date, price, shares);
}

// StockThread functions...
StockThread::StockThread(wxThreadKind tk, Sector* sec, StockNode* S) 
	: wxThread(tk), m_parent(S)
{
	
}

wxThread::ExitCode StockThread::Entry()
{
	if (TestDestroy())
		return (wxThread::ExitCode)0;
	
	this->m_parent->ClockChange();
	wxPostEvent(this->m_parent, wxCommandEvent());
	return (wxThread::ExitCode)0;
}

StockThread::~StockThread()
{

}

// StockNode functions...
StockNode::StockNode(wxString t, wxDateTime* p, Portfolio* P, Sector* S, bool reinvest, wxString T) 
	: Return_node(&P->market_value, &S->market_value, nullptr), m_ticker(t), m_parentclock(p), m_grand_parent(P), m_parent(S), DivReinvest(reinvest)
{
	this->sectorID = S->GetID();
	this->Bind(ThreadComplete, &StockNode::OnThreadComplete, this);
	if (this->DivReinvest)
	{
		wxDateTime div_date = IsValidDate(T, "Div reinvest date is not valid in StockNode constructor!");
		this->div_reinvest_start_date = div_date;
	}
}

StockNode::StockNode(wxString t, wxDateTime* p, Portfolio* P, Sector* S)
	: Return_node(&P->market_value, &S->market_value, nullptr), m_ticker(t), m_parentclock(p), m_grand_parent(P), m_parent(S)
{
	this->sectorID = S->GetID();
	this->Bind(ThreadComplete, &StockNode::OnThreadComplete, this);
}

StockNode::StockNode(const StockNode& sn) : Return_node(sn.PortfolioMarketValue, sn.SectorMarketValue, sn.StockMarketValue), m_ticker(sn.m_ticker)
{
	this->historical_prices = sn.historical_prices;
	this->bought = sn.bought;
	this->m_parentclock = sn.m_parentclock;
	this->inital_purchase = sn.inital_purchase;
	this->m_parent = sn.m_parent;
	this->m_grand_parent = sn.m_grand_parent;
	this->SetSummaryData(sn.current_Data);
	this->DivReinvest = sn.DivReinvest;
	this->div_reinvest_start_date = sn.div_reinvest_start_date;
	this->sectorID = sn.sectorID;

}

StockNode::~StockNode()
{
	this->DeleteParser();
}

bool StockNode::Purchase(long id, wxString s, double price_per_share, double shares)
{
	wxDateTime date = IsValidDate(s, "Invalid or future date in StockNode::Purchase!");
	if (!date.IsValid())
		return false;

	if (shares == 0.0)
	{
		wxFAIL_MSG("Shares passed to StockNode::Purchase is 0! Purchase failed!");
		return false;
	}

	return this->InitiatePurchase(id, date, price_per_share, shares);
}

bool StockNode::InitiatePurchase(long id, wxDateTime date, double price_per_share, double shares)
{
	bool needsupdate = false;
	if (this->m_Purchase(id, date, price_per_share, shares))
	{
		if (this->bought.size() > 1)
		{
			if (this->inital_purchase.IsValid() && !this->m_FindDateInHistoricalPrices(this->inital_purchase))
				needsupdate = true;
			else if (this->purchaseDateSwitch)
				needsupdate = true;;
		}
		else
			needsupdate = true;
	}

	if (needsupdate)
	{
		if (this->UpDate(true))
			this->Calibrate();
		this->m_active = 1;
	}

	if (this->bought.size() > 1)
		this->ShareDivstoLastPurchase();

	this->UpToDate = 1;
	return true;
}

bool StockNode::Sell(long& id, wxDateTime& date, double& price_per_share, double& shares)
{
	stock_node* sn = this->FindLot(id);
	if (!sn)
	{
		wxString lot = "";
		lot << id;
		wxFAIL_MSG("Could not find lot number: " + lot + " in StockNode::Sell!");
		return false;
	}

	return this->m_Sell(id, date, price_per_share, shares, sn);
}

bool StockNode::IdMatch(long l)
{
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		if (this->bought[i].IdMatch(l))
			return true;
	}

	return false;
}

wxString StockNode::GetTicker() const
{
	return this->m_ticker;
}

wxString StockNode::GetLongName()
{
	return this->current_Data.Longname;
}

double StockNode::GetShares()
{
	wxDateTime T = *this->m_parentclock;
	return this->div_shares + this->shares;
	return this->m_GetShares(&T);
}

wxDateTime StockNode::GetPurchaseDate()
{
	return this->GetInitalPurchaseDate();
}

bool StockNode::TickerMatch(wxString ticker)
{
	if (this->m_ticker == ticker)
		return true;

	return false;
}

wxString StockNode::GetNextEarningsDate()
{
	return this->current_Data.earningsdate;
}

bool StockNode::IsActive() const
{
	return this->m_active;
}

void StockNode::Calibrate(bool force)
{
	if (this->UpToDate && !force)
		return;
	else
		this->UpDate(force);

	this->_52weekDeviation = this->GetDeviation(260);
	this->_90dayDeviation = this->GetDeviation(90);
	this->_30dayDeviation = this->GetDeviation(30);

	wxDateTime T = *this->m_parentclock;

	double _shares = this->m_GetShares(&T);
	double divshares = this->GetDividendShares();
	if (!_shares)
	{
		// not active at this date..
	//	this->m_active = 0;
		this->SetNewValues(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		this->svd = StockViewerData(this, this->current_Data.Longname, this->m_ticker, "", "",
			this->sectorID, _shares + divshares, 0.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
			0.0, this->GetDividends(), 0.0);

		this->UpToDate = 1;
		return;
	}

	double daycostbasis = this->GetDayCostBasis(&T);
	double weekcostbasis = this->GetWeekCostBasis(&T);
	double quartercostbasis = this->GetQuarterCostBasis(&T);
	double yearcostbasis = this->GetYearCostBasis(&T);
	wxDateTime initial = this->GetInitalPurchaseDate();
	double originalcostbasis = this->GetTotalCostBasis(&T);
	double marketvalue = this->GetMarketValue(&T);
	T = *this->m_parentclock;
	double _dividends = this->GetDividends();

	wxString ex_div = this->GetLatestEx_Div_Date();

	this->SetNewValues(originalcostbasis, daycostbasis, weekcostbasis, quartercostbasis, yearcostbasis, marketvalue, _shares, divshares, _dividends);
	this->beta = this->current_Data.Beta;
	this->UpToDate = 1;
	double _marketprice = wxDateTime::Today() == *this->m_parentclock ? this->current_Data.marketprice : marketvalue / this->GetShares();
	double _previous_close = wxDateTime::Today() == *this->m_parentclock ? this->current_Data.previousclose : this->GetPreviousClose(&T);

	// We put an astrisk next to the name if the stock is enrolled in dividend re investment
	// and there is a pending re investment that needs to be entered by the user...
	wxString t = this->PendingReInvestment() ? this->m_ticker + " *" : this->m_ticker;

	this->svd = StockViewerData(this, this->current_Data.Longname, t, this->current_Data.earningsdate, this->inital_purchase.Format(STANDARD_DATE),
		this->sectorID, _shares + divshares, originalcostbasis / _shares, _marketprice, _previous_close,
		this->day_return, this->week_return, this->quarter_return, this->year_return, this->total_return, originalcostbasis,
		marketvalue, this->GetDividends(), this->current_Data.Beta, 0.0, ex_div);

	this->UpToDate = 1;
}

void StockNode::ClockChange()
{
	UpToDate = 0;
	this->Calibrate();
}

// StockNode private functions...

void StockNode::CalcRatiosOfChildren(wxDateTime* T)
{
	wxDateTime today;
	if (!T)
		today = *this->m_parentclock;
	else
		today = wxDateTime::Today();
	wxVector<double> ratio;
	double total = 0.0;
	for (auto& v : this->bought)
	{
		ratio.push_back((v.GetShares(&today) + v.GetDividendShares(&today)) * this->current_Data.marketprice);
		total += *ratio.rbegin();
	}

	// check to make sure total is not zero. If it is then there is no shares to distribute. Return false;
	if (!total)
	{
		wxFAIL_MSG("total market value is zero in StockNode::DistributeReInvestmentShares(Dividend&, double&)! Returning false!");
		return;
	}

	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		if (ratio[i])
			this->bought[i].SetSiblingRatio(ratio[i] / total);
	}
}

wxString StockNode::GetLatestEx_Div_Date()
{
	for (auto& v : this->bought)
	{
		wxDateTime* T = NULL;
		if (v.IsActive())
		{
			T = v.GetLatestDivDate();
			if (T && T->IsValid())
				return T->Format(STANDARD_DATE);
		}
	}

	return "NA";
}

bool StockNode::DistributeReInvestmentShares(double& s)
{
	wxVector<double> ratio;
	wxDateTime today(wxDateTime::Today());
	double total = 0.0;
	for (auto& v : this->bought)
	{
		ratio.push_back((v.GetShares(&today) + v.GetDividendShares(&today)) * this->current_Data.marketprice);
		total += *ratio.rbegin();
	}

	// check to make sure total is not zero. If it is then there is no shares to distribute. Return false;
	if (!total)
	{
		wxFAIL_MSG("total market value is zero in StockNode::DistributeReInvestmentShares(double&)! Returning false!");
		return false;
	}

	bool result = true;
	bool endresult = true;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		if (ratio[i])
		{
			double rat = ratio[i] / total;
			double val = s * rat;
			result = this->bought[i].SetReInvestShares(val);
			if (!result && endresult)
				endresult = false;
		}
	}

	this->UpToDate = false;
	return endresult;
}

bool StockNode::DistributeReInvestmentShares(Dividend& div, double& s)
{
	wxDateTime today(wxDateTime::Today());
	if (*this->m_parentclock != wxDateTime::Today())
		this->CalcRatiosOfChildren(&today);

	wxVector<double> ratio;
	for (auto& v : this->bought)
	{
		double val = s * v.GetSiblingRatio();
		if (val)
			v.SetReInvestShares(div, val);
	}

	this->UpToDate = false;
	return true;
}

bool StockNode::UpDate(bool force_update)
{
	wxDateTime today = wxDateTime::Today();
	GetWorkDate(today);
	if (!this->current_Data.date.IsValid() || this->current_Data.date < today || force_update)
	{
		if (!this->parser)
			this->parser = this->NewParser();

		if (force_update)
			this->parser->UpDateSummaryData();
		else
			this->parser->UpDateAll();

		this->DeleteParser();

		return true;
	}
	return true;
}

bool StockNode::Historical_prices_UpToDate()
{
	size_t size = this->historical_prices.size();
	if (!size)
		return false;

	wxDateTime latest = this->GetLatestMarketOpenDate();
	if (this->historical_prices.rbegin()->date == latest)
		return true;

	return false;
}

double StockNode::GetDeviation(int days)
{
	int size = days;
	auto it = this->historical_prices.rbegin();
	if (it == this->historical_prices.rend())
		return 0.0;

	double total = 0.0, standardDiv = 0.0;
	while (it != this->historical_prices.rend() && days)
	{
		--days;
		total += it->close;
		++it;
	}

	if (days != 0)
		size -= days;

	double mean = total / size;

	days = size;
	it = this->historical_prices.rbegin();
	while (it != this->historical_prices.rend() && days)
	{
		--days;
		standardDiv += std::pow((it->close - mean), 2);
		++it;
	}

	return std::sqrt(standardDiv / (size - 1));
}
/*
Day_Prices* StockNode::GetHistoricalIndexIter()
{
	for (auto it = this->historical_prices.begin(); it != this->historical_prices.end(); ++it)
	{
		if (it->date == *this->m_parentclock)
			return it;
	}

	return this->historical_prices.end();
}
*/
int StockNode::m_GetStartWeekDay(wxDateTime* T)
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

wxDateTime StockNode::GetLatestMarketOpenDate()
{
	wxDateTime T = wxDateTime::Today();
	GetWorkDate(T);
	return T;
}

double StockNode::GetClosePrice(wxDateTime* T)
{
	if (!T)
	{
		wxMessageBox("wxDateTime* is null in StockNode::GetClosePrice!");
		return 0.0;
	}

	if (this->historical_prices.rbegin() != this->historical_prices.rend())
		wxString enddate = this->historical_prices.rbegin()->date.Format(STANDARD_DATE);

	for (size_t i = 0; i < this->historical_prices.size(); ++i)
	{
		wxString Tclose = T->Format(STANDARD_DATE);
		wxString close = this->historical_prices[i].date.Format(STANDARD_DATE);
		double closep = this->historical_prices[i].close;
		if (this->historical_prices[i].date == *T)
			return this->historical_prices[i].close;
	}

	return 0.0;
}

double StockNode::GetPreviousClose(wxDateTime* T)
{
	if (!T)
	{
		if (this->historical_prices.rbegin() == this->historical_prices.rend())
			return 0.0;
		auto it = this->historical_prices.rbegin();
		++it;
		if (it == this->historical_prices.rend())
			return 0.0;
		return it->close;
	}

	wxDateTime temp = *T;
	GetWorkDate(temp);
	for (size_t i = 0; i < this->historical_prices.size(); ++i)
	{
		if (this->historical_prices[i].date == temp)
		{
			if (i > 0)
				return this->historical_prices[--i].close;
			else
				return 0.0;
		}
	}

	wxMessageBox("Date not found in StockNode::GetPreviousClose!");
	return 0.0;
}

double StockNode::GetEarliestWeekDayClose(wxDateTime* T)
{
	wxDateTime date = wxDateTime::Today();
	if (T)
		date = *T;

	date = this->GetFirstDayOfTheWeekDate(&date);

	return this->GetClosePrice(&date);
}

double StockNode::GetQuarterStartClose(wxDateTime* T)
{
	wxDateTime date = wxDateTime::Today();
	if (T)
		date = *T;

	date = this->GetQuarterStartDate(&date);
	
	return this->GetClosePrice(&date);
}

wxDateTime StockNode::GetQuarterStartDate(wxDateTime* T)
{
	wxDateTime date = wxDateTime::Today();
	if (T)
		date = *T;
	wxString before = date.Format(STANDARD_DATE);
	date = wxDateTime(1, GetQuarterStartMonth(date.GetMonth()), date.GetYear());
	GetForwardWorkDay(date);
	wxString after = date.Format(STANDARD_DATE);
	return date;
}

double StockNode::GetYearStartClose(wxDateTime* T)
{
	wxDateTime date = wxDateTime::Today();
	if (T)
		date = *T;

	date = wxDateTime(2, wxDateTime::Month::Jan, date.GetYear());
	GetForwardWorkDay(date);
	wxString year = date.Format(STANDARD_DATE);
	return this->GetClosePrice(&date);
}

double StockNode::GetDayCostBasis(wxDateTime* T)
{
	double cost_basis = 0.0;
	double price = 0.0;
	if (wxDateTime::Today() != *T)
		price = this->GetPreviousClose(T);
	else
		price = this->GetPreviousClose(nullptr);
	double temp = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		temp = this->bought[i].GetPurchasePrice(T);
		if (!temp)
			cost_basis += price * this->bought[i].GetShares(T);
		else
			cost_basis += temp * this->bought[i].GetShares(T);
	}

	return cost_basis;
}

double StockNode::GetWeekCostBasis(wxDateTime* T)
{
	if (!T)
	{
		wxMessageBox("wxDateTime* in StockNode::GetWeekCostBasis is nullptr!");
		return 0.0;
	}

	wxDateTime copy = *T;
	double cost_basis = 0.0;
	double price = this->GetEarliestWeekDayClose(&copy);
	double temp = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		double s = this->bought[i].GetShares(&copy);
		temp = this->bought[i].GetPurchasePrice(this->m_GetStartWeekDay(&copy), copy.GetDayOfYear(), copy.GetYear());
		if (!temp)
			cost_basis += price * this->bought[i].GetShares(&copy);
		else
			cost_basis += temp * this->bought[i].GetShares(&copy);
	}

	return cost_basis;
}
double StockNode::GetQuarterCostBasis(wxDateTime* T)
{
	if (!T)
	{
		wxMessageBox("wxDateTime* in StockNode::GetQuarterCostBasis is nullptr!");
		return 0.0;
	}

	wxDateTime copy = *T;
	double cost_basis = 0.0;
	double price = this->GetQuarterStartClose(&copy);
	double temp = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		temp = this->bought[i].GetPurchasePrice(GetQuarterStartMonth(copy.GetMonth()), copy.GetDayOfYear(), copy.GetYear());
		if (!temp)
			cost_basis += price * this->bought[i].GetShares(&copy);
		else
			cost_basis += temp * this->bought[i].GetShares(&copy);
	}

	return cost_basis;
}

double StockNode::GetYearCostBasis(wxDateTime* T)
{
	if (!T)
	{
		wxMessageBox("wxDateTime* in StockNode::GetYearCostBasis is nullptr!");
		return 0.0;
	}
	
	wxDateTime copy = *T;
	double cost_basis = 0.0;
	double price = this->GetYearStartClose(&copy);
	double temp = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		temp = this->bought[i].GetPurchasePrice(copy.GetYear(), copy.GetDayOfYear());
		if (!temp)
			cost_basis += price * this->bought[i].GetShares(&copy);
		else
			cost_basis += temp * this->bought[i].GetShares(&copy);
	}

	return cost_basis;
}

double StockNode::GetTotalCostBasis(wxDateTime* T)
{
	if (!T)
	{
		wxMessageBox("wxDateTime* in StockNode::GetTotalCostBasis is nullptr!");
		return 0.0;
	}

	wxDateTime copy = *T;
	double cost_basis = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
		cost_basis += this->bought[i].GetPurchasePrice() * this->bought[i].GetShares(&copy);

	return cost_basis;
}

double StockNode::GetMarketValue(wxDateTime* T)
{
	if (!T)
	{
		wxMessageBox("wxDateTime* in StockNode::GetMarketValue is nullptr!");
		return 0.0;
	}
	wxDateTime copy = *T;

	if (*T == wxDateTime::Today())
		return this->current_Data.marketprice * this->m_GetShares(T) + this->GetDividendShares();

	GetWorkDate(copy);
	return this->GetClosePrice(&copy) * (this->m_GetShares(&copy) + this->GetDividendShares());
}

void StockNode::m_CheckActive()
{
	for (auto& v : this->bought)
	{
		if (v.IsActive())
		{
			this->m_active = true;
			return;
		}
	}

	this->m_active = false;
}

bool StockNode::m_Purchase(long& id, wxDateTime& date, double& price_per_share, double& shares)
{
	wxDateTime r_date(wxInvalidDateTime);
	if (this->DivReinvest)
		r_date = this->div_reinvest_start_date;
	this->bought.push_back(stock_node(id, this->m_ticker, date, price_per_share, shares, BUY, r_date));
	this->lastPurchase = &this->bought[this->bought.size() - 1];

	if (!this->inital_purchase.IsValid())
		this->inital_purchase = date;
	else
	{
		if (this->inital_purchase > date)
		{
			this->inital_purchase = date;
			this->purchaseDateSwitch = true;
		}
	}
	this->UpToDate = 0;
	return true;
}

bool StockNode::m_Sell(long& id, wxDateTime& date, double& price_per_share, double& shares, stock_node* sn)
{
	wxDateTime r_date(wxInvalidDateTime);
	if (this->DivReinvest)
		r_date = this->div_reinvest_start_date;
	sn->SetSibling(stock_node(id, this->m_ticker, date, price_per_share, shares, SELL, r_date));

	// Signal that the StockNode now is not up to date and needs to be calibrated...
	this->UpToDate = 0;

	// We need to see if all shares were sold for all lots, and if so, we need to make this StockNode inactive so it doesn't get included in the portfolio's calibration...
	this->m_CheckActive();
	return true;
}

stock_node* StockNode::FindLot(long& id)
{
	for (auto& v : this->bought)
	{
		if (v.IdMatch(id))
			return &v;
	}
	
	return NULL;
}

Parser* StockNode::NewParser()
{
	this->DeleteParser();
	wxDateTime* latest_close_date = this->m_GetLatestCloseDate();
	wxDateTime* divdate = this->m_GetLatestDividendDate();
	wxString close_date_string = latest_close_date && !this->purchaseDateSwitch ? latest_close_date->Format(DATE_KEY) : "";
	wxString div_date = divdate ? divdate->Format(DATE_KEY) : "";
	wxString enddate = this->historical_prices.size() && this->purchaseDateSwitch ? this->historical_prices[0].date.Format(DATE_KEY) : "";
	return new Parser(this, this->m_ticker, this->GetInitalPurchaseDate().Format(DATE_KEY), close_date_string, 
		div_date, SetHistoricalDataCB, SetSummaryDataCB, SetDividendsCB, enddate);
}

void StockNode::DeleteParser()
{
	if (this->parser)
		delete this->parser;

	this->parser = nullptr;
}

void StockNode::ShareDivstoLastPurchase()
{
	if (this->lastPurchase)
	{
		for (auto& v : this->bought)
			v.ShareDivsWithSibling(this->lastPurchase);
	}

	this->purchaseDateSwitch = false;
}

double StockNode::m_GetShares(wxDateTime* date)
{
	double shares = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
		shares += this->bought[i].GetShares(date);

	return shares;
}

wxDateTime StockNode::GetFirstDayOfTheWeekDate(wxDateTime* date)
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

wxDateTime* StockNode::m_GetLatestCloseDate()
{
	return this->historical_prices.rbegin() != this->historical_prices.rend() ? &this->historical_prices.rbegin()->date : nullptr;
}

wxDateTime* StockNode::m_GetLatestDividendDate()
{
	wxDateTime* latest = nullptr;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		wxDateTime* temp = this->bought[i].GetLatestDivDate();
		if ((latest && !latest->IsValid()) || !latest)
			latest = temp;
		else
		{
			if (temp && temp->IsValid())
			{
				if (*latest < *temp)
					latest = temp;
			}
		}
	}
	return latest;
}

wxDateTime StockNode::GetInitalPurchaseDate()
{
	wxDateTime date(wxInvalidDateTime);
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		if (!date.IsValid() || date > this->bought[i].GetPurchaseDate())
			date = this->bought[i].GetPurchaseDate();
	}

	return date;
}

wxDateTime* StockNode::m_FindDateInHistoricalPrices(wxDateTime& date)
{
	for (size_t i = 0; i < this->historical_prices.size(); ++i)
	{
		if (this->historical_prices[i].date == date)
			return &this->historical_prices[i].date;
	}

	return nullptr;
}

wxDateTime* StockNode::m_FindDateInHistoricaDivs(wxDateTime& date)
{
	wxDateTime* div_date = nullptr;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		div_date = this->bought[i].GetDividendDates(date);
		if (div_date)
			return div_date;
	}

	return nullptr;
}

void StockNode::SetSummaryData(SummaryData sd)
{
	this->UpToDate = 0;
	this->m_beta = this->current_Data.Beta;
	this->current_Data = sd;
	if (sd.beta == "NotFound")
		this->current_Data.Beta = this->m_beta;
}

void StockNode::SetDividends(Dividend d)
{
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		if (this->bought[i].GetPurchaseDate() < d.ex_Div)
			this->bought[i].HandleDivReInvest(d);
	}
}

void StockNode::SetPaymentDate(Dividend& dividend, wxDateTime& date)
{
	for (auto& v : this->bought)
		v.HandlePaymentDate(dividend, date);

	this->UpToDate = 0;
}

bool StockNode::SetReInvestShares(double& s)
{
	wxDateTime date(wxDateTime::Today());
	double shares = this->m_GetShares(&date);
	shares += this->GetDividendShares(true);
	if (s > shares)
		shares = s - shares;
	else
		shares = s;

	return this->DistributeReInvestmentShares(shares);
}

bool StockNode::SetReInvestShares(Dividend& div, double& d)
{
	wxDateTime date(wxDateTime::Today());
	double shares = d;
	double my_shares = this->m_GetShares(&date);
	my_shares += this->GetDividendShares(true);
	if (shares > my_shares)
		my_shares = shares - my_shares;
	else
		my_shares = shares;

	return this->DistributeReInvestmentShares(div, my_shares);
}

void StockNode::OnThreadComplete(wxCommandEvent&)
{
	this->m_parent->ThreadComplete(this);
}

Day_Prices* StockNode::GetDayPricesOfLastMarketOpen()
{
	if (this->historical_prices.rbegin() != this->historical_prices.rend())
		return &*this->historical_prices.rbegin();

	return NULL;
}

double StockNode::GetDividends()
{
	wxDateTime T = *this->m_parentclock;
	double d = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
	{
		d += this->bought[i].GetDividends(&T);
	}

	return d;
}

wxVector<Dividend> StockNode::GetDividendVec()
{
	wxVector<Dividend> vec;
	for (size_t i = 0; i < this->bought.size(); ++i)
		this->bought[i].GetDividendVec(vec);
	
	return vec;
}

void StockNode::GetDivWithAllReInvestmentShares(Dividend& d)
{
	d.re_invest_shares = 0.0;
	for (auto& v : this->bought)
		v.AddDivSharesToDiv(d);
}

wxVector<Dividend> StockNode::GetAllDividends()
{
	wxVector<Dividend> vec;
	for (auto& v : this->bought)
		v._FillClientDividendVec(vec);

	return vec;
}

StockViewerData* StockNode::GetStockViewerData()
{
	return &this->svd;
}

bool StockNode::PendingReInvestment()
{
	for (auto& v : this->bought)
	{
		if (v.IsPendingDivReInvest())
			return true;
	}

	return false;
}

void StockNode::Save()
{
	DataStream ds("SavedFiles/" + this->m_ticker + ".bin", wxPosixPermissions::wxS_DEFAULT, wxFile::OpenMode::write);
	if (!ds.IsOK())
		return;

	ds.WritewxDateTime(this->inital_purchase);
	ds.WriteBool(this->DivReinvest);

	if (this->DivReinvest)
		ds.WritewxDateTime(this->div_reinvest_start_date);

	ds.WriteSummaryData(this->current_Data);
	ds.WriteData(this->m_beta);
	ds.WriteData(this->m_active);
	ds.WriteDayPricesVector(this->historical_prices);

	size_t size = this->bought.size();
	ds.WriteData(size);
	for (size_t i = 0; i < size; ++i)
		this->bought[i].Save(ds);
}

void StockNode::Retrieve()
{
	DataStream ds("SavedFiles/" + this->m_ticker + ".bin", wxPosixPermissions::wxS_DEFAULT, wxFile::OpenMode::read);
	if (!ds.IsOK())
		return;

	ds.ReadwxDateTime(this->inital_purchase);
	ds.ReadBool(this->DivReinvest);

	if (this->DivReinvest)
		ds.ReadwxDateTime(this->div_reinvest_start_date);

	ds.ReadSummaryData(this->current_Data);
	ds.ReadData(this->m_beta);
	ds.ReadData(this->m_active);
	ds.ReadDayPricesVector(this->historical_prices);

	size_t size;
	ds.ReadData(size);
	for (size_t i = 0; i < size; ++i)
	{
		this->bought.push_back(stock_node(0, this->m_ticker, wxDateTime(wxInvalidDateTime), 0.0, 0.0, Action::UNDEFINED, wxDateTime(wxInvalidDateTime)));
		this->bought[this->bought.size() - 1].Retrieve(ds);
	}

//	this->_90dayDeviation = this->GetDeviation(90);
	this->Calibrate();
}

double StockNode::GetDividendShares(bool today)
{
	wxDateTime T = today ? wxDateTime::Today() : *this->m_parentclock;
	double d = 0.0;
	for (size_t i = 0; i < this->bought.size(); ++i)
		d += this->bought[i].GetDividendShares(&T);

	return d;
}

wxVector<stock_node*> StockNode::GetChildren()
{
	wxVector<stock_node*> v;
	for (auto& it : this->bought)
	{
		if (it.GetShares(NULL))
			v.push_back(&it);
	}

	return v;
}

void StockNode::SetHistoricalData(Day_Prices day)
{
	this->UpToDate = 0;
	size_t i = 0;
	while (i < this->historical_prices.size() && this->historical_prices[i].date < day.date)
		++i;
	if ((i == this->historical_prices.size()) || (i == 0 && !this->historical_prices.size()))
		this->historical_prices.push_back(day);
	else
		this->historical_prices.insert(&this->historical_prices[i], day);
}

// Indices functions...
Indices::Indices(_INDEX_ _index, wxDateTime& date, wxDateTime* parentclock) : Return_node(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0), index(_index), start(date),
	m_parentclock(parentclock)
{
	switch (index)
	{
	case _INDEX_::DOW_JONES: this->ticker = _DOW; break;
	case _INDEX_::NASDAQ: this->ticker = _NAS; break;
	case _INDEX_::SP_500: this->ticker = _SP; break;
	default: wxFAIL_MSG("_INDEX_ passed to Indices constructor does not match in switch statement!"); return;
	}

	//this->Calibrate();
}

Indices::Indices() : Return_node(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
{

}

Indices::~Indices()
{
	this->DeleteParser();
}

void Indices::Calibrate(bool force)
{
	if (this->UpToDate && !force)
		return;
	else
		this->UpDate(force);

	this->_52weekDeviation = GetDeviation(260, this->historical_prices);
	this->_90dayDeviation = GetDeviation(90, this->historical_prices);
	this->_30dayDeviation = GetDeviation(30, this->historical_prices);

	wxDateTime T = *this->m_parentclock;
	wxDateTime copy2 = *this->m_parentclock;
	double daycostbasis = G_GetPreviousClose(&T, this->historical_prices);
	T = *this->m_parentclock;
	double weekcostbasis = GetEarliestWeekDayClose(&T, &this->start, this->historical_prices);
	T = *this->m_parentclock;
	double quartercostbasis = G_GetQuarterStartClose(&T, &this->start, this->historical_prices);
	T = *this->m_parentclock;
	double yearcostbasis = GetYearStartClose(&T, &this->start, this->historical_prices);
	wxDateTime initial = this->start;
	double originalcostbasis = this->historical_prices.size() ? this->historical_prices[0].close : 0.0;
	double marketvalue = G_GetClosePrice(&T, this->historical_prices);
	T = *this->m_parentclock;
	double _shares = 1;
	double divshares = 0;
	double _dividends = 0;

	wxString ex_div = "";

	this->SetNewValues(originalcostbasis, daycostbasis, weekcostbasis, quartercostbasis, yearcostbasis, marketvalue, _shares, divshares, _dividends);
	this->UpToDate = 1;
	double _marketprice = wxDateTime::Today() == *this->m_parentclock ? this->current_Data.marketprice : marketvalue;
	double _previous_close = wxDateTime::Today() == *this->m_parentclock ? this->current_Data.previousclose : daycostbasis;

	wxString t = this->ticker;

	this->svd = StockViewerData(this, this->current_Data.Longname, t, this->current_Data.earningsdate, this->start.Format(STANDARD_DATE),
		SECTOR_INVALID, _shares + divshares, originalcostbasis / _shares, _marketprice, _previous_close,
		this->day_return, this->week_return, this->quarter_return, this->year_return, this->total_return, originalcostbasis,
		marketvalue, _dividends, this->current_Data.Beta, 0.0, ex_div);

	this->UpToDate = 1;
}

void Indices::ClockChange()
{
	UpToDate = 0;
	this->Calibrate();
}

void Indices::SetHistoricalData(Day_Prices day)
{
	this->UpToDate = 0;
	size_t i = 0;
	while (i < this->historical_prices.size() && this->historical_prices[i].date < day.date)
		++i;
	if ((i == this->historical_prices.size()) || (i == 0 && !this->historical_prices.size()))
		this->historical_prices.push_back(day);
	else
		this->historical_prices.insert(&this->historical_prices[i], day);
}

void Indices::SetSummaryData(SummaryData sd)
{
	this->UpToDate = 0;
	this->m_beta = this->current_Data.Beta;
	this->current_Data = sd;
	if (sd.beta == "NotFound")
		this->current_Data.Beta = this->m_beta;
}

StockViewerData* Indices::GetStockViewerData()
{
	return &this->svd;
}

_INDEX_ Indices::GetIndice()
{
	return this->index;
}

void Indices::Save()
{
	DataStream ds("SavedFiles/" + this->ticker + ".bin", wxPosixPermissions::wxS_DEFAULT, wxFile::OpenMode::write);
	if (!ds.IsOK())
		return;

	ds.WritewxDateTime(this->start);

	ds.WriteSummaryData(this->current_Data);
	ds.WriteData(this->m_beta);
	ds.WriteDayPricesVector(this->historical_prices);
}

void Indices::Retrieve()
{
	DataStream ds("SavedFiles/" + this->ticker + ".bin", wxPosixPermissions::wxS_DEFAULT, wxFile::OpenMode::read);
	if (!ds.IsOK())
		return;

	ds.ReadwxDateTime(this->start);

	ds.ReadSummaryData(this->current_Data);
	ds.ReadData(this->m_beta);
	ds.ReadDayPricesVector(this->historical_prices);
}

bool Indices::UpDate(bool force_update)
{
	wxDateTime today = wxDateTime::Today();
	GetWorkDate(today);
	if (!this->current_Data.date.IsValid() || this->current_Data.date < today || force_update)
	{
		if (!this->parser)
			this->parser = this->NewParser();

		if (force_update)
			this->parser->UpDateSummaryData(true);
		else
		{
			this->parser->UpDateSummaryData(true);
			this->parser->UpDateHistoricalPrices(true);
		}

		this->DeleteParser();

		return true;
	}
	return true;
}

Parser* Indices::NewParser()
{
	this->DeleteParser();
	wxDateTime latest_close_date = GetLatestCloseDate(this->historical_prices);
	wxString close_date_string = latest_close_date.IsValid() ? latest_close_date.Format(DATE_KEY) : "";
	wxString enddate = this->historical_prices.size() ? this->historical_prices[0].date.Format(DATE_KEY) : "";
	return new Parser(this, this->ticker, this->start.Format(DATE_KEY), close_date_string, SetHistoricalDataCB, INDICES, SetSummaryDataCB);
}

void Indices::DeleteParser()
{
	if (this->parser)
		delete this->parser;

	this->parser = NULL;
}

// Sector Functions...

Sector::Sector(_Sector s, wxDateTime* date, Portfolio* P) 
	: Return_node(&P->market_value, nullptr, nullptr), id(s), m_parentclock(date), m_parent(P)
{
	SectorClass& sc = GetSectorClass();
	this->Sector_Name = sc.GetSectorString(s);
}

Sector::~Sector()
{

}

bool Sector::Purchase(long lot, wxString ticker, wxString date, double price, double shares, bool div_re, wxString T)
{
	this->sn = this->GetStockNode(ticker);
	if (!this->sn)
		this->sn = this->CreateStockNode(lot, ticker, div_re, T);

	if (!this->sn->Purchase(lot, date, price, shares))
	{
		if (this->sn->IsActive())
		{
			this->sn = NULL;
			this->stocks.erase(this->sn);
		}
		return false;
	}

	return true;
}

bool Sector::AddToPosition(long& lot, wxString& date, double& price, double& shares, bool& div_re, wxString& T)
{
	if (!this->sn)
	{
		wxFAIL_MSG("this->sn in Sector::AddToPosition is nullptr!");
		return false;
	}

	if (!this->sn->Purchase(lot, date, price, shares))
	{
		if (this->sn->IsActive())
		{
			this->sn = NULL;
			this->stocks.erase(this->sn);
		}
		return false;
	}

	return true;
}

bool Sector::Sell(long& lot, wxDateTime& d, double& shares, double& price)
{
	if (!this->sn)
		wxMessageBox("this->sn is NULL in Sector::Sell!");

	this->sn = this->GetStockNode(lot);
	if (!this->sn)
	{
		wxMessageBox("this->sn is NULL in Sector::Sell! Returning false!");
		return false;
	}

	bool result = this->sn->Sell(lot, d, price, shares);
	return result;
}

bool Sector::AddReInvestShares(double& shares)
{
	if (!this->sn)
	{
		wxMessageBox("this->sn is NULL in Sector::AddReInvestShares(double&)!");
		return false;
	}

	return this->sn->SetReInvestShares(shares);
}

bool Sector::AddReInvestShares(Dividend& div, double& shares)
{
	if (!this->sn)
	{
		wxMessageBox("this->sn is NULL in Sector::AddReInvestShares(Dividend&, double&)!");
		return false;
	}

	return this->sn->SetReInvestShares(div, shares);
}

bool Sector::AddDividend(Dividend& div)
{
	if (!this->sn)
	{
		wxMessageBox("this->sn is NULL in Sector::AddDividend!");
		return false;
	}

	this->sn->SetDividends(div);
	return true;
}

void Sector::AddDividendPaymentDate(Dividend& dividend, wxDateTime& date)
{
	if (!this->sn)
	{
		wxMessageBox("this->sn is NULL in Sector::AddDividendPaymentDate!");
		return;
	}

	this->sn->SetPaymentDate(dividend, date);
}

void Sector::GetDivWithAllReInvestmentShares(Dividend& d)
{
	if (!this->sn)
	{
		wxMessageBox("this->sn is NULL in Sector::GetDivWithAllReInvestmentShares!");
		return;
	}

	this->sn->GetDivWithAllReInvestmentShares(d);
}

wxVector<Dividend> Sector::GetDividendsFromStagedStock()
{
	if (!this->sn)
	{
		wxMessageBox("this->sn is NULL in Sector::GetAllDividends!");
		return wxVector<Dividend>();
	}

	return this->sn->GetAllDividends();
}

bool Sector::IsId(_Sector s)
{
	if (this->id == s)
		return true;

	return false;
}

wxVector<StockNode*> Sector::GetStockVector()
{
	wxVector<StockNode*> sn;
	for (size_t i = 0; i < this->stocks.size(); ++i)
	{
		if (this->stocks[i].IsActive())
			sn.push_back(&this->stocks[i]);
	}

	return sn;
}

wxVector<stock_node*> Sector::GetAllLotData()
{
	wxVector<stock_node*> v;
	return v;
}

wxVector<stock_node*> Sector::GetLotData()
{
	if (!this->sn)
	{
		wxMessageBox("this->sn is NULL in Sector::GetLotData!");
		wxVector<stock_node*> V;
		return V;
	}

	return this->sn->GetChildren();
}

Pair Sector::GetPair()
{
	if (!this->sn)
	{
		wxFAIL_MSG("this->sn is NULL in Sector::GetPair()!");
		return Pair("", 0.0);
	}

	Pair p(this->sn->GetTicker(), this->sn->GetDividends());
	this->sn = NULL;
	return p;
}

void Sector::Calibrate(bool datechange)
{
	this->Reset();
	for (size_t i = 0; i < this->stocks.size(); ++i)
	{
		if (datechange)
			this->stocks[i].ClockChange();
		else
			this->stocks[i].Calibrate();
		if (this->stocks[i].IsActive())
			this->AddNewValues(stocks[i].GetReturnNode());
	}

	this->ReturnCalibarate();
	this->svd = StockViewerData(this, "", this->Sector_Name, "", "",
		this->GetID(), this->shares + this->div_shares, this->original_cost_basis / this->shares, this->market_value / this->shares, 0.0,
		this->day_return, this->week_return, this->quarter_return, this->year_return, this->total_return, this->original_cost_basis,
		this->market_value, this->dividends, 0.0);
}

const StockViewerData* Sector::GetStockViewerData() const
{
	return &this->svd;
}

void Sector::OnClockChange()
{
	this->threads_running = 0;
	wxVector<StockThread*> threads;
	for (auto& v : this->stocks)
	{
		threads.push_back(new StockThread(wxThreadKind::wxTHREAD_DETACHED, this, &v));
		++this->threads_running;
	}

	for (auto& v : threads)
		v->Run();
}

void Sector::CalcRatiosOfChildren()
{
	this->beta = 0.0;
	for (size_t i = 0; i < this->stocks.size(); ++i)
	{
		this->stocks[i].CalcRatios();
		this->beta += this->stocks[i].GetReturnNode()->sector_beta;
	}

	this->svd.beta = beta;
	this->CalcRatios();
}

void Sector::Save()
{
	DataStream ds("SavedFiles/" + this->Sector_Name + ".bin", wxPosixPermissions::wxS_DIR_DEFAULT, wxFile::OpenMode::write);
	if (!ds.IsOK())
		return;

	wxVector<wxString> v;
	for (size_t i = 0; i < this->stocks.size(); ++i)
	{
		v.push_back(this->stocks[i].GetTicker());
		this->stocks[i].Save();
	}

	ds.WriteStringwxVector(v);
}

void Sector::Retrieve()
{
	DataStream ds("SavedFiles/" + this->Sector_Name + ".bin", wxPosixPermissions::wxS_DIR_DEFAULT, wxFile::OpenMode::read);
	if (!ds.IsOK())
		return;

	wxVector<wxString> v;
	ds.ReadStringwxVector(v);
	for (size_t i = 0; i < v.size(); ++i)
	{
		this->stocks.push_back(StockNode(v[i], this->m_parentclock, this->m_parent, this));
		this->stocks[this->stocks.size() - 1].Retrieve();
	}
}

void Sector::Update()
{
	for (size_t i = 0; i < this->stocks.size(); ++i)
		this->stocks[i].Calibrate(true);
}

bool Sector::IsStockActive(wxString& ticker) const
{
	if (this->sn)
	{
		if (this->sn->GetTicker() == ticker)
			return this->sn->IsActive();
	}

	for (auto& v : this->stocks)
	{
		if (v.GetTicker() == ticker)
			return v.IsActive();
	}

	wxFAIL_MSG("this->sn was nullptr or ticker is not equal to this->sn->GetTicker() in Sector::IsStockActive!");
	return true;
}

bool Sector::IsActive() const
{
	for (auto& v : this->stocks)
	{
		if (v.IsActive())
			return true;
	}

	return false;
}

wxString Sector::GetSectorName() const
{
	return this->Sector_Name;
}

void Sector::ThreadComplete(StockNode* sn)
{
	this->threads_running--;
	if (sn->IsActive())
		this->AddNewValues(sn->GetReturnNode());

	if (!this->threads_running)
	{
		this->ReturnCalibarate();
		this->svd = StockViewerData(this, "", this->Sector_Name, "", "",
			this->GetID(), this->shares, this->original_cost_basis / this->shares, this->market_value / this->shares, 0.0,
			this->day_return, this->week_return, this->quarter_return, this->year_return, this->total_return, this->original_cost_basis,
			this->market_value, this->dividends, 0.0);

			this->m_parent->ThreadComplete(this);
	}
}

bool Sector::IsChild(wxString& ticker)
{
	this->sn = NULL;
	this->sn = this->GetStockNode(ticker);

	return sn ? true : false;
}

wxString Sector::GetLongNameOfStock(wxString& t)
{
	if (IsChild(t))
		this->sn->GetLongName();

	return "";
}

wxString Sector::GetLongNameOfStock()
{
	if (!this->sn)
	{
		wxFAIL_MSG("this->sn in Sector::GetLongNameOfStock is nullptr!");
		return "";
	}

	return this->sn->GetLongName();
}

bool Sector::IsChild(long& id)
{
	this->sn = NULL;
	this->sn = this->GetStockNode(id);

	return sn ? true : false;
}

int Sector::GetNumItems(_PortfolioType p)
{
	switch (p)
	{
	case _PortfolioType::STOCK: return this->GetStockSize();
	}

	return 0;
}

wxDateTime Sector::GetEarliestPurchaseDate()
{
	wxDateTime date;
	for (auto& v : this->stocks)
	{
		if (!date.IsValid())
			date = v.GetPurchaseDate();
		else
		{
			if (date > v.GetPurchaseDate())
				date = v.GetPurchaseDate();
		}
	}

	return date;
}

// Sector private functions...

StockNode* Sector::GetStockNode(wxString ticker)
{
	for (size_t i = 0; i < this->stocks.size(); ++i)
	{
		if (this->stocks[i].TickerMatch(ticker))
			return &this->stocks[i];
	}

	return nullptr;
}

StockNode* Sector::GetStockNode(long& id)
{
	for (auto& v : this->stocks)
	{
		if (v.IdMatch(id))
			return &v;
	}

	return NULL;
}

StockNode* Sector::CreateStockNode(long l, wxString ticker, bool div_re, wxString T)
{
	this->stocks.push_back(StockNode(ticker, this->m_parentclock, this->m_parent, this, div_re, T));
	return &this->stocks[this->stocks.size() - 1];
}

int Sector::GetStockSize()
{
	return this->stocks.size();
}

// Portfolio Functions...

Portfolio::Portfolio(wxFrame* f, double money, wxDateTime* m) 
	: Return_node(nullptr, nullptr, nullptr), m_parent(f), m_parentclock(m)//, dow_jones("DJI", m_parentclock, this, nullptr, false, ""),
	//nasdaq("Nasdaq", m_parentclock, this, nullptr, false, "")
{
	Holidays& h = GetHolidays();
	h.Retrieve();

	wxString data;
	Parser parser(data);
	this->gainers = parser.GetStockGainers();
	this->losers = parser.GetStockLosers();
}

bool Portfolio::Purchase(_Sector s, wxString ticker, wxString purchasedate, double price, double shares, bool div_re, wxString T)
{
	long lotnumber = this->GetLot();
	wxDateTime date;
	date.ParseDate(purchasedate);
	if (!this->cash.RequestPurchase(lotnumber, price * shares))
	{
		this->DeleteLot(lotnumber);
		return false;
	}

	if (!this->m_Purchase(lotnumber, s, ticker, purchasedate, price, shares, div_re, T))
	{
		this->DeleteLot(lotnumber);
		return false;
	}

	this->Calibrate();
	return true;
}

bool Portfolio::AddToPosition(wxString purchasedate, double price, double shares, bool div_re, wxString T)
{
	long lotnumber = this->GetLot();
	wxDateTime date;
	date.ParseDate(purchasedate);
	if (!this->cash.RequestPurchase(lotnumber, price * shares))
	{
		this->DeleteLot(lotnumber);
		return false;
	}

	if (!this->m_AddToPosition(lotnumber, purchasedate, price, shares, div_re, T))
	{
		this->DeleteLot(lotnumber);
		return false;
	}

	this->Calibrate();
	return true;
}

bool Portfolio::NewDepositSchedule(double d, int i, wxDateTime date)
{
	bool result = this->cash.NewDepositSchedule(d, i, date);
	if (result)
		this->cash.UpdateCash();

	return result;
}

bool Portfolio::DoesTickerExist(wxString& t)
{
	return this->RequestSell(t);
}

bool Portfolio::StageStock(wxString& t)
{
	this->sec = NULL;
	sec = this->GetSector(t);

	return sec ? true : false;
}

wxString Portfolio::StageStockandGetLongName(wxString& t)
{
	if (this->StageStock(t))
		return this->sec->GetLongNameOfStock();

	return "";
}

bool Portfolio::RequestSell(wxString& ticker)
{
	this->sec = NULL;
	sec = this->GetSector(ticker);
	
	return sec ? true : false;
}

bool Portfolio::Sell(long& lot, wxDateTime& d, double& shares, double& price)
{
	if (!this->sec)
		wxMessageBox("this->sec is NULL in Portfolio::Sell!");

	this->sec = this->GetSector(lot);
	if (!this->sec)
	{
		wxString id = "";
		id << lot;
		wxMessageBox("this->sec is NULL in Portfolio::Sell! lot passed to function is: " + id);
		return false;
	}

	bool result = this->sec->Sell(lot, d, shares, price);
	if (result)
	{
		Pair p = this->sec->GetPair();
		bool result = cash.CompleteSale(p, d, price * shares, lot);
		this->Calibrate();
		return result;
	}

	return false;
}

bool Portfolio::AddReInvestShares(double& shares)
{
	if (!this->sec)
	{
		wxMessageBox("this->sec is NULL in Portfolio::AddReInvestShares(double&)!");
		return false;
	}

	if (this->sec->AddReInvestShares(shares))
	{
		this->Calibrate();
		return true;
	}

	return false;
}

bool Portfolio::AddReInvestShares(Dividend& div, double& shares)
{
	if (!this->sec)
	{
		wxMessageBox("this->sec is NULL in Portfolio::AddReInvestShares(Dividend&, double&)!");
		return false;
	}

	if (this->sec->AddReInvestShares(div, shares))
	{
		this->Calibrate();
		return true;
	}

	return false;
}

bool Portfolio::AddDividend(Dividend& div)
{
	if (!this->sec)
	{
		wxMessageBox("this->sec is NULL in Portfolio::AddDividend!");
		return false;
	}

	if (this->sec->AddDividend(div))
	{
		this->Calibrate();
		return true;
	}

	return false;
}

void Portfolio::SetDividendPaymentDate(Dividend& dividend, wxDateTime& date)
{
	if (!this->sec)
	{
		wxMessageBox("this->sec is NULL in Portfolio::AddDividendPaymentDate!");
		return;
	}

	this->sec->AddDividendPaymentDate(dividend, date);
	this->Calibrate();
}

void Portfolio::GetDivWithAllReInvestmentShares(Dividend& d)
{
	if (!this->sec)
	{
		wxMessageBox("this->sec is NULL in Portfolio::GetDivWithAllReInvestmentShares!");
		return;
	}

	this->sec->GetDivWithAllReInvestmentShares(d);
}

wxVector<Dividend> Portfolio::GetDividendsFromStagedStock()
{
	if (!this->sec)
	{
		wxMessageBox("this->sec is NULL in Portfolio::GetDividendsFromStagedStock!");
		return wxVector<Dividend>();
	}

	return this->sec->GetDividendsFromStagedStock();
}

wxVector<deposit_pair> Portfolio::GetDepositVector()
{
	return this->cash.GetDepositVector();
}

wxVector<stock_node*> Portfolio::GetLotData()
{
	if (!this->sec)
	{
		wxMessageBox("this->sec in Portfolio::GetLotData is NULL!");
		wxVector<stock_node*> sn;
		return sn;
	}

	return this->sec->GetLotData();
}

int Portfolio::GetNumItems(_PortfolioType p)
{
	int num = 0;
	for (size_t i = 0; i < this->sectors.size(); ++i)
		num += this->sectors[i].GetNumItems(p);

	return num;	
}

wxDateTime Portfolio::GetEarliestPurchaseDate()
{
	wxDateTime earliest;
	for (auto& v : this->sectors)
	{
		if (!earliest.IsValid())
			earliest = v.GetEarliestPurchaseDate();
		else
		{
			if (earliest > v.GetEarliestPurchaseDate())
				earliest = v.GetEarliestPurchaseDate();
		}
	}

	return earliest;
}

const wxVector<StockNode*> Portfolio::GetStockNodeItems()
{
	wxVector<StockNode*> sn;
	wxVector<StockNode*> returnvec;
	for (size_t i = 0; i < this->sectors.size(); ++i)
	{
		sn = this->sectors[i].GetStockVector();
		for (size_t j = 0; j < sn.size(); ++j)
			returnvec.push_back(sn[j]);
	}

	return returnvec;
}

wxVector<Indices*> Portfolio::GetIndices()
{
	wxVector<Indices*> vec;
	vec.push_back(&this->dow);
	vec.push_back(&this->nas);
	vec.push_back(&this->sp);
	return vec;
}

void Portfolio::Calibrate(bool datechange)
{
	this->Reset();
	if (datechange)
	{
		this->dow.ClockChange();
		this->nas.ClockChange();
		this->sp.ClockChange();
	}
	else
	{
		this->dow.Calibrate();
		this->nas.Calibrate();
		this->sp.Calibrate();
	}
	for (size_t i = 0; i < this->sectors.size(); ++i)
	{
		this->sectors[i].Calibrate(datechange);
		this->AddNewValues(this->sectors[i].GetReturnNode());
	}

	this->ReturnCalibarate();
	double temp_beta = this->CalcRatiosOfAllChildren();

	this->svd = StockViewerData(this, "", "Total", "", "",
		_Sector::SECTOR_INVALID, this->shares + this->div_shares, this->original_cost_basis / this->shares, this->market_value / this->shares, 0.0,
		this->day_return, this->week_return, this->quarter_return, this->year_return, this->total_return, this->original_cost_basis,
		this->market_value, this->dividends, temp_beta, this->cash.GetFreeCash());
}

double Portfolio::CalcRatiosOfAllChildren()
{
	this->beta = 0.0;
	for (auto& v : this->sectors)
	{
		v.CalcRatiosOfChildren();
		this->beta += v.GetReturnNode()->portfolio_beta;
	}

	if (this->GetFreeCash())
		this->PortfolioPerc = this->market_value / this->GetFreeCash();

	return beta;
}

StockViewerData* Portfolio::GetStockViewerData()
{
	return &this->svd;
}

void Portfolio::Save()
{
	this->cash.Save();
	DataStream ds("SavedFiles/Portfolio.bin", wxPosixPermissions::wxS_DEFAULT, wxFile::OpenMode::write);
	if (!ds.IsOK())
		return;

	ds.WriteLongwxVector(this->lot);
	wxVector<_Sector> v;
	for (size_t i = 0; i < this->sectors.size(); ++i)
		v.push_back(sectors[i].GetID());

	ds.WritewxVector(v);

	for (size_t i = 0; i < this->sectors.size(); ++i)
		this->sectors[i].Save();

	this->dow.Save();
	this->nas.Save();
	this->sp.Save();
}

void Portfolio::Retrieve()
{
	this->cash.Retrieve();
	DataStream ds("SavedFiles/Portfolio.bin", wxPosixPermissions::wxS_DEFAULT, wxFile::OpenMode::read);
	if (!ds.IsOK())
		return;

	ds.ReadLongwxVector(this->lot);
	wxVector<_Sector> v;
	ds.ReadwxVector(v);

	for (size_t i = 0; i < v.size(); ++i)
	{
		Sector* S = this->CreateSector(v[i]);
		S->Retrieve();
	}
	wxDateTime earliest = this->GetEarliestPurchaseDate();
	dow = Indices(DOW_JONES, earliest, m_parentclock);
	nas = Indices(NASDAQ, earliest, m_parentclock);
	sp = Indices(SP_500, earliest, m_parentclock);

	this->dow.Retrieve();
	this->nas.Retrieve();
	this->sp.Retrieve();

	this->Calibrate();
}

double Portfolio::GetFreeCash()
{
	return this->cash.GetFreeCash();
}

wxVector<DayGainersandLosers> Portfolio::GetDayGainers()
{
	return this->gainers;
}

wxVector<DayGainersandLosers> Portfolio::GetDayLosers()
{
	return this->losers;
}

SummaryData Portfolio::QuoteLookup(wxString ticker)
{
	Parser parser(ticker);
	SummaryData sd =  parser.GetSummaryData();
	sd.description = parser.GetDescription(ticker);
	return sd;
}

bool Portfolio::AddDeposit(wxDateTime& date, double& amount)
{
	return this->cash.Deposit(date, amount);
}

bool Portfolio::AddWithdrawl(wxDateTime& date, double& amount)
{
	return this->cash.Withdrawl(date, amount);
}

void Portfolio::ForceUpdate()
{
	this->Calibrate(true);
}

void Portfolio::DateChange()
{
	this->Calibrate(true);
	return;

	this->threads_running = 0;

	this->dow.ClockChange();
	this->nas.ClockChange();
	this->sp.ClockChange();

	for (auto& v : this->sectors)
	{
		v.OnClockChange();
		++this->threads_running;
	}
}

void Portfolio::Update()
{
	this->DateChange();
}

void Portfolio::ThreadComplete(Sector* s)
{
	this->threads_running--;
	this->AddNewValues(s->GetReturnNode());

	if (!this->threads_running)
	{
		this->ReturnCalibarate();
		this->CalcRatiosOfAllChildren();

		this->svd = StockViewerData(this, "", "Total", "", "",
			_Sector::SECTOR_INVALID, this->shares, this->original_cost_basis / this->shares, this->market_value / this->shares, 0.0,
			this->day_return, this->week_return, this->quarter_return, this->year_return, this->total_return, this->original_cost_basis,
			this->market_value, this->dividends, this->cash.GetFreeCash());

		wxPostEvent(this->m_parent, wxThreadEvent());
	}
}

void Portfolio::OnThreadRetrieveComplete(Sector* s)
{
	this->threads_running--;
	if (!this->threads_running)
	{
		this->Calibrate();
		wxPostEvent(this->m_parent, wxThreadEvent(1));
	}
}

bool Portfolio::IsStockActive(wxString& ticker)
{
	if (this->sec)
	{
		return this->sec->IsStockActive(ticker);
	}

	wxFAIL_MSG("this->sec is nullptr in Portfolio::IsStockActive!");
	return true;
}

wxString Portfolio::GetSectorName(wxString& ticker)
{
	Sector* S = this->GetSector(ticker);
	if (!S)
	{
		wxMessageBox("Cannot find Sector in Portfolio::GetSectorName!");
		return _SECTOR_INVALID;
	}
	
	return S->GetSectorName();
}

const Sector* Portfolio::GetConstSector(wxString& ticker) 
{
	const Sector* s = this->GetSector(ticker);
	return s;
}

// Portfolio private functions...

bool Portfolio::m_AddToPosition(long& lot, wxString& purchasedate, double& price, double& shares, bool& div_re, wxString& T)
{
	if (!this->sec)
	{
		wxFAIL_MSG("this->sec in Portfolio::m_AddToPosition is nullptr!");
		return false;
	}

	if (!this->sec->AddToPosition(lot, purchasedate, price, shares, div_re, T))
		return false;

	wxDateTime date;
	date.ParseDate(purchasedate);
	bool result = this->cash.Purchase(lot, price * shares, date);
	if (result)
	{
		this->cash.AddDividends(this->sec->GetPair());
		return true;
	}

	return result;
}

long Portfolio::GetLot()
{
	long increment = 1;
	if (!lot.size())
	{
		lot.push_back(increment);
		return increment;
	}

	for (size_t i = 0; i < this->lot.size(); ++i)
	{
		if (lot[i] != increment)
		{
			lot.insert(&lot[i], increment);
			return increment;
		}
		++increment;
	}

	lot.push_back(increment);
	return increment;
}

void Portfolio::DeleteLot(long& l)
{
	for (size_t i = 0; i < this->lot.size(); ++i)
	{
		if (lot[i] == l)
		{
			lot.erase(&lot[i]);
			return;
		}
	}

	wxMessageBox("lot failed to be erased in Portfolio::DeleteLot!");
}

bool Portfolio::m_Purchase(long lotnumber, _Sector s, wxString ticker, wxString purchasedate, double price, double shares, bool div_re, wxString T)
{
	Sector* S = this->GetSector(s);
	if (!S)
	{
		S = this->CreateSector(s);
	}

	if (!S->Purchase(lotnumber, ticker, purchasedate, price, shares, div_re, T))
		return false;

	wxDateTime date;
	date.ParseDate(purchasedate);
	bool result = this->cash.Purchase(lotnumber, price * shares, date);
	if (result)
	{
		this->cash.AddDividends(S->GetPair());
		return true;
	}

	return result;
}

Sector* Portfolio::GetSector(_Sector s)
{
	for (size_t i = 0; i < this->sectors.size(); ++i)
	{
		if (this->sectors[i].IsId(s))
			return &this->sectors[i];
	}

	return nullptr;
}

Sector* Portfolio::GetSector(wxString& ticker)
{
	for (auto& v : this->sectors)
	{
		if (v.IsChild(ticker))
			return &v;
	}

	return NULL;
}

Sector* Portfolio::GetSector(long& lot)
{
	for (auto& v : this->sectors)
	{
		if (v.IsChild(lot))
			return &v;
	}

	return NULL;
}

Sector* Portfolio::CreateSector(_Sector s)
{
	this->sectors.push_back(Sector(s, this->m_parentclock, this));
	return &this->sectors[this->sectors.size() - 1];
}