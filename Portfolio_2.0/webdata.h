#pragma once
#define CURL_STATICLIB

#include "wx/wx.h"
#include"curl/curl.h"
#include <chrono>


class webdata
{

public:
	webdata(bool redirect = false);
	~webdata();
	bool IsGoodRequest();
public:
	bool postrequest();
	bool getwebdata(wxString&);
	void seturl(wxString);
	void setusername(wxString);
	void setpassword(wxString);
	void DeleteURLs();

	//set how many pages to get data from
	void setpages(int);

public:
	char* url = nullptr;
	char* username = nullptr;
	char* password = nullptr;
	int pages = 1;

private:
	// IS GOOD REQUEST...
	bool IGR = false;
	bool follow_redirect;
};

