# TouchGuard

Disables Mac touchpad for a user-specified amount of time each time a key is pressed on the keyboard. This prevents accidental touchpad input (e.g. palm of hand moving over the edge of the touchpad) from being detected as a tap and causing the cursor to jump to a different line while the user is typing.

**Download latest release from [here](https://github.com/thesyntaxinator/TouchGuard/releases)**

*NOTE: Must be run with administrative privileges.*

----------------
## Usage (non-tech savvy)
- Open Terminal, type "chmod +x ", drag the drop the downloaded file into the Terminal window, and press enter (this only needs to be done once after downloading the file).
- Then type "sudo", drag and drop the downloaded file into the Terminal window, type " -time 0.2" and press enter. You may be prompted for your password; if so, type it and presss enter. Note that you will not see the cursor move while typing your password -- this is normal and done for security reasons.
- Keep the terminal window open. If you close the window, the program will exit. You can hide the window by typing "command-h".
- You will need to manually relaunch the app each time you restart your computer using the above sequence of steps. A future goal is to create an installer with an option to automatically run the program (with elevated privileges) every time the computer starts. If you would like to work on this, feel free to fork the project and let me know if you get it working (see contact info under "Support" below).

------------------
## Sample command line usage (for the more tech-savvy)
`
# make the downloaded release file executable
chmod +x TouchGuard
# run it
sudo ./TouchGuard -time 0.2
`

Launches TouchGuard with a time interval of 200 ms (disables the touchpad for 200 ms each time a key is pressed on the keyboard). I have found this to be effective for me -- if you are still having issues (e.g. you can't use the trackpad immediately after typing, or your cursor still jumps), you can adjust the time interval up or down as needed.

*Note: You will need to manually relaunch the app each time you restart your computer. A future goal is to create an installer with an option to automatically run the program (with elevated privileges) every time the computer starts. If you would like to work on this, feel free to fork the project and let me know if you get it working (see contact info under "Support" below).*

----------------
## Support
Questions? Comments? Feedback? Issues? Open a new issue [here](https://github.com/thesyntaxinator/TouchGuard/issues) or email syntaxsoftsupport@icloud.com.

