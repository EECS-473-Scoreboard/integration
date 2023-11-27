#include "wearable.h"

#include "usart.h"

typedef struct {
    uint8_t start;
    uint16_t id;
    uint16_t data;
    uint8_t end;
} event_t;

typedef struct {
    event_t *Head;
    event_t *Tail;
    event_t events[5];
    int data_size;
} queue_t;

static uint8_t EventBuf[6];
static uint8_t uartBuf[1];
static uint8_t completeEvent = 0;
static uint8_t receiving = 0;
static int eventpos = 0;
static queue_t que;

void init_wearable() {
    HAL_UART_Receive_DMA(&huart5, uartBuf, 1);
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

static void push_back(uint8_t indata[6]) {
    uint8_t start = indata[0];
    uint16_t id_UH = indata[1] << 8;
    uint16_t id_LH = indata[2];
    uint16_t data_UH = indata[3] << 8;
    uint16_t data_LH = indata[4];
    uint8_t end = indata[5];
    uint16_t data = data_UH | data_LH;
    uint16_t id = id_UH | id_LH;
    que.Tail->data = data;
    que.Tail->id = id;
    que.Tail->start = start;
    que.Tail->end = end;
    if (que.Tail < que.events + 4) {
        que.Tail = que.Tail + 1;
    } else {
        que.Tail = que.events;
    }
    if (que.Tail == que.Head && que.data_size > 0) {
        if (que.Head < que.events + 4) {
            que.Head = que.Head + 1;
        } else {
            que.Head = que.events;
        }
    }
    if (que.data_size < 5)
        que.data_size++;
}

static void pop() {
    if (que.Head - que.events < 4) {
        que.Head = que.Head + 1;
    } else {
        que.Head = que.events;
    }
    if (que.data_size > 0)
        que.data_size--;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart != &huart5)
        return;

    if (uartBuf[0] == '{') {
        receiving = 1;
    }
    if (receiving == 1) {
        EventBuf[eventpos] = uartBuf[0];
        ++eventpos;
        if (uartBuf[0] == '}' && eventpos == 6) {
            receiving = 0;
            completeEvent = 1;
        }
    }

    if (completeEvent == 1) {
        push_back(EventBuf);
        eventpos = 0;
        completeEvent = 0;
    }
    HAL_UART_Receive_DMA(&huart5, uartBuf, 1);
}

wearable_event_t poll_wearable() {
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