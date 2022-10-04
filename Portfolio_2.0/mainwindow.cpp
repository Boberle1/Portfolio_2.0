#include "mainwindow.h"

wxBEGIN_EVENT_TABLE(mainwindow, wxFrame)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(GridNode, wxStaticText)
wxEND_EVENT_TABLE()

auto constexpr TICKER = "Ticker";
auto constexpr SHARES = "Shares";
auto constexpr PRICE = "Price";
auto constexpr _PREVIOUS_CLOSE = "Previous Close";
auto constexpr COST_BASIS = "Cost Basis";
auto constexpr TOTAL_VALUE = "Total Value";
auto constexpr DAY_GAIN = "Day Gain";
auto constexpr WEEK_GAIN = "Week Gain";
auto constexpr QUARTER_GAIN = "Quarter Gain";
auto constexpr YEAR_GAIN = "Year Gain";
auto constexpr TOTAL_GAIN = "Total Gain";
auto constexpr PURCHASE_DATE = "Purchase Date";
auto constexpr PURCHASE_PRICE = "Purchase Price";
auto constexpr _MARKET_VALUE = "Market Value";
auto constexpr EARNINGS_DATE = "Next Earnings Date";
auto constexpr DIV_DATE = "Ex-Dividend";
auto constexpr DIVIDEND = "Dividend";
auto constexpr TOTAL_DIVIDENDS = "Total Dividends";
auto constexpr DIV_RE_INVEST = "Dividend Re-Investment";
auto constexpr _SECTOR = "Sector";
auto constexpr PORTFOLIO_PERC = "Portfolio Percent";

// Virtual List view functions...
VirtualListView::VirtualListView(wxWindow* w, wxWindowID id, wxPoint p, wxSize s, Portfolio* port) : wxListView(w, id, p, s, wxLC_REPORT | wxLC_VIRTUAL), portfolio(port)
{
	this->SetBackgroundColour(wxColour(50, 55, 52));
	this->SetForegroundColour(wxColour(255, 255, 255));

	this->AppendColumn(TICKER), Col.push_back(TICKER);
	this->AppendColumn(SHARES), Col.push_back(SHARES);
	this->AppendColumn(PRICE), Col.push_back(PRICE);
	this->AppendColumn(COST_BASIS), Col.push_back(COST_BASIS);
	this->AppendColumn(TOTAL_VALUE), Col.push_back(TOTAL_VALUE);
	this->AppendColumn(DAY_GAIN), Col.push_back(DAY_GAIN);
	this->AppendColumn(WEEK_GAIN), Col.push_back(WEEK_GAIN);

	this->InitializeView();
}

wxString VirtualListView::OnGetItemText(long index, long col) const
{
	const ItemData* data = &this->items[index];
	wxString test = this->GetItemValue(data, col);
	return test;
}

wxString VirtualListView::GetItemValue(const ItemData* id, long index) const
{
	if (this->Col[index] == TICKER)
		return id->stock->GetTicker();
	if (this->Col[index] == SHARES)
		return wxNumberFormatter::ToString(id->stock->GetShares(), 2);
	if (this->Col[index] == PRICE)
		return wxNumberFormatter::ToString(id->stock->GetPrice(), 2);
	if (this->Col[index] == COST_BASIS)
		return "0.0";
	if (this->Col[index] == TOTAL_VALUE)
		return "0.0";
	if (this->Col[index] == DAY_GAIN)
		return wxNumberFormatter::ToString(id->stock->GetDayReturn(), 2) + "%";
	if (this->Col[index] == WEEK_GAIN)
		return wxNumberFormatter::ToString(id->stock->GetWeekReturn(), 2) + "%";

	return "no value";
}

void VirtualListView::InitializeView()
{
	this->SetItemCount(portfolio->GetNumItems(_PortfolioType::STOCK));
	wxVector<StockNode*> sn = this->portfolio->GetStockNodeItems();

	for (size_t i = 0; i < sn.size(); ++i)
		this->items.push_back(ItemData(sn[i]));

	this->RefreshOnUpdate();
}

void VirtualListView::RefreshOnUpdate()
{
	this->UpdateItemsCount();
	this->Refresh();
}

void VirtualListView::UpdateItemsCount()
{
	this->SetItemCount(this->items.size());
}

// GridView functions...

GridNode::GridNode(GridView* gv, wxWindow* w, size_t row, size_t col, GridNode* up, GridNode* right, GridNode* down, GridNode* left, wxString tval, double dval, bool percent)
	: wxStaticText(w, wxID_ANY, tval), m_grid_view(gv), m_parent(w), m_row(row), m_col(col), m_up(up),
	m_right(right), m_down(down), m_left(left), t_val(tval), d_val(dval), is_double(true), is_percent(percent), is_empty(false)
{
	int size = 13;
	if (!col)
		this->Bind(wxEVT_LEFT_DOWN, &GridNode::OnClickEvent, this);
	
	else if (!row)
	{
		this->Bind(wxEVT_LEFT_DOWN, &GridNode::OnClickEvent, this);
		size = 14;
	}

	wxFont font(size, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_HEAVY);
	wxStaticText* t = new wxStaticText(this, wxID_ANY, "");
	this->SetForegroundColour(wxColour(255, 255, 255));
	this->SetBackgroundColour(m_parent->GetBackgroundColour());
	this->SetFont(font);
	
	this->normal = m_parent->GetBackgroundColour();
	this->onclick = wxColour(42, 43, 43);
}

GridNode::GridNode(GridView* gv, wxWindow* w, size_t row, size_t col, GridNode* up, GridNode* right, GridNode* down, GridNode* left, wxString tval, int ival)
	: wxStaticText(w, wxID_ANY, tval), m_grid_view(gv), m_parent(w), m_row(row), m_col(col), m_up(up), m_right(right),
	m_down(down), m_left(left), t_val(tval), int_val(ival), is_int(true), is_empty(false)
{
	int size = 13;
	if (!col)
		this->Bind(wxEVT_LEFT_DOWN, &GridNode::OnClickEvent, this);

	else if (!row)
	{
		this->Bind(wxEVT_LEFT_DOWN, &GridNode::OnClickEvent, this);
		size = 14;
	}

	wxFont font(size, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_HEAVY);
	this->SetForegroundColour(wxColour(105, 177, 245));
	this->SetBackgroundColour(m_parent->GetBackgroundColour());
	this->SetFont(font);

	this->normal = m_parent->GetBackgroundColour();
	this->onclick = wxColour(42, 43, 43);
}

GridNode::GridNode(GridView* gv, wxWindow* w, size_t row, size_t col, GridNode* up, GridNode* right, GridNode* down, GridNode* left, wxString tval)
	: wxStaticText(w, wxID_ANY, tval), m_grid_view(gv), m_parent(w), m_row(row), m_col(col), m_up(up), m_right(right), m_down(down), m_left(left), t_val(tval), is_empty(false)
{
	int size = 13;
	if (!col)
		this->Bind(wxEVT_LEFT_DOWN, &GridNode::OnClickEvent, this);

	else if (!row)
	{
		this->Bind(wxEVT_LEFT_DOWN, &GridNode::OnClickEvent, this);
		size = 14;
	}

	wxFont font(size, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_HEAVY);
	this->SetForegroundColour(wxColour(255, 255, 255));
	this->SetBackgroundColour(m_parent->GetBackgroundColour());
	this->SetFont(font);

	this->normal = m_parent->GetBackgroundColour();
	this->onclick = wxColour(42, 43, 43);
}

GridNode::GridNode(GridView* gv, wxWindow* w, size_t row, size_t col, GridNode* up, GridNode* right, GridNode* down, GridNode* left)
	: wxStaticText(w, wxID_ANY, ""), m_grid_view(gv), m_parent(w), m_row(row), m_col(col), m_up(up), m_right(right), m_down(down), m_left(left), is_empty(true)
{
	int size = 13;
	if (!col)
		this->Bind(wxEVT_LEFT_DOWN, &GridNode::OnClickEvent, this);

	else if (!row)
	{
		this->Bind(wxEVT_LEFT_DOWN, &GridNode::OnClickEvent, this);
		size = 14;
	}

	wxFont font(size, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_HEAVY);
	this->SetForegroundColour(wxColour(255, 255, 255));
	this->SetBackgroundColour(m_parent->GetBackgroundColour());
	this->SetFont(font);

	this->normal = m_parent->GetBackgroundColour();
	this->onclick = wxColour(42, 43, 43);
}

GridNode::~GridNode()
{

}

bool GridNode::SetNewVal(double val, bool ispercent, bool iscolor)
{
	if (iscolor)
	{
		wxColour color;
		if (val == 0.0)
			this->SetForegroundColour(wxColour(187, 196, 14));
		else if (val > 0.0)
			this->SetForegroundColour(wxColour(13, 158, 20));
		else
			this->SetForegroundColour(wxColour(217, 7, 28));
	}
	this->d_val = val;
	this->is_percent = ispercent;
	wxString percent = is_percent ? "%" : "";
	this->SetLabel(wxNumberFormatter::ToString(this->d_val, 2) + percent);
	this->Refresh();
	this->is_empty = false;
	return true;
}

bool GridNode::SetNewVal(int i)
{
	this->int_val = i;
	this->SetLabel(wxString::Format("%i", this->int_val));
	this->Refresh();
	this->is_empty = false;
	return true;
}

bool GridNode::SetNewVal(wxString val)
{
	
	this->t_val = val;
	this->SetLabel(val);
	this->Refresh();
	this->is_empty = this->t_val.IsEmpty() ? true : false;
	return true;
}

void GridNode::SetUP(GridNode* gn)
{
	this->m_up = gn;
}

void GridNode::SetRight(GridNode* gn)
{
	this->m_right = gn;
}

void GridNode::SetDown(GridNode* gn)
{
	this->m_down = gn;
}

void GridNode::SetLeft(GridNode* gn)
{
	this->m_left = gn;
}

GridNode* GridNode::GetUP()
{
	return this->m_up;
}

GridNode* GridNode::GetRight()
{
	return this->m_right;
}

GridNode* GridNode::GetDown()
{
	return this->m_down;
}
GridNode* GridNode::GetLeft()
{
	return this->m_left;
}

bool GridNode::IsEmpty()
{
	return this->is_empty;
}

bool GridNode::IsID(int id)
{
	if (this->GetId() == id)
		return true;

	return false;
}

void GridNode::OnClickEvent(wxMouseEvent& evt)
{
	this->SetBackgroundColour(this->GetBackgroundColour() == this->onclick ? this->normal : this->onclick);
	this->Refresh();
	if (!this->m_row)
	{
		if (this->GetBackgroundColour() == this->onclick)
			this->m_grid_view->OnClickColItem(this);
		else
			this->m_grid_view->OnClickColItem(nullptr);

		if (this->m_down)
		{
			if (this->GetBackgroundColour() == this->onclick)
				this->m_down->OnClickColON();
			else
				this->m_down->OnClickColOff();
		}
	}
	else
	{
		if (this->GetBackgroundColour() == this->onclick)
			this->m_grid_view->OnClickRowItem(this);
		else
			this->m_grid_view->OnClickRowItem(nullptr);

		if (this->m_right)
		{
			if (this->GetBackgroundColour() == this->onclick)
				this->m_right->OnClickRowON();
			else
				this->m_right->OnClickRowOff();
		}
	}
}

void GridNode::OnClickRowOff()
{
	this->SetBackgroundColour(this->normal);
	this->Refresh();
	if (this->m_right)
		this->m_right->OnClickRowOff();
}

void GridNode::OnClickColOff()
{
	this->SetBackgroundColour(this->normal);
	this->Refresh();
	if (this->m_down)
		this->m_down->OnClickColOff();
}

void GridNode::OnClickColON()
{
	this->SetBackgroundColour(this->onclick);
	this->Refresh();
	if (this->m_down)
		this->m_down->OnClickColON();
}

void GridNode::OnClickRowON()
{
	this->SetBackgroundColour(this->onclick);
	this->Refresh();
	if (this->m_right)
		this->m_right->OnClickRowON();
}

void GridNode::OnClickItemOff()
{
	this->UnClick();
	this->Refresh();
}

void GridNode::ChangeColorToClicked(int direction)
{
	this->SetBackgroundColour(this->GetBackgroundColour() == this->onclick ? this->normal : this->onclick);
	this->Refresh();
	GridNode* gn = direction ? this->m_right : m_down;
	if (gn)
		gn->ChangeColorToClicked(direction);
}

void GridNode::UnClick()
{
	this->SetBackgroundColour(this->normal);
}

// Private VirtualListView functions....
void VirtualListView::m_SetItemCount()
{
	this->SetItemCount(100);
}

// GridView functions...

GridView::GridView(wxWindow* w, int cols) : wxGridSizer(cols), m_parent(w)
{
	this->SetTitleRow();
	this->FillGrid();
}

GridView::~GridView()
{
	
}

void GridView::SetNewRow(wxString ticker, double shares, double price, double previo_close, double day_gain, double week_gain, 
	double quarter_gain, double year_gain, double total_gain)
{
	size_t size = this->GetCols();
	GridNode* first = this->GetFirstEmptyRow();
	if (!first)
		return;
	for (size_t i = 0; i < size; ++i)
	{
		switch (i)
		{
		case 0: first->SetNewVal(ticker); break;
		case 1: first->SetNewVal(shares, false, false); break;
		case 2: first->SetNewVal(price, false, false); break;
		case 3: first->SetNewVal(previo_close, false, false); break;
		case 4: first->SetNewVal(day_gain, true, true); break;
		case 5: first->SetNewVal(week_gain, true, true); break;
		case 6: first->SetNewVal(quarter_gain, true, true); break;
		case 7: first->SetNewVal(year_gain, true, true); break;
		case 8: first->SetNewVal(total_gain, true, true); break;
		}
		
		first = first->GetRight();
		if (first)
			continue;
		else 
			break;

	}
}

void GridView::SetNewRow(StockViewerData* svd)
{
	size_t size = this->GetCols();
	GridNode* first = this->GetFirstEmptyRow();
	if (!first)
		return;
	for (size_t i = 0; i < size; ++i)
	{
		first->SetNewVal(this->GetLabelText(i, svd));
		first = first->GetRight();
		if (first)
			continue;
		else
			break;
	}
}

void GridView::Cleanup()
{
	GridNode* nextrow = nullptr;
	GridNode* currentrow = this->head;
	while (1)
	{
		if (currentrow)
			nextrow = currentrow->GetDown();
		this->DeleteRow(currentrow);
		if (nextrow)
			currentrow = nextrow;
		
		else
			break;
	}
}

void GridView::OnClickRowItem(GridNode* gn)
{
	this->ClickColOff();
	if (this->grid_row)
		this->grid_row->OnClickRowOff();

	this->grid_row = gn;
}

void GridView::OnClickColItem(GridNode* gn)
{
	this->ClickRowOff();
	if (this->grid_col)
		this->grid_col->OnClickColOff();

	this->grid_col = gn;
}

void GridView::OnClickItem(GridNode* gn)
{
	if (this->clicked_node)
		this->clicked_node->OnClickItemOff();

	this->clicked_node = gn;
}

// private GridView functions...

void GridView::SetTitleRow()
{
	size_t cols = this->GetCols();
	wxString val = "";
	GridNode* last = nullptr;
	for (size_t i = 0; i < cols; ++i)
	{
		val = this->GetColTitle(i);
		if (i == 0)
		{
			this->head = new GridNode(this, this->m_parent, (size_t)0, i, NULL, NULL, NULL, NULL, val);
			this->Add(this->head, 0, wxALIGN_TOP | wxTOP | wxLEFT | wxEXPAND, 4);
		}
		else if (i == 1)
		{
			this->head->SetRight(new GridNode(this, this->m_parent, (size_t)0, i, nullptr, nullptr, nullptr, this->head, val));
			this->Add(head->GetRight(), 0, wxALIGN_TOP | wxTOP | wxLEFT | wxEXPAND, 4);
			last = this->head->GetRight();
		}
		else
		{
			last->SetRight(new GridNode(this, this->m_parent, (size_t)0, i, nullptr, nullptr, nullptr, last , val));
			this->Add(last->GetRight(), 0, wxALIGN_TOP | wxTOP | wxLEFT | wxEXPAND, 4);
			last = last->GetRight();
		}
		++this->gridnode_Size;
	}
}

void GridView::FillGrid()
{
	GridNode* last = nullptr;
	GridNode* last_row = head;
	GridNode* current_row = nullptr;
	size_t rowsize = this->GetCols();
	size_t size = rowsize * 2;
	for (size_t i = 0; i < size; ++i)
	{
		if (i > 0)
			last_row = current_row;
		for (size_t j = 0; j < this->GetCols(); ++j)
		{
//			GridNode* t = new GridNode(this->m_parent, i + 1, j, nullptr, nullptr, nullptr, nullptr);
//			this->Add(t, 0, wxALIGN_TOP | wxTOP | wxLEFT | wxEXPAND, 4);

			if (j == 0)
			{
				last_row->SetDown(new GridNode(this, this->m_parent, i + 1, j, last_row, nullptr, nullptr, nullptr));
				this->Add(last_row->GetDown(), 0, wxALIGN_TOP | wxTOP | wxLEFT | wxEXPAND, 4);
				last = current_row = last_row->GetDown();
			}
			else 
			{
				GridNode* lastrowitem = this->GetRight(last_row, j);
				GridNode* currentrowitem = this->GetFarthestRight(current_row);
				if (currentrowitem)
				{
					currentrowitem->SetRight(new GridNode(this, this->m_parent, i + 1, j, lastrowitem, nullptr, nullptr, last));
					this->Add(currentrowitem->GetRight(), 0, wxALIGN_TOP | wxTOP | wxLEFT | wxEXPAND, 4);
					if (lastrowitem)
						lastrowitem->SetDown(currentrowitem->GetRight());
				}
			}
			++this->gridnode_Size;
		}
	}
}

void GridView::ClickRowOff()
{
	if (this->grid_row)
		this->grid_row->OnClickRowOff();

	this->grid_row = nullptr;
}

void GridView::ClickColOff()
{
	if (this->grid_col)
		this->grid_col->OnClickColOff();

	this->grid_col = nullptr;
}

wxString GridView::GetLabelText(size_t& index, StockViewerData* svd)
{
	if (!svd)
	{
		wxFAIL_MSG("StockViewerData* is nullptr in GridView::GetLabelText!");
		return "";
	}

	wxString collabel = this->GetColTitle(index);
	if (collabel == TICKER)
		return svd->GetTicker();
	if (collabel == SHARES)
		return svd->GetShares();
	if (collabel == PRICE)
		return svd->GetPrice();
	if (collabel == _PREVIOUS_CLOSE)
		return svd->GetPreviousClose();
	if (collabel == DAY_GAIN)
		return svd->GetDayGain();
	if (collabel == WEEK_GAIN)
		return svd->GetWeekGain();
	if (collabel == QUARTER_GAIN)
		return svd->GetQuarterGain();
	if (collabel == YEAR_GAIN)
		return svd->GetYearGain();
	if (collabel == TOTAL_GAIN)
		return svd->GetTotalGain();
	if (collabel == COST_BASIS)
		return svd->GetCostBasis();
	if (collabel == _MARKET_VALUE)
		return svd->GetMarketValue();
	if (collabel == PURCHASE_PRICE)
		return svd->GetPurchasePrice();
	if (collabel == TOTAL_DIVIDENDS)
		return svd->GetDividends();
	if (collabel == EARNINGS_DATE)
		return svd->GetEarningsDate();
	if (collabel == PURCHASE_DATE)
		return svd->GetPurchaseDate();
	return "";
}

wxString GridView::GetColTitle(size_t col)
{
	switch (col)
	{
	case 0: return TICKER;
	case 1: return SHARES;
	case 2: return PRICE;
	case 3: return _PREVIOUS_CLOSE;
	case 4: return DAY_GAIN;
	case 5: return WEEK_GAIN;
	case 6: return QUARTER_GAIN;
	case 7: return YEAR_GAIN;
	case 8: return TOTAL_GAIN;
	case 9: return COST_BASIS;
	case 10: return _MARKET_VALUE;
	case 11: return PURCHASE_PRICE;
	case 12: return TOTAL_DIVIDENDS;
	case 13: return EARNINGS_DATE;
	case 14: return PURCHASE_DATE;
	case 15: return "";
	case 16: return "";
	case 17: return "";
	case 18: return "";
	case 19: return "";
	}

	return "";
}

GridNode* GridView::GetFirstEmptyRow()
{
	GridNode* next = this->head->GetDown();
	if (!next)
		return next;
	while (1)
	{
		if (next)
		{
			if (next->IsEmpty())
				return next;

			next = next->GetDown();
		}
		else 
			return nullptr;
	}

	return nullptr;
}

GridNode* GridView::GetFarthestRight(GridNode* g)
{
	if (!g)
	{
		wxFAIL_MSG("GridNode* passed to GridView::GetFarthestRight is nullptr!");
		return nullptr;
	}

	while (1)
	{
		if (!g->GetRight())
			return g;

		g = g->GetRight();
	}

	return nullptr;
}

GridNode* GridView::GetFarthestLeft(GridNode* g)
{
	if (!g)
	{
		wxFAIL_MSG("GridNode* passed to GridView::GetFarthestLeft is nullptr!");
		return nullptr;
	}

	while (1)
	{
		if (!g->GetLeft())
			return g;

		g = g->GetLeft();
	}

	return nullptr;
}

GridNode* GridView::GetFarthestTop(GridNode* g)
{
	if (!g)
	{
		wxFAIL_MSG("GridNode* passed to GridView::GetFarthestTop is nullptr!");
		return nullptr;
	}

	while (1)
	{
		if (!g->GetUP())
			return g;

		g = g->GetUP();
	}

	return nullptr;
}

GridNode* GridView::GetFarthestBottom(GridNode* g)
{
	if (!g)
	{
		wxFAIL_MSG("GridNode* passed to GridView::GetFarthestBottom is nullptr!");
		return nullptr;
	}

	while (1)
	{
		if (!g->GetDown())
			return g;

		g = g->GetDown();
	}

	return nullptr;
}

GridNode* GridView::GetRight(GridNode* g, size_t i, size_t start)
{
	if (!g)
	{
		wxFAIL_MSG("GridNode* passed to GridView::GetRight is nullptr!");
		return nullptr;
	}

	GridNode* next = nullptr;
	while (start < i)
	{
		if (!g->GetRight())
			return g;
		g = g->GetRight();
		++start;
	}

	return g;
}

GridNode* GridView::GetLeft(GridNode* g, size_t i, size_t start)
{
	if (!g)
	{
		wxFAIL_MSG("GridNode* passed to GridView::GetLeft is nullptr!");
		return nullptr;
	}

	while (start < i)
	{
		if (!g->GetLeft())
			return g;
		g = g->GetLeft();
		++start;
	}

	return g;
}

GridNode* GridView::GetUp(GridNode* g, size_t i, size_t start)
{
	if (!g)
	{
		wxFAIL_MSG("GridNode* passed to GridView::GetUP is nullptr!");
		return nullptr;
	}

	while (start < i)
	{
		if (!g->GetUP())
			return g;
		g = g->GetUP();
		++start;
	}

	return g;
}

GridNode* GridView::GetDown(GridNode* g, size_t i, size_t start)
{
	if (!g)
	{
		wxFAIL_MSG("GridNode* passed to GridView::GetDown is nullptr!");
		return nullptr;
	}

	while (start < i)
	{
		if (!g->GetDown())
			return g;
		g = g->GetDown();
		++start;
	}

	return g;
}

void GridView::DeleteRow(GridNode* g)
{
	if (!g)
		return;

	DeleteRow(g->GetRight());
	delete g;
	g = nullptr;
	--this->gridnode_Size;
}

GridNode* GridView::FindItem(GridNode* gn, int id)
{
	if (!gn)
		return nullptr;
	if (gn->IsID(id))
		return gn;

	return this->FindItem(gn->GetRight(), id);
}

void GridView::HighlightRow(GridNode* gn)
{
	
}

void GridView::HighlightCol(GridNode* gn)
{
	
}

GridNode* GridView::FindGridNode(int id)
{
	GridNode* current = this->head;
	GridNode* nextrow = nullptr;

	while (1)
	{
		if (current)
			nextrow = current->GetDown();

		GridNode* item = this->FindItem(current, id);

		if (item)
			return item;

		if (nextrow)
			current = nextrow;

		else 
			return nullptr;
	}

}


// wxSize(1700, 900))
// mainwindow functions...
mainwindow::mainwindow() : wxFrame(nullptr, 10000, "Brando's Investments", wxPoint(0, 0), wxSize(wxGetDisplaySize())), main_clock(wxDateTime::Today()),
	 port(this, 100000, &main_clock)
{
	this->Bind(wxEVT_CLOSE_WINDOW, &mainwindow::CloseEvent, this);
	this->Bind(wxEVT_THREAD, &mainwindow::OnThreadCompletion, this);
//	this->m_listCtrl = new VirtualListView(this, wxID_ANY, wxDefaultPosition, wxSize(wxSize(this->GetSize().GetWidth() - 10, this->GetSize().GetHeight() - 10)));
	this->port.NewDepositSchedule(25.0, 14, wxDateTime(30, wxDateTime::Month::Dec, 2021));
	wxString PurchaseDate = "03 / 19 / 2020";
	wxString later = "01 / 2 / 2021";
	wxString earlier = "01 / 2 / 2019";
	this->port.Purchase(_Sector::LARGECAP, "DIA", PurchaseDate, 40.414, 5, true, PurchaseDate);
	this->port.Purchase(_Sector::LARGECAP, "DIA", later, 100.414, 5, true, later);
	this->port.Purchase(_Sector::LARGECAP, "DIA", earlier, 90.414, 5, true, earlier);
	this->port.Purchase(_Sector::TECH, "AMD", PurchaseDate, 50, 10, true, PurchaseDate);
	this->port.Purchase(_Sector::LARGECAP, "SPY", PurchaseDate, 40.414, 5, false, PurchaseDate);
	this->port.Purchase(_Sector::TECH, "QQQ", PurchaseDate, 50, 10, false, PurchaseDate);

	this->grid_panel = this->GetRightWin();
	this->grid_panel->Bind(wxEVT_CLOSE_WINDOW, &mainwindow::ClosePanelEvent, this);
	this->left_win = this->GetLeftWin();
	this->bottom_frame = this->GetBottomFrame();
	this->bottom_frame->SetBackgroundColour(wxColour(28, 56, 100));
	wxBoxSizer* top = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* main = new wxBoxSizer(wxVERTICAL);
	top->Add(this->left_win, 1, wxEXPAND);
	top->Add(this->grid_panel, 3, wxEXPAND);
	main->Add(top, 3, wxEXPAND);
	main->Add(bottom_frame, 1, wxEXPAND);
	this->SetSizer(main);
	this->Layout();
	this->Show(true);
}

mainwindow::~mainwindow()
{
	
}

wxFrame* mainwindow::PurchaseWin()
{
	return NULL;
}

void mainwindow::CloseEvent(wxCloseEvent& evt)
{
	evt.Skip();
}

void mainwindow::OnThreadCompletion(wxThreadEvent& evt)
{
	wxVector<StockNode*> stn = this->port.GetStockNodeItems();
	for (size_t i = 0; i < stn.size(); ++i)
		this->grid_view->SetNewRow(stn[i]->GetTicker(), stn[i]->GetShares(), stn[i]->GetPrice(), stn[i]->GetPreviousClose(),
			stn[i]->GetDayReturn(), stn[i]->GetWeekReturn(), stn[i]->GetQuarterReturn(), stn[i]->GetYearReturn(), stn[i]->GetTotalReturn(true));
	this->Refresh();
}

void mainwindow::ClosePanelEvent(wxCloseEvent& evt)
{
	this->grid_view->Cleanup();
}

// mainwindow private functions..

wxWindow* mainwindow::GetLeftWin()
{
	wxWindow* w = new wxWindow(this, wxID_ANY);
	w->SetBackgroundColour(wxColour(100,100,100));
	return w;
}

wxScrolled<wxPanel>* mainwindow::GetRightWin()
{
	wxScrolled<wxPanel>* P = new wxScrolled<wxPanel>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	P->SetScrollbars(5, 5, 50, 50);
	P->EnableScrolling(true, true);
//	wxPanel* w = new wxPanel(scrolled, wxID_ANY);
	P->SetBackgroundColour(wxColour(0, 0, 0));
	this->grid_view = new GridView(P, 20);

	wxVector<StockNode*> stn = this->port.GetStockNodeItems();
	for (size_t i = 0; i < stn.size(); ++i)
		this->grid_view->SetNewRow(stn[i]->GetStockViewerData());

	P->SetSizer(grid_view);
	return P;
}

wxWindow* mainwindow::GetBottomFrame()
{
	return new wxWindow(this, wxID_ANY);
}