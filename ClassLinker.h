/********************************************************************************
*                                                                               *
* ClassLinker.h -- Custom functions compilation                                 *
*                                                                               *
* Made by Lulin Pollux.          MIT License                                    *
*                                                                               *
********************************************************************************/


/* 모든 코드파일에서 쓰이는 전처리문, 구조체 등의 목록
--------------------------------------------------------------------*/
//글자색 변경할 때 숫자 대신 사용
#define RESET 7
#define DARK_BLUE 1
#define DARK_GREEN 2
#define BRIGHT_BLUE 3
#define DARK_RED 4
#define DARK_PURPLE 5
#define DARK_YELLOW 6
#define DARK_WHITE 7
#define GRAY 8
#define BLUE 9
#define GREEN 10
#define SKY_BLUE 11
#define RED 12
#define PURPLE 13
#define YELLOW 14
#define WHITE 15
/*--------------------------------------------------------------------*/


/* Audio Player.c 의 함수 목록
--------------------------------------------*/
//음악을 재생하는 함수
int audioPlayer(char *filePath, char *fileName);

//키보드 입력 감시 스레드 함수
DWORD WINAPI keyboardInput(LPVOID arg);

//오디오 제어함수
int audioController(int keyInput);

//재생상태를 보여주는 함수
int showPlayStatus(int *keyInput);


/* Console.c 의 함수 목록
--------------------------------------------*/
//콘솔의 글자색을 변경한다.
int textcolor(unsigned short color_number);

//콘솔의 커서를 설정한다.
int setCursorVisible(BOOL visible_mode);


/* File.c 의 함수 목록
--------------------------------------------*/
//파일 대화상자로 파일을 선택하는 함수
int fileDialog(_Out_ char *filePath, int filePath_size, _Out_ char *fileName, int fileName_size);


/* MCI Controller.c 의 함수 목록
--------------------------------------------*/
//MCI 오류를 출력하는 함수
int printMciError(int errorCode);

//각 모드에 알맞는 설명을 출력하는 함수
int printModeDescription(int mode);

//오디오 파일을 여는 함수
int openAudioFile(_Out_ MCIDEVICEID *deviceID, MCI_OPEN_PARMS mciOpenParms);

//오디오 파일을 닫는 함수
int closeAudioFile(MCIDEVICEID deviceID, MCI_GENERIC_PARMS mciGenericParms);

//오디오 파일을 재생하는 함수
int playAudioFile(MCIDEVICEID deviceID, MCI_PLAY_PARMS mciPlayParms);

//오디오 파일 재생을 일시정지하는 함수
//메모리에 오디오 파일을 유지시켜, 다시 재생할 때 빠르게 재생한다.
int pauseAudioFile(MCIDEVICEID deviceID, MCI_GENERIC_PARMS mciGenericParms);

//오디오 파일 재생을 재개하는 함수
int resumeAudioFile(MCIDEVICEID deviceID, MCI_GENERIC_PARMS mciGenericParms);

//오디오 파일 재생을 일시정지하는 함수
//메모리에서 오디오 파일을 내려버린다.
int stopAudioFile(MCIDEVICEID deviceID, MCI_GENERIC_PARMS mciGenericParms);

//시간 형식을 설정하는 함수
int setTimeFormat(MCIDEVICEID deviceID, MCI_SET_PARMS mciSetParms, DWORD timeFormat);

//재생 위치를 이동시키는 함수 (단위: Miliseconds)
int seekAudioFile(MCIDEVICEID deviceID, MCI_SEEK_PARMS mciSeekParms, DWORD seekTo_ms);

//현재 모드를 가져오는 함수
int getCurrentMode(MCIDEVICEID deviceID, MCI_STATUS_PARMS mciStatusParms, _Out_ int *mode);

//재생할 파일의 총 길이를 가져오는 함수
int getAudioFileLength(MCIDEVICEID deviceID, MCI_STATUS_PARMS mciStatusParms, _Out_ int *totalLen_ms);

//현재 재생위치를 가져오는 함수
int getCurrentPosition(MCIDEVICEID deviceID, MCI_STATUS_PARMS mciStatusParms, _Out_ int *position_ms);