#pragma once
#define CURL_STATICLIB

#include "wx/wx.h"
#include"curl/curl.h"


class webdata
{

public:
	webdata();
	~webdata();
	bool IsGoodRequest();
public:
	bool postrequest();
	bool getwebdata(wxString&);
	void seturl(wxString);
	void DeleteURL();

	//set how many pages to get data from
	void setpages(int);

public:
	char* url = nullptr;
	int pages = 1;

private:
	// IS GOOD REQUEST...
	bool IGR = false;
};

