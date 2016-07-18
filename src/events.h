#ifndef EVENTS_H
#define EVENTS_H 1

#include <pebble.h>

// Ensure changes here are synced to ImagesByEvent definition below.
// Because C doesn't have built-in Hashmaps :(
typedef enum Events {
  EventWakeUp,
  EventBreakfast,
  EventMorningBrushTeeth,
  EventGetDressed,
  EventShoes,
  EventSchool,
  EventPlayground,
  EventHome,
  EventLunch,
  EventPlay,
  EventMusic,
  EventQuiet,
  EventAfternoonReading,
  EventDinner,
  EventBath,
  EventPyjamas,
  EventMilk,
  EventEveningBrushTeeth,
  EventEveningReading,
  EventBed
} Events;

// Ensure changes here are synced to Events enum definition above.
// Because C doesn't have built-in Hashmaps :(
static const int ImagesByEvent[] = {
  /* EventWakeUp */            RESOURCE_ID_IMAGE_SUNRISE,
  /* EventBreakfast */         RESOURCE_ID_IMAGE_BOWL_OF_FOOD,
  /* EventMorningBrushTeeth */ RESOURCE_ID_IMAGE_TOOTHBRUSH,
  /* EventGetDressed */        RESOURCE_ID_IMAGE_SHIRT,
  /* EventShoes */             RESOURCE_ID_IMAGE_SOCK,
  /* EventSchool */            RESOURCE_ID_IMAGE_LETTER_BLOCKS,
  /* EventPlayground */        RESOURCE_ID_IMAGE_SLIDE,
  /* EventHome */              RESOURCE_ID_IMAGE_HOME,
  /* EventLunch */             RESOURCE_ID_IMAGE_BURGER,
  /* EventPlay */              RESOURCE_ID_IMAGE_CAR,
  /* EventMusic */             RESOURCE_ID_IMAGE_MUSIC_NOTE,
  /* EventQuiet */             RESOURCE_ID_IMAGE_HEADPHONE,
  /* EventAfternoonReading */  RESOURCE_ID_IMAGE_BOOK,
  /* EventDinner */            RESOURCE_ID_IMAGE_FOOD_PLATE,
  /* EventBath */              RESOURCE_ID_IMAGE_BATH_TUB,
  /* EventPyjamas */           RESOURCE_ID_IMAGE_PYJAMAS,
  /* EventMilk */              RESOURCE_ID_IMAGE_MILK_CARTON,
  /* EventEveningBrushTeeth */ RESOURCE_ID_IMAGE_TOOTHBRUSH,
  /* EventEveningReading */    RESOURCE_ID_IMAGE_BOOK,
  /* EventBed */               RESOURCE_ID_IMAGE_BED,
};

#define SCHEDULED_EVENT_TUPLE_SIZE 3

// yes, struct, I know, whatever... this works
#define SCHEDULED_EVENT_HOUR_INDEX 0
#define SCHEDULED_EVENT_MINUTE_INDEX 1
#define SCHEDULED_EVENT_EVENT_INDEX 2

// IMPORTANT: Keep this in chronological order so that
// we don't need to sort it programmatically every time we
// want to find something.
static const int scheduled_events[][SCHEDULED_EVENT_TUPLE_SIZE] = {
  /* hour, minute, event */
  {  7, 30, EventWakeUp },
  {  7, 40, EventBreakfast },
  {  8, 10, EventMorningBrushTeeth },
  {  8, 15, EventGetDressed },
  {  8, 25, EventShoes },
  {  8, 30, EventSchool },
  { 12, 15, EventPlayground },
  { 12, 45, EventHome },
  { 13, 00, EventLunch },
  { 13, 30, EventPlay },
  { 14, 00, EventQuiet },
  { 15, 00, EventMusic },
  { 17, 30, EventDinner },
  { 18, 00, EventBath },
  { 18, 45, EventPyjamas },
  { 19, 10, EventMilk },
  { 19, 20, EventEveningBrushTeeth },
  { 19, 30, EventEveningReading },
  { 19, 45, EventBed },
};

#endif
