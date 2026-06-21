# Cat Litter Reminder

A small ESP32-based device that tracks and displays when a household member last cleaned the cat litter, with planned future reminders (via Home Assistant) and cat-mood animations.

## Language

**Cleaning Event**:
The act of a household member cleaning the cat litter, recorded by pressing the device's button. Only the most recent Cleaning Event is retained — earlier ones are overwritten, not logged.
_Avoid_: Clean, scoop, press

**Last Cleaned Timestamp**:
The point in time the most recent Cleaning Event occurred. Persisted to flash so it survives power loss and reboots, independent of the device's live clock.
_Avoid_: Last cleaned date, last press time

**Household Member**:
Any person who presses the button to record a Cleaning Event. The device does not distinguish between individual Household Members — there is no per-person identity or tracking.
_Avoid_: User, person, owner

**Time Sync**:
The device obtaining accurate current time from an NTP server over WiFi on boot (and periodically thereafter). A Cleaning Event cannot be recorded until at least one successful Time Sync has occurred, to prevent recording an incorrect timestamp.
_Avoid_: Clock, NTP (as a synonym for the whole process)

**Mood** (future):
A display state derived from how much time has elapsed since the Last Cleaned Timestamp, intended to drive the planned cat animation feature. One of four values, in increasing order of elapsed time: Happy, Content, Annoyed, Disgusted. Not yet implemented.
_Avoid_: Status, state

**Reminder** (future):
A notification sent to the household, triggered by Home Assistant when the time elapsed since the Last Cleaned Timestamp crosses a threshold that Home Assistant — not the device — decides. Not yet implemented.
_Avoid_: Alert, notification (as the project-specific concept)
