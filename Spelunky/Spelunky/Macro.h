#pragma once

/**********************************************************************
@ DEBUG AND SAFETY
***********************************************************************/
	// debug mode 
#ifdef _DEBUG
		//Debug colorchange define
#define DEBUG_COLOR(color) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color)

	// Debug text color attribute
#define DEBUG_WHITE 7
#define DEBUG_LIGHT_WHITE 15
#define DEBUG_BLUE 1
#define DEBUG_GREEN 2
#define DEBUG_CYAN 3
#define DEBUG_RED 4
#define DEBUG_YELLOW 6

	// Debug Text
#define DEBUG_TEXT(color, text) if(_DEBUG){ \
DEBUG_COLOR(color); cout << text << endl;}

	// for check this well activate
#define DEBUG_CONFIRM(text, confirm) if(_DEBUG){            \
DEBUG_COLOR(DEBUG_LIGHT_WHITE); cout << text;               \
DEBUG_COLOR(DEBUG_WHITE);       cout << " [ ";              \
DEBUG_COLOR(DEBUG_CYAN);        cout << confirm;            \
DEBUG_COLOR(DEBUG_WHITE);       cout << " ]" << endl; }

	// for check this well create or delete 
#define DEBUG_GET_TYPE(text, type) if(_DEBUG){              \
DEBUG_COLOR(DEBUG_LIGHT_WHITE); cout << text;               \
DEBUG_COLOR(DEBUG_WHITE);       cout << " \" ";             \
DEBUG_COLOR(DEBUG_GREEN);       cout << type;               \
DEBUG_COLOR(DEBUG_WHITE);       cout << " \"" << endl; }

#else
#define DEBUG_COLOR(color)
#define DEBUG_TEXT(color, text)
#define DEBUG_CONFIRM(text, confirm)
#define DEBUG_GET_TYPE(text, type)

#endif // !_DEBUG || DEBUG

