// Visual Studio에서 작성된 코드입니다.
// 텍스트 게임이기 때문에 puts와 printf가 많다. 이 점 양해해주시기 바랍니다.

#include <stdio.h> // 기본 입출력을 위한 헤더파일
#include <stdlib.h> // 파일 입출력 등을 사용하기 위한 헤더파일
#include <time.h> // 무작위 기능을 구현하기 위해서 사용
#include <string.h> // 문자열이 같은지 비교하는 strcmp함수를 사용하기 위해서 추가한 헤더파일이다.
#include <Windows.h> // 콘솔 그래픽을 지정해주기 위해서 추가해야 하는 헤더파일이다.

#define Story 9 // 총 스토리 수를 지정
#define Special 3 // 스폐셜 스토리 수를 지정

int start(); // 시작
inline void tutorial(); // 게임 설명

inline void next(int y); // 다음으로 넘기는 함수
int penalty(int like); // 선택지 이외를 선택했을 경우 주어지는 벌칙.
inline void gotoxy(int x, int y); // 글자를 원하는 화면 좌표로 이동시켜주는 함수
char do_you_want_to_skip(char stage[]); // 각 스테이지 별로 건너뛰는 것을 물어보는 함수

// 스테이지 함수들
int stage1(int like, char nickname[]);
int stage2(int like, char nickname[]);
int stage3(int like, char nickname[]);
int stage4(int like, char nickname[]);
int stage5(int like, char nickname[]);
int stage6(int like, char nickname[]);
int stage7(int like, char nickname[]);
int stage8(int like, char nickname[]);
int stage9(int like, char nickname[]);

// 스폐셜 스토리 함수들
int special1(int like, char nickname[]);
int special2(int like, char nickname[]);
int special3(int like, char nickname[]);

// 숨겨진 이스터에그
int same_name(int like);
int best_name(int like, char nickname[]);

typedefuct _Ranking // 최고 점수를 담기 위해서 만든 구조체
{
	char nickname[20]; // 이름
	int score; // 점수
}Rank;

int main()
{
	int (*fp[Story])(int, char*) = { stage1, stage2, stage3, stage4, stage5, stage6, stage7, stage8, stage9 }; // 스테이지 진행
	int (*sp[Special])(int, char*) = { special1, special2, special3 }; // 스폐셜 스토리 개방시 작동
	errno_t err; // fopen_s 함수의 경우 반환 자료형이 이것이기 때문에 이것에 그 값을 담는다.
	FILE* rp = NULL; // ranking point 변수, 최고 점수를 확인할 수 있도록 만든 파일 포인터 (r모드 전용)
	FILE* rankp = NULL; // 물론 닫기는 하지만 웬지 같은 걸 쓰면 터져버릴까봐. (실제로 이 기능 만드는데 하루 절반 날려먹음...)
	Rank RANK; // 위에서 선언한 구조체의 변수를 선언
	int i = 0, exit, like = 0, score; // i는 반복문을 위해서, exit은 반환값, like는 호감도, score는 점수를 담기 위한 변수이다.
	char name[20]; // 닉네임을 저장할 변수이다.
	char nickname[20] = { '\0' }; // 플레이어 이름
	char skip; // 건너뛰기를 물어보기 위해서 생성한 변수

	system("tiTle 마연시"); // 콘솔창 제목을 지정해줍니다. tiTle라는 키워드로 설정 가능하며 저 키워드를 제거시 작동이 안 됨.
	gotoxy(55, 0); // 제목을 놓고 싶은 위치에 좌표 지정
	puts("마연시"); // 시작화면 출력
	gotoxy(20, 6); // 메뉴 화면을 놓고 싶은 위치에 좌표 지정
	puts("시작하기 : y\t초기화하기 : f\t최고 점수 확인 : b\t종료하기 : n"); // 메뉴 출력
	gotoxy(90, 28);
	puts("제작자 : 서민준(아키노리 마온)"); // 깨알같은 제작자 명시
	exit = start(); // 메뉴를 받는 함수 호출
	if (exit == 0) // 만약 n을 눌러서 0이 반환되었다면 게임 종료
	{
		return 0;
	}

	system("cls"); // 일단 화면을 깨끗하게 지운다.
	while (getchar() != '\n') {} // 전에 입력한 버퍼 제거

	gotoxy(44, 4);
	puts("튜토리얼을 건너뛰시겠습니까?"); // 튜토리얼을 볼 것인지 혹은 건너뛸 것인지 질문
	gotoxy(42, 6);
	puts("y : 건너뛰기\tn : 튜토리얼 보기"); // 메뉴 제시
	

	while (1)
	{
		static int y = 8; // 일단 y 좌표 지정

		gotoxy(55, y);
		scanf_s("%c", &skip, 1); // 플레이어의 의사 입력받기

		if (skip == 'y') // 스킵하다면 튜토리얼을 볼 필요 없이 그냥 닉네임 설정으로 이동하고 바로 스테이지 플레이
		{
			break;
		}
		else if (skip == 'n') // 튜토리얼을 본다면 튜토리얼 실행
		{
			tutorial();
			break;
		}
		else // 그 두가지 입력이 아니라면 잘못된 입력으로 판단하고 다시 입력받게 한다.
		{
			y++;
			while (getchar() != '\n') {} // 잘못 입력한 버퍼 제거하기
			gotoxy(37, y);
			puts("메뉴를 잘못 선택하셨습니다. 다시 선택해주세요."); // 오류 메시지 출력
			y++;
		}
	}

	gotoxy(24, 4);
	puts("당신의 이름을 입력해주세요.(영어는 19자, 한글은 9자 이내로 작성해주세요.)"); // 20바이트로 배열을 잡았기 때문이다.
	gotoxy(22, 6);
	puts("입력 범위를 넘어선다면 입력 범위만큼만 닉네임으로 들어갑니다."); // gets_s 함수의 특성
	
	while (1)
	{

		gotoxy(55, 10);
		gets_s(nickname, sizeof(nickname));
		break;
	}
	while(getchar() != '\n'); // 혹시 남아있을지 모르는 버퍼 제거
	
	err = fopen_s(&rp, "result.txt", "r"); // 이스터에그를 발동시키기 위해서 파일이 필요하다.
	if (err != 0)
	{
		gotoxy(37, 4);
		printf("파일 열기 실패! 빨리 개발자에게 신고를!\n");
		return 0;
	}
	
	fscanf_s(rp, "%s", RANK.nickname, sizeof(RANK.nickname));
	
	fclose(rp);

	if (strcmp(nickname, "마온") == 0) // 숨겨진 이스터에그 발동.
	{
		char your_name[20] = { "마온(플레이어)" };
		like = same_name(like);
		strcpy_s(nickname, sizeof(your_name), your_name);
	}
	else if (strcmp(nickname, RANK.nickname) == 0) // 위 조건을 만족하지 않았을 때의 발동 조건. 만약 저 둘이 같이 발동된다면 유저들은 매번 같은 이름으로만 플레이하겠지?
	{
		like = best_name(like, nickname);
	}
	

	for (i = 0; i < Story; i++)
	{
		like = fp[i](like, nickname); // 스토리 진행 함수 포인터를 사용하였기 때문에 이런 식으로도 할 수 있다.
	}

	system("cls");

	if (like >= 30) // 특수 조건을 줌으로써 숨겨진 이야기 기능을 구현
	{
		gotoxy(8, 4);
		printf("여기서부터는 스폐셜 이야기입니다. 당신이 얼마나 마온의 호감도를 올렸느냐에 따라 스토리 개수가 달라집니다.\n\n");
		gotoxy(40, 6);
		system("pause");

		like = sp[0](like, nickname); // 함수 포인터 배열을 만들어서 조금 더 간편하게 코드 작성
	}
	if (like >= 35)
	{
		like = sp[1](like, nickname);
	}
	if (like >= 40)
	{
		like = sp[2](like, nickname);
	}

	system("cls");
	
	err = fopen_s(&rp, "result.txt", "r"); // 이 파일이 최고 기록을 저장하는 파일이다.
	if (err != 0)
	{
		gotoxy(37, 4);
		printf("파일 열기 실패! 빨리 개발자에게 신고를!\n");
		return 0;
	}
	gotoxy(40, 6);
	fscanf_s(rp, "%s %d", name, sizeof(name), &score); // 일단 최고 기록으로 등록되어 있는 정보를 복사한다.

	strcpy_s(RANK.nickname, sizeof(name), name); // 그 뒤, 그 정보들을 구조체 멤버에 저장. 아마 처음에 문자열이여서 문자열도 다시 한 번 받아야 하는 것 같다.
	RANK.score = score;

	if (like >= RANK.score) // 만약 최고 점수가 현 플레이어의 호감도보다 낮다면 정보를 갱신한다.
	{
		strcpy_s(RANK.nickname, sizeof(nickname), nickname);
		RANK.score = like;
		gotoxy(22, 4);
		printf("오! 최고 점수를 갱신했네요! 당신은 마온이를 가장 잘 이해해주는 사람이군요!\n\n"); // 정보가 갱신되었을 때 확인 메시지 출력
	}
	else
	{
		gotoxy(18, 4);
		printf("아직 멀었어요! 아직 %s님에 비해서 마온이를 이해하지 못한다고요! 조금 더 분발하세요!\n\n", RANK.nickname); // 최고 기록이 변경되지 않았다는 것을 알려주는 메시지
	}
	
	gotoxy(40, 8);
	printf("현재 최고 점수 : %s님 : %d점\n\n", RANK.nickname, RANK.score); // 현 최고 점수를 보여줌으로써 자신의 점수와 비교할 수 있도록 지원

	fclose(rp); // 우선 현재 최고 점수를 읽어서 정보를 받는 구간의 끝

	if (RANK.score >= score) // 만약 최고 점수가 읽어들인 점수보다 같거나 크다면 (안 바뀌면 내용은 같을 것이고, 바뀌었다면 내용 갱신)
	{
		err = fopen_s(&rankp, "result.txt", "w");
		if (err != 0)
		{
			gotoxy(30, 4);
			printf("파일 열기 실패! 빨리 개발자에게 신고를!\n");
			return 0;
		}

		fprintf_s(rankp, "%s %d", RANK.nickname, RANK.score);

		fclose(rankp); // 여기까지가 새로 갱신된 최고 점수를 다시 바꾸는 구간의 끝
	}

	gotoxy(40, 10);
	printf("플레이어 점수 : %s님 : %d점\n\n", nickname, like); // 최고 점수 말고 본인 기록을 출력
	gotoxy(15, 14);
	printf("참고로 최고 점수는 가장 최근에 최고 점수를 기록한 사람의 데이터가 저장되는 점 양해바랍니다.\n\n"); // 동점의 경우 동시 출력은 실력상 아직 무리이기 때문에 어쩔 수 없다.
	gotoxy(15, 16);
	printf("이야기는 여기까지입니다. 이런 쓸데없는 프로그램을 실행시켜주신 분들께 감사인사를 드립니다.\n"); // 마무리 인사

	return 1;
}

int start()
{
	errno_t er;
	FILE* format = NULL; // 초기화를 할 때 사용하는 파일 포인터
	FILE* best = NULL; // 최고 점수를 확인할 때 사용하는 파일 포인터
	char start;
	char ranker[20]; // 최고 기록을 세운 사람의 이름을 저장할 배열
	int bestscore; // 최고 기록 점수를 저장할 변수
	int y = 8;

	while (1)
	{
		gotoxy(55, y);
		scanf_s("%c", &start, 1); // 메뉴에 있는 보기 중에 하나를 선택합니다.
		if (start == 'y')
		{	
			return 1; // 1을 반환하면서 함수 종료. 그러고 보니 int를 반환하는 함수에서도 return만 쓸 수 있나?
		}
		else if (start == 'f') // 초기화
		{
			er = fopen_s(&format, "result.txt", "w"); // 초기화하기 위한 메뉴이므로 파일을 w 모드로 연다.
			if (er != 0)
			{
				gotoxy(37, y);
				printf("파일 생성 실패! 예상치 못 한 오류인 것 같습니다!\n"); // 만약 생성하지 못 한 경우를 대비한 오류 메시지
				return 0; // 메시지 출력 이후 프로세스 종료
			}

			fprintf(format, "%s %d", "___", -100); // 아무도 안 했고 현 상황에서 -100점은 어떻게 해도 무리니까.
			fclose(format); // 그 뒤, 이 파일은 닫는다.

			while (getchar() != '\n') {} // 방금 입력한 f 지우기
			y++; // 한 칸 내려감으로써 내가 입력한 메뉴로부터 한 칸 내려간 곳에 출력
		}
		else if (start == 'b') // 최고 점수 확인
		{
			y++;
			er = fopen_s(&best, "result.txt", "r"); // 현재 기록되어 있는 최고 점수를 불러온다.
			if (er != 0)
			{
				gotoxy(37, y);
				printf("파일 열기 실패! 빨리 개발자에게 신고를!\n"); // 만약 배포했을 때 같이 있는 메모장을 다운 받지 않은 경우를 대비해 오류 메시지 출력
				return 0; // 프로세스 종료
			}

			fscanf_s(best, "%s %d", ranker, sizeof(ranker), &bestscore); // 파일에 기록되어 있는 최고 기록 보유자의 이름과 최고 기록을 배열과 변수에 저장
			gotoxy(37, y);
			printf("현재 최고 점수는 %s님 : %d점입니다.\n", ranker, bestscore); // 그리고 그 내용을 출력해서 플레이어에게 확인시켜준다.
			fclose(best); // 방금 연 파일 닫기
			
			while (getchar() != '\n') {}; // 방금 입력한 b 제거. 그리고 혹시 남아있을 지 모르는 버퍼 제거
			y++;
		}
		else if (start == 'n') // 종료
		{
			return 0;
		}
		else // 만약 선택지에 없는 것을 골랐을 경우
		{
			while (getchar() != '\n') {} // 버퍼 제거를 위한 방법. 여러 문자를 입력하면 버퍼 안에 저장되기 때문에 일단 제거해줘야 한다.
			y++;
			gotoxy(37, y);
			puts("메뉴를 잘못 선택하셨습니다. 다시 입력해주세요."); // 메시지를 출력함으로써 다시 선택할 수 있도록 한다.
			y++;
		}
	}
}

void tutorial() // 게임 설명
{
	system("cls"); // 게임 메뉴 화면을 지우기 위해서 실행한다.
	
	gotoxy(40, 4);
	puts("마온이와 연애를 해보는 게임입니다.");
	gotoxy(37, 6);
	puts("주의! 오타쿠가 만든 텍스트게임입니다.");
	gotoxy(35, 8);
	puts("마온이의 성별은 당신의 이성으로 생각해주세요.\n");
	gotoxy(33, 10);
	puts("여러분은 얼마나 마온이를 잘 이해할 수 있을까요?\n");
	next(12); // 일단 이 정도에서 1page 소개를 한 것으로 설정.
	gotoxy(41, 4);
	puts("마온이의 초기 호감도는 0입니다.\n");
	gotoxy(26, 6);
	puts("최대한 마온이 당신에게 호감을 가질 수 선택지를 잘 선택해주세요!\n");
	next(8);
}

inline void next(int y) // 미연시같은 곳에서 터치하면 넘어가는 것과 같은 기능의 함수. 엄청 많이 쓰기 때문에 inline을 사용하였다.
{
	gotoxy(40, y);
	system("pause"); // 일시 정지
	system("cls"); // 화면 깨끗하게 지우기
}

int penalty(int like) // 벌칙
{
	static int y = 4;

	srand(time(NULL)); // 무작위 함수 rand 함수를 사용하기 위해서 설정

	gotoxy(33, y);
	puts("선택지에 없는 선택을 하셨습니다. 제작자가 싫어합니다.\n");
	y += 2;
	gotoxy(43, y);
	puts("패널티 : 마온의 호감도가 감소합니다.");
	like -= rand() % 10 + 1; // 무작위로 1 ~ 10이 선택되어 호감도 감소
	y += 2;
	gotoxy(50, y);
	printf("현재 호감도 : %d\n", like); // 감소된 호감도를 출력
	y += 2;
	gotoxy(40, y);
	system("pause"); // 멈추지 않으면 바로 다음으로 넘어가기 때문에 메시지 및 호감도 출력을 볼 수 없다.

	return like;
}

inline void gotoxy(int x, int y) // 가운데 정렬을 목적으로 한다.
{
	COORD Pos;

	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

char do_you_want_to_skip(char stage[])
{
	char skip;

	system("cls");

	gotoxy(44, 4);
	printf("%s을(를) 건너뛰시겠습니까?\n", stage);
	gotoxy(42, 6);
	puts("y : 건너뛰기\tn : 스토리 보기");

	while (1)
	{
		static int y = 8;

		gotoxy(55, y);
		scanf_s("%c", &skip, 1);
		if (skip == 'y' || skip == 'n')
		{
			system("cls");
			return skip;
		}
		else
		{
			y++;
			while (getchar() != '\n') {} // 잘못 입력한 버퍼 제거하기
			gotoxy(37, y);
			puts("메뉴를 잘못 선택하셨습니다. 다시 선택해주세요.");
			y++;
		}
	}
}

int stage1(int like, char nickname[])
{
	int act; // 선택지를 선택할 때 그 수를 저장하기 위한 변수
	char skip; // 스킵 변수를 저장할 변수
	char stage[] = { "stage1" }; // skip 하기 위해서 함수를 호출할 때 매개변수로 넘겨주기 위한 배열

	system("cls"); // 전 화면을 깨끗하게 지우고 새로 시작하기 위해서 설정
	// 이 함수에 버퍼 제거가 없는 것은 이스터에그 함수 발동 시 버퍼를 제거할 것이 없어서 getchar 함수가 작동되어 공백인 화면이 출력되기 때문이다.

	skip = do_you_want_to_skip(stage);
	if (skip == 'n')
	{
		gotoxy(35, 4);
		puts("학교에서 하교를 해야 하는데, 비가 오기 시작했다.");
		next(6);
	}

	gotoxy(33, 4);
	puts("때마침 마온이 우산을 안 가져온 것 같다. 어떻게 할까?\n");

	gotoxy(33, 8);
	printf("%d. 마온이에게 \"우산 안 가져왔어?\"라고 말한다.\n\n", 1);
	gotoxy(33, 10);
	printf("%d. 말 없이 다가가서 머리 위에 자신의 우산을 씌어준다.\n\n", 2);
	gotoxy(33, 12);
	printf("%d. 마온이에게 \"같이 가자!\"고 말한다.\n\n", 3);

	gotoxy(55, 14);
	scanf_s("%d", &act);
	system("cls");

	switch (act)
	{
		case 1:
			gotoxy(35, 4);
			printf("%s : \"마온, 우산 안 가져왔어? 같이 쓰고 갈래?\"\n", nickname);
			next(6);
			gotoxy(40, 4);
			puts("마온 : \"응? 나도 우산 가지고 왔는데?\"");
			next(6);
			gotoxy(45, 4);
			printf("%s : \"아... 그렇구나...\"\n", nickname);
			next(6);
			gotoxy(24, 4);
			puts("당신은 밀려오는 쑥쓰러움 때문에 마온이 부르는 소리도 듣지 못한채 뛰어갔습니다.\n");
			gotoxy(50, 6);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 8);
			system("pause");
			break;
		case 2:
			gotoxy(42, 4);
			puts("다가가서 말 없이 우산을 씌어준다.");
			next(6);
			gotoxy(45, 4);
			puts("마온 : \"뭐 하냐?\"");
			next(6);
			gotoxy(38, 4);
			puts("자세히보니 마온의 손에는 우산이 들려있었다.");
			next(6);
			gotoxy(43, 4);
			printf("%s : \"아...... 미안.\"\n", nickname);
			next(6);
			gotoxy(34, 4);
			puts("마온 : \"아냐, 그래도 나한테 관심을 가져준거잖아? 고마워.\"");
			next(6);
			gotoxy(28, 4);
			puts("그렇게 훈훈하게 마무리되었지만 같이 가는 동안 말을 건네지 못했습니다.\n");
			gotoxy(28, 6);
			puts("하지만 마온은 당신의 배려에 감동한 것 같습니다. 호감도가 증가합니다.\n");
			like++;
			gotoxy(52, 8);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 10);
			system("pause");
			break;
		case 3:
			gotoxy(43, 4);
			printf("%s : \"마온! 같이 가자!\"\n", nickname);
			next(6);
			gotoxy(27, 4);
			puts("당신이 부르는 소리에 뒤돌아보는 마온. 당신을 보고 옅은 미소를 보입니다.\n");
			gotoxy(45, 6);
			puts("마온 : \"그래, 그러든지.\"");
			next(8);
			gotoxy(36, 4);
			puts("그렇게 이런저런 수다를 떨면서 같이 귀가했습니다.\n");
			gotoxy(25, 6);
			puts("평소 홀로 하교하던 마온에게는 색다른 경험이었습니다. 호감도가 증가합니다.\n");
			like += 3;
			gotoxy(50, 8);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 10);
			system("pause");
			break;
		default:
			like = penalty(like);
			break;
	}
	return like;
}

int stage2(int like, char nickname[])
{
	int act;
	char skip;
	char stage[] = { "stage2" };

	system("cls");
	while (getchar() != '\n') {} // 버퍼 제거를 위한 방법. 여러 문자를 입력하면 버퍼 안에 저장되기 때문에 일단 제거해줘야 한다.

	skip = do_you_want_to_skip(stage);
	if (skip == 'n')
	{
		gotoxy(28, 4);
		puts("마온이와 함께 데이트하고 있는 당신. 우연찮게 발견한 가게에 같이 들어간다.");
		next(6);
		gotoxy(34, 4);
		puts("가게에 들어와보니 이곳은 동물 옷을 파는 곳이었다!");
		next(6);
	}

	gotoxy(18, 2);
	puts("일단 가게에 들어왔으니 뭔가 사긴 사야 할 것 같다. 당신은 마온이에게 무엇을 선물할까?");

	gotoxy(30, 6);
	printf("%d. 아니, 아이 쇼핑만 할 수도 있지. 안 그래? 굳이 살 필요 없어!\n\n", 1);
	gotoxy(30, 8);
	printf("%d. 그래도 선물해주면 좋아하겠지? 저 고양이 귀는 어떨까?\n\n", 2);
	gotoxy(30, 10);
	printf("%d. 어, 저 나무늘보 옷...... 마온이 닮지 않았나? 저걸로 할까?\n\n", 3);

	gotoxy(55, 12);
	scanf_s("%d", &act);
	system("cls");

	switch (act) // switch를 통해서 선택지에 따른 다른 결과를 볼 수 있게 만듬.
	{
		case 1:
			gotoxy(34, 4);
			printf("%s : \"우리 그냥 다른 데나 갈까? 살 것도 없고.\"\n", nickname);
			next(6);
			gotoxy(34, 4);
			puts("마온 : \"그럼 대체 왜 들어온거야? 귀찮게 시리.\"");
			next(6);
			gotoxy(45, 4);
			puts("호감도가 감소하였습니다.\n");
			like -= 10;
			gotoxy(48, 6);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 8);
			system("pause");
			break;
		case 2:
			gotoxy(30, 4);
			printf("%s : \"마온, 이 고양이 귀 어때? 잘 어울릴 것 같은데 줄게!\"\n", nickname);
			next(6);
			gotoxy(30, 4);
			puts("마온 : \"그걸 평상시에 쓸 수 있을까? 난 별로 갖고 싶지 않은데...\"");
			next(6);
			gotoxy(45, 4);
			puts("호감도가 감소하였습니다.\n");
			like -= 4;
			gotoxy(48, 6);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 8);
			system("pause");
			break;
		case 3:
			gotoxy(30, 4);
			printf("%s : \"마온, 이 나무늘보 옷, 잘 어울릴 것 같아! 한 번 입어봐!\"\n", nickname);
			next(6);
			gotoxy(26, 4);
			puts("마온 : \"어? 그래, 뭐. 입어보라니 입어는 볼텐데... 너무 기대는 하지 말아줘.\"");
			next(6);
			gotoxy(50, 4);
			puts("갈아입은 마온");
			next(6);
			gotoxy(24, 4);
			printf("%s : \"역시 잘 어울려! 평소 너의 모습이랑 많이 닮아서 그런가? 그거 선물로 줄게!\"\n", nickname);
			next(6);
			gotoxy(42, 4);
			puts("마온 : \"잠깐만, 너무 비싸지 않아?\"");
			next(6);
			gotoxy(12, 4);
			printf("%s : \"괜찮다니까! 넌 너무 돈에 대한 걱정이 많아! 내가 주고 싶어서 그런 거니까 신경 쓰지 마!\"\n", nickname);
			next(6);
			gotoxy(37, 4);
			puts("마온 : \"그래... 그렇다면, 잘 쓸게. 고마워.\"");
			next(6);
			gotoxy(28, 4);
			puts("약간 부끄러워 하는 마온. 그 뒤로 마온은 그 옷을 잠옷으로 썼다고 합니다.\n");
			like += 5;
			gotoxy(50, 6);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 8);
			system("pause");
			break;
		default: // 예외 처리. 벌칙을 주는 것이 좋다고 생각함.
			like = penalty(like);
			break;
	}
	return like;
}

int stage3(int like, char nickname[])
{
	int act;
	char skip;
	char stage[] = { "stage3" };

	system("cls");
	while (getchar() != '\n') {}

	skip = do_you_want_to_skip(stage);
	if (skip == 'n')
	{
		gotoxy(30, 4);
		printf("%s : \"오늘은 너무 덥네. 시원한 거라도 먹으러 갈까?\"\n\n", nickname);
		gotoxy(16, 6);
		printf("너무 더운 날씨에 지친 소리를 내보는 %s, 그리고 마온은 근처에 매점이 있는 것을 발견한다.\n", nickname);
		next(8);
		gotoxy(12, 4);
		puts("마온 : \"그러게. 나도 이런 날씨에 밖에 나오다니. 우리 심연적으로 자살하고 싶어하는 걸지도......\"");
		next(6);
		gotoxy(43, 4);
		puts("마온 : \"뭐라도 먹지 않을래?\"");
		next(6);
	}

	gotoxy(50, 2);
	puts("어떻게 할까?\n");

	gotoxy(35, 6);
	printf("%d. 물 좀 사올테니까 여기서 기다릴래?\n\n", 1);
	gotoxy(35, 8);
	printf("%d. 어? 마온이 사는 거야? 잘 부탁해!\n\n", 2);
	gotoxy(35, 10);
	printf("%d. 그러면 땀 좀 빼게 매콤한 거라도 먹을까?\n\n", 3);

	gotoxy(54, 12);
	scanf_s("%d", &act);

	system("cls");

	switch (act)
	{
		case 1:
			gotoxy(36, 4);
			printf("%s : \"여기서 잠시만 기다려. 물 사올테니까.\"\n", nickname);
			next(6);
			gotoxy(38, 4);
			puts("마온 : \"뭔 헛소리야? 너 더위 먹었어?\"");
			next(6);
			gotoxy(41, 4);
			printf("%s : \'응? 나 뭔가 잘못했나?\'\n", nickname);
			gotoxy(42, 6);
			printf("%s : \"아니? 정상인데?\"\n", nickname);
			next(8);
			gotoxy(32, 4);
			puts("마온 : \"너만 그 시원한 곳으로 피신하겠다는 거냐?!\"\n");
			gotoxy(40, 6);
			puts("그렇게 말하면서 당신을 때리는 마온");
			next(8);
			gotoxy(35, 4);
			printf("%s : \"아, 미안해. 그럼 빨리 가자. 너무 덥다.\"\n\n", nickname);
			gotoxy(28, 6);
			puts("그렇게 그들은 매점에 가서 잠시 동안 시원한 공기에 몸을 식혔습니다.");
			next(8);
			gotoxy(28, 4);
			puts("하지만, 배려가 조금 부족했네요. 조금만 더 생각했으면 좋았을텐데.\n");
			like--;
			gotoxy(48, 6);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 8);
			system("pause");
			break;
		case 2:
			gotoxy(36, 4);
			printf("%s : \"어? 마온이 사는 거야? 잘 부탁해!\n", nickname);
			next(6);
			gotoxy(32, 4);
			puts("마온 : \"너 설마 까먹었냐? 내가 왜 지금 여기 나와있는건데?\"");
			next(6);
			gotoxy(25, 4);
			puts("사실 마온은 오늘 집에서 낭만적인 히키코모리 생활을 즐길 예정이었다.\n");
			gotoxy(14, 6);
			puts("하지만, 봉사활동을 2인 1조로 진행하는 수행평가 때문에 지금 당신과 함께 하고 오는 길이었던 것이었다.");
			next(8);
			gotoxy(43, 4);
			printf("%s : \"윽...... 죄송합니다.\"\n\n", nickname);
			gotoxy(32, 6);
			puts("자신의 잘못을 제대로 깨달은 당신은 마온에게 사과합니다.");
			next(8);
			gotoxy(34, 4);
			puts("마온 : \"뭐, 알았으면 빨리 매점으로 들어가자.\"\n");
			gotoxy(47, 6);
			printf("%s : \"아, 옙!\"\n", nickname);
			next(8);
			gotoxy(24, 4);
			puts("당신의 생각하지 않고 내뱉은 말로 인해서 마온의 기분은 조금 언짢아졌습니다.\n");
			gotoxy(38, 6);
			puts("말 할 때는 생각하고 말하기! 잊지 맙시다!\n");
			like -= 4;
			gotoxy(52, 8);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 10);
			system("pause");
			break;
		case 3:
			gotoxy(36, 4);
			printf("%s :\"그러면 땀 좀 빼게 매콤한 거라도 먹을까?\"\n\n", nickname);
			gotoxy(28, 6);
			printf("%s : \'더운 날에 땀을 흘리면 몸이 시원해진다고 들어본 적이 있다.\'\n", nickname);
			gotoxy(27, 8);
			printf("%s : \'그러기 위해서 매콤한 음식이나 새콤한 음식이 좋다고 들었다.\'\n", nickname);
			next(10);
			gotoxy(26, 4);
			puts("마온 : \"웃기지 마. 물론 매운 음식이 효과적이라고는 해도 매운 건 안 돼!");
			next(6);
			gotoxy(34, 4);
			printf("%s : \"왜 그래? 그리고 방금 너 효과적이라면서......\"\n", nickname);
			next(6);
			gotoxy(32, 4);
			puts("마온 : \"아니! 내가 급식실에서 밥 먹을 때를 떠올려보라고!\"");
			next(6);
			gotoxy(50, 4);
			printf("%s \"급식실?\"\n", nickname);
			next(6);
			gotoxy(34, 4);
			puts("\'그러고 보니 마온, 항상 매운 음식은... 못 먹었지!!!\'");
			next(6);
			gotoxy(36, 4);
			puts("마온 : \"떠오른 모양이네. 뭐, 알았으면 됐어.\"\n");
			gotoxy(20, 6);
			printf("%s : \"아, 미안해. 대신이라고 하기는 뭐하지만 집 근처의 냉면집에 가서 냉면이라도 먹자.\"\n", nickname);
			next(8);
			gotoxy(50, 4);
			puts("마온 : \"그러든지.\"\n");
			gotoxy(14, 6);
			puts("말로는 괜찮은 것 같지만 절대로 괜찮지 않습니다. 조금 더 마온에 대해서 신경써 줄 필요가 있어 보이네요.\n");
			like -= 6;
			gotoxy(50, 8);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 10);
			system("pause");
			break;
		default:
			like = penalty(like);
			break;
	}

	return like;
}

int stage4(int like, char nickname[])
{
	int act;
	char skip;
	char stage[] = { "stage4" };

	system("cls");
	while (getchar() != '\n') {}

	skip = do_you_want_to_skip(stage);
	if (skip == 'n')
	{
		gotoxy(26, 4);
		puts("오늘은 학교 축제 날이다. 당신은 현재 마온과 함께 부스를 돌아다니고 있다.");
		next(6);
		gotoxy(24, 2);
		printf("%s : \"그건 그렇고 먹을거랑 볼거리도 있고, 이번에는 꽤 잘 된 것 같은데?\"\n\n", nickname);
	}

	gotoxy(20, 4);
	puts("마온 : \"그러게. 작년에 비하면 많이 잘 한 것 같다고나 할까? 그래서 어디부터 갈까?\"");

	gotoxy(33, 8);
	printf("%d. 다트 게임 부스\n\n", 1);
	gotoxy(33, 10);
	printf("%d. 페이스 페인팅 부스\n\n", 2);
	gotoxy(33, 12);
	printf("%d. 응? 뭔가 달려오는 소리가 들리는 것 같은데?\n\n", 3);

	gotoxy(52, 14);
	scanf_s("%d", &act);

	system("cls");

	switch (act)
	{
		case 1:
			gotoxy(18, 4);
			printf("%s : \"그러면, 다트 게임하러 가지 않을래? 소문에 의하면 보상도 꽤나 괜찮다고 하더라고.\"\n", nickname);
			next(6);
			gotoxy(26, 4);
			puts("마온 : \"다트 게임이라... 무료로 하는 게임인 것 같으니까 한 번 해볼까?\"");
			next(6);
			gotoxy(42, 4);
			puts("그렇게 다트 게임 부스로 온 두 사람");
			next(6);
			gotoxy(5, 4);
			puts("부스 운영 학생의 말에 의하면 돌아가는 돌림판에 다트를 던져서 맞춘 칸에 있는 미션을 수행하면 되는 게임인 것 같다.");
			next(6);
			gotoxy(43, 4);
			puts("마온 : \"좋아! 그럼 내가 먼저 할게!\"\n");
			gotoxy(48, 6);
			puts("자신있게 다트를 던진다.");
			next(8);
			gotoxy(44, 4);
			printf("마온 : \"어디보자, 벌칙이... ?!\"");
			next(6);
			gotoxy(30, 4);
			puts("마온이 뽑은 벌칙은 바로... 가장 좋아하는 사람에게 고백하기?!");
			next(6);
			gotoxy(30, 4);
			puts("마온 : \"아니, 잠깐만. 무슨 벌칙이 이래? 이거 뭔가 이상한데?!\"\n");
			gotoxy(26, 6);
			puts("그렇게 말하면서 마온은 당신을 바라봅니다. 얼굴이 약간 붉어진 것 같습니다.");
			next(8);
			gotoxy(24, 4);
			printf("%s : \"뭐... 확실히 난처한 벌칙이긴 하네. 괜찮아. 내가 어떻게든 해볼테니까.\"\n\n", nickname);
			gotoxy(50, 6);
			puts("마온 : \"어떻게?\"");
			next(8);
			gotoxy(34, 4);
			printf("%s : \"뭐, 일단 저기 있는 벌칙 면제를 노려본다던가?\"\n\n", nickname);
			gotoxy(26, 6);
			puts("마온 : \"제정신이야? 저게 면적이 얼마나 작은데! 그리고 못 맞추면 너도......\"");
			next(8);
			gotoxy(26, 4);
			printf("%s : \"괜찮아. 못 미덥겠지만 이럴때는 조금은 믿어줬으면 하는데 말이지.\"\n\n", nickname);
			gotoxy(26, 6);
			puts("그렇게 말하며 진지하게 다트를 집어드는 당신을 아무 말 없이 바라보는 마온");
			next(8);
			gotoxy(30, 4);
			puts("당신은 잠시 긴장되는 마음을 가라앉힌 뒤, 과녁을 향해 다트를 던졌다!");
			next(6);
			gotoxy(34, 4);
			puts("마온 : \"뭐야... 진짜로 맞춘거야? 저 좁은 면적을?!\"\n");
			gotoxy(28, 6);
			puts("굉장히 놀라는 마온. 그리고 그런 모습을 보면서 슬쩍 웃는 당신");
			next(8);
			gotoxy(34, 4);
			printf("%s : \"뭐, 내가 한다면 하는 사람이니까 말이지.\"\n\n", nickname);
			gotoxy(34, 6);
			puts("마온 : \"고마워! 답례로 내가 맛있는 거 사줄게!\"");
			next(8);
			gotoxy(28, 4);
			printf("%s : \"오! 마온이 사주는 거라면 뭐든지 땡큐라고! 잘 먹을게.\"\n", nickname);
			next(6);
			gotoxy(22, 4);
			puts("마온이 당신을 좋아하는 지는 알지 못했지만 마온이 돈을 쓴 장면을 봤기에 만족한 당신.\n");
			gotoxy(24, 6);
			puts("또한 자신의 곤란한 상황을 면하게 해준 당신에게 감사를 느끼는 마온. 훈훈하네요.\n");
			like += 6;
			gotoxy(50, 8);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 10);
			system("pause");
			break;
		case 2:
			gotoxy(35, 4);
			printf("%s : \"그러면 페이스 페인팅 부스에 가볼까?\"\n\n", nickname);
			gotoxy(40, 6);
			puts("마온 : \"너 그런 쪽에 관심있었어?\"");
			next(8);
			gotoxy(33, 4);
			printf("%s : \"아니... 그런 건 아닌데... 다 생각이 있지.\"\n", nickname);
			next(6);
			gotoxy(30, 4);
			puts("페이스 페인팅 부스에 도착한 두 사람. 그리고 당황하는 마온.");
			next(6);
			gotoxy(33, 4);
			puts("마온 : \"잠깐만, 설마... 너 내 얼굴에 낙서하려고?!\"\n");
			gotoxy(40, 6);
			printf("%s : \"지금 눈치채도 이미 늦었어!\"\n", nickname);
			next(8);
			gotoxy(22, 4);
			puts("근처에 있던 붓을 꺼내든 당신은 마온의 얼굴에 웃을 수밖에 없는 그림을 그립니다.");
			next(6);
			gotoxy(42, 4);
			puts("마온 : \"자,잠깐만. 너 진짜로!!\"\n");
			gotoxy(26, 6);
			puts("마온도 팔을 뻗어 붓을 꺼낸 뒤, 그의 얼굴에 낙서를 하기 시작합니다.");
			next(8);
			gotoxy(30, 4);
			puts("그렇게 몇 분 동안 진흙탕 싸움이 진행되었고...... 그 결과......");
			next(6);
			gotoxy(43, 4);
			puts("마온 : \"아... 이제 지친다.\"\n");
			gotoxy(42, 6);
			printf("%s : \"그러게... 조금 지나쳤나?\"\n", nickname);
			next(8);
			gotoxy(38, 4);
			puts("마온 : \"뭐, 확실히 그랬을지도. 하지만...\"\n");
			gotoxy(44, 6);
			printf("%s : \"하지만, 뭐?\"\n", nickname);
			next(8);
			gotoxy(45, 4);
			puts("마온 : \"아무것도 아냐.\"\n");
			gotoxy(42, 6);
			printf("%s : \"에? 뭐야, 궁금하게시리.\"\n", nickname);
			next(8);
			gotoxy(30, 4);
			puts("마온 : \'오랫만에 친구와 함께 진심으로 즐겁게 놀고 즐겁게 웃었으니까.\'\n");
			like += 7;
			gotoxy(50, 6);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 8);
			system("pause");
			break;
		case 3:
			gotoxy(26, 4);
			printf("%s : \"마온? 뭔가 이쪽으로 달려오는 것 같은 소리가 들리지 않아?\"\n\n", nickname);
			gotoxy(44, 6);
			puts("마온 : \"그러게? 어디 보자...?!\"");
			next(8);
			gotoxy(34, 4);
			puts("갑자기 어디선가 달려온 메이드 복을 입고 있는 학생.\n");
			gotoxy(38, 6);
			puts("학생 A : \"미안, 잠시만 마온 좀 빌릴게~.\"\n");
			gotoxy(44, 8);
			printf("%s, 마온 : \"자,잠깐만...\"\n\n", nickname);
			gotoxy(14, 10);
			puts("갑자기 나타난 여학생은 마온을 데리고 어디론가 가버렸다. 아무래도 메이드 카페에서 일하는 학생인 것 같다.");
			next(12);
			gotoxy(26, 4);
			printf("%s : \"메이드 카페인가...? 그나저나 왜 데려간 거야? 빨리 데리러 가야지.\"\n", nickname);
			next(6);
			gotoxy(50, 4);
			puts("잠시 뒤, 메이드 카페");
			next(6);
			gotoxy(42, 4);
			printf("%s : \"어이, 마온. 여기 있냐?\"\n\n", nickname);
			gotoxy(44, 6);
			puts("??? : \"어서오세요. 주...\"");
			next(8);
			gotoxy(16, 4);
			puts("당신에게 인사한 건 다름아닌 마온이었다. 왜인지 모르겠지만 마온은 메이드복을 입고 있었다!\n");
			gotoxy(28, 6);
			puts("당신이 여성이라면 마온이 여장을 하고 메이드복을 입었다고 가정하자.");
			next(8);
			gotoxy(50, 4);
			printf("%s : \"......\"\n\n", nickname);
			gotoxy(16, 6);
			puts("마온 : \"착각하지 마! 이건... 그러니까... 널 위해서 입은 게 아니라... 어쩔 수 없이 입은거라고!\"\n");
			gotoxy(42, 8);
			puts("얼굴이 붉게 달아오른 마온이 소리쳤다.");
			next(10);
			gotoxy(20, 4);
			printf("%s : \"이야, 좋은 구경했는데? 설마 네가 이렇게 메이드복이 잘 어울릴줄이야.\"\n\n", nickname);
			gotoxy(44, 6);
			puts("얼굴이 더 붉어지는 마온");
			next(8);
			gotoxy(24, 4);
			puts("마온 : \"으... 어쩔 수 없지. 너도 손님이니까. 일단 자리에 앉으시죠, 주...인님...\"");
			next(6);
			gotoxy(32, 4);
			puts("마온의 그런 모습을 보고 당신도 갑자기 두근거리기 시작합니다.\n");
			gotoxy(12, 6);
			puts("당신은 메이드 마온에게 좋은 대접을 받았습니다. 마온도 의외로 열심히하네요. 당신에게 칭찬받은 덕분일까요?\n");
			like += 3;
			gotoxy(50, 8);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 10);
			system("pause");
			break;
		default:
			like = penalty(like);
			break;
	}

	return like;
}

int stage5(int like, char nickname[])
{
	int act;
	char skip;
	char stage[] = { "stage5" };

	system("cls");
	while (getchar() != '\n') {} // 버퍼 제거를 위한 방법. 여러 문자를 입력하면 버퍼 안에 저장되기 때문에 일단 제거해줘야 한다.

	skip = do_you_want_to_skip(stage);
	if (skip == 'n')
	{
		while (getchar() != '\n') {}

		gotoxy(40, 4);
		puts("마온과 함께 노래방에 왔다. 그런데......");
		next(6);
		gotoxy(42, 4);
		printf("%s : \'너무 못 부른다고!!!\'\n", nickname);
		next(6);
		gotoxy(24, 4);
		printf("%s : \'아니, 나도 못 부르지만, 설마 마온, 나보다 못 부르는 정도였다니!\'\n", nickname);
		next(6);
		gotoxy(48, 4);
		puts("노래를 다 부른 마온");
		next(6);
		gotoxy(8, 4);
		puts("마온 : \"아! 내 노래 별로였지? 못 부르는 건 알고 있었는데... 그래도 온 김에 부르긴 해야 할 것 같아서......\"");
		next(6);
	}
	
	gotoxy(40, 2);
	puts("마온의 노래를 어떻게 평가해야 할까?\n");

	gotoxy(40, 6);
	printf("%d. 노래를 잘 불렀다고 칭찬한다.\n\n", 1);
	gotoxy(40, 8);
	printf("%d. 노래를 못 불렀다고 돌직구를 날린다.\n\n", 2);
	gotoxy(40, 10);
	printf("%d. 연습하면 잘 할 수 있을 거라고 위로한다.\n\n", 3);

	gotoxy(56, 12);
	scanf_s("%d", &act);

	system("cls");

	switch (act)
	{
	case 1:
		gotoxy(36, 4);
		printf("%s : \"잘 부르는데 왜 그래? 나보다 잘 하는구만.\"\n", nickname);
		next(6);
		gotoxy(30, 4);
		puts("마온 : \"웃기고 있네. 나 어차피 못 부르는 거 다 알고 있거든?\"");
		next(6);
		gotoxy(38, 4);
		puts("아무래도 펙트를 말했어야 한 것 같습니다......");
		like -= 2;
		gotoxy(52, 6);
		printf("현재 호감도 %d\n", like);
		gotoxy(40, 8);
		system("pause");
		break;
	case 2:
		gotoxy(24, 4);
		printf("%s : \"솔직히 이 정도로 못 부를줄은 몰랐어. 나도 못 부르는 편인데 말이지.\"\n", nickname);
		next(6);
		gotoxy(38, 4);
		puts("마온 : \"역시 그렇지? 미안, 앞으로는......\"");
		next(6);
		gotoxy(7, 4);
		printf("%s : \"미안해. 네가 노래 부르고 싶지 않았을지도 모르는데 그냥 데리고 와버려서. 그러니까 다른 곳으로 가자.\"\n", nickname);
		next(6);
		gotoxy(22, 4);
		puts("당신은 마온에게 정중히 사과합니다. 마온은 그런 당신을 보고 당황스러워합니다.\n");
		gotoxy(22, 6);
		puts("마온 : \"아냐, 괜찮아. 아직 시간 조금 남았잖아? 이번엔 같이 부르지 않을래?\"\n");
		gotoxy(38, 8);
		puts("그렇게 말하며 남은 마이크를 건네주는 마온");
		next(10);
		gotoxy(36, 4);
		puts("당신은 그 마이크를 받아서 같이 노래를 부릅니다.");
		next(6);
		gotoxy(18, 4);
		puts("당신 탓이 아닌데도 사과하고 같이 노래를 불러준 당신에게 마온은 조금의 호감을 느낍니다.\n");
		like += 2;
		gotoxy(50, 6);
		printf("현재 호감도 : %d\n\n", like);
		gotoxy(40, 8);
		system("pause");
		break;
	case 3:
		gotoxy(34, 4);
		printf("%s : \"괜찮아. 조금만 더 열심히 노력하면 분명......\"\n\n", nickname);
		gotoxy(44, 6);
		puts("마온 : \"잘 할 수 있을 거라고?\"");
		next(8);
		gotoxy(38, 4);
		puts("갑자기 가라앉는 분위기에 당신은 당황합니다.");
		next(6);
		gotoxy(28, 4);
		puts("마온 : \"노력하면 된다. 이건 더 이상 손쓸 도리가 없으니까 하는 말이잖아?\"\n");
		gotoxy(44, 6);
		printf("%s : \"아니... 꼭 그런건......\"\n", nickname);
		next(8);
		gotoxy(34, 4);
		puts("마온 : \"미안하지만 나 먼저 갈게. 돈은 내일 갚을테니까.\"\n");
		gotoxy(46, 6);
		printf("%s : \"자... 잠깐만!\"\n", nickname);
		next(8);
		gotoxy(10, 4);
		puts("당신은 마온을 붙잡으려 하였지만 그러지 못했습니다. 그나마 다행인 점은 마온도 돌아가면서 반성했다는 점이네요.\n");
		like--;
		gotoxy(50, 6);
		printf("현재 호감도 : %d\n", like);
		gotoxy(40, 8);
		system("pause");
		break;
	default:
		like = penalty(like);
		break;
	}
	return like;
}

int stage6(int like, char nickname[])
{
	int act;
	char skip;
	char stage[] = { "stage6" };

	system("cls");
	while (getchar() != '\n') {}

	skip = do_you_want_to_skip(stage);
	if (skip == 'n')
	{
		gotoxy(26, 4);
		printf("마온 : \"저기, %s. 너는 만들어진 모든 것들이 의미가 있다고 생각해?\"\n", nickname);
		next(6);
		gotoxy(30, 4);
		printf("%s : \"뭐야, 갑자기? 하지만, 그러게... 아마 그렇지 않을까?\"\n", nickname);
		next(6);
		gotoxy(44, 4);
		puts("마온 : \"흐음... 그렇구나.\"\n");
		gotoxy(36, 6);
		printf("%s : \"그건 그렇고 갑자기 왜 물어본거야?\"\n\n", nickname);
		gotoxy(44, 8);
		puts("마온 : \"아무것도 아니야.\"\n");
		gotoxy(34, 10);
		printf("%s : \"뭐야, 궁금하게. 뭐, 말 안 할 거면 됐어.\"\n", nickname);
		next(12);
		gotoxy(20, 4);
		puts("당신은 마온이 무슨 의도로 그런 질문을 했는지 궁금했지만 그냥 넘어가기로 했습니다.");
		next(6);
		gotoxy(50, 4);
		puts("그리고 며칠 뒤");
		next(6);
		gotoxy(46, 4);
		puts("마온 : \"자, 받아.\"\n");
		gotoxy(46, 6);
		puts("뭔가를 건네는 마온");
		next(8);
		gotoxy(48, 4);
		printf("%s : \"뭐야?\"\n\n", nickname);
		gotoxy(42, 6);
		puts("마온 : \"일단 받기나 하셔.\"\n");
		gotoxy(42, 8);
		puts("마온이 건네는 것을 받아든 당신");
		next(10);
		gotoxy(46, 4);
		printf("%s : \"응? 초콜릿?\"\n", nickname);
		next(6);
		gotoxy(6, 4);
		puts("마온 : \"뭐, 우리 지금 관계로 지내게 된지도 꽤 된 것 같은데 뭔가 해준게 없는 것 같아서... 한 번 만들어봤어.\"\n");
		gotoxy(37, 6);
		puts("그렇게 말하면서 얼굴이 살짝 붉어지는 마온.");
		next(8);
		gotoxy(30, 4);
		printf("%s : \"그냥 같이 있어주는 것만으로도 충분한데 말이지.\"\n\n", nickname);
		gotoxy(36, 6);
		puts("그 말을 들은 마온은 얼굴이 더욱 붉어집니다.");
		next(8);
		gotoxy(14, 4);
		puts("마온 : \"시끄럽고! 빨리 먹어보기나 하셔! 아, 그리고 나 요리 엄청 못하니까 맛 없어도 모른다고?\"\n");
		gotoxy(34, 6);
		printf("%s : \"와, 그건 조금 신경쓰이는 발언입니다만?\"\n", nickname);
		next(8);
		gotoxy(30, 4);
		puts("그렇게 말하면서 손에 들려있는 초콜릿 한 개를 입에 넣어본다.\n");
		gotoxy(34, 6);
		puts("그리고 얼마 뒤, 강렬하게 전해져 오는 맛. 이건......");
		next(8);
		gotoxy(36, 4);
		printf("%s : \'그래, 마온에게 요리를 기대한 내 잘못이지.\'\n", nickname);
		next(6);
		gotoxy(20, 2);
		puts("그런 생각을 하며 앞을 바라보자 아무래도 마온은 내 평가를 기다리고 있는 것 같았다.\n");
	}
	
	gotoxy(50, 6);
	puts("뭐라고 대답해줄까?\n");

	gotoxy(33, 10);
	printf("%d. 먹기에는 힘들지 모르지만 그래도......\n\n", 1);
	gotoxy(33, 12);
	printf("%d. 뭐, 솔직히 맛 없긴 한데. 나보다는 잘 하는 것 같네.\n\n", 2);
	gotoxy(33, 14);
	printf("%d. 너 그냥 앞으로는 요리 하지 마.\n\n", 3);

	gotoxy(55, 16);
	scanf_s("%d", &act);

	system("cls");

	switch (act)
	{
		case 1:
			gotoxy(35, 4);
			printf("%s : \"먹기에는 힘들지 모르지만 그래도......\"\n\n", nickname);
			gotoxy(48, 6);
			puts("마온 : \"그래도?\"");
			next(8);
			gotoxy(34, 4);
			printf("%s : \"만들어진 것에는 모두 의미가 있는 거니까.\"\n\n", nickname);
			gotoxy(26, 6);
			printf("%s : \'드디어 알 수 있을 것 같았다. 마온이 왜 그런 질문을 했었는지.\'", nickname);
			next(8);
			gotoxy(28, 4);
			printf("%s : \"이 초콜릿, 네가 내가 먹어줬으면 하는 마음을 담아서 만든거잖아?\"\n\n", nickname);
			gotoxy(28, 6);
			printf("%s : \"그런데 그 의미를 다하지 못 하고 끝난다니. 그건 너무 슬프잖아?\"\n\n", nickname);
			gotoxy(32, 8);
			puts("그렇게 말하면서 당신은 손에 들려있는 초콜릿을 집어먹는다.");
			next(10);
			gotoxy(35, 4);
			printf("마온 : \"아냐, %s! 그렇게까지 무리하지 않아도......\"\n\n", nickname);
			gotoxy(35, 6);
			printf("%s : \"아, 잘 먹었어. 네 마음 잘 전해졌다고, 마온.\"\n", nickname);
			next(8);
			gotoxy(32, 4);
			puts("잠시 고개를 떨구는 마온. 그러더니 곧 고개를 들며 말합니다.\n");
			gotoxy(28, 6);
			puts("마온 : \"조금만 기다려. 다음에는 반드시 맛있어서 먹게 해줄테니까.\"\n");
			gotoxy(22, 8);
			puts("그렇게 말하면서 마온은 걸음을 옮깁니다. 당신은 그런 마온을 흐뭇하게 바라봅니다.\n");
			like += 3;
			gotoxy(50, 10);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 12);
			system("pause");
			break;
		case 2:
			gotoxy(30, 4);
			printf("%s : \"뭐, 솔직히 맛 없긴 한데. 나보다는 잘 하는 것 같네.\"\n\n", nickname);
			gotoxy(38, 6);
			puts("마온 : \"그렇구나. 맛 없다는 뜻이구나.\"");
			next(8);
			gotoxy(24, 4);
			printf("%s : \"잠깐만, 맛 없다고 하긴 했지만 그렇게 맛이 없는 것도 아니라고?\"\n\n", nickname);
			gotoxy(26, 6);
			puts("마온 : \"웃기지 마. 네 요리보다 맛있는데 그런 말이 나올리가 없잖아!\"");
			next(8);
			gotoxy(12, 4);
			puts("그 말을 듣고서야 당신은 최근 자신의 집에서 해준 요리를 마온이 엄청 맛있게 먹은 기억을 떠올립니다.\n");
			gotoxy(48, 6);
			printf("%s : \"아......\"\n", nickname);
			next(8);
			gotoxy(36, 4);
			puts("마온 : \"맛 없으면 돌려줘. 내가 먹을테니까.\"\n");
			gotoxy(30, 6);
			puts("그렇게 말하면서 손 위에 올려져있던 초콜릿을 가져가는 마온.");
			next(8);
			gotoxy(45, 4);
			printf("%s : \"자,잠깐만, 마...\"\n\n", nickname);
			gotoxy(14, 6);
			puts("그녀를 불러세우려했지만 어떤 말을 해주면 좋을지 떠올리지 못한 당신은 그저 자리에 멈춰서고 맙니다.\n");
			like -= 2;
			gotoxy(50, 8);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 10);
			system("pause");
			break;
		case 3:
			gotoxy(38, 4);
			printf("%s : \"너 그냥 앞으로는 요리 하지 마.\"\n", nickname);
			next(6);
			gotoxy(22, 4);
			puts("마온 : \"와, 너무하네, 진짜. 맛없을거라고 알고는 있었지만 그런 말이 나오는건......");
			next(6);
			gotoxy(36, 4);
			printf("%s : \"앞으로는 계속 내가 해줄테니까.\"\n\n", nickname);
			gotoxy(26, 6);
			puts("잠깐 경직하는 마온. 갑작스러운 상황에 잠시 머리가 어지러운 것 같습니다.");
			next(8);
			gotoxy(26, 4);
			puts("이내 사고회로가 터져버린 마온은 얼굴을 붉힌 채 어디론가 뛰쳐가버립니다.\n");
			gotoxy(30, 6);
			puts("하지만 당신은 쫓아가지 않습니다. 아니, 쫓아가지 못 한 거죠.\n");
			gotoxy(32, 8);
			puts("당신도 얼굴이 마온만큼 새빨갛습니다. 괜찮은건가요?");
			next(10);
			gotoxy(28, 4);
			puts("하지만, 뭐, 갑작스럽긴해도 둘 사이에 큰 진전이 있을 것 같네요.\n");
			like += 9;
			gotoxy(50, 6);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 8);
			system("pause");
			break;
		default:
			like = penalty(like);
			break;
	}

	return like;
}

int stage7(int like, char nickname[])
{
	int act;
	char skip;
	char stage[] = { "stage7" };

	system("cls");
	while (getchar() != '\n') {}
	skip = do_you_want_to_skip(stage);
	if (skip == 'n')
	{
		gotoxy(46, 4);
		puts("마온 : \"놀이공원?\"\n");
		gotoxy(44, 6);
		printf("%s : \"그래, 놀이공원!\"\n\n", nickname);
		gotoxy(38, 8);
		puts("당신은 그렇게 말하면서 표를 건넵니다.");
		next(10);
		gotoxy(32, 4);
		puts("마온 : \"데이트 신청은 그렇다치고, 왜 놀이공원이야?\"\n");
		gotoxy(28, 6);
		printf("%s : \"부모님이랑 같이 장보러 갔는데 제비뽑기에서 뽑았지.\"\n", nickname);
		next(8);
		gotoxy(38, 4);
		puts("마온 : \"뭐, 상관없나. 그럼 내일 보자.\"\n");
		gotoxy(48, 6);
		printf("%s : \"그래.\"\n", nickname);
		next(8);
		gotoxy(54, 4);
		puts("다음 날");
		next(6);
		gotoxy(44, 4);
		printf("%s : \"마온, 기다렸어?\"\n\n", nickname);
		gotoxy(32, 6);
		puts("마온 : \"아니? 별로. 그리고 아직 약속시간 5분 전이잖아?\"");
		next(8);
		gotoxy(45, 4);
		puts("둘 다 살짝 쓴웃음을 짓는다.");
		next(6);
		gotoxy(38, 4);
		printf("%s : \"마온, 오늘도 옷 잘 어울리네.\"\n\n", nickname);
		gotoxy(42, 6);
		puts("마온 : \"그러는 너도 잘 어울려.\"");
		next(8);
	}
	
	gotoxy(40, 2);
	puts("마온 : \"그래서 어디부터 갈 거야?\"\n");

	gotoxy(46, 6);
	printf("%d. 귀신의 집\n\n", 1);
	gotoxy(46, 8);
	printf("%d. 롤러코스터\n\n", 2);
	gotoxy(46, 10);
	printf("%d. 관람차\n\n", 3);

	gotoxy(52, 12);
	scanf_s("%d", &act);

	system("cls");

	switch (act)
	{
		case 1:
			gotoxy(42, 4);
			printf("%s : \"귀신의 집에 갈까?\"\n\n", nickname);
			gotoxy(38, 6);
			puts("마온 : \"윽... 뭐... 가고 싶다면야...\"");
			next(8);
			gotoxy(44, 4);
			puts("귀신의 집에 입성한 두 사람");
			next(6);
			gotoxy(46, 4);
			puts("마온 : \"꺄아악!\"\n");
			gotoxy(40, 6);
			puts("귀신을 본 마온은 비명을 지른다.");
			next(8);
			gotoxy(34, 4);
			printf("%s : \"뭐야, 마온. 설마 귀신 무서워하는 거야?\"\n\n", nickname);
			gotoxy(36, 6);
			puts("마온 : \"무,무서워 할 수도 있는 거 아냐?\"\n");
			next(8);
			gotoxy(41, 4);
			printf("%s : \"뭐, 그렇게 무서우면......\"\n\n", nickname);
			gotoxy(39, 6);
			puts("그렇게 말하면서 마온의 손을 잡는 당신");
			next(8);
			gotoxy(48, 4);
			puts("마온 : \"에?\"\n");
			gotoxy(10, 6);
			puts("갑작스런 행동에 조금 당황한 마온. 하지만 이내 당황스러운 마음이 두근거리는 마음으로 바뀌기 시작한다.");
			next(8);
			gotoxy(28, 4);
			puts("그렇게 마온은 귀신이 나와서 무서울 때마다 당신의 손을 꼭 잡았습니다.\n");
			gotoxy(30, 6);
			puts("귀신의 집에서 나왔을 때 두 사람 모두 얼굴이 붉어져있었습니다.\n");
			like += 6;
			gotoxy(50, 8);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 10);
			system("pause");
			break;
		case 2:
			gotoxy(36, 4);
			printf("%s : \"롤러코스터라도 타러 가는게 어떨까?\"\n\n", nickname);
			gotoxy(44, 6);
			puts("마온 : \"뭐, 그러든가.\"");
			next(8);
			gotoxy(38, 4);
			puts("마온 : \"잠깐만, 이건 조금 너무하지 않아?\"\n");
			gotoxy(39, 6);
			printf("%s : \"그러게. 이건 조금 너무 긴데?\"\n", nickname);
			next(8);
			gotoxy(39, 4);
			puts("마온 : \"뭐, 다른거라도 타다가 올까?\"\n");
			gotoxy(42, 6);
			printf("%s : \"그래, 그러는 게 좋겠어.\"\n", nickname);
			next(8);
			gotoxy(20, 4);
			puts("그렇게 범퍼카, 회전 목마 등 다양한 놀이기구를 탔지만 결국 롤러코스터는 타지 못했습니다.\n");
			next(6);
			gotoxy(44, 4);
			puts("마온 : \"조금 아쉽네.\"\n");
			gotoxy(32, 6);
			printf("%s : \"그러게. 뭐, 그래도 다음에 오면 되니까 말이지.\"\n", nickname);
			next(8);
			gotoxy(18, 4);
			puts("마온은 이런 일로 당신을 싫어하지는 않습니다. 그리고 당신과의 추억도 많이 쌓였으니까 말이죠.\n");
			like++;
			gotoxy(50, 6);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 8);
			system("pause");
			break;
		case 3:
			gotoxy(44, 4);
			printf("%s : \"관람차 타러 갈래?\"\n\n", nickname);
			gotoxy(42, 6);
			puts("마온 : \"나쁘지 않을 것 같네.\"");
			next(8);
			gotoxy(46, 4);
			puts("관람차에 탄 두 사람");
			next(6);
			gotoxy(24, 4);
			puts("마온 : \"관람차는 처음 타보는데... 역시 정경이 좋으니까 나쁘지 않은 것 같네.\"\n");
			gotoxy(46, 6);
			printf("%s : \"응, 그러게.\"\n", nickname);
			next(8);
			gotoxy(30, 4);
			puts("마온 : \"사람들은 이런 거 주로 야경을 보려고 밤에 타잖아?\"\n");
			gotoxy(44, 6);
			printf("%s : \"어? 어, 뭐 그렇지.\"\n", nickname);
			next(8);
			gotoxy(34, 4);
			printf("%s : \'이런... 마온은 야경을 보고 싶었던 걸까?\'\n", nickname);
			next(6);
			gotoxy(34, 4);
			puts("마온 : \"지금 이 시간대에 나랑 같이 탄 이유가 뭐야?\"\n");
			gotoxy(45, 6);
			printf("%s : \"그... 그건...\"\n", nickname);
			next(8);
			gotoxy(32, 4);
			printf("%s : \"밤에 타면 야경보다 예쁜 네 모습이 어둠에 가려지잖아?\"\n\n", nickname);
			gotoxy(8, 6);
			puts("그 말을 들은 마온은 잠시 동안 입을 다물더니... 이내 곧 말의 뜻을 이해했는지 머리에서 김이 나기 시작했습니다.");
			next(8);
			gotoxy(10, 4);
			puts("아무래도 스트라이크 존이었던 것 같습니다. 어? 그런데 당신 괜찮아요? 당신 머리에서도 김이 나오는데요???\n");
			like += 9;
			gotoxy(50, 6);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 8);
			system("pause");
			break;
		default:
			like = penalty(like);
			break;
	}

	return like;
}

int stage8(int like, char nickname[])
{
	int act;
	char skip;
	char stage[] = { "stage8" };

	system("cls");
	while (getchar() != '\n') {}
	
	skip = do_you_want_to_skip(stage);
	if (skip == 'n')
	{
		gotoxy(45, 4);
		puts("봄비가 오는 어느 봄 날");
		next(6);
		gotoxy(40, 4);
		puts("학생 A : \"다들 각오는 되어 있겠지?\"\n");
		gotoxy(44, 6);
		puts("학생들 : \"그럼! 당연하지!\"");
		next(8);
		gotoxy(36, 4);
		puts("학생 A : \"좋아! 그러면 지금부터 눈치게임이다!\"\n");
		gotoxy(48, 6);
		puts("학생들 : \"예!!!\"");
		next(8);
		gotoxy(14, 4);
		puts("오늘은 시험이 끝난 날이었기 때문에 오전에 시험을 다 보고 남은 시간동안은 학교에 있어야했다.\n");
		gotoxy(8, 6);
		puts("그래서 지루한 시간을 타파하고자 시작한 것이 바로 이 눈치게임이었다. 물론 동의하지 않은 학생들도 있었지만");
		next(8);
		gotoxy(26, 4);
		printf("%s : \"뭐, 그래도 그냥 무료하게 보내는 것보다는 나을지도 모르겠네.\"\n\n", nickname);
		gotoxy(18, 6);
		puts("마온 : \"글쎄... 나는 아니라고 본다만. 이런 귀찮은 일을 할 시간에 차라리 책을 읽는게...\"");
		next(8);
		gotoxy(24, 4);
		printf("%s : \"그래도 개인적으로는 마온이 열심히 하는 모습을 보고 싶은데 말이지~\"\n", nickname);
		gotoxy(30, 6);
		puts("마온 : \"이런 일로 열심히 할 것 같다는 생각은 안 든다만.\"");
		next(8);
		gotoxy(33, 4);
		puts("학생 A : \"자! 그럼 지금부터 눈치게임 시~작!\"\n");
		gotoxy(22, 6);
		puts("그렇게 귀찮아하는 몇몇 학생들과 열정적인 다수의 학생들이 게임을 시작했다.");
		next(8);
		gotoxy(28, 4);
		puts("그리고, 마온은 결국 일어나지 않았고, 결국 게임에서 탈락하였다.");
		next(6);
		gotoxy(32, 4);
		puts("학생 A : \"오! 마온, 탈락이야? 그럼 벌칙을 받아야지!\"\n");
		gotoxy(30, 6);
		puts("마온 : \"뭔 소리야?! 벌칙이 있다는 소리는 못 들었다고!\"");
		next(8);
		gotoxy(24, 4);
		puts("학생 A : \"원래 게임은 벌칙 없이 하면 재미가 없다고. 자, 마온한테 뭘 시킬까?\"\n");
		gotoxy(28, 6);
		puts("마온 : \"아, 소수 의견 묵살인겁니까? 역시 빌어먹을 사회네.\"\n");
		gotoxy(26, 8);
		printf("%s : \"뭐, 진정해. 벌칙이라고 해도 그렇게 심한 건 안 나올테니까.\"", nickname);
		next(10);
		gotoxy(30, 4);
		puts("학생 A : \"다수결로 결정한 결과, 빼빼로 게임을 하겠습니다!\"\n");
		gotoxy(30, 6);
		puts("마온 : \"빼빼로 게임?! 웃기지 마! 그걸 대체 누구하고......\"");
		next(8);
		gotoxy(34, 4);
		printf("학생들 : \"그야, 당연히 네 연인이지, 안 그래 %s?\"\n\n", nickname);
		gotoxy(44, 6);
		printf("%s : \"응? 뭐라고? 나?!\"\n", nickname);
		next(8);
		gotoxy(22, 4);
		puts("학생 A : \"우리들 앞에서 공식적으로 염장질 할 수 있게 해준다는데, 하시는게?\"\n");
		gotoxy(44, 6);
		printf("%s, 마온 : \"으......\"\n", nickname);
		next(8);
	}
	
	gotoxy(44, 2);
	printf("마온 : \"어떻게 할래, %s?\"\n\n", nickname);

	gotoxy(40, 6);
	printf("%d. 대답하지 않고 교실을 뛰쳐나간다.\n\n", 1);
	gotoxy(40, 8);
	printf("%d. 그래, 하자.\n\n", 2);
	gotoxy(40, 10);
	printf("%d. 눈을 감고 기다린다.\n\n", 3);

	gotoxy(55, 12);
	scanf_s("%d", &act);

	system("cls");

	switch (act)
	{
		case 1:
			gotoxy(37, 4);
			printf("%s : \'그런 걸 할 수 있을리가 없잖아!!!\'\n\n", nickname);
			gotoxy(14, 6);
			puts("당신은 그렇게 생각하며 교실을 뛰쳐나갑니다. 뒤에서는 반 친구들이 당신을 부르는 소리가 들려왔습니다.");
			next(8);
			gotoxy(32, 4);
			puts("마온도 확실히 부끄럽다고 생각은 했습니다. 하지만......\n");
			gotoxy(24, 6);
			puts("마온 : \'으... 확실히 부끄럽지만, 선택받지 못 한건 조금 마음이 아픈데?\'\n");
			like -= 2;
			gotoxy(50, 8);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 10);
			system("pause");
			break;
		case 2:
			gotoxy(46, 4);
			printf("%s : \"그래, 하자.\"\n\n", nickname);
			gotoxy(46, 6);
			puts("마온 : \"진심이야?\"");
			next(8);
			gotoxy(28, 4);
			printf("%s : \"뭐, 확실히 부끄럽긴하지만, 벌칙이라고 생각하고 하자고.\"\n\n", nickname);
			gotoxy(30, 6);
			puts("마온 : \"진짜, 너의 그런 성격은 이럴 때 보면 참 부럽다니까.\"");
			next(8);
			gotoxy(38, 4);
			puts("그렇그 그들은 빼빼로게임을 진행합니다.\n");
			gotoxy(28, 6);
			puts("당연하지만 주변에서 반 친구들은 \"꺄아~\" 같은 효과음을 냈죠.");
			next(8);
			gotoxy(32, 4);
			puts("뭐, 그래도 적당한 부분에서 잘랐다고요? 벌칙이잖아요?\n");
			gotoxy(22, 6);
			puts("하지만, 두 사람은 당연히 서로에게 더 호감을 느꼈겠죠. 이게 흔들다리 효과인가?\n");
			like += 4;
			gotoxy(50, 8);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 10);
			system("pause");
			break;
		case 3:
			gotoxy(50, 4);
			puts("눈을 감는 당신");
			next(6);
			gotoxy(38, 4);
			puts("마온 : \"야, 설마 나보고 먼저 하라고?\"\n");
			gotoxy(26, 6);
			printf("%s : \"당연하지. 난 말했을텐데? 너의 적극적인 모습을 보고 싶다고말야.\"\n", nickname);
			next(8);
			gotoxy(30, 4);
			puts("당신의 조그마한 장난이네요. 이걸 마온은 어떻게 받아들일까요?");
			next(6);
			gotoxy(26, 4);
			puts("잠시동안 서로 간에 아무 말도 오가지 않았습니다. 하지만, 이내 곧......\n");
			gotoxy(40, 6);
			puts("마온 : \"그럼, 그대로 가만히 있어.\"");
			next(8);
			gotoxy(40, 4);
			puts("그리고 입에서 느껴지는 빼빼로의 맛");
			gotoxy(42, 6);
			puts("당신은 잠시 눈을 떠봅니다.");
			next(8);
			gotoxy(22, 4);
			puts("부끄러움을 엄청나게 느끼면서도 용기를 살짝 낸 마온의 모습이 눈앞에 보입니다.\n");
			gotoxy(36, 6);
			puts("이런, 마온보다 당신이 더 두근거린 것 같은데요?\n");
			gotoxy(50, 8);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 10);
			system("pause");
			break;
		default:
			like = penalty(like);
			break;
	}

	return like;
}

int stage9(int like, char nickname[])
{
	int act;
	char skip;
	char stage[] = { "stage9" };

	system("cls");
	while (getchar() != '\n') {}

	skip = do_you_want_to_skip(stage);
	if (skip == 'n')
	{
		gotoxy(30, 4);
		printf("%s : \"마온! 이번 주 토요일에 행사가 열리는 모양이야.\"\n", nickname);
		gotoxy(48, 6);
		puts("마온 : \"무슨 축제?\"");
		next(8);
		gotoxy(44, 4);
		printf("%s : \"불꽃놀이 축제야.\"", nickname);
		gotoxy(42, 6);
		puts("마온 : \"그래서? 같이 가자고?\"");
		next(8);
		gotoxy(28, 4);
		puts("마온 : \"싫어. 귀찮아. 왜 더운 여름밤에 그걸 보러 가야 되는 거야?\"");
		gotoxy(44, 6);
		puts("바로 거절 당해 버린 당신");
		next(8);
	}
	
	gotoxy(40, 4);
	puts("어떻게 해야 마온이 같이 축제를 가줄까요?");

	gotoxy(40, 8);
	printf("%d. 유카타 차림을 보고 싶다고 말한다.", 1);
	gotoxy(40, 10);
	printf("%d. 맛있는 먹거리를 즐기자고 말한다.", 2);
	gotoxy(40, 12);
	printf("%d. 추억을 남기러 가자고 말한다.", 3);

	gotoxy(56, 14);
	scanf_s("%d", &act);

	system("cls");

	switch (act)
	{
		case 1:
			gotoxy(34, 4);
			printf("%s : \"아쉽네. 마온의 유카타, 보고 싶었는데......\"", nickname);
			gotoxy(28, 6);
			puts("마온 : \"아니, 이렇게 더운 날에 유카타를 입으라고? 더워 죽는다고.\"");
			next(8);
			gotoxy(22, 4);
			printf("%s : \"역시 그렇겠지? 그래도 아쉽네. 분명 잘 어울릴 것 같은데 말야.\"", nickname);
			gotoxy(46, 6);
			puts("당신을 힐끔 쳐다보는 마온");
			next(8);
			gotoxy(28, 4);
			puts("마온 : \"뭐, 어울릴지는 모르겠지만 입기 적당한 날이면 입어줄게.\"");
			gotoxy(24, 6);
			printf("%s : \"그러면 말야, 오늘은 유카타 말고 간편한 차림으로 축제에 가자!\"", nickname);
			next(8);
			gotoxy(34, 4);
			puts("마온 : \"아니! 유카타랑 축제 중에 하나만 선택하라고!\"");
			gotoxy(34, 6);
			puts("그렇게 투덜댔지만 결국 축제에 갔다고 합니다.");
			gotoxy(46, 8);
			puts("마온이는 츤데레군요. 츤츤~");
			gotoxy(50, 10);
			printf("현재 호감도 : %d", like);
			gotoxy(40, 12);
			system("pause");
			break;
		case 2:
			gotoxy(34, 4);
			printf("%s : \"축제에서만 먹을 수 있는 게 있는 모양이야.\"", nickname);
			gotoxy(50, 6);
			puts("마온 : \"뭔데?\"");
			next(8);
			gotoxy(40, 4);
			printf("%s : \"사과 사탕이나, 빙수같은 거?\"", nickname);
			gotoxy(26, 6);
			puts("마온 : \"그거 먹으러 갈 시간에 그냥 집에서 선풍기 틀고 책이나 읽을래.\"");
			next(8);
			gotoxy(50, 4);
			printf("%s : \"넵......\"", nickname);
			gotoxy(38, 6);
			puts("마온 : \"그렇게까지 시무룩해 할 건 없잖아?\"");
			next(8);
			gotoxy(34, 4);
			puts("마온 : \"그럼 축제 대신 우리 집에서 노는 건 어때?\"");
			gotoxy(46, 6);
			printf("%s : \"그래도 돼?\"", nickname);
			next(8);
			gotoxy(20, 4);
			puts("마온 : \"그래. 아무것도 안 하면 계속 그 상태일테니까. 정말이지, 불편한 성격이야.\"");
			gotoxy(42, 6);
			printf("%s : \"미안, 신경쓰게 해서.\"", nickname);
			next(8);
			gotoxy(26, 4);
			puts("마온 : \"괜찮아. 너의 그런 성격까지 다 포함해서 너를 좋아하는 거니까.\"");
			next(6);
			gotoxy(34, 4);
			puts("방금 그 말에 굉장히 두근거렸습니다. 아, 물론 당신이요.");
			gotoxy(50, 6);
			printf("현재 호감도 : %d", like);
			gotoxy(40, 8);
			system("pause");
			break;
		case 3:
			gotoxy(14, 4);
			printf("%s : \"뭐, 안 가도 상관은 없지만...... 그래도 가면 한 편의 추억 정도는 되어 줄 수 있는데 말이지.\"", nickname);
			gotoxy(30, 6);
			printf("마온 : \"오, %s, 꽤나 멋진 말을 할 줄 알잖아? 그래도......\"", nickname);
			next(8);
			gotoxy(28, 4);
			puts("마온 : \"넌 이미 나한테 있어서 빠질 수 없는 추억의 조각이라고.\"");
			gotoxy(16, 6);
			printf("%s : \"그건 나도 마찬가지야. 그러니까 그 추억의 조각을 조금 더 만들어보는 건 어때?\"", nickname);
			next(8);
			gotoxy(24, 4);
			puts("마온 : \"%s, 너 말이야, 요즘 화술이 많이 늘었네. 좋아, 같이 가줄게.\"", nickname);
			gotoxy(34, 6);
			printf("%s : \"뭐, 너 덕분이지. 아무튼 고마워, 마온.\"", nickname);
			next(8);
			gotoxy(36, 4);
			puts("당신의 화술이 늘은 것을 보고 마온은 뿌듯해하네요.");
			like++;
			gotoxy(50, 6);
			printf("현재 호감도 : %d", like);
			gotoxy(40, 8);
			system("pause");
			break;
		default:
			like = penalty(like);
			break;
	}

	return like;
}

int special1(int like, char nickname[])
{
	int act;
	char skip;
	char stage[] = { "special1" };

	system("cls");
	while (getchar() != '\n') {}

	skip = do_you_want_to_skip(stage);
	if (skip == 'n')
	{
		gotoxy(24, 4);
		puts("마온이 다쳐서 병원에 입원했다는 소식을 들은 당신은 병문안을 가기로 결정했다.");
		next(6);
		gotoxy(32, 4);
		printf("%s : \"마온, 괜찮아? 입원할정도로 크게 다친거야?\"\n", nickname);
		next(6);
		gotoxy(32, 4);
		puts("마온 : \"아냐, 괜찮아. 그 정도로 크게 다친 건 아니야.\"");
		next(6);
		gotoxy(34, 4);
		printf("%s : \"그래... 그렇구나. 그렇다면 다행이네.\"\n\n", nickname);
		gotoxy(40, 6);
		printf("%s : \'하지만... 뭔가 이상한데......\'\n", nickname);
		next(8);
		gotoxy(26, 4);
		puts("마온 : \"저기, 계속 서 있는 것도 뭐하니까 자리에 앉는게 어때?\"");
		next(6);
		gotoxy(48, 4);
		printf("%s : \"어, 그래.\"\n", nickname);
		next(6);
		gotoxy(34, 2);
		puts("그렇게 잠시 동안 둘은 아무 말도 주고 받지 않았습니다.\n");
		gotoxy(22, 4);
		puts("당신은 그동안 마온의 얼굴을 쳐다봤습니다. 마온한테서 위화감이 계속해서 느껴집니다.\n");
	}
	
	gotoxy(50, 8);
	puts("어떻게 할까요?\n");

	gotoxy(42, 10);
	printf("%d. 마온을 살짝 안아준다.\n\n", 1);
	gotoxy(42, 12);
	printf("%d. 병문안 선물을 사러 간다.\n\n", 2);
	gotoxy(42, 14);
	printf("%d. 지금 상태로 계속 지켜본다.\n\n", 3);

	gotoxy(55, 16);
	scanf_s("%d", &act);

	system("cls");
	switch (act)
	{
	case 1:
		gotoxy(28, 4);
		puts("마온이 창 밖을 하염없이 바라보고 있을 때 뒤에서 살짝 안아준다.");
		next(6);
		gotoxy(40, 4);
		printf("마온 : \"에?! %s? 갑자기 왜 그래?\"\n", nickname);
		next(6);
		gotoxy(42, 4);
		puts("그 말에 조용히 입을 여는 당신\n");
		gotoxy(14, 6);
		printf("%s : \"저기... 뭐랄까... 외롭다거나, 힘들다거나, 괴로울 때는 나한테 말해줬으면 좋겠는데...\"\n", nickname);
		gotoxy(34, 8);
		printf("%s : \"일단 그래도 친구잖아? 넌 혼자가 아니라고.\"", nickname);
		next(10);
		gotoxy(38, 4);
		puts("그리고 잠시 동안 병실 안은 침묵이 흐른다.");
		next(6);
		gotoxy(42, 4);
		puts("잠시동안의 침묵을 깬 것은 마온");
		next(6);
		gotoxy(30, 4);
		puts("마온 : \"저기... 미안한데. 조금만 더 이렇게... 함께 있어줘.\"\n");
		gotoxy(42, 6);
		puts("울먹거리는 듯한 목소리가 들려온다.");
		next(8);
		gotoxy(32, 4);
		printf("%s : \"그래. 네가 원할때까지 계속 이러고 있을게.\"\n", nickname);
		next(6);
		gotoxy(36, 4);
		puts("당신의 상냥함이 마온의 깊은 마음속을 녹여줬습니다.\n");
		like += 8;
		gotoxy(50, 6);
		printf("현재 호감도 : %d\n", like);
		gotoxy(40, 8);
		system("pause");
		break;
	case 2:
		gotoxy(32, 4);
		printf("%s : \"뭐 마실래? 다쳐서 어디 가지도 못했을 거 아냐.\"\n", nickname);
		next(6);
		gotoxy(38, 4);
		puts("마온 : \"어? 굳이 안 사줘도 되는데 말이지.\"");
		next(6);
		gotoxy(14, 4);
		printf("%s : \"아니! 진짜 답답하네! 그냥 사준다거나 준다고 할 때는 \"감사합니다.\"하고 받는 거라고!\"\n", nickname);
		next(6);
		gotoxy(40, 4);
		puts("마온 : \"어? 어... 그럼 매실로 부탁할게.\"");
		next(6);
		gotoxy(36, 4);
		printf("%s : \"오케이! 잠깐만 기다려. 금방 갔다 올게.\"\n", nickname);
		next(6);
		gotoxy(24, 4);
		puts("그렇게 병실을 나서는 당신의 모습을 보면서 마온은 살짝 미소를 머금습니다.\n");
		like += 3;
		gotoxy(50, 6);
		printf("현재 호감도 : %d\n", like);
		gotoxy(40, 8);
		system("pause");
		break;
	case 3:
		gotoxy(36, 4);
		puts("그렇게 둘 다 아무 말 없이 시간이 흘러갔습니다.");
		next(6);
		gotoxy(42, 4);
		puts("잠시 뒤, 마온이 입을 열었습니다.");
		next(6);
		gotoxy(40, 4);
		puts("마온 : \"저기, 넌 왜 나를 좋아하는거야?\"");
		next(6);
		gotoxy(20, 4);
		puts("갑자기 날라온 기습질문! 하지만 당신은 당황하지 않고 마음 속에 있는 진심을 말합니다.");
		next(6);
		gotoxy(0, 4);
		printf("%s : \"갑작스럽긴 하지만, 그래도 뭐. 항상 보이지 않는 곳에서 최선을 다하는 점? 나 같으면 그런 건 무리거든. 존경스러워.\"\n", nickname);
		next(6);
		gotoxy(34, 4);
		puts("마온 : \"그렇구나...... 그렇게 말하니까 쑥스럽네.\"\n");
		gotoxy(22, 6);
		printf("%s : \"뭐라고? 네가 물어봐서 대답해준거잖아. 솔직히 이쪽이 더 쑥스럽다고!\"\n", nickname);
		next(8);
		gotoxy(20, 4);
		puts("그 대화를 계기로 둘은 이런저런 수다를 떨었습니다. 처음의 침묵이 거짓말같았습니다.\n");
		like++;
		gotoxy(50, 6);
		printf("현재 호감도 : %d\n", like);
		gotoxy(40, 8);
		system("pause");
		break;
	default:
		like = penalty(like);
		break;
	}

	return like;
}

int special2(int like, char nickname[])
{
	int act;
	char skip;
	char stage[] = { "special2" };

	system("cls");
	while (getchar() != '\n') {}

	skip = do_you_want_to_skip(stage);
	if (skip == 'n')
	{
		gotoxy(40, 4);
		puts("강한 햇빛이 내려찌는 어느 한 여름날\n");
		gotoxy(40, 6);
		puts("둘은 같이 수영장으로 놀러가기로 했다.");
		next(8);
		gotoxy(34, 4);
		printf("%s : \"올해는 작년에 비해서 더 더운 것 같네.\"\n\n", nickname);
		gotoxy(14, 6);
		puts("마온 : \"그러게. 솔직히 얘기해서 수영장 가는 것보다는 집에서 책 읽는게 더 낫지 않았을까 하는 생각?\"");
		next(8);
		gotoxy(36, 4);
		printf("%s : \"아, 그건 미안하게 됐네. 그래도...\"\n\n", nickname);
		gotoxy(30, 6);
		puts("마온 : \"농담이야. 뭐, 친구 부탁이니까 가끔은 괜찮지.\"");
		next(8);
		gotoxy(42, 4);
		puts("그렇게 그들은 수영장에 도착했다.");
		next(6);
		gotoxy(42, 4);
		printf("%s : \"아직 안 나온건가?\"\n\n", nickname);
		gotoxy(44, 6);
		puts("마온 : \"여, 기다렸어?\"");
		next(8);
		gotoxy(40, 4);
		printf("%s : \"아냐, 나도 방금 전...\"\n\n", nickname);
		gotoxy(44, 6);
		puts("마온 : \"응? 왜 그래?\"");
		next(8);
		gotoxy(42, 2);
		printf("%s : \'너무 잘 어울리잖아?!\'\n\n", nickname);
	}
	
	gotoxy(30, 4);
	puts("마온의 수영복이 잘 어울린다고 생각한 당신, 어떻게 행동할까?\n");

	gotoxy(38, 8);
	printf("%d. 솔직하게 잘 어울린다고 칭찬한다.\n\n", 1);
	gotoxy(38, 10);
	printf("%d. 말을 돌린다.\n\n", 2);
	gotoxy(38, 12);
	printf("%d. 마온을 바라본다.\n\n", 3);
	gotoxy(55, 14);
	scanf_s("%d", &act);

	system("cls");
	
	switch (act)
	{
		case 1:
			gotoxy(34, 4);
			printf("%s : \"아니, 그냥, 뭐랄까, 네 수영복... 잘 어울려서.\n", nickname);
			next(6);
			gotoxy(42, 4);
			puts("마온 : \"이건 꽤나 신선한데?\"\n");
			gotoxy(42, 6);
			printf("%s : \"그건 또 뭔 소리야?\"\n", nickname);
			next(8);
			gotoxy(20, 4);
			puts("마온 : \"뭐, 평소에 패션에는 관심이 없으니까. 동생한테도 그런 말 못 듣는 경우가 많거든.\"\n");
			gotoxy(28, 6);
			printf("%s : \"아... 그렇구나... 그래도 방금 내가 한 말은 진심이라고!\"\n", nickname);
			next(8);
			gotoxy(30, 4);
			puts("마온 : \"알고 있어. 네가 거짓말을 할 녀석이 아니라는 것쯤은.\"");
			next(6);
			gotoxy(20, 4);
			puts("당신을 믿어주는 마온의 모습이 꽤나 신선했던 것 같군요. 당신은 잠시 두근거립니다.\n");
			gotoxy(18, 6);
			puts("하지만, 마온도 마찬가지죠. 좋아하는 당신에게 그런 말을 들었다는 것은 기쁠 수밖에 없겠죠.\n");
			like++;
			gotoxy(50, 8);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 10);
			system("pause");
			break;
		case 2:
			gotoxy(40, 4);
			printf("%s : \"아니, 우리 뭐 타러 갈까?\"\n\n", nickname);
			gotoxy(38, 6);
			puts("마온 : \"어... 그래. 뭐... 저건 어떨까?");
			next(8);
			gotoxy(35, 4);
			puts("그냥 그렇게 아무 일 없이 즐겁게 놀다 왔습니다.");
			gotoxy(50, 6);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 8);
			system("pause");
			break;
		case 3:
			gotoxy(40, 4);
			puts("아무 말 없이 마온을 바라보는 당신.");
			next(6);
			gotoxy(36, 4);
			puts("마온 : \"뭐야, 왜 그래? 혹시 열사병이야?\"\n");
			gotoxy(35, 6);
			puts("그렇게 말하면서 자신의 이마에 손을 대는 마온");
			next(8);
			gotoxy(28, 4);
			puts("그렇게 당신과 가까워진 마온때문에 당신은 열이 나기 시작합니다.\n");
			gotoxy(36, 6);
			puts("그렇게 열이 나서 당신은 쓰러지고 맙니다.");
			next(8);
			gotoxy(38, 4);
			puts("마온은 계속해서 당신을 간병해줬습니다.\n");
			gotoxy(30, 6);
			puts("마온 : \"정말이지. 이런 나보다도 건강하지 못하면 안 되지.\"");
			next(8);
			gotoxy(38, 4);
			printf("%s : \"미안해. 한심한 모습을 보여줬네.\"\n\n", nickname);
			gotoxy(20, 6);
			puts("마온 : \"그렇게 미안하면 빨리 나아서 오늘 계획한 것보다 더 좋은 계획으로 찾아오라고!\"");
			next(8);
			gotoxy(8, 4);
			puts("물론 열이 난 것은 당신의 잘못은 아니었지만, 귀찮음을 무릎쓰고 놀러간 마온이 조금 실망하는 건 어쩔 수 없죠.\n");
			like--;
			gotoxy(50, 6);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 8);
			system("pause");
			break;
		default:
			like = penalty(like);
			break;
	}
	return like;
}

int special3(int like, char nickname[])
{
	int act;
	char skip;
	char stage[] = { "special3" };

	system("cls");
	while (getchar() != '\n') {}

	skip = do_you_want_to_skip(stage);
	if (skip == 'n')
	{
		gotoxy(36, 4);
		printf("마온 : \"앞으로 며칠 뒤면 시험이네, 그치 %s\"\n\n", nickname);
		gotoxy(36, 6);
		puts("마온의 목소리가 평소보다 더 어둡게 들려온다.");
		next(8);
		gotoxy(42, 4);
		printf("%s : \"어? 어... 그러게...\"\n", nickname);
		gotoxy(12, 6);
		printf("%s \'아마도 내가 시험을 못 보기 때문이겠지. 그래도 날 걱정해준다는 건 꽤 나쁘지 않은 것 같은데?\'\n", nickname);
		next(8);
		gotoxy(36, 2);
		puts("마온 : \"그래서 이번 시험은 어떻게 할 생각이야?\"\n");
	}
	
	gotoxy(42, 4);
	printf("%s : \"그러게...... 어떻게 할까?\"\n\n", nickname);

	gotoxy(40, 8);
	printf("%d. 뭐, 어떻게든 되지 않을까?\n\n", 1);
	gotoxy(40, 10);
	printf("%d. 저한테 공부를 가르쳐주시지 않겠습니까?!\n\n", 2);
	gotoxy(40, 12);
	printf("%d. 어차피 해봤자 안 될텐데 놀아야지.\n\n", 3);

	gotoxy(55, 14);
	scanf_s("%d", &act);

	system("cls");

	switch (act)
	{
		case 1:
			gotoxy(41, 4);
			printf("%s : \"뭐, 어떻게든 되지 않을까?\"\n\n", nickname);
			gotoxy(40, 6);
			puts("마온 : \"뭐야? 뭐가 그렇게 불확실해?\"");
			next(8);
			gotoxy(24, 4);
			printf("%s : \"하지만, 다른 애들처럼 학원을 다니지도 않는데 어떻게 공부하라는 거야?\"\n\n", nickname);
			gotoxy(32, 6);
			puts("마온 : \"뭐, 그렇다면 적어도 교과서라도 소리내서 읽어.\"");
			next(8);
			gotoxy(40, 4);
			printf("%s : \"왜 교과서를 소리내서 읽어?\"\n\n", nickname);
			gotoxy(30, 6);
			puts("마온  : \"소리내서 말하면 더 잘 외워져. 아, 물론 소스는 나야.\"");
			next(8);
			gotoxy(40, 4);
			printf("%s : \"OK! 그럼 지금부터 공부한다!\"\n", nickname);
			next(6);
			gotoxy(24, 4);
			puts("그렇게 마온이가 알려준 방식으로 공부해봤더니 시험 성적이 조금은 올랐습니다.\n");
			gotoxy(32, 6);
			puts("당신의 꾸준히 노력하는 모습을 보며 마온은 뿌듯해합니다.\n");
			like++;
			gotoxy(50, 8);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 10);
			system("pause");
			break;
		case 2:
			gotoxy(36, 4);
			printf("%s : \"저한테 공부를 가르쳐주시지 않겠습니까?!\"\n\n", nickname);
			gotoxy(38, 6);
			puts("그렇게 말하면서 살짝 고개를 숙이는 당신.");
			next(8);
			gotoxy(34, 4);
			puts("마온 : \"뭐, 나라도 괜찮다면. 아, 오늘부터 할거지?\"\n");
			gotoxy(40, 6);
			printf("%s : \"어? 일단 그럴 생각인데?\"\n", nickname);
			next(8);
			gotoxy(36, 4);
			puts("마온 : \"아, 그럼 오늘 끝나고 우리 집으로 가자.\"\n");
			gotoxy(42, 6);
			printf("%s : \"아, 그래주면 고맙지.\"\n", nickname);
			next(8);
			gotoxy(40, 4);
			puts("그렇게 정규 수업이 끝난 뒤, 마온의 집");
			next(6);
			gotoxy(45, 4);
			printf("%s : \"실례합니다.\"\n\n", nickname);
			gotoxy(40, 6);
			puts("마온 : \"네, 마음껏 실례하세요.\"");
			next(8);
			gotoxy(34, 4);
			puts("마온의 반응으로 보아 집에는 아무도 없는 것 같았다.");
			next(6);
			gotoxy(32, 4);
			puts("마온 : \"자, 그럼 공부하자. 첫 번째 시험이 수학이였지?\"\n");
			gotoxy(46, 6);
			printf("%s : \"어. 맞아.\"\n", nickname);
			next(8);
			gotoxy(40, 4);
			puts("마온 : \"응? 내 수학책이 어디있지?\"\n");
			gotoxy(46, 6);
			printf("%s : \"왜 그래?\"\n", nickname);
			next(8);
			gotoxy(36, 4);
			puts("마온 : \"미안. 아무래도 수학 책을 두고 온 것 같아.\"\n");
			gotoxy(16, 6);
			puts("마온 : \'이런. 아까 챙겨온다는 걸 그냥 두고 왔네. 하필 오늘 같은 날에... 이건 불행인걸...\'");
			next(8);
			gotoxy(49, 4);
			printf("%s : \"아냐.\"\n\n", nickname);
			gotoxy(50, 6);
			puts("마온 : \"응?\"");
			next(8);
			gotoxy(44, 4);
			printf("%s : \"불행이 아니라고.\"\n\n", nickname);
			gotoxy(44, 6);
			puts("마온 : \"갑자기 뭔 소리야?\"");
			next(8);
			gotoxy(28, 4);
			printf("%s : \"분명히 또 \'이건 불행이야.\'같은 생각 하고 있었지? 다 안다고.\"\n\n", nickname);
			gotoxy(44, 6);
			puts("마온 : \"뭐야, 어떻게?\"");
			next(8);
			gotoxy(20, 4);
			printf("%s : \"너랑 지낸지는 얼마 안 됐지만 너에게 관심을 품을 만큼 너를 계속 봐왔으니까.\"\n\n", nickname);
			gotoxy(24, 6);
			puts("마온은 얼굴이 조금 붉어지기 시작한다. 그것도 모른채 당신은 계속 말을 이어간다.");
			next(8);
			gotoxy(6, 4);
			printf("%s : \"그건 불행이 아니야. 타인을 도와주고 배려해주기 때문에 그런거지. 난 오히려 좋은 습관이라고 생각해.\"\n\n", nickname);
			gotoxy(32, 6);
			puts("당신은 마온을 향해 살짝 웃습니다. 마온은 잠시 동안 입을 다뭅니다.");
			next(8);
			gotoxy(40, 4);
			printf("마온 : \"뭐야! %s주제에! 건방지다고!\"\n\n", nickname);
			gotoxy(20, 6);
			puts("그렇게 말하면서 당신을 때리는 마온. 자세히 들여다보니 얼굴이 완전히 홍당무가 되었네요.\n");
			like += 7;
			gotoxy(50, 8);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 10);
			system("pause");
			break;
		case 3:
			gotoxy(34, 4);
			printf("%s : \"뭐, 어차피 해봤자 안 될텐데 놀아야지.\"\n", nickname);
			next(6);
			gotoxy(46, 4);
			puts("마온 : \"뭔 소리야!\"\n");
			gotoxy(38, 6);
			puts("아까보다 조금 더 목소리가 날카로워졌다.");
			next(8);
			gotoxy(36, 4);
			puts("마온 : \"물론 안 된다고 생각하는 건 좋은거야.\"\n");
			gotoxy(36, 6);
			printf("%s : \'저기요? 뭔가 잘못된 것 같습니다만?\'\n", nickname);
			next(8);
			gotoxy(30, 4);
			puts("마온 : \"물론 자기디스와 부정적인 사고는 필요해. 하지만 말야......\"\n");
			gotoxy(18, 6);
			puts("마온 : \"할 수 있는 건 다 해보고 나서 그런 말을 해야 진정한 의미가 있는 거라고! 알겠어?\"");
			next(8);
			gotoxy(48, 4);
			printf("%s : \"아, 옙!\"\n", nickname);
			next(6);
			gotoxy(14, 4);
			puts("이런, 그녀의 기분이 조금 언짢아진 것 같네요. 확실히 그녀는 평소에 끝까지 노력해보는 성격이니까요.\n");
			like -= 2;
			gotoxy(50, 6);
			printf("현재 호감도 : %d\n", like);
			gotoxy(40, 8);
			system("pause");
			break;
		default:
			like = penalty(like);
			break;
	}

	return like;
}

int same_name(int like)
{
	// 만약 히로인의 이름인 "마온"과 같은 이름으로 입력했을 경우, 발동되는 이벤트다.
	system("cls");

	gotoxy(28, 4);
	puts("마온 : \"어라? 나한테 관심이 있는 사람도 있었어? 너도 참 별나구나.\"");
	next(6);
	gotoxy(26, 4);
	puts("마온 : \"뭐, 너도 마온이야? 이 이름 그렇게 흔한 이름은 아닌데 말이지.\"");
	next(6);
	gotoxy(20, 4);
	puts("마온 : \"뭐, 이것도 인연이려나? 그럼, 뭐, 친구로써 잘 부탁할게, 동명이인 클래스메이트\""); // 동명이인 클래스메이트는 너의 췌장을 먹고 싶어에서 참고했습니다.
	next(6);
	gotoxy(16, 4);
	puts("외톨이인 마온에게 관심을 가져준 이름이 같은 당신. 마온은 그 두 점에서 어느 정도 관심을 가집니다.\n");
	like += 9;
	gotoxy(50, 6);
	printf("현재 호감도 : %d\n", like);
	gotoxy(40, 8);
	system("pause");

	return like;
}

int best_name(int like, char nickname[])
{
	// 최고 점수를 세운 플레이어 이름과 같은 이름을 입력했을 경우, 같은 사람이라는 가정 하에 발생하는 이스터에그

	system("cls");

	gotoxy(36, 4);
	printf("마온 : \"어라? %s? 이 게임 또 해주러 온거야?\n", nickname);
	next(6);
	gotoxy(30, 4);
	puts("마온 : \"너 답네. 진짜로. 이거 칭찬이니까. 그래, 정말로...\"\n");
	next(6);
	gotoxy(50, 4);
	puts("마온 : \"고마워!\"\n");
	like += 5;
	gotoxy(50, 6);
	printf("현재 호감도 : %d\n", like);
	gotoxy(40, 8);
	system("pause");

	return like;
}
