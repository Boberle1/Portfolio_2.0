#pragma once
#include "Portfolio.h"
#include "wx/listctrl.h"
#include "wx/valnum.h"
#include "wx/display.h"
#include "wx/dialog.h"
#include "wx/validate.h"
#include "wx/statline.h"

// helper for Decipherhtmlcodes
char DecipherHelper(char& a, char& b);

// Helper function to decipher html character codes...
wxString Decipherhtmlcodes(wxString&);

class ItemData
{
public:
	ItemData(StockNode* item) : stock(item)
	{
		
	};

	StockNode* stock = nullptr;
	long id = 0;
};

class VirtualListView : public wxListView
{
public:
	VirtualListView(wxWindow* w, wxWindowID id, wxPoint p, wxSize s, Portfolio*);
	virtual wxString OnGetItemText(long, long) const wxOVERRIDE;
private:
	wxString GetItemValue(const ItemData*, long) const;
	void InitializeView();
	void m_SetItemCount();
	void RefreshOnUpdate();
	void UpdateItemsCount();
	wxVector<wxString> Col;
	wxVector<ItemData> items;
	Portfolio* portfolio = NULL;
};

class GridView;

//simple struct for GridNode SetNewValue
struct gridpair
{
	gridpair(wxString v, bool b) : val(v), valcolor(b) {}
	wxString val;
	bool valcolor;
};

class GridNode : public wxStaticText
{
public:
	GridNode(GridView*, wxWindow*, size_t, size_t, GridNode*, GridNode*, GridNode*, GridNode*, wxString, int);
	~GridNode();
	bool SetNewVal(gridpair, bool total_row = false);
	void SetUP(GridNode*);
	void SetRight(GridNode*);
	void SetDown(GridNode*);
	void SetLeft(GridNode*);
	GridNode* GetUP();
	GridNode* GetRight();
	GridNode* GetDown();
	GridNode* GetLeft();
	bool IsEmpty();
	bool IsID(int);
	bool IsMatch(wxString);
	void OnClickEvent(wxMouseEvent&);
	void OnClickRowOff();
	void OnClickColOff();
	void OnClickItemOff();
	void OnClickColON();
	void OnClickRowON();
	void ChangeColorToClicked(int);
	void UnClick();
	wxDECLARE_EVENT_TABLE();
private:
	GridNode* m_up = nullptr;
	GridNode* m_right = nullptr;
	GridNode* m_down = nullptr;
	GridNode* m_left = nullptr;
	bool is_double = false;
	bool is_int = false;
	bool is_percent = false;
	bool is_empty = true;
	wxString t_val = "";
	double d_val = 0.0;
	int int_val = 0;
	size_t m_row = 0;
	size_t m_col = 0;
	wxColour normal = wxColour(13, 10, 36);
	wxColour summaryitem = wxColour(21, 41, 46);
	wxColour onclick;
	wxColour textcolor;
	wxWindow* m_parent = nullptr;
	GridView* m_grid_view = nullptr;
	bool SummaryItem = false;
};

class GridView : public wxGridSizer
{
public:
	GridView(wxWindow*, int);
	~GridView();
	void SetNewRow(StockViewerData*, bool total_row = false);
	void UpdateRow(StockViewerData*, bool total_row = false);
	bool DoesItemExist(wxString);
	void Cleanup();
	void OnClickColItem(GridNode*);
	void OnClickRowItem(GridNode*);
	void OnClickItem(GridNode*);
	void LayoutGrid();
private:
//	wxStaticText* GetNewItem(size_t, size_t, double, bool ispercent = true);
	void SetTitleRow();
	void FillGrid();
	void ClickRowOff();
	void ClickColOff();
	gridpair GetLabelText(size_t&, StockViewerData*);
	wxString GetColTitle(size_t);
	GridNode* GetFirstEmptyRow();
	GridNode* GetFirstMatch(wxString);
	GridNode* GetFarthestRight(GridNode*);
	GridNode* GetFarthestLeft(GridNode*);
	GridNode* GetFarthestTop(GridNode*);
	GridNode* GetFarthestBottom(GridNode*);
	GridNode* GetRight(GridNode*, size_t, size_t start = 0);
	GridNode* GetLeft(GridNode*, size_t, size_t start = 0);
	GridNode* GetUp(GridNode*, size_t, size_t start = 0);
	GridNode* GetDown(GridNode*, size_t, size_t start = 0);

private:
	void DeleteRow(GridNode*);
	GridNode* FindItem(GridNode*, int);
	void HighlightRow(GridNode*);
	void HighlightCol(GridNode*);
	GridNode* FindGridNode(int);
	GridNode* GetSummaryRow();
	void SwapSummaryRow(GridNode*&);
private:
	GridNode* head = nullptr;
	const Portfolio* port = NULL;
	wxWindow* m_parent;
	int gridnode_Size = 0;
	size_t filledrows = 0;
	GridNode* grid_col = nullptr;
	GridNode* grid_row = nullptr;
	GridNode* clicked_node = nullptr;
	GridNode* summaryrow = nullptr;
	bool SummaryRowSet = false;
};

class mainwindow;

class PortfolioWin : public wxWindow
{
public:
	PortfolioWin(mainwindow*, wxWindowID, wxDateTime*, StockViewerData*, Portfolio*);
	void Update();
private:
	void Create();
	void SetControlFonts();
	void DateChange(wxCommandEvent&);
	void OnUpdateButtonPress(wxCommandEvent&);
	wxDECLARE_EVENT_TABLE();
private:
	wxTextCtrl* datepcker = nullptr;
	wxDateTime* mainclock = nullptr;
	wxDateTime today;
	wxStaticText* todaysdate = nullptr;
	wxStaticText* todaysdateDisplay = nullptr;
	wxStaticText* Cash = nullptr;
	wxStaticText* CashDisplay = nullptr;
	wxStaticText* AccountValue = nullptr;
	wxStaticText* AccountValueDisplay = nullptr;
	wxStaticText* MarketValue = nullptr;
	wxStaticText* MarketValueDisplay = nullptr;
	wxStaticText* TotalReturn = nullptr;
	wxStaticText* TotalReturnDisplay = nullptr;
	wxStaticText* DayReturn = nullptr;
	wxStaticText* DayReturnDisplay = nullptr;
	StockViewerData* svd = nullptr;
	mainwindow* m_parent = nullptr;
	wxPanel* panel = nullptr;
	Portfolio* port = nullptr;
	wxButton* updatebutton = nullptr;
	wxColour Red = wxColour(217, 7, 28);
	wxColour Green = wxColour(6, 84, 27);
	wxColour Blue = wxColour(36, 11, 191);
};

struct PurchaseKit
{
	PurchaseKit(wxString t, _Sector sect, _Sub_Sector sub, double p, double s, wxString date, wxString reinvest_date, bool reinvest)
		: m_ticker(t), m_sect(sect), m_sub(sub), m_price(p), m_shares(s), m_date(date), m_reinvest_date(reinvest_date), m_reinvest(reinvest) {}
	wxString m_ticker = "";
	_Sector m_sect = _Sector::BOND;
	_Sub_Sector m_sub = _Sub_Sector::SUB_SECTOR_INVALID;
	double m_price = 0.0;
	double m_shares = 0.0;
	wxString m_date = "";
	wxString m_reinvest_date = "";
	bool m_reinvest = false;
};

struct GenericKit
{
	GenericKit(wxDateTime& d, double& dd) : date(d), amount(dd) {}
	double amount = 0.0;
	wxDateTime date;
};

struct SellKit
{
	SellKit(wxDateTime& d, double& price, double& shares)
		: date(d), m_price(price), m_shares(shares) {}

	wxDateTime date;
	double m_shares = 0.0;
	double m_price = 0.0;
	long id = -1;
};

class Dialog : public wxDialog
{
public:
	template <typename T>
	Dialog(_EnterDialog, mainwindow*, wxWindowID, wxPoint, wxSize, wxString, T*);
	PurchaseKit GetPurchaseKit();
	GenericKit GetGenericKit();
	SellKit GetSellKit();
	wxDECLARE_EVENT_TABLE();
private:
	void CreateStockEntry();
	void CreateQuoteWin();
	void CreateDepositWin();
	void CreateWithdrawlWin();
	void CreateDepositScheduleWin();
	void CreateSellStockWin();
	void CreateAddDividendWin();

	// helper func for CreateStockEntry, it creates the wxChoice controls for selecting sector and industry(subsector)...
	void CreateChoiceControls();
	void CreateDayGainers_LosersWin(bool gainers = true);
	void OnCheckClick(wxCommandEvent&);
	void OnCancelDialog(wxCommandEvent&);
	void OnCloseDialog(wxCloseEvent&);
	void OnOkClick(wxCommandEvent&);
	void OnSectorChoice(wxCommandEvent&);

	// event functio for DayGainers_LosersWin...
	void OnMouseDown(wxMouseEvent&);
private:
	bool HandlePurchaseOkClick();
	bool HandleDepositOkClick();
	void HandleDepositScheduleOkClick();
	bool HandleWithdrawlOkClick();
	bool HandleSellStockOkClick();
	void HandleAddDividendOkClick();
private:
	mainwindow* m_parent = nullptr;
	wxStaticText* s_ticker = nullptr;
	wxTextCtrl* m_ticker = nullptr;
	
	wxStaticText* sector_choice = nullptr;
	wxChoice* choice = nullptr;
	wxChoice* subSector = nullptr;
	wxString* sectStringPtr = nullptr;
	wxString* subsecStringPtr = nullptr;

	wxStaticText* s_price = nullptr;
	wxTextCtrl* m_price = nullptr;

	wxStaticText* s_shares = nullptr;
	wxTextCtrl* m_shares = nullptr;

	wxStaticText* s_date = nullptr;
	wxTextCtrl* m_date = nullptr;

	wxCheckBox* check = nullptr;
	wxStaticText* s_reinvest_date = nullptr;
	wxTextCtrl* m_reinvest_date = nullptr;

	wxString _ticker = "";
	double _price = 0.0;
	double _shares = 0.0;
	wxString _date = "";
	wxString _reinvest_date = "";
	wxDateTime datetime;
	wxDateTime redatetime;
	_EnterDialog m_type = STOCK_PURCHASE_DIALOG;
//	SummaryData sd;
	SummaryData* sumptr = nullptr;
	GenericKit* sellkit = NULL;
	wxVector<DayGainersandLosers>* gainers = nullptr;
};

class SellStockWin : public wxSingleChoiceDialog
{
public:
	SellStockWin(mainwindow*, wxWindowID, wxString, wxString, wxArrayString);
private:
	mainwindow* m_parent = NULL;
};

class BottomFrame : public wxWindow
{
public: 
	BottomFrame(mainwindow*, wxWindowID);
private:
	void Create();
private:
	mainwindow* m_parent = nullptr;
	wxStaticText* materials = nullptr;
	wxStaticText* comm_services = nullptr;
	wxStaticText* cons_cycl = nullptr;
	wxStaticText* cons_def = nullptr;
	wxStaticText* energy = nullptr;
	wxStaticText* finance = nullptr;
	wxStaticText* healthcare = nullptr;
	wxStaticText* industrial = nullptr;
	wxStaticText* real_estate = nullptr;
	wxStaticText* tech = nullptr;
	wxStaticText* utilities = nullptr;
};

class mainwindow : public wxFrame
{
public:
	mainwindow();
	~mainwindow();
	wxFrame* PurchaseWin();
	void DialogCancel();
	void PurchaseDataTransfer(PurchaseKit&);
	// Event functions...
	void CloseEvent(wxCloseEvent&);
	void ClosePanelEvent(wxCloseEvent&);
	void OnThreadCompletion(wxThreadEvent&);
	void OnPurchaseMenu(wxCommandEvent&);
	void OnSellMenu(wxCommandEvent&);
	void OnSave(wxCommandEvent&);
	void OnQuoteLookup(wxCommandEvent&);
	void OnKeyDown(wxKeyEvent&);
	void OnAddDeposit(wxCommandEvent&);
	void OnWithdrawl(wxCommandEvent&);
	void OnAddDividend(wxCommandEvent&);	
	void OnMarketGainers(wxCommandEvent&);
	void OnMarketLosers(wxCommandEvent&);
	void OnAddDepositSchdule(wxCommandEvent&);

	//function called by PortfolioWin when user pressed enter after typing in a date...
	void OnDateChange();

	// function called by PortfolioWin when user presses update button on left window;
	void OnUpdate();
	wxDECLARE_EVENT_TABLE();
private:
	wxWindow* GetLeftWin();
	wxScrolled<wxPanel>* GetRightWin();
	wxWindow* GetBottomFrame();
	void UpdateGridView();
	void SaveFile();
	void RetrieveFile();
private:
	wxScrolled<wxPanel>* grid_panel = NULL;
	wxScrolledWindow* scrolled = nullptr;
	GridView* grid_view = NULL;
	PortfolioWin* portwin = nullptr;
	BottomFrame* bottom_frame = nullptr;
	Dialog* dialog = nullptr;
	SellStockWin* sellstock = NULL;
	wxTextEntryDialog* quote = nullptr;
	wxTextEntryDialog* sell = NULL;
	wxWindow* right_win = nullptr;
//	wxWindow* bottom_frame = NULL;
	Portfolio port;
	webdata data;
	wxDateTime main_clock;
};

