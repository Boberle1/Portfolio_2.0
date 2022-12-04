#pragma once
#include "Portfolio.h"
#include "wx/listctrl.h"
#include "wx/valnum.h"
#include "wx/display.h"
#include "wx/dialog.h"
#include "wx/validate.h"
#include "wx/statline.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

struct TextExtent
{
	TextExtent(double w1, double h1) : w(w1), h(h1) {}
	double w = 0;
	double h = 0;
};

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

class GridCanvas : public wxWindow
{
public:
	GridCanvas(wxWindow*, wxSize, wxString&, int);
	void OnPaint(wxPaintEvent&);
	TextExtent* GetTextExtent();
	bool IsMatch(wxString&);
	bool IsEmpty();
	void OnMouseEnter(wxMouseEvent&);
	void OnMouseLeave(wxMouseEvent&);
	wxDECLARE_EVENT_TABLE();
protected:
	void m_SetValue(wxString&);
	void m_SetValue(wxString&, bool color);
	wxString m_GetValue();
	void SetToTotalCanvas();
	void SetTotTotalStartRowCanvas();
	void SetToHeader();
	void SetToRowStartCanvas();
	void SetToDataCellColor();
	void SetToNormalBackground();
	void SetToMediumFont();
	void SetToLargeFont();
	void LeftClick();
	void RightClick();
private:
	void Initialize();
	void SetTextColor(int);
	void SetBackgroundColor();
private:
	wxString value = "";

	// this flag value is used to determine how to display the data...
	int flag = 0;

	// Possible fonts..
	wxFont rowStartFont = wxFont(14, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxFont normalFont = wxFont(13, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM);

	// textfont used in the paint method
	// Its just a place holder for any of the fonts above...
	wxFont textfont;

	// Possible text colours...
	wxColour green = wxColour(13, 158, 20);
	wxColour red = wxColour(217, 7, 28);
	wxColour yellow = wxColour(176, 194, 16);
	wxColour colHeaderColour = wxColour(213, 220, 227);
	wxColour rowHeaderColour = wxColour(182, 203, 242);
	wxColour white = wxColour(255, 255, 255);

	// textcolour used in the paint method
	// Its just a place holder for any of the text colours above...
	wxColour textcolor;
	
	// Possible background colours...
	wxColour normal = wxColour(13, 10, 36);
	wxColour totalRowColor = wxColour(21, 41, 46);
	wxColour onClick;
	wxColour hover = wxColour(11, 117, 143);

	// backgroud color is used in the paint method...
	// It is just a place holder for any of the background colors above...
	wxColour background;

	TextExtent textextent;
};

class GridNode;

class Gnode : public GridCanvas
{
public:
	Gnode(GridView*, wxWindow*, wxSize, size_t, size_t, Gnode*, Gnode*, Gnode*, Gnode*, wxString&, int);
	Gnode(GridView*, wxWindow*, wxSize, size_t, size_t, Gnode*, Gnode*, Gnode*, Gnode*, int);

	void SetValue(wxString&);
	void SetValue(wxString&, bool color);

	void SetUP(Gnode*);
	void SetRight(Gnode*);
	void SetDown(Gnode*);
	void SetLeft(Gnode*);
	Gnode* GetUP();
	Gnode* GetRight();
	Gnode* GetDown();
	Gnode* GetLeft();

	void OnRightClick(wxMouseEvent&);
	wxDECLARE_EVENT_TABLE();
private:
	wxString emptystring = "";
	Gnode* m_up = nullptr;
	Gnode* m_right = nullptr;
	Gnode* m_down = nullptr;
	Gnode* m_left = nullptr;

	GridView* m_parent = NULL;
};

class GridNode : public wxStaticText
{
public:
	GridNode(GridView*, wxWindow*, size_t, size_t, GridNode*, GridNode*, GridNode*, GridNode*, wxString, int);
	GridNode(const GridNode&);
	void operator=(const GridNode&);
	~GridNode();
	bool CopyText(const GridNode&);
	void SetMySizer(wxSizer* sizer) { this->mySizer = sizer; }
	void LayoutMySizer() { this->mySizer->Layout(); }

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
	bool Clear();
//	bool IsID(int);
	bool IsMatch(wxString);
	void OnClickEvent(wxMouseEvent&);
	void OnRightClick(wxMouseEvent&);
	void OnClickRowOff();
	void OnClickColOff();
	void OnClickItemOff();
	void OnClickColON();
	void OnClickRowON();
	void ChangeColorToClicked(int);
	void UnClick();
	wxDECLARE_EVENT_TABLE();
private:
	void InitializeGridNode();
	void SetBackGroundColor();
private:
	GridNode* m_up = nullptr;
	GridNode* m_right = nullptr;
	GridNode* m_down = nullptr;
	GridNode* m_left = nullptr;
	bool is_empty = true;
	wxString t_val = "";
	size_t m_row = 0;
	size_t m_col = 0;

	// Background Colours...
	wxColour normal = wxColour(13, 10, 36);
	wxColour summaryitem = wxColour(21, 41, 46);
	wxColour onclick;

	//Foreground Colours...
	wxColour red = wxColour(217, 7, 28);
	wxColour green = wxColour(13, 158, 20);
	wxColour textcolor;

	wxWindow* m_parent = nullptr;
	GridView* m_grid_view = nullptr;
	bool SummaryItem = false;
	int Gridtype = -1;
	wxSizer* mySizer = NULL;
};

class mainwindow;

class GridView : public wxGridSizer
{
public:
	GridView(wxWindow*, int, int);
	~GridView();
	void SetNewRow(StockViewerData*);
	void UpdateRow(StockViewerData*);
//	void SetNewRow(StockViewerData*, bool total_row = false);
//	void UpdateRow(StockViewerData*, bool total_row = false);
	bool DoesItemExist(wxString);
	bool ItemExist(wxString&);
	void Cleanup();
	void OnClickColItem(GridNode*);
	void OnClickRowItem(GridNode*);
	void OnClickItem(GridNode*);
	void LayoutGrid();
	void RightClickAlert(wxString&, wxPoint&);
	wxString GetRightClickTicker();
	bool RemoveRow(wxString&);
private:
//	wxStaticText* GetNewItem(size_t, size_t, double, bool ispercent = true);
	void SetTitleRow();
	void FillGrid();
	void ClickRowOff();
	void ClickColOff();
	gridpair GetLabelText(size_t&, StockViewerData*);
	wxString GetStringLabel(size_t&, StockViewerData*);
	wxString GetColTitle(size_t);
	int GetGridNodeFlags(size_t&, size_t&);
	Gnode* GetEmptyRow();
	Gnode* GetMatch(wxString&);
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

	Gnode* GetFarthestRight(Gnode*);
	Gnode* GetRight(Gnode*, size_t, size_t start = 0);
	Gnode* GetLeft(Gnode*, size_t, size_t start = 0);
	Gnode* GetUp(Gnode*, size_t, size_t start = 0);
	Gnode* GetDown(Gnode*, size_t, size_t start = 0);

private:
	void RemoveRow(GridNode*);
	void MoveRowUp(GridNode*);
	void ClearRow(GridNode*);
	void DeleteRow(GridNode*);
//	GridNode* FindItem(GridNode*, int);
	void HighlightRow(GridNode*);
	void HighlightCol(GridNode*);
//	GridNode* FindGridNode(int);
	GridNode* GetSummaryRow();
	void SwapSummaryRow(GridNode*&);
private:
	GridNode* head = nullptr;
	Gnode* Head = NULL;
	const Portfolio* port = NULL;
	wxWindow* m_parent;
	int gridnode_Size = 0;
	size_t filledrows = 0;
	size_t filledNodes = 0;
	GridNode* grid_col = nullptr;
	GridNode* grid_row = nullptr;
	GridNode* clicked_node = nullptr;
	GridNode* summaryrow = nullptr;
	bool SummaryRowSet = false;
	wxString rightclick_ticker = "";
	wxVector<GridNode*> gridnode;
	wxVector<wxString> headers;
	int itemsize = 0;
};

class PortfolioWin : public wxWindow
{
public:
	PortfolioWin(mainwindow*, wxWindowID, wxDateTime*, StockViewerData*, Portfolio*);
	void Update();
	void OnClick(wxString&);
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
	wxStaticText* beginParenths = nullptr;
	wxStaticText* dollarsign = nullptr;
	wxStaticText* endParenths = nullptr;
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
	wxStaticText* TotalReturnDollar = NULL;
	wxStaticText* t_percent = NULL;
	wxStaticText* t_dollar = NULL;
	wxStaticText* t_beginparenths = NULL;
	wxStaticText* t_endParenths = NULL;
	wxStaticText* DayReturn = nullptr;
	wxStaticText* DayReturnDisplay = nullptr;
	wxStaticText* DayReturnDollar = NULL;
	wxStaticText* d_percent = NULL;
	wxStaticText* d_dollar = NULL;
	wxStaticText* d_beginparenths = NULL;
	wxStaticText* d_endParenths = NULL;
	StockViewerData* svd = nullptr;
	mainwindow* m_parent = nullptr;
	wxPanel* panel = nullptr;
	Portfolio* port = nullptr;
	wxButton* updatebutton = nullptr;
	wxColour Red = wxColour(217, 7, 28);
//	wxColour Green = wxColour(6, 84, 27);
	wxColour Green = wxColour(16, 82, 31);
	wxColour Blue = wxColour(36, 11, 191);
	wxColour DarkGrey = wxColour(43, 43, 43);
	wxColour parenths = wxColour(0, 0, 0);
	wxColour percents = wxColour(41, 47, 227);

	Gnode* canvas = NULL;
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

template <typename A, typename B>
struct GenericKitTemplate
{
	GenericKitTemplate(A a, B b) : thing1(a), thing2(b) {}
	A thing1;
	B thing2;
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

// Simple struct for storing the ticker name, longname and the sector name that its in...
struct string_three
{
	string_three(wxString& t, wxString& l, wxString& s) : ticker(t), longname(l), sector(s) {}
	string_three() {}
	wxString ticker = "";
	wxString longname = "";
	wxString sector = "";
};

// Simple struct that creates the ctrls for entering dividend data...
struct DivCtrls
{
	DivCtrls(wxDialog*, Dividend&);
	wxBoxSizer*& GetTopSizer();
	wxBoxSizer*& GetBottomSizer();
	void OnEdit();
	
	wxTextCtrl* payment_date = NULL;
	wxTextCtrl* ex_div_date = NULL;
	wxTextCtrl* amount = NULL;
	wxTextCtrl* reInvestmentShares = NULL;
	wxDialog* m_parent = NULL;

	wxStaticText* s_payment_date = NULL;
	wxStaticText* s_ex_div_date = NULL;
	wxStaticText* s_amount = NULL;
	wxStaticText* s_reInvestmentShares = NULL;

	wxString _payment_date = "";
	wxString _ex_div_date = "";
	double d_amount = 0.0;
	double d_reInvestmentShares = 0.0;

	wxButton* edit = NULL;
	wxButton* ok = NULL;
	wxButton* cancel = NULL;

	wxBoxSizer* payment_sizerV = NULL;
	wxBoxSizer* ex_sizerV = NULL;
	wxBoxSizer* reinvestment_sizerV = NULL;
	wxBoxSizer* amount_sizerV = NULL;
	wxBoxSizer* button_sizerH = NULL;
	wxBoxSizer* sizerH = NULL;

	Dividend div;
};

class Dialog : public wxDialog
{
public:
	template <typename T>
	Dialog(_EnterDialog, mainwindow*, wxWindowID, wxPoint, wxSize, wxString, T*);
	~Dialog();
	PurchaseKit GetPurchaseKit();
	GenericKit GetGenericKit();
	SellKit GetSellKit();
	double GetReInvestShares();
	wxString GetTicker();
	int GetDivChoice();
	Dividend GetDividend();
	wxDECLARE_EVENT_TABLE();
private:
	void CreateStockEntry();
	void CreateQuoteWin();
	void CreateDepositWin();
	void CreateWithdrawlWin();
	void CreateDepositScheduleWin();
	void CreateSellStockWin();
	void CreateAddDividendWin();
	void CreateAddReInvestShares();
	void CreateDividendActionWin();
	void CreateEditDivWin();

	// helper func for CreateStockEntry, it creates the wxChoice controls for selecting sector and industry(subsector)...
	void CreateChoiceControls();
	void CreateDayGainers_LosersWin(bool gainers = true);
	void CreateSubSectorWin();
	void OnCheckClick(wxCommandEvent&);
	void OnCancelDialog(wxCommandEvent&);
	void OnCloseDialog(wxCloseEvent&);
	void OnOkClick(wxCommandEvent&);
	void OnSectorChoice(wxCommandEvent&);
	void OnDividendChoice(wxCommandEvent&);
	void OnEditClick(wxCommandEvent&);
	void OnKeyDown(wxKeyEvent&);

	// event functio for DayGainers_LosersWin...
	void OnMouseDown(wxMouseEvent&);
	void OnMouseEnter(wxMouseEvent&);
	void OnMouseLeave(wxMouseEvent&);

private:
	bool HandlePurchaseOkClick();
	bool HandleDepositOkClick();
	void HandleDepositScheduleOkClick();
	bool HandleWithdrawlOkClick();
	bool HandleSellStockOkClick();
	void HandleAddDividendOkClick();
	bool HandleReInvestSharesOkClick();
	bool OnEditDivWinOK();
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
	wxString* _ticker_ptr = NULL;
	double _price = 0.0;
	double _shares = 0.0;
	wxString _date = "";
	wxString _reinvest_date = "";
	wxDateTime datetime;
	wxDateTime redatetime;
	_EnterDialog m_type = STOCK_PURCHASE_DIALOG;
	SummaryData* sumptr = nullptr;
	GenericKit* sellkit = NULL;
	wxVector<DayGainersandLosers>* gainers = nullptr;
    wxVector<SubSector>* sub = NULL;
	string_three string_t;
	GenericKitTemplate<Dividend, wxString>* generic_kit_template = NULL;

	// This holds the button id pressed that is an indicater of what the user wants to do to an existing dividend...
	int dividend_action = 0;
	DivCtrls* divctrl = NULL;
};

class GenericListWin : public wxSingleChoiceDialog
{
public:
	GenericListWin(mainwindow*, wxWindowID, wxString, wxString, wxArrayString);
private:
	mainwindow* m_parent = NULL;
};

class BottomFrame : public wxWindow
{
public: 
	BottomFrame(mainwindow*, wxWindowID);
	wxDECLARE_EVENT_TABLE();
	void OnMouseEnter(wxMouseEvent&);
	void OnMouseLeave(wxMouseEvent&);
	void OnMouseDown(wxMouseEvent&);
private:
	void Create();
	void BindEvents(wxStaticText&);
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
	wxStaticText* textenter = NULL;
};

class mainwindow : public wxFrame
{
public:
	mainwindow();
	~mainwindow();
	void PurchaseWin(wxString&, wxString&);
	void AddDividendWin(wxString&, wxString&);
	void OnViewDividendWin(wxString&);
	void OnModifyDividendWin(Dividend&, wxString&, wxString&);
	void OnAddDivPayoutDate(Dividend&, wxString&);
	void OnAddDivReInvestmentShares(Dividend&, double&);
	void DialogCancel();
	void PurchaseDataTransfer(PurchaseKit&);
	void ReInvestSharesWin(wxString&, wxString&);
	bool ValidateExistingTicker(wxString&);
	void SectorClick(wxVector<SubSector>*, wxString&);
	void RightClickGrid(wxString&, wxPoint&);

	// Event functions...
	void CloseEvent(wxCloseEvent&);
	void ClosePanelEvent(wxCloseEvent&);
	void OnThreadCompletion(wxThreadEvent&);

	// Helper functions for OnPurchaseMenu...
	void PurchaseWin(wxString&);
	void OnPurchaseMenu(wxCommandEvent&);
	// End of helper functions for OnPurchaseMenu and OnPurchaseMenu itself...

	//Helper functions for OnSellMenu...
	stock_node* LotSelectionWindow(wxString&);
	void UserEnterSellDataWin(stock_node*);
	void OnSellMenu(wxCommandEvent&);
	// End of helper functions for OnSellMenu..

	void OnSellPopupClick(wxCommandEvent&);
	void OnPurchasePopupClick(wxCommandEvent&);
	void OnAddReInvestSharesPopup(wxCommandEvent&);
	void OnQuoteLookupPopup(wxCommandEvent&);
	void OnAddDividendPopup(wxCommandEvent&);
	void OnViewDividendPopup(wxCommandEvent&);

	void OnViewDividend(wxCommandEvent&);
	void OnReInvestSharesMenu(wxCommandEvent&);
	void OnSave(wxCommandEvent&);
	void OnQuoteLookup(wxCommandEvent&);
	void OnKeyDown(wxKeyEvent&);
	void OnAddDeposit(wxCommandEvent&);
	void OnWithdrawl(wxCommandEvent&);
	void OnMarketGainers(wxCommandEvent&);
	void OnMarketLosers(wxCommandEvent&);
	void OnAddDepositSchdule(wxCommandEvent&);
	void OnRightClick(wxMouseEvent& evt);
	void OnAddDividend(wxCommandEvent&);
	void OnViewDeposits(wxCommandEvent&);

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
	void DeletePopupMenu();
	void CreatePopupMenu();
private:
	wxScrolled<wxPanel>* grid_panel = NULL;
	GridView* grid_view = NULL;
	PortfolioWin* portwin = nullptr;
	BottomFrame* bottom_frame = nullptr;
	Dialog* dialog = nullptr;
	GenericListWin* generic_list = NULL;

	// For entering ticker to perform an action on it example: sell, buy add dividend ect...
	wxTextEntryDialog* generic_entry = NULL;

	wxWindow* right_win = nullptr;

	Portfolio port;
	wxDateTime main_clock;

	// For storing the ticker that was clicked on for convenience...
	wxString rightclick_ticker = "";

	//Popup Menu Items...
	wxMenuItem* p_sell = NULL;
	wxMenuItem* p_buy = NULL;
	wxMenuItem* p_quote = NULL;
	wxMenuItem* p_add_div_reinvest = NULL;
	wxMenuItem* p_add_div = NULL;
	wxMenuItem* p_view_dividends = NULL;
	wxMenuItem* p_ohlc = NULL;
	wxMenu* popup = NULL;
	int counter = 0;
};

