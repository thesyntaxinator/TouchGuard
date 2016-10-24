TouchGuard

Disables Mac touchpad for a user-specified amount of time each time a key is pressed on the keyboard. This prevents accidental touchpad input (e.g. palm of hand moving over the edge of the touchpad) from being detected as a tap and causing the cursor to jump to a different line while the user is typing.

*NOTE: Must be run with administrative privileges.*

Usage: sudo ./TouchGuard -time 0.2
Launches TouchGuard with a time interval of 200 ms (disables the touchpad for 200 ms each time a key is pressed on the keyboard). I have found this to be effective for me -- if you are still having issues (e.g. you can't use the trackpad immediately after typing, or your cursor still jumps), you can adjust the time interval up or down as needed.

Comments