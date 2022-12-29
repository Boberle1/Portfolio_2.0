#pragma once

constexpr auto STANDARD_DATE = "%m/%d/%G";
constexpr auto STANDARD_TIME = "%H:%M:%S";
constexpr auto DATE_KEY = "%G-%m-%d";

constexpr auto _DOW = "DJI";
constexpr auto _NAS = "IXIC";
constexpr auto _SP = "GSPC";

constexpr int DayValue = 86400000;

// Market close
const wxDateTime _MarketClose(wxDateTime::wxDateTime_t(15));
const wxDateTime _MarketOpen(wxDateTime::wxDateTime_t(8), 30);

// expressions to be inserted into FizViz url ids to get different information...
auto constexpr _INSERTION = "INSERTION";
auto constexpr _INSERT_TYPE = "TYPE";
auto constexpr _OVERVIEW = "110";
auto constexpr _VALUATION = "120";
auto constexpr _PERFORMANCE = "140";

// defining valuation dividers
auto constexpr LARGECAPBEGIN	= 10000000000;
auto constexpr MIDCAPBEGIN		= 2000000000;
auto constexpr SMALLCAPBEGIN	= 300000000;

// words to insert into url for fetching idustrial(subsector) data
auto constexpr _bASIC_MATERIALS = "basicmaterials";
auto constexpr _cOMMUNICATION_SERVICES = "communicationservices";
auto constexpr _cONSUMER_DESCRETIONARY = "consumercyclical";
auto constexpr _cONSUMER_STAPLES = "consumerdefensive";
auto constexpr _eNERGY = "energy";
auto constexpr _fINANCIAL = "financial";
auto constexpr _hEALTH_CARE = "healthcare";
auto constexpr _iNDUSTRIALS = "industrials";
auto constexpr _rEAL_ESTATE = "realestate";
auto constexpr _tECHNOLOGY = "technology";
auto constexpr _uTILITIES = "utilities";

// words to insert int url for fetching stocks from a certain industries...
auto constexpr ms_bASIC_MATERIALS = "ms_basic_materials";
auto constexpr ms_cOMMUNICATION_SERVICES = "ms_communication_services";
auto constexpr ms_cONSUMER_DESCRETIONARY = "ms_consumer_cyclical";
auto constexpr ms_cONSUMER_STAPLES = "ms_consumer_defensive";
auto constexpr ms_eNERGY = "ms_energy";
auto constexpr ms_fINANCIAL = "ms_financial_services";
auto constexpr ms_hEALTH_CARE = "ms_healthcare";
auto constexpr ms_iNDUSTRIALS = "ms_industrials";
auto constexpr ms_rEAL_ESTATE = "ms_real_estate";
auto constexpr ms_tECHNOLOGY = "ms_technology";
auto constexpr ms_uTILITIES = "ms_utilities";

// etf is its own subsector
auto constexpr _ETF = "Exchange Traded Fund";

auto constexpr _COMMUNICATION_SERVICES = "Communication Services";
auto constexpr _CONSUMER_DESCRETIONARY = "Consumer Discretionary", _CONSUMER_DESCRETIONARY_ALTERNATE = "Consumer Cyclical";
auto constexpr _CONSUMER_STAPLES = "Consumer Staples", _CONSUMER_STAPLES_ALTERNATE = "Consumer Defensive";
auto constexpr _ENERGY = "Energy";
auto constexpr _FINANCIALS = "Financial";
auto constexpr _HEALTH_CARE = "Healthcare";
auto constexpr _INDUSTRIALS = "Industrials";
auto constexpr _INFORMATION_TECHNOLOGY = "Technology";
auto constexpr _MATERIALS = "Basic Materials";
auto constexpr _REAL_ESTATE = "Real Estate";
auto constexpr _UTILITIES = "Utilities";

// COMMUNICATION SERVICES...
auto constexpr _ADVERTISING_AGENCIES = "Advertising Agencies";
auto constexpr _BRODCASTING = "Broadcasting";
auto constexpr _ELECTRONIC_GAMING_AND_MULTIMEDIA = "Electronic Gaming & Multimedia";
auto constexpr _ENTERTAINMENT = "Entertainment";
auto constexpr _INTERNET_CONTENT_AND_CREATION = "Internet Content & Information";
auto constexpr _PUBLISHING = "Publishing";
auto constexpr _TELECOM_SERVICES = "Telecom Services";

// CONSUMER DESCRETIONARY
auto constexpr _APPAREL_MANUFACTURING = "Apparel Manufacturing";
auto constexpr _APPAREL_RETAIL = "Apparel Retail";
auto constexpr _AUTO_TRUCK_AND_DEALERSHIPS = "Auto & Truck Dealerships";
auto constexpr _AUTO_MANUFACTURERS = "Auto Manufacturers";
auto constexpr _AUTO_PARTS = "Auto Parts";
auto constexpr _DEPARTMENT_STORES = "Department Stores";
auto constexpr _FOOTWEAR_AND_ACCESSORIES = "Footwear & Accessories";
auto constexpr _FURNISHIN_FIXTURES_AND_APPLIANCES = "Furnishings, Fixtures & Appliances";
auto constexpr _GAMBLING = "Gambling";
auto constexpr _HOME_IMPROVMENT_RETAIL = "Home Improvement Retail";
auto constexpr _INTERNET_RETAIL = "Internet Retail";
auto constexpr _LEISURE = "Leisure";
auto constexpr _LODGING = "Lodging";
auto constexpr _LUXURY_GOODS = "Luxury Goods";
auto constexpr _PACKAGING_AND_CONTAINERS = "Packaging & Containers";
auto constexpr _PERSONAL_SERVICES = "Personal Services";
auto constexpr _RECREATIONAL_VEHICLES = "Recreational Vehicles";
auto constexpr _RESIDENTIAL_CONSTRUCTION = "Residential Construction";
auto constexpr _RESORTS_AND_CASINOS = "Resorts & Casinos";
auto constexpr _RESTAURANTS = "Restaurants";
auto constexpr _SPECIALTY_RETAIL = "Specialty Retail";
auto constexpr _TEXTILE_MANUFACTURING = "Textile Manufacturing";
auto constexpr _TRAVEL_SERVICES = "Travel Services";

// CONSUMER STAPLES...
auto constexpr _BEVERAGES_BREWERS = "Beverages - Brewers";
auto constexpr _BEVERAGES_NON_ALCOHOLIC = "Beverages - Non-Alcoholic";
auto constexpr _BEVERAGES_WINERIES_AND_DESTILLERIES = "Beverages - Wineries & Distilleries";
auto constexpr _CONFECTIONERS = "Confectioners";
auto constexpr _DISCOUNT_STORES = "Discount Stores";
auto constexpr _EDUCATION_AND_TRAINING_SERVICES = "Education & Training Services";
auto constexpr _FARM_PRODUCTS = "Farm Products";
auto constexpr _FOOD_DISTRIBUTION = "Food Distribution";
auto constexpr _GROCERY_STORES = "Grocery Stores";
auto constexpr _HOUSEHOLD_AND_PERSONAL_PRODUCTS = "Household & Personal Products";
auto constexpr _PACKEDGED_FOODS = "Packaged Foods";
auto constexpr _TOBACCO = "Tobacco";

// ENERGY...
auto constexpr _OIL_AND_GAS_DRILLING = "Oil & Gas Drilling";
auto constexpr _OIL_AND_GAS_E_AND_P = "Oil & Gas E&P";
auto constexpr _OIL_AND_GAS_EQUIPMENT_AND_SERVICES = "Oil & Gas Equipment & Services";
auto constexpr _OIL_AND_GAS_INTEGRATED = "Oil & Gas Integrated";
auto constexpr _OIL_AND_GAS_MIDSTREAM = "Oil & Gas Midstream";
auto constexpr _OIL_AND_GAS_REFINING_AND_MARKETING = "Oil & Gas Refining & Marketing";
auto constexpr _THERMAL_COAL = "Thermal Coal";
auto constexpr _URANIUM = "Uranium";

// FINANCIALS...
auto constexpr _ASSET_MANAGEMENT = "Asset Management";
auto constexpr _BANKS_DIVERSIFIED = "Banks - Diversified";
auto constexpr _BANKS_REGIONAL = "Banks - Regional";
auto constexpr _CAPITAL_MARKETS = "Capital Markets";
auto constexpr _CREDIT_SERVICES = "Credit Services";
auto constexpr _FINANCIAL_CONGLOMERATES = "Financial Conglomerates";
auto constexpr _FINANCIAL_DATA_AND_STOCK_EXCHANGES = "Financial Data & Stock Exchanges";
auto constexpr _INSURANCE_DIVERSIFIED = "Insurance - Diversified";
auto constexpr _INSURANCE_LIFE = "Insurance - Life";
auto constexpr _INSURANCE_PROPERTY_AND_CASUALTY = "Insurance - Property & Casualty";
auto constexpr _INSURANCE_REINSURANCE = "Insurance - Reinsurance";
auto constexpr _INSURANCE_SPECIALTY = "Insurance - Specialty";
auto constexpr _INSURANCE_BROKERS = "Insurance Brokers";
auto constexpr _MORTGAGE_FINANCE = "Mortgage Finance";
auto constexpr _SHELL_COMPANIES = "Shell Companies";

// HEALTH CARE...
auto constexpr _BIOTECHNOLOGY = "Biotechnology";
auto constexpr _DIAGNOSTICS_AND_RESEARCH = "Diagnostics & Research";
auto constexpr _DRUG_MANUFACTURES_GENERAL = "Drug Manufacturers - General";
auto constexpr _DRUG_MANUFACTURES_SPECIALTY_AND_GENERIC = "Drug Manufacturers - Specialty & Generic";
auto constexpr _HEALTH_INFORMATION_AND_SERVICES = "Health Information Services";
auto constexpr _HEALTH_CARE_PLANS = "Healthcare Plans";
auto constexpr _MEDICAL_CARE_FACILITIES = "Medical Care Facilities";
auto constexpr _MEDICAL_DEVICES = "Medical Devices";
auto constexpr _MEDICAL_DISTRIBUTION = "Medical Distribution";
auto constexpr _MEDICAL_INSTRUMENTS_AND_SUPPLIES = "Medical Instruments & Supplies";
auto constexpr _PHARMECUTICALS_RETAILERS = "Pharmaceutical Retailers";

// INDUSTRIALS...
auto constexpr _AEROSPACE_AND_DEFENSE = "Aerospace & Defense";
auto constexpr _AIRLINES = "Airlines";
auto constexpr _AIRPORTS_AND_SERVICES = "Airports & Air Services";
auto constexpr _BUILDING_PRODUCTS_ANDE_EQUIPMENT = "Building Products & Equipment";
auto constexpr _BUSINESS_EQUIPMENT_AND_SUPPLIES = "Business Equipment & Supplies";
auto constexpr _CONGLOMERATES = "Conglomerates";
auto constexpr _CONSULTING_SERVICES = "Consulting Services";
auto constexpr _ELECTRICAL_EQUIPMENT_AND_PARTS = "Electrical Equipment & Parts";
auto constexpr _ENGINEERING_AND_CONSTRUCTION = "Engineering & Construction";
auto constexpr _FARM_AND_HEAVY_CONSTRUCTION_MACHINERY = "Farm & Heavy Construction Machinery";
auto constexpr _INDUSTRIAL_DISTRIBUTION = "Industrial Distribution";
auto constexpr _INTEGRATED_FREIGHT_AND_LOGISTICS = "Integrated Freight & Logistics";
auto constexpr _MARINE_SHIPPING = "Marine Shipping";
auto constexpr _METAL_FABRICATION = "Metal Fabrication";
auto constexpr _POLLUTION_AND_TREATMENT_CONTROLS = "Pollution & Treatment Controls";
auto constexpr _RAILROADS = "Railroads";
auto constexpr _RENTAL_AND_LEASING_SERVICES = "Rental & Leasing Services";
auto constexpr _SECURITY_AND_PROTECTION_SERVICES = "Security & Protection Services";
auto constexpr _SPECIALTY_BUSINESS_SERVICES = "Specialty Business Services";
auto constexpr _SPECIALTY_INDUSTRIAL_MACHINERY = "Specialty Industrial Machinery";
auto constexpr _STAFFING_AND_EMPLOYMENT_SERVICES = "Staffing & Employment Services";
auto constexpr _TOOLS_AND_ACCESSORIES = "Tools & Accessories";
auto constexpr _TRUCKING = "Trucking";
auto constexpr _WASTE_MANAGEMENT = "Waste Management";

// INFORMATION TECHNOLOGY...
auto constexpr _COMMUNICATIONS_EQUIPMENT = "Communication Equipment";
auto constexpr _COMPUTER_HARDWARE = "Computer Hardware";
auto constexpr _CONSUMER_ELECTRONICS = "Consumer Electronics";
auto constexpr _ELECTRONIC_COMPONENTS = "Electronic Components";
auto constexpr _ELECTRONICS_AND_COMPUTER_DISTRIBUTION = "Electronics & Computer Distribution";
auto constexpr _INFORMATION_TECHNOLOGY_SERVICES = "Information Technology Services";
auto constexpr _SCIENTIFIC_AND_TECHNICAL_INSTRUMENTS = "Scientific & Technical Instruments";
auto constexpr _SEMICONDUCTOR_EQUIPMENT_AND_MATERIALS = "Semiconductor Equipment & Materials";
auto constexpr _SEMICONDUCTORS = "Semiconductors";
auto constexpr _SOFTWARE_APPLICATION = "Software - Application";
auto constexpr _SOFTWARE_INFRASTRUCTURE = "Software - Infrastructure";
auto constexpr _SOLAR = "Solar";

// MATERIALS...
auto constexpr _AGRICULTURE_INPUTS = "Agricultural Inputs";
auto constexpr _ALUMINUM = "Aluminum";
auto constexpr _BUILDING_MATERIALS = "Building Materials";
auto constexpr _CHEMICALS = "Chemicals";
auto constexpr _COKING_COAL = "Coking Coal";
auto constexpr _COPPER = "Copper";
auto constexpr _GOLD = "Gold";
auto constexpr _LUMBER_AND_WOOD_PRODUCTION = "Lumber & Wood Production";
auto constexpr _OTHER_INDUSTRIAL_METALS_AND_MINING = "Other Industrial Metals & Mining";
auto constexpr _OTHER_PRECIOUS_METALS_AND_MINING = "Other Precious Metals & Mining";
auto constexpr _PAPER_AND_PAPER_PRODUCTS = "Paper & Paper Products";
auto constexpr _SILVER = "Silver";
auto constexpr _SPECIALTY_CHEMICALS = "Specialty Chemicals";
auto constexpr _STEEL = "Steel";

// REAL ESTATE...
auto constexpr _REAL_ESTATE_DEVELOPMENT = "Real Estate - Development";
auto constexpr _REAL_ESTATE_DIVERSIFIED = "Real Estate - Diversified";
auto constexpr _REAL_ESTATE_SERVICES = "Real Estate Services";
auto constexpr _REIT_DIVERSIFIED = "REIT - Diversified";
auto constexpr _REIT_HEALTH_CARE_FACILITIES = "REIT - Healthcare Facilities";
auto constexpr _REIT_HOTEL_AND_MOTEL = "REIT - Hotel & Motel";
auto constexpr _REIT_INDUSTRIAL = "REIT - Industrial";
auto constexpr _REIT_MORTGAGE = "REIT - Mortgage";
auto constexpr _REIT_OFFICE = "REIT - Office";
auto constexpr _REIT_RESIDENTIAL = "REIT - Residential";
auto constexpr _REIT_RETAIL = "REIT - Retail";
auto constexpr _REIT_SPECIALTY = "REIT - Specialty";

// UTITLITES...
auto constexpr _UTILITIES_DIVERSIFIED = "Utilities - Diversified";
auto constexpr _UTILITIES_INDEPENDENT_POWER_PRODUCERS = "Utilities - Independent Power Producers";
auto constexpr _UTITLITIES_REGULATED_ELECTRIC = "Utilities - Regulated Electric";
auto constexpr _UTITLITIES_REGULATED_GAS = "Utilities - Regulated Gas";
auto constexpr _UTITLITIES_REGULATED_WATER = "Utilities - Regulated Water";
auto constexpr _UTITLITIES_RENEWABLE = "Utilities - Renewable";

auto constexpr _SECTOR_INVALID = "Invalid Sector";

enum GridCanvasFlag
{
	TOTALROWHEAD = 2,
	TOTALROW = 4,
	ROWHEAD = 8,
	COLHEAD = 16,
	COLORDEPENDENT = 32,
	DATACELL = 64,
	MEDIUMFONT = 128,
	LARGEFONT = 256,
	BACKGROUDNORMAL = 512
};

enum Action
{
	BUY = 0,
	SELL,
	UNDEFINED
};

enum Returns
{
	DAY = UNDEFINED + 1,
	WEEK,
	MONTH,
	QUARTER,
	YEAR,
	ALL_TIME
};

enum _Sector
{
	BOND = ALL_TIME + 1,
	SMALLCAP,
	MIDCAP,
	LARGECAP,
	CRYPTO,
	FOREIGN,
	COMMUNICATION_SERVICES,
	CONSUMER_DESCRETIONARY,
	CONSUMER_STAPLES,
	ENERGY,
	FINANCIALS,
	HEALTH_CARE,
	INDUSTRIALS,
	INFORMATION_TECHNOLOGY,
	MATERIALS,
	REAL_ESTATE,
	UTILITIES,
	SECTOR_INVALID
};

enum _Sub_Sector
{
	// COMMUNICATION SERVICES...
	ADVERTISING_AGENCIES = SECTOR_INVALID + 1,
	BRODCASTING,
	ELECTRONIC_GAMING_AND_MULTIMEDIA,
	ENTERTAINMENT,
	INTERNET_CONTENT_AND_CREATION,
	PUBLISHING,
	TELECOM_SERVICES,


	// CONSUMER DESCRETIONARY...
	APPAREL_MANUFACTURING,
	APPAREL_RETAIL,
	AUTO_TRUCK_AND_DEALERSHIPS,
	AUTO_MANUFACTURERS,
	AUTO_PARTS,
	DEPARTMENT_STORES,
	FOOTWEAR_AND_ACCESSORIES,
	FURNISHIN_FIXTURES_AND_APPLIANCES,
	GAMBLING,
	HOME_IMPROVMENT_RETAIL,
	INTERNET_RETAIL,
	LEISURE,
	LODGING,
	LUXURY_GOODS,
	PACKAGING_AND_CONTAINERS,
	PERSONAL_SERVICES,
	RECREATIONAL_VEHICLES,
	RESIDENTIAL_CONSTRUCTION,
	RESORTS_AND_CASINOS,
	RESTAURANTS,
	SPECIALTY_RETAIL,
	TEXTILE_MANUFACTURING,
	TRAVEL_SERVICES,

	// CONSUMER STAPLES...
	BEVERAGES_BREWERS,
	BEVERAGES_NON_ALCOHOLIC,
	BEVERAGES_WINERIES_AND_DESTILLERIES,
	CONFECTIONERS,
	DISCOUNT_STORES,
	EDUCATION_AND_TRAINING_SERVICES,
	FARM_PRODUCTS,
	FOOD_DISTRIBUTION,
	GROCERY_STORES,
	HOUSEHOLD_AND_PERSONAL_PRODUCTS,
	PACKEDGED_FOODS,
	TOBACCO,

	// ENERGY...
	OIL_AND_GAS_DRILLING,
	OIL_AND_GAS_E_AND_P,
	OIL_AND_GAS_EQUIPMENT_AND_SERVICES,
	OIL_AND_GAS_INTEGRATED,
	OIL_AND_GAS_MIDSTREAM,
	OIL_AND_GAS_REFINING_AND_MARKETING,
	THERMAL_COAL,
	URANIUM,

	// FINANCIALS...
	ASSET_MANAGEMENT,
	BANKS_DIVERSIFIED,
	BANKS_REGIONAL,
	CAPITAL_MARKETS,
	CREDIT_SERVICES,
	FINANCIAL_CONGLOMERATES,
	FINANCIAL_DATA_AND_STOCK_EXCHANGES,
	INSURANCE_DIVERSIFIED,
	INSURANCE_LIFE,
	INSURANCE_PROPERTY_AND_CASUALTY,
	INSURANCE_REINSURANCE,
	INSURANCE_SPECIALTY,
	INSURANCE_BROKERS,
	MORTGAGE_FINANCE,
	SHELL_COMPANIES,

	// HEALTH CARE...
	BIOTECHNOLOGY,
	DIAGNOSTICS_AND_RESEARCH,
	DRUG_MANUFACTURES_GENERAL,
	DRUG_MANUFACTURES_SPECIALTY_AND_GENERIC,
	HEALTH_INFORMATION_AND_SERVICES,
	HEALTH_CARE_PLANS,
	MEDICAL_CARE_FACILITIES,
	MEDICAL_DEVICES,
	MEDICAL_DISTRIBUTION,
	MEDICAL_INSTRUMENTS_AND_SUPPLIES,
	PHARMECUTICALS_RETAILERS,

	// INDUSTRIALS...
	AEROSPACE_AND_DEFENSE,
	AIRLINES,
	AIRPORTS_AND_SERVICES,
	BUILDING_PRODUCTS_AND_EQUIPMENT,
	BUSINESS_EQUIPMENT_AND_SUPPLIES,
	CONGLOMERATES,
	CONSULTING_SERVICES,
	ELECTRICAL_EQUIPMENT_AND_PARTS,
	ENGINEERING_AND_CONSTRUCTION,
	FARM_AND_HEAVY_CONSTRUCTION_MACHINERY,
	INDUSTRIAL_DISTRIBUTION,
	INTEGRATED_FREIGHT_AND_LOGISTICS,
	MARINE_SHIPPING,
	METAL_FABRICATION,
	POLLUTION_AND_TREATMENT_CONTROLS,
	RAILROADS,
	RENTAL_AND_LEASING_SERVICES,
	SECURITY_AND_PROTECTION_SERVICES,
	SPECIALTY_BUSINESS_SERVICES,
	SPECIALTY_INDUSTRIAL_MACHINERY,
	STAFFING_AND_EMPLOYMENT_SERVICES,
	TOOLS_AND_ACCESSORIES,
	TRUCKING,
	WASTE_MANAGEMENT,

	// INFORMATION TECHNOLOGY...
	COMMUNICATIONS_EQUIPMENT,
	COMPUTER_HARDWARE,
	CONSUMER_ELECTRONICS,
	ELECTRONIC_COMPONENTS,
	ELECTRONICS_AND_COMPUTER_DISTRIBUTION,
	INFORMATION_TECHNOLOGY_SERVICES,
	SCIENTIFIC_AND_TECHNICAL_INSTRUMENTS,
	SEMICONDUCTOR_EQUIPMENT_AND_MATERIALS,
	SEMICONDUCTORS,
	SOFTWARE_APPLICATION,
	SOFTWARE_INFRASTRUCTURE,
	SOLAR,

	// MATERIALS...
	AGRICULTURE_INPUTS,
	ALUMINUM,
    BUILDING_MATERIALS,
	CHEMICALS,
	COKING_COAL,
	COPPER,
	GOLD,
	LUMBER_AND_WOOD_PRODUCTION,
	OTHER_INDUSTRIAL_METALS_AND_MINING,
    OTHER_PRECIOUS_METALS_AND_MINING,
	PAPER_AND_PAPER_PRODUCTS,
	SILVER,
	SPECIALTY_CHEMICALS,
	STEEL,

	// REAL ESTATE...
	REAL_ESTATE_DEVELOPMENT,
	REAL_ESTATE_DIVERSIFIED,
	REAL_ESTATE_SERVICES,
	REIT_DIVERSIFIED,
	REIT_HEALTH_CARE_FACILITIES,
	REIT_HOTEL_AND_MOTEL,
	REIT_INDUSTRIAL,
	REIT_MORTGAGE,
	REIT_OFFICE,
	REIT_RESIDENTIAL,
	REIT_RETAIL,
	REIT_SPECIALTY,

	// UTITLITES...
	UTILITIES_DIVERSIFIED,
	UTILITIES_INDEPENDENT_POWER_PRODUCERS,
	UTITLITIES_REGULATED_ELECTRIC,
	UTITLITIES_REGULATED_GAS,
	UTITLITIES_REGULATED_WATER,
	UTITLITIES_RENEWABLE,
	SUB_SECTOR_INVALID,

    // ETF 
	ETF
};

enum _PortfolioType
{
	STOCK = SUB_SECTOR_INVALID + 1,
	SECTOR,
	PORTFOLIO,
	LOT_PURCHASE,
	SOLD,
	INDICES,
	OPTIONS
};

enum _EnterDialog
{
	STOCK_PURCHASE_DIALOG = OPTIONS + 1,
	STOCK_SALE_DIALOG,
	DIVIDEND_SHARE_DIALOG,
	QUOTE_WIN,
	ENTER_DEPOSIT,
	WITHDRAWL,
	ENTER_DEPOSIT_SCHEDULE,
	SELL_STOCK, 
	DIVIDEND_ACTION_WIN,
	ENTER_DIVIDEND,
	REMOVE_DIVIDEND, 
	ADD_DIV_PAYMENT_DATE,
	ADJUST_DIV_AMOUNT,
	DAY_GAINERS_WIN,
	DAY_LOSERS_WIN,
	SUBSECTORWIN,
	SECTORSTOCKWIN
};

enum _MenuItemIDs
{
	QUOTE_LOOKUP = SECTORSTOCKWIN + 1,
	ADD_DIV,
	REMOVE_DIV,
	NEW_DEPOSIT,
	NEW_DEPOSIT_SCHEDULE,
	VIEW_DEPOSITS,
	VIEW_DIVIDENDS,
	_WITHDRAWL,
	_SELL_STOCK,
	ADD_DIV_SHARES,
	DAY_GAINERS_MENU,
	DAY_LOSERS_MENU,

	//popup menu ids...
	P_QUOTE,
	P_ADD_DIV_REINVEST,
	P_ADD_DIV,
	P_REMOVE_DIV,
	P_VIEW_DIVIDENDS,
	P_SELL_STOCK, 
	P_STOCK_PURCHASE,
	P_GET_OHLC        // Get Open,High,Low,Close
};

enum _PortfolioWin
{
	DATEPCKER = P_GET_OHLC + 1
};

enum _INDEX_
{
	DOW_JONES = DATEPCKER + 1,
	NASDAQ,
	SP_500
};
