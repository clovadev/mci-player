#include <Windows.h>
#include <stdio.h>
#include "ClassLinker.h"

//필요한 구조체 변수, 배열을 선언한다.
MCIDEVICEID deviceID = 0;	//장치 드라이버 ID 저장
MCI_GENERIC_PARMS mciGenericParms = { 0 };  //일반 구조체 변수
MCI_OPEN_PARMS mciOpenParms = { 0 };		//오디오 파일을 열 때 필요한 구조체 변수
MCI_PLAY_PARMS mciPlayParms = { 0 };		//재생할 때 필요한 구조체 변수
MCI_SEEK_PARMS mciSeekParms = { 0 };		//위치 옮길 때 필요한 구조체 변수
MCI_SET_PARMS mciSetParms = { 0 };			//MCI 설정에 필요한 구조체 변수
MCI_STATUS_PARMS mciStatusParms = { 0 };	//현재 상태를 받아올 때 필요한 구조체 변수


//오디오를 재생하는 함수
int audioPlayer(char *filePath, char *fileName)
{
	int retval;

	//파일 이름에서 확장자를 분리하여 저장한다.
	char fileType[10];
	strcpy_s(fileType, sizeof(fileType), strrchr(fileName, '.'));

	//열 파일의 이름과 형태를 기술한다.
	mciOpenParms.lpstrElementName = filePath;
	if (strcmp(fileType, ".mp3") == 0)
		mciOpenParms.lpstrDeviceType = "mpegvideo";
	else if (strcmp(fileType, ".wav") == 0)
		mciOpenParms.lpstrDeviceType = "waveaudio";
	else if (strcmp(fileType, ".mid") == 0)
		mciOpenParms.lpstrDeviceType = "sequencer";
	else
	{
		printf("해당 파일형식은 지원하지 않습니다. \n");
		return 1;
	}

	//오디오 파일 열기
	retval = openAudioFile(&deviceID, mciOpenParms);
	if (retval != 0)
		return 1;

	//시간 형식을 밀리초로 설정한다.
	retval = setTimeFormat(deviceID, mciSetParms, MCI_FORMAT_MILLISECONDS);
	if (retval != 0)
		return 1;

	//파일을 재생한다.
	retval = playAudioFile(deviceID, mciPlayParms);
	if (retval != 0)
		return 1;

	//재생하는 파일의 이름을 출력한다.
	textcolor(SKY_BLUE);
	printf("  %s\n\n", fileName);
	textcolor(RESET);

	//키보드 입력 감시 스레드를 실행한다.
	int keyInput = 0;
	HANDLE hThread1 = CreateThread(NULL, 0, keyboardInput, &keyInput, 0, NULL);
	if (hThread1 == NULL)
		return 1;
	else
		CloseHandle(hThread1);

	while (1)
	{
		//현재 재생상태를 표시한다.
		retval = showPlayStatus(&keyInput);
		if (retval != 0)
			return 1;

		//키 입력이 감지되면 해당 키의 명령을 수행한다.
		retval = audioController(keyInput);
		if (retval != 0)
			return 1;
		
		//키보드 입력변수를 초기화한다.
		keyInput = 0;
	}

	return 0;
}

//키보드 입력 감시 스레드 함수
DWORD WINAPI keyboardInput(LPVOID arg)
{
	int *keyInput = arg;

	while (1)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x0001)
			*keyInput = VK_SPACE;
		else if (GetAsyncKeyState(VK_LEFT) & 0x0001)
			*keyInput = VK_LEFT;
		else if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
			*keyInput = VK_RIGHT;

		Sleep(100);
	}

	return 0;
}

//재생상태를 보여주는 함수
int showPlayStatus(int *keyInput)
{
	int retval;

	//필요한 변수를 선언한다.
	int mode, position_ms, totalLen_ms;

	while (*keyInput == 0)
	{
		//현재 모드, 현재 위치, 파일 총 길이를 가져온다.
		retval = getCurrentMode(deviceID, mciStatusParms, &mode);
		if (retval != 0)
			return 1;
		retval = getCurrentPosition(deviceID, mciStatusParms, &position_ms);
		if (retval != 0)
			return 1;
		retval = getAudioFileLength(deviceID, mciStatusParms, &totalLen_ms);
		if (retval != 0)
			return 1;

		//커서를 행의 맨 앞으로 옮김
		printf("\r");

		//현재 위치를 표시한다.
		printf(" %d:%02d  ", position_ms / 1000 / 60, position_ms / 1000 % 60);

		//진행 바를 표시한다.
		if (position_ms == 0)
			printf("--------------------------------------------------");
		else
		{
			float percent = position_ms / (float)totalLen_ms * 100;
			int progress = (int)percent / 2;

			for (int i = 1; i <= progress; i++)
				printf("#");
			for (int i = 1; i <= 50 - progress; i++)
				printf("-");
		}

		//총 길이를 표시한다.
		printf("  %d:%02d", totalLen_ms / 1000 / 60, totalLen_ms / 1000 % 60);

		//새로고침 간격을 설정한다.
		Sleep(100);
	}

	//커서를 행의 맨 앞으로 옮김
	printf("\r");

	return 0;
}

//오디오 제어함수
int audioController(int keyInput)
{
	int retval;

	switch (keyInput) {
	case VK_SPACE:
	{
		//일시정지 또는 재개
		int mode = 0;
		getCurrentMode(deviceID, mciStatusParms, &mode);
		if (mode == MCI_MODE_PLAY)
			pauseAudioFile(deviceID, mciGenericParms);
		else if (mode == MCI_MODE_PAUSE)
			resumeAudioFile(deviceID, mciGenericParms);
		break;
	}

	case VK_LEFT:
	{
		//현재 재생위치를 알아냄
		int position_ms;
		retval = getCurrentPosition(deviceID, mciStatusParms, &position_ms);
		if (retval != 0)
			return 1;

		//현재위치에서 10초를 뺀다. (음수면 처음위치로 이동)
		position_ms -= 10000;
		if (position_ms < 0)
			seekAudioFile(deviceID, mciSeekParms, 0);
		else
			seekAudioFile(deviceID, mciSeekParms, position_ms);

		//옮긴 위치에서 재생
		retval = playAudioFile(deviceID, mciPlayParms);
		if (retval != 0)
			return 1;

		break;
	}

	case VK_RIGHT:
	{
		//현재 재생위치를 알아냄
		int position_ms;
		retval = getCurrentPosition(deviceID, mciStatusParms, &position_ms);
		if (retval != 0)
			return 1;

		//전체길이를 알아냄
		int totalLen_ms;
		retval = getAudioFileLength(deviceID, mciStatusParms, &totalLen_ms);
		if (retval != 0)
			return 1;

		//현재위치에서 10초를 더한다. (전체길이를 넘어가면 끝으로 이동)
		position_ms += 10000;
		if (position_ms > totalLen_ms)
			seekAudioFile(deviceID, mciSeekParms, totalLen_ms);
		else
			seekAudioFile(deviceID, mciSeekParms, position_ms);

		//옮긴 위치에서 재생
		retval = playAudioFile(deviceID, mciPlayParms);
		if (retval != 0)
			return 1;

		break;
	}
	}

	return 0;
}