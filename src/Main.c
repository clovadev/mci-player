#pragma comment(lib, "winmm.lib")
#include <Windows.h>
#include <stdio.h>
#include "ClassLinker.h"


//메인 함수
int main(int argc, char *args[])
{
	int retval;

	//콘솔창을 설정한다.
	SetConsoleTitle("Audio Player");
	system("mode con cols=65 lines=7");
	retval = setCursorVisible(FALSE);
	if (retval != 0)
		return 1;

	//필요한 배열 선언
	char filePath[1024] = { '\0' };	//파일의 경로 저장
	char fileName[256] = { '\0' };	//파일의 이름 저장

	//오디오 파일을 메인 매개변수로 바로 재생한다.
	if (argc >= 2)
	{
		strcpy_s(filePath, sizeof(filePath), args[1]);
		strcpy_s(fileName, sizeof(fileName), strrchr(filePath, '\\') + 1);
	}
	else
	{
		//파일 대화상자를 사용하여 파일의 경로, 이름을 가져온다.
		printf("재생할 오디오 파일을 선택해주세요... \n");
		retval = fileDialog(filePath, sizeof(filePath), fileName, sizeof(fileName));
		if (retval != 0)
		{
			printf("오류: 파일 열기 실패. \n");
			return 1;
		}
	}

	//콘솔창을 조정한다.
	system("cls");
	printf("\n");

	//음악 재생기를 사용한다.
	retval = audioPlayer(filePath, fileName);
	if (retval != 0)
		return 1;

	//프로그램 종료
	return 0;
}