#include "mainwindow.h"

wxBEGIN_EVENT_TABLE(mainwindow, wxFrame)
	EVT_MENU(STOCK_PURCHASE_DIALOG, OnPurchaseMenu)
	EVT_MENU(wxID_SAVE, OnSave)
	EVT_MENU(wxID_HOME, OnHome)
	EVT_MENU(wxID_EXECUTE, OnLoadSectorStocks)
	EVT_MENU(QUOTE_LOOKUP, OnQuoteLookup)
	EVT_MENU(VIEW_CHART, SampleStockChartView)
	EVT_MENU(_MenuItemIDs::NEW_DEPOSIT, OnAddDeposit)
	EVT_MENU(_MenuItemIDs::_WITHDRAWL, OnWithdrawl)
	EVT_MENU(_MenuItemIDs::NEW_DEPOSIT_SCHEDULE, OnAddDepositSchdule)
	EVT_MENU(_MenuItemIDs::ADD_DIV, OnAddDividend)
	EVT_MENU(_MenuItemIDs::VIEW_DIVIDENDS, OnViewDividend)
	EVT_MENU(_MenuItemIDs::ADD_DIV_SHARES, OnReInvestSharesMenu)
	EVT_MENU(_MenuItemIDs::DAY_GAINERS_MENU, OnMarketGainers)
	EVT_MENU(_MenuItemIDs::DAY_LOSERS_MENU, OnMarketLosers)
	EVT_MENU(_MenuItemIDs::_SELL_STOCK, OnSellMenu)
	EVT_MENU(_MenuItemIDs::P_QUOTE, OnQuoteLookupPopup)
	EVT_MENU(_MenuItemIDs::P_SELL_STOCK, OnSellPopupClick)
	EVT_MENU(_MenuItemIDs::P_STOCK_PURCHASE, OnPurchasePopupClick)
	EVT_MENU(_MenuItemIDs::P_ADD_DIV_REINVEST, OnAddReInvestSharesPopup)
	EVT_MENU(_MenuItemIDs::P_ADD_DIV, OnAddDividendPopup)
	EVT_MENU(_MenuItemIDs::P_VIEW_DIVIDENDS, OnViewDividendPopup)
	EVT_MENU(_MenuItemIDs::P_GRAPH, OnChartView)
	EVT_MENU(_MenuItemIDs::VIEW_DEPOSITS, OnViewDeposits)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(PortfolioWin, wxWindow)
	EVT_TEXT_ENTER(_PortfolioWin::DATEPCKER, DateChange)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(Dialog, wxDialog)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(BottomFrame, wxWindow)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(SectorStockWindow, wxFrame)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(ChartControl, wxWindow)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(ChartControlWin, wxWindow)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(X_Canvas, wxWindow)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(GridCanvas2, wxWindow)
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
auto constexpr DAY_GAIN$ = "Day Gain $";
auto constexpr WEEK_GAIN$ = "Week Gain $";
auto constexpr QUARTER_GAIN$ = "Quarter Gain $";
auto constexpr YEAR_GAIN$ = "Year Gain $";
auto constexpr TOTAL_GAIN$ = "Total Gain $";
auto constexpr PURCHASE_DATE = "Purchase Date";
auto constexpr PURCHASE_PRICE = "Purchase Price";
auto constexpr _MARKET_VALUE = "Market Value";
auto constexpr EARNINGS_DATE = "Next Earnings Date";
auto constexpr DIV_DATE = "Ex-Dividend";
auto constexpr DIVIDEND = "Dividend";
auto constexpr TOTAL_DIVIDENDS = "Total Dividends";
auto constexpr DIV_RE_INVEST = "Dividend Re-Investment";
auto constexpr _SECTOR = "Sector";
auto constexpr _EX_DIV = "Ex-Div Date";
auto constexpr PORTFOLIO_PERC = "Portfolio Percent";
auto constexpr _52WEEK_DEVIATION = "52Week Deviation";
auto constexpr _90DAY_DEVIATION = "90Day Deviation";
auto constexpr _30DAY_DEVIATION = "30Day Deviation";
auto constexpr WEEK_START_CLOSE = "W/start close";
auto constexpr WEEK_END_CLOSE = "W/end close";
auto constexpr BETA = "Beta";
auto constexpr GRIDROW = 1;
auto constexpr GRIDCOL = 2;
auto constexpr GRIDROWHEAD = 3;

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
	wxString oneamp = "&";

	s.Replace(amp, "&");
//	s.Replace(oneamp, "&&");
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

// MotionCanvas Functions..

MotionCanvas::MotionCanvas(wxWindow* w, wxSize size, wxVector<StockViewerData*> _svd_vec) : wxWindow(w, wxID_ANY, wxDefaultPosition, size),
	svd_vec(_svd_vec), textextent(0.0, 0.0), textextent2(0.0, 0.0), textextent3(0.0, 0.0)
{
	this->SetBackgroundColour(w->GetBackgroundColour());
	this->SetBackgroundStyle(wxBackgroundStyle::wxBG_STYLE_PAINT);
	this->Bind(wxEVT_PAINT, &MotionCanvas::OnPaint, this);
	this->Bind(wxEVT_ERASE_BACKGROUND, &MotionCanvas::OnEraseBackground, this);
}

void MotionCanvas::OnPaint(wxPaintEvent& evt)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
	if (!gc)
		return;

	wxPoint rectOrigin = this->FromDIP(wxPoint(0, 0));
	wxSize rectSize = this->FromDIP(this->GetSize());

	gc->SetBrush(gc->CreateBrush(wxBrush(this->background, wxBrushStyle::wxBRUSHSTYLE_SOLID)));
	gc->DrawRoundedRectangle(rectOrigin.x, rectOrigin.y, rectSize.GetWidth(), rectSize.GetHeight(), 5.0);

	gc->SetFont(gc->CreateFont(this->textfont, this->textcolor));

	bool bottom_in_motion = false;
	wxString value2 = "Red";
	wxString value = "";
	wxString value3 = "";
	wxString curr_price_literal = "Current Price";
	wxString previous_close_literal = "Previous Close";
	wxString day_gain_literal = "Day Gain";
	wxString week_gain_literal = "Week Gain";
	wxString quarter_gain_literal = "Quater Gain";
	wxString year_gain_literal = "Year Gain";
	wxString curr_price = "";
	wxString previous_close = "";
	wxString day_gain = "";
	wxString quarter_gain = "";
	if (top_distance_double == 4)
	{
		bottom_in_motion = true;
		// we pause the motion for the top graphic and re print it at this position until all of
		// its information has passed on the screen on the bottom of the rect...
		if (iter >= this->svd_vec.size())
			iter = 0;

		// we begin with the first element of info to pass across the screen...
		value = svd_vec[iter]->GetLongName();
		value = Decipherhtmlcodes(value);
		if (!iteminfo_iter)
		{
			value2 = curr_price_literal;
			value3 = this->svd_vec[iter]->GetPrice();
			this->bottom_text_color = this->green;
			gc->GetTextExtent(value2, &textextent2.w, &textextent2.h);
			gc->GetTextExtent(value3, &textextent3.w, &textextent3.h);
		}
		else if (iteminfo_iter == 1)
		{
			value2 = previous_close_literal;
			value3 = this->svd_vec[iter]->GetPreviousClose();
			this->bottom_text_color = this->green;
			gc->GetTextExtent(value2, &textextent2.w, &textextent2.h);
			gc->GetTextExtent(value3, &textextent3.w, &textextent3.h);
		}
		else if (iteminfo_iter == 2)
		{

			value3 = this->svd_vec[iter]->GetDayGain() + '%';
			value2 = day_gain_literal;
			int result = IsStringPNZ(value3);
			if (result == 0)
				this->bottom_text_color = this->yellow;
			else if (result == 1)
				this->bottom_text_color = this->green;
			else
				this->bottom_text_color = this->red;

			gc->GetTextExtent(value2, &textextent2.w, &textextent2.h);
			gc->GetTextExtent(value3, &textextent3.w, &textextent3.h);
		}
		else if (iteminfo_iter == 3)
		{
			value2 = week_gain_literal;
			value3 = this->svd_vec[iter]->GetWeekGain() + '%';
			int result = IsStringPNZ(value3);
			if (result == 0)
				this->bottom_text_color = this->yellow;
			else if (result == 1)
				this->bottom_text_color = this->green;
			else
				this->bottom_text_color = this->red;

			gc->GetTextExtent(value2, &textextent2.w, &textextent2.h);
			gc->GetTextExtent(value3, &textextent3.w, &textextent3.h);
		}
		else if (iteminfo_iter == 4)
		{
			value2 = quarter_gain_literal;
			value3 = this->svd_vec[iter]->GetQuarterGain() + '%';
			int result = IsStringPNZ(value3);
			if (result == 0)
				this->bottom_text_color = this->yellow;
			else if (result == 1)
				this->bottom_text_color = this->green;
			else
				this->bottom_text_color = this->red;

			gc->GetTextExtent(value2, &textextent2.w, &textextent2.h);
			gc->GetTextExtent(value3, &textextent3.w, &textextent3.h);
		}
		else if (iteminfo_iter == 5)
		{
			value2 = year_gain_literal;
			value3 = this->svd_vec[iter]->GetYearGain() + '%';
			int result = IsStringPNZ(value3);
			if (result == 0)
				this->bottom_text_color = this->yellow;
			else if (result == 1)
				this->bottom_text_color = this->green;
			else
				this->bottom_text_color = this->red;

			gc->GetTextExtent(value2, &textextent2.w, &textextent2.h);
			gc->GetTextExtent(value3, &textextent3.w, &textextent3.h);
		}
	}
	else if (top_distance_double == -21000.0)
	{
		value = this->svd_vec[iter]->GetLongName();
		value = Decipherhtmlcodes(value);
		gc->GetTextExtent(value, &textextent.w, &textextent.h);
		top_distance = textextent.w * -1;
		this->top_distance_double = (double)this->top_distance;
	}
	else if (top_distance_double > (double)rectSize.GetWidth())
	{
		if (iter + 1 >= this->svd_vec.size())
			iter = 0;
		else 
			++iter;
		value = this->svd_vec[iter]->GetLongName();
		value = Decipherhtmlcodes(value);
		gc->GetTextExtent(value, &textextent.w, &textextent.h);
		top_distance = textextent.w * -1;
		this->top_distance_double = (double)this->top_distance;
	}
	else
	{
		value = this->svd_vec[iter]->GetLongName();
		value = Decipherhtmlcodes(value);
		gc->GetTextExtent(value, &textextent.w, &textextent.h);
	}

	gc->DrawText(value, rectOrigin.x + this->top_distance_double, (rectOrigin.y + ((rectSize.GetHeight() / 2.0) / 2.0)) - textextent.h / 2.0); //Draw in top half of rect
	if (top_distance_double != 4.0)
	{
		++top_distance;
		this->top_distance_double += .5;
	}

	if (bottom_in_motion)
	{
		if (bottom_distance == -21000)
			bottom_distance = (textextent2.w + textextent3.w) * -1;

		gc->DrawText(value2, rectOrigin.x + this->bottom_distance_double,
			(((rectOrigin.y + rectSize.GetHeight()) * (3.0 / 4.0)) - textextent2.h / 2.0));

		gc->SetFont(gc->CreateFont(this->textfont, this->bottom_text_color));
		gc->DrawText(value3, rectOrigin.x + this->bottom_distance_double + textextent2.w + 6.0,
			(((rectOrigin.y + rectSize.GetHeight()) * (3.0 / 4.0)) - textextent3.h / 2.0)); // Draw in bottom half of rect
		this->bottom_distance++;
		this->bottom_distance_double = (double)bottom_distance - .2;
		if (bottom_distance > rectSize.GetWidth())
		{
			bottom_distance = -21000;
			if (iteminfo_iter == 5)
			{
				iteminfo_iter = 0;
				++top_distance_double;
			}
			else
				++iteminfo_iter;
		}
	}
	delete gc;
	return;
}

void MotionCanvas::OnEraseBackground(wxEraseEvent& evt)
{

}

// GridVal Functions...

GridVal::GridVal(wxWindow* w, wxString& val, int flags, wxSize size):
	value(val), textcolor(white), flag(flags), mySize(size)
{
	this->Initialize();
}

void GridVal::Initialize()
{
	this->ResetBools();

	if (flag & GridCanvasFlag::COLHEAD)
		this->SetToHeader();
	if (flag & GridCanvasFlag::ROWHEAD)
		this->SetToRowStartCanvas();
	if (flag & GridCanvasFlag::DATACELL)
		this->SetToDataCellColor();
	if (flag & GridCanvasFlag::MEDIUMFONT)
		this->SetToMediumFont();
	if (flag & GridCanvasFlag::LARGEFONT)
		this->SetToLargeFont();
	if (flag & GridCanvasFlag::TOTALROWHEAD)
		this->SetTotTotalStartRowCanvas();
	if (flag & GridCanvasFlag::TOTALROW)
		this->SetToTotalCanvas();
	if (flag & GridCanvasFlag::BACKGROUDNORMAL)
		this->SetToNormalBackground();
	if (flag & GridCanvasFlag::COLORDEPENDENT)
	{
		if (this->value.length())
			this->SetTextColor(IsStringPNZ(this->value));
	}

	this->CalcDoubleValue();
}

void GridVal::CalcDoubleValue()
{
	if (this->value.IsEmpty())
		return;

	wxString temp = this->value;

	int index = temp.find('%');
	if (index != -1)
		temp = temp.Mid(0, index);

	index = temp.find('$');
	if (index != -1)
		temp = temp.Mid(1);

	temp.ToDouble(&this->double_val);
}

void GridVal::SetToTotalCanvas()
{
	this->isTotalRow = true;
	this->background = this->totalRowColor;
}

void GridVal::SetTotTotalStartRowCanvas()
{
	this->isTotalRow = true;
	this->textcolor = this->rowHeaderColour;
	this->textfont = this->rowStartFont;
	this->SetToTotalCanvas();
}

void GridVal::SetToHeader()
{
	this->isHeaderCol = true;
	this->textcolor = this->colHeaderColour;
	this->textfont = this->rowStartFont;
	this->background = this->normal;
}

void GridVal::SetToRowStartCanvas()
{
	this->isHeaderRow = true;
	this->textcolor = this->rowHeaderColour;
	this->textfont = this->rowStartFont;
	this->background = this->normal;
}

void GridVal::SetToDataCellColor()
{
	this->isDataCell = true;
	this->textcolor = this->white;
}

void GridVal::SetToNormalBackground()
{
	this->background = this->normal;
}

void GridVal::SetToMediumFont()
{
	this->textfont = this->normalFont;
}

void GridVal::SetToLargeFont()
{
	this->textfont = this->rowStartFont;
}

void GridVal::SetTextColor(int i)
{
	if (!this->isTotalRow)
		this->isDataCell = true;

	switch (i)
	{
	case 1: this->textcolor = this->green; return;
	case 0: this->textcolor = this->yellow; return;
	case -1: this->textcolor = this->red; return;
	}

	wxFAIL_MSG("Switch in GridCanvas::SetTextColor failed!");
}

void GridVal::SetBackgroundColor()
{
	if (flag & GridCanvasFlag::BACKGROUDNORMAL)
		this->background = this->normal;
	if (flag & GridCanvasFlag::TOTALROWHEAD)
		this->background = this->totalRowColor;
	if (flag & GridCanvasFlag::TOTALROW)
		this->background = this->totalRowColor;
	if (flag & GridCanvasFlag::COLHEAD)
		this->background = this->normal;
	if (flag & GridCanvasFlag::ROWHEAD)
		this->background = this->normal;
}

void GridVal::ResetBools()
{
	this->isTotalRow = false;
	this->isHeaderRow = false;
	this->isHeaderCol = false;
	this->isDataCell = false;
}

void GridVal::CopyGridVal(const GridVal& gv)
{
	this->backup_flag = this->flag;
	this->backup_value = this->value;

	this->flag = gv.flag;
	this->value = gv.value;

	this->Initialize();
}

void GridVal::CopyGridValData(GridValData& gvd)
{
	this->flag = gvd.flags;
	this->value = gvd.value;

	this->Initialize();
}

bool GridVal::IsMatch(wxString& s)
{
	if (this->value.find("*") != -1)
	{
		if (s.find("*") == -1)
			return s + " *" == this->value;
	}

	else if (s.find("*") != -1)
	{
		wxString temp = value;
		temp += " *";
		if (temp == s)
			return true;
		else
			return false;
	}

	return s == this->value;
}

bool GridVal::IsEmpty()
{
	return this->value.IsEmpty();
}

void GridVal::SetValue(wxString& val)
{
	if (this->value == val)
		return;

	this->value = val;

	if (flag & GridCanvasFlag::COLORDEPENDENT)
		this->SetTextColor(IsStringPNZ(this->value));

	this->CalcDoubleValue();
}

void GridVal::SetHover()
{
	this->background = this->hover;
}

void GridVal::RemoveHover()
{
	this->SetBackgroundColor();
}

bool GridVal::IsMyPixel(const wxPoint& p)
{
	if (this->relativePosition.x <= p.x && this->relativePosition.x + this->mySize.GetWidth() >= p.x
		&& this->relativePosition.y <= p.y && this->relativePosition.y + this->mySize.GetHeight() >= p.y)
		return true;

	return false;
}

void GridVal::Clear(int flags)
{
	this->value = "";

	if (flags == -1)
		this->SetToNormalBackground();
	else
	{
		this->flag = flags;
		this->Initialize();
	}
}

// Gnode2 functions...
Gnode2::Gnode2(GridView2* gv, wxWindow* w, wxSize size, size_t row, size_t col, Gnode2* up, Gnode2* right, Gnode2* down, Gnode2* left, wxString& val, int flags)
	: GridVal(w, val, flags, size), m_parent(gv), rows(row), cols(col), m_up(up), m_right(right), m_down(down), m_left(left)
{

}

Gnode2::Gnode2(GridView2* gv, wxWindow* w, wxSize size, size_t row, size_t col, Gnode2* up, Gnode2* right, Gnode2* down, Gnode2* left, int flags)
	: GridVal(w, emptystring, flags, size), m_parent(gv), rows(row), cols(col), m_up(up), m_right(right), m_down(down), m_left(left)
{

}

void Gnode2::CopyGnode2(const Gnode2& gn)
{
	this->CopyGridVal(gn);
}

void Gnode2::SetUP(Gnode2* gn)
{
	this->m_up = gn;
}

void Gnode2::SetRight(Gnode2* gn)
{
	this->m_right = gn;
}

void Gnode2::SetDown(Gnode2* gn)
{
	this->m_down = gn;
}

void Gnode2::SetLeft(Gnode2* gn)
{
	this->m_left = gn;
}

Gnode2* Gnode2::GetUP()
{
	return this->m_up;
}

Gnode2* Gnode2::GetRight()
{
	return this->m_right;
}

Gnode2* Gnode2::GetDown()
{
	return this->m_down;
}

Gnode2* Gnode2::GetLeft()
{
	return this->m_left;
}

Gnode2* Gnode2::GetFarthestLeft()
{
	Gnode2* neighbor = this->GetLeft();
	Gnode2* temp = NULL;

	// If null return self;
	if (!neighbor)
		return this;

	while (neighbor)
	{
		temp = neighbor;
		neighbor = neighbor->GetLeft();
	}

	return temp;
}

void Gnode2::CopyFromTopNeighbor()
{
	Gnode2* neighbor = this->GetUP();
	if (neighbor)
		this->CopyGnode2(*neighbor);

//	this->SetNodeToolTip();

	neighbor = this->GetRight();
	if (neighbor)
		neighbor->CopyFromTopNeighbor();
}

void Gnode2::CopyFromBottomNeighbor()
{
	Gnode2* neighbor = this->GetDown();
	if (neighbor)
		this->CopyGnode2(*neighbor);

//	this->SetNodeToolTip();

	neighbor = this->GetRight();
	if (neighbor)
		neighbor->CopyFromBottomNeighbor();
}

void Gnode2::CopyGnodeData(wxVector<GridValData>& v, GridValData* gvd)
{
	if (gvd == v.end())
		return;

	this->CopyGridValData(*gvd);

//	this->SetNodeToolTip();

	Gnode2* neighbor = this->GetRight();
	if (neighbor)
		neighbor->CopyGnodeData(v, ++gvd);
}

void Gnode2::GetRowData(wxVector<GridValData>& v)
{
	v.push_back(this);
	Gnode2* neighbor = this->GetRight();
	if (neighbor)
		neighbor->GetRowData(v);
}

void Gnode2::ClearSelf(bool from_bottom)
{
	int flags = -1;
	Gnode2* neighbor = from_bottom ? this->GetDown() : this->GetUP();
	if (neighbor)
		flags = neighbor->Getflags();
	else if (this->GetDown())
		flags = this->GetDown()->Getflags();
	this->Clear(flags);

	neighbor = this->GetRight();
	if (neighbor)
		neighbor->ClearSelf(from_bottom);

//	this->SetToolTip(NULL);
}

void Gnode2::ClearSelf(Gnode2* gn)
{
	if (IsNull(gn, "Gnode2* in Gnode2::ClearSelf is nullptr!"))
		return;

	this->Clear(gn->Getflags());

	gn = gn->GetRight();
	Gnode2* neighbor = this->GetRight();

	if (gn && neighbor)
		neighbor->ClearSelf(gn);

}

// Private VirtualListView functions....
void VirtualListView::m_SetItemCount()
{
	this->SetItemCount(100);
}

GridCanvas2::GridCanvas2(mainwindow* w, int r, int c, wxPoint p, wxSize s, Gnode2*& h, wxSize childsize) : 
	wxWindow(w, wxID_ANY, p, s, wxFULL_REPAINT_ON_RESIZE),
	rows(r), cols(c), head(h), child_size(childsize), textextent(0.0, 0.0), m_parent(w)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT); // needed for windows
	this->SetBackgroundColour(wxColour(0, 0, 0));
	this->Bind(wxEVT_PAINT, &GridCanvas2::OnPaint, this);

	this->Bind(wxEVT_SIZE, &GridCanvas2::OnSizeChange, this);
	this->Bind(wxEVT_ENTER_WINDOW, &GridCanvas2::OnMouseEnter, this);
	this->Bind(wxEVT_MOTION, &GridCanvas2::OnMouseMove, this);
	this->Bind(wxEVT_LEAVE_WINDOW, &GridCanvas2::OnMouseLeave, this);
	this->Bind(wxEVT_RIGHT_DOWN, &GridCanvas2::OnRightDown, this);
	this->Bind(wxEVT_LEFT_DOWN, &GridCanvas2::OnLeftDown, this);

	this->horizontalrange = (c * childsize.GetWidth()) + ((1 + c) * this->distance);
	this->verticalrange = (r * childsize.GetHeight()) + ((1 + r) * this->distance);
}

GridCanvas2::~GridCanvas2()
{
	delete this->m_hrzntl;
	delete this->m_vrtcl;
}

void GridCanvas2::SetHeadNode(Gnode2* h)
{
	this->head = h;
}

void GridCanvas2::OnPaint(wxPaintEvent& evt)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	SmartPointer<wxGraphicsContext> gc(wxGraphicsContext::Create(dc));
	if (!gc)
		return;

	if (!this->head)
		return;

	Gnode2* next = this->head;
	wxSize rectSize = this->FromDIP(this->GetSize());
	wxSize childRectSize = this->FromDIP(this->child_size);

	if (head)
		child_size = head->GetMySize();

	while (next)
	{ 
		wxPoint rectOrigin;
		rectOrigin = wxPoint(next->GetMyPosition().x - this->x_position, next->GetMyPosition().y - this->y_position);

		next->SetMyRelativePosition(rectOrigin);
		this->background = next->GetBackColour();
		this->textcolor = next->GetTextColour();
		this->textfont = next->GetTextFont();
		if (rectOrigin.x + childRectSize.GetWidth() > 0 && rectOrigin.y + childRectSize.GetHeight() > 0 
			&& rectOrigin.x < rectSize.GetWidth() && rectOrigin.y < rectSize.GetHeight())
		{
			next->SetVisibility(true);
			this->PaintChild(&*gc, next->GetTextValue(), rectOrigin, childRectSize);
		}
		else
			next->SetVisibility(false);

		next = this->GetNextElem(next);
	}
}

void GridCanvas2::ScrollChanged(wxScrollEvent& evt)
{
	int orientation = evt.GetOrientation();
	if (orientation == wxHORIZONTAL)
	{
		if (!x_position)
			this->x_position = evt.GetPosition();
		else
			this->x_position = evt.GetPosition();
	}
	else
	{
		this->y_position = evt.GetPosition();
	}

	this->Refresh();
}

void GridCanvas2::OnLineChanged(wxScrollEvent& evt)
{
	int orientation = evt.GetOrientation();
	if (orientation == wxHORIZONTAL)
	{
		int pos = evt.GetPosition();
		if (pos == this->maxScrollPosH && pos == this->x_position)
			return;
		if (this->x_position > evt.GetPosition() + 1)
		{
			this->x_position = evt.GetPosition() - (this->head->GetMySize().GetWidth() + this->distance);
			if (this->x_position < 0)
				this->x_position = 0;
		}
		else if (this->x_position < evt.GetPosition())
		{
			this->x_position = evt.GetPosition() + this->head->GetMySize().GetWidth() + this->distance;
			if (this->x_position > ((this->m_virtualSize.GetWidth() + 20) - this->GetClientSize().GetWidth()))
				this->x_position = this->m_hrzntl->GetRange();
		}
		else
			return;

		this->m_hrzntl->SetThumbPosition(this->x_position);
	}
	else
	{
		int pos = evt.GetPosition();
		if (pos == this->maxScrollPosV && pos == this->x_position)
			return;
		if (this->y_position > evt.GetPosition() + 1)
		{
			this->y_position = evt.GetPosition() - (this->head->GetMySize().GetHeight() + this->distance);
			if (this->y_position < 0)
				this->y_position = 0;
		}
		else if (this->y_position < evt.GetPosition())
		{
			this->y_position = evt.GetPosition() + this->head->GetMySize().GetHeight() + this->distance;
			if (this->y_position > ((this->m_virtualSize.GetHeight() + 20) - this->GetClientSize().GetHeight()))
				this->y_position = this->m_vrtcl->GetRange();
		}
		else
			return;
		this->m_vrtcl->SetThumbPosition(this->y_position);
	}

	this->Refresh();
}

void GridCanvas2::OnScrollTop(wxScrollEvent& evt)
{

}

void GridCanvas2::OnScrollBottom(wxScrollEvent& evt)
{

}

void GridCanvas2::OnSizeChange(wxSizeEvent& evt)
{
	int size = 20;
	if (!this->m_hrzntl)
	{
		evt.Skip();
		return;
	}

	this->m_hrzntl->SetPosition(wxPoint(0, this->GetClientSize().GetHeight() - size));
	this->m_vrtcl->SetPosition(wxPoint(this->GetSize().GetWidth() - 20, 0));

	this->m_hrzntl->SetSize(wxSize(this->GetClientSize().GetWidth(), size));
	this->m_vrtcl->SetSize(wxSize(20, this->GetClientSize().GetHeight() - size));
}

void GridCanvas2::OnMouseEnter(wxMouseEvent& evt)
{
	wxPoint p = evt.GetPosition();

	if (focus)
	{
		if (this->IsFocus(focus, p))
			return;

		if (this->CheckNeighborsForFocus(p))
		{
			SetMyToolTip();
			return;
		}
	}

	Gnode2* next = this->head;
	next = this->FindMyFocus(p);
	if (!next)
	{
//		wxMessageBox("My focus was not found in GridCanvas2::OnMouseEnter!");
		return;
	}

	this->SetMyToolTip();
}

void GridCanvas2::OnMouseMove(wxMouseEvent& evt)
{
	wxPoint p = evt.GetPosition();
	if (this->focus)
	{
		if (this->IsFocus(focus, p))
			return;

		if (this->CheckNeighborsForFocus(p))
		{
			this->SetMyToolTip();
			return;
		}

		Gnode2* next = this->FindMyFocus(p);
		if (!next)
			return;

		SetMyToolTip();
	}

	Gnode2* next = this->FindMyFocus(p);
	if (!next)
		return;

	SetMyToolTip();
}

void GridCanvas2::OnMouseLeave(wxMouseEvent& evt)
{
	if (this->focus)
	{
		this->focus->RemoveHover();
		this->Refresh();
	}
	this->focus = NULL;
	this->SetToolTip(NULL);
}

void GridCanvas2::OnRightDown(wxMouseEvent& evt)
{
	wxString ticker = "";
	if (focus)
	{
		if (focus->IsTotalRow() || focus->IsDataCell() || focus->IsColHeader())
			return;

		ticker = focus->GetTextValue();
		wxCoord x, y;
		evt.GetPosition(&x, &y);
		wxPoint screen = this->FromDIP(this->GetScreenPosition());
		x += screen.x;
		y += screen.y;
		wxPoint p(x, y);
		int index = ticker.find(" ");
		if (index != -1)
			ticker = ticker.Mid(0, index);
		this->m_parent->RightClickGrid(ticker, p);
	}
}

void GridCanvas2::OnLeftDown(wxMouseEvent& evt)
{
	wxPoint p = evt.GetPosition();
	wxSize size = this->head->GetMySize();
	if (p.y + this->distance > size.y + distance)
		return;

	if (this->focus)
	{
		this->focus->GetParent()->OnSortClick(this->focus);
		this->Refresh();
	}
}

void GridCanvas2::SetScrollBars()
{
	int size = 20;
	this->m_hrzntl = new wxScrollBar(this, wxID_ANY, wxPoint(0, this->GetClientSize().GetHeight() - size), 
		wxSize(this->GetClientSize().GetWidth(), size), wxSB_HORIZONTAL);
	this->m_vrtcl = new wxScrollBar(this, wxID_ANY, wxPoint(this->GetSize().GetWidth() - 20, 0), 
		wxSize(20, this->GetClientSize().GetHeight() - size), wxSB_VERTICAL);

	this->m_hrzntl->SetRange((this->horizontalrange + size) - this->GetClientSize().GetWidth());
	this->m_vrtcl->SetRange((this->verticalrange + size) - this->GetClientSize().GetHeight());

	this->maxScrollPosH = this->m_hrzntl->GetRange() - this->m_hrzntl->GetThumbSize();
	this->maxScrollPosV = this->m_vrtcl->GetRange() - this->m_vrtcl->GetThumbSize();

	this->m_hrzntl->Bind(wxEVT_SCROLL_LINEDOWN, &GridCanvas2::OnLineChanged, this);
	this->m_hrzntl->Bind(wxEVT_SCROLL_LINEUP, &GridCanvas2::OnLineChanged, this);
	this->m_vrtcl->Bind(wxEVT_SCROLL_LINEDOWN, &GridCanvas2::OnLineChanged, this);
	this->m_vrtcl->Bind(wxEVT_SCROLL_LINEUP, &GridCanvas2::OnLineChanged, this);
	this->m_hrzntl->Bind(wxEVT_SCROLL_THUMBTRACK, &GridCanvas2::ScrollChanged, this);
	this->m_vrtcl->Bind(wxEVT_SCROLL_THUMBTRACK, &GridCanvas2::ScrollChanged, this);
	this->m_hrzntl->Bind(wxEVT_SCROLL_TOP, &GridCanvas2::OnScrollTop, this);
	this->m_vrtcl->Bind(wxEVT_SCROLL_TOP, &GridCanvas2::OnScrollTop, this);
	this->m_hrzntl->Bind(wxEVT_SCROLL_BOTTOM, &GridCanvas2::OnScrollBottom, this);
	this->m_vrtcl->Bind(wxEVT_SCROLL_BOTTOM, &GridCanvas2::OnScrollBottom, this);
}

void GridCanvas2::Initialize()
{
	Gnode2* next = NULL;
	next = this->head;
	int index = 0;
	int rowIndex = 0;
	int colIndex = 0;
	Gnode2* rowHead = next;
	Gnode2* previous = next;
	bool horizontal = false;
	bool horizontalcatch = true;
//	wxPoint lastpoint;
//	wxPoint firstDraw;
	wxSize rectSize = this->FromDIP(this->GetSize());
	if (head)
		child_size = head->GetMySize();
	this->m_virtualSize = this->FromDIP(wxSize(((cols + 1) * this->distance) + (child_size.GetWidth() * cols),
		((rows + 1) * this->distance) + (child_size.GetHeight() * (1 + rows))));

	while (head)
	{
		wxPoint rectOrigin;
		if (!index)
			rectOrigin = this->FromDIP(wxPoint(4, 4));
		else
		{
			if (horizontal)
			{
				rectOrigin = this->FromDIP(wxPoint(previous->GetMyPosition().x + this->child_size.GetWidth() + this->distance,
					previous->GetMyPosition().y));
			}
			else
			{
				rectOrigin = this->FromDIP(wxPoint(previous->GetMyPosition().x,
					previous->GetMyPosition().y + this->child_size.GetHeight() + this->distance));
			}
		}
		next->SetMyPosition(rectOrigin);
		
		if (!next->GetRight())
		{
			rowIndex = 0;
			colIndex++;
			next = rowHead->GetDown();
			previous = rowHead;
			if (!next)
				break;
			rowHead = next;
			horizontal = false;
			horizontalcatch = false;

		}
		else
		{
			++rowIndex;
			previous = next;
			next = next->GetRight();
			horizontal = true;
		}
		++index;
	}
}

void GridCanvas2::PaintChild(wxGraphicsContext* gc, wxString& value, wxPoint& rectOrigin, wxSize& rectSize)
{
	gc->SetBrush(*wxTheBrushList->FindOrCreateBrush(background, wxBrushStyle::wxBRUSHSTYLE_SOLID));
	gc->DrawRoundedRectangle(rectOrigin.x, rectOrigin.y, rectSize.GetWidth(), rectSize.GetHeight(), 5.0);

	// If the value of text is empty, no need to draw text...
	if (value.empty())
	{
		//		delete gc;
		return;
	}

	gc->SetFont(gc->CreateFont(this->textfont, this->textcolor));

	gc->GetTextExtent(value, &textextent.w, &textextent.h);

//	(rectOrigin.x + rectSize.GetWidth() / 2.0) - textextent.x / 2.0)
//	gc->DrawText(value, rectOrigin.x + this->distance, (rectOrigin.y + rectSize.GetHeight() / 2.0) - textextent.h / 2.0);
	gc->DrawText(value, (rectOrigin.x + rectSize.GetWidth() / 2.0) - (textextent.w / 2.0), (rectOrigin.y + rectSize.GetHeight() / 2.0) - textextent.h / 2.0);
}

void GridCanvas2::OnDynamicPaint()
{
	if (!focus)
		return;

	wxSize size = this->FromDIP(focus->GetMySize());
	wxPoint position = this->FromDIP(focus->GetMyRelativePosition());
	wxClientDC cdc(this);
	wxBufferedDC dc(&cdc, size);

//	dc.Clear();

	SmartPointer<wxGraphicsContext> gc(wxGraphicsContext::Create(dc));
	if (!gc)
		return;

	this->background = focus->GetBackColour();
	this->textcolor = focus->GetTextColour();
	this->textfont = focus->GetTextFont();
	this->PaintChild(&*gc, focus->GetTextValue(), position, size);
}

Gnode2* GridCanvas2::GetNextElem(Gnode2* gn, bool row)
{
	if (!gn)
		return NULL;

	Gnode2* next = gn->GetRight();
	if (!next)
	{
		if (row)
			return NULL;

		next = gn->GetFarthestLeft();
		if (!next)
		{
			wxMessageBox("Something went wrong in GridCanvas2::GetNextElem! GetFarthest left returned nullptr!");
			return NULL;
		}
		return next->GetDown();
	}
	else
		return next;
}

Gnode2* GridCanvas2::FindMyFocus(wxPoint& p)
{
	Gnode2* next = this->head;
	while (next)
	{
		if (this->IsFocus(next, p))
			return focus;

		next = this->GetNextElem(next);
	}

	return NULL;
}

void GridCanvas2::SetMyToolTip()
{
	if (this->focus)
	{
		this->focus->SetHover();
		Gnode2* right = focus->GetFarthestLeft();
		if (right && right->GetTextValue() != "Total" && right != this->head)
			this->SetToolTip(right->GetTextValue());
		else
			this->SetToolTip(NULL);
	}

	this->Refresh();
//	this->OnDynamicPaint();
}

bool GridCanvas2::IsFocus(Gnode2* gn, wxPoint& p)
{
	if (!gn)
		return false;

	if (gn->IsMyPixel(p))
	{
		if (focus == gn)
			return true;

		if (focus)
		{
			focus->RemoveHover();
			this->Refresh();
		}

		this->focus = gn;
		return true;
	}

	return false;
}

bool GridCanvas2::CheckNeighborsForFocus(wxPoint& p)
{
	if (!focus)
		return false;

	// Check all neighbors...
	Gnode2* up = focus->GetUP();
	if (this->IsFocus(up, p))
		return true;

	Gnode2* down = focus->GetDown();
	if (this->IsFocus(down, p))
		return true;

	Gnode2* right = focus->GetRight();
	if (this->IsFocus(right, p))
		return true;

	Gnode2* left = focus->GetLeft();
	if (this->IsFocus(left, p))
		return true;

	// kitty corner left up
	Gnode2* lefttup = left ? left->GetUP() : NULL;
	if (this->IsFocus(lefttup, p))
		return true;

	// kitty corner right up
	Gnode2* rightup = right ? right->GetUP() : NULL;
	if (this->IsFocus(rightup, p))
		return true;

	// kitty corner left down
	Gnode2* leftdown = left ? left->GetDown() : NULL;
	if (this->IsFocus(leftdown, p))
		return true;

	// kitty corner right down
	Gnode2* rightdown = right ? right->GetDown() : NULL;
	if (this->IsFocus(rightdown, p))
		return true;
	
	return false;
}

// ChartControl functions...
// wxFULL_REPAINT_ON_RESIZE needed for Windows
ChartControl::ChartControl(wxWindow* grandparent, wxWindow* parent, wxWindowID id, 
	const wxPoint& pos, const wxSize& size, wxString& _title, wxVector<Day_Prices>* v) 
	: wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE), m_grandparent(grandparent), m_parent(parent), values(v), title(_title)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT); // needed for windows
	this->SetBackgroundColour("Black");
	this->Bind(wxEVT_PAINT, &ChartControl::OnPaint, this);
	this->Bind(wxEVT_ENTER_WINDOW, &ChartControl::EnterWindow, this);
	this->Bind(wxEVT_MOTION, &ChartControl::Motion, this);
	this->Bind(wxEVT_LEFT_DOWN, &ChartControl::OnLeftDown, this);
//	this->Bind(wxEVT_CLOSE_WINDOW, &ChartControl::OnClose, this);

	this->SetMinSize(size);
}

ChartControl::~ChartControl()
{
	this->m_grandparent->Enable();
	this->m_grandparent->SetFocus();
}

void ChartControl::SetNewVector(wxVector<Day_Prices>* v)
{
	this->values = v;
}

void ChartControl::OnClose(wxCloseEvent& evt)
{
	this->m_grandparent->Enable();
	evt.Skip();
}

void ChartControl::EnterWindow(wxMouseEvent& evt)
{
	wxPoint p = evt.GetPosition();
	int range = 2;
	for (auto& it : this->mypair)
	{
		int rangetest_x = p.x < int(it.first.m_x) ? int(it.first.m_x) - p.x : p.x - int(it.first.m_x);
		if (rangetest_x <= range)
		{
			int rangetest_y = p.y < int(it.first.m_y) ? int(it.first.m_y) - p.y : p.y - int(it.first.m_y);
			if (rangetest_y <= range)
			{
				if (this->GetToolTipText().Find(it.second.date.Format(STANDARD_DATE)) != -1)
					return;
				wxString msg = it.second.date.Format(STANDARD_DATE) + "\n Close: " + wxNumberFormatter::ToString(it.second.close, 2);
				this->SetToolTip(msg);
				return;
			}
		}
	}
}

void ChartControl::Motion(wxMouseEvent& evt)
{
	int range = 2;
	wxPoint p = evt.GetPosition();
	for (auto& it : this->mypair)
	{
		int rangetest_x = p.x < int(it.first.m_x) ? int(it.first.m_x) - p.x : p.x - int(it.first.m_x);
		if (rangetest_x <= range)
		{
			int rangetest_y = p.y < int(it.first.m_y) ? int(it.first.m_y) - p.y : p.y - int(it.first.m_y);
			if (rangetest_y <= range)
			{
				if (this->GetToolTipText().Find(it.second.date.Format(STANDARD_DATE)) != -1)
					return;
				wxString msg = it.second.date.Format(STANDARD_DATE) + "\n Close: " + wxNumberFormatter::ToString(it.second.close, 2);
				this->SetToolTip(msg);
				return;
			}
		}
	}
}

void ChartControl::OnLeftDown(wxMouseEvent& evt)
{
	this->SetToolTip(NULL);
}

void ChartControl::OnPaint(wxPaintEvent& evt)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	if (gc && values->size() > 0)
	{
		wxFont titleFont = wxFont(wxNORMAL_FONT->GetPointSize() * 2.0,
			wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

		gc->SetFont(titleFont, *wxWHITE);

		double tw, th;
		gc->GetTextExtent(this->title, &tw, &th);

		const double titleTopBottomMinimumMargin = this->FromDIP(10);

		wxRect2DDouble fullArea{ 0, 0, static_cast<double>(GetSize().GetWidth()), static_cast<double>(GetSize().GetHeight()) };

		const double marginX = fullArea.GetSize().GetWidth() / 8.0;
		const double marginTop = std::max(fullArea.GetSize().GetHeight() / 8.0, titleTopBottomMinimumMargin * 2.0 + th);
		const double marginBottom = fullArea.GetSize().GetHeight() / 8.0;
		double labelsToChartAreaMargin = this->FromDIP(10);

		wxRect2DDouble chartArea = fullArea;
		chartArea.Inset(marginX, marginTop, marginX, marginBottom);

		gc->DrawText(this->title, (fullArea.GetSize().GetWidth() - tw) / 2.0, (marginTop - th) / 2.0);

		wxAffineMatrix2D normalizedToChartArea{};
		normalizedToChartArea.Translate(chartArea.GetLeft(), chartArea.GetTop());
		normalizedToChartArea.Scale(chartArea.m_width, chartArea.m_height);

		wxVector<double> temp;
		for (auto it = this->values->begin(); it < this->values->end(); ++it)
			temp.push_back(it->close);

		double lowValue = *std::min_element(temp.begin(), temp.end());
		double highValue = *std::max_element(temp.begin(), temp.end());
//		double lowValue = std::min_element(values->begin(), values->end(), [ascending](Day_Prices* left, Day_Prices* right) {
//			return genericCompare(left->close < right->close); })->close;
//		double highValue = std::max_element(values->begin(), values->end(), [](Day_Prices* left, Day_Prices* right) {
//			return left->close > right->close; })->close;

		//	const std::tuple<int, double, double>& st = [segmentCount, rangeLow, rangeHigh] = calculateChartSegmentCountAndRange(lowValue, highValue);
		const std::tuple<int, double, double>& st = calculateChartSegmentCountAndRange(lowValue, highValue);

		double yLinesCount = st._Myfirst._Val + 1;
		double yValueSpan = st._Get_rest()._Get_rest()._Myfirst._Val - st._Get_rest()._Myfirst._Val;

		lowValue = st._Get_rest()._Myfirst._Val;
		highValue = st._Get_rest()._Get_rest()._Myfirst._Val;
		/*
	double yValueSpan = rangeHigh - rangeLow;
	lowValue = rangeLow;
	highValue = rangeHigh;

	double yLinesCount = segmentCount + 1;
	*/

		wxAffineMatrix2D normalizedToValue{};
		normalizedToValue.Translate(0, highValue);
		normalizedToValue.Scale(1, -1);
		normalizedToValue.Scale(static_cast<double>(values->size() - 1), yValueSpan);

		gc->SetPen(*wxThePenList->FindOrCreatePen(wxColor(128, 128, 128)));
		gc->SetFont(*wxNORMAL_FONT, *wxWHITE);

		for (int i = 0; i < yLinesCount; i++)
		{
			double normalizedLineY = static_cast<double>(i) / (yLinesCount - 1);

			auto lineStartPoint = normalizedToChartArea.TransformPoint({ 0, normalizedLineY });
			auto lineEndPoint = normalizedToChartArea.TransformPoint({ 1, normalizedLineY });

			wxPoint2DDouble linePoints[] = { lineStartPoint, lineEndPoint };
			gc->StrokeLines(2, linePoints);

			double valueAtLineY = normalizedToValue.TransformPoint({ 0, normalizedLineY }).m_y;

			auto text = wxString::Format("%.2f", valueAtLineY);
			text = wxControl::Ellipsize(text, dc, wxELLIPSIZE_MIDDLE, chartArea.GetLeft() - labelsToChartAreaMargin);

			double tw, th;
			gc->GetTextExtent(text, &tw, &th);
			gc->DrawText(text, chartArea.GetLeft() - labelsToChartAreaMargin - tw, lineStartPoint.m_y - th / 2.0);
		}

		wxPoint2DDouble leftHLinePoints[] = {
			normalizedToChartArea.TransformPoint({0, 0}),
			normalizedToChartArea.TransformPoint({0, 1}) };

		wxPoint2DDouble rightHLinePoints[] = {
			normalizedToChartArea.TransformPoint({1, 0}),
			normalizedToChartArea.TransformPoint({1, 1}) };

		gc->StrokeLines(2, leftHLinePoints);
		gc->StrokeLines(2, rightHLinePoints);

		wxPoint2DDouble* pointArray = new wxPoint2DDouble[values->size()];

		wxAffineMatrix2D valueToNormalized = normalizedToValue;
		valueToNormalized.Invert();
		wxAffineMatrix2D valueToChartArea = normalizedToChartArea;
		valueToChartArea.Concat(valueToNormalized);

		this->mypair.clear();

		double placeholder = 0;
		int size = values->size();
		for (size_t i = 0; i < values->size(); i++)
		{
			placeholder = (double)i;
			wxPoint2DDouble po = valueToChartArea.TransformPoint(wxPoint2DDouble(placeholder, values->at(i).close));
			pointArray[i] = po;

			mypair.push_back(std::pair<wxPoint2DDouble, Day_Prices>(po, values->at(i)));
		}

		gc->SetPen(wxPen(*wxCYAN, 1));
		gc->StrokeLines(values->size(), pointArray);

		delete[] pointArray;
		delete gc;
	}
}

std::tuple<int, double, double> ChartControl::calculateChartSegmentCountAndRange(double origLow, double origHigh)
{
	constexpr double rangeMults[] = { 0.2, 0.25, 0.5, 1.0, 2.0, 2.5, 5.0 };
	constexpr int maxSegments = 6;

	double magnitude = std::floor(std::log10(origHigh - origLow));

	for (auto r : rangeMults)
	{
		double stepSize = r * std::pow(10.0, magnitude);
		double low = std::floor(origLow / stepSize) * stepSize;
		double high = std::ceil(origHigh / stepSize) * stepSize;

		int segments = round((high - low) / stepSize);

		if (segments <= maxSegments)
		{
			return std::make_tuple(segments, low, high);
		}
	}

	// return some defaults in case rangeMults and maxSegments are mismatched
	return std::make_tuple(10, origLow, origHigh);
}

// X_Canvas functions...
X_Canvas::X_Canvas(ChartControlWin* cw, wxPanel* p, wxWindowID id, const wxPoint point, const wxSize size, 
	wxColour foreground, wxColour background, wxColour hover_background) : wxWindow(p, id, point, size),
	m_grand_parent(cw), m_parent(p), x_color(foreground), normal_background(background), hover(hover_background)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT); // needed for windows
	this->SetBackgroundColour(p->GetBackgroundColour());
	this->Bind(wxEVT_ENTER_WINDOW, &X_Canvas::OnEnterWin, this);
	this->Bind(wxEVT_LEAVE_WINDOW, &X_Canvas::OnWinLeave, this);
	this->Bind(wxEVT_LEFT_DOWN, &X_Canvas::OnLeftDown, this);
	this->Bind(wxEVT_PAINT, &X_Canvas::OnPaint, this);
	this->Bind(wxEVT_ERASE_BACKGROUND, &X_Canvas::OnErase, this);
	this->m_foreground = x_color;
	this->m_background = normal_background;
}

void X_Canvas::OnEnterWin(wxMouseEvent& evt)
{
	this->m_background = this->hover;
	this->Refresh();
}

void X_Canvas::OnWinLeave(wxMouseEvent& evt)
{
	this->m_background = this->normal_background;
	this->Refresh();
}

void X_Canvas::OnLeftDown(wxMouseEvent& evt)
{
	this->m_grand_parent->OnExit();
}

void X_Canvas::OnPaint(wxPaintEvent& evt)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
	if (!gc)
		return;

	// top points
	wxPoint x1y1;
	wxPoint x2y2;

	// bottom points
	wxPoint x3y3;
	wxPoint x4y4;

	wxPoint origin = this->FromDIP(wxPoint(0, 0));
	wxSize originSize = this->FromDIP(this->GetSize());

	gc->SetBrush(*wxTheBrushList->FindOrCreateBrush(this->m_background, wxBrushStyle::wxBRUSHSTYLE_SOLID));
	gc->DrawRectangle(origin.x, origin.y, originSize.GetWidth(), originSize.GetHeight());

	// draw x...
	x1y1 = wxPoint(origin.x + 4, origin.y + 4);
	x2y2 = wxPoint(origin.x + originSize.GetWidth() - 4, origin.y + 4);

	x3y3 = wxPoint(origin.x + originSize.GetWidth() - 4, origin.y + originSize.GetHeight() - 4);
	x4y4 = wxPoint(origin.x + 4, origin.y + (originSize.GetHeight() - 4));

	gc->SetPen(*wxThePenList->FindOrCreatePen(this->x_color, 2, wxPenStyle::wxPENSTYLE_SOLID));
	gc->StrokeLine(x1y1.x, x1y1.y, x3y3.x, x3y3.y);
	gc->StrokeLine(x2y2.x, x2y2.y, x4y4.x, x4y4.y);

	delete gc;
}

void X_Canvas::OnErase(wxEraseEvent& evt)
{

}

// ChartControlWin functions...
ChartControlWin::ChartControlWin(mainwindow* m, wxWindowID id, const wxPoint& p, const wxSize& size, 
	wxString& s, wxVector<Day_Prices>* v, StockViewerData* _svd, _ChartControlType _type)
	: wxWindow(m, id, p, size, wxCLIP_CHILDREN), m_parent(m), svd(_svd), ticker(s), type(_type)
{
	if (v->size())
	{
		this->rangebegin = v->begin()->date.Format(STANDARD_DATE);
		this->range_end = v->rbegin()->date.Format(STANDARD_DATE);
		this->rangebegin_temp = this->rangebegin;
		this->range_end_temp = this->range_end;
	}
	wxString chartcontrolname = ticker.Mid(0, ticker.Find(":"));
	this->chart = new ChartControl(m_parent, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, chartcontrolname, v);
	this->Create();
}

void ChartControlWin::SetNewValues(wxVector<Day_Prices>* v, StockViewerData* _svd)
{
	this->chart->SetNewVector(v);
	this->svd = _svd;
	this->ReCalibratePanelData();
}

void ChartControlWin::OnExit()
{
	this->m_parent->GoToHomeWindow();
}

void ChartControlWin::OnClickPanel(wxMouseEvent& evt)
{
	this->SetFocus();
}

void ChartControlWin::OnPressEnter(wxCommandEvent& evt)
{
	this->SetFocus();

	this->rangebegin = this->start_range->GetValue();
	this->range_end = this->end_range->GetValue();

	if (rangebegin == this->rangebegin_temp && range_end == this->range_end_temp)
		return;

	wxDateTime start;
	wxDateTime end;
	if (!start.ParseDate(this->rangebegin))
	{
		this->start_range->SetLabelText(this->rangebegin_temp);
		this->end_range->SetLabelText(this->range_end_temp);
		return;
	}

	if (!end.ParseDate(this->range_end))
	{
		this->start_range->SetLabelText(this->rangebegin_temp);
		this->end_range->SetLabelText(this->range_end_temp);
		return;
	}

	if (start == end)
	{
		this->start_range->SetLabelText(this->rangebegin_temp);
		this->end_range->SetLabelText(this->range_end_temp);
		return;
	}

	if (start > end)
	{
		this->start_range->SetLabelText(this->rangebegin_temp);
		this->end_range->SetLabelText(this->range_end_temp);
		return;
	}

	this->rangebegin_temp = this->rangebegin;
	this->range_end_temp = this->range_end;

	wxString temp_tick = ticker.Mid(ticker.Find(":") + 1);
	this->m_parent->OnChartWinChangeDateRange(temp_tick, start, end);
}

void ChartControlWin::Create()
{
//	this->SetBackgroundColour(wxColour(69, 70, 71));
	this->SetBackgroundColour(wxColour(0, 0, 0));

	wxColour green = wxColour(13, 158, 20);
	wxColour red = wxColour(217, 7, 28);

	wxBoxSizer* main = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* ctrls = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* range_hor = new wxBoxSizer(wxHORIZONTAL);

	wxPanel* P = new wxPanel(this, wxID_ANY);
	P->SetBackgroundColour(wxColour(0, 0, 0));
	P->Bind(wxEVT_LEFT_DOWN, &ChartControlWin::OnClickPanel, this);

	this->exit = new X_Canvas(this, P, wxID_ANY, wxDefaultPosition, wxSize(20, 20), red, P->GetBackgroundColour(), wxColour(38, 37, 38));
	wxStaticText* _ticker = new wxStaticText(P, wxID_ANY, ticker.Mid(ticker.Find(":") + 1));
//	wxStaticText* range = new wxStaticText(P, wxID_ANY, "Range:\n" + rangebegin + " - " + range_end);

	wxStaticText* range = new wxStaticText(P, wxID_ANY, " - ");
	this->start_range = new wxTextCtrl(P, wxID_ANY, this->rangebegin, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	this->start_range->Bind(wxEVT_TEXT_ENTER, &ChartControlWin::OnPressEnter, this);
	this->start_range->SetFont(wxFont(15, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->start_range->SetBackgroundColour(P->GetBackgroundColour());
	this->start_range->SetForegroundColour(wxColour(255, 255, 255));
	this->end_range = new wxTextCtrl(P, wxID_ANY, this->range_end, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	this->end_range->Bind(wxEVT_TEXT_ENTER, &ChartControlWin::OnPressEnter, this);
	this->end_range->SetFont(wxFont(15, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->end_range->SetBackgroundColour(P->GetBackgroundColour());
	this->end_range->SetForegroundColour(wxColour(255, 255, 255));
//	this->start_range->SetValidator(wxTextValidator(wxFILTER_NONE, &this->rangebegin));
//	this->end_range->SetValidator(wxTextValidator(wxFILTER_NONE, &this->range_end));

	// literals...
	wxStaticText* sector_literal = new wxStaticText(P, wxID_ANY, "Sector:");
	wxStaticText* portfolio_per_literal = new wxStaticText(P, wxID_ANY, "Percent of Portfolio:");
	wxStaticText* sector_per_literal = new wxStaticText(P, wxID_ANY, "Percent of Sector:");
	
	wxStaticText* shares_literal = NULL;
	if (this->type == OWNED)
		shares_literal = new wxStaticText(P, wxID_ANY, "Shares:");
	wxStaticText* daygain_literal = new wxStaticText(P, wxID_ANY, "Day Gain:");
	wxStaticText* weekgain_literal = new wxStaticText(P, wxID_ANY, "Week Gain:");
	wxStaticText* quartergain_literal = new wxStaticText(P, wxID_ANY, "Quarter Gain:");
	wxStaticText* yeargain_literal = new wxStaticText(P, wxID_ANY, "Year Gain:");
	wxStaticText* totalgain_literal = new wxStaticText(P, wxID_ANY, "Total Gain:");

	// actual values...
	wxStaticText* sector = new wxStaticText(P, wxID_ANY, svd->GetSectorName());
	wxStaticText* portfolio_per = new wxStaticText(P, wxID_ANY, svd->GetPortfolioPerc() + "%");
	wxStaticText* sector_per = new wxStaticText(P, wxID_ANY, svd->GetSectorPerc() + "%");
	
	if (this->type == OWNED)
		this->shares = new wxStaticText(P, wxID_ANY, svd->GetShares());
	this->daygain = new wxStaticText(P, wxID_ANY, svd->GetDayGain() + "%");
	this->weekgain = new wxStaticText(P, wxID_ANY, svd->GetWeekGain() + "%");
	this->quartergain = new wxStaticText(P, wxID_ANY, svd->GetQuarterGain() + "%");
	this->yeargain = new wxStaticText(P, wxID_ANY, svd->GetYearGain() + "%");
	this->totalgain = new wxStaticText(P, wxID_ANY, svd->GetTotalGain() + "%");

	wxColour value(123, 120, 191);
	_ticker->SetFont(wxFont(16, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	_ticker->SetBackgroundColour(P->GetBackgroundColour());
	_ticker->SetForegroundColour(wxColour(127, 130, 128));
	range->SetFont(wxFont(16, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	range->SetBackgroundColour(P->GetBackgroundColour());
	range->SetForegroundColour(wxColour(127, 130, 128));

	sector_literal->SetForegroundColour(range->GetForegroundColour());
	portfolio_per_literal->SetForegroundColour(range->GetForegroundColour());
	sector_per_literal->SetForegroundColour(range->GetForegroundColour());
	portfolio_per->SetForegroundColour(value);
	sector_per->SetForegroundColour(value);
	sector->SetForegroundColour(value);

	if (this->type == OWNED)
		shares_literal->SetForegroundColour(range->GetForegroundColour());

	daygain_literal->SetForegroundColour(range->GetForegroundColour());
	weekgain_literal->SetForegroundColour(range->GetForegroundColour());
	quartergain_literal->SetForegroundColour(range->GetForegroundColour());
	yeargain_literal->SetForegroundColour(range->GetForegroundColour());
	totalgain_literal->SetForegroundColour(range->GetForegroundColour());

	sector_literal->SetFont(range->GetFont());
	portfolio_per_literal->SetFont(range->GetFont());
	sector_per_literal->SetFont(range->GetFont());
	portfolio_per->SetFont(range->GetFont());
	sector_per->SetFont(range->GetFont());
	sector->SetFont(range->GetFont());

	if (this->type == OWNED)
		shares_literal->SetFont(range->GetFont());

	daygain_literal->SetFont(range->GetFont());
	weekgain_literal->SetFont(range->GetFont());
	quartergain_literal->SetFont(range->GetFont());
	yeargain_literal->SetFont(range->GetFont());
	totalgain_literal->SetFont(range->GetFont());

	if (this->type == OWNED)
		shares->SetFont(range->GetFont());

	daygain->SetFont(range->GetFont());
	weekgain->SetFont(range->GetFont());
	quartergain->SetFont(range->GetFont());
	yeargain->SetFont(range->GetFont());
	totalgain->SetFont(range->GetFont());

	if (this->type == OWNED)
		shares->SetForegroundColour(green);

	SetStaticTextColor(*daygain, green, red);
	SetStaticTextColor(*weekgain, green, red);
	SetStaticTextColor(*quartergain, green, red);
	SetStaticTextColor(*yeargain, green, red);
	SetStaticTextColor(*totalgain, green, red);

	if (this->type == OWNED)
	{
		daygain->SetLabel(daygain->GetLabelText() + " ($" + svd->GetDayReturn$() + ")");
		weekgain->SetLabel(weekgain->GetLabelText() + " ($" + svd->GetWeekReturn$() + ")");
		quartergain->SetLabel(quartergain->GetLabelText() + " ($" + svd->GetQuarterReturn$() + ")");
		yeargain->SetLabel(yeargain->GetLabelText() + " ($" + svd->GetYearReturn$() + ")");
		totalgain->SetLabel(totalgain->GetLabelText() + " ($" + svd->GetTotalReturn$() + ")");
	}

	ctrls->AddSpacer(5);
	ctrls->Add(this->exit, 0, wxALIGN_RIGHT | wxRIGHT, 10);
	ctrls->Add(_ticker, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 10);
	range_hor->Add(this->start_range, 0, wxALIGN_CENTER_VERTICAL);
	range_hor->AddSpacer(5);
	range_hor->Add(range, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 10);
	range_hor->AddSpacer(5);
	range_hor->Add(this->end_range, 0, wxALIGN_CENTER_VERTICAL);

	ctrls->Add(range_hor, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, 10);
	ctrls->Add(sector_literal, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->Add(sector, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->AddSpacer(20);
	ctrls->Add(portfolio_per_literal, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->Add(portfolio_per, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->AddSpacer(20);
	ctrls->Add(sector_per_literal, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->Add(sector_per, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->AddSpacer(20);

	if (this->type == OWNED)
	{
		ctrls->Add(shares_literal, 0, wxALIGN_LEFT | wxLEFT, 20);
		ctrls->Add(shares, 0, wxALIGN_LEFT | wxLEFT, 20);
		ctrls->AddSpacer(20);
	}

	ctrls->Add(daygain_literal, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->Add(daygain, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->AddSpacer(20);
	ctrls->Add(weekgain_literal, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->Add(weekgain, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->AddSpacer(20);
	ctrls->Add(quartergain_literal, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->Add(quartergain, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->AddSpacer(20);
	ctrls->Add(yeargain_literal, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->Add(yeargain, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->AddSpacer(20);
	ctrls->Add(totalgain_literal, 0, wxALIGN_LEFT | wxLEFT, 20);
	ctrls->Add(totalgain, 0, wxALIGN_LEFT | wxLEFT, 20);

	P->SetSizer(ctrls);
	main->Add(this->chart, 4, wxEXPAND | wxRIGHT, 2);
	main->Add(P, 1, wxEXPAND);
	this->SetSizer(main);
}

void ChartControlWin::ReCalibratePanelData()
{
	wxColour green = wxColour(13, 158, 20);
	wxColour red = wxColour(217, 7, 28);

	if (this->type == OWNED)
		this->shares->SetLabel(svd->GetShares());

	this->daygain->SetLabel(svd->GetDayGain() + "%");
	this->weekgain->SetLabel(svd->GetWeekGain() + "%");
	this->quartergain->SetLabel(svd->GetQuarterGain() + "%");
	this->yeargain->SetLabel(svd->GetYearGain() + "%");
	this->totalgain->SetLabel(svd->GetTotalGain() + "%");

	SetStaticTextColor(*daygain, green, red);
	SetStaticTextColor(*weekgain, green, red);
	SetStaticTextColor(*quartergain, green, red);
	SetStaticTextColor(*yeargain, green, red);
	SetStaticTextColor(*totalgain, green, red);

	if (this->type == OWNED)
	{
		daygain->SetLabel(daygain->GetLabelText() + " ($" + svd->GetDayReturn$() + ")");
		weekgain->SetLabel(weekgain->GetLabelText() + " ($" + svd->GetWeekReturn$() + ")");
		quartergain->SetLabel(quartergain->GetLabelText() + " ($" + svd->GetQuarterReturn$() + ")");
		yeargain->SetLabel(yeargain->GetLabelText() + " ($" + svd->GetYearReturn$() + ")");
		totalgain->SetLabel(totalgain->GetLabelText() + " ($" + svd->GetTotalReturn$() + ")");
	}
}

GridView2::GridView2(wxWindow* m, mainwindow* w, int row, int col, wxSize childsize) 
	: GridCanvas2(w, row, col, wxDefaultPosition, wxDefaultSize, head, childsize),
	m_parent(w), rows(row), cols(col)
{
	for (size_t i = 0; i < cols; ++i)
		this->headers.push_back(this->GetColTitle(i));
	this->SetTitleRow();
	this->FillGrid();
}

GridView2::~GridView2()
{
	Gnode2* next = this->head;
	Gnode2* rowdown = next->GetDown();
	while (1)
	{
		Gnode2* nextone = NULL;
		if (next)
			nextone = next->GetRight();
		else
			return;
		if (nextone)
		{
			delete next;
			next = nextone;
		}
		else
		{
			delete next;
			next = rowdown;
			if (rowdown)
				rowdown = rowdown->GetDown();
		}
	}
}

void GridView2::SetNumOfItems(int items)
{
	if (this->itemsize == items)
		return;

	if (itemsize < items)
	{
		int index = items - itemsize;
		for (size_t i = 0; i < index; ++i)
		{
			Gnode2* neighbor = this->summaryrow->GetDown();
			if (neighbor)
				neighbor->CopyFromTopNeighbor();
			this->summaryrow->ClearSelf();
			this->summaryrow = neighbor;
		}
	}
	else
	{
		Gnode2* neighbor = this->summaryrow->GetUP();
		if (neighbor)
			neighbor->CopyFromBottomNeighbor();
		this->summaryrow->ClearSelf(true);
		this->summaryrow = neighbor;
	}

	itemsize = items;
}

void GridView2::SetNewRow(StockViewerData* svd)
{
	Gnode2* first = this->GetEmptyRow();
	if (!first)
	{
		wxFAIL_MSG("Gnode* first in GridView::SetNewRow is NULL!");
		return;
	}

	for (size_t i = 0; i < cols; ++i)
	{
		this->filledNodes++;
		wxString label = this->GetStringLabel(i, svd);
		first->SetValue(label);

		first = first->GetRight();
		if (first)
			continue;
		else
			break;
	}
}

bool GridView2::ItemExist(wxString& s)
{
	Gnode2* first = this->GetMatch(s);
	if (first)
		return true;

	return false;
}

void GridView2::OnSortClick(Gnode2* gn)
{
	wxString collabel = gn->GetTextValue();

	if (collabel == TICKER)
	{
		this->SortStrings(gn);
		return;
	}
	if (collabel == SHARES)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == PRICE)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == PURCHASE_PRICE)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == _PREVIOUS_CLOSE)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == DAY_GAIN)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == WEEK_GAIN)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == QUARTER_GAIN)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == YEAR_GAIN)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == TOTAL_GAIN)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == DAY_GAIN$)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == WEEK_GAIN$)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == QUARTER_GAIN$)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == YEAR_GAIN$)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == TOTAL_GAIN$)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == COST_BASIS)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == _MARKET_VALUE)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == PURCHASE_PRICE)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == TOTAL_DIVIDENDS)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == EARNINGS_DATE)
	{
		return;
	}
	if (collabel == PURCHASE_DATE)
	{
		return;
	}
	if (collabel == _EX_DIV)
	{
		return;
	}
	if (collabel == _SECTOR)
	{
		this->SortStrings(gn);
		return;
	}
	if (collabel == PORTFOLIO_PERC)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == _52WEEK_DEVIATION)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == _90DAY_DEVIATION)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == _30DAY_DEVIATION)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == WEEK_START_CLOSE)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == WEEK_END_CLOSE)
	{
		this->SortDouble(gn);
		return;
	}
	if (collabel == BETA)
	{
		this->SortDouble(gn);
		return;
	}
}

bool GridView2::RemoveRow(wxString& t)
{
	Gnode2* match = this->GetMatch(t);
	if (!match)
	{
		wxFAIL_MSG("GridView::RemoveRow could not find match for ticker: " + t);
		return false;
	}

	Gnode2* temp = NULL;
	while (1)
	{
		match->CopyFromBottomNeighbor();
		temp = match->GetDown();
		if (!temp)
			break;
		if (temp == this->summaryrow)
			break;

		match = temp;
	}

	if (temp)
	{
		temp->ClearSelf(true);
		this->summaryrow = temp->GetUP();
	}
	else
	{
		temp = match->GetUP();
		if (temp)
			temp = temp->GetUP();

		if (temp)
			match->ClearSelf(temp);
	}

	this->Refresh();
	return true;
}

void GridView2::SortStrings(Gnode2* gn)
{
	acending = acending ? false : true;
	wxVector<GridValData> first;
	wxVector<GridValData> second;
	Gnode2* sorting_node = gn->GetDown();
	while (sorting_node)
	{
		Gnode2* temp = sorting_node->GetFarthestLeft();
		if (temp == this->summaryrow)
			break;

		second.push_back(GridValData(temp));
		first.push_back(GridValData(sorting_node));

		sorting_node = sorting_node->GetDown();
	}

	bool sWitch = true;
	while (sWitch)
	{
		sWitch = false;
		for (size_t i = 0; i < first.size(); ++i)
		{
			if (i + 1 < first.size())
			{
				if (this->acending)
				{
					if ((first[i].gn->GetTextValue() > first[i + 1].gn->GetTextValue()))
					{
						sWitch = true;
						GridValData temp = first[i];
						first[i] = first[i + 1];
						first[i + 1] = temp;
					}
				}
				else
				{
					if ((first[i].gn->GetTextValue() < first[i + 1].gn->GetTextValue()))
					{
						sWitch = true;
						GridValData temp = first[i];
						first[i] = first[i + 1];
						first[i + 1] = temp;
					}
				}
			}
		}
	}

	wxVector<wxVector<GridValData>> sorted;
	for (size_t i = 0; i < first.size(); ++i)
	{
		first[i].gn = first[i].gn->GetFarthestLeft();
		wxVector<GridValData> temp;
		first[i].gn->GetRowData(temp);
		sorted.push_back(temp);
	}

	for (size_t i = 0; i < sorted.size(); ++i)
	{
		if (sorted[i].begin() != sorted[i].end() && sorted[i].begin()->gn == second[i].gn)
			continue;

		second[i].gn->CopyGnodeData(sorted[i], sorted[i].begin());
	}
}

void GridView2::SortDouble(Gnode2* gn)
{
	acending = acending ? false : true;
	wxVector<GridValData> first;
	wxVector<GridValData> second;
	Gnode2* sorting_node = gn->GetDown();
	while (sorting_node)
	{
		Gnode2* temp = sorting_node->GetFarthestLeft();
		if (temp == this->summaryrow)
			break;

		second.push_back(GridValData(temp));
		first.push_back(GridValData(sorting_node));

		sorting_node = sorting_node->GetDown();
	}

	bool sWitch = true;
	while (sWitch)
	{
		sWitch = false;
		for (size_t i = 0; i < first.size(); ++i)
		{
			if (i + 1 < first.size())
			{
				if (this->acending)
				{
					double firstone = first[i].gn->GetDoubleValue();
					double secondone = first[i + 1].gn->GetDoubleValue();
					if ((first[i].gn->GetDoubleValue() > first[i + 1].gn->GetDoubleValue()))
					{
						sWitch = true;
						GridValData temp = first[i];
						first[i] = first[i + 1];
						first[i + 1] = temp;
					}
				}
				else
				{
					if ((first[i].gn->GetDoubleValue() < first[i + 1].gn->GetDoubleValue()))
					{
						sWitch = true;
						GridValData temp = first[i];
						first[i] = first[i + 1];
						first[i + 1] = temp;
					}
				}
			}
		}
	}

	wxVector<wxVector<GridValData>> sorted;
	for (size_t i = 0; i < first.size(); ++i)
	{
		first[i].gn = first[i].gn->GetFarthestLeft();
		wxVector<GridValData> temp;
		first[i].gn->GetRowData(temp);
		sorted.push_back(temp);
	}

	for (size_t i = 0; i < sorted.size(); ++i)
	{
		if (sorted[i].begin() != sorted[i].end() && sorted[i].begin()->gn == second[i].gn)
			continue;

		second[i].gn->CopyGnodeData(sorted[i], sorted[i].begin());
	}
}


void GridView2::UpdateRow(StockViewerData* svd)
{
	size_t size = cols;
	Gnode2* first = this->GetMatch(svd->ticker);
	if (!first)
		return;
	for (size_t i = 0; i < size; ++i)
	{
		wxString label = this->GetStringLabel(i, svd);
		first->SetValue(label);

		first = first->GetRight();
		if (first)
			continue;
		else
			break;
	}
}

void GridView2::SetTitleRow()
{
	wxString val = "";
	Gnode2* last = nullptr;

	for (size_t i = 0; i < cols; ++i)
	{
		val = this->GetColTitle(i);
		if (i == 0)
		{
			this->head = new Gnode2(this, this->m_parent, wxSize(204, 30), (size_t)0, i, NULL, NULL, NULL, NULL, val, GridCanvasFlag::COLHEAD);
//			this->children.push_back(SmartPointer<Gnode2>(head));
		}
		else if (i == 1)
		{
			this->head->SetRight(new Gnode2(this, this->m_parent, wxSize(204, 30), (size_t)0, i, nullptr, nullptr, nullptr, this->head, val, GridCanvasFlag::COLHEAD));
			last = this->head->GetRight();
//			this->children.push_back(SmartPointer<Gnode2>(last));
		}
		else
		{
			last->SetRight(new Gnode2(this, this->m_parent, wxSize(204, 30), (size_t)0, i, nullptr, nullptr, nullptr, last, val, GridCanvasFlag::COLHEAD));
			last = last->GetRight();
//			this->children.push_back(SmartPointer<Gnode2>(last));

		}
		this->filledNodes++;
		++this->gridnode_Size;
	}
	this->SetHeadNode(this->head);
}

wxString GridView2::GetColTitle(size_t col)
{
	if (this->headers.size() && col < this->headers.size())
		return this->headers[col];

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
	case 10: return DAY_GAIN$;
	case 11: return WEEK_GAIN$;
	case 12: return QUARTER_GAIN$;
	case 13: return YEAR_GAIN$;
	case 14: return TOTAL_GAIN$;
	case 15: return COST_BASIS;
	case 16: return _MARKET_VALUE;
	case 17: return TOTAL_DIVIDENDS;
	case 18: return EARNINGS_DATE;
	case 19: return PURCHASE_DATE;
	case 20: return _EX_DIV;
	case 21: return _SECTOR;
	case 22: return PORTFOLIO_PERC;
	case 23: return _52WEEK_DEVIATION;
	case 24: return _90DAY_DEVIATION;
	case 25: return _30DAY_DEVIATION;
	case 26: return WEEK_START_CLOSE;
	case 27: return WEEK_END_CLOSE;
	case 28: return BETA;
	default: return "";
	}

	return "";
}

Gnode2* GridView2::GetEmptyRow()
{
	Gnode2* next = this->head->GetDown();
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
			return NULL;
	}

	return NULL;
}

wxString GridView2::GetStringLabel(size_t& col, StockViewerData* svd)
{
	if (!svd)
	{
		wxFAIL_MSG("StockViewerData* is nullptr in GridView::GetStringLabel!");
		return "";
	}

	wxString collabel = this->GetColTitle(col);
	if (collabel.IsEmpty())
		return "";

	if (collabel == TICKER)
		return svd->GetTicker();
	if (collabel == SHARES)
		return svd->GetShares();
	if (collabel == PRICE)
		return svd->GetPrice();
	if (collabel == PURCHASE_PRICE)
		return svd->GetPurchasePrice();
	if (collabel == _PREVIOUS_CLOSE)
		return svd->GetPreviousClose();
	if (collabel == DAY_GAIN)
		return svd->GetDayGain() + "%";
	if (collabel == WEEK_GAIN)
		return svd->GetWeekGain() + "%";
	if (collabel == QUARTER_GAIN)
		return svd->GetQuarterGain() + "%";
	if (collabel == YEAR_GAIN)
		return svd->GetYearGain() + "%";
	if (collabel == TOTAL_GAIN)
		return svd->GetTotalGain() + "%";

	if (collabel == DAY_GAIN$)
		return "$" + svd->GetDayReturn$();
	if (collabel == WEEK_GAIN$)
		return "$" + svd->GetWeekReturn$();
	if (collabel == QUARTER_GAIN$)
		return "$" + svd->GetQuarterReturn$();
	if (collabel == YEAR_GAIN$)
		return "$" + svd->GetYearReturn$();
	if (collabel == TOTAL_GAIN$)
		return "$" + svd->GetTotalReturn$();

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
	if (collabel == _EX_DIV)
		return svd->GetExDivDate();
	if (collabel == _SECTOR)
		return svd->GetSectorName();
	if (collabel == PORTFOLIO_PERC)
		return svd->GetPortfolioPerc();
	if (collabel == _52WEEK_DEVIATION)
		return svd->Get52Week_Deviation();
	if (collabel == _90DAY_DEVIATION)
		return svd->Get90Day_Deviation();
	if (collabel == _30DAY_DEVIATION)
		return svd->Get30Day_Deviation();
	if (collabel == BETA)
		return svd->GetBeta();
	return "";
}

int GridView2::GetGridNodeFlags(size_t& col, size_t& row)
{
	if (this->headers.size() <= col)
		return 0;

	if (!col)
	{
		if (itemsize != row)
			return GridCanvasFlag::ROWHEAD;
		else
			return GridCanvasFlag::DATACELL | GridCanvasFlag::LARGEFONT | GridCanvasFlag::TOTALROW;
	}

	// check to see if this would be the total row...
	if (itemsize == row)
	{
		if (this->headers[col].find("Gain") != -1)
			return GridCanvasFlag::COLORDEPENDENT | GridCanvasFlag::LARGEFONT | GridCanvasFlag::TOTALROW;
		else
			return GridCanvasFlag::DATACELL | GridCanvasFlag::LARGEFONT | GridCanvasFlag::TOTALROW;
	}

	if (this->headers[col].find("Gain") != -1)
		return GridCanvasFlag::COLORDEPENDENT | GridCanvasFlag::MEDIUMFONT | GridCanvasFlag::BACKGROUDNORMAL;
	else
		return GridCanvasFlag::DATACELL | GridCanvasFlag::MEDIUMFONT | GridCanvasFlag::BACKGROUDNORMAL;
}

Gnode2* GridView2::GetFarthestRight(Gnode2* g)
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

Gnode2* GridView2::GetRight(Gnode2* g, size_t i, size_t start)
{
	if (!g)
	{
		wxFAIL_MSG("GridNode* passed to GridView::GetRight is nullptr!");
		return nullptr;
	}

	while (start < i)
	{
		if (!g->GetRight())
			return g;
		g = g->GetRight();
		++start;
	}

	return g;
}

Gnode2* GridView2::GetMatch(wxString& s)
{
	Gnode2* next = this->head->GetDown();
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
			return NULL;
	}

	return NULL;
}

void GridView2::FillGrid()
{
	Gnode2* last = nullptr;
	Gnode2* last_row = this->head;
	Gnode2* current_row = nullptr;
	size_t rowsize = rows;
	size_t size = rowsize * 2;
	size_t colsize = cols;
	for (size_t i = 0; i < size; ++i)
	{
		if (i > 0)
			last_row = current_row;
		for (size_t j = 0; j < colsize; ++j)
		{
			int flags = GetGridNodeFlags(j, i);
			if (j == 0)
			{
				last_row->SetDown(new Gnode2(this, this->m_parent, wxSize(204, 30), i + 1, (size_t)j, last_row, nullptr, nullptr, nullptr, flags));
				last = current_row = last_row->GetDown();
				if (i == this->itemsize + 1)
					this->summaryrow = last_row;
			}
			else
			{
				Gnode2* lastrowitem = this->GetRight(last_row, j);
				Gnode2* currentrowitem = this->GetFarthestRight(current_row);
				if (currentrowitem)
				{
					currentrowitem->SetRight(new Gnode2(this, this->m_parent, wxSize(204, 30), i + 1, j, lastrowitem, nullptr, nullptr, currentrowitem, flags));
					if (lastrowitem)
						lastrowitem->SetDown(currentrowitem->GetRight());
				}
			}
			++this->gridnode_Size;
		}
	}
}

MyThread::MyThread(PortfolioWin* p, MotionCanvas* _gc) :
	wxThread(wxTHREAD_DETACHED), gc(_gc), m_parent(p)
{

}

MyThread::~MyThread()
{
	this->m_parent->thread = NULL;
}

wxThread::ExitCode MyThread::Entry()
{
	while (!this->TestDestroy())
	{
		wxMicroSleep(1000);
		if (gc)
			this->gc->Refresh();
	}

	return (wxThread::ExitCode)0;
}

MyQuoteThread::MyQuoteThread(Portfolio* p, mainwindow* m, wxString& _ticker) :
	wxThread(wxTHREAD_DETACHED), port(p), m_parent(m), ticker(_ticker)
{

}

MyQuoteThread::~MyQuoteThread()
{
	m_parent->quotethread = NULL;
}

wxThread::ExitCode MyQuoteThread::Entry()
{
	if (!this->TestDestroy())
		this->m_parent->smd = port->QuoteLookup(ticker);

	this->m_parent->OnQuoteThread();
	return (wxThread::ExitCode)0;
}

LoadSectorStocksThread::LoadSectorStocksThread(mainwindow* m) : wxThread(wxThreadKind::wxTHREAD_DETACHED), m_parent(m)
{

}

LoadSectorStocksThread::~LoadSectorStocksThread()
{
	this->m_parent->mainwindow_thread = NULL;
}

wxThread::ExitCode LoadSectorStocksThread::Entry()
{
	SectorClass& sc = GetSectorClass();
	sc.LoadAllStocks(this, this->m_parent);
	wxPostEvent(this->m_parent, wxThreadEvent(1));
	return wxThread::ExitCode(0);
}

PortfolioWin::PortfolioWin(mainwindow* parent, wxWindowID id, wxDateTime* main, StockViewerData* sv, Portfolio* p) 
	: wxWindow(parent, id), mainclock(main), svd(sv), port(p), m_parent(parent), today(wxDateTime::Today())
{
	this->SetBackgroundColour(wxColour(100, 100, 100));
	this->Create();
}

void PortfolioWin::Update()
{
	if (this->thread && !this->thread->IsRunning())
		this->thread->Run();

	StockViewerData* svd = port->GetStockViewerData();
	this->CashDisplay->SetLabel("$" + wxNumberFormatter::ToString(this->port->GetFreeCash(), 2));
	this->DayReturnDisplay->SetLabel(svd->GetDayGain());
	this->DayReturnDollar->SetLabel(svd->GetDayReturn$());
	this->AccountValueDisplay->SetLabel("$" + wxNumberFormatter::ToString(svd->market_value + port->GetFreeCash(), 2));
	this->MarketValueDisplay->SetLabel("$" + svd->GetMarketValue());
	this->TotalReturnDisplay->SetLabel(svd->GetTotalGain());
	this->TotalReturnDollar->SetLabel(svd->GetTotalReturn$());

	SetStaticTextColor(*this->DayReturnDisplay, this->Green, this->Red);
	SetStaticTextColor(*this->DayReturnDollar, this->Green, this->Red);
	SetStaticTextColor(*this->TotalReturnDisplay, this->Green, this->Red);
	SetStaticTextColor(*this->TotalReturnDollar, this->Green, this->Red);

	this->Layout();
	this->Refresh();
}

void PortfolioWin::ClosingSoon()
{
	if (this->thread)
	{
		this->thread->Delete();
		while (this->thread)
		{

		}
	}
}

void PortfolioWin::Create()
{
	// initialize controls...
	this->panel = new wxPanel(this, wxID_ANY);
	this->panel->SetBackgroundColour(this->GetBackgroundColour());
	this->updatebutton = new wxButton(this->panel, wxID_ANY, "Update");
	this->updatebutton->Bind(wxEVT_BUTTON, &PortfolioWin::OnUpdateButtonPress, this);
	this->datepcker = new wxTextCtrl(panel, _PortfolioWin::DATEPCKER, this->today.Format(STANDARD_DATE), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_PROCESS_ENTER);

	wxString val = DIV_RE_INVEST;
	
	wxVector<Indices*> in_vec = this->port->GetIndices();
	wxVector<StockViewerData*> s_vec;
	for (auto& v : in_vec)
		s_vec.push_back(v->GetStockViewerData());
	this->canvas = new MotionCanvas(this->panel, wxSize(204, 60), s_vec);

	this->thread = new MyThread(this, this->canvas);
	this->todaysdate = new wxStaticText(panel, wxID_ANY, "Today's Date");
	this->todaysdateDisplay = new wxStaticText(panel, wxID_ANY, this->today.Format(STANDARD_DATE));
	this->Cash = new wxStaticText(panel, wxID_ANY, "Cash");
	this->CashDisplay = new wxStaticText(panel, wxID_ANY, "$" + wxNumberFormatter::ToString(this->port->GetFreeCash(), 2));
	this->DayReturn = new wxStaticText(panel, wxID_ANY, "Day Return");
	this->DayReturnDisplay = new wxStaticText(panel, wxID_ANY, svd->GetDayGain());
	this->DayReturnDollar = new wxStaticText(panel, wxID_ANY, svd->GetDayReturn$());
	this->AccountValue = new wxStaticText(panel, wxID_ANY, "Account Value");
	this->AccountValueDisplay = new wxStaticText(panel, wxID_ANY, "$" + svd->GetTotalValue());
	this->MarketValue = new wxStaticText(panel, wxID_ANY, "Market Value");
	this->MarketValueDisplay = new wxStaticText(panel, wxID_ANY, "$" + svd->GetMarketValue());
	this->TotalReturn = new wxStaticText(panel, wxID_ANY, "Total Return");
	this->TotalReturnDisplay = new wxStaticText(panel, wxID_ANY, svd->GetTotalGain());
	this->TotalReturnDollar = new wxStaticText(panel, wxID_ANY, svd->GetTotalReturn$());

	this->t_percent = new wxStaticText(panel, wxID_ANY, "%");
	this->t_dollar = new wxStaticText(panel, wxID_ANY, "$");
	this->t_beginparenths = new wxStaticText(panel, wxID_ANY, "(");
	this->t_endParenths = new wxStaticText(panel, wxID_ANY, ")");

	this->d_percent = new wxStaticText(panel, wxID_ANY, "%");
	this->d_dollar = new wxStaticText(panel, wxID_ANY, "$");
	this->d_beginparenths = new wxStaticText(panel, wxID_ANY, "(");
	this->d_endParenths = new wxStaticText(panel, wxID_ANY, ")");

	this->SetControlFonts();

	
	wxBoxSizer* main = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* panelsizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* panelsizerH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* datepckerH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* totaldisplayH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* daydisplayH = new wxBoxSizer(wxHORIZONTAL);

	totaldisplayH->Add(this->TotalReturnDisplay, 0);
	totaldisplayH->Add(this->t_percent, 0);
	totaldisplayH->AddSpacer(5);
	totaldisplayH->Add(this->t_beginparenths, 0);
	totaldisplayH->Add(this->t_dollar, 0);
	totaldisplayH->Add(this->TotalReturnDollar, 0);
	totaldisplayH->Add(this->t_endParenths, 0);

	daydisplayH->Add(this->DayReturnDisplay, 0);
	daydisplayH->Add(this->d_percent, 0);
	daydisplayH->AddSpacer(5);
	daydisplayH->Add(this->d_beginparenths, 0);
	daydisplayH->Add(this->d_dollar, 0);
	daydisplayH->Add(this->DayReturnDollar, 0);
	daydisplayH->Add(this->d_endParenths, 0);

	datepckerH->Add(this->datepcker, 1, wxTOP, 10);
	datepckerH->Add(this->updatebutton, 0, wxLEFT | wxTOP, 10);
	main->Add(datepckerH, 0, wxALIGN_CENTER_HORIZONTAL);
	main->Add(this->todaysdate, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);

	main->AddSpacer(5);
	main->Add(this->todaysdateDisplay, 0, wxALIGN_LEFT | wxLEFT, 10);

	main->AddSpacer(5);
	main->Add(this->canvas, 1, wxEXPAND | wxALL, 10);
//	main->Add(line4H, 0, wxEXPAND | wxALL, 5);
	main->Add(this->Cash, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	main->AddSpacer(5);
	main->Add(this->CashDisplay, 1, wxALIGN_LEFT | wxLEFT, 10);
//	main->Add(line1H, 0, wxEXPAND | wxALIGN_TOP | wxTOP | wxLEFT | wxRIGHT, 5);
	main->Add(this->DayReturn, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	main->AddSpacer(5);
	main->Add(daydisplayH, 1, wxALIGN_LEFT | wxLEFT, 10);
//	main->Add(line2H, 0, wxEXPAND | wxALL, 5);
	main->Add(this->AccountValue, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	main->AddSpacer(5);
	main->Add(this->AccountValueDisplay, 1, wxALIGN_LEFT | wxLEFT, 10);
	main->AddSpacer(5);
	main->Add(this->MarketValue, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	main->AddSpacer(5);
	main->Add(this->MarketValueDisplay, 1, wxALIGN_LEFT | wxLEFT, 10);
//	main->Add(line3H, 0, wxEXPAND | wxALL, 5);
	main->Add(this->TotalReturn, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	main->AddSpacer(5);
	main->Add(totaldisplayH, 1, wxALIGN_LEFT | wxLEFT, 10);

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
	this->DayReturnDollar->SetFont(wxFont(19, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->AccountValue->SetFont(wxFont(17, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->AccountValueDisplay->SetFont(wxFont(19, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->MarketValue->SetFont(this->AccountValue->GetFont());
	this->MarketValueDisplay->SetFont(this->AccountValueDisplay->GetFont());
	this->TotalReturn->SetFont(wxFont(17, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->TotalReturnDisplay->SetFont(wxFont(19, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->TotalReturnDollar->SetFont(this->TotalReturnDisplay->GetFont());

	this->t_percent->SetFont(wxFont(19, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->t_dollar->SetFont(this->t_percent->GetFont());
	this->t_beginparenths->SetFont(this->t_percent->GetFont());
	this->t_endParenths->SetFont(this->t_percent->GetFont());

	this->d_percent->SetFont(this->t_percent->GetFont());
	this->d_dollar->SetFont(this->t_percent->GetFont());
	this->d_beginparenths->SetFont(this->t_percent->GetFont());
	this->d_endParenths->SetFont(this->t_percent->GetFont());

	this->d_dollar->SetForegroundColour(this->parenths);
	this->d_percent->SetForegroundColour(this->percents);
	this->d_beginparenths->SetForegroundColour(this->parenths);
	this->d_endParenths->SetForegroundColour(this->parenths);
	
	this->t_dollar->SetForegroundColour(this->parenths);
	this->t_percent->SetForegroundColour(this->percents);
	this->t_beginparenths->SetForegroundColour(this->parenths);
	this->t_endParenths->SetForegroundColour(this->parenths);

	this->todaysdate->SetForegroundColour(this->DarkGrey);
	this->Cash->SetForegroundColour(this->DarkGrey);
	this->DayReturn->SetForegroundColour(this->DarkGrey);
	this->AccountValue->SetForegroundColour(this->DarkGrey);
	this->MarketValue->SetForegroundColour(this->DarkGrey);
	this->TotalReturn->SetForegroundColour(this->DarkGrey);

	this->CashDisplay->SetForegroundColour(this->Green);
	this->MarketValueDisplay->SetForegroundColour(this->Green);
	this->AccountValueDisplay->SetForegroundColour(this->Green);

	SetStaticTextColor(*this->DayReturnDisplay, this->Green, this->Red);
	SetStaticTextColor(*this->DayReturnDollar, this->Green, this->Red);
	SetStaticTextColor(*this->TotalReturnDisplay, this->Green, this->Red);
	SetStaticTextColor(*this->TotalReturnDollar, this->Green, this->Red);
}

void PortfolioWin::DateChange(wxCommandEvent& evt)
{
	if (!this->datepcker)
		return;

	wxDateTime test;
	if (!test.ParseDate(this->datepcker->GetValue()))
	{
		this->datepcker->SetLabelText(wxDateTime::Today().Format(STANDARD_DATE));
		this->datepcker->Refresh();
		return;
	}

	if (test > wxDateTime::Today())
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
	wxString date = this->mainclock->Format(STANDARD_DATE);
	this->m_parent->OnDateChange(date);
}

void PortfolioWin::OnUpdateButtonPress(wxCommandEvent& evt)
{
	if (IsMarketOpen())
		this->m_parent->OnUpdate();
}

//DivCtrls functions...
DivCtrls::DivCtrls(wxDialog* parent, Dividend& d) : m_parent(parent), div(d)
{
	wxString empty = "";
	this->sizerH = new wxBoxSizer(wxHORIZONTAL);
	this->button_sizerH = new wxBoxSizer(wxHORIZONTAL);
	this->payment_sizerV = new wxBoxSizer(wxVERTICAL);
	this->ex_sizerV = new wxBoxSizer(wxVERTICAL);
	this->amount_sizerV = new wxBoxSizer(wxVERTICAL);
	this->reinvestment_sizerV = new wxBoxSizer(wxVERTICAL);

	this->ex_div_date = new wxTextCtrl(m_parent, wxID_ANY);
	this->payment_date = new wxTextCtrl(m_parent, wxID_ANY);
	this->amount = new wxTextCtrl(m_parent, wxID_ANY);
	this->reInvestmentShares = new wxTextCtrl(m_parent, wxID_ANY);

	this->_ex_div_date = div.ex_Div.Format(STANDARD_DATE);
	this->_payment_date = div.payment_date.IsValid() ? div.payment_date.Format(STANDARD_DATE) : empty;
	this->d_amount = div.div;
	this->d_reInvestmentShares = div.re_invest_shares;

	this->ex_div_date->SetValidator(wxTextValidator(wxFILTER_EMPTY, &this->_ex_div_date));
	this->payment_date->SetValidator(wxTextValidator(wxFILTER_NONE, &this->_payment_date));
	this->amount->SetValidator(wxFloatingPointValidator<double>(&this->d_amount,
		wxNUM_VAL_ZERO_AS_BLANK | wxFILTER_EMPTY | wxNUM_VAL_NO_TRAILING_ZEROES));
	this->reInvestmentShares->SetValidator(wxFloatingPointValidator<double>(&this->d_reInvestmentShares,
		wxNUM_VAL_ZERO_AS_BLANK | wxFILTER_EMPTY | wxNUM_VAL_NO_TRAILING_ZEROES));

	this->ex_div_date->Disable();
	this->payment_date->Disable();
	this->amount->Disable();
	this->reInvestmentShares->Disable();

	this->s_payment_date = new wxStaticText(m_parent, wxID_ANY, "Dividend Payment Date");
	this->s_ex_div_date = new wxStaticText(m_parent, wxID_ANY, "Ex-Dividend Date");
	this->s_amount = new wxStaticText(m_parent, wxID_ANY, "Dividend amount");
	this->s_reInvestmentShares = new wxStaticText(m_parent, wxID_ANY, "Re-Investment Shares");

	this->s_payment_date->SetFont(this->s_payment_date->GetFont().MakeBold());
	this->s_ex_div_date->SetFont(this->s_ex_div_date->GetFont().MakeBold());
	this->s_amount->SetFont(this->s_amount->GetFont().MakeBold());
	this->s_reInvestmentShares->SetFont(this->s_amount->GetFont().MakeBold());

	this->edit = new wxButton(m_parent, wxID_ANY, "Edit");
	this->ok = new wxButton(m_parent, wxID_OK, "OK");
	this->cancel = new wxButton(m_parent, wxID_CANCEL, "Cancel");

	this->payment_sizerV->Add(this->s_payment_date, 0, wxALIGN_LEFT | wxBOTTOM, 5);
	this->payment_sizerV->Add(this->payment_date, 0, wxALIGN_LEFT);
	this->ex_sizerV->Add(this->s_ex_div_date, 0, wxALIGN_LEFT | wxBOTTOM, 5);
	this->ex_sizerV->Add(this->ex_div_date, 0, wxALIGN_LEFT);
	this->amount_sizerV->Add(this->s_amount, 0, wxALIGN_LEFT | wxBOTTOM, 5);
	this->amount_sizerV->Add(this->amount, 0, wxALIGN_LEFT);
	this->reinvestment_sizerV->Add(this->s_reInvestmentShares, 0, wxALIGN_LEFT | wxBOTTOM | wxLEFT, 5);
	this->reinvestment_sizerV->Add(this->reInvestmentShares, 0, wxALIGN_LEFT | wxLEFT, 5);

	this->sizerH->Add(amount_sizerV, 1, wxALIGN_CENTER_VERTICAL);
	this->sizerH->Add(ex_sizerV, 1, wxALIGN_CENTER_VERTICAL);
	this->sizerH->Add(payment_sizerV, 1, wxALIGN_CENTER_VERTICAL);
	this->sizerH->AddSpacer(5);
	this->sizerH->Add(reinvestment_sizerV, 1, wxALIGN_CENTER_VERTICAL);

	this->button_sizerH->Add(this->edit, 0, wxRIGHT, 10);
	this->button_sizerH->Add(this->ok, 0, wxRIGHT, 10);
	this->button_sizerH->Add(this->cancel, 0);
}

wxBoxSizer*& DivCtrls::GetTopSizer()
{
	return this->sizerH;
}

wxBoxSizer*& DivCtrls::GetBottomSizer()
{
	return this->button_sizerH;
}

void DivCtrls::OnEdit()
{
	if (this->ex_div_date->IsEnabled())
	{
		this->ex_div_date->Disable();
		this->payment_date->Disable();
		this->amount->Disable();
		this->reInvestmentShares->Disable();
		this->payment_date->Disable();
	}
	else
	{
		this->ex_div_date->Enable();
		this->payment_date->Enable();
		this->amount->Enable();
		if (this->div.DivReinvest)
			this->reInvestmentShares->Enable();
		this->payment_date->SetFocus();
	}
}

template <typename T>
Dialog::Dialog(_EnterDialog type, mainwindow* parent, wxWindowID id, wxPoint p, wxSize s, wxString msg, T* t) : wxDialog(parent, id, msg, p, s), 
	m_parent(parent), m_type(type)
{
	this->Bind(wxEVT_CLOSE_WINDOW, &Dialog::OnCloseDialog, this);
	this->Bind(wxEVT_BUTTON, &Dialog::OnOkClick, this);

	switch (type)
	{
	case STOCK_PURCHASE_DIALOG: 
	{
		if (sizeof(wxString) == sizeof(*t))
			this->_ticker_ptr = reinterpret_cast<wxString*>(t);
		else
		{
			this->string_t = *reinterpret_cast<string_three*>(t);
			this->_ticker_ptr = &this->string_t.longname;
		}
		this->CreateStockEntry(); 
		break;
	}
	case _EnterDialog::SELL_STOCK: this->sellkit = reinterpret_cast<GenericKit*>(t); this->CreateSellStockWin(); break;
	case _EnterDialog::QUOTE_WIN: sumptr = reinterpret_cast<SummaryData*>(t); this->CreateQuoteWin(); break;
	case _EnterDialog::ENTER_DEPOSIT: this->CreateDepositWin(); break;
	case _EnterDialog::ENTER_DEPOSIT_SCHEDULE: this->CreateDepositScheduleWin(); break;
	case _EnterDialog::WITHDRAWL: this->CreateWithdrawlWin(); break;
	case _EnterDialog::DIVIDEND_ACTION_WIN: this->generic_kit_template = reinterpret_cast<GenericKitTemplate<Dividend, wxString>*>(t); this->CreateEditDivWin(); break;
	case _EnterDialog::ENTER_DIVIDEND: this->_ticker_ptr = reinterpret_cast<wxString*>(t); this->CreateAddDividendWin(); break;
	case _EnterDialog::DIVIDEND_SHARE_DIALOG: this->_ticker_ptr = reinterpret_cast<wxString*>(t); this->CreateAddReInvestShares(); break;
	case _EnterDialog::DAY_GAINERS_WIN: this->gainers = reinterpret_cast<wxVector<DayGainersandLosers>*>(t); this->CreateDayGainers_LosersWin(true); break;
	case _EnterDialog::DAY_LOSERS_WIN: this->gainers = reinterpret_cast<wxVector<DayGainersandLosers>*>(t); this->CreateDayGainers_LosersWin(false); break;
	case _EnterDialog::SUBSECTORWIN: this->sub = reinterpret_cast<wxVector<SubSector>*>(t); this->CreateSubSectorWin(); break;
	case _EnterDialog::CHART_VIEW_DATA_ENTRY: this->CreateChart_View_Data_Entry(); break;
	default: wxFAIL_MSG("_EnterDialog type does not match any types!"); this->Destroy();
	}
}

Dialog::~Dialog()
{
	if (this->divctrl)
		delete this->divctrl;

	this->divctrl = NULL;
}

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

double Dialog::GetReInvestShares()
{
	return this->_shares;
}

wxString Dialog::GetTicker()
{
	return this->_ticker;
}

int Dialog::GetDivChoice()
{
	return this->dividend_action;
}

Dividend Dialog::GetDividend()
{
	wxDateTime ex;
	wxDateTime paymentdate;

	ex.ParseFormat(this->divctrl->_ex_div_date);
	paymentdate.ParseDate(this->divctrl->_payment_date);
	Dividend div(0, ex, this->divctrl->d_amount);
	div.payment_date = paymentdate;
	div.re_invest_shares = this->divctrl->d_reInvestmentShares;

	return div;
}

void Dialog::GetChartViewData(wxString& ticker, wxDateTime& start, wxDateTime& end)
{
	ticker = this->_ticker;
	start = this->datetime;
	end = this->redatetime;
}

void Dialog::CreateStockEntry()
{
	wxColour c(205, 209, 206);
	SetColor(*this, wxColour(157, 157, 163));

	if (this->_ticker_ptr)
	{
		this->s_ticker = new wxStaticText(this, wxID_ANY, *this->_ticker_ptr);
		this->s_ticker->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	}

	this->CreateChoiceControls();

	this->sector_choice = new wxStaticText(this, wxID_ANY, "Choose Sector");
	wxStaticText* sub_sector = new wxStaticText(this, wxID_ANY, "Choose Industry");
	this->s_price = new wxStaticText(this, wxID_ANY, "Enter Price");
	this->m_price = new wxTextCtrl(this, wxID_ANY);
	this->m_price->SetValidator(wxFloatingPointValidator<double>(&this->_price, 
		wxNumValidatorStyle::wxNUM_VAL_ZERO_AS_BLANK | wxFILTER_EMPTY | wxNUM_VAL_NO_TRAILING_ZEROES));
	SetColor(*this->m_price, c);

	this->s_shares = new wxStaticText(this, wxID_ANY, "Enter Shares");
	this->m_shares = new wxTextCtrl(this, wxID_ANY);
	this->m_shares->SetValidator(wxFloatingPointValidator<double>(&this->_shares, 
		wxNUM_VAL_ZERO_AS_BLANK | wxFILTER_EMPTY | wxNUM_VAL_NO_TRAILING_ZEROES));
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
//	wxBoxSizer* tick = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sect = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* subsect = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* price = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* share = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* date = new wxBoxSizer(wxVERTICAL);

//	tick->Add(this->s_ticker, 0, wxALIGN_LEFT);
//	tick->Add(this->m_ticker, 0, wxALIGN_LEFT);

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

//	topH->Add(tick, 1, wxALIGN_CENTER_VERTICAL);
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

	mainV->Add(this->s_ticker, 0, wxALIGN_CENTER_HORIZONTAL);
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
	wxString empty = "";
	SetColor(*this, wxColour(207, 208, 212));

	wxBoxSizer* mainV = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* topHlongname = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* topVbutton = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* topH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* staticH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* staticH2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bottomH = new wxBoxSizer(wxHORIZONTAL);

	wxString dayreturn = wxNumberFormatter::ToString(sumptr->previousclose ? (sumptr->marketprice / sumptr->previousclose - 1) * 100 : 0.0, 2) + "%";
	wxString dayreturn$ = "($" + wxNumberFormatter::ToString(sumptr->marketprice - sumptr->previousclose, 2) + ")";
	wxString earningsdate = "Next Earnings: ";
	earningsdate += sumptr->earningsdate == "NotFound" ? empty : sumptr->earningsdate;

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

	staticH->Add(new wxStaticLine(this, wxID_ANY), 1, wxEXPAND);
	staticH2->Add(new wxStaticLine(this, wxID_ANY), 1, wxEXPAND);

	wxButton* B = new wxButton(this, wxID_OK, "Purchase");
	B->SetBackgroundColour(wxColour(2, 99, 23));

	topVbutton->Add(B, 1, wxBOTTOM | wxRIGHT | wxTOP | wxALIGN_RIGHT, 5);
	topHlongname->Add(longname, 1, wxBOTTOM | wxLEFT | wxTOP | wxALIGN_LEFT, 5);
	topHlongname->Add(topVbutton, 1, wxBOTTOM | wxLEFT | wxTOP | wxALIGN_LEFT, 5);

	topH->Add(curr_price, 0, wxALIGN_BOTTOM);
	topH->Add(_dayreturn, 0, wxLEFT | wxALIGN_BOTTOM, 5);
	topH->Add(_dayreturn$, 0, wxLEFT | wxALIGN_BOTTOM, 5);
	topH->AddSpacer(10);
	topH->Add(_earningsdate, 0, wxLEFT | wxRIGHT | wxALIGN_BOTTOM, 20);

	bottomH->Add(open, 1, wxALIGN_LEFT | wxLEFT, 10);
	bottomH->Add(previousclose, 1, wxALIGN_LEFT | wxLEFT, 15);

	mainV->Add(topHlongname, 0, wxBOTTOM);
//	mainV->Add(longname, 0, wxBOTTOM | wxLEFT |wxTOP | wxALIGN_LEFT, 5);
	mainV->Add(staticH, 0, wxEXPAND | wxALL, 5);
	mainV->Add(topH, 1, wxALIGN_LEFT | wxLEFT, 5);
	mainV->Add(staticH2, 0, wxEXPAND | wxALL, 5);
	mainV->Add(bottomH, 0, wxBOTTOM, 10);
	mainV->Add(beta, 0, wxBOTTOM | wxLEFT | wxALIGN_LEFT, 10);
//	mainV->Add(this->CreateSeparatedSizer(this->CreateTextSizer(Decipherhtmlcodes(sumptr->description))), 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	this->SetSizer(mainV);
	mainV->Fit(this);
	mainV->SetSizeHints(this);
	this->Layout();
	mainV->Add(this->CreateSeparatedSizer(this->CreateTextSizer(Decipherhtmlcodes(sumptr->description), B->GetPosition().x - longname->GetPosition().x)), 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
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
		ticker->Bind(wxEVT_ENTER_WINDOW, &Dialog::OnMouseEnter, this);
		ticker->Bind(wxEVT_LEAVE_WINDOW, &Dialog::OnMouseLeave, this);
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

void Dialog::CreateChart_View_Data_Entry()
{
	SetColor(*this, wxColour(207, 208, 212));
	wxColour green(5, 166, 10), red(201, 4, 27);

	wxBoxSizer* main = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* hor = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bottomH = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* title = new wxStaticText(this, wxID_ANY, "Enter Ticker and Date range to view Chart");
	title->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));

	this->s_ticker = new wxStaticText(this, wxID_ANY, "Enter Ticker");
	this->m_ticker = new wxTextCtrl(this, wxID_ANY);
	this->m_ticker->SetValidator(wxTextValidator(wxFILTER_ALPHA | wxFILTER_EMPTY, &this->_ticker));
	this->m_ticker->GetValidator()->Bind(wxEVT_KEY_DOWN, &Dialog::OnKeyDown, this);

	this->s_date = new wxStaticText(this, wxID_ANY, "Start Date");
	this->m_date = new wxTextCtrl(this, wxID_ANY);
	this->m_date->SetValidator(wxTextValidator(wxFILTER_EMPTY, &this->_date));

	this->_reinvest_date = wxDateTime::Today().Format(STANDARD_DATE);
	this->s_reinvest_date = new wxStaticText(this, wxID_ANY, "End Date");
	this->m_reinvest_date = new wxTextCtrl(this, wxID_ANY);
	this->m_reinvest_date->SetValidator(wxTextValidator(wxFILTER_NONE, &this->_reinvest_date));

	wxButton* ok = new wxButton(this, wxID_OK, "OK");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "CANCEL");

	ok->Bind(wxEVT_BUTTON, &Dialog::OnOkClick, this);
	cancel->Bind(wxEVT_BUTTON, &Dialog::OnCancelDialog, this);

	hor->Add(this->s_ticker, 0, wxALIGN_CENTER_VERTICAL);
	hor->AddSpacer(5);
	hor->Add(this->m_ticker, 0, wxALIGN_CENTER_VERTICAL);

	hor->AddSpacer(20);
	hor->Add(this->s_date, 0, wxALIGN_CENTER_VERTICAL);
	hor->AddSpacer(5);
	hor->Add(this->m_date, 0, wxALIGN_CENTER_VERTICAL);

	hor->AddSpacer(20);
	hor->Add(this->s_reinvest_date, 0, wxALIGN_CENTER_VERTICAL);
	hor->AddSpacer(5);
	hor->Add(this->m_reinvest_date, 0, wxALIGN_CENTER_VERTICAL);

	bottomH->Add(ok, 0, wxRIGHT, 10);
	bottomH->Add(cancel, 0);

	main->Add(title, 1, wxALIGN_CENTER_HORIZONTAL | wxTOP, 10);
	main->AddSpacer(10);
	main->Add(hor, 1, wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxRIGHT, 5);
	main->AddSpacer(5);
	main->Add(bottomH, 0, wxALIGN_RIGHT | wxBOTTOM | wxRIGHT, 5);

	this->SetSizer(main);
	main->Fit(this);
	main->SetSizeHints(this);
	this->Layout();
	this->CenterOnParent();
}

void Dialog::CreateSubSectorWin()
{
	SetColor(*this, wxColour(207, 208, 212));
	wxColour green(5, 166, 10), red(201, 4, 27);

//	wxGridSizer* grid = new wxGridSizer(6, 5, 5);
	wxGridSizer* grid = new wxGridSizer(6, wxSize(4, 4));

	wxStaticText* _sectorname = new wxStaticText(this, wxID_ANY, "Sector Name");
	wxStaticText* _dayreturn = new wxStaticText(this, wxID_ANY, "Day Return");
	wxStaticText* _weekreturn = new wxStaticText(this, wxID_ANY, "Week Return");
	wxStaticText* _monthreturn = new wxStaticText(this, wxID_ANY, "Month Return");
	wxStaticText* _quarterreturn = new wxStaticText(this, wxID_ANY, "Quarter Return");
	wxStaticText* _ytdreturn = new wxStaticText(this, wxID_ANY, "YTD Return");

	_sectorname->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	_dayreturn->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	_weekreturn->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	_monthreturn->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	_quarterreturn->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	_ytdreturn->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));

	grid->Add(_sectorname, 1, wxALIGN_LEFT | wxLEFT, 5);
	grid->Add(_dayreturn, 0, wxALIGN_LEFT | wxLEFT, 10);
	grid->Add(_weekreturn, 0, wxALIGN_LEFT | wxLEFT, 10);
	grid->Add(_monthreturn, 0, wxALIGN_LEFT | wxLEFT, 10);
	grid->Add(_quarterreturn, 0, wxALIGN_LEFT | wxLEFT, 10);
	grid->Add(_ytdreturn, 0, wxALIGN_LEFT | wxLEFT, 10);

	for (auto it = this->sub->begin(); it != this->sub->end(); ++it)
	{
		wxString sectorname = it->GetSectorName();
		wxStaticText* ticker = new wxStaticText(this, wxID_ANY, Decipherhtmlcodes(sectorname));
		ticker->SetForegroundColour(wxColour(17, 121, 212));

		wxStaticText* dayreturn = new wxStaticText(this, wxID_ANY, it->GetDayReturnString());
		wxStaticText* weekreturn = new wxStaticText(this, wxID_ANY, it->GetWeekReturnString());
		wxStaticText* monthreturn = new wxStaticText(this, wxID_ANY, it->GetMonthReturnString());
		wxStaticText* quarterreturn = new wxStaticText(this, wxID_ANY, it->GetQuarterReturnString());
		wxStaticText* ytdreturn = new wxStaticText(this, wxID_ANY, it->GetYTDReturnString());

		ticker->SetFont(wxFont(12, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
		dayreturn->SetFont(wxFont(12, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_SEMIBOLD));
		weekreturn->SetFont(wxFont(12, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_SEMIBOLD));
		monthreturn->SetFont(wxFont(12, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_SEMIBOLD));
		quarterreturn->SetFont(wxFont(12, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_SEMIBOLD));
		ytdreturn->SetFont(wxFont(12, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_SEMIBOLD));

		SetStaticTextColor(*dayreturn, green, red);
		SetStaticTextColor(*weekreturn, green, red);
		SetStaticTextColor(*monthreturn, green, red);
		SetStaticTextColor(*quarterreturn, green, red);
		SetStaticTextColor(*ytdreturn, green, red);

		grid->Add(ticker, 1, wxALIGN_LEFT | wxLEFT, 5);
		grid->Add(dayreturn, 0, wxALIGN_LEFT | wxLEFT, 10);
		grid->Add(weekreturn, 0, wxALIGN_LEFT | wxLEFT, 10);
		grid->Add(monthreturn, 0, wxALIGN_LEFT | wxLEFT, 10);
		grid->Add(quarterreturn, 0, wxALIGN_LEFT | wxLEFT, 10);
		grid->Add(ytdreturn, 0, wxALIGN_LEFT | wxLEFT, 10);
	}

	this->SetSizer(grid);
	grid->Fit(this);
	grid->SetSizeHints(this);
	this->Layout();
	this->CenterOnParent();

	this->ShowModal();
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

	this->s_ticker = new wxStaticText(this, wxID_ANY, *this->_ticker_ptr);
	this->s_ticker->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));

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

	topH->Add(s_date, 0, wxALIGN_CENTER_VERTICAL);
	topH->AddSpacer(5);
	topH->Add(m_date, 0, wxALIGN_CENTER_VERTICAL);
	topH->AddSpacer(20);
	topH->Add(s_price, 0, wxALIGN_CENTER_VERTICAL);
	topH->AddSpacer(5);
	topH->Add(m_price, 0, wxALIGN_CENTER_VERTICAL);

	bottomH->Add(ok, 0, wxRIGHT, 10);
	bottomH->Add(cancel, 0);

	mainV->Add(this->s_ticker, 1, wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxRIGHT, 10);
	mainV->Add(topH, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	mainV->Add(bottomH, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

	this->m_date->SetFocus();
	this->SetSizer(mainV);
	mainV->Fit(this);
	mainV->SetSizeHints(this);
	this->Layout();
	this->CenterOnParent();

//	this->ShowModal();
}

void Dialog::CreateAddReInvestShares()
{
	wxColour c(205, 209, 206);
	SetColor(*this, wxColour(157, 157, 163));

	wxBoxSizer* mainV = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* topH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bottomH = new wxBoxSizer(wxHORIZONTAL);

	this->s_ticker = new wxStaticText(this, wxID_ANY, *this->_ticker_ptr);
	this->s_ticker->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	//	this->m_ticker = new wxTextCtrl(this, wxID_ANY);
	//	this->m_ticker->SetValidator(wxTextValidator(wxFILTER_EMPTY, &this->_ticker));
	//	this->m_ticker->GetValidator()->Bind(wxEVT_KEY_DOWN, &Dialog::OnKeyDown, this);
	//	SetColor(*this->m_ticker, c);

	this->s_shares = new wxStaticText(this, wxID_ANY, "Shares:");
	this->m_shares = new wxTextCtrl(this, wxID_ANY);
	this->m_shares->SetValidator(wxFloatingPointValidator<double>(&this->_shares, wxNumValidatorStyle::wxNUM_VAL_ZERO_AS_BLANK | wxFILTER_EMPTY));
	SetColor(*this->m_shares, c);

	wxButton* ok = new wxButton(this, wxID_OK, "OK");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "CANCEL");

	ok->Bind(wxEVT_BUTTON, &Dialog::OnOkClick, this);
	cancel->Bind(wxEVT_BUTTON, &Dialog::OnCancelDialog, this);

	///	topH->Add(s_ticker, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	//	topH->Add(m_ticker, 0, wxALIGN_CENTER_VERTICAL);
	//	topH->AddSpacer(30);
	topH->Add(s_shares, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	topH->Add(m_shares, 0, wxALIGN_CENTER_VERTICAL);

	bottomH->Add(ok, 0, wxRIGHT, 10);
	bottomH->Add(cancel, 0);

	mainV->Add(this->s_ticker, 1, wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxRIGHT, 10);
	mainV->Add(topH, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	mainV->Add(bottomH, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

	this->SetSizer(mainV);
	mainV->Fit(this);
	mainV->SetSizeHints(this);
	this->Layout();
	this->CenterOnParent();
}

void Dialog::CreateDividendActionWin()
{
	wxColour c(205, 209, 206);
	SetColor(*this, wxColour(157, 157, 163));

	wxBoxSizer* mainV = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* topH = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bottomH = new wxBoxSizer(wxHORIZONTAL);

	Dividend& div = this->generic_kit_template->thing1;
	this->s_ticker = new wxStaticText(this, wxID_ANY, this->generic_kit_template->thing2);
	this->s_ticker->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));

	wxStaticText* ex_div_desc = new wxStaticText(this, wxID_ANY, "Ex-Div-Date:");
	wxStaticText* ex_div = new wxStaticText(this, wxID_ANY, div.ex_Div.Format(STANDARD_DATE));
	wxStaticText* amount_desc = new wxStaticText(this, wxID_ANY, "Dividend Amount:");
	wxStaticText* amount = new wxStaticText(this, wxID_ANY, wxNumberFormatter::ToString(div.div, 3));

	ex_div_desc->SetFont(ex_div_desc->GetFont().MakeBold());
	ex_div->SetFont(ex_div->GetFont().MakeBold());
	amount_desc->SetFont(amount_desc->GetFont().MakeBold());
	amount->SetFont(amount->GetFont().MakeBold());

	wxArrayString string;
	string.Add("Remove Dividend");
	string.Add("Add Payment Date");
	string.Add("Change Dividend Amount");
	this->choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, string);
	this->choice->SetStringSelection("Remove Dividend");
	this->choice->Bind(wxEVT_CHOICE, &Dialog::OnDividendChoice, this);
	this->dividend_action = REMOVE_DIVIDEND;

	wxButton* ok = new wxButton(this, wxID_OK, "OK");
	wxButton* cancel = new wxButton(this, wxID_CANCEL, "CANCEL");

	ok->Bind(wxEVT_BUTTON, &Dialog::OnOkClick, this);
	cancel->Bind(wxEVT_BUTTON, &Dialog::OnCancelDialog, this);

	bottomH->Add(ok, 0, wxRIGHT, 10);
	bottomH->Add(cancel, 0);

	topH->Add(ex_div_desc, 0, wxRIGHT, 5);
	topH->Add(ex_div, 0, wxRIGHT, 20);
	topH->Add(amount_desc, 0, wxRIGHT, 5);
	topH->Add(amount, 0, wxRIGHT, 5);

	mainV->Add(this->s_ticker, 1, wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxRIGHT, 10);
	mainV->Add(topH, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	mainV->Add(choice, 0, wxALIGN_CENTER_HORIZONTAL);
	mainV->AddSpacer(10);
	mainV->Add(bottomH, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

	this->SetSizer(mainV);
	mainV->Fit(this);
	mainV->SetSizeHints(this);
	this->Layout();
	this->CenterOnParent();
}

void Dialog::CreateEditDivWin()
{
	wxColour c(205, 209, 206);
	SetColor(*this, wxColour(157, 157, 163));

	wxBoxSizer* mainV = new wxBoxSizer(wxVERTICAL);

	Dividend& div = this->generic_kit_template->thing1;
	this->s_ticker = new wxStaticText(this, wxID_ANY, this->generic_kit_template->thing2);
	this->s_ticker->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));

	this->divctrl = new DivCtrls(this, div);

	this->divctrl->edit->Bind(wxEVT_BUTTON, &Dialog::OnEditClick, this);
	this->divctrl->ok->Bind(wxEVT_BUTTON, &Dialog::OnOkClick, this);
	this->divctrl->cancel->Bind(wxEVT_BUTTON, &Dialog::OnCancelDialog, this);

	mainV->Add(this->s_ticker, 1, wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxRIGHT, 10);
	mainV->Add(this->divctrl->GetTopSizer(), 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	mainV->Add(this->divctrl->GetBottomSizer(), 0, wxALIGN_RIGHT | wxALL, 10);

	this->SetSizer(mainV);
	mainV->Fit(this);
	mainV->SetSizeHints(this);
	this->Layout();
	this->CenterOnParent();
}

void Dialog::CreateChoiceControls()
{
	// Create choice controls...
	this->choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	this->subSector = new wxChoice(this, wxID_ANY);

	// If Dialog was called with string_three struct then we can skip all this because the user is buying a stock they
	// Already purchased before. Which means the sector does not have to be selected. We already know it.
	// There for we will fill in the choice control with the sector sent int the string_three struct and disable it...
	if (&this->string_t.longname == this->_ticker_ptr)
	{
		this->choice->Append(this->string_t.sector);
		this->choice->SetSelection(0);
		this->subSector->Append("Exchange Traded Fund");
		this->subSector->SetSelection(0);
		this->choice->Disable();
		this->subSector->Disable();
		return;
	}

	// Bind choice to event OnSectorChoice...
	this->choice->Bind(wxEVT_CHOICE, &Dialog::OnSectorChoice, this);

	const wxVector<_Sector>* scptr = GetSectorClass().GetSector();
	const wxVector<wxString>* scptrs = GetSectorClass().GetSectorString();

	for (size_t i = 0; i < scptrs->size() - 1; ++i)
		this->choice->Append(scptrs->at(i));

	this->choice->SetStringSelection(scptrs->at(8 + 5));

	const wxVector<wxString>* subvec = GetSectorClass().GetSubSectorString(scptr->at(8 + 5));
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
	m_parent->DialogCancel();
	this->Destroy();
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
	case _EnterDialog::DIVIDEND_ACTION_WIN: 
	{
		if (!this->OnEditDivWinOK())
			return;
		evt.Skip(); 
		return;
	}
	case _EnterDialog::ENTER_DIVIDEND: this->HandleAddDividendOkClick(); break;
	case _EnterDialog::DIVIDEND_SHARE_DIALOG: 
	{
		if (!this->HandleReInvestSharesOkClick())
			return;
		evt.Skip();
		break;
	}
	case _EnterDialog::WITHDRAWL: 
	{
		if (this->HandleWithdrawlOkClick())
			evt.Skip();
		return;
	}
	case _EnterDialog::CHART_VIEW_DATA_ENTRY:
	{
		if (this->HandleChartViewDataEntry())
			evt.Skip();
		return;
	}
	}
}

void Dialog::OnSectorChoice(wxCommandEvent& evt)
{
	wxString sector = this->choice->GetStringSelection();
	int type = GetSectorClass().GetSectorID(sector);
	if (type <= FOREIGN)
	{
		this->subSector->Clear();
		this->subSector->Append(_ETF);
		this->subSector->SetStringSelection(_ETF);
		return;
	}

//	const wxVector<wxString>* sub = GetSectorClass().GetSubSectorString(_Sector(this->choice->GetSelection() + *GetSectorClass().GetSector()->begin() + 8));
	const wxVector<wxString>* sub = GetSectorClass().GetSubSectorString(_Sector(type));
	this->subSector->Clear();

	if (!sub->size())
		return;

	for (size_t i = 0; i < sub->size(); ++i)
		this->subSector->Append(sub->at(i));

	this->subSector->SetStringSelection(*sub->begin());
}

void Dialog::OnDividendChoice(wxCommandEvent& evt)
{
	wxString selection = this->choice->GetStringSelection();
	if (selection == "Remove Dividend")
		this->dividend_action = REMOVE_DIVIDEND;
	if (selection == "Add Payment Date")
		this->dividend_action = ADD_DIV_PAYMENT_DATE;
	if (selection == "Change Dividend Amount")
		this->dividend_action = ADJUST_DIV_AMOUNT;
}

void Dialog::OnEditClick(wxCommandEvent& evt)
{
	if (!this->divctrl)
	{
		wxFAIL_MSG("this->divctrl is NUll in Dialog::OnEditClick!");
		return;
	}

	this->divctrl->OnEdit();
}

void Dialog::OnKeyDown(wxKeyEvent& evt)
{
	int n = evt.GetKeyCode();
	wxTextEntry* t = NULL;
	if (this->m_ticker)
	{
		wxTextValidator* text = (wxTextValidator*)this->m_ticker->GetValidator();
		t = text ? text->GetTextEntry() : NULL;
	}
	if (t)
		t->ForceUpper();
	evt.Skip();
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

void Dialog::OnMouseEnter(wxMouseEvent& evt)
{
	wxStaticText* T = (wxStaticText*)this->FindItem(evt.GetId());
	if (!T)
	{
		wxFAIL_MSG("Could not find item in Dialog::OnMouseEnter!");
		return;
	}

	T->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_EXTRABOLD));
	this->Layout();
}

void Dialog::OnMouseLeave(wxMouseEvent& evt)
{
	wxStaticText* T = (wxStaticText*)this->FindItem(evt.GetId());
	if (!T)
	{
		wxFAIL_MSG("Could not find item in Dialog::OnMouseLeave!");
		return;
	}

	T->SetFont(wxFont(12, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD));
	this->Layout();
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

bool Dialog::HandleReInvestSharesOkClick()
{
	if (!this->_shares)
	{
		wxMessageBox("You cannot enter zero shares!");
		return false;
	}

	return true;
}

bool Dialog::HandleChartViewDataEntry()
{
	
	if (!this->datetime.ParseDate(this->_date))
	{
		wxMessageBox("You entered an invalid start date range!");
		return false;
	}

	if (!this->redatetime.ParseDate(this->_reinvest_date))
	{
		wxMessageBox("You entered an invalid end date range!");
		return false;
	}

	if (this->redatetime < this->datetime)
	{
		wxMessageBox("Your end date is before your start date!");
		return false;
	}

	if (this->datetime == this->redatetime)
	{
		wxMessageBox("Your dates are the same day!");
		return false;
	}


	return true;
}

bool Dialog::OnEditDivWinOK()
{
	if (IsNull(this->divctrl, "this->divctrl is NULL in Dialog::OnEditDivWinOK!"))
		return false;

	Dividend& div = this->generic_kit_template->thing1;

	wxDateTime valid;
	if (!valid.ParseDate(this->divctrl->_ex_div_date))
	{
		wxMessageBox("Invalid date in Ex-Dividend Date!");
		return false;
	}

	if (!this->divctrl->_payment_date.IsEmpty())
	{
		if (!valid.ParseDate(this->divctrl->_payment_date))
		{
			wxMessageBox("Invalid date in Dividend Payment Date!");
			return false;
		}
		else if (div.payment_date.IsValid())
		{
			if (valid <= div.ex_Div)
			{
				wxMessageBox("You are adding a payment date of " + this->divctrl->_payment_date + " which is\nless than or equal to the Ex Dividend Date of " 
					+ div.ex_Div.Format(STANDARD_DATE));
				return false;
			}
		}

		return true;
	}

	return true;
}

// SellStockWin functions...
GenericListWin::GenericListWin(mainwindow* m, wxWindowID id, wxString msg, wxString caption, wxArrayString string) :
	wxSingleChoiceDialog(m, msg, caption, string), m_parent(m)
{

}

// BottomFrame functions...
BottomFrame::BottomFrame(mainwindow* m, wxWindowID id) : wxWindow(m, id), m_parent(m)
{
	this->Create();
}

void BottomFrame::OnMouseEnter(wxMouseEvent& evt)
{
	this->textenter = NULL;
	switch (evt.GetId())
	{
	case _Sector::MATERIALS: textenter = this->materials; break;
	case _Sector::COMMUNICATION_SERVICES: textenter = this->comm_services; break;
	case _Sector::CONSUMER_DESCRETIONARY: textenter = this->cons_cycl; break;
	case _Sector::CONSUMER_STAPLES: textenter = this->cons_def; break;
	case _Sector::ENERGY: textenter = this->energy; break;
	case _Sector::FINANCIALS: textenter = this->finance; break;
	case _Sector::HEALTH_CARE: textenter = this->healthcare; break;
	case _Sector::INDUSTRIALS: textenter = this->industrial; break;
	case _Sector::REAL_ESTATE: textenter = this->real_estate; break;
	case _Sector::INFORMATION_TECHNOLOGY: textenter = this->tech; break;
	case _Sector::UTILITIES: textenter = this->utilities; break;
	}
	if (this->textenter)
	{
		this->textenter->SetBackgroundColour(wxColour(179, 4, 27));
		this->textenter->Refresh();
	}
}

void BottomFrame::OnMouseLeave(wxMouseEvent& evt)
{
	if (this->textenter)
	{
		this->textenter->SetBackgroundColour(this->GetBackgroundColour());
		this->textenter->Refresh();
	}
	this->textenter = NULL;
}

void BottomFrame::OnMouseDown(wxMouseEvent& evt)
{
	wxString temp = "Sector";
	int id = evt.GetId();
	SectorClass& sc = GetSectorClass();
	ParentSector* ps = sc.GetSector(_Sector(id));
	wxString name = ps ? ps->GetSectorName() : temp;

	wxVector<SubSector>* v = NULL;
	switch (evt.GetId())
	{
	case _Sector::MATERIALS: v = sc.GetSubSectorVec(_Sector::MATERIALS);  break;
	case _Sector::COMMUNICATION_SERVICES: v = sc.GetSubSectorVec(_Sector::COMMUNICATION_SERVICES); break;
	case _Sector::CONSUMER_DESCRETIONARY: v = sc.GetSubSectorVec(_Sector::CONSUMER_DESCRETIONARY); break;
	case _Sector::CONSUMER_STAPLES: v = sc.GetSubSectorVec(_Sector::CONSUMER_STAPLES); break;
	case _Sector::ENERGY: v = sc.GetSubSectorVec(_Sector::ENERGY); break;
	case _Sector::FINANCIALS: v = sc.GetSubSectorVec(_Sector::FINANCIALS); break;
	case _Sector::HEALTH_CARE: v = sc.GetSubSectorVec(_Sector::HEALTH_CARE); break;
	case _Sector::INDUSTRIALS: v = sc.GetSubSectorVec(_Sector::INDUSTRIALS); break;
	case _Sector::REAL_ESTATE: v = sc.GetSubSectorVec(_Sector::REAL_ESTATE); break;
	case _Sector::INFORMATION_TECHNOLOGY: v = sc.GetSubSectorVec(_Sector::INFORMATION_TECHNOLOGY); break;
	case _Sector::UTILITIES: v = sc.GetSubSectorVec(_Sector::UTILITIES); break;
	}

	if (v != NULL)
		this->m_parent->SectorClick(v, name);
	else
	{
		wxString Id = "";
		Id << id;
		wxFAIL_MSG("Switch failed in BottomFrame::OnMouseDown! No matching number for: " + Id);
	}
}

void BottomFrame::OnMouseRightDown(wxMouseEvent& evt)
{
	wxString temp = "Sector";
	int id = evt.GetId();
	SectorClass& sc = GetSectorClass();
	ParentSector* ps = sc.GetSector(_Sector(id));
	wxString name = ps ? ps->GetSectorName() : temp;

	wxVector<SectorStock>* v = ps ? ps->GetSectorStockVec() : NULL;

	if (v != NULL)
		this->m_parent->SectorRightClick(v, name);
	else
	{
		wxString Id = "";
		Id << id;
		wxFAIL_MSG("Switch failed in BottomFrame::OnMouseDown! No matching number for: " + Id);
	}
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
	this->tech = new wxStaticText(this, _Sector::INFORMATION_TECHNOLOGY, "Technology");
	this->utilities = new wxStaticText(this, _Sector::UTILITIES, "Utilities");

	this->BindEvents(*this->materials);
	this->BindEvents(*this->comm_services);
	this->BindEvents(*this->cons_cycl);
	this->BindEvents(*this->cons_def);
	this->BindEvents(*this->energy);
	this->BindEvents(*this->finance);
	this->BindEvents(*this->healthcare);
	this->BindEvents(*this->industrial);
	this->BindEvents(*this->real_estate);
	this->BindEvents(*this->tech);
	this->BindEvents(*this->utilities);

	int size = 5;
	int sectors = 11;
	wxBoxSizer* mainH = new wxBoxSizer(wxHORIZONTAL);

	for (int j = 0; j < sectors; ++j)
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
		for (int i = 0; i < size; ++i)
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

void BottomFrame::BindEvents(wxStaticText& t)
{
	t.Bind(wxEVT_ENTER_WINDOW, &BottomFrame::OnMouseEnter, this);
	t.Bind(wxEVT_LEAVE_WINDOW, &BottomFrame::OnMouseLeave, this);
	t.Bind(wxEVT_LEFT_DOWN, &BottomFrame::OnMouseDown, this);
	t.Bind(wxEVT_RIGHT_DOWN, &BottomFrame::OnMouseRightDown, this);
}

// SectorStockWindow Functions...
SectorStockWindow::SectorStockWindow(mainwindow* m, wxVector<SectorStock>* v, wxString& s) : wxFrame(m, wxID_ANY, s),
	m_parent(m), myvec(v), sectorname(s)
{
	std::sort(myvec->begin(), myvec->end(), [](SectorStock& first, SectorStock& second) {
		return first.d_change_perc > second.d_change_perc;
		});
	this->Bind(wxEVT_CLOSE_WINDOW, &SectorStockWindow::CloseEvent, this);
	this->Create();
}

void SectorStockWindow::CloseEvent(wxCloseEvent& evt)
{
	this->m_parent->Enable();
	evt.Skip();
}

void SectorStockWindow::ScrollBottom(wxScrollWinEvent& evt)
{
	int orientation = evt.GetOrientation();
	
	int x = 0, y = 0;
	P->GetScrollPixelsPerUnit(&x, &y);
	
	int ysize = P->GetScrollHelper()->GetScrollPageSize(wxVERTICAL);
	int position = P->GetScrollPos(orientation) + ysize;

	int scrolled = (y * position);
	wxPoint point = this->lastticker->GetPosition();
	wxPoint unscrolledpoint = P->CalcUnscrolledPosition(point);
	if (unscrolledpoint.y > scrolled)
	{
		evt.Skip();
		return;
	}

	wxColour green = wxColour(5, 166, 10), red = wxColour(201, 4, 27);
	wxFont plain = wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD);

	int i = 0;
	if (it == myvec->end())
	{
		evt.Skip();
		return;
	}

	P->Hide();
	for (this->it; it != this->myvec->end(); ++it)
	{
		if (i > 100)
			break;
		wxStaticText* temp = new wxStaticText(P, wxID_ANY, it->ticker);
		temp->SetFont(plain);
		this->vert1->Add(temp, 0, wxALIGN_LEFT | wxALL, 5);
		this->lastticker = temp;

		wxStaticText* price = new wxStaticText(P, wxID_ANY, it->price);
		price->SetFont(plain);
		this->vert2->Add(price, 0, wxALIGN_LEFT | wxALL, 5);

		wxStaticText* change = new wxStaticText(P, wxID_ANY, it->change);
		change->SetFont(plain);
		this->vert3->Add(change, 0, wxALIGN_LEFT | wxALL, 5);
		SetStaticTextColor(*change, green, red);

		wxStaticText* percent_change = new wxStaticText(P, wxID_ANY, it->percent_change + '%');
		percent_change->SetFont(plain);
		this->vert4->Add(percent_change, 0, wxALIGN_LEFT | wxALL, 5);
		SetStaticTextColor(*percent_change, green, red);

		wxStaticText* market_cap = new wxStaticText(P, wxID_ANY, it->market_cap);
		market_cap->SetFont(plain);
		this->vert5->Add(market_cap, 1, wxALIGN_LEFT | wxALL, 5);
		++i;
	}

	P->Show();
	this->Layout();
	this->Refresh();
}

void SectorStockWindow::Create()
{
	wxColour green = wxColour(5, 166, 10), red = wxColour(201, 4, 27);
	wxFont plain = wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD);
	wxFont title = wxFont(20, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD);

	P = new wxScrolled<wxWindow>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	P->SetScrollbars(25, 25, 50, 50);
	P->EnableScrolling(true, true);

	P->Bind(wxEVT_SCROLLWIN_BOTTOM, &SectorStockWindow::ScrollBottom, this);
	P->Bind(wxEVT_SCROLLWIN_TOP, &SectorStockWindow::ScrollBottom, this);
	P->Bind(wxEVT_SCROLLWIN_LINEDOWN, &SectorStockWindow::ScrollBottom, this);
	P->Bind(wxEVT_SCROLLWIN_LINEUP, &SectorStockWindow::ScrollBottom, this);
	P->Bind(wxEVT_SCROLLWIN_PAGEDOWN, &SectorStockWindow::ScrollBottom, this);
	P->Bind(wxEVT_SCROLLWIN_PAGEUP, &SectorStockWindow::ScrollBottom, this);
	P->Bind(wxEVT_SCROLLWIN_THUMBRELEASE, &SectorStockWindow::ScrollBottom, this);
	P->Bind(wxEVT_SCROLLWIN_THUMBTRACK, &SectorStockWindow::ScrollBottom, this);

	P->SetMinSize(wxSize(wxGetDisplaySize().x / 2, wxGetDisplaySize().y / 2));
	wxBoxSizer* scrmain = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* main = new wxBoxSizer(wxVERTICAL);

	this->vert1 = new wxBoxSizer(wxVERTICAL);
	this->vert2 = new wxBoxSizer(wxVERTICAL);
	this->vert3 = new wxBoxSizer(wxVERTICAL);
	this->vert4 = new wxBoxSizer(wxVERTICAL);
	this->vert5 = new wxBoxSizer(wxVERTICAL);

	wxStaticText* sectortext = new wxStaticText(this, wxID_ANY, this->sectorname);
	sectortext->SetFont(title);

	main->Add(sectortext, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

	int i = 0;
	for (this->it = this->myvec->begin(); it != this->myvec->end(); ++it)
	{
		if (i > 100)
			break;
		if (it == this->myvec->begin())
		{
			wxStaticText* t = new wxStaticText(P, wxID_ANY, "Ticker");
			t->SetFont(plain);
			this->vert1->Add(t, 0, wxALIGN_LEFT | wxALL, 5);

			wxStaticText* p = new wxStaticText(P, wxID_ANY, "Price");
			p->SetFont(plain);
			this->vert2->Add(p, 0, wxALIGN_LEFT | wxALL, 5);

			wxStaticText* c = new wxStaticText(P, wxID_ANY, "Change");
			c->SetFont(plain);
			this->vert3->Add(c, 0, wxALIGN_LEFT | wxALL, 5);

			wxStaticText* pc = new wxStaticText(P, wxID_ANY, "Percent Change");
			pc->SetFont(plain);
			this->vert4->Add(pc, 0, wxALIGN_LEFT | wxALL, 5);

			wxStaticText* mc = new wxStaticText(P, wxID_ANY, "Market Cap");
			mc->SetFont(plain);
			this->vert5->Add(mc, 0, wxALIGN_LEFT | wxALL, 5);
		}

		wxStaticText* temp = new wxStaticText(P, wxID_ANY, it->ticker);
		temp->SetFont(plain);
		this->vert1->Add(temp, 0, wxALIGN_LEFT | wxALL, 5);
		this->lastticker = temp;

		wxStaticText* price = new wxStaticText(P, wxID_ANY, it->price);
		price->SetFont(plain);
		this->vert2->Add(price, 0, wxALIGN_LEFT | wxALL, 5);

		wxStaticText* change = new wxStaticText(P, wxID_ANY, it->change);
		change->SetFont(plain);
		this->vert3->Add(change, 0, wxALIGN_LEFT | wxALL, 5);
		SetStaticTextColor(*change, green, red);

		wxStaticText* percent_change = new wxStaticText(P, wxID_ANY, it->percent_change + '%');
		percent_change->SetFont(plain);
		this->vert4->Add(percent_change, 0, wxALIGN_LEFT | wxALL, 5);
		SetStaticTextColor(*percent_change, green, red);

		wxStaticText* market_cap = new wxStaticText(P, wxID_ANY, it->market_cap);
		market_cap->SetFont(plain);
		this->vert5->Add(market_cap, 1, wxALIGN_LEFT | wxALL, 5);
		++i;
	}

	scrmain->Add(vert1, 1, wxALIGN_CENTER_VERTICAL);
	scrmain->Add(vert2, 1, wxALIGN_CENTER_VERTICAL);
	scrmain->Add(vert3, 1, wxALIGN_CENTER_VERTICAL);
	scrmain->Add(vert4, 1, wxALIGN_CENTER_VERTICAL);
	scrmain->Add(vert5, 1, wxALIGN_CENTER_VERTICAL);

	P->SetSizer(scrmain);
	scrmain->Fit(P);
	main->Add(P, 4, wxEXPAND | wxALL, 10);

	this->SetSizer(main);
	main->Fit(this);
	this->CenterOnParent();
	this->Layout();
	this->Show();
}

// mainwindow functions...

mainwindow::mainwindow() : wxFrame(nullptr, 10000, "Brando's Investments", wxPoint(0, 0), wxSize(wxGetDisplaySize())), main_clock(wxDateTime::Today()),
	 port(this, 0, &main_clock)
{
	wxSize displaysize = wxGetDisplaySize();
	displaysize.y += -50;
	this->SetSize(displaysize);
	this->SetBackgroundColour(wxColour(0, 0, 0));
	SectorClass& sc = GetSectorClass();
	sc.ActivateData();
	this->CreatePopupMenu();
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
	deposits->Append(new wxMenuItem(deposits, VIEW_DEPOSITS, "View Deposits"));
	deposits->Append(new wxMenuItem(deposits, NEW_DEPOSIT_SCHEDULE, "Add Deposit Schedule"));
	divs->Append(new wxMenuItem(divs, ENTER_DIVIDEND, "Add Dividend"));
	divs->Append(new wxMenuItem(divs, VIEW_DIVIDENDS, "View Dividend List"));
	divs->Append(new wxMenuItem(divs, ADD_DIV_SHARES, "Re-Investment Shares"));

	m->Append(new wxMenuItem(m, wxID_SAVE, "Save"));
	m->Append(new wxMenuItem(m, wxID_EXECUTE, "Load Stock Data"));
	markets->Append(new wxMenuItem(markets, QUOTE_LOOKUP, "Quote Lookup"));
	markets->Append(new wxMenuItem(markets, DAY_GAINERS_MENU, "Market Gainers"));
	markets->Append(new wxMenuItem(markets, DAY_LOSERS_MENU, "Market Losers"));
	markets->Append(new wxMenuItem(markets, VIEW_CHART, "View Chart"));
	bar->Append(m, "File");
	bar->Append(stock, "Invest");
	bar->Append(deposits, "Finance");
	bar->Append(divs, "Dividends");
	bar->Append(markets, "Markets");
	this->SetMenuBar(bar);
	this->Bind(wxEVT_CLOSE_WINDOW, &mainwindow::CloseEvent, this);

	this->Bind(wxEVT_THREAD, &mainwindow::OnQuoteThread, this);

	statbar = new wxStatusBar(this, wxID_BOTTOM);
	statbar->SetStatusText("Ready");
	this->SetStatusBar(statbar);

	this->RetrieveFile();
	this->portwin = (PortfolioWin*)this->GetLeftWin();
	this->GetRightWin(true);
	this->bottom_frame = (BottomFrame*)this->GetBottomFrame();
	this->bottom_frame->SetBackgroundColour(wxColour(28, 56, 100));
	this->top = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* main = new wxBoxSizer(wxVERTICAL);
	top->Add(this->portwin, 1, wxEXPAND);
	top->Add(this->grid_canvas, 4, wxEXPAND);
	main->Add(top, 3, wxEXPAND);
	main->Add(bottom_frame, 1, wxEXPAND);
	this->SetSizer(main);
	this->UpdateGridView();
	this->Layout();
	this->grid_canvas->Initialize();
	this->grid_canvas->SetScrollBars();
	this->Show(true);
}

mainwindow::~mainwindow()
{
	this->DeletePopupMenu();
	this->DeleteSampleStock();
}

void mainwindow::PurchaseWin(wxString& ticker, wxString& longname)
{
	wxString sector_name;
	const Sector* s = this->port.GetConstSector(ticker); 
	if(s)
		sector_name = s->GetSectorName();

	string_three st(ticker, longname, sector_name);
	this->dialog = new Dialog(STOCK_PURCHASE_DIALOG, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Enter Stock Data", &st);
	if (this->dialog->ShowModal() == wxID_OK)
	{
		PurchaseKit kit = this->dialog->GetPurchaseKit();

		// Here we call port.AddToPosition instead of port.Purchase because we already own some of this stock or previously owned it in which it is already in the system...
		this->port.AddToPosition(kit.m_date, kit.m_price, kit.m_shares, kit.m_reinvest, kit.m_reinvest_date);
		dialog = nullptr;
		this->UpdateGridView();
	}
}

void mainwindow::AddDividendWin(wxString& ticker, wxString& longname)
{
	this->dialog = new Dialog(ENTER_DIVIDEND, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Enter Dividend Date & Amount", &longname);
	if (this->dialog->ShowModal() == wxID_OK)
	{
		GenericKit gk = this->dialog->GetGenericKit();
		Dividend div(0, gk.date, gk.amount);
		if (!this->port.AddDividend(div))
			wxMessageBox("Failed to add dividend to " + ticker);
	}
}

void mainwindow::OnViewDividendWin(wxString& ticker)
{
	wxString longname = this->port.StageStockandGetLongName(ticker);
	wxVector<Dividend> sorted = this->port.GetDividendsFromStagedStock();
	if (!sorted.size())
	{
		wxMessageBox("No dividends to list for " + ticker);
		return;
	}
	wxString na = "NA";
	wxArrayString string;
	for (auto& v : sorted)
	{
		wxString item = "";
		wxString Bool = v.DivReinvest ? "Yes" : "No";
		wxString payable_date = v.payment_date.IsValid() ? v.payment_date.Format(STANDARD_DATE) : na;
		item = "Ex_Div: " + v.ex_Div.Format(STANDARD_DATE) + "    Payment Date: " + payable_date + "    Amount: " + wxNumberFormatter::ToString(v.div, 4) + "    Re-Invest Elligible: " + Bool +
			"    Re-Invested amount: " + wxNumberFormatter::ToString(v.re_invest_shares, 4);
		string.Add(item);
	}

	this->generic_list = new GenericListWin(this, wxID_ANY, "Click on Dividend to Remove", ticker, string);
	if (this->generic_list->ShowModal() == wxID_OK)
	{
		int selection = this->generic_list->GetSelection();
		Dividend& dev = sorted[selection];
		this->generic_list->Destroy();
		this->OnModifyDividendWin(dev, ticker, longname);
	}
	else
		this->generic_list->Destroy();
}

void mainwindow::OnModifyDividendWin(Dividend& d, wxString& ticker, wxString& longname)
{
	this->port.GetDivWithAllReInvestmentShares(d);
	GenericKitTemplate<Dividend, wxString> kit(d, longname);
	this->dialog = new Dialog(_EnterDialog::DIVIDEND_ACTION_WIN, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Dividend Modification Window", &kit);
	bool modified_payment_date = false;
	bool modified_shares = false;
	if (this->dialog->ShowModal() == wxID_OK)
	{
		Dividend div = this->dialog->GetDividend();
		if (div.payment_date.IsValid())
		{
			if (!d.payment_date.IsValid())
			{
				this->port.SetDividendPaymentDate(d, div.payment_date);
				modified_payment_date = true;
			}
			else if (d.payment_date.IsValid() && d.payment_date != div.payment_date)
			{
				wxBell();
				if (wxYES == wxMessageBox("Are you sure you want to replace payment date: " + d.payment_date.Format(STANDARD_DATE) +
					" with payment date: " + div.payment_date.Format(STANDARD_DATE) + "?", "", wxYES_NO))
				{
					this->port.SetDividendPaymentDate(d, div.payment_date);
					modified_payment_date = true;
				}
			}
		}

		if (div.re_invest_shares)
		{
			if (d.re_invest_shares)
			{
				wxBell();
				if (wxYES == wxMessageBox("Are you sure you want to change the re-investment shares from: " + wxNumberFormatter::ToString(d.re_invest_shares, 4) +
					" to re-investment shares: " + wxNumberFormatter::ToString(div.re_invest_shares, 4), "", wxYES_NO))
					modified_payment_date = this->port.AddReInvestShares(d, div.re_invest_shares);
			}
			else
			{
				if (modified_payment_date || div.payment_date.IsValid())
				{
					if (div.payment_date > wxDateTime::Today())
					{
						modified_shares = false;
						wxMessageBox("You can't add dividend shares before the payment_date of: " + div.payment_date.Format(STANDARD_DATE));
					}
					else
						modified_shares = this->port.AddReInvestShares(d, div.re_invest_shares);
				}
				else
					modified_shares = this->port.AddReInvestShares(d, div.re_invest_shares);
			}
		}


		if (modified_payment_date || modified_shares)
		{
			wxString one = "";
			wxString two = one;
			if (modified_payment_date)
				one = "Successfully changed payment date to " + div.payment_date.Format(STANDARD_DATE) + "\n";
			if (modified_shares)
				one += "Successfully added re-investment shares of " + wxNumberFormatter::ToString(div.re_invest_shares, 4);
			wxMessageBox(one);
			this->UpdateGridView();
		}

		this->dialog = NULL;
		return;
	}
//	this->dialog->Destroy();
	this->dialog = NULL;
}

void mainwindow::OnAddDivPayoutDate(Dividend& div, wxString& t)
{
	wxString stringdate = "";
	this->generic_entry = new wxTextEntryDialog(this, "Enter Payment Date For " + t);
	this->generic_entry->SetTextValidator(wxTextValidator(wxFILTER_EMPTY, &stringdate));
	this->generic_entry->CenterOnParent();
	wxDateTime date;
	bool first = false;

	do {
		if (first)
			wxMessageBox("Invalid date entered for " + t);
		first = true;
		if (this->generic_entry->ShowModal() == wxID_OK)
		{
			this->TransferDataFromWindow();
		}
		else
		{
			this->generic_entry->Destroy();
			this->generic_entry = NULL;
			return;
		}
	} while (!date.ParseDate(stringdate) || div.ex_Div > date);

	if (div.payment_date.IsValid())
	{
		wxBell();
		int answer = wxMessageBox("This dividend already has a payment date of " + div.payment_date.Format(STANDARD_DATE) + 
			". \nAre you sure you want to replace it with " + stringdate + "?", t,  wxYES_NO);

		if (answer != wxYES)
		{
			this->generic_entry->Destroy();
			this->generic_entry = NULL;
			return;
		}
	}
	this->port.SetDividendPaymentDate(div, date);
	this->UpdateGridView();

	this->generic_entry->Destroy();
	this->generic_entry = NULL;
}

void mainwindow::OnAddDivReInvestmentShares(Dividend& div, double& s)
{

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

void mainwindow::ReInvestSharesWin(wxString& ticker, wxString& longname)
{
	this->dialog = new Dialog(_EnterDialog::DIVIDEND_SHARE_DIALOG, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Dividend ReInvestment Shares", &longname);
	if (this->dialog->ShowModal() == wxID_OK)
	{
		double shares = dialog->GetReInvestShares();
		if (!this->port.AddReInvestShares(shares))
		{
			wxMessageBox("Unsuccessful in adding shares to " + ticker);
			return;
		}
	}

	this->UpdateGridView();
}

bool mainwindow::ValidateExistingTicker(wxString& t)
{
	return this->port.DoesTickerExist(t);
}

void mainwindow::SectorClick(wxVector<SubSector>* v, wxString& name)
{
	this->dialog = new Dialog(_EnterDialog::SUBSECTORWIN, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, name, v);
}

void mainwindow::SectorRightClick(wxVector<SectorStock>* v, wxString& sector)
{
	this->sectorstockwin = new SectorStockWindow(this, v, sector);
	this->Disable();
}

void mainwindow::RightClickGrid(wxString& ticker, wxPoint& p)
{
	this->rightclick_ticker = ticker;
	this->p_sell->SetItemLabel("Sell " + ticker);
	this->p_buy->SetItemLabel("Purchase " + ticker);
	this->p_view_dividends->SetItemLabel("View " + ticker + " Dividends");
	this->p_graph->SetItemLabel("View " + ticker + " Chart");
	bool result = PopupMenu(this->popup, p);
}

void mainwindow::OnChartWinChangeDateRange(wxString& ticker, wxDateTime& start, wxDateTime& end)
{
	StockNode* sn = NULL;
	if (port.StageStock(ticker))
		sn = port.GetStagedStock();

	if (sn)
	{
		// temperary calibrate for chart...
		sn->Calibrate(false, &end);

		this->chartwin->SetNewValues(sn->GetSubVector(start, end), sn->GetStockViewerData());
		this->chartwin->Layout();
		this->chartwin->Refresh();

		sn->Calibrate(false);
		return;
	}

	SampleStock* s = new SampleStock(ticker, this->sample->GetSummaryData(), this->sample->GetSectorName(),
		this->sample->GetIndustryName(), start, end, this->sample->GetSectorId());
	this->DeleteSampleStock();
	this->sample = s;

	this->chartwin->SetNewValues(this->sample->GetDayPrices(), this->sample->GetStockViewerData());
	this->chartwin->Layout();
	this->chartwin->Refresh();
}

void mainwindow::OnThreadCompletion()
{
	this->UpdateGridView();
	this->Layout();
	this->Show(true);
}

void mainwindow::OnThreadCompletion(wxThreadEvent& evt)
{
	switch (evt.GetId())
	{
	case 1: this->statbar->SetStatusText("Stocks Successfully Loaded..."); return;
	case 2: this->statbar->SetStatusText(evt.GetString()); return;
	}

	evt.Skip();
}

void mainwindow::CloseEvent(wxCloseEvent& evt)
{
	if (this->quotethread)
	{
		this->quotethread->Delete();
		while (this->quotethread)
		{

		}
	}
	this->portwin->ClosingSoon();
	this->Hide();
	this->Destroy();
}

void mainwindow::PurchaseWin(wxString& ticker)
{
	this->dialog = new Dialog(STOCK_PURCHASE_DIALOG, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Enter Stock Data", &ticker);
	if (this->dialog->ShowModal() == wxID_OK)
	{
		PurchaseKit kit = this->dialog->GetPurchaseKit();
		this->port.Purchase(kit.m_sect, ticker, kit.m_date, kit.m_price, kit.m_shares, kit.m_reinvest, kit.m_reinvest_date);
		dialog = nullptr;
		this->UpdateGridView();
	}
}

void mainwindow::OnPurchaseMenu(wxCommandEvent& evt)
{
	this->generic_entry = new wxTextEntryDialog(this, "Enter Ticker Symbol");
	wxString ticker = "";
	this->generic_entry->SetTextValidator(wxTextValidator(wxFILTER_ALPHA | wxFILTER_EMPTY, &ticker));
	this->generic_entry->GetTextValidator()->Bind(wxEVT_KEY_DOWN, &mainwindow::OnKeyDown, this);
	this->generic_entry->CenterOnParent();

	// check to see if we already own some of the stock being purchased...
	bool insystem = false;
	
	if (this->generic_entry->ShowModal() == wxID_OK)
	{
		// First see if this is a stock already owned, then we dont need to validate the ticker...
		wxString longname = this->port.StageStockandGetLongName(ticker);
		SummaryData sd;

		// If longname is empty than it was not found and needs to be verified by looking up a quote on yahoo finance for it...
		if (longname.IsEmpty())
		{
			sd = port.QuoteLookup(ticker);
			if (sd.Longname == "NotFound" || sd.marketprice == 0.00)
			{
				// Not a valid ticker, notify the user and return...
				wxMessageDialog* d = new wxMessageDialog(this, "Ticker: " + ticker + " does not exist!");
				if (d->ShowModal())
					d->Destroy();

				this->generic_entry = NULL;
				return;
			}
		}

		this->generic_entry = NULL;
		if (longname == "")
			this->PurchaseWin(ticker);
		else
		{
			longname = sd.Longname;
			longname = Decipherhtmlcodes(longname);
			this->PurchaseWin(ticker, longname);
		}
	}

	this->generic_entry = NULL;
}

stock_node* mainwindow::LotSelectionWindow(wxString& ticker)
{
	wxVector<stock_node*> sn = this->port.GetLotData();
	if (!sn.size())
	{
		if (this->generic_entry->ShowModal())
		{
			this->generic_entry->Destroy();
			this->generic_entry = NULL;
		}
		return NULL;
	}
	wxArrayString string;
	wxDateTime T = wxDateTime::Today();
	for (auto& v : sn)
	{
		wxString s = " Lot: " + wxNumberFormatter::ToString(v->GetLotNumber()) + "  Purchase Date: " + v->GetPurchaseDate().Format(STANDARD_DATE) + "  Purchase Price: " +
			wxNumberFormatter::ToString(v->GetPurchasePrice(), 5) + "  Shares: " + wxNumberFormatter::ToString(v->GetShares(NULL), 5) + "  Costbasis: " +
			wxNumberFormatter::ToString(v->GetCostBasis(&T), 2);
		string.push_back(s);
	}

	this->generic_list = new GenericListWin(this, wxID_ANY, "Pick lot to sell", ticker, string);
	stock_node* stock_n = NULL;
	if (this->generic_list->ShowModal() == wxID_OK)
	{
		int selection = this->generic_list->GetSelection();
		stock_n = sn[selection];
		this->generic_list->Destroy();
	}
	else
	{
		this->generic_list->Destroy();
		return NULL;
	}

	return stock_n;
}

void mainwindow::UserEnterSellDataWin(stock_node* stock_n)
{
	wxString ticker = stock_n->GetTicker();
	long lot = stock_n->GetLotNumber();
	wxDateTime date = stock_n->GetPurchaseDate();
	double shares = stock_n->GetShares(NULL);
	GenericKit kit(date, shares);
	this->dialog = new Dialog(_EnterDialog::SELL_STOCK, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ticker, &kit);

	if (this->dialog->ShowModal() == wxID_OK)
	{
		SellKit kit = this->dialog->GetSellKit();
		kit.id = lot;
		if (this->port.Sell(lot, kit.date, kit.m_shares, kit.m_price))
		{
			if (!this->port.IsStockActive(ticker))
				this->grid_canvas->RemoveRow(ticker);
			
			this->UpdateGridView();
			this->portwin->Update();
		}
	}
}

void mainwindow::OnSellMenu(wxCommandEvent& evt)
{
	this->generic_entry = new wxTextEntryDialog(this, "Enter Ticker Symbol");
	wxString user = "";
	this->generic_entry->SetTextValidator(wxTextValidator(wxFILTER_ALPHA | wxFILTER_EMPTY, &user));
	this->generic_entry->GetTextValidator()->Bind(wxEVT_KEY_DOWN, &mainwindow::OnKeyDown, this);
	this->generic_entry->CenterOnParent();
	do
	{
		if (this->generic_entry->ShowModal() == wxID_CANCEL)
			return;
	} while (!this->port.RequestSell(user));
	
	stock_node* stock_n = this->LotSelectionWindow(user);
	if (!stock_n)
		return;

	this->UserEnterSellDataWin(stock_n);
}
/*
void mainwindow::SampleStockChartView(wxCommandEvent& evt)
{
	wxString thi = "";
	this->DeleteSampleStock();
	this->dialog = new Dialog(_EnterDialog::CHART_VIEW_DATA_ENTRY, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Enter Data", &thi);

	wxString ticker = "";
	wxDateTime start;
	wxDateTime end;
	SummaryData sd;
	wxString sector = "N/A";
	wxString industry = "N/A";

	if (this->dialog->ShowModal() == wxID_OK)
	{
		this->dialog->GetChartViewData(ticker, start, end);

		SectorClass& sc = GetSectorClass();
		SectorStock* scs = sc.FindSectorStock(ticker);
		if (!scs)
		{
			wxBell();
			if (wxYES == wxMessageBox("Could not find in stocks on file. Would you like to check online?", "", wxYES_NO))
			{
				sd = port.QuoteLookup(ticker);
				if (!sd.IsValid())
				{
					wxBell();
					wxMessageDialog* d = new wxMessageDialog(this, "Data for " + ticker + " could not be found!");
					if (d->ShowModal())
						d->Destroy();
					return;
				}
				else
				{
					port.GetSectorStockIsIn(ticker, sector, industry);
				}
				this->sample = new SampleStock(ticker, sd, sector, industry, start, end, sc.GetSectorID(sector));
				this->ChartViewHelper(sample);
				return;
			}
			return;
		}
		sd.Longname = scs->longname;
		sd.marketprice = scs->d_price;
		this->sample = new SampleStock(ticker, sd, scs->sectorname, industry, start, end, scs->sec);

		this->ChartViewHelper(sample);
	}
}
*/
void mainwindow::SampleStockChartView(wxCommandEvent& evt)
{
	wxString thi = "";
	this->dialog = new Dialog(_EnterDialog::CHART_VIEW_DATA_ENTRY, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Enter Data", &thi);

	wxString ticker = "";
	wxDateTime start; 
	wxDateTime end;
	wxString sector = "N/A";
	wxString industry = "N/A";
	if (this->dialog->ShowModal() == wxID_OK)
	{
		this->dialog->GetChartViewData(ticker, start, end);
	}
	else
	{
		this->DeleteSampleStock();
		return;
	}

	StockNode* sn = NULL;
	SectorStock* scs = NULL;
	if (port.StageStock(ticker))
	{
		sn = port.GetStagedStock();
		wxString longname = sn->GetLongName();

		// temperary calibrate for chart...
		sn->Calibrate(false, &end);

		this->ChartViewHelper(ticker, longname, sn->GetSubVector(start, end), sn->GetStockViewerData(), OWNED);

		// Recalibrate back to normal...
		sn->Calibrate(false);
		this->DeleteSampleStock();
		return;
	}
	
	SectorClass& sc = GetSectorClass();
	scs = sc.FindSectorStock(ticker);
	
	SummaryData sd;
	if (scs)
	{
		sd.Longname = scs->longname;
		sd.marketprice = scs->d_price;
		this->DeleteSampleStock();
		this->sample = new SampleStock(ticker, sd, scs->sectorname, industry, start, end, scs->sec);
		this->ChartViewHelper(ticker, sd.Longname, this->sample->GetDayPrices(), this->sample->GetStockViewerData(), UNOWNED);
		return;
	}
	
	wxBell();
	if (wxYES == wxMessageBox("Could not find in stocks on file. Would you like to check online?", "", wxYES_NO))
	{
		sd = port.QuoteLookup(ticker);
		if (!sd.IsValid())
		{
			wxBell();
			wxMessageDialog* d = new wxMessageDialog(this, "Data for " + ticker + " could not be found!");
			if (d->ShowModal())
				d->Destroy();
			return;
		}
		else
		{
			if (sd.Longname.find("ETF") == -1)
				port.GetSectorStockIsIn(ticker, sector, industry);
			else
				sector = _ETF;
		}

		this->DeleteSampleStock();
		this->sample = new SampleStock(ticker, sd, sector, industry, start, end, sc.GetSectorID(sector));
		this->ChartViewHelper(ticker, sd.Longname, sample->GetDayPrices(), sample->GetStockViewerData(), UNOWNED);
	}
}

void mainwindow::OnHome(wxCommandEvent& evt)
{
	this->GoToHomeWindow();
}

void mainwindow::GoToHomeWindow()
{
	if (!this->chartwin)
		return;

//	this->grid_panel->Destroy();
//	this->grid_panel = this->GetRightWin();
	this->top->Replace(this->chartwin, this->grid_canvas);
	this->grid_canvas->Enable();
	this->chartwin->Hide();
	this->chartwin->Destroy();
	this->chartwin = NULL;
	this->Layout();
	this->Refresh();
//	this->Show();
//	this->Layout();
//	this->Update();
}

void mainwindow::OnSellPopupClick(wxCommandEvent&)
{
	wxString ticker = this->rightclick_ticker;
	if (!this->port.RequestSell(ticker))
	{
		wxMessageBox("Something went wrong! " + ticker + " cannot be sold or any other action!");
		return;
	}

	stock_node* sn = this->LotSelectionWindow(ticker);
	if (!sn)
		return;

	this->UserEnterSellDataWin(sn);
}

void mainwindow::OnPurchasePopupClick(wxCommandEvent& evt)
{
	wxString ticker = this->rightclick_ticker;
	wxString longname = this->port.StageStockandGetLongName(ticker);
	longname = Decipherhtmlcodes(longname);

	this->PurchaseWin(ticker, longname);
}

void mainwindow::OnAddReInvestSharesPopup(wxCommandEvent& evt)
{
	wxString ticker = this->rightclick_ticker;
	wxString longname = this->port.StageStockandGetLongName(ticker);
	longname = Decipherhtmlcodes(longname);

	this->ReInvestSharesWin(ticker, longname);
}

void mainwindow::OnQuoteLookupPopup(wxCommandEvent& evt)
{
	wxString ticker = this->rightclick_ticker;
	this->smd = port.QuoteLookup(ticker, false, true);

	if (smd.Longname == "NotFound")
	{
		wxMessageDialog* d = new wxMessageDialog(this, "Data for " + this->rightclick_ticker + " could not be found!");
		if (d->ShowModal())
			d->Destroy();
		this->DeleteQuoteThread();
		return;
	}

	this->dialog = new Dialog(_EnterDialog::QUOTE_WIN, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ticker, &smd);
}

void mainwindow::ChartViewHelper(const wxString& ticker, const wxString& longname, wxVector<Day_Prices>* v, StockViewerData* svd, _ChartControlType type)
{
	wxString value = longname;
	value = Decipherhtmlcodes(value) + ":" + ticker;

	if (this->grid_canvas->IsShown())
	{
		this->chartwin = new ChartControlWin(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, value, v, svd, type);
//		this->chartwin = new ChartControlWin(this, wxID_ANY, this->grid_panel->GetPosition(), this->grid_panel->GetSize(), value, v, svd, type);
//		this->top->Hide(this->grid_canvas);
//		this->grid_panel->Disable();
		this->top->Replace(this->grid_canvas, this->chartwin);
		this->grid_canvas->Disable();
		this->Layout();
//		this->chartwin->Layout();
//		this->chartwin->Show();
		this->chartwin->Refresh();
		return;
	}
	else
	{
		ChartControlWin* c = new ChartControlWin(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, value, v, svd, type);
		this->chartwin->Hide();
		this->top->Replace(this->chartwin, c);
		this->top->Hide(c);
		this->chartwin->Destroy();
		this->chartwin = c;
	}

//	this->chartwin->Show();
	this->top->Show(this->chartwin);
	this->top->Layout();
	this->chartwin->Refresh();
	return;
	this->top->Layout();
	this->Layout();
	this->Refresh();
}
/*
void mainwindow::ChartViewHelper(SampleStock* sn)
{
	wxString temp = sn->GetTicker();
	wxString tick = sn->GetLongName();
	tick = Decipherhtmlcodes(tick);
	tick += ":" + temp;
	if (this->grid_panel->IsShown())
	{
		this->chartwin = new ChartControlWin(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, tick, sn->GetDayPrices(), sn->GetStockViewerData());
		this->top->Replace(this->grid_panel, this->chartwin);
		this->grid_panel->Hide();
	}
	else
	{
		ChartControlWin* c = new ChartControlWin(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, tick, sn->GetDayPrices(), sn->GetStockViewerData());
		this->top->Replace(this->chartwin, c);
		this->chartwin->Destroy();
		this->chartwin = c;
	}
//	this->grid_panel->Hide();
//	this->chartwin = new ChartControlWin(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, tick, sn->GetDayPrices(), sn->GetStockViewerData());
//	if (this->grid_panel->IsShown())
//		this->top->Replace(this->grid_panel, this->chartwin);
	this->Layout();
	this->Refresh();
}
*/
void mainwindow::OnChartView(wxCommandEvent& evt)
{
	wxString tick = this->rightclick_ticker;
	StockNode* sn = NULL;
	if (port.StageStock(tick))
		sn = port.GetStagedStock();
	else
		return;
	
	if (!sn)
		return;
	wxString temp = tick;
	tick = sn->GetLongName();
	tick = Decipherhtmlcodes(tick);
	tick += ":" + temp;
	this->chartwin = new ChartControlWin(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, tick, sn->GetDayPrices(), sn->GetStockViewerData(), OWNED);
	this->top->Replace(this->grid_canvas, this->chartwin);
	this->grid_canvas->Disable();
	this->Layout();
	this->chartwin->Refresh();
}

void mainwindow::OnChartView(wxString& ticker)
{
	wxString tick = this->rightclick_ticker;
	StockNode* sn = NULL;
	if (port.StageStock(tick))
		sn = port.GetStagedStock();
	else
		return;
}

void mainwindow::OnLoadSectorStocks(wxCommandEvent& evt)
{
	this->mainwindow_thread = new LoadSectorStocksThread(this);
	if (mainwindow_thread->Run() != wxThreadError::wxTHREAD_NO_ERROR)
	{
		wxMessageBox("mainwindow_thread failed to run in mainwindow::OnLoadSectorStocks!");
		delete this->mainwindow_thread;
		this->mainwindow_thread = NULL;
		return;
	}

	this->statbar->SetStatusText("Fetching Stock Data...");
}

void mainwindow::OnQuoteThread()
{

	if (smd.Longname == "NotFound")
	{
		wxMessageDialog* d = new wxMessageDialog(this, "Data for " + this->rightclick_ticker + " could not be found!");
		if (d->ShowModal())
			d->Destroy();
		this->DeleteQuoteThread();
		return;
	}

	this->dialog = new Dialog(_EnterDialog::QUOTE_WIN, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, this->rightclick_ticker, &smd);
}

void mainwindow::OnQuoteThread(wxThreadEvent& evt)
{
	if (evt.GetId() != 1)
	{
		evt.Skip();
		return;
	}

	if (smd.Longname == "NotFound")
	{
		wxMessageDialog* d = new wxMessageDialog(this, "Data for " + this->rightclick_ticker + " could not be found!");
		if (d->ShowModal())
			d->Destroy();
		this->DeleteQuoteThread();
		return;
	}

	this->dialog = new Dialog(_EnterDialog::QUOTE_WIN, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, this->rightclick_ticker, &smd);
}

void mainwindow::OnAddDividendPopup(wxCommandEvent& evt)
{
	wxString ticker = this->rightclick_ticker;
	wxString longname = this->port.StageStockandGetLongName(ticker);

	this->AddDividendWin(ticker, longname);
}

void mainwindow::OnViewDividendPopup(wxCommandEvent& evt)
{
	wxString ticker = this->rightclick_ticker;
	this->OnViewDividendWin(ticker);
}

void mainwindow::OnViewDividend(wxCommandEvent& evt)
{
	wxString ticker = "";
	this->generic_entry = new wxTextEntryDialog(this, "Enter ticker");
	this->generic_entry->SetTextValidator(wxTextValidator(wxFILTER_EMPTY, &ticker));
	this->generic_entry->GetTextValidator()->Bind(wxEVT_KEY_DOWN, &mainwindow::OnKeyDown, this);
	this->generic_entry->CenterOnParent();
	wxString longname = "";
	do {
		if (this->generic_entry->ShowModal() == wxID_OK)
			this->generic_entry->TransferDataFromWindow();
		else
		{
			this->generic_entry = NULL;
			return;
		}
		longname = this->port.StageStockandGetLongName(ticker);

	} while (longname.IsEmpty());

	this->OnViewDividendWin(ticker);
}

void mainwindow::OnReInvestSharesMenu(wxCommandEvent& evt)
{
	wxString ticker = "";
	this->generic_entry = new wxTextEntryDialog(this, "Enter ticker");
	this->generic_entry->SetTextValidator(wxTextValidator(wxFILTER_EMPTY, &ticker));
	this->generic_entry->GetTextValidator()->Bind(wxEVT_KEY_DOWN, &mainwindow::OnKeyDown, this);
	this->generic_entry->CenterOnParent();
	wxString longname = "";
	do {
		if (this->generic_entry->ShowModal() == wxID_OK)
			this->generic_entry->TransferDataFromWindow();
		else
		{
			this->generic_entry = NULL;
			return;
		}
		longname = this->port.StageStockandGetLongName(ticker);

	} while (longname.IsEmpty());

	this->ReInvestSharesWin(ticker, longname);
}

void mainwindow::OnSave(wxCommandEvent& evt)
{
	this->SaveFile();
}

void mainwindow::OnQuoteLookup(wxCommandEvent& evt)
{
	SummaryData sd;
	this->generic_entry = new wxTextEntryDialog(this, "Enter Ticker Symbol");
	wxString user = "";
	this->generic_entry->SetTextValidator(wxTextValidator(wxFILTER_EMPTY, &user));
	this->generic_entry->GetTextValidator()->Bind(wxEVT_KEY_DOWN, &mainwindow::OnKeyDown, this);
	this->generic_entry->CenterOnParent();
	if (this->generic_entry->ShowModal() == wxID_OK)
	{
		bool validate = this->generic_entry->Validate();
		sd = port.QuoteLookup(this->generic_entry->GetValue(), false, true);
		if (sd.Longname == "NotFound")
		{
			wxMessageDialog* d = new wxMessageDialog(this, "Data for " + user + " could not be found!");
			if (d->ShowModal())
				d->Destroy();
			return;
		}
		this->dialog = new Dialog(_EnterDialog::QUOTE_WIN, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, user, &sd);
	}
}

void mainwindow::OnKeyDown(wxKeyEvent& evt)
{
	int n = evt.GetKeyCode();
	wxTextEntry* t = NULL;
	if (generic_entry)
		t = this->generic_entry->GetTextValidator()->GetTextEntry();
	else
	{
		wxFAIL_MSG("this->generic_entry is nullptr in mainwindow::OnKeyDown!");
		return;
	}
	if (t)
		t->ForceUpper();
	evt.Skip();
}

void mainwindow::ClosePanelEvent(wxCloseEvent& evt)
{
	
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
	this->generic_entry = new wxTextEntryDialog(this, "Enter Ticker Symbol");
	wxString ticker = "";
	this->generic_entry->SetTextValidator(wxTextValidator(wxFILTER_EMPTY, &ticker));
	this->generic_entry->GetTextValidator()->Bind(wxEVT_KEY_DOWN, &mainwindow::OnKeyDown, this);
	this->generic_entry->CenterOnParent();
	wxString longname = "";
	do {
		if (this->generic_entry->ShowModal() == wxID_OK)
		{
			this->generic_entry->TransferDataFromWindow();
			longname = this->port.StageStockandGetLongName(ticker);
		}
		else
			return;
	} while (longname.IsEmpty());

	this->AddDividendWin(ticker, longname);
}

void mainwindow::OnViewDeposits(wxCommandEvent& evt)
{
	wxVector<deposit_pair> sorted = this->port.GetDepositVector();

	wxArrayString string;
	if (!sorted.size())
	{
		wxMessageBox("There are no deposits to view.");
		return;
	}

	double total = 0.0;
	for (auto& v : sorted)
	{
		total += v.value;
		wxString item = "";
		item = "Deposit Date: " + v.T.Format(STANDARD_DATE) + "  Deposit: $" + wxNumberFormatter::ToString(v.value, 2);
		string.Add(item);
	}

	this->generic_list = new GenericListWin(this, wxID_ANY, "Total Deposits: $" + wxNumberFormatter::ToString(total, 2), "Past Deposits", string);
	this->generic_list->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_MODERN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	if (this->generic_list->ShowModal() == wxID_OK)
	{
		this->generic_list->Destroy();
	}
	else
		this->generic_list->Destroy();
}

void mainwindow::OnMarketGainers(wxCommandEvent&)
{
	wxDateTime T(wxDateTime::Today());
	GetWorkDate(T);
	wxVector<DayGainersandLosers> sorted = this->port.GetDayGainers();
	this->dialog = new Dialog(_EnterDialog::DAY_GAINERS_WIN, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Losers as of " + T.Format(STANDARD_DATE), &sorted);
}

void mainwindow::OnMarketLosers(wxCommandEvent&)
{
	wxDateTime T(wxDateTime::Today());
	GetWorkDate(T);
	wxVector<DayGainersandLosers> sorted = this->port.GetDayLosers();
	this->dialog = new Dialog(_EnterDialog::DAY_LOSERS_WIN, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, "Losers as of " + T.Format(STANDARD_DATE), &sorted);
}

void mainwindow::OnAddDepositSchdule(wxCommandEvent& evt)
{

}

void mainwindow::OnRightClick(wxMouseEvent& evt)
{
	int ca = 1;
}

void mainwindow::OnDateChange(wxString& date)
{
	wxString title = "Updating to date: ";
	title += date;
	wxString msg = "Updating...";
//	wxGenericProgressDialog pr(title, msg, 200, this, wxPD_APP_MODAL|wxPD_AUTO_HIDE|wxPD_SMOOTH);
//	pr.CenterOnScreen();
//	pr.Show();
//	bool b = pr.Update(50);
	this->port.DateChange();
//	pr.Update(200, "Finished...");
//	pr.Refresh();
	this->UpdateGridView();
	this->portwin->Update();
}

void mainwindow::OnUpdate()
{
	this->port.ForceUpdate();
	this->UpdateGridView();
	this->portwin->Update();
}

// mainwindow private functions..

wxWindow* mainwindow::GetLeftWin()
{
	PortfolioWin* win = new PortfolioWin(this, wxID_ANY, &this->main_clock, this->port.GetStockViewerData(), &this->port);
	return win;
}


wxScrolledCanvas* mainwindow::GetRightWin()
{
	int size = port.GetNumItems(_PortfolioType::STOCK);
	wxScrolled<wxWindow>* P = new wxScrolled<wxWindow>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxCLIP_CHILDREN);
	P->SetScrollbars(25, 25, 50, 50);
	P->EnableScrolling(true, true);
	P->SetBackgroundColour(wxColour(0, 0, 0));
	this->grid_canvas = new GridView2(P, this, 30, 60, wxSize(204, 30));
//	this->grid_view = new GridView(P, 30, size);
	wxBoxSizer* b = new wxBoxSizer(wxHORIZONTAL);
	b->Add(grid_canvas, 1);
	P->SetSizer(b);
	P->SetTargetWindow(this->grid_canvas);
//	P->SetSizer(grid_view);
	return P;
}

void mainwindow::GetRightWin(bool b)
{
	this->grid_canvas = new GridView2(this, this, 60, 30, wxSize(204, 30));
}

wxWindow* mainwindow::GetBottomFrame()
{
	return new BottomFrame(this, wxID_ANY);
}

void mainwindow::UpdateGridView()
{
	wxVector<StockNode*> stocks = this->port.GetStockNodeItems();
	this->grid_canvas->SetNumOfItems(stocks.size());
	for (size_t i = 0; i < stocks.size(); ++i)
	{
		StockViewerData* svd = stocks[i]->GetStockViewerData();
		if (this->grid_canvas->ItemExist(svd->ticker))
			this->grid_canvas->UpdateRow(svd);
		else
			this->grid_canvas->SetNewRow(svd);
	}

	StockViewerData* svd = this->port.GetStockViewerData();
	if (this->grid_canvas->ItemExist(svd->ticker))
		this->grid_canvas->UpdateRow(svd);
	else
		this->grid_canvas->SetNewRow(svd);

	if (this->portwin)
		this->portwin->Update();

	return;
	// NOTHING AFTER HERE. TESTING NEW CANVAS...
}

void mainwindow::SaveFile()
{
	this->port.Save();
	SectorClass& sc = GetSectorClass();
	sc.Save();
}

void mainwindow::RetrieveFile()
{
	this->port.Retrieve();
}

void mainwindow::DeleteSampleStock()
{
	if (this->sample)
		delete this->sample;

	this->sample = NULL;
}

void mainwindow::DeletePopupMenu()
{
	delete this->popup;
//	delete this->p_sell;
//	delete this->p_buy;
//	delete this->p_quote;
//	delete this->p_add_div;
//	delete this->p_remove_div;
//	delete this->p_ohlc;
}

void mainwindow::DeleteQuoteThread()
{
	if (this->quotethread)
	{
		this->quotethread->Delete();
		while (this->quotethread)
		{

		}
	}
}

void mainwindow::DeleteLoadStocksThread()
{
	if (this->mainwindow_thread)
	{
		this->mainwindow_thread->Delete();
		while (this->mainwindow_thread)
		{

		}
	}
}

void mainwindow::CreatePopupMenu()
{
	this->popup = new wxMenu();
	this->p_sell = new wxMenuItem(popup, _MenuItemIDs::P_SELL_STOCK, "Sell");
	this->p_buy = new wxMenuItem(popup, _MenuItemIDs::P_STOCK_PURCHASE, "Purchase");
	this->p_quote = new wxMenuItem(popup, _MenuItemIDs::P_QUOTE, "Quote Lookup");
	this->p_add_div_reinvest = new wxMenuItem(popup, _MenuItemIDs::P_ADD_DIV_REINVEST, "Add Div Re-invest shares");
	this->p_add_div = new wxMenuItem(popup, _MenuItemIDs::P_ADD_DIV, "Add Dividend");
	this->p_view_dividends = new wxMenuItem(popup, _MenuItemIDs::P_VIEW_DIVIDENDS, "Empty");
	this->p_graph = new wxMenuItem(popup, _MenuItemIDs::P_GRAPH, "View");

	this->popup->Append(this->p_buy);
	this->popup->Append(this->p_sell);
	this->popup->Append(this->p_quote);
	this->popup->Append(this->p_add_div_reinvest);
	this->popup->Append(this->p_add_div);
	this->popup->Append(this->p_view_dividends);
	this->popup->Append(this->p_graph);
}