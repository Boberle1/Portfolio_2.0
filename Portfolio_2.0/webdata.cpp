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

static size_t header_callback(char* buffer, size_t size, size_t nmemb, void* param)
{
	wxString* html = static_cast<wxString*>(param);
	size_t totalsize = size * nmemb;
	html->Append(static_cast<char*>(buffer));
	return totalsize;
}

webdata::webdata(bool redirect) : follow_redirect(redirect)
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

	bool result = false;
	CURLcode res = CURLE_OK;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	CURL* curl = curl_easy_init();
	
	wxString headerfile = "";
	if (curl) {
		--pages;
		CURLcode URL = curl_easy_setopt(curl, CURLOPT_URL, url);
		if (this->follow_redirect)
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		CURLcode writefunc = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, setdata);
//		CURLcode headerfunc = curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
		CURLcode write = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
		CURLcode header = curl_easy_setopt(curl, CURLOPT_WRITEHEADER, &headerfile);
		CURLcode verbose = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//		CURLcode something = curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
//		CURLcode something2 = curl_easy_setopt(curl, CURLOPT_COOKIEJAR, c);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		int index = data.find("Redirecting");
		if (index == -1 || index > 100)
			result = true;
	}

	if (CURLE_OK != res)
	{
		curl_global_cleanup();
		return false;
	}

	curl_global_cleanup();

	return result;
}

void webdata::seturl(wxString s)
{
	if (url)
		DeleteURLs();
	url = StringToChar(s);
}

void webdata::setusername(wxString s)
{
	if (username)
		delete[] username;
	username = StringToChar(s);
}

void webdata::setpassword(wxString s)
{
	if (password)
		delete[] password;
	password = StringToChar(s);
}

void webdata::DeleteURLs()
{
	delete[] url;
	url = nullptr;
	if (username)
		delete[] username;
	if (password)
		delete[] password;

	username = nullptr;
	password = nullptr;
}