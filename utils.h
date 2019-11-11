void showErrorMsg(const char* text) {
	al_show_native_message_box(display, "ERRO",
		"Ocorreu o seguinte erro e o programa sera finalizado:",
		text, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

int random(int lower, int upper) {
	return (rand() %
		(upper - lower + 1)) + lower;
}

void clickOnButton(Button button, Cursor cursor, void (*callback)()) {
	int endXButton = button.x + al_get_bitmap_width(button.bitmap);
	int endYButton = button.y + al_get_bitmap_height(button.bitmap);

	bool onXRange = cursor.x > button.x && cursor.x < endXButton;
	bool onYRange = cursor.y > button.y && cursor.y < endYButton;

	if (onXRange && onYRange) {
		(*callback)();
	}
}

int centerBitmapX(ALLEGRO_BITMAP* bitmap, int width) {
	return (width / 2) - (al_get_bitmap_width(bitmap) / 2);
}

int centerBitmapY(ALLEGRO_BITMAP* bitmap, int height) {
	return (height / 2) - (al_get_bitmap_width(bitmap) / 2);
}

char* getKeyValue(char* strKey, char* strSection, char* strFileName)
{
	long m_lRetValue;
	char ac_Result[255];

	// Get the info from the .ini file	
	m_lRetValue = GetPrivateProfileString((LPCTSTR)strSection, (LPCTSTR)strKey,
		(LPWSTR)"", (LPWSTR)ac_Result, 255, (LPCTSTR)strFileName);

	char* strResult(ac_Result);
	return strResult;
}

CString getKeyValue(CString strKey, CString strSection, CString strFileName)
{
	char ac_Result[255];

	// Get the info from the .ini file	
	long m_lRetValue = GetPrivateProfileString((LPCTSTR)strSection, (LPCTSTR)strKey,
		(LPWSTR)"", (LPWSTR)ac_Result, 255, (LPCTSTR)strFileName);

	CString strResult(ac_Result);
	return strResult;
}

long setKey(CString strValue, CString strKey, CString strSection, CString strFileName)
{
	long m_lRetValue = WritePrivateProfileString(strSection, strKey,
		strValue, strFileName);

	return m_lRetValue;
}

CStringList* getSectionData(CString strSection, CString strFileName)
{
	char ac_Result[4000];  //change size depending on needs
	CStringList* sectionDataList = new CStringList();
	long m_lRetValue;

	m_lRetValue = GetPrivateProfileSection((LPCTSTR)strSection, (LPWSTR)ac_Result, 4000, (LPCTSTR)strFileName);

	CString strSectionData;
	for (int i = 0; i < m_lRetValue; i++)
	{
		if (ac_Result[i] != '\0') {
			strSectionData = strSectionData + ac_Result[i];
		}
		else {
			if (strSectionData != "") {
				sectionDataList->InsertAfter(sectionDataList->GetTailPosition(), strSectionData);
			}
			strSectionData = "";
		}
	}

	return sectionDataList;
}