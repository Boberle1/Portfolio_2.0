#include "webdata.h"

char* StringToChar(wxString& s)
{
	char* c;
	c = new char[s.size() + 1];
	for (size_t i = 0; i < s.size(); ++i)
		c[i] = s[i];

	c[s.size()] = '\0';

	return c;
}


static size_t setdata(void* buffer, size_t size, size_t nmemb, void* param)
{
	wxString* html = static_cast<wxString*>(param);
	size_t totalsize = size * nmemb;
	html->Append(static_cast<char*>(buffer));
	return totalsize;
}

webdata::webdata()
{

}

webdata::~webdata()
{
	if (url)
		delete[] url;
	url = nullptr;
}

bool webdata::IsGoodRequest()
{
	return IGR;
}

void webdata::setpages(int p)
{
	pages = p;
}

bool webdata::postrequest()
{

	return true;
}

bool webdata::getwebdata(wxString& data)
{
	if (url == nullptr)
		return false;

	if (pages == 0) {
		return true;
	}

	CURLcode res;
	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL* curl = curl_easy_init();
	if (curl) {
		--pages;
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, setdata);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	if (CURLE_OK != res)
	{
		curl_global_cleanup();
		return false;
	}

	curl_global_cleanup();
	return true;
}

void webdata::seturl(wxString s)
{
	if (url)
		DeleteURL();
	url = StringToChar(s);
}

void webdata::DeleteURL()
{
	delete[] url;
	url = nullptr;
}