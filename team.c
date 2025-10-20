// ------------------------------
// 응용 시나리오
// ------------------------------
/*
 이 프로그램은 편의점과 같은 유통 업종에서 매일 입고되는 상품의 재고를 효율적으로 관리하기 위해 만든 재고관리 시뮬레이터입니다.
 유통기한이 존재하는 상품의 입고, 저장, 판매, 폐기 과정을 자동화해서 사람이 직접 하나하나 확인하지 않아도 되도록 만들어졌습니다.
 
 저희는 유통기한이 5일인 우유 재고관리 시뮬레이터를 설계했습니다.
 매일 아침, 입고된 상품의 개수를 입력하면 재고에 추가합니다.
 그리고 마감할 때 판매된 상품의 개수를 입력하면 재고에서 판매된 상품을 차감합니다.
 매일 유통기한이 지난 상품을 자동으로 폐기 처리하여 재고를 최신 상태로 유지합니다.
 마지막으로 재고 상태와 유통기한이 지난 상품의 개수를 출력하여 사용자가 쉽게 확인할 수 있도록 했습니다.

 편의점 재고 관리 시스템은 기본적으로 먼저 들어온 재고를 먼저 판매하는 선입선출 구조이기 때문에
 선형 큐를 선택하여 코드를 구성했습니다.
*/

/*
 코드 참조: 강의 자료
*/
#include <stdio.h>
#include <stdlib.h>

// 큐 크기
#define MAX_QUEUE_SIZE 1000
// 유통기한
#define EXPIRY 5

// 구조체 정의
typedef int element;
typedef struct {
	int  front;
	int rear;
	element data[MAX_QUEUE_SIZE];
} QueueType;

// 에러 함수 정의
void error(const char *message) {
	fprintf(stderr, "%s\n", message);
	exit(1);
}

// 큐 초기화 함수 정의
void init_queue(QueueType *q) {
	// 초기값이 -1인 이유는 enqueue 함수에서 값을 먼저 증가시키기 때문
	// 초기값이 0이면 첫 번째 데이터가 data[1]에 저장됨
	q->rear = -1;
	q->front = -1;
}

// 큐 포화 여부 확인 함수 정의
int is_full(QueueType *q) {
	// rear가 큐의 최대 크기 -1과 같으면 포화 상태
	// 왜냐하면 rear가 0부터 시작하기 때문
	if (q->rear == MAX_QUEUE_SIZE - 1)
		return 1;
	else
		return 0;
}

// 큐 공백 여부 확인 함수 정의
int is_empty(QueueType *q) {
	// front와 rear가 같으면 공백 상태
	// 왜냐하면 front가 데이터를 제거할 때마다 증가하기 때문
	// 즉, front가 rear와 같아지면 큐에 데이터가 없다는 의미
	if (q->front == q->rear)
		return 1;
	else
		return 0;
}

// 데이터 삽입 함수 정의
void enqueue(QueueType *q, int item) {
	if (is_full(q)) {
		error("큐가 포화상태입니다.");
		return;
	}
	
	// rear를 먼저 증가시키고 데이터를 삽입
	q->data[++(q->rear)] = item;
}

// 데이터 삭제 함수 정의
int dequeue(QueueType *q) {
	if (is_empty(q)) {
		error("큐가 공백상태입니다.");
		return -1;
	}

	int item = q->data[++(q->front)];
	return item;
}

// 메인 함수 정의
int main() {
	int day = 1;
	QueueType display;
	init_queue(&display);

	while (1) {
		// 오늘 입고된 물건 개수 사용자 입력
		int item_cnt; // 변수 초기화
		int sold_cnt; // 변수 초기화

		printf("\nDay %d \n", day);
		printf("오늘 입고된 물건 개수 입력 (종료: -1): ");
		scanf("%d", &item_cnt);

		// 종료 조건 정의
		if (item_cnt == -1) break; // -1 입력 시 종료
		if (item_cnt < 0) { // -1 미만 입력 시 재입력 요청
			printf("다시 입력하십시오. 입력 값: %d \n", item_cnt);
			continue;
		}

		// (1) 입고 처리
		for (int i = 0; i < item_cnt; i++) {
			if (!is_full(&display))
				enqueue(&display, EXPIRY);
			else {
				printf("큐가 가득 찼습니다.\n");
				break;
			}
		}

		// (2) 판매 처리
		printf("오늘 판매된 물건 개수 입력: ");
		scanf("%d", &sold_cnt);
		for (int i = 0; i < sold_cnt; i++) {
			if (!is_empty(&display)) dequeue(&display);  // 판매 = 가장 오래된 상품 제거
			else {
				printf("판매할 재고가 없습니다.\n");
				break;
			}
		}

		// (3) 유통기한 관리
		int expired = 0;
		int total = display.rear - display.front;
		for (int i = 0; i < total; i++) {
			int life = dequeue(&display) - 1;
			if (life <= 0)
				expired++;
			else
				enqueue(&display, life);
		}

		// (4) 결과 출력
		int stock = display.rear - display.front;
		printf("오늘 재고 수: %d\n", stock);
		printf("오늘 유통기한 지난 물건 수: %d\n", expired);
		
		if (stock <= 100) {
			printf("재고 상태 (유통기한): ");
			// GPT: 큐 내부 모든 요소 출력
			for (int i = display.front + 1; i <= display.rear; i++) {
				printf("%d ", display.data[i]);
			}
			printf("\n");
		}
		else {
			printf("재고 상태: (100개 초과, 생략)\n");
		}
		day++;
	}

	printf("\n프로그램 끝\n");
	return 0;
}

/*
 프로그램을 실행하면 첫째 날의 알림과 함께
 오늘 입고할 물건의 개수와 판매된 물건의 개수를 입력하게 됩니다.
 
 [1일차]
 먼저 5개의 상품을 입고하고, 오늘은 판매가 없다고 입력하겠습니다.
 입고가 완료되면 현재 재고 수, 유통기한이 지난 물건 수,
 그리고 각 상품의 남은 유통기한이 출력됩니다.
 모든 상품은 유통기한이 10일로 시작하며,
 하루가 지나면서 9일로 감소한 것을 확인할 수 있습니다.
 
 [2일차]
 이번에는 4개의 상품을 입고하고, 동시에 2개의 상품이 판매되었다고 입력하겠습니다.
 큐의 선입선출(FIFO) 원리에 따라 가장 먼저 입고된 1일차 상품 2개가
 판매되어 큐의 맨 앞에서 제거됩니다.
 결과적으로 재고는 7개가 되었으며,
 유통기한이 8일 남은 상품(1일차 잔여분)과 9일 남은 상품(2일차 신규 입고분)이
 함께 출력되어 있는 것을 볼 수 있습니다.

 [3일차~4일차]
 0 enter 0 enter - 2번 반복
 
 [5일차]
 3, 4일차는 입고와 판매가 없었다고 가정하고 5일차로 넘어갑니다.
 새로 2개의 상품을 입고하고, 오늘 1개의 상품이 판매되었다고 입력하겠습니다.
 하루가 지나면서 첫째 날 입고된 상품들의 유통기한이 0이 되어 자동으로 폐기됩니다.
 결과창에는 오늘 유통기한이 지난 물건 수가 2개로 표시되고,
 판매로 인해 재고에서 추가로 1개가 줄어든 것을 확인할 수 있습니다.
*/
