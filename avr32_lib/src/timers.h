/* timers.c
 * aleph
 *
 * ultra-simple software timer API
 */


#ifndef _TIMERS_H_
#define _TIMERS_H_

#include "types.h"

//------------------------------
//----- types

// callback function
typedef void (*timer_callback_t)( void* caller );

// timer class, element in linked list
typedef struct _softTimer {
  // decreasing count of ticks remaining
  u32 ticksRemain;
  // interval in ticks
  u32 ticks;
  // callback function pointer
  timer_callback_t callback;   	
  // pointer to next timer in linked list
  // this should be initialized to NULL
  // FIXME? guess we should store a pointer to the last element too,
  /// it would make searching a little faster/cleaner
  struct _softTimer* next;
  // arbitrary argument to differentiate different callers
  // using the same callback
  void* caller;
} softTimer_t;

//------------------------------
//---- functions

// initialize timers
void init_timers( void );
// add a timer to the processing list
// return 1 if set, 0 if not
u8 timer_add( softTimer_t* timer, u32 ticks, timer_callback_t callback, void* valler);
// find remove a timer from the processing list
// return 1 if removed, 0 if not found
u8 timer_remove( softTimer_t* timer );
// process the timer list; call this on each tick.
void process_timers( void );

#endif // header guard
