#include "wearable.h"

#include "usart.h"

#define PACKET_SIZE 6
#define MAX_EVENTS 5
#define PACKET_START '{'
#define PACKET_END '}'

typedef struct {
    uint8_t start;
    uint16_t id;
    uint16_t data;
    uint8_t end;
} event_t;

typedef struct {
    event_t *Head;
    event_t *Tail;
    event_t events[MAX_EVENTS];
    int data_size;
} queue_t;

static uint8_t EventBuf[PACKET_SIZE];
static uint8_t uartBuf[MAX_EVENTS * PACKET_SIZE];
static size_t bytes_rcvd = 0;
static queue_t que;

void init_wearable() {
    __HAL_RCC_HSEM_CLK_ENABLE();
    HAL_UART_Receive_IT(&huart7, uartBuf, 1);
    que.data_size = 0;
    que.Head = que.events;
    que.Tail = que.events;
}

static const event_t front() {
    if (que.data_size > 0)
        return *que.Head;
    else {
        event_t event = {0, 0};
        return event;
    }
}

static void push_back() {
    uint8_t start = EventBuf[0];
    uint16_t id = (EventBuf[1] << 8) + EventBuf[2];
    uint16_t data = (EventBuf[3] << 8) + EventBuf[4];
    uint8_t end = EventBuf[5];

    que.Tail->data = data;
    que.Tail->id = id;
    que.Tail->start = start;
    que.Tail->end = end;
    if (que.Tail < que.events + MAX_EVENTS - 1) {
        que.Tail = que.Tail + 1;
    } else {
        que.Tail = que.events;
    }
    if (que.Tail == que.Head && que.data_size > 0) {
        if (que.Head < que.events + MAX_EVENTS - 1) {
            que.Head = que.Head + 1;
        } else {
            que.Head = que.events;
        }
    }
    if (que.data_size < MAX_EVENTS)
        que.data_size++;
}

static void pop() {
    if (que.Head - que.events < MAX_EVENTS - 1) {
        que.Head = que.Head + 1;
    } else {
        que.Head = que.events;
    }
    if (que.data_size > 0)
        que.data_size--;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart != &huart7)
        return;

    HAL_UART_Receive_IT(&huart7, &uartBuf[bytes_rcvd++], 1);
}

wearable_event_t poll_wearable() {
    static bool receiving = false;
    static size_t packet_progress = 0;

    for (size_t i = 0; i < bytes_rcvd; i++) {
        uint8_t b = uartBuf[i];
        EventBuf[packet_progress++] = b;
        if (receiving) {
            if (b == PACKET_END) {
                if (packet_progress == PACKET_SIZE) {
                    push_back();
                } else {
                    receiving = false;
                }
                packet_progress = 0;
            } else {
                if (packet_progress == PACKET_SIZE) {
                    receiving = false;
                    packet_progress = 0;
                }
            }
        } else {
            if (b == PACKET_START) {
                receiving = true;
            } else {
                packet_progress = 0;
            }
        }
    }
    bytes_rcvd = 0;

    wearable_event_t event;
    if (front().id == 0 && front().data == 0) {
        event.bits = NO_WEARABLE_EVENT;
    } else {
        event.fields.id = front().id;
        event.fields.act = front().data;
        pop();
    }

    return event;
}