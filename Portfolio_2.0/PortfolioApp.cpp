#include "PortfolioApp.h"

wxIMPLEMENT_APP(PortfolioApp);

PortfolioApp::PortfolioApp()
{
	
}

bool PortfolioApp::OnInit()
{
	window = new mainwindow();
	return true;
}
