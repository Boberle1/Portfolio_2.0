#pragma once
#include "Portfolio.h"
#include "wx/listctrl.h"
#include "wx/valnum.h"
#include "wx/display.h"

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

class GridNode : public wxStaticText
{
public:
	GridNode(GridView*, wxWindow*, size_t, size_t, GridNode*, GridNode*, GridNode*, GridNode*, wxString, double, bool percent);
	GridNode(GridView*, wxWindow*, size_t, size_t, GridNode*, GridNode*, GridNode*, GridNode*, wxString, int);
	GridNode(GridView*, wxWindow*, size_t, size_t, GridNode*, GridNode*, GridNode*, GridNode*, wxString);
	GridNode(GridView*, wxWindow*, size_t, size_t, GridNode*, GridNode*, GridNode*, GridNode*);
	~GridNode();
	bool SetNewVal(double, bool, bool);
	bool SetNewVal(int);
	bool SetNewVal(wxString);
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
	wxColour normal;
	wxColour onclick;
	wxWindow* m_parent = nullptr;
	GridView* m_grid_view = nullptr;
};

class GridView : public wxGridSizer
{
public:
	GridView(wxWindow*, int);
	~GridView();
	void SetNewRow(wxString, double, double, double, double, double, double, double, double);
	void SetNewRow(StockViewerData*);
	void Cleanup();
	void OnClickColItem(GridNode*);
	void OnClickRowItem(GridNode*);
	void OnClickItem(GridNode*);
private:
//	wxStaticText* GetNewItem(size_t, size_t, double, bool ispercent = true);
	void SetTitleRow();
	void FillGrid();
	void ClickRowOff();
	void ClickColOff();
	wxString GetLabelText(size_t&, StockViewerData*);
	wxString GetColTitle(size_t);
	GridNode* GetFirstEmptyRow();
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
private:
	GridNode* head = nullptr;
	wxVector<StockNode*> stocks;
	wxVector<GridNode*> rows;
	wxVector<Sector*> sectors;
	const Portfolio* port = NULL;
	wxWindow* m_parent;
	int gridnode_Size = 0;
	GridNode* grid_col = nullptr;
	GridNode* grid_row = nullptr;
	GridNode* clicked_node = nullptr;
};

class mainwindow : public wxFrame
{
public:
	mainwindow();
	~mainwindow();
	wxFrame* PurchaseWin();
	void CloseEvent(wxCloseEvent&);
	void ClosePanelEvent(wxCloseEvent&);
	void OnThreadCompletion(wxThreadEvent&);
	wxDECLARE_EVENT_TABLE();
private:
	wxWindow* GetLeftWin();
	wxScrolled<wxPanel>* GetRightWin();
	wxWindow* GetBottomFrame();
private:
	wxScrolled<wxPanel>* grid_panel = NULL;
	wxScrolledWindow* scrolled = nullptr;
	GridView* grid_view = NULL;
	wxWindow* left_win = nullptr;
	wxWindow* right_win = nullptr;
	wxWindow* bottom_frame = NULL;
	Portfolio port;
	webdata data;
	wxDateTime main_clock;
};

