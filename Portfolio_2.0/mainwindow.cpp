#include "mainwindow.h"

wxBEGIN_EVENT_TABLE(mainwindow, wxFrame)
	EVT_MENU(STOCK_PURCHASE_DIALOG, OnPurchaseMenu)
	EVT_MENU(wxID_SAVE, OnSave)
	EVT_MENU(QUOTE_LOOKUP, OnQuoteLookup)
	EVT_MENU(_MenuItemIDs::NEW_DEPOSIT, OnAddDeposit)
	EVT_MENU(_MenuItemIDs::_WITHDRAWL, OnWithdrawl)
	EVT_MENU(_MenuItemIDs::NEW_DEPOSIT_SCHEDULE, mainwindow::OnAddDepositSchdule)
	EVT_MENU(_MenuItemIDs::ADD_DIV, OnAddDividend)
	EVT_MENU(_MenuItemIDs::DAY_GAINERS_MENU, OnMarketGainers)
	EVT_MENU(_MenuItemIDs::DAY_LOSERS_MENU, OnMarketLosers)
	EVT_MENU(_MenuItemIDs::_SELL_STOCK, OnSellMenu)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(GridNode, wxStaticText)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(PortfolioWin, wxWindow)
	EVT_TEXT_ENTER(_PortfolioWin::DATEPCKER, DateChange)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(Dialog, wxDialog)
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
auto constexpr _52WEEK_DEVIATION = "52Week Deviation";
auto constexpr _90DAY_DEVIATION = "90Day Deviation";
auto constexpr _30DAY_DEVIATION = "30Day Deviation";
auto constexpr GRIDROW = 1;
auto constexpr GRIDCOL = 2;

void SetColor(wxWindow& w, wxColour c, bool background = true)
{
//	if (IsNull(w, "wxWindow* is null in SetColor!"))
//		return;

//	wxWindow& W = *w;
	if (background)
		w.SetBackgroundColour(c);
	else
		w.SetForegroundColour(c);
}

// helper for Decipherhtmlcodes
char DecipherHelper(char& a, char& b)
{
	switch (a)
	{
	case '0':
	{
		switch (b)
		{
		case 'a': return (char)10;
		case 'b': return (char)11;
		case 'c': return (char)12;
		case 'd': return (char)13;
		case 'e': return (char)14;
		case 'f': return (char)15;
		}
	}
	case '1':
	{
		switch (b)
		{
		case 'a': return (char)26;
		case 'b': return (char)27;
		case 'c': return (char)28;
		case 'd': return (char)29;
		case 'e': return (char)30;
		case 'f': return (char)31;
		}
	}
	case '2':
	{
		switch (b)
		{
		case 'a': return (char)42;
		case 'b': return (char)43;
		case 'c': return (char)44;
		case 'd': return (char)45;
		case 'e': return (char)46;
		case 'f': return (char)47;
		}
	}
	case '3':
	{
		switch (b)
		{
		case 'a': return (char)58;
		case 'b': return (char)59;
		case 'c': return (char)60;
		case 'd': return (char)61;
		case 'e': return (char)62;
		case 'f': return (char)63;
		}
	}
	default: return (char)0;
	}
}

wxString Decipherhtmlcodes(wxString& s)
{
	wxString hex = "#x";
	wxString end = ';';
	wxString amp = "&amp;";

	s.Replace(amp, "&&");
	s.Replace("&#x27;", '\'');
	s.Replace("#x27;", "\'");
	return s;
	int start = s.find(hex);
	if (start == -1)
	{
		start = s.find(amp);
		if (start == -1)
			return s;

		wxString newstring = "";
		newstring = s.Mid(0, start) + "&&" + s.Mid(start + amp.size());
		return newstring;
	}

	hex = s.Mid(start + 2, 2);
	int hexend = s.find(end);
	long l;
	if (!hex.ToLong(&l))
	{
		char a = hex[0];
		char b = hex[1];
		char newchar = DecipherHelper(a, b);
		wxString newstring = "";
		newstring = s.Mid(0, start) + newchar + s.Mid(hexend + 1);
		return newstring;
	}
	int n = l;
	char newchar = ' ';
	if (n <= 19)
		newchar = (char)n + 6;
	else if (n <= 29)
		newchar = (char)n + 12;
	else if (n <= 39)
		newchar = (char)n + 18;
	else if (n <= 49)
		newchar = (char)n + 24;

	wxString newstring = "";
	newstring = s.Mid(0, start) + newchar + s.Mid(hexend + 1);
	return newstring;
}

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

GridNode::GridNode(GridView* gv, wxWindow* w, size_t row, size_t col, GridNode* up, GridNode* right, GridNode* down, GridNode* left, wxString tval, int gridtype)
	: wxStaticText(w, wxID_ANY, tval), m_grid_view(gv), m_parent(w), m_row(row), m_col(col), m_up(up), m_right(right), m_down(down), m_left(left), t_val(tval)
{
	this->SetBackgroundColour(this->normal);
	this->Bind(wxEVT_LEFT_DOWN, &GridNode::OnClickEvent, this);
	this->onclick = wxColour(39, 38, 51);

	if (this->t_val.size())
	{
		this->is_empty = false;
	}

	if (GRIDROW == gridtype)
	{
		this->textcolor = wxColour(182, 203, 242);
		this->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		this->SetForegroundColour(this->textcolor);
	}
	if (GRIDCOL == gridtype)
	{
		this->textcolor = wxColour(213, 220, 227);
		this->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		this->SetForegroundColour(this->textcolor);
	}
	if (gridtype == -1)
		this->SetFont(wxFont(13, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
}

GridNode::~GridNode()
{

}

bool GridNode::SetNewVal(gridpair gp, bool total_row)
{
	if (total_row)
	{
		this->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		this->SetBackgroundColour(this->summaryitem);
		this->SummaryItem = true;
	}
	else
	{
		this->SummaryItem = false;
		if (this->GetBackgroundColour() == this->summaryitem)
			this->SetBackgroundColour(this->normal);
	}

	if (gp.valcolor && this->textcolor != wxColour(182, 203, 242) && this->textcolor != wxColour(213, 220, 227))
	{
		int i = gp.val.find('-');
		if (i != -1)
			this->textcolor = wxColour(217, 7, 28); 
		else
			this->textcolor = wxColour(13, 158, 20); 
		
		this->SetForegroundColour(this->textcolor);
	}
	else if (this->textcolor != wxColour(182, 203, 242) && this->textcolor != wxColour(213, 220, 227))
	{
		this->textcolor = wxColour(255, 255, 255);
		this->SetForegroundColour(this->textcolor);
	}

	this->t_val = gp.val;
	this->SetLabel(gp.val);
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

bool GridNode::IsMatch(wxString s)
{
	if (s == t_val)
		return true;

	return false;
}

void GridNode::OnClickEvent(wxMouseEvent& evt)
{
	if (this->GetBackgroundColour() == this->onclick)
	{
		this->SetBackgroundColour(this->SummaryItem ? this->summaryitem : this->normal);
	}
	else
		this->SetBackgroundColour(this->onclick);

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
	this->SetBackgroundColour(this->SummaryItem ? this->summaryitem : this->normal);

	this->Refresh();
	if (this->m_right)
		this->m_right->OnClickRowOff();
}

void GridNode::OnClickColOff()
{
	this->SetBackgroundColour(this->SummaryItem ? this->summaryitem : this->normal);

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
	if (this->GetBackgroundColour() == this->onclick)
	{
		this->SetBackgroundColour(this->SummaryItem ? this->summaryitem : this->normal);
	}
	else 
		this->SetBackgroundColour(this->onclick);
	this->Refresh();
	GridNode* gn = direction ? this->m_right : m_down;
	if (gn)
		gn->ChangeColorToClicked(direction);
}

void GridNode::UnClick()
{
	this->SetBackgroundColour(this->SummaryItem ? this->summaryitem : this->normal);
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

void GridView::SetNewRow(StockViewerData* svd, bool total_row)
{
	GridNode* first = this->GetFirstEmptyRow();
	if (!first)
		return;

	if (total_row)
		this->summaryrow = first;

	else if (first->GetUP() == this->summaryrow)
		this->SwapSummaryRow(first);

	GridNode* test = first->GetUP();

	this->filledrows++;

	size_t size = this->GetCols();
	for (size_t i = 0; i < size; ++i)
	{
		first->SetNewVal(this->GetLabelText(i, svd), total_row);
		first = first->GetRight();
		if (first)
			continue;
		else
			break;
	}
}

void GridView::UpdateRow(StockViewerData* svd, bool total_row)
{
	size_t size = this->GetCols();
	GridNode* first = this->GetFirstMatch(svd->ticker);
	if (!first)
		return;
	for (size_t i = 0; i < size; ++i)
	{
		first->SetNewVal(this->GetLabelText(i, svd), total_row);
		first = first->GetRight();
		if (first)
			continue;
		else
			break;
	}
}

bool GridView::DoesItemExist(wxString s)
{
	GridNode* first = this->GetFirstMatch(s);
	if (first)
		return true;

	return false;
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

void GridView::LayoutGrid()
{
	this->Layout();
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
			this->head = new GridNode(this, this->m_parent, (size_t)0, i, NULL, NULL, NULL, NULL, val, GRIDCOL);
			this->Add(this->head, 0, wxALIGN_TOP | wxTOP | wxLEFT | wxEXPAND, 4);
		}
		else if (i == 1)
		{
			this->head->SetRight(new GridNode(this, this->m_parent, (size_t)0, i, nullptr, nullptr, nullptr, this->head, val, GRIDCOL));
			this->Add(head->GetRight(), 0, wxALIGN_TOP | wxTOP | wxLEFT | wxEXPAND, 4);
			last = this->head->GetRight();
		}
		else
		{
			last->SetRight(new GridNode(this, this->m_parent, (size_t)0, i, nullptr, nullptr, nullptr, last , val, GRIDCOL));
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
				last_row->SetDown(new GridNode(this, this->m_parent, i + 1, j, last_row, nullptr, nullptr, nullptr, "", GRIDROW));
				this->Add(last_row->GetDown(), 0, wxALIGN_TOP | wxTOP | wxLEFT | wxEXPAND, 4);
				last = current_row = last_row->GetDown();
			}
			else 
			{
				GridNode* lastrowitem = this->GetRight(last_row, j);
				GridNode* currentrowitem = this->GetFarthestRight(current_row);
				if (currentrowitem)
				{
					currentrowitem->SetRight(new GridNode(this, this->m_parent, i + 1, j, lastrowitem, nullptr, nullptr, last, "", -1));
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

gridpair GridView::GetLabelText(size_t& index, StockViewerData* svd)
{
	if (!svd)
	{
		wxFAIL_MSG("StockViewerData* is nullptr in GridView::GetLabelText!");
		return gridpair("", false);
	}

	wxString collabel = this->GetColTitle(index);
	if (collabel == TICKER)
		return gridpair(svd->GetTicker(), false);
	if (collabel == SHARES)
		return gridpair(svd->GetShares(), false);
	if (collabel == PRICE)
		return gridpair(svd->GetPrice(), false);
	if (collabel == PURCHASE_PRICE)
		return gridpair(svd->GetPurchasePrice(), false);
	if (collabel == _PREVIOUS_CLOSE)
		return gridpair(svd->GetPreviousClose(), false);
	if (collabel == DAY_GAIN)
		return gridpair(svd->GetDayGain(), true);
	if (collabel == WEEK_GAIN)
		return gridpair(svd->GetWeekGain(), true);
	if (collabel == QUARTER_GAIN)
		return gridpair(svd->GetQuarterGain(), true);
	if (collabel == YEAR_GAIN)
		return gridpair(svd->GetYearGain(), true);
	if (collabel == TOTAL_GAIN)
		return gridpair(svd->GetTotalGain(), true);
	if (collabel == COST_BASIS)
		return gridpair(svd->GetCostBasis(), false);
	if (collabel == _MARKET_VALUE)
		return gridpair(svd->GetMarketValue(), false);
	if (collabel == PURCHASE_PRICE)
		return gridpair(svd->GetPurchasePrice(), false);
	if (collabel == TOTAL_DIVIDENDS)
		return gridpair(svd->GetDividends(), false);
	if (collabel == EARNINGS_DATE)
		return gridpair(svd->GetEarningsDate(), false);
	if (collabel == PURCHASE_DATE)
		return gridpair(svd->GetPurchaseDate(), false);
	if (collabel == PORTFOLIO_PERC)
		return gridpair(svd->GetPortfolioPerc(), false);
	if (collabel == _52WEEK_DEVIATION)
		return gridpair(svd->Get52Week_Deviation(), false);
	if (collabel == _90DAY_DEVIATION)
		return gridpair(svd->Get90Day_Deviation(), false);
	if (collabel == _30DAY_DEVIATION)
		return gridpair(svd->Get30Day_Deviation(), false);
	return gridpair("", false);
}

wxString GridView::GetColTitle(size_t col)
{
	switch (col)
	{
	case 0: return TICKER;
	case 1: return SHARES;
	case 2: return PRICE;
	case 3: return PURCHASE_PRICE;
	case 4: return _PREVIOUS_CLOSE;
	case 5: return DAY_GAIN;
	case 6: return WEEK_GAIN;
	case 7: return QUARTER_GAIN;
	case 8: return YEAR_GAIN;
	case 9: return TOTAL_GAIN;
	case 10: return COST_BASIS;
	case 11: return _MARKET_VALUE;
	case 12: return PURCHASE_PRICE;
	case 13: return TOTAL_DIVIDENDS;
	case 14: return EARNINGS_DATE;
	case 15: return PURCHASE_DATE;
	case 16: return PORTFOLIO_PERC;
	case 17: return _52WEEK_DEVIATION;
	case 18: return _90DAY_DEVIATION;
	case 19: return _30DAY_DEVIATION;
	case 20: return "";
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

GridNode* GridView::GetFirstMatch(wxString s)
{
	GridNode* next = this->head->GetDown();
	if (!next)
		return next;
	while (1)
	{
		if (next)
		{
			if (next->IsMatch(s))
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

GridNode* GridView::GetSummaryRow()
{
	return this->summaryrow;
}

void GridView::SwapSummaryRow(GridNode*& gn)
{
	if (IsNull(gn, "GridNode passed to GridView::SwapSummaryRow is nullptr!"))
		return;

	this->summaryrow = gn;
	gn = gn->GetUP();
}

PortfolioWin::PortfolioWin(mainwindow* parent, wxWindowID id, wxDateTime* main, StockViewerData* sv, Portfolio* p) 
	: wxWindow(parent, id), mainclock(main), svd(sv), port(p), m_parent(parent), today(wxDateTime::Today())
{
	this->SetBackgroundColour(wxColour(100, 100, 100));
	this->Create();
}

void PortfolioWin::Update()
{
	StockViewerData* svd = port->GetStockViewerData();
	this->CashDisplay->SetLabel("$" + wxNumberFormatter::ToString(this->port->GetFreeCash(), 2));
	this->DayReturnDisplay->SetLabel(svd->GetDayGain() + " " + "($" + svd->GetDayReturn$() + ")");
	this->AccountValueDisplay->SetLabel("$" + wxNumberFormatter::ToString(svd->market_value + port->GetFreeCash(), 2));
	this->TotalReturnDisplay->SetLabel(svd->GetTotalGain() + " " + "($" + svd->GetTotalReturn$() + ")");

	if (this->DayReturnDisplay->GetLabelText().find('-') == -1)
		this->DayReturnDisplay->SetForegroundColour(this->Green);
	else
		this->DayReturnDisplay->SetForegroundColour(this->Red);
	if (this->TotalReturnDisplay->GetLabelText().find('-') == -1)
		this->TotalReturnDisplay->SetForegroundColour(this->Green);
	else
		this->TotalReturnDisplay->SetForegroundColour(this->Red);

	this->Refresh();
}

void PortfolioWin::Create()
{
	// initialize controls...
	this->panel = new wxPanel(this, wxID_ANY);
	this->panel->SetBackgroundColour(this->GetBackgroundColour());
	this->updatebutton = new wxButton(this->panel, wxID_ANY, "Update");
	this->updatebutton->Bind(wxEVT_BUTTON, &PortfolioWin::OnUpdateButtonPress, this);
	this->datepcker = new wxTextCtrl(panel, _PortfolioWin::DATEPCKER, this->today.Format(STANDARD_DATE), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_PROCESS_ENTER);

	this->todaysdate = new wxStaticText(panel, wxID_ANY, "Today's Date");
	this->todaysdateDisplay = new wxStaticText(panel, wxID_ANY, this->today.Format(STANDARD_DATE));
	this->Cash = new wxStaticText(panel, wxID_ANY, "Cash");
	this->CashDisplay = new wxStaticText(panel, wxID_ANY, "$" + wxNumberFormatter::ToString(this->port->GetFreeCash(), 2));
	this->DayReturn = new wxStaticText(panel, wxID_ANY, "Day Return");
	this->DayReturnDisplay = new wxStaticText(panel, wxID_ANY, svd->GetDayGain());
	this->AccountValue = new wxStaticText(panel, wxID_ANY, "Account Value");
	this->AccountValueDisplay = new wxStaticText(panel, wxID_ANY, "$" + svd->GetTotalValue());
	this->TotalReturn = new wxStaticText(panel, wxID_ANY, "Total Return");
	this->TotalReturnDisplay = new wxStaticText(panel, wxID_ANY, svd->GetTotalGain());
	this->SetControlFonts();

	// Initialize sizers...
	wxBoxSizer* main = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* panelsizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* panelsizerH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* datepckerH = new wxBoxSizer(wxHORIZONTAL);
	datepckerH->Add(this->datepcker, 1, wxTOP, 10);
	datepckerH->Add(this->updatebutton, 0, wxLEFT | wxTOP, 10);
	main->Add(datepckerH, 0, wxALIGN_CENTER_HORIZONTAL);
	main->Add(this->todaysdate, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	main->AddSpacer(5);
	main->Add(this->todaysdateDisplay, 1, wxALIGN_LEFT | wxLEFT, 10);
	main->Add(this->Cash, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	main->AddSpacer(5);
	main->Add(this->CashDisplay, 1, wxALIGN_LEFT | wxLEFT, 10);
	main->Add(this->DayReturn, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	main->AddSpacer(5);
	main->Add(this->DayReturnDisplay, 1, wxALIGN_LEFT | wxLEFT, 10);
	main->Add(this->AccountValue, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	main->AddSpacer(5);
	main->Add(this->AccountValueDisplay, 1, wxALIGN_LEFT | wxLEFT, 10);
	main->Add(this->TotalReturn, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	main->AddSpacer(5);
	main->Add(this->TotalReturnDisplay, 1, wxALIGN_LEFT | wxLEFT, 10);

	panel->SetSizer(main);
	panelsizer->Add(panel, 1, wxEXPAND);
	panelsizerH->Add(panelsizer, 1, wxEXPAND);
	this->SetSizer(panelsizerH);
	panelsizerH->Fit(this);
	panelsizerH->SetSizeHints(this);
	this->Layout();
//	this->Show(true);
}

void PortfolioWin::SetControlFonts()
{
	this->datepcker->SetFont(wxFont(13, wxFontFamily::wxFONTFAMILY_MODERN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->updatebutton->SetFont(datepcker->GetFont());
	this->todaysdate->SetFont(wxFont(17, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->todaysdateDisplay->SetFont(wxFont(19, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->Cash->SetFont(wxFont(17, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->CashDisplay->SetFont(wxFont(19, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->DayReturn->SetFont(wxFont(17, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->DayReturnDisplay->SetFont(wxFont(19, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->AccountValue->SetFont(wxFont(17, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->AccountValueDisplay->SetFont(wxFont(19, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->TotalReturn->SetFont(wxFont(17, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->TotalReturnDisplay->SetFont(wxFont(19, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	
	this->todaysdate->SetForegroundColour(this->Blue);
	this->Cash->SetForegroundColour(this->Blue);
	this->DayReturn->SetForegroundColour(this->Blue);
	this->AccountValue->SetForegroundColour(this->Blue);
	this->TotalReturn->SetForegroundColour(this->Blue);

	this->CashDisplay->SetForegroundColour(this->Green);
	this->AccountValueDisplay->SetForegroundColour(this->Green);

	if (this->DayReturnDisplay->GetLabelText().find('-') == -1)
		this->DayReturnDisplay->SetForegroundColour(this->Green);
	else
		this->DayReturnDisplay->SetForegroundColour(this->Red);
	if (this->TotalReturnDisplay->GetLabelText().find('-') == -1)
		this->TotalReturnDisplay->SetForegroundColour(this->Green);
	else
		this->TotalReturnDisplay->SetForegroundColour(this->Red);
}

void PortfolioWin::DateChange(wxCommandEvent& evt)
{
	if (!this->datepcker)
		return;

	wxDateTime test;
	wxString date = this->datepcker->GetValue();
	if (!test.ParseDate(this->datepcker->GetValue()))
	{
		this->datepcker->SetLabelText(wxDateTime::Today().Format(STANDARD_DATE));
		this->datepcker->Refresh();
		return;
	}

	if (test >= wxDateTime::Today())
	{
		this->datepcker->SetLabelText(wxDateTime::Today().Format(STANDARD_DATE));
		this->datepcker->Refresh();
		return;
	}

	if (!this->mainclock)
	{
		wxFAIL_MSG("mainclock in PortfolioWin::DateChange is nullptr!");
		return;
	}
	
	*this->mainclock = test;

	this->m_parent->OnDateChange();
}

void PortfolioWin::OnUpdateButtonPress(wxCommandEvent& evt)
{
	this->m_parent->OnUpdate();
}


template <typename T>
Dialog::Dialog(_EnterDialog type, mainwindow* parent, wxWindowID id, wxPoint p, wxSize s, wxString msg, T* t) : wxDialog(parent, id, msg, p, s), m_parent(parent), m_type(type)
{
	this->Bind(wxEVT_CLOSE_WINDOW, &Dialog::OnCloseDialog, this);
	this->Bind(wxEVT_BUTTON, &Dialog::OnOkClick, this);

	switch (type)
	{
	case STOCK_PURCHASE_DIALOG: this->CreateStockEntry(); break;
	case _EnterDialog::SELL_STOCK: this->sellkit = reinterpret_cast<GenericKit*>(t); this->CreateSellStockWin(); break;
	case _EnterDialog::QUOTE_WIN: sumptr = reinterpret_cast<SummaryData*>(t); this->CreateQuoteWin(); break;
	case _EnterDialog::ENTER_DEPOSIT: this->CreateDepositWin(); break;
	case _EnterDialog::ENTER_DEPOSIT_SCHEDULE: this->CreateDepositScheduleWin(); break;
	case _EnterDialog::WITHDRAWL: this->CreateWithdrawlWin(); break;
	case _EnterDialog::ENTER_DIVIDEND: this->CreateAddDividendWin(); break;
	case _EnterDialog::DAY_GAINERS_WIN: this->gainers = reinterpret_cast<wxVector<DayGainersandLosers>*>(t); this->CreateDayGainers_LosersWin(true); break;
	case _EnterDialog::DAY_LOSERS_WIN: this->gainers = reinterpret_cast<wxVector<DayGainersandLosers>*>(t); this->CreateDayGainers_LosersWin(false); break;
	default: wxFAIL_MSG("_EnterDialog type does not match any types!"); this->Destroy();
	}
}
/*
Dialog::Dialog(_EnterDialog type, mainwindow* parent, wxWindowID id, wxPoint p, wxSize s, wxString msg, SummaryData sum) 
	: wxDialog(parent, id, msg, p, s), m_parent(parent), m_type(type), sd(sum)
{

	this->Bind(wxEVT_CLOSE_WINDOW, &Dialog::OnCloseDialog, this);
	this->Bind(wxEVT_BUTTON, &Dialog::OnOkClick, this);

	switch (type)
	{
	case QUOTE_WIN: this->CreateQuoteWin(); break;
	default: wxFAIL_MSG("_EnterDialog type does not match any types!"); this->Destroy();
	}
}
*/
PurchaseKit Dialog::GetPurchaseKit()
{
	wxString selection = this->choice->GetStringSelection();
	_Sector sec = GetSectorClass().GetSectorID(selection);
	_Sub_Sector sub = GetSectorClass().GetSubSectorID(sec, this->subSector->GetStringSelection());
	PurchaseKit kit(this->_ticker, sec, sub, this->_price, this->_shares, this->_date, this->_reinvest_date, this->check->IsChecked());

	return kit;
}

GenericKit Dialog::GetGenericKit()
{
	return GenericKit(this->datetime, this->_price);
}

SellKit Dialog::GetSellKit()
{
	return SellKit(this->datetime, this->_price, this->_shares);
}

void Dialog::CreateStockEntry()
{
	wxColour c(205, 209, 206);
	SetColor(*this, wxColour(157, 157, 163));
//	this->SetBackgroundColour(wxColour(157, 157, 163));
	// init controls...
	this->s_ticker = new wxStaticText(this, wxID_ANY, "Enter Ticker");
	this->m_ticker = new wxTextCtrl(this, wxID_ANY);
	this->m_ticker->SetValidator(wxTextValidator(wxFILTER_ALPHA | wxFILTER_EMPTY, &this->_ticker));
	SetColor(*this->m_ticker, c);

	this->CreateChoiceControls();

	this->sector_choice = new wxStaticText(this, wxID_ANY, "Choose Sector");
	wxStaticText* sub_sector = new wxStaticText(this, wxID_ANY, "Choose Industry");
	this->s_price = new wxStaticText(this, wxID_ANY, "Enter Price");
	this->m_price = new wxTextCtrl(this, wxID_ANY);
	this->m_price->SetValidator(wxFloatingPointValidator<double>(&this->_price, wxNumValidatorStyle::wxNUM_VAL_ZERO_AS_BLANK | wxFILTER_EMPTY));
	SetColor(*this->m_price, c);

	this->s_shares = new wxStaticText(this, wxID_ANY, "Enter Shares");
	this->m_shares = new wxTextCtrl(this, wxID_ANY);
	this->m_shares->SetValidator(wxFloatingPointValidator<double>(&this->_shares, wxNumValidatorStyle::wxNUM_VAL_ZERO_AS_BLANK | wxFILTER_EMPTY));
	SetColor(*this->m_shares, c);

	this->s_date = new wxStaticText(this, wxID_ANY, "Enter Date");
	this->m_date = new wxTextCtrl(this, wxID_ANY);
	this->m_date->SetValidator(wxTextValidator(wxFILTER_EMPTY, &this->_date));
	SetColor(*this->m_date, c);
	
	this->s_reinvest_date = new wxStaticText(this, wxID_ANY, "Dividend Reinvest Start Date");
	this->m_reinvest_date = new wxTextCtrl(this, wxID_ANY);
	this->m_reinvest_date->SetValidator(wxTextValidator(wxFILTER_NONE, &this->_reinvest_date));
	SetColor(*this->m_reinvest_date, c);

	this->check = new wxCheckBox(this, wxID_ANY, "Re-invest");
	this->check->Bind(wxEVT_CHECKBOX, &Dialog::OnCheckClick, this);

	wxButton* ok = new wxButton(this, wxID_OK, "OK");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "CANCEL");

	ok->Bind(wxEVT_BUTTON, &Dialog::OnOkClick, this);
	cancel->Bind(wxEVT_BUTTON, &Dialog::OnCancelDialog, this);
	
	// create sizers...
	wxBoxSizer* mainV = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* topH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bottomH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* tick = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sect = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* subsect = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* price = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* share = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* date = new wxBoxSizer(wxVERTICAL);

	tick->Add(this->s_ticker, 0, wxALIGN_LEFT);
	tick->Add(this->m_ticker, 0, wxALIGN_LEFT);

	sect->Add(this->sector_choice, 0, wxALIGN_LEFT);
	sect->Add(this->choice, 0, wxALIGN_LEFT);

	subsect->Add(sub_sector, 0, wxALIGN_LEFT);
	subsect->Add(this->subSector, 0, wxALIGN_LEFT);

	price->Add(this->s_price, 0, wxALIGN_LEFT);
	price->Add(this->m_price, 0, wxALIGN_LEFT);

	share->Add(this->s_shares, 0, wxALIGN_LEFT);
	share->Add(this->m_shares, 0, wxALIGN_LEFT);

	date->Add(this->s_date, 0, wxALIGN_LEFT);
	date->Add(this->m_date, 0, wxALIGN_LEFT);

	topH->Add(tick, 1, wxALIGN_CENTER_VERTICAL);
	topH->Add(sect, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	topH->Add(subsect, 0, wxALIGN_CENTER_VERTICAL);
	topH->Add(price, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	topH->Add(share, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	topH->Add(date, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

	//static box stuff...
	wxBoxSizer* reivest_date = new wxBoxSizer(wxVERTICAL);
	reivest_date->Add(s_reinvest_date, 0, wxALIGN_RIGHT | wxLEFT, 10);
	reivest_date->Add(m_reinvest_date, 0, wxALIGN_RIGHT | wxLEFT, 10);
	wxStaticBoxSizer* _static = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, "Dividend Re-investment"), wxHORIZONTAL);
	_static->Add(check, 1, wxALL, 10);
	_static->Add(reivest_date, 1);

	// add buttons to bottom sizer...
	bottomH->Add(ok, 0, wxRIGHT, 10);
	bottomH->Add(cancel, 0);

	mainV->Add(topH, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	mainV->Add(_static, 1, wxALIGN_CENTER_HORIZONTAL);
	mainV->Add(bottomH, 0, wxALIGN_RIGHT | wxALL, 10);

	this->SetSizer(mainV);
	mainV->Fit(this);
	mainV->SetSizeHints(this);
	this->Layout();
	this->CenterOnParent();

//	this->ShowModal();
}

void Dialog::CreateQuoteWin()
{
	SetColor(*this, wxColour(207, 208, 212));

	wxBoxSizer* mainV = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* topH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* staticH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* staticH2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bottomH = new wxBoxSizer(wxHORIZONTAL);

	wxString dayreturn = wxNumberFormatter::ToString(sumptr->previousclose ? (sumptr->marketprice / sumptr->previousclose - 1) * 100 : 0.0, 2) + "%";
	wxString dayreturn$ = "($" + wxNumberFormatter::ToString(sumptr->marketprice - sumptr->previousclose, 2) + ")";
	wxString earningsdate = "Next Earnings: ";
	earningsdate += sumptr->earningsdate == "NotFound" ? "" : sumptr->earningsdate;

	wxStaticText* longname = new wxStaticText(this, wxID_ANY, Decipherhtmlcodes(sumptr->Longname));
	longname->SetForegroundColour(wxColour(74, 99, 224));

	wxStaticText* _dayreturn = new wxStaticText(this, wxID_ANY, dayreturn);
	wxStaticText* _dayreturn$ = new wxStaticText(this, wxID_ANY, dayreturn$);
	wxStaticText* previousclose = new wxStaticText(this, wxID_ANY, "Previous Close: " + sumptr->prev_close);
	wxStaticText* open = new wxStaticText(this, wxID_ANY, "Open: " + sumptr->open);
	wxStaticLine* stat_line = new wxStaticLine(this, wxID_ANY);
	wxStaticText* curr_price = new wxStaticText(this, wxID_ANY, sumptr->curr_price);
	wxStaticText* _earningsdate = new wxStaticText(this, wxID_ANY, earningsdate);
	wxStaticText* beta = new wxStaticText(this, wxID_ANY, "Beta: " + sumptr->beta);

	longname->SetFont(wxFont(20, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	curr_price->SetFont(wxFont(46, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	_dayreturn->SetFont(wxFont(20, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	_dayreturn$->SetFont(wxFont(20, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	open->SetFont(wxFont(20, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	previousclose->SetFont(wxFont(20, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	_earningsdate->SetFont(wxFont(20, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	beta->SetFont(wxFont(20, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	if (dayreturn.find('-') != -1)
	{
		_dayreturn->SetForegroundColour(wxColour(201, 15, 8));
		_dayreturn$->SetForegroundColour(wxColour(201, 15, 8));
	}
	else
	{
		_dayreturn->SetForegroundColour(wxColour(8, 156, 23));
		_dayreturn$->SetForegroundColour(wxColour(8, 156, 23));
	}

	wxStaticLine* stat1 = new wxStaticLine(this, wxID_ANY);
	wxStaticLine* stat2 = new wxStaticLine(this, wxID_ANY);
	stat1->SetForegroundColour("Black");
	stat2->SetForegroundColour("Black");
	

	staticH->Add(new wxStaticLine(this, wxID_ANY), 1, wxEXPAND);
	staticH2->Add(new wxStaticLine(this, wxID_ANY), 1, wxEXPAND);

	topH->Add(curr_price, 0, wxALIGN_BOTTOM);
	topH->Add(_dayreturn, 0, wxLEFT | wxALIGN_BOTTOM, 5);
	topH->Add(_dayreturn$, 0, wxLEFT | wxALIGN_BOTTOM, 5);
	topH->AddSpacer(10);
	topH->Add(_earningsdate, 0, wxLEFT | wxRIGHT | wxALIGN_BOTTOM, 20);

	bottomH->Add(open, 1, wxALIGN_LEFT | wxLEFT, 10);
	bottomH->Add(previousclose, 1, wxALIGN_LEFT | wxLEFT, 15);

	mainV->Add(longname, 0, wxBOTTOM | wxLEFT |wxTOP | wxALIGN_LEFT, 5);
	mainV->Add(staticH, 0, wxEXPAND | wxALL, 5);
	mainV->Add(topH, 1, wxALIGN_LEFT | wxLEFT, 5);
	mainV->Add(staticH2, 0, wxEXPAND | wxALL, 5);
	mainV->Add(bottomH, 0, wxBOTTOM, 10);
	mainV->Add(beta, 0, wxBOTTOM | wxLEFT | wxALIGN_LEFT, 10);
	mainV->Add(this->CreateSeparatedSizer(this->CreateTextSizer(Decipherhtmlcodes(sumptr->description), 800)), 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	this->SetSizer(mainV);
	mainV->Fit(this);
	mainV->SetSizeHints(this);
	this->Layout();
	this->CenterOnParent();

	this->ShowModal();
}

void Dialog::CreateDayGainers_LosersWin(bool gainers)
{
	SetColor(*this, wxColour(207, 208, 212));
	wxColour foreground = gainers ? wxColour(5, 166, 10) : wxColour(201, 4, 27);

	wxGridSizer* grid = new wxGridSizer(5, 5, 5);

	wxStaticText* _ticker = new wxStaticText(this, wxID_ANY, "Ticker");
	wxStaticText* _marketprice = new wxStaticText(this, wxID_ANY, "Price");
	wxStaticText* _previousclose = new wxStaticText(this, wxID_ANY, "Previous Close");
	wxStaticText* _perchange = new wxStaticText(this, wxID_ANY, "Change%");
	wxStaticText* _change = new wxStaticText(this, wxID_ANY, "Change");

	_ticker->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	_ticker->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	_marketprice->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	_previousclose->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	_perchange->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	_change->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));

	grid->Add(_ticker, 1, wxALIGN_LEFT | wxLEFT, 5);
	grid->Add(_marketprice, 1, wxALIGN_LEFT);
	grid->Add(_previousclose, 1, wxALIGN_LEFT);
	grid->Add(_perchange, 1, wxALIGN_LEFT | wxLEFT, 10);
	grid->Add(_change, 1, wxALIGN_LEFT);

	for (auto it = this->gainers->begin(); it != this->gainers->end(); ++it)
	{
		wxStaticText* ticker = new wxStaticText(this, wxID_ANY, it->ticker);
		ticker->Bind(wxEVT_LEFT_DOWN, &Dialog::OnMouseDown, this);
		ticker->SetForegroundColour(wxColour(17, 121, 212));
	
		wxStaticText* marketprice = new wxStaticText(this, wxID_ANY, wxNumberFormatter::ToString(it->_marketprice, 2));
		wxStaticText* previousclose = new wxStaticText(this, wxID_ANY, wxNumberFormatter::ToString(it->_previouclose, 2));
		wxStaticText* perchange = new wxStaticText(this, wxID_ANY, wxNumberFormatter::ToString(it->_gain_loss * 100, 2) + '%');
		wxStaticText* change = new wxStaticText(this, wxID_ANY, '(' + wxNumberFormatter::ToString(it->_change, 2) + ')');

		ticker->SetFont(wxFont(12, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
		marketprice->SetFont(wxFont(12, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_SEMIBOLD));
		previousclose->SetFont(wxFont(12, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_SEMIBOLD));
		perchange->SetFont(wxFont(12, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_SEMIBOLD));
		change->SetFont(wxFont(12, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_SEMIBOLD));

		perchange->SetForegroundColour(foreground);
		change->SetForegroundColour(foreground);

		grid->Add(ticker, 1, wxALIGN_LEFT | wxLEFT, 5);
		grid->Add(marketprice, 1, wxALIGN_LEFT);
		grid->Add(previousclose, 1, wxALIGN_LEFT);
		grid->Add(perchange, 1, wxALIGN_LEFT | wxLEFT, 10);
		grid->Add(change, 1, wxALIGN_LEFT);
	}

	this->SetSizer(grid);
	grid->Fit(this);
	grid->SetSizeHints(this);
	this->Layout();
	this->CenterOnParent();

	this->ShowModal();
}

void Dialog::OnCheckClick(wxCommandEvent& evt)
{
	if (this->check->IsChecked())
	{
		this->m_reinvest_date->Enable(true);
		this->_reinvest_date = this->m_date->GetValue();
		this->m_reinvest_date->SetValue(this->_reinvest_date);
	}
	else
	{
		this->m_reinvest_date->Enable(false);
		this->m_reinvest_date->SetLabel("");
	}
}

void Dialog::CreateDepositWin()
{
	wxColour c(205, 209, 206);
	SetColor(*this, wxColour(157, 157, 163));

	wxBoxSizer* mainV = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* topH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bottomH = new wxBoxSizer(wxHORIZONTAL);

	this->s_price = new wxStaticText(this, wxID_ANY, "Deposit Amount:");
	this->m_price = new wxTextCtrl(this, wxID_ANY);
	this->m_price->SetValidator(wxFloatingPointValidator<double>(&this->_price, wxNumValidatorStyle::wxNUM_VAL_ZERO_AS_BLANK | wxFILTER_EMPTY));
	SetColor(*this->m_price, c);

	this->s_date = new wxStaticText(this, wxID_ANY, "Enter Date:");
	this->m_date = new wxTextCtrl(this, wxID_ANY);
	this->m_date->SetValidator(wxTextValidator(wxFILTER_EMPTY, &this->_date));
	SetColor(*this->m_date, c);

	wxButton* ok = new wxButton(this, wxID_OK, "OK");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "CANCEL");

	ok->Bind(wxEVT_BUTTON, &Dialog::OnOkClick, this);
	cancel->Bind(wxEVT_BUTTON, &Dialog::OnCancelDialog, this);
	
	topH->Add(s_date, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	topH->Add(m_date, 0, wxALIGN_CENTER_VERTICAL);
	topH->AddSpacer(30);
	topH->Add(s_price, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	topH->Add(m_price, 0, wxALIGN_CENTER_VERTICAL);

	bottomH->Add(ok, 0, wxRIGHT, 10);
	bottomH->Add(cancel, 0);

	mainV->Add(topH, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	mainV->Add(bottomH, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

	this->SetSizer(mainV);
	mainV->Fit(this);
	mainV->SetSizeHints(this);
	this->Layout();
	this->CenterOnParent();

//	this->ShowModal();
}

void Dialog::CreateWithdrawlWin()
{
	wxColour c(205, 209, 206);
	SetColor(*this, wxColour(157, 157, 163));

	wxBoxSizer* mainV = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* topH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bottomH = new wxBoxSizer(wxHORIZONTAL);

	this->s_price = new wxStaticText(this, wxID_ANY, "Withdrawl Amount:");
	this->m_price = new wxTextCtrl(this, wxID_ANY);
	this->m_price->SetValidator(wxFloatingPointValidator<double>(&this->_price, wxNumValidatorStyle::wxNUM_VAL_ZERO_AS_BLANK | wxFILTER_EMPTY));
	SetColor(*this->m_price, c);

	this->s_date = new wxStaticText(this, wxID_ANY, "Enter Date:");
	this->m_date = new wxTextCtrl(this, wxID_ANY);
	this->m_date->SetValidator(wxTextValidator(wxFILTER_EMPTY, &this->_date));
	SetColor(*this->m_date, c);

	wxButton* ok = new wxButton(this, wxID_OK, "OK");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "CANCEL");

	ok->Bind(wxEVT_BUTTON, &Dialog::OnOkClick, this);
	cancel->Bind(wxEVT_BUTTON, &Dialog::OnCancelDialog, this);

	topH->Add(s_date, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	topH->Add(m_date, 0, wxALIGN_CENTER_VERTICAL);
	topH->AddSpacer(30);
	topH->Add(s_price, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	topH->Add(m_price, 0, wxALIGN_CENTER_VERTICAL);

	bottomH->Add(ok, 0, wxRIGHT, 10);
	bottomH->Add(cancel, 0);

	mainV->Add(topH, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	mainV->Add(bottomH, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

	this->SetSizer(mainV);
	mainV->Fit(this);
	mainV->SetSizeHints(this);
	this->Layout();
	this->CenterOnParent();

//	this->ShowModal();
}

void Dialog::CreateDepositScheduleWin()
{
	wxBoxSizer* mainV = new wxBoxSizer(wxVERTICAL);

	this->SetSizer(mainV);
	mainV->Fit(this);
	mainV->SetSizeHints(this);
	this->Layout();
	this->CenterOnParent();

	this->ShowModal();
}

void Dialog::CreateSellStockWin()
{
	wxColour c(205, 209, 206);
	SetColor(*this, wxColour(157, 157, 163));

	wxBoxSizer* mainV = new wxBoxSizer(wxVERTICAL);

	this->s_price = new wxStaticText(this, wxID_ANY, "Enter Price");
	this->m_price = new wxTextCtrl(this, wxID_ANY);
	this->m_price->SetValidator(wxFloatingPointValidator<double>(&this->_price, wxNumValidatorStyle::wxNUM_VAL_ZERO_AS_BLANK | wxFILTER_EMPTY));
	SetColor(*this->m_price, c);

	this->s_shares = new wxStaticText(this, wxID_ANY, "Enter Shares");
	this->m_shares = new wxTextCtrl(this, wxID_ANY, wxNumberFormatter::ToString(this->sellkit->amount, 5));
	this->m_shares->SetValidator(wxFloatingPointValidator<double>(&this->_shares, wxNumValidatorStyle::wxNUM_VAL_ZERO_AS_BLANK | wxFILTER_EMPTY));
	SetColor(*this->m_shares, c);

	this->s_date = new wxStaticText(this, wxID_ANY, "Enter Date");
	this->m_date = new wxTextCtrl(this, wxID_ANY);
	this->m_date->SetValidator(wxTextValidator(wxFILTER_EMPTY, &this->_date));
	SetColor(*this->m_date, c);

	wxButton* ok = new wxButton(this, wxID_OK, "OK");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "CANCEL");

	ok->Bind(wxEVT_BUTTON, &Dialog::OnOkClick, this);
	cancel->Bind(wxEVT_BUTTON, &Dialog::OnCancelDialog, this);

	wxBoxSizer* price = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* share = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* date = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* topH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bottomH = new wxBoxSizer(wxHORIZONTAL);

	price->Add(this->s_price, 0, wxALIGN_LEFT);
	price->Add(this->m_price, 0, wxALIGN_LEFT);

	share->Add(this->s_shares, 0, wxALIGN_LEFT);
	share->Add(this->m_shares, 0, wxALIGN_LEFT);

	date->Add(this->s_date, 0, wxALIGN_LEFT);
	date->Add(this->m_date, 0, wxALIGN_LEFT);

	topH->Add(price, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	topH->Add(share, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	topH->Add(date, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

	bottomH->Add(ok, 0, wxRIGHT, 10);
	bottomH->Add(cancel, 0);

	mainV->Add(topH, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	mainV->Add(bottomH, 0, wxALIGN_RIGHT | wxALL, 10);

	this->SetSizer(mainV);
	mainV->Fit(this);
	mainV->SetSizeHints(this);
	this->Layout();
	this->CenterOnParent();

//	this->ShowModal();
}

void Dialog::CreateAddDividendWin()
{
	wxColour c(205, 209, 206);
	SetColor(*this, wxColour(157, 157, 163));

	wxBoxSizer* mainV = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* topH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bottomH = new wxBoxSizer(wxHORIZONTAL);

	this->s_price = new wxStaticText(this, wxID_ANY, "Dividend Amount:");
	this->m_price = new wxTextCtrl(this, wxID_ANY);
	this->m_price->SetValidator(wxFloatingPointValidator<double>(&this->_price, wxNumValidatorStyle::wxNUM_VAL_ZERO_AS_BLANK | wxFILTER_EMPTY));
	SetColor(*this->m_price, c);

	this->s_date = new wxStaticText(this, wxID_ANY, "Enter Date:");
	this->m_date = new wxTextCtrl(this, wxID_ANY);
	this->m_date->SetValidator(wxTextValidator(wxFILTER_EMPTY, &this->_date));
	SetColor(*this->m_date, c);

	wxButton* ok = new wxButton(this, wxID_OK, "OK");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "CANCEL");

	ok->Bind(wxEVT_BUTTON, &Dialog::OnOkClick, this);
	cancel->Bind(wxEVT_BUTTON, &Dialog::OnCancelDialog, this);

	topH->Add(s_date, wxALIGN_CENTER_VERTICAL);
	topH->Add(m_date, wxALIGN_CENTER_VERTICAL);
	topH->AddSpacer(20);
	topH->Add(s_price, wxALIGN_CENTER_VERTICAL);
	topH->Add(m_price, wxALIGN_CENTER_VERTICAL);

	bottomH->Add(ok, 0, wxRIGHT, 10);
	bottomH->Add(cancel, 0);

	mainV->Add(topH, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	mainV->Add(bottomH, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

	this->SetSizer(mainV);
	mainV->Fit(this);
	mainV->SetSizeHints(this);
	this->Layout();
	this->CenterOnParent();

//	this->ShowModal();
}

void Dialog::CreateChoiceControls()
{
	// Create choice controls...
	this->choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	this->subSector = new wxChoice(this, wxID_ANY);

	// Bind choice to event OnSectorChoice...
	this->choice->Bind(wxEVT_CHOICE, &Dialog::OnSectorChoice, this);

	const wxVector<_Sector>* scptr = GetSectorClass().GetSector();
	const wxVector<wxString>* scptrs = GetSectorClass().GetSectorString();

	for (size_t i = 0; i < scptrs->size() - 1; ++i)
		this->choice->Append(scptrs->at(i));

	this->choice->SetStringSelection(scptrs->at(8 + 7));

	const wxVector<wxString>* subvec = GetSectorClass().GetSubSectorString(scptr->at(8 + 7));
	for (size_t i = 0; i < subvec->size(); ++i)
		this->subSector->Append(subvec->at(i));

	this->subSector->SetStringSelection(subvec->at(0));
}

void Dialog::OnCancelDialog(wxCommandEvent& evt)
{
	this->Destroy();
	m_parent->DialogCancel();
}

void Dialog::OnCloseDialog(wxCloseEvent& evt)
{
	this->Destroy();
	m_parent->DialogCancel();
}

void Dialog::OnOkClick(wxCommandEvent& evt)
{
	if (!this->Validate())
		return;

	this->TransferDataFromWindow();

	switch (this->m_type)
	{
	case _EnterDialog::STOCK_PURCHASE_DIALOG: 
	{
		if (this->HandlePurchaseOkClick())
			evt.Skip();
		return;
	}
	case _EnterDialog::ENTER_DEPOSIT: 
	{		
		if (!this->HandleDepositOkClick())
			return;
		evt.Skip();
		break;
	}
	case _EnterDialog::ENTER_DEPOSIT_SCHEDULE: this->HandleDepositScheduleOkClick(); break;
	case _EnterDialog::SELL_STOCK: 
	{
		if (!this->HandleSellStockOkClick())
			return;
		evt.Skip();
		break;
	}
	case _EnterDialog::ENTER_DIVIDEND: this->HandleAddDividendOkClick(); break;
	case _EnterDialog::WITHDRAWL: 
	{
		if (this->HandleWithdrawlOkClick())
			evt.Skip();
		return;
	}
	}
}

void Dialog::OnSectorChoice(wxCommandEvent& evt)
{
	wxString sector = this->choice->GetStringSelection();
	int index = this->choice->GetSelection();
	int type = GetSectorClass().GetSectorID(sector);
	if (type <= FOREIGN)
	{
		this->subSector->Clear();
		this->subSector->Append(_ETF);
		this->subSector->SetStringSelection(_ETF);
		return;
	}

	const wxVector<wxString>* sub = GetSectorClass().GetSubSectorString(_Sector(this->choice->GetSelection() + *GetSectorClass().GetSector()->begin() + 8));
	this->subSector->Clear();

	if (!sub->size())
		return;

	for (size_t i = 0; i < sub->size(); ++i)
		this->subSector->Append(sub->at(i));

	this->subSector->SetStringSelection(*sub->begin());
}

void Dialog::OnMouseDown(wxMouseEvent& evt)
{
	wxStaticText* T = (wxStaticText*)this->FindItem(evt.GetId());
	if (!T)
		return;
	wxString tick = T->GetLabelText();
	Parser parser(tick);
	SummaryData sd = parser.GetSummaryData();
	sd.description = parser.GetDescription(tick);
	this->Disable();
	Dialog* D = new Dialog(_EnterDialog::QUOTE_WIN, this->m_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, tick, &sd);
	this->Enable();
}

bool Dialog::HandlePurchaseOkClick()
{
	wxDateTime purch;
	wxDateTime re;
	if (!purch.ParseDate(this->_date))
	{
		wxMessageDialog d(this, "Date is invalid.");
		d.ShowModal();
		return false;
	}

	if (purch > wxDateTime::Today())
	{
		wxMessageDialog d(this, "Purchase date is greater than today's date");
		d.ShowModal();
		return false;
	}

	if (this->check->IsChecked())
	{
		if (!re.ParseDate(this->_reinvest_date))
		{
			wxMessageDialog d(this, "Re-investment is date invalid.");
			d.ShowModal();
			return false;
		}

		if (re < purch)
		{
			wxMessageDialog d(this, "Re-investment date is earlier than purchase date.");
			d.ShowModal();
			return false;
		}
	}

	return true;
	this->Destroy();
//	wxString selection = this->choice->GetStringSelection();
//	PurchaseKit kit(this->_ticker, GetStringToSector(this->choice->GetStringSelection()), 
//		this->_price, this->_shares, this->_date, this->_reinvest_date, this->check->IsChecked());

//	this->m_parent->PurchaseDataTransfer(kit);
}

bool Dialog::HandleDepositOkClick()
{
	if (!this->datetime.ParseDate(this->m_date->GetValue()))
		return false;
	else if (wxDateTime::Today() < this->datetime)
		return false;

	return true;
}

void Dialog::HandleDepositScheduleOkClick()
{
	
}

bool Dialog::HandleWithdrawlOkClick()
{
	if (!this->datetime.ParseDate(this->m_date->GetValue()))
		return false;
	else if (wxDateTime::Today() < this->datetime)
		return false;

	return true;
}

bool Dialog::HandleSellStockOkClick()
{
	if (this->_shares > this->sellkit->amount)
	{
		wxMessageBox("You can not sell more shares than you own in " + this->GetTitle());
		return false;
	}

	if (!datetime.ParseDate(this->_date))
	{
		wxMessageBox(this->_date + " is not a valid date");
		return false;
	}
	if (this->sellkit->date > datetime)
	{
		wxMessageBox("The sold date you enterd is earlier than your purchase date of " + this->GetTitle());
		return false;
	}

	this->Destroy();
	return true;
}

void Dialog::HandleAddDividendOkClick()
{

}

// SellStockWin functions...
SellStockWin::SellStockWin(mainwindow* m, wxWindowID id, wxString msg, wxString caption, wxArrayString string) : 
	wxSingleChoiceDialog(m, msg, caption, string), m_parent(m)
{

}

// BottomFrame functions...
BottomFrame::BottomFrame(mainwindow* m, wxWindowID id) : wxWindow(m, id)
{
	this->Create();
}

void BottomFrame::Create()
{
	this->SetBackgroundColour(wxColour(28, 56, 100));
	this->materials = new wxStaticText(this, _Sector::MATERIALS, "Materials");
	this->comm_services = new wxStaticText(this, _Sector::COMMUNICATION_SERVICES, "Communication Services");
	this->cons_cycl = new wxStaticText(this, _Sector::CONSUMER_DESCRETIONARY, "Consumer Cyclical");
	this->cons_def = new wxStaticText(this, _Sector::CONSUMER_STAPLES, "Consumer Staples");
	this->energy = new wxStaticText(this, _Sector::ENERGY, "Energy");
	this->finance = new wxStaticText(this, _Sector::FINANCIALS, "Financial");
	this->healthcare = new wxStaticText(this, _Sector::HEALTH_CARE, "Healthcare");
	this->industrial = new wxStaticText(this, _Sector::INDUSTRIALS, "Industrials");
	this->real_estate = new wxStaticText(this, _Sector::REAL_ESTATE, "Real Estate");
	this->tech = new wxStaticText(this, _Sector::TECH, "Technology");
	this->utilities = new wxStaticText(this, _Sector::UTILITIES, "Utilities");

	int size = 5;
	int sectors = 11;
	wxBoxSizer* mainH = new wxBoxSizer(wxHORIZONTAL);

	for (size_t j = 0; j < sectors; ++j)
	{
		_Sector s = _Sector::SECTOR_INVALID;
		wxString _sector = "";
		wxStaticText* text = nullptr;
		switch (j)
		{
		case 0: s = _Sector::MATERIALS; text = this->materials; break;
		case 1: s = _Sector::COMMUNICATION_SERVICES; text = this->comm_services; break;
		case 2: s = _Sector::CONSUMER_DESCRETIONARY; text = this->cons_cycl; break;
		case 3: s = _Sector::CONSUMER_STAPLES; text = this->cons_def; break;
		case 4: s = _Sector::ENERGY; text = this->energy; break;
		case 5: s = _Sector::FINANCIALS; text = this->finance; break;
		case 6: s = _Sector::HEALTH_CARE; text = this->healthcare; break;
		case 7: s = _Sector::INDUSTRIALS; text = this->industrial; break;
		case 8: s = _Sector::REAL_ESTATE; text = this->real_estate; break;
		case 9: s = _Sector::INFORMATION_TECHNOLOGY; text = this->tech; break;
		case 10: s = _Sector::UTILITIES; text = this->utilities; break;
		}
		const ParentSector* ps = GetSectorClass().GetSector(s);
		wxBoxSizer* ver = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* left = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* right = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* b = new wxBoxSizer(wxHORIZONTAL);
		text->SetFont(text->GetFont().MakeBold());
		ver->Add(text, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 5);
		for (size_t i = 0; i < size; ++i)
		{
			wxString something = "";
			wxString data = "";
			switch (i)
			{
			case 0: something = "Day Return:"; data = ps->GetDayReturnString(); break;
			case 1: something = "Week Return:"; data = ps->GetWeekReturnString(); break;
			case 2: something = "Month Return:"; data = ps->GetMonthReturnString(); break;
			case 3: something = "Quarter Return:"; data = ps->GetQuarterReturnString(); break;
			case 4: something = "YTD Return:"; data = ps->GetYTDReturnString();
			}
			wxStaticText* desc = new wxStaticText(this, wxID_ANY, something);
			wxStaticText* _data = new wxStaticText(this, wxID_ANY, data);
			_data->SetFont(_data->GetFont().MakeBold());
			SetStaticTextColor(*_data, wxColour(6, 161, 53), wxColour(252, 42, 23));
			left->Add(desc, 1, wxALIGN_LEFT);
			right->Add(_data, 1, wxALIGN_RIGHT);
		}
		b->Add(left, wxALIGN_LEFT);
		b->Add(right, wxALIGN_RIGHT);
		ver->Add(b, 1, wxALIGN_CENTER_HORIZONTAL);
		mainH->Add(ver, 1, wxALIGN_CENTER_VERTICAL);
	}

	this->SetSizer(mainH);
	this->Layout();
}

// mainwindow functions...

mainwindow::mainwindow() : wxFrame(nullptr, 10000, "Brando's Investments", wxPoint(0, 0), wxSize(wxGetDisplaySize())), main_clock(wxDateTime::Today()),
	 port(this, 0, &main_clock)
{
	SectorClass& sc = GetSectorClass();
	sc.ActivateData();

	wxMenuBar* bar = new wxMenuBar();
	wxMenu* m = new wxMenu();
	wxMenu* stock = new wxMenu();
	wxMenu* deposits = new wxMenu();
	wxMenu* divs = new wxMenu();
	wxMenu* markets = new wxMenu();

	stock->Append(new wxMenuItem(stock, _SELL_STOCK, "Sell Stock"));
	stock->Append(new wxMenuItem(stock, _EnterDialog::STOCK_PURCHASE_DIALOG, "Purchase Stock"));
	deposits->Append(new wxMenuItem(deposits, NEW_DEPOSIT, "Add Deposit"));
	deposits->Append(new wxMenuItem(deposits, _WITHDRAWL, "Withdrawl"));
	deposits->Append(new wxMenuItem(deposits, NEW_DEPOSIT_SCHEDULE, "Add Deposit Schedule"));
	divs->Append(new wxMenuItem(divs, ENTER_DIVIDEND, "Add Dividend"));
	divs->Append(new wxMenuItem(divs, REMOVE_DIVIDEND, "Remove Dividend"));
	divs->Append(new wxMenuItem(divs, ADD_DIV_SHARES, "Re-Investment Shares"));

	m->Append(new wxMenuItem(m, wxID_SAVE, "Save"));
	markets->Append(new wxMenuItem(markets, QUOTE_LOOKUP, "Quote Lookup"));
	markets->Append(new wxMenuItem(markets, DAY_GAINERS_MENU, "Market Gainers"));
	markets->Append(new wxMenuItem(markets, DAY_LOSERS_MENU, "Market Losers"));
	bar->Append(m, "File");
	bar->Append(stock, "Invest");
	bar->Append(deposits, "Finance");
	bar->Append(divs, "Dividends");
	bar->Append(markets, "Markets");
	this->SetMenuBar(bar);
	this->Bind(wxEVT_CLOSE_WINDOW, &mainwindow::CloseEvent, this);
	this->Bind(wxEVT_THREAD, &mainwindow::OnThreadCompletion, this);
//	this->m_listCtrl = new VirtualListView(this, wxID_ANY, wxDefaultPosition, wxSize(wxSize(this->GetSize().GetWidth() - 10, this->GetSize().GetHeight() - 10)));
//	this->port.NewDepositSchedule(25.0, 14, wxDateTime(30, wxDateTime::Month::Dec, 2021));
	//SPY BUY 374.3487 3
	//NVDA BUY 129.785 3
	this->RetrieveFile();
	this->portwin = (PortfolioWin*)this->GetLeftWin();
	this->grid_panel = this->GetRightWin();
	this->grid_panel->Bind(wxEVT_CLOSE_WINDOW, &mainwindow::ClosePanelEvent, this);
	this->bottom_frame = (BottomFrame*)this->GetBottomFrame();
	this->bottom_frame->SetBackgroundColour(wxColour(28, 56, 100));
	wxBoxSizer* top = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* main = new wxBoxSizer(wxVERTICAL);
	top->Add(this->portwin, 1, wxEXPAND);
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

void mainwindow::DialogCancel()
{
	this->dialog = nullptr;
	this->Enable();
}

void mainwindow::PurchaseDataTransfer(PurchaseKit& kit)
{
//	this->dialog->Destroy();
	this->port.Purchase(kit.m_sect, kit.m_ticker, kit.m_date, kit.m_price, kit.m_shares, kit.m_reinvest, kit.m_reinvest_date);
	this->dialog = nullptr;
	this->UpdateGridView();
}

void mainwindow::CloseEvent(wxCloseEvent& evt)
{
	this->Hide();
	this->Destroy();
}

void mainwindow::OnThreadCompletion(wxThreadEvent& evt)
{
//	wxVector<StockNode*> stn = this->port.GetStockNodeItems();
//	for (size_t i = 0; i < stn.size(); ++i)
//		this->grid_view->SetNewRow(stn[i]->GetTicker(), stn[i]->GetShares(), stn[i]->GetPrice(), stn[i]->GetPreviousClose(),
//			stn[i]->GetDayReturn(), stn[i]->GetWeekReturn(), stn[i]->GetQuarterReturn(), stn[i]->GetYearReturn(), stn[i]->GetTotalReturn(true));
//	this->Refresh();
}

void mainwindow::OnPurchaseMenu(wxCommandEvent& evt)
{
	this->dialog = new Dialog(STOCK_PURCHASE_DIALOG, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Enter Stock Data", (int*)nullptr);
	if (this->dialog->ShowModal() == wxID_OK)
	{
		PurchaseKit kit = this->dialog->GetPurchaseKit();
		this->port.Purchase(kit.m_sect, kit.m_ticker, kit.m_date, kit.m_price, kit.m_shares, kit.m_reinvest, kit.m_reinvest_date);
		dialog = nullptr;
		this->UpdateGridView();
	}
}

void mainwindow::OnSellMenu(wxCommandEvent& evt)
{
	this->sell = new wxTextEntryDialog(this, "Enter Ticker Symbol");
	wxString user = "";
	this->sell->SetTextValidator(wxTextValidator(wxFILTER_ALPHA | wxFILTER_EMPTY, &user));
	this->sell->GetTextValidator()->Bind(wxEVT_KEY_DOWN, &mainwindow::OnKeyDown, this);
	this->sell->CenterOnParent();
	do
	{
		if (this->sell->ShowModal() == wxID_CANCEL)
			return;
	} while (!this->port.RequestSell(user));
	
	wxVector<stock_node*> sn = this->port.GetLotData();
	if (!sn.size())
	{
		if (this->sell->ShowModal())
		{
			this->sell->Destroy();
			this->sell = NULL;
		}
		return;
	}
	wxArrayString string;
	wxDateTime T = wxDateTime::Today();
	for (auto& v : sn)
	{
		wxString s = " Lot: " + wxNumberFormatter::ToString(v->GetLotNumber()) + "  Purchase Date: " + v->GetPurchaseDate().Format(STANDARD_DATE) + "  Purchase Price: " + 
			wxNumberFormatter::ToString(v->GetPurchasePrice(), 5) + "  Shares: " + wxNumberFormatter::ToString(v->GetShares(NULL), 5) + "  Costbasis: " + 
			wxNumberFormatter::ToString(v->GetCostBasis(&T), 2);
		string.push_back(s);
		string.push_back(s);
		string.push_back(s);
		string.push_back(s);
		string.push_back(s);
		string.push_back(s);
		string.push_back(s);
		string.push_back(s);
		string.push_back(s);
	}

	this->sellstock = new SellStockWin(this, wxID_ANY, "Pick lot to sell", user, string);
	stock_node* stock_n = NULL;
	if (this->sellstock->ShowModal() == wxID_OK)
	{
		int selection = this->sellstock->GetSelection();
		stock_n = sn[selection];
		this->sellstock->Destroy();
	}

	long lot = stock_n->GetLotNumber();
	wxDateTime date = stock_n->GetPurchaseDate();
	double shares = stock_n->GetShares(NULL);
	GenericKit kit(date, shares);
	this->dialog = new Dialog(_EnterDialog::SELL_STOCK, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, user, &kit);

	if (this->dialog->ShowModal() == wxID_OK)
	{
		SellKit kit = this->dialog->GetSellKit();
		kit.id = lot;
		this->port.Sell(lot, kit.date, kit.m_shares, kit.m_price);
	}
}

void mainwindow::OnSave(wxCommandEvent& evt)
{
	this->SaveFile();
}

void mainwindow::OnQuoteLookup(wxCommandEvent& evt)
{
	SummaryData sd;
	this->quote = new wxTextEntryDialog(this, "Enter Ticker Symbol");
	wxString user = "";
	this->quote->SetTextValidator(wxTextValidator(wxFILTER_ALPHA | wxFILTER_EMPTY, &user));
	this->quote->GetTextValidator()->Bind(wxEVT_KEY_DOWN, &mainwindow::OnKeyDown, this);
	this->quote->CenterOnParent();
	if (this->quote->ShowModal() == wxID_OK)
	{
		bool validate = this->quote->Validate();
		sd = port.QuoteLookup(this->quote->GetValue());
		if (sd.Longname == "NotFound")
		{
			wxMessageDialog* d = new wxMessageDialog(this, "Data for " + this->quote->GetValue() + " could not be found!");
			if (d->ShowModal())
				d->Destroy();
			return;
		}
		this->dialog = new Dialog(_EnterDialog::QUOTE_WIN, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, this->quote->GetValue(), &sd);
	}
}

void mainwindow::OnKeyDown(wxKeyEvent& evt)
{
	int n = evt.GetKeyCode();
	wxTextEntry* t = NULL;
	if (quote)
		t = this->quote->GetTextValidator()->GetTextEntry();
	else if (this->sell)
		t = this->sell->GetTextValidator()->GetTextEntry();
	t->ForceUpper();
	evt.Skip();
}

void mainwindow::ClosePanelEvent(wxCloseEvent& evt)
{
	this->grid_view->Cleanup();
}

void mainwindow::OnAddDeposit(wxCommandEvent& evt)
{
	this->dialog = new Dialog(_EnterDialog::ENTER_DEPOSIT, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Enter amount you want to deposit", (int*)nullptr);
	if (this->dialog->ShowModal() == wxID_OK)
	{
		GenericKit gk = this->dialog->GetGenericKit();
		if (this->port.AddDeposit(gk.date, gk.amount))
			this->portwin->Update();
		else
			wxMessageDialog* d = new wxMessageDialog(this, "Could not complete deposit!");
	}
}

void mainwindow::OnWithdrawl(wxCommandEvent& evt)
{
	this->dialog = new Dialog(_EnterDialog::WITHDRAWL, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Enter Amount you want to withdraw", (int*)nullptr);
	if (this->dialog->ShowModal() == wxID_OK)
	{
		GenericKit gk = this->dialog->GetGenericKit();
		if (this->port.AddWithdrawl(gk.date, gk.amount))
			this->portwin->Update();
		else
		{
			wxMessageDialog* d = new wxMessageDialog(this, "Could not complete withdrawl!");
			d->ShowModal();
		}
	}
}

void mainwindow::OnAddDividend(wxCommandEvent& evt)
{

}

void mainwindow::OnMarketGainers(wxCommandEvent&)
{
	wxDateTime T(wxDateTime::Today());
	GetWorkDate(T);
	wxVector<DayGainersandLosers> vec = this->port.GetDayGainers();
	this->dialog = new Dialog(_EnterDialog::DAY_GAINERS_WIN, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Losers as of " + T.Format(STANDARD_DATE), &vec);
}

void mainwindow::OnMarketLosers(wxCommandEvent&)
{
	wxDateTime T(wxDateTime::Today());
	GetWorkDate(T);
	wxVector<DayGainersandLosers> vec = this->port.GetDayLosers();
	this->dialog = new Dialog(_EnterDialog::DAY_LOSERS_WIN, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Losers as of " + T.Format(STANDARD_DATE), &vec);
}

void mainwindow::OnAddDepositSchdule(wxCommandEvent& evt)
{

}

void mainwindow::OnDateChange()
{
	this->port.DateChange();
	this->UpdateGridView();
	this->portwin->Update();
}

void mainwindow::OnUpdate()
{
	this->port.Update();
	this->UpdateGridView();
	this->portwin->Update();
}

// mainwindow private functions..

wxWindow* mainwindow::GetLeftWin()
{
	PortfolioWin* win = new PortfolioWin(this, wxID_ANY, &this->main_clock, this->port.GetStockViewerData(), &this->port);
//	wxWindow* w = new wxWindow(this, wxID_ANY);
//	w->SetBackgroundColour(wxColour(100,100,100));
	return win;
}

wxScrolled<wxPanel>* mainwindow::GetRightWin()
{
	wxScrolled<wxPanel>* P = new wxScrolled<wxPanel>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	P->SetScrollbars(5, 5, 50, 50);
	P->EnableScrolling(true, true);
//	wxPanel* w = new wxPanel(scrolled, wxID_ANY);
	P->SetBackgroundColour(wxColour(0, 0, 0));
	this->grid_view = new GridView(P, 20);
	this->UpdateGridView();
	/*
	wxVector<StockNode*> stn = this->port.GetStockNodeItems();
	for (size_t i = 0; i < stn.size(); ++i)
		this->grid_view->SetNewRow(stn[i]->GetStockViewerData());

	this->grid_view->SetNewRow(this->port.GetStockViewerData(), true);
	*/
	P->SetSizer(grid_view);
	return P;
}

wxWindow* mainwindow::GetBottomFrame()
{
	return new BottomFrame(this, wxID_ANY);
}

void mainwindow::UpdateGridView()
{
	if (!this->grid_view)
	{
		wxMessageBox("grid_veiw is null in mainwindow::UpdateGridView!");
		return;
	}

	const wxVector<StockNode*> stocks = this->port.GetStockNodeItems();
	for (size_t i = 0; i < stocks.size(); ++i)
	{
		StockViewerData* svd = stocks[i]->GetStockViewerData();
		if (this->grid_view->DoesItemExist(svd->ticker))
			this->grid_view->UpdateRow(svd);
		else
			this->grid_view->SetNewRow(svd);
	}
	
	StockViewerData* svd = this->port.GetStockViewerData();
	if (this->grid_view->DoesItemExist(svd->ticker))
		this->grid_view->UpdateRow(svd, true);
	else
		this->grid_view->SetNewRow(svd, true);

	this->grid_view->LayoutGrid();

	if (this->portwin)
		this->portwin->Update();
}

void mainwindow::SaveFile()
{
	this->port.Save();
}

void mainwindow::RetrieveFile()
{
	this->port.Retrieve();
}