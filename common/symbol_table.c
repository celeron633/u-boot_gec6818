//#include <stdlib.h>
//#include <string.h>
#include <common.h>

#define FRIENDLYARM_PATH "/images/"

#define russell_memset   memset
#define russell_memcpy   memcpy
#define russell_strncpy  strncpy
#define russell_strcpy   strcpy
#define russell_toupper  toupper
#define russell_tolower  tolower
#define russell_strlen   strlen
#define russell_strncmp  strncmp
#define russell_strcmp   strcmp


struct {
	char name[64];
	char value[256];
} SymbolTab[50];

int RussellAppendSymbol(char _name[], char _value[])
{
	int i;
	int found_slot = 0;
	// printf("append %s %s\n", _name, _value);
	for (i = 0; i < sizeof(SymbolTab) / sizeof(SymbolTab[0]); i++) {
		if (russell_strcmp(SymbolTab[i].name, "") == 0) {
			found_slot = 1;
			break;
		}
		if (russell_strcmp(SymbolTab[i].name, _name) == 0) {
			return -1;
		}
	}
	if (found_slot) {
		russell_strcpy(SymbolTab[i].name, _name);
		russell_strcpy(SymbolTab[i].value, _value);
		setenv(_name, _value);
		return 0;
	}

	return -1;
}

char *RussellQuerySymbol(const char *_name)
{
	int i;
	int found = 0;
	for (i = 0; i < sizeof(SymbolTab) / sizeof(SymbolTab[0]); i++) {
		if (russell_strcmp(SymbolTab[i].name, "") == 0) {
			break;
		}
		if (russell_strcmp(SymbolTab[i].name, _name) == 0) {
			found = 1;
			break;
		}
	}
	if (found) {
		return SymbolTab[i].value;
	}
	return 0;
}

static char *TxtBegin;
static unsigned TxtLen;
static char *TxtPos;
static unsigned LineNo;
void RussellInitScanner(char *begin, unsigned len)
{
	TxtBegin = begin;
	TxtLen = len;
	TxtPos = begin;
	LineNo = 0;
}

char *RussellScannerGetLine(char *buffer)
{
	int count;
 TOP:
	count = 0;
	while (TxtPos < TxtBegin + TxtLen) {
		int c = *TxtPos;
		TxtPos++;
		if (c == '\r') {
			continue;
		}
		if (c == '\n') {
			LineNo++;
			break;
		}
		buffer[count] = c;
		count++;
		if (count >= 1023) {
			break;
		}
	}
	if (count == 0) {
		if (TxtPos < TxtBegin + TxtLen) {
			goto TOP;
		}
		return 0;
	}
	buffer[count] = 0;
	return buffer;
}

void RussellSkipName(char buffer[])
{
	char temp[1024];
	char *s = buffer;
	char *d = temp;
	
	if (buffer == 0) {
		return;
	}
	for (; *s;) {
		int c = *s;
		s++;
		if (c == ' ' || c == '\t' || c == '_' || c == '-') {
			continue;
		}
		*d++ = c;
	}
	*d = 0;
	russell_strcpy(buffer, temp);
}

void RussellSkipSpace(char buffer[])
{
	char temp[1024];
	char *begin = buffer;
	int len;
	for (;;) {
		int c = *begin;
		if (c == 0) {
			break;
		}
		if (c != ' ' && c != '\t') {
			break;
		}
		begin++;
	}
	len = russell_strlen(begin);
	while (len > 0) {
		int c = begin[len - 1];
		if (c != ' ' && c != '\t') {
			break;
		}
		begin[len - 1] = 0;
		len--;
	}
	russell_strcpy(temp, begin);
	russell_strcpy(buffer, temp);
}

void RussellFilePathRegular(char buffer[])
{
	char temp[1024];
	char *p;
	int l;
	if (buffer == 0 || buffer[0] == 0) {
		return;
	}
	russell_strcpy(temp, FRIENDLYARM_PATH);
	l = russell_strlen(FRIENDLYARM_PATH);
	russell_strcpy(temp + l, buffer);
	russell_strcpy(buffer, temp);

	p = buffer;
	for (;;) {
		int c = *p;
		if (c == 0) {
			break;
		}
		if (c == '\\') {
			*p = '/';
		}
		p++;
	}
}

void RussellUpper(char buffer[])
{
	char *p = buffer;
	if (p == 0) {
		return;
	}
	for (;;) {
		int c = *p;
		if (c == 0) {
			break;
		}
		if (c >= 'a' && c <= 'z') {
			*p = c - 'a' + 'A';
		}
		p++;
	}
}

int RussellGetNameValueFromString(char buffer[], char name[], char value[])
{
	char *mid;
	for (mid = buffer + 1; *mid; mid++) {
		if (*mid == '=') {
			break;
		}
	}
	if (*mid != '=') {
		return -1;
	}

	*mid = 0;
	russell_strcpy(name, buffer);
	russell_strcpy(value, mid + 1);
	
	RussellUpper(name);
	RussellSkipName(name);
	if (russell_strlen(name) == 0) {
		return -1;
	}
	RussellSkipSpace(value);
	return 0;
}

int RussellScanWholeConfigFile(const unsigned char *RevPtr, const unsigned RevLen)
{
	char buffer[1024];
	char name[1024];
	char value[1024];
	int IsError = 0;

	RussellInitScanner((char *)RevPtr, RevLen);

	if (!RussellScannerGetLine(buffer)) {
		return 0;
	}
	if (strcmp(buffer, "#This line cannot be removed. by FriendlyARM(www.arm9.net)") != 0) {
		return -1;
	} 

	while (RussellScannerGetLine(buffer)) {
		RussellSkipSpace(buffer);
		if (buffer[0] == '#' || buffer[0] == '\0') {
			continue;
		} else {
			int ret =
			    RussellGetNameValueFromString(buffer, name, value);
			if (ret < 0) {
				IsError = 1;
				break;
			}
			ret = RussellAppendSymbol(name, value);
			if (ret < 0) {
				IsError = 1;
				break;
			}
		}
	}

	if (IsError) {
		return -LineNo;
	}
	return 1;
}

