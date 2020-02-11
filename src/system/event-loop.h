/** \file event-loop.h
  * \brief Contains the evloop_t struct and functions to manipulate it
  */

#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <stdbool.h>
#include "../utils/circular-queue.h"
#include "../utils/closure.h"
#include "../system/event.h"
#include "../system/syspools.h"

/** \brief The event loop object
  *
  * This object represents an event loop. It contains a two queues:
  * 1. The inbound event queue, which is the feeding point of the event loop.
  * 2. The outbound schedule queue, which holds reusable events already run.
  */
typedef struct evloop evloop_t;
struct evloop{
    syspools_t *pools; //!< Quick reference to the system's pools

    /** \brief The system's event queue
      *
      * The event queue hold events ready to be processed.
      * Any event directly pushed into this queue or
      * enqueued through any of the `evloop_enqueue`
      * functions wiill be run sequentially on the next
      * runloop.
      */
    cqueue_t *event_queue;

    /** \brief The system's schedule queue
      *
      * The schedule queue holds events that are not immediately disposable
      * (i.e. recurring timers) that have already been processed and should be
      * put back on the scheduler's queue.
      */
     cqueue_t *schedule_queue;
};

/** \brief Initialises an event loop
  *
  * \param event_loop The evloop_t instance to be initialised
  * \param pools The system's internal pools
  * \param event_queue The system's event queue
  * \param schedule_queue The system's schedule queue
  */
void evloop_init(
    evloop_t *event_loop,
    syspools_t *pools,
    cqueue_t *event_queue,
    cqueue_t *schedule_queue
);

/** \brief Triggers a runloop.
  *
  * This function flushes the event queue and processes each event in it.
  * Afterwards, depending on the event type, it disposes of the event in
  * different ways.
  *
  * Each iteration of this cycle is called a runloop.
  *
  * \param event_loop The evloop_t instance to be run
  */
void evloop_run(evloop_t *event_loop);

/** \brief Enqueues an arbitrary event to be processed
  *
  * \param event_loop The evloop_t instance into which the event wil be put
  * \param event The event to be enqueued
  */
void evloop_enqueue_event(evloop_t *event_loop, event_t *event);

/** \brief Enqueues a closure to be invoked
  *
  * \param event_loop The evloop_t instance into which the closure will be enqueued
  * \param closure The closure to be enqueued
  */
void evloop_enqueue_closure(evloop_t *event_loop, closure_t *closure);

#endif /* end of include guard: EVENT_LOOP_H */
