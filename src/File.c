#include <Windows.h>


//파일 대화상자로 파일을 선택하는 함수
int fileDialog(_Out_ char *filePath, int filePath_size, _Out_ char *fileName, int fileName_size)
{
	int retval;

	//파일 대화상자에 사용하는 구조체 변수, 버퍼를 선언한다.
	OPENFILENAME ofn = { 0 };

	//결과 저장 배열을 초기화한다.
	memset(fileName, 0, fileName_size);

	//구조체 변수를 초기화한다.
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "오디오 파일(mp3, wav, mid)\0*.mp3;*.wav;*.mid\0모든 파일(*.*)\0*.*\0\0";
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = filePath_size;
	ofn.lpstrFileTitle = fileName;
	ofn.nMaxFileTitle = fileName_size;
	ofn.lpstrTitle = "오디오 파일 열기";
	ofn.Flags = OFN_EXPLORER;

	//파일 대화상자를 연다.
	retval = GetOpenFileName(&ofn);
	if (retval != TRUE)
		return 1;

	return 0;
}