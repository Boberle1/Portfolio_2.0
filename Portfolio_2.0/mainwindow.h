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
class GridView2;

//simple struct for GridNode SetNewValue
struct gridpair
{
	gridpair(wxString v, bool b) : val(v), valcolor(b) {}
	wxString val;
	bool valcolor;
	
};

class MotionCanvas : public wxWindow
{
public:
//	MotionCanvas(wxWindow*, wxSize, wxVector<wxString>);
	MotionCanvas(wxWindow*, wxSize, wxVector<StockViewerData*>);
	void OnPaint(wxPaintEvent&);
	void OnEraseBackground(wxEraseEvent&);
private:
	wxColour background = wxColour(21, 41, 46);
	int iter = 0;
	wxVector<wxString> vec;
	wxVector<wxVector<wxString>> str_vec;
	wxVector<StockViewerData*> svd_vec;
	wxColour textcolor = wxColour(182, 203, 242);
	wxColour bottom_text_color;
	wxColour red = wxColour(219, 4, 22);
	wxColour green = wxColour(16, 82, 31);
	wxColour yellow = wxColour(176, 194, 16);
	wxFont textfont = wxFont(14, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	TextExtent textextent;
	TextExtent textextent2;
	TextExtent textextent3;
	int top_distance = -21000;
	double top_distance_double = -21000.0;
	int bottom_distance = -21000;
	double bottom_distance_double = -21000.0;
	int iteminfo_iter = 0;
};

struct GridValData;

class GridVal
{
public:
	GridVal(wxWindow*, wxString&, int, wxSize);
	void CopyGridValData(GridValData&);
	bool IsMatch(wxString&);
	bool IsEmpty();
	void SetValue(wxString&);
	void SetHover();
	void RemoveHover();
	bool IsDataCell() { return this->isDataCell; }
	bool IsTotalRow() { return this->isTotalRow; }
	bool IsRowHead() { return this->isHeaderRow; }
	bool IsColHeader() { return this->isHeaderCol; }
	// Passing a point to this function will check if this point lands on the rectangle that encases this window...
	bool IsMyPixel(const wxPoint&);

	// Setting whether it is shown onscreen or not...
	void SetVisibility(bool b) { this->isShown = b; }
	bool IsShown() { return this->isShown; }

	wxColour& GetBackColour() { return this->background; }
	wxColour& GetTextColour() { return this->textcolor; }
	wxString& GetTextValue() { return this->value; }
	wxFont& GetTextFont() { return this->textfont; }
	void SetMyPosition(wxPoint& p) { this->myPosition = p; }
	void SetMyRelativePosition(wxPoint& p) { this->relativePosition = p; }
	wxPoint& GetMyPosition() { return this->myPosition; }
	wxPoint& GetMyRelativePosition() { return this->relativePosition; }
	wxSize& GetMySize() { return this->mySize; }
	int Getflags() { return this->flag; }
	double GetDoubleValue() { return this->double_val; }

private:
	void Initialize();
	void CalcDoubleValue();
	void SetTextColor(int);
	void SetBackgroundColor();
	void SetToTotalCanvas();
	void SetTotTotalStartRowCanvas();
	void SetToHeader();
	void SetToRowStartCanvas();
	void SetToDataCellColor();
	void SetToNormalBackground();
	void SetToMediumFont();
	void SetToLargeFont();
	void ResetBools();
protected:
	void CopyGridVal(const GridVal&);
	void Clear(int flags = -1);


protected:

	wxString value = "";

	// textfont used in the paint method
	// Its just a place holder for any of the fonts below...
	wxFont textfont;

	// textcolour used in the paint method
	// Its just a place holder for any of the text colours below...
	wxColour textcolor;

	// backgroud color is used in the paint method...
	// It is just a place holder for any of the background colors below...
	wxColour background;

	wxPoint myPosition;
	wxPoint relativePosition;
	wxSize mySize;
	bool isShown = false;
	double double_val = 0.0;

private:

	// This string stores the value that was replaced when a new value was copied over...
	wxString backup_value = "";

	// this flag value is used to determine how to display the data...
	int flag = 0;

	// This flag stores the value that was replaced when a new value was copied over...
	int backup_flag = 0;

	// Possible fonts..
	wxFont rowStartFont = wxFont(14, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxFont normalFont = wxFont(13, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM);

	// Possible text colours...
	wxColour green = wxColour(13, 158, 20);
	wxColour red = wxColour(217, 7, 28);
	wxColour yellow = wxColour(176, 194, 16);
	wxColour colHeaderColour = wxColour(213, 220, 227);
	wxColour rowHeaderColour = wxColour(182, 203, 242);
	wxColour white = wxColour(255, 255, 255);


	// Possible background colours...
	wxColour normal = wxColour(13, 10, 36);
	wxColour totalRowColor = wxColour(21, 41, 46);
	wxColour onClick;
	wxColour hover = wxColour(11, 117, 143);

	int isTotalRow = false;
	int isHeaderRow = false;
	int isHeaderCol = false;
	int isDataCell = false;
};

class GridCanvas2;

class Gnode2 : public GridVal
{
public:
	Gnode2(GridView2*, wxWindow*, wxSize, size_t, size_t, Gnode2*, Gnode2*, Gnode2*, Gnode2*, wxString&, int);
	Gnode2(GridView2*, wxWindow*, wxSize, size_t, size_t, Gnode2*, Gnode2*, Gnode2*, Gnode2*, int);
	// Copies everything..
	void CopyGnode2(const Gnode2&);
	void SetUP(Gnode2*);
	void SetRight(Gnode2*);
	void SetDown(Gnode2*);
	void SetLeft(Gnode2*);
	Gnode2* GetUP();
	Gnode2* GetRight();
	Gnode2* GetDown();
	Gnode2* GetLeft();
	Gnode2* GetFarthestLeft();
	void CopyFromTopNeighbor();
	void CopyFromBottomNeighbor();
	void CopyGnodeData(wxVector<GridValData>&, GridValData*);
	void GetRowData(wxVector<GridValData>&);
	void ClearSelf(bool from_bottom = false);
	void ClearSelf(Gnode2*);
	GridView2* GetParent() { return this->m_parent; }
	
private:
	GridView2* m_parent = NULL;
	Gnode2* m_up = nullptr;
	Gnode2* m_right = nullptr;
	Gnode2* m_down = nullptr;
	Gnode2* m_left = nullptr;
	wxString emptystring = "";
	int rows = 0;
	int cols = 0;
};

class mainwindow;

class GridCanvas2 : public wxWindow
{
public:
	GridCanvas2(mainwindow*, int, int, wxPoint, wxSize, Gnode2*&, wxSize);
	~GridCanvas2();
	void SetHeadNode(Gnode2*);
	void OnPaint(wxPaintEvent&);
	void ScrollChanged(wxScrollEvent&);
	void OnLineChanged(wxScrollEvent&);
	void OnScrollTop(wxScrollEvent&);
	void OnScrollBottom(wxScrollEvent&);
	void OnSizeChange(wxSizeEvent&);
	void OnMouseEnter(wxMouseEvent&);
	void OnMouseMove(wxMouseEvent&);
	void OnMouseLeave(wxMouseEvent&);
	void OnRightDown(wxMouseEvent&);
	void OnLeftDown(wxMouseEvent&);
	void SetScrollBars();
	void Initialize();
	wxDECLARE_EVENT_TABLE();
private:
	void PaintChild(wxGraphicsContext*, wxString&, wxPoint&, wxSize&);
	void OnDynamicPaint();

	// The second param will send a null Gnode2 after getting to the end of the row
	// You would set this true when you only want to cycle through the current row the 
	// Gnode2 you sent in as a paramater...
	Gnode2* GetNextElem(Gnode2*, bool row = false);
	Gnode2* FindMyFocus(wxPoint&);
	void SetMyToolTip();
	bool IsFocus(Gnode2*, wxPoint&);
	bool CheckNeighborsForFocus(wxPoint&);
private:
	mainwindow* m_parent = NULL;
	Gnode2* head = NULL;
	Gnode2* focus = NULL;
	wxSize child_size = wxSize(204, 30);
	int rows = 0;
	int cols = 0;
	wxColour background;
	wxColour textcolor;
	wxFont textfont;
	int distance = 4;
	TextExtent textextent;
	wxScrollBar* m_vrtcl = NULL;
	wxScrollBar* m_hrzntl = NULL;
	int y_position = 0;
	int x_position = 0;
	int horizontalrange = 0;
	int verticalrange = 0;
	int maxScrollPosH = 0;
	int minScrollPosH = 0;
	int maxScrollPosV = 0;
	int minScrollPosV = 0;
};

struct GridValData
{
	GridValData(Gnode2* g) : gn(g)
	{
		this->flags = gn->Getflags();
		this->value = gn->GetTextValue();
	}

	Gnode2* gn = NULL;
	int flags = 0;
	wxString value = "";
};

class ChartControlWin;

class ChartControl : public wxWindow
{
public:
	ChartControl(wxWindow* grandparent, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, wxString&, wxVector<Day_Prices>*);
	~ChartControl();
	wxVector<Day_Prices>* values = NULL;
	wxString title = "";
	void SetNewVector(wxVector<Day_Prices>*);
	wxDECLARE_EVENT_TABLE();
	void OnPaint(wxPaintEvent& evt);
	void OnClose(wxCloseEvent&);
	void EnterWindow(wxMouseEvent&);
	void Motion(wxMouseEvent&);
	void OnLeftDown(wxMouseEvent&);
private:
	std::tuple<int, double, double> calculateChartSegmentCountAndRange(double origLow, double origHigh);
private:
	wxWindow* m_parent = NULL;
	wxWindow* m_grandparent = NULL;
	wxVector<std::pair<wxPoint2DDouble, Day_Prices>> mypair;
};

class ChartControlWin;

// class to display an x for exiting window
class X_Canvas : public wxWindow
{
public:
	X_Canvas(ChartControlWin*, wxPanel*, wxWindowID, const wxPoint, const wxSize, wxColour, wxColour, wxColour);
	wxDECLARE_EVENT_TABLE();
	void OnEnterWin(wxMouseEvent&);
	void OnWinLeave(wxMouseEvent&);
	void OnLeftDown(wxMouseEvent&);
	void OnPaint(wxPaintEvent&);
	void OnErase(wxEraseEvent&);
private:
	ChartControlWin* m_grand_parent = NULL;
	wxPanel* m_parent = NULL;
	wxColour x_color;
	wxColour hover;
	wxColour normal_background;
	wxColour m_background;
	wxColour m_foreground;
	bool _hover = false;
};

class ChartControlWin : public wxWindow
{
public:
	ChartControlWin(mainwindow*, wxWindowID, const wxPoint&, const wxSize&, wxString&, wxVector<Day_Prices>*, StockViewerData*, _ChartControlType);
	void SetNewValues(wxVector<Day_Prices>*, StockViewerData*);
	void OnExit();
	wxDECLARE_EVENT_TABLE();
	void OnClickPanel(wxMouseEvent&);
	void OnPressEnter(wxCommandEvent&);
private:
	void Create();
	void ReCalibratePanelData();
private:
	ChartControl* chart = NULL;
	mainwindow* m_parent = NULL;
	StockViewerData* svd = NULL;
	wxTextCtrl* start_range = NULL;
	wxTextCtrl* end_range = NULL;
	wxString ticker = "";
	wxString rangebegin_temp = "";
	wxString range_end_temp = "";
	wxString rangebegin = "";
	wxString range_end = "";
	X_Canvas* exit = NULL;

	wxStaticText* shares = NULL;
	wxStaticText* daygain = NULL;
	wxStaticText* weekgain = NULL;
	wxStaticText* quartergain = NULL;
	wxStaticText* yeargain = NULL;
	wxStaticText* totalgain = NULL;
	_ChartControlType type = UNOWNED;
};

class GridView2 : public GridCanvas2
{
public:
	GridView2(wxWindow*, mainwindow*, int, int, wxSize);
	~GridView2();
	void SetNumOfItems(int);
	void SetNewRow(StockViewerData*);
	void UpdateRow(StockViewerData*);
	bool ItemExist(wxString&);
	virtual void OnSortClick(Gnode2*);
	virtual bool RemoveRow(wxString&);
protected:
	void SortStrings(Gnode2*);
	void SortDouble(Gnode2*);
	void SetTitleRow();
	void FillGrid();
private:
	wxString GetColTitle(size_t);
	Gnode2* GetEmptyRow();
	wxString GetStringLabel(size_t&, StockViewerData*);
	int GetGridNodeFlags(size_t&, size_t&);
	Gnode2* GetFarthestRight(Gnode2*);
	Gnode2* GetRight(Gnode2*, size_t, size_t start = 0);
	Gnode2* GetMatch(wxString&);
protected:
	wxVector<wxString> headers;
	Gnode2* head = NULL;
	mainwindow* m_parent = NULL;
	int cols = 0;
	int rows = 0;
	int filledNodes = 0;
	int gridnode_Size = 0;
	int itemsize = 1;
	Gnode2* summaryrow = NULL;
	bool acending = false;
};

class PortfolioWin;

class MyThread : public wxThread
{
public:
	MyThread(PortfolioWin*, MotionCanvas*);
	~MyThread();
	wxThread::ExitCode Entry();
	void EndAfterEntry() { end = true; }
private:
	bool end = false;
	MotionCanvas* gc = NULL;
	PortfolioWin* m_parent = NULL;
};

class MyQuoteThread : public wxThread
{
public:
	MyQuoteThread(Portfolio*, mainwindow*, wxString&);
	~MyQuoteThread();
	wxThread::ExitCode Entry();
private:
	Portfolio* port = NULL;
	mainwindow* m_parent = NULL;
	wxString ticker = "";
};

class LoadSectorStocksThread : public wxThread
{
public:
	LoadSectorStocksThread(mainwindow*);
	~LoadSectorStocksThread();
	wxThread::ExitCode Entry() wxOVERRIDE;
private:
	mainwindow* m_parent = NULL;
};

class PortfolioWin : public wxWindow
{
public:
	PortfolioWin(mainwindow*, wxWindowID, wxDateTime*, StockViewerData*, Portfolio*);
	void Update();
	void ClosingSoon();
private:
	void Create();
	void SetControlFonts();
	void DateChange(wxCommandEvent&);
	void OnUpdateButtonPress(wxCommandEvent&);
	wxDECLARE_EVENT_TABLE();
public:
	MyThread* thread = NULL;
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
	wxColour Green = wxColour(16, 82, 31);
	wxColour Blue = wxColour(36, 11, 191);
	wxColour DarkGrey = wxColour(43, 43, 43);
	wxColour parenths = wxColour(0, 0, 0);
	wxColour percents = wxColour(41, 47, 227);

	MotionCanvas* canvas = NULL;
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
	void GetChartViewData(wxString&, wxDateTime&, wxDateTime&);
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
	void CreateChart_View_Data_Entry();
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
	bool HandleChartViewDataEntry();
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
	void OnMouseRightDown(wxMouseEvent&);
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

class SectorStockWindow : public wxFrame
{
public: 
	SectorStockWindow(mainwindow*, wxVector<SectorStock>*, wxString&);
	wxDECLARE_EVENT_TABLE();
	void CloseEvent(wxCloseEvent&);
	void ScrollBottom(wxScrollWinEvent&);
private:
	void Create();
private:
	wxVector<SectorStock>* myvec = NULL;
	mainwindow* m_parent = NULL;
	wxString sectorname = "Sector";

	wxBoxSizer* vert1 = NULL;
	wxBoxSizer* vert2 = NULL;
	wxBoxSizer* vert3 = NULL;
	wxBoxSizer* vert4 = NULL;
	wxBoxSizer* vert5 = NULL;

	wxScrolled<wxWindow>* P = NULL;

	// this will be the last one prited to the screen...
	SectorStock* it = NULL;
	wxStaticText* lastticker = NULL;
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
	void SectorRightClick(wxVector<SectorStock>*, wxString&);
	void RightClickGrid(wxString&, wxPoint&);
	void OnChartWinChangeDateRange(wxString&, wxDateTime&, wxDateTime&);

	// OnThreadCompletion callback function for portfolio to call after it retrieves all of its data...
	void OnThreadCompletion();
	void OnThreadCompletion(wxThreadEvent&);

	// Event functions...
	void CloseEvent(wxCloseEvent&);
	void ClosePanelEvent(wxCloseEvent&);

	// Helper functions for OnPurchaseMenu...
	void PurchaseWin(wxString&);
	void OnPurchaseMenu(wxCommandEvent&);
	// End of helper functions for OnPurchaseMenu and OnPurchaseMenu itself...

	//Helper functions for OnSellMenu...
	stock_node* LotSelectionWindow(wxString&);
	void UserEnterSellDataWin(stock_node*);
	void OnSellMenu(wxCommandEvent&);
	// End of helper functions for OnSellMenu..

	void OnHome(wxCommandEvent&);
	void SampleStockChartView(wxCommandEvent&);
//	void SampleStockChartView(wxCommandEvent&);

	void GoToHomeWindow();
	void OnSellPopupClick(wxCommandEvent&);
	void OnPurchasePopupClick(wxCommandEvent&);
	void OnAddReInvestSharesPopup(wxCommandEvent&);
	void OnQuoteLookupPopup(wxCommandEvent&);
	void ChartViewHelper(const wxString&, const wxString&, wxVector<Day_Prices>*, StockViewerData*, _ChartControlType);
//	void ChartViewHelper(SampleStock*);
	void OnChartView(wxCommandEvent&);
	void OnChartView(wxString&);
	void OnLoadSectorStocks(wxCommandEvent&);

	// thread calls this after OnQuoteLookupPopup...
	void OnQuoteThread();
	void OnQuoteThread(wxThreadEvent&);
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
	void OnDateChange(wxString&);

	// function called by PortfolioWin when user presses update button on left window;
	void OnUpdate();
	wxDECLARE_EVENT_TABLE();
public:
	MyQuoteThread* quotethread = NULL;
	SummaryData smd;
	LoadSectorStocksThread* mainwindow_thread = NULL;
private:
	wxWindow* GetLeftWin();
	wxScrolledCanvas* GetRightWin();
	void GetRightWin(bool);
	wxWindow* GetBottomFrame();
	void UpdateGridView();
	void SaveFile();
	void RetrieveFile();
	void DeleteSampleStock();
	void DeletePopupMenu();
	void DeleteQuoteThread();
	void DeleteLoadStocksThread();
	void CreatePopupMenu();
private:
	wxScrolledCanvas* grid_panel = NULL;
	GridView2* grid_canvas= NULL;
	PortfolioWin* portwin = nullptr;
	BottomFrame* bottom_frame = nullptr;
	Dialog* dialog = nullptr;
	GenericListWin* generic_list = NULL;
	SectorStockWindow* sectorstockwin = NULL;
	ChartControl* chart = NULL;
	wxFrame* chartframe = NULL;
	ChartControlWin* chartwin = NULL;
	SampleStock* sample = NULL;

	wxBoxSizer* top = NULL;

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
	wxMenuItem* p_graph = NULL;
	wxMenu* popup = NULL;
	int counter = 0;

	wxStatusBar* statbar = NULL;
};

