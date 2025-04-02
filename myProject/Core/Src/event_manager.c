#include "event_manager.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

// First element on the event list or NULL if list if empty
static Event* head = NULL;

void EVENT_MANAGER_Init(void) {
    head = NULL;
}

bool EVENT_MANAGER_RegisterEvent(Event* event, OnEventHandler onEvent, void* context) {
    if (event == NULL) {
        return false;
    }

    Event* current = head;
    while (current != NULL) {
        if (event == current) {

            current->onEvent = onEvent;
            current->context = context;
            return true;
        }
        current = current->next;
    }

    event->isScheduled = false;
    event->scheduledTime = 0;
    event->onEvent = onEvent;
    event->context = context;
    event->next = NULL;

    if (head == NULL) {
        head = event;
    } else {
        current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = event;
    }

    return true;
}




void EVENT_MANAGER_UnregisterEvent(Event* event) {
    if (event == NULL || head == NULL) {
        return;
    }

    if (head == event) {
        head = event->next;
        event->next = NULL;
        event->isScheduled = false;
        return;
    }

    Event* current = head;
    while (current->next != NULL && current->next != event) {
        current = current->next;
    }

    if (current->next == event) {
        current->next = event->next;
        event->next = NULL;
        event->isScheduled = false;
    }
}

bool EVENT_MANAGER_ScheduleEvent(Event* event, uint64_t time) {
    if (event == NULL) {
        return false;
    }

    event->scheduledTime = time;
    event->isScheduled = true;
    return true;
}

void EVENT_MANAGER_Proc(uint64_t currentTime) {
    Event* current = head;
    while (current != NULL) {
        if (current->isScheduled && current->scheduledTime <= currentTime) {
            current->isScheduled = false;
            if (current->onEvent) {
                current->onEvent(current, current->scheduledTime, current->context);
            }
        }
        current = current->next;
    }
    return;
}
