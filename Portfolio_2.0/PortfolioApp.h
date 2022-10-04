#pragma once
#include "mainwindow.h"

class PortfolioApp : public wxApp
{
public:
	PortfolioApp();
	virtual bool OnInit();

public:
	mainwindow* window = nullptr;
};


