#include "mainwindow.h"

wxBEGIN_EVENT_TABLE(mainwindow, wxFrame)
	EVT_MENU(STOCK_PURCHASE_DIALOG, OnPurchaseMenu)
	EVT_MENU(wxID_SAVE, OnSave)
	EVT_MENU(QUOTE_LOOKUP, OnQuoteLookup)
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
	EVT_MENU(_MenuItemIDs::VIEW_DEPOSITS, OnViewDeposits)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(GridCanvas, wxWindow)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(Gnode, wxWindow)
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
	if (top_distance == 4)
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

			value3 = this->svd_vec[iter]->GetDayGain();
			value2 = day_gain_literal;
			int result = IsStringPNZ(value3);
			if (result == 0)
				this->bottom_text_color = this->yellow;
			else if (result == 1)
				this->bottom_text_color = this->green;
			else
				this->bottom_text_color = this->red;

			value3 += "%";
			gc->GetTextExtent(value2, &textextent2.w, &textextent2.h);
			gc->GetTextExtent(value3, &textextent3.w, &textextent3.h);
		}
		else if (iteminfo_iter == 3)
		{
			value2 = week_gain_literal;
			value3 = this->svd_vec[iter]->GetWeekGain();
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
			value3 = this->svd_vec[iter]->GetQuarterGain();
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
			value3 = this->svd_vec[iter]->GetYearGain();
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
	else if (top_distance == -21000)
	{
		value = this->svd_vec[iter]->GetLongName();
		value = Decipherhtmlcodes(value);
		gc->GetTextExtent(value, &textextent.w, &textextent.h);
		top_distance = textextent.w * -1;
	}
	else if (top_distance > rectSize.GetWidth())
	{
		if (iter + 1 >= this->svd_vec.size())
			iter = 0;
		else 
			++iter;
		value = this->svd_vec[iter]->GetLongName();
		value = Decipherhtmlcodes(value);
		gc->GetTextExtent(value, &textextent.w, &textextent.h);
		top_distance = textextent.w * -1;
	}
	else
	{
		value = this->svd_vec[iter]->GetLongName();
		value = Decipherhtmlcodes(value);
		gc->GetTextExtent(value, &textextent.w, &textextent.h);
	}

	gc->DrawText(value, rectOrigin.x + this->top_distance, (rectOrigin.y + ((rectSize.GetHeight() / 2.0) / 2.0)) - textextent.h / 2.0); //Draw in top half of rect
	if (top_distance != 4)
		++top_distance;
	if (bottom_in_motion)
	{
		if (bottom_distance == -21000)
			bottom_distance = (textextent2.w + textextent3.w) * -1;

		gc->DrawText(value2, rectOrigin.x + this->bottom_distance,
			(((rectOrigin.y + rectSize.GetHeight()) * (3.0 / 4.0)) - textextent2.h / 2.0));

		gc->SetFont(gc->CreateFont(this->textfont, this->bottom_text_color));
		gc->DrawText(value3, rectOrigin.x + this->bottom_distance + textextent2.w + 6,
			(((rectOrigin.y + rectSize.GetHeight()) * (3.0 / 4.0)) - textextent3.h / 2.0)); // Draw in bottom half of rect
		this->bottom_distance++;
		if (bottom_distance > rectSize.GetWidth())
		{
			bottom_distance = -21000;
			if (iteminfo_iter == 5)
			{
				iteminfo_iter = 0;
				++top_distance;
			}
			else
				++iteminfo_iter;
		}
	}
	delete gc;
	return;
}

GridCanvas::GridCanvas(wxWindow* w, wxSize size, wxString& val, int flags) : wxWindow(w, wxID_ANY, wxDefaultPosition, size),
	value(val), textcolor(white), flag(flags), textextent(0.0, 0.0)
{
	this->Bind(wxEVT_ENTER_WINDOW, &GridCanvas::OnMouseEnter, this);
	this->Bind(wxEVT_LEAVE_WINDOW, &GridCanvas::OnMouseLeave, this);
	this->SetMaxSize(size);
	this->SetMinSize(size);
	this->SetBackgroundColour(w->GetBackgroundColour());
	this->SetBackgroundStyle(wxBackgroundStyle::wxBG_STYLE_PAINT);
	this->Bind(wxEVT_PAINT, &GridCanvas::OnPaint, this);

	this->Initialize();
}

void GridCanvas::OnPaint(wxPaintEvent& evt)
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

	// If the value of text is empty, no need to draw text...
	if (this->value.empty())
	{
		delete gc;
		return;
	}

	gc->SetFont(gc->CreateFont(this->textfont, this->textcolor));

	gc->GetTextExtent(this->value, &textextent.w, &textextent.h);

	gc->DrawText(this->value, rectOrigin.x + this->distance, (rectOrigin.y + rectSize.GetHeight() / 2.0) - textextent.h / 2.0);

	delete gc;
	
	/*
	int width = parent->GetSize().x / 12;
	int height = 0;
	wxSize proportionSize(width, width * (2.0 / 6.0));
	wxPoint rectOrigin = this->FromDIP(wxPoint(0, 0));
	wxSize rectSize = this->FromDIP(proportionSize);
	wxColour font = wxColour(255, 255, 255);
	//	gc->SetPen(wxPen(wxColour(255, 0, 0), 2));

	if (mousedown)
	{
		gc->SetBrush(*wxGREEN_BRUSH);
		font = wxColour(0, 0, 0);
	}
	else if (evt.GetId() == 2)
	{
		wxColour begin = wxColour(0, 0, 0);
		wxColour end = wxColour(255, 255, 255);
		wxGraphicsBrush gb = gc->CreateLinearGradientBrush(rectOrigin.x, rectOrigin.y, rectOrigin.x + rectSize.GetWidth(), rectOrigin.y + rectSize.GetHeight(), begin, end);
		gc->SetBrush(gb);
	}
	else
	{
		gc->SetBrush(*wxBLUE_BRUSH);
	}

	gc->DrawRoundedRectangle(rectOrigin.x, rectOrigin.y, rectSize.GetWidth(), rectSize.GetHeight(), rectSize.GetHeight() / 5.0);

	wxFont f = wxFont(*wxNORMAL_FONT);
	int pointsize = 8;
	f.SetPointSize(pointsize);
	f.SetFamily(wxFontFamily::wxFONTFAMILY_MODERN);
	f = f.MakeBold();
	wxGraphicsFont gf = gc->CreateFont(f, font);
	gc->SetFont(gf);
	double w = 0.0;
	double h = 0.0;
	gc->GetTextExtent(text, &w, &h);
	gc->DrawText(text, rectOrigin.x + (rectSize.GetWidth() / 2.0) - w / 2.0, (rectOrigin.y + rectSize.GetHeight() / 2.0) - h / 2.0);

	delete gc;
	*/
}

TextExtent* GridCanvas::GetTextExtent()
{
	return &this->textextent;
}

bool GridCanvas::IsMatch(wxString& s)
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

bool GridCanvas::IsEmpty()
{
	return this->value.IsEmpty();
}

void GridCanvas::OnMouseEnter(wxMouseEvent& evt)
{
	this->background = this->hover;
	this->Refresh();
}

void GridCanvas::OnMouseLeave(wxMouseEvent& evt)
{
	this->SetBackgroundColor();
	this->Refresh();
}

void GridCanvas::CopyGridCanvasData(GridCanvasData& gcd)
{
	this->flag = gcd.flags;
	this->value = gcd.value;

	this->Initialize();
	this->Refresh();
}

void GridCanvas::CopyGridCanvas(const GridCanvas& gc)
{
	this->backup_flag = this->flag;
	this->backup_value = this->value;

	this->flag = gc.flag;
	this->value = gc.value;

	this->Initialize();
	this->Refresh();
}

void GridCanvas::CopyGridCanvasFromBackup(const GridCanvas& gc)
{
	this->backup_flag = this->flag;
	this->backup_value = this->value;

	this->flag = gc.backup_flag;
	this->value = gc.backup_value;

	this->Initialize();
	this->Refresh();
}

void GridCanvas::CopyGridCanvasAttributes(const GridCanvas& gc)
{
	this->flag = gc.flag;
	this->value = "";
	this->Initialize();
}

void GridCanvas::Clear(int flags)
{
	this->value = "";

	if (flags == -1)
		this->SetToNormalBackground();
	else
	{
		this->flag = flags;
		this->Initialize();
	}

	this->Refresh();
}

void GridCanvas::m_SetValue(wxString& val)
{
	if (this->value == val)
		return;

	this->value = val;

	if (flag & GridCanvasFlag::COLORDEPENDENT)
		this->SetTextColor(IsStringPNZ(this->value));

	this->Refresh();
}

wxString GridCanvas::m_GetValue()
{
	return this->value;
}

void GridCanvas::SetToTotalCanvas()
{
	this->background = this->totalRowColor;
}

void GridCanvas::SetTotTotalStartRowCanvas()
{
	this->textcolor = this->rowHeaderColour;
	this->textfont = this->rowStartFont;
	this->SetToTotalCanvas();
}

void GridCanvas::SetToHeader()
{
	this->textcolor = this->colHeaderColour;
	this->textfont = this->rowStartFont;
	this->background = this->normal;
}

void GridCanvas::SetToRowStartCanvas()
{
	this->textcolor = this->rowHeaderColour;
	this->textfont = this->rowStartFont;
	this->background = this->normal;
}

void GridCanvas::SetToDataCellColor()
{
	this->textcolor = this->white;
}

void GridCanvas::SetToNormalBackground()
{
	this->background = this->normal;
}

void GridCanvas::SetToMediumFont()
{
	this->textfont = this->normalFont;
}

void GridCanvas::SetToLargeFont()
{
	this->textfont = this->rowStartFont;
}

void GridCanvas::LeftClick()
{
	this->background = this->background == this->onClick ? this->normal : this->onClick;
	this->Refresh();
}

void GridCanvas::RightClick()
{

}

int GridCanvas::Getflags()
{
	return this->flag;
}

wxString GridCanvas::GetCanvasStringValue()
{
	return this->value;
}

double GridCanvas::GetCanvasDoubleValue()
{
	double d = 0.0;
	wxString temp = this->value;

	int index = temp.find('%');
	if (index != -1)
		temp = temp.Mid(0, index);

	index = temp.find('$');
	if (index != -1)
		temp = temp.Mid(1);

	if (temp.ToDouble(&d))
		return d;

	wxFAIL_MSG("ToDouble failed in GridCanvas::GetDoubleValue! value is: " + value);
	return 0.0;
}

wxString GridCanvas::GetCanvasStringBackupValue()
{
	return this->backup_value;
}

double GridCanvas::GetCanvasDoubleBackupValue()
{
	double d = 0.0;
	if (this->backup_value.IsEmpty())
		return 0.0;

	wxString temp = this->backup_value;
	int index = temp.find('%');
	if (index != -1)
		temp = temp.Mid(0, index);
	if (temp.ToDouble(&d))
		return d;

	return 0.0;
}

void GridCanvas::Initialize()
{
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
}

void GridCanvas::SetTextColor(int i)
{
	switch (i)
	{
	case 1: this->textcolor = this->green; return;
	case 0: this->textcolor = this->yellow; return;
	case -1: this->textcolor = this->red; return;
	}

	wxFAIL_MSG("Switch in GridCanvas::SetTextColor failed!");
}

void GridCanvas::SetBackgroundColor()
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

// Gnode functions...
Gnode::Gnode(GridView* gv, wxWindow* w, wxSize size, size_t row, size_t col, Gnode* up, Gnode* right, Gnode* down, Gnode* left, wxString& val, int flags)
	: GridCanvas(w, size, val, flags), m_parent(gv), m_up(up), m_right(right), m_down(down), m_left(left)
{
	if(flags & GridCanvasFlag::ROWHEAD)
		this->Bind(wxEVT_RIGHT_DOWN, &Gnode::OnRightClick, this);
	if (flags & GridCanvasFlag::COLHEAD)
		this->Bind(wxEVT_LEFT_DCLICK, &Gnode::OnSortClick, this);
}

Gnode::Gnode(GridView* gv, wxWindow* w, wxSize size, size_t row, size_t col, Gnode* up, Gnode* right, Gnode* down, Gnode* left, int flags)
	: GridCanvas(w, size, emptystring, flags), m_parent(gv), m_up(up), m_right(right), m_down(down), m_left(left)
{
	if (flags & GridCanvasFlag::ROWHEAD)
		this->Bind(wxEVT_RIGHT_DOWN, &Gnode::OnRightClick, this);
	if (flags & GridCanvasFlag::COLHEAD)
		this->Bind(wxEVT_LEFT_DCLICK, &Gnode::OnSortClick, this);
}

//void Gnode::BindMyThread()
//{
//	this->Bind(wxEVT_THREAD, &Gnode::OnThreadEvent, this);
//	this->SetMotion();
//}

void Gnode::CopyGnode(const Gnode& gn)
{
	this->CopyGridCanvas(gn);
}

void Gnode::CopyGnodeAttributes(const Gnode& gn)
{
	this->CopyGridCanvasAttributes(gn);
}

void Gnode::CopyRow(Gnode& gn)
{
	this->CopyGridCanvas(gn);

	this->SetNodeToolTip();

	Gnode* swapto = this->GetRight();
	Gnode* swapfrom = gn.GetRight();

	if(swapto && swapfrom)
		swapto->CopyRow(*swapfrom);
}

void Gnode::CopyRowFromBackup(Gnode& gn)
{
	this->CopyGridCanvasFromBackup(gn);

	Gnode* swapto = this->GetRight();
	Gnode* swapfrom = gn.GetRight();

	if (swapto && swapfrom)
		swapto->CopyRowFromBackup(*swapfrom);
}

void Gnode::CopyFromTopNeighbor()
{
	Gnode* neighbor = this->GetUP();
	if (neighbor)
		this->CopyGnode(*neighbor);

	this->SetNodeToolTip();

	neighbor = this->GetRight();
	if (neighbor)
		neighbor->CopyFromTopNeighbor();
}

void Gnode::CopyFromBottomNeighbor()
{
	Gnode* neighbor = this->GetDown();
	if (neighbor)
		this->CopyGnode(*neighbor);

	this->SetNodeToolTip();

	neighbor = this->GetRight();
	if (neighbor)
		neighbor->CopyFromBottomNeighbor();
}

void Gnode::CopyGnodeData(wxVector<GridCanvasData>& v, GridCanvasData* gcd)
{
	if (gcd == v.end())
		return;

	this->CopyGridCanvasData(*gcd);

	this->SetNodeToolTip();

	Gnode* neighbor = this->GetRight();
	if (neighbor)
		neighbor->CopyGnodeData(v, ++gcd);
}

void Gnode::ClearSelf(bool from_bottom)
{
	int flags = -1;
	Gnode* neighbor = from_bottom ? this->GetDown() : this->GetUP();
	if (neighbor)
		flags = neighbor->Getflags();
	else if (this->GetDown())
		flags = this->GetDown()->Getflags();
	this->Clear(flags);

	neighbor = this->GetRight();
	if (neighbor)
		neighbor->ClearSelf(from_bottom);

	this->SetToolTip(NULL);
}

void Gnode::ClearSelf(Gnode* gn)
{
	if (IsNull(gn, "Gnode* in Gnode::ClearSelf is nullptr!"))
		return;

	this->Clear(gn->Getflags());

	gn = gn->GetRight();
	Gnode* neighbor = this->GetRight();

	if (gn && neighbor)
		neighbor->ClearSelf(gn);

	this->SetToolTip(NULL);
}

int Gnode::GetMyFlags()
{
	return this->Getflags();
}

void Gnode::SetValue(wxString& s)
{
	this->m_SetValue(s);
	this->SetNodeToolTip();
}

void Gnode::GetRowData(wxVector<GridCanvasData>& v)
{
	v.push_back(this);
	Gnode* neighbor = this->GetRight();
	if (neighbor)
		neighbor->GetRowData(v);
}

void Gnode::SetUP(Gnode* gn)
{
	this->m_up = gn;
}

void Gnode::SetRight(Gnode* gn)
{
	this->m_right = gn;
}

void Gnode::SetDown(Gnode* gn)
{
	this->m_down = gn;
}

void Gnode::SetLeft(Gnode* gn)
{
	this->m_left = gn;
}

Gnode* Gnode::GetUP()
{
	return this->m_up;
}

Gnode* Gnode::GetRight()
{
	return this->m_right;
}

Gnode* Gnode::GetDown()
{
	return this->m_down;
}

Gnode* Gnode::GetLeft()
{
	return this->m_left;
}

Gnode* Gnode::GetFarthestLeft()
{
	Gnode* neighbor = this->GetLeft();
	Gnode* temp = NULL;

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

wxString Gnode::GetStringValue()
{
	return this->GetCanvasStringValue();
}

wxString Gnode::GetStringBackupValue()
{
	return this->GetCanvasStringBackupValue();
}

double Gnode::GetDoubleBackupValue()
{
	return this->GetCanvasDoubleBackupValue();
}

double Gnode::GetDoubleValue()
{
	return this->GetCanvasDoubleValue();
}

void Gnode::OnRightClick(wxMouseEvent& evt)
{
	wxString ticker = this->m_GetValue();
	if (ticker.IsEmpty())
		return;
	wxCoord x, y;
	evt.GetPosition(&x, &y);
	x += this->GetScreenPosition().x;
	y = this->GetScreenPosition().y - (2 * y);
	wxPoint p(x, y);
	int index = ticker.find(" ");
	if (index != -1)
		ticker = ticker.Mid(0, index);
	this->m_parent->RightClickAlert(ticker, p);
}

void Gnode::OnSortClick(wxMouseEvent& evt)
{
	this->m_parent->OnSortClick(this);
}

void Gnode::OnLeftClick(wxMouseEvent& evt)
{
	Gnode* left = this->GetFarthestLeft();
	this->SetToolTip(left->GetStringValue());
}

void Gnode::SetNodeToolTip()
{
	int flags = this->Getflags();
	if (!(flags & GridCanvasFlag::TOTALROW || flags & GridCanvasFlag::TOTALROWHEAD)
		&& !(flags & GridCanvasFlag::COLHEAD || flags & GridCanvasFlag::ROWHEAD))
	{
		Gnode* left = this->GetFarthestLeft();
		wxString left_value = left->GetStringValue();
		this->SetToolTip(left_value);
	}
}

//void Gnode::OnThreadEvent(wxThreadEvent& evt)
//{
//	this->Refresh();
//}

// GridView functions...

/*
GridNode::GridNode(GridView* gv, wxWindow* w, size_t row, size_t col, GridNode* up, GridNode* right, GridNode* down, GridNode* left, wxString tval, int gridtype)
	: wxStaticText(w, wxID_ANY, tval), m_grid_view(gv), m_parent(w), m_row(row), m_col(col), m_up(up), m_right(right), m_down(down), m_left(left), t_val(tval), Gridtype(gridtype)
{
	this->InitializeGridNode();

	
}

GridNode::GridNode(const GridNode& gn) 
	: wxStaticText(gn.m_parent, wxID_ANY, gn.t_val), m_grid_view(gn.m_grid_view), 
	m_parent(gn.m_parent), m_row(gn.m_row), m_col(gn.m_col), m_up(gn.m_up), m_right(gn.m_right), m_down(gn.m_down), m_left(gn.m_left), 
	t_val(gn.t_val), Gridtype(gn.Gridtype)
{
	this->SetLabel(gn.GetLabel());
	this->SetFont(gn.GetFont());
	if (gn.SummaryItem)
		this->SummaryItem = true;

	this->SetBackgroundColour(gn.GetBackgroundColour());
	this->SetForegroundColour(gn.GetForegroundColour());
}

void GridNode::operator=(const GridNode& gn)
{
	this->m_grid_view = gn.m_grid_view;
	this->m_parent = gn.m_parent;
	this->m_row = gn.m_row;
	this->m_col = gn.m_col;
	this->m_up = gn.m_up;
	this->m_right = gn.m_right;
	this->m_down = gn.m_down;
	this->m_left = gn.m_left;
	this->t_val = gn.t_val;
	this->Gridtype = gn.Gridtype;

	this->SetLabel(gn.GetLabel());
	this->SetFont(gn.GetFont());
	if (gn.SummaryItem)
		this->SummaryItem = true;

	this->SetBackgroundColour(gn.GetBackgroundColour());
	this->SetForegroundColour(gn.GetForegroundColour());
}

GridNode::~GridNode()
{

}

bool GridNode::CopyText(const GridNode& gn)
{
	this->t_val = gn.t_val;
	this->Gridtype = gn.Gridtype;
	this->SetLabel(gn.GetLabel());
	this->SetFont(gn.GetFont());
	if (gn.SummaryItem)
		this->SummaryItem = true;
	
	this->SetBackgroundColour(gn.GetBackgroundColour());
	this->SetForegroundColour(gn.GetForegroundColour());

	this->Refresh();
	return true;
}

bool GridNode::SetNewVal(gridpair gp, bool total_row)
{
	// Check to see if its just adding the same text, if so we just return false...
	if (this->t_val == gp.val)
		return false;
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
		if(IsStringPositive(gp.val))
			this->textcolor = wxColour(13, 158, 20);
		else
			this->textcolor = wxColour(217, 7, 28);
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

bool GridNode::Clear()
{
	this->SetLabel("");
	this->t_val = "";
	this->SetBackgroundColour(this->normal);
	this->Refresh();
	return true;
}

bool GridNode::IsMatch(wxString s)
{
	// If t_val has the * in the ticker because there is an upcoming dividend reinvestment date than the tickerw wont match so we need to add one to it...
	if (this->t_val.find("*") != -1)
	{
		if (s.find("*") == -1)
			s += " *";
	}
	else if (s.find("*") != -1)
	{
		wxString temp = t_val;
		temp += " *";
		if (temp == s)
			return true;
		else
			return false;
	}

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

void GridNode::OnRightClick(wxMouseEvent& evt)
{
	if (this->SummaryItem)
		return;
	evt.SetEventObject(this);
	wxCoord x, y;
	evt.GetPosition(&x, &y);
	x += this->GetScreenPosition().x;
	y = this->GetScreenPosition().y - (2 * y);
	wxPoint p(x, y);
	wxString ticker = this->GetLabel();
	int index = ticker.find(" ");
	if (index != -1)
		ticker = ticker.Mid(0, index);
	this->m_grid_view->RightClickAlert(ticker, p);
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

void GridNode::InitializeGridNode()
{
	this->SetBackgroundColour(this->normal);
	this->Bind(wxEVT_LEFT_DOWN, &GridNode::OnClickEvent, this);
	this->onclick = wxColour(39, 38, 51);

	if (this->t_val.size())
	{
		this->is_empty = false;
	}

	if (GRIDROW == this->Gridtype)
	{
		this->Bind(wxEVT_RIGHT_DOWN, &GridNode::OnRightClick, this);
		this->textcolor = wxColour(182, 203, 242);
		this->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		this->SetForegroundColour(this->textcolor);
	}
	if (GRIDCOL == this->Gridtype)
	{
		this->textcolor = wxColour(213, 220, 227);
		this->SetFont(wxFont(14, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
		this->SetForegroundColour(this->textcolor);
	}
	if (this->Gridtype == -1)
		this->SetFont(wxFont(13, wxFontFamily::wxFONTFAMILY_ROMAN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM));
}
*/
// Private VirtualListView functions....
void VirtualListView::m_SetItemCount()
{
	this->SetItemCount(100);
}

// GridView functions...

GridView::GridView(wxWindow* w, int cols, int _itemsize) : wxGridSizer(cols), m_parent(w), itemsize(_itemsize)
{
	for (size_t i = 0; i < cols; ++i)
		this->headers.push_back(this->GetColTitle(i));
	this->SetTitleRow();
	this->FillGrid();
}

GridView::~GridView()
{
	
}

void GridView::SetNumOfItems(int items)
{
	if (this->itemsize == items)
		return;

	if (itemsize < items)
	{
		Gnode* neighbor = this->summaryrow->GetDown();
		if (neighbor)
			neighbor->CopyFromTopNeighbor();
		this->summaryrow->ClearSelf();
		this->summaryrow = neighbor;
	}
	else
	{
		Gnode* neighbor = this->summaryrow->GetUP();
		if (neighbor)
			neighbor->CopyFromBottomNeighbor();
		this->summaryrow->ClearSelf(true);
		this->summaryrow = neighbor;
	}

	itemsize = items;
}

void GridView::SetNewRow(StockViewerData* svd)
{
	Gnode* first = this->GetEmptyRow();
	if (!first)
	{
		wxFAIL_MSG("Gnode* first in GridView::SetNewRow is NULL!");
		return;
	}

	size_t size = this->GetCols();
	for (size_t i = 0; i < size; ++i)
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

void GridView::UpdateRow(StockViewerData* svd)
{
	size_t size = this->GetCols();
	Gnode* first = this->GetMatch(svd->ticker);
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
/*
void GridView::SetNewRow(StockViewerData* svd, bool total_row)
{
	GridNode* first = this->GetFirstEmptyRow();
	if (!first)
		return;

	if (total_row)
		this->summaryrow = first;

	else if (first->GetUP() == this->summaryrow)
		this->SwapSummaryRow(first);

	this->filledrows++;

	size_t size = this->GetCols();
	for (size_t i = 0; i < size; ++i)
	{
		this->filledNodes++;
		first->SetNewVal(this->GetLabelText(i, svd), total_row);
		this->gridnode.push_back(first);

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
		wxSizerItem* grid = this->GetItem(first);
		if (first->SetNewVal(this->GetLabelText(i, svd), total_row))
			this->gridnode.push_back(first);

		first = first->GetRight();
		if (first)
			continue;
		else
			break;
	}
}
*/
bool GridView::DoesItemExist(wxString s)
{
//	GridNode* first = this->GetFirstMatch(s);
//	if (first)
//		return true;

//	return false;
	return false;
}

bool GridView::ItemExist(wxString& s)
{
	Gnode* first = this->GetMatch(s);
	if (first)
		return true;

	return false;
}

void GridView::Cleanup()
{

}

void GridView::OnClickRowItem(Gnode* gn)
{
	//this->ClickColOff();
	//if (this->grid_row)
	//	this->grid_row->OnClickRowOff();

	//this->grid_row = gn;
}

void GridView::OnClickColItem(Gnode* gn)
{
	//this->ClickRowOff();
	//if (this->grid_col)
	//	this->grid_col->OnClickColOff();

	//this->grid_col = gn;
}

void GridView::OnClickItem(Gnode* gn)
{
	//if (this->clicked_node)
	///	this->clicked_node->OnClickItemOff();

	//this->clicked_node = gn;
}

void GridView::LayoutGrid()
{
	//if (this->filledNodes = this->gridnode.size())
	//	this->Layout();
	//else
	//{
	//	for (auto v : this->gridnode)
	//		v->LayoutMySizer();
	//}
}

void GridView::RightClickAlert(wxString& ticker, wxPoint& point)
{
	this->rightclick_ticker = ticker;
	mainwindow* p = (mainwindow*)this->m_parent->GetParent();
	p->RightClickGrid(ticker, point);
}

wxString GridView::GetRightClickTicker()
{
	return this->rightclick_ticker;
}

bool GridView::RemoveRow(wxString& t)
{
	Gnode* match = this->GetMatch(t);
	if (!match)
	{
		wxFAIL_MSG("GridView::RemoveRow could not find match for ticker: " + t);
		return false;
	}

	Gnode* temp = NULL;
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
	
//	GridNode* match = this->GetFirstMatch(t);
//	if (!match)
//		return false;

//	this->RemoveRow(match);
//	mainwindow* m = (mainwindow*)this->m_parent;
//	this->LayoutGrid();
//	return true;
	return true;
}

// private GridView functions...

void GridView::SetTitleRow()
{
	size_t cols = this->GetCols();
	wxString val = "";
	Gnode* last = nullptr;

	for (size_t i = 0; i < cols; ++i)
	{
		val = this->GetColTitle(i);
		if (i == 0)
		{
			this->head = new Gnode(this, this->m_parent, wxSize(204, 30), (size_t)0, i, NULL, NULL, NULL, NULL, val, GridCanvasFlag::COLHEAD);
			this->Add(this->head, 0, wxALIGN_TOP | wxTOP | wxLEFT, 4);
		}
		else if (i == 1)
		{
			this->head->SetRight(new Gnode(this, this->m_parent, wxSize(204, 30), (size_t)0, i, nullptr, nullptr, nullptr, this->head, val, GridCanvasFlag::COLHEAD));
			this->Add(head->GetRight(), 0, wxALIGN_TOP | wxTOP | wxLEFT, 4);
			last = this->head->GetRight();
		}
		else
		{
			last->SetRight(new Gnode(this, this->m_parent, wxSize(204, 30), (size_t)0, i, nullptr, nullptr, nullptr, last, val, GridCanvasFlag::COLHEAD));
			wxSizerItem* sizer = this->Add(last->GetRight(), 0, wxALIGN_TOP | wxTOP | wxLEFT, 4);
			last = last->GetRight();

		}
		this->filledNodes++;
		++this->gridnode_Size;
	}
}

void GridView::OnSortClick(Gnode* gn)
{
	wxString collabel = gn->GetStringValue();

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

void GridView::SortStrings(Gnode* gn)
{
	acending = acending ? false : true;
	wxVector<GridCanvasData> first;
	wxVector<GridCanvasData> second;
	Gnode* sorting_node = gn->GetDown();
	while (sorting_node)
	{
		Gnode* temp = sorting_node->GetFarthestLeft();
		if (temp == this->summaryrow)
			break;

		second.push_back(GridCanvasData(temp));
		first.push_back(GridCanvasData(sorting_node));

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
					if ((first[i].gn->GetStringValue() > first[i + 1].gn->GetStringValue()))
					{
						sWitch = true;
						GridCanvasData temp = first[i];
						first[i] = first[i + 1];
						first[i + 1] = temp;
					}
				}
				else
				{
					if ((first[i].gn->GetStringValue() < first[i + 1].gn->GetStringValue()))
					{
						sWitch = true;
						GridCanvasData temp = first[i];
						first[i] = first[i + 1];
						first[i + 1] = temp;
					}
				}
			}
		}
	}

	wxVector<wxVector<GridCanvasData>> sorted;
	for (size_t i = 0; i < first.size(); ++i)
	{
		first[i].gn = first[i].gn->GetFarthestLeft();
		wxVector<GridCanvasData> temp;
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

void GridView::SortDouble(Gnode* gn)
{
	acending = acending ? false : true;
	wxVector<GridCanvasData> first;
	wxVector<GridCanvasData> second;
	Gnode* sorting_node = gn->GetDown();
	while (sorting_node)
	{
		Gnode* temp = sorting_node->GetFarthestLeft();
		if (temp == this->summaryrow)
			break;

		second.push_back(GridCanvasData(temp));
		first.push_back(GridCanvasData(sorting_node));

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
					if ((first[i].gn->GetDoubleValue() > first[i + 1].gn->GetDoubleValue()))
					{
						sWitch = true;
						GridCanvasData temp = first[i];
						first[i] = first[i + 1];
						first[i + 1] = temp;
					}
				}
				else
				{
					if ((first[i].gn->GetDoubleValue() < first[i + 1].gn->GetDoubleValue()))
					{
						sWitch = true;
						GridCanvasData temp = first[i];
						first[i] = first[i + 1];
						first[i + 1] = temp;
					}
				}
			}
		}
	}

	wxVector<wxVector<GridCanvasData>> sorted;
	for (size_t i = 0; i < first.size(); ++i)
	{
		first[i].gn = first[i].gn->GetFarthestLeft();
		wxVector<GridCanvasData> temp;
		first[i].gn->GetRowData(temp);
		sorted.push_back(temp);
	}

	for (size_t i = 0; i < sorted.size(); ++i)
	{
		if (sorted[i].begin() != sorted[i].end() && sorted[i].begin()->gn == second[i].gn )
			continue;

		second[i].gn->CopyGnodeData(sorted[i], sorted[i].begin());
	}
}

void GridView::FillGrid()
{
	Gnode* last = nullptr;
	Gnode* last_row = this->head;
	Gnode* current_row = nullptr;
	size_t rowsize = this->GetCols();
	size_t size = rowsize * 2;
	size_t colsize = this->GetCols();
	for (size_t i = 0; i < size; ++i)
	{
		if (i > 0)
			last_row = current_row;
		for (size_t j = 0; j < colsize; ++j)
		{
			int flags = GetGridNodeFlags(j, i);
			if (j == 0)
			{
				last_row->SetDown(new Gnode(this, this->m_parent, wxSize(204, 30), i + 1, (size_t)j, last_row, nullptr, nullptr, nullptr, flags));
				this->Add(last_row->GetDown(), 0, wxALIGN_TOP | wxTOP | wxLEFT | wxEXPAND, 4);
				last = current_row = last_row->GetDown();
				if (i == this->itemsize + 1)
					this->summaryrow = last_row;
			}
			else
			{
				Gnode* lastrowitem = this->GetRight(last_row, j);
				Gnode* currentrowitem = this->GetFarthestRight(current_row);
				if (currentrowitem)
				{
					currentrowitem->SetRight(new Gnode(this, this->m_parent, wxSize(204, 30), i + 1, j, lastrowitem, nullptr, nullptr, currentrowitem, flags));
					wxSizerItem* sizer2 = this->Add(currentrowitem->GetRight(), 0, wxALIGN_TOP | wxTOP | wxLEFT | wxEXPAND, 4);
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
//	if (this->grid_row)
//		this->grid_row->OnClickRowOff();

//	this->grid_row = nullptr;
}

void GridView::ClickColOff()
{
//	if (this->grid_col)
//		this->grid_col->OnClickColOff();

//	this->grid_col = nullptr;
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
		return gridpair(svd->GetDayGain() + "%", true);
	if (collabel == WEEK_GAIN)
		return gridpair(svd->GetWeekGain(), true);
	if (collabel == QUARTER_GAIN)
		return gridpair(svd->GetQuarterGain(), true);
	if (collabel == YEAR_GAIN)
		return gridpair(svd->GetYearGain(), true);
	if (collabel == TOTAL_GAIN)
		return gridpair(svd->GetTotalGain() + "%", true);
	
	if (collabel == DAY_GAIN$)
		return gridpair("$" + svd->GetDayReturn$(), true);
	if (collabel == WEEK_GAIN$)
		return gridpair("$" + svd->GetWeekReturn$(), true);
	if (collabel == QUARTER_GAIN$)
		return gridpair("$" + svd->GetQuarterReturn$(), true);
	if (collabel == YEAR_GAIN$)
		return gridpair("$" + svd->GetYearReturn$(), true);
	if (collabel == TOTAL_GAIN$)
		return gridpair("$" + svd->GetTotalReturn$(), true);
	
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
	if (collabel == _EX_DIV)
		return gridpair(svd->GetExDivDate(), false);
	if (collabel == _SECTOR)
		return gridpair(svd->GetSectorName(), false);
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

wxString GridView::GetStringLabel(size_t& col, StockViewerData* svd)
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
		return svd->GetWeekGain();
	if (collabel == QUARTER_GAIN)
		return svd->GetQuarterGain();
	if (collabel == YEAR_GAIN)
		return svd->GetYearGain();
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

wxString GridView::GetColTitle(size_t col)
{
//	if (col >= this->headers.size())
//	{
//		wxMessageBox("colsize is greater than the headers size in GridView::GetcolTitle!");
//		return "";
//	}
//
//	return this->headers[col];

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

int GridView::GetGridNodeFlags(size_t& col, size_t& row)
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

Gnode* GridView::GetEmptyRow()
{
	Gnode* next = this->head->GetDown();
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

Gnode* GridView::GetMatch(wxString& s)
{
	Gnode* next = this->head->GetDown();
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

Gnode* GridView::GetFirstEmptyRow()
{
	Gnode* next = this->head->GetDown();
	if (!next)
		return next;
	while (1)
	{
		if (next->IsEmpty())
			return next;
		next = next->GetDown();
		if (!next)
			return NULL;
	}

	return NULL;
}

Gnode* GridView::GetFarthestRight(Gnode* g)
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

Gnode* GridView::GetRight(Gnode* g, size_t i, size_t start)
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

Gnode* GridView::GetLeft(Gnode* g, size_t i, size_t start)
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

Gnode* GridView::GetUp(Gnode* g, size_t i, size_t start)
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

Gnode* GridView::GetDown(Gnode* g, size_t i, size_t start)
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
/*
void GridView::RemoveRow(GridNode* g)
{
	GridNode* remove = g;
	GridNode* down = this->GetDown(g, 1);
	if (down == remove)
	{
		// Reached end of grid...
		this->ClearRow(remove);
		return;
	}

	// we need to move the row below it up to the row we are removing...
	GridNode* next = down;
	GridNode* temp = next;
	while ((next && temp) && (!next->IsEmpty() && !temp->IsEmpty()))
	{
		this->MoveRowUp(next);
		next = temp->GetDown();
		if (!next->IsEmpty())
			temp = next;
	}
	this->ClearRow(temp);
}

void GridView::MoveRowUp(GridNode* gn)
{
	GridNode* down = gn;
	GridNode* up = gn->GetUP();
	if (!up)
	{
		wxFAIL_MSG("GridView::MoveRowUp gn->GetUp is nullptr!");
		return;
	}

	while (gn && up)
	{
		up->CopyText(*down);
		down = down->GetRight();
		up = up->GetRight();
	}
}

void GridView::ClearRow(GridNode* gn)
{
	while (gn)
	{
		gn->Clear();
		gn = gn->GetRight();
	}
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

void GridView::HighlightRow(GridNode* gn)
{
	
}

void GridView::HighlightCol(GridNode* gn)
{
	
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
*/
// GridItemView functions...
GridItemView::GridItemView(wxWindow* w, _PortfolioType t, int col, int _itemsize)
	: GridView(w, col, _itemsize), m_parent(w), type(t)
{

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
		this->Sleep(1);
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

	wxPostEvent(this->m_parent, wxThreadEvent());
	return (wxThread::ExitCode)0;
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

void PortfolioWin::OnClick(wxString& val)
{
	
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

	/*
	wxStaticLine* line1 = new wxStaticLine(panel, wxID_ANY);
	wxBoxSizer* line1H = new wxBoxSizer(wxHORIZONTAL);
	line1->SetForegroundColour(wxColour(255, 255, 255));
	line1H->Add(line1, 1, wxEXPAND);

	wxStaticLine* line2 = new wxStaticLine(panel, wxID_ANY);
	wxBoxSizer* line2H = new wxBoxSizer(wxHORIZONTAL);
	line2->SetBackgroundColour(wxColour(255, 255, 255));
	line2H->Add(line2, 1, wxEXPAND);

	wxStaticLine* line3 = new wxStaticLine(panel, wxID_ANY);
	wxBoxSizer* line3H = new wxBoxSizer(wxHORIZONTAL);
	line3H->Add(line3, 1, wxEXPAND);

	wxStaticLine* line4 = new wxStaticLine(panel, wxID_ANY);
	wxBoxSizer* line4H = new wxBoxSizer(wxHORIZONTAL);
	line4H->Add(line4, 1, wxEXPAND);
	*/
	// Initialize sizers...
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
	this->_payment_date = div.payment_date.IsValid() ? div.payment_date.Format(STANDARD_DATE) : "";
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
	case _EnterDialog::SECTORSTOCKWIN: this->secstock = reinterpret_cast<wxVector<SectorStock>*>(t); this->CreateSectorStockWin(); break;
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

void Dialog::CreateStockEntry()
{
	wxColour c(205, 209, 206);
	SetColor(*this, wxColour(157, 157, 163));
//	this->SetBackgroundColour(wxColour(157, 157, 163));
	// init controls...
//	this->s_ticker = new wxStaticText(this, wxID_ANY, "Enter Ticker");
//	this->m_ticker = new wxTextCtrl(this, wxID_ANY);
//	this->m_ticker->SetValidator(wxTextValidator(wxFILTER_ALPHA | wxFILTER_EMPTY, &this->_ticker));
//	this->m_ticker->GetValidator()->Bind(wxEVT_KEY_DOWN, &Dialog::OnKeyDown, this);
//	SetColor(*this->m_ticker, c);

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

//	wxStaticLine* stat1 = new wxStaticLine(this, wxID_ANY);
//	wxStaticLine* stat2 = new wxStaticLine(this, wxID_ANY);
// 
//	stat1->SetForegroundColour("Black");
//	stat2->SetForegroundColour("Black");
	

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

void Dialog::CreateSectorStockWin()
{
	SetColor(*this, wxColour(207, 208, 212));
	wxColour green(5, 166, 10), red(201, 4, 27);
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
	int id = evt.GetId();
	SectorClass& sc = GetSectorClass();
	ParentSector* ps = sc.GetSector(_Sector(id));
	wxString name = ps ? ps->GetSectorName() : "Sector";

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
	int id = evt.GetId();
	SectorClass& sc = GetSectorClass();
	ParentSector* ps = sc.GetSector(_Sector(id));
	wxString name = ps ? ps->GetSectorName() : "Sector";

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
	this->it++;
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

	sectortext->IsShownOnScreen();
	wxPoint point = this->lastticker->GetPosition();

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
	SectorClass& sc = GetSectorClass();
	sc.ActivateData();
	Parser parser(this, _Sector::INDUSTRIALS);
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
	this->Bind(wxEVT_THREAD, &mainwindow::OnQuoteThread, this);
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
	top->Add(this->grid_panel, 4, wxEXPAND);
	main->Add(top, 3, wxEXPAND);
	main->Add(bottom_frame, 1, wxEXPAND);
	this->SetSizer(main);
	this->UpdateGridView();
	this->Layout();
	this->Show(true);
}

mainwindow::~mainwindow()
{
	this->DeletePopupMenu();
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

	wxArrayString string;
	for (auto& v : sorted)
	{
		wxString item = "";
		wxString Bool = v.DivReinvest ? "Yes" : "No";
		wxString payable_date = v.payment_date.IsValid() ? v.payment_date.Format(STANDARD_DATE) : "NA";
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
	return;

	wxColour green = wxColour(5, 166, 10),  red = wxColour(201, 4, 27);
	wxFont plain = wxFont(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD);
	wxFont title = wxFont(20, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD);

	wxFrame* frame = new wxFrame(this, wxID_ANY, "SectorStocks");
	wxScrolled<wxWindow>* P = new wxScrolled<wxWindow>(frame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	P->SetScrollbars(25, 25, 50, 50);
	P->EnableScrolling(true, true);

	P->SetMinSize(wxSize(wxGetDisplaySize().x / 2, wxGetDisplaySize().y / 2));
	wxBoxSizer* scrmain = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* main = new wxBoxSizer(wxVERTICAL);

	wxStaticText* sectortext = new wxStaticText(frame, wxID_ANY, sector);
	sectortext->SetFont(title);

	main->Add(sectortext, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

	wxBoxSizer* vert1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* vert2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* vert3 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* vert4 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* vert5 = new wxBoxSizer(wxVERTICAL);

	int i = 0;
	for (auto it = v->begin(); it != v->end(); ++it)
	{
		if (i > 99)
			break;
		if (it == v->begin())
		{
			wxStaticText* t = new wxStaticText(P, wxID_ANY, "Ticker");
			t->SetFont(plain);
			vert1->Add(t, 0, wxALIGN_LEFT | wxALL, 5);
			 
			wxStaticText* p = new wxStaticText(P, wxID_ANY, "Price");
			p->SetFont(plain);
			vert2->Add(p, 0, wxALIGN_LEFT | wxALL, 5);

			wxStaticText* c = new wxStaticText(P, wxID_ANY, "Change");
			c->SetFont(plain);
			vert3->Add(c, 0, wxALIGN_LEFT | wxALL, 5);

			wxStaticText* pc = new wxStaticText(P, wxID_ANY, "Percent Change");
			pc->SetFont(plain);
			vert4->Add(pc, 0, wxALIGN_LEFT | wxALL, 5);

			wxStaticText* mc = new wxStaticText(P, wxID_ANY, "Market Cap");
			mc->SetFont(plain);
			vert5->Add(mc, 0, wxALIGN_LEFT | wxALL, 5);
		}

		wxStaticText* temp = new wxStaticText(P, wxID_ANY, it->ticker);
		temp->SetFont(plain);
		vert1->Add(temp, 0, wxALIGN_LEFT | wxALL, 5);

		wxStaticText* price = new wxStaticText(P, wxID_ANY, it->price);
		price->SetFont(plain);
		vert2->Add(price, 0, wxALIGN_LEFT | wxALL, 5);

		wxStaticText* change = new wxStaticText(P, wxID_ANY, it->change);
		change->SetFont(plain);
		vert3->Add(change, 0, wxALIGN_LEFT | wxALL, 5);
		SetStaticTextColor(*change, green, red);

		wxStaticText* percent_change = new wxStaticText(P, wxID_ANY, it->percent_change + '%');
		percent_change->SetFont(plain);
		vert4->Add(percent_change, 0, wxALIGN_LEFT | wxALL, 5);
		SetStaticTextColor(*percent_change, green, red);

		wxStaticText* market_cap = new wxStaticText(P, wxID_ANY, it->market_cap);
		market_cap->SetFont(plain);
		vert5->Add(market_cap, 1, wxALIGN_LEFT | wxALL, 5);
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

	frame->SetSizer(main);
	main->Fit(frame);
	frame->CenterOnParent();
	frame->Layout();
	frame->Show();
}

void mainwindow::RightClickGrid(wxString& ticker, wxPoint& p)
{
	this->rightclick_ticker = ticker;
	this->p_sell->SetItemLabel("Sell " + ticker);
	this->p_buy->SetItemLabel("Purchase " + ticker);
	this->p_view_dividends->SetItemLabel("View " + ticker + " Dividends");
	bool result = PopupMenu(this->popup, p);
}

void mainwindow::OnThreadCompletion()
{
	this->UpdateGridView();
	this->Layout();
	this->Show(true);
}

void mainwindow::OnThreadCompletion(wxThreadEvent& evt)
{
	this->OnThreadCompletion();
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
				this->grid_view->RemoveRow(ticker);
			
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

void mainwindow::OnSellPopupClick(wxCommandEvent&)
{
	wxString ticker = this->grid_view->GetRightClickTicker();
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
	wxString ticker = this->grid_view->GetRightClickTicker();
	wxString longname = this->port.StageStockandGetLongName(ticker);
	longname = Decipherhtmlcodes(longname);

	this->PurchaseWin(ticker, longname);
}

void mainwindow::OnAddReInvestSharesPopup(wxCommandEvent& evt)
{
	wxString ticker = this->grid_view->GetRightClickTicker();
	wxString longname = this->port.StageStockandGetLongName(ticker);
	longname = Decipherhtmlcodes(longname);

	this->ReInvestSharesWin(ticker, longname);
}

void mainwindow::OnQuoteLookupPopup(wxCommandEvent& evt)
{
	wxString ticker = this->grid_view->GetRightClickTicker();
	this->quotethread = new MyQuoteThread(&this->port, this, ticker);
	this->quotethread->Run();
}

void mainwindow::OnQuoteThread(wxThreadEvent& evt)
{
	if (smd.Longname == "NotFound")
	{
		wxMessageDialog* d = new wxMessageDialog(this, "Data for " + this->grid_view->GetRightClickTicker() + " could not be found!");
		if (d->ShowModal())
			d->Destroy();
		this->DeleteQuoteThread();
		return;
	}

	this->dialog = new Dialog(_EnterDialog::QUOTE_WIN, this, wxID_ANY, wxDefaultPosition, wxDefaultSize, this->grid_view->GetRightClickTicker(), &smd);
}

void mainwindow::OnAddDividendPopup(wxCommandEvent& evt)
{
	wxString ticker = this->grid_view->GetRightClickTicker();
	wxString longname = this->port.StageStockandGetLongName(ticker);

	this->AddDividendWin(ticker, longname);
}

void mainwindow::OnViewDividendPopup(wxCommandEvent& evt)
{
	wxString ticker = this->grid_view->GetRightClickTicker();
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
		sd = port.QuoteLookup(this->generic_entry->GetValue());
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
	wxScrolled<wxWindow>* P = new wxScrolled<wxWindow>(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	P->SetScrollbars(25, 25, 50, 50);
	P->EnableScrolling(true, true);
	P->SetBackgroundColour(wxColour(0, 0, 0));
	this->grid_view = new GridView(P, 30, size);
	
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
	this->grid_view->SetNumOfItems(stocks.size());
	for (size_t i = 0; i < stocks.size(); ++i)
	{
		StockViewerData* svd = stocks[i]->GetStockViewerData();
		if (this->grid_view->ItemExist(svd->ticker))
			this->grid_view->UpdateRow(svd);
		else
			this->grid_view->SetNewRow(svd);
	}
	
	StockViewerData* svd = this->port.GetStockViewerData();
	if (this->grid_view->ItemExist(svd->ticker))
		this->grid_view->UpdateRow(svd);
	else
		this->grid_view->SetNewRow(svd);

	this->grid_view->LayoutGrid();

	if (this->portwin)
		this->portwin->Update();
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

void mainwindow::CreatePopupMenu()
{
	this->popup = new wxMenu();
	this->p_sell = new wxMenuItem(popup, _MenuItemIDs::P_SELL_STOCK, "Sell");
	this->p_buy = new wxMenuItem(popup, _MenuItemIDs::P_STOCK_PURCHASE, "Purchase");
	this->p_quote = new wxMenuItem(popup, _MenuItemIDs::P_QUOTE, "Quote Lookup");
	this->p_add_div_reinvest = new wxMenuItem(popup, _MenuItemIDs::P_ADD_DIV_REINVEST, "Add Div Re-invest shares");
	this->p_add_div = new wxMenuItem(popup, _MenuItemIDs::P_ADD_DIV, "Add Dividend");
	this->p_view_dividends = new wxMenuItem(popup, _MenuItemIDs::P_VIEW_DIVIDENDS, "Empty");
	this->p_ohlc = new wxMenuItem(popup, _MenuItemIDs::P_GET_OHLC, "Open High Low Close");

	this->popup->Append(this->p_buy);
	this->popup->Append(this->p_sell);
	this->popup->Append(this->p_quote);
	this->popup->Append(this->p_add_div_reinvest);
	this->popup->Append(this->p_add_div);
	this->popup->Append(this->p_view_dividends);
	this->popup->Append(this->p_ohlc);
}