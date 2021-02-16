#include <Windows.h>
#include <stdio.h>


//콘솔의 글자색을 변경한다.
int textcolor(unsigned short color_number)
{
	int retval;

	retval = SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
	if (retval == FALSE)
		return 1;

	return 0;
}

//콘솔에서 커서의 가시성을 설정한다.
int setCursorVisible(BOOL visible_mode)
{
	int retval;

	//커서 정보를 저장할 구조체 변수를 선언한다.
	CONSOLE_CURSOR_INFO cursorInfo = { 0 };
	
	//현재 커서의 정보를 받아온다.
	retval = GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	if (retval == FALSE)
		return 1;

	//Visible 멤버를 설정한다.
	cursorInfo.bVisible = visible_mode;

	//커서를 안보이게 설정한다.
	retval = SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	if (retval == FALSE)
		return 1;

	return 0;
}