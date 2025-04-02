#ifndef EVENT_MANAGER_H_
#define EVENT_MANAGER_H_

#include <stdint.h>
#include <stdbool.h>

// Forward declaration needed for OnEventHandler type
struct Event;

/**
 * Event handler function type.
 * @param[in] event event handle
 * @param[in] scheduledTime time at which the event was planned
 * @param[in] context context of the event
 */
typedef void (*OnEventHandler)(struct Event* event, uint64_t scheduledTime, void* context);

/// Event descriptor
typedef struct Event {
	bool isScheduled;			//< Flag stating if the event is scheduled (true) or not (false)
	uint64_t scheduledTime;		//< Time at which the event is scheduled
	OnEventHandler onEvent;		//< Pointer to user handler function
	void* context;				//< Pointer to user context data
	struct Event* next;         //< Pointer to the next element in the list
} Event;


/**
 * Initializes the event manager. This function should clear all information about all events.
 */
void EVENT_MANAGER_Init(void);

/**
 * Registers an event.
 *
 * @param[in] pointer to the Event description structure
 * @param[in] onEvent callback function to be called when the event is run
 * @param[in] context optional pointer to pass to the event (NULL if unused)
 *
 * @return true if event was registered, false otherwise
 */
bool EVENT_MANAGER_RegisterEvent(Event* event, OnEventHandler onEvent, void* context);

/**
 * Unregisters an event.
 *
 * @param[in] pointer to the Event description structure
 */
void EVENT_MANAGER_UnregisterEvent(Event* event);

/**
 * Schedules an event to be executed at a given time.
 *
 * @param[in] pointer to the Event description structure
 * @param[in] time time at which the event should run
 *
 * @return true if event was scheduled, false otherwise
 */
bool EVENT_MANAGER_ScheduleEvent(Event* event, uint64_t time);

/**
 * Processes the events and executes event handlers. This function should be called within main program loop.
 *
 * @param[in] currentTime current time
 */
void EVENT_MANAGER_Proc(uint64_t currentTime);

#endif /* EVENT_MANAGER_H_ */
