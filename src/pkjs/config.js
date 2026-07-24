module.exports = [
    {
        "type": "heading",
        "defaultValue": "TIMEGAL Settings"
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Animation"
            },
            {
                "type": "select",
                "messageKey": "Frequency",
                "label": "Auto-Animate",
                "defaultValue": "1",
                "options": [
                    { "label": "Every Hour", "value": "0" },
                    { "label": "Every Minute", "value": "1" },
                    { "label": "Startup Only", "value": "3" },
                    { "label": "All Animation Off", "value": "2" }
                ]
            },
            {
                "type": "toggle",
                "messageKey": "AnimateOnFlick",
                "label": "Animate on Wrist Flick",
                "defaultValue": false
            },
            {
                "type": "toggle",
                "messageKey": "BluetoothDisconnectAnimation",
                "label": "Bluetooth Disconnect Animation",
                "defaultValue": true
            }
        ]
    },
    {
        "type": "section",
        "capabilities": ["NOT_PLATFORM_EMERY"],
        "items": [
            {
                "type": "heading",
                "defaultValue": "Display"
            },
            {
                "type": "select",
                "messageKey": "HudContent",
                "label": "Top HUD Content",
                "defaultValue": "1",
                "options": [
                    { "label": "Current Date (CURRENT)", "value": "1" },
                    { "label": "Weekday (WEEKDAY)", "value": "2" },
                    { "label": "Seconds (SECONDS)", "value": "4" },
                    { "label": "Phone Battery (BATTERY)", "value": "3" },
                    { "label": "Off", "value": "0" }
                ],
                "description": "Phone Battery only works if your phone's browser JavaScript environment exposes it. In the case it is unsupported, N/A will be shown."
            },
            {
                "type": "toggle",
                "messageKey": "CurrentRight",
                "label": "Move Top HUD to Right Side",
                "defaultValue": false,
                "capabilities": ["NOT_PLATFORM_CHALK"]
            },
            {
                "type": "toggle",
                "messageKey": "FlashCurrentWhileCharging",
                "label": "Flash Top HUD Heading While Charging",
                "defaultValue": true
            },
            {
                "type": "toggle",
                "messageKey": "PlayerMode",
                "label": "Top HUD Heading Uses PLAYER1/PLAYER2",
                "defaultValue": false
            },
            {
                "type": "toggle",
                "messageKey": "ShowBattery",
                "label": "Show Watch Battery (GAL X)",
                "defaultValue": true,
                "description": "The X shows the tens digit of your watch battery. For example: 90% shows as GAL 9. (100% also shows as GAL 9.)"
            }
        ]
    },
    {
        "type": "section",
        "capabilities": ["PLATFORM_EMERY"],
        "items": [
            {
                "type": "heading",
                "defaultValue": "Display"
            },
            {
                "type": "select",
                "messageKey": "HudContentLeft",
                "label": "Top HUD Content (Left Side)",
                "defaultValue": "1",
                "options": [
                    { "label": "Current Date (CURRENT)", "value": "1" },
                    { "label": "Weekday (WEEKDAY)", "value": "2" },
                    { "label": "Seconds (SECONDS)", "value": "4" },
                    { "label": "Phone Battery (BATTERY)", "value": "3" },
                    { "label": "Off", "value": "0" }
                ]
            },
            {
                "type": "select",
                "messageKey": "HudContentRight",
                "label": "Top HUD Content (Right Side)",
                "defaultValue": "2",
                "options": [
                    { "label": "Current Date (CURRENT)", "value": "1" },
                    { "label": "Weekday (WEEKDAY)", "value": "2" },
                    { "label": "Seconds (SECONDS)", "value": "4" },
                    { "label": "Phone Battery (BATTERY)", "value": "3" },
                    { "label": "Off", "value": "0" }
                ],
                "description": "Phone Battery only works if your phone's browser JavaScript environment exposes it. In the case it is unsupported, N/A will be shown."
            },
            {
                "type": "select",
                "messageKey": "FlashSide",
                "label": "Flash Top HUD Heading While Charging",
                "defaultValue": "1",
                "options": [
                    { "label": "Left Side", "value": "1" },
                    { "label": "Right Side", "value": "2" },
                    { "label": "Off", "value": "0" }
                ]
            },
            {
                "type": "toggle",
                "messageKey": "PlayerMode",
                "label": "Top HUD Heading(s) Use PLAYER1/PLAYER2",
                "defaultValue": false
            },
            {
                "type": "toggle",
                "messageKey": "ShowBattery",
                "label": "Show Watch Battery (GAL X)",
                "defaultValue": true,
                "description": "The X shows the tens digit of your watch battery. For example: 90% shows as GAL 9. (100% also shows as GAL 9.)"
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Haptics"
            },
            {
                "type": "select",
                "messageKey": "VibrateOnAnimationType",
                "label": "On Animation",
                "defaultValue": "0",
                "options": [
                    { "label": "Short Vibration", "value": "1" },
                    { "label": "Long Vibration", "value": "2" },
                    { "label": "Double Vibration", "value": "3" },
                    { "label": "Off", "value": "0" }
                ]
            },
            {
                "type": "toggle",
                "messageKey": "SuppressStartupVibes",
                "label": "Suppress Startup Animation Vibrations",
                "defaultValue": true
            },
            {
                "type": "select",
                "messageKey": "VibrateOnBTDisconnectType",
                "label": "On Bluetooth Disconnect",
                "defaultValue": "0",
                "options": [
                    { "label": "Short Vibration", "value": "1" },
                    { "label": "Long Vibration", "value": "2" },
                    { "label": "Double Vibration", "value": "3" },
                    { "label": "Off", "value": "0" }
                ]
            },
            {
                "type": "select",
                "messageKey": "VibrateEveryHourType",
                "label": "Every Hour",
                "defaultValue": "0",
                "options": [
                    { "label": "Short Vibration", "value": "1" },
                    { "label": "Long Vibration", "value": "2" },
                    { "label": "Double Vibration", "value": "3" },
                    { "label": "Off", "value": "0" }
                ]
            },
            {
                "type": "toggle",
                "messageKey": "VibrateDuringQuietTime",
                "label": "Allow Vibration During Quiet Time",
                "defaultValue": false,
                "capabilities": ["NOT_PLATFORM_APLITE"]
            }
        ]
    },
    {
        "type": "section",
        "capabilities": ["PLATFORM_FLINT"],
        "items": [
            {
                "type": "heading",
                "defaultValue": "Sound"
            },
            {
                "type": "toggle",
                "messageKey": "PlaySoundOnAnimation",
                "label": "Play Sound On Animation",
                "defaultValue": false
            },
            {
                "type": "toggle",
                "messageKey": "SuppressStartupSounds",
                "label": "Suppress Startup Animation Sounds",
                "defaultValue": true
            },
            {
                "type": "toggle",
                "messageKey": "PlaySoundOnBTDisconnect",
                "label": "Play Sound On Bluetooth Disconnect",
                "defaultValue": false
            },
            {
                "type": "toggle",
                "messageKey": "PlaySoundEveryHour",
                "label": "Play Sound Every Hour",
                "defaultValue": false
            },
            {
                "type": "toggle",
                "messageKey": "SoundDuringQuietTime",
                "label": "Allow Sound During Quiet Time",
                "defaultValue": false
            }
        ]
    },
    {
        "type": "section",
        "capabilities": ["PLATFORM_EMERY"],
        "items": [
            {
                "type": "heading",
                "defaultValue": "Sound"
            },
            {
                "type": "toggle",
                "messageKey": "PlaySoundOnAnimation",
                "label": "Play Sound On Animation",
                "defaultValue": false
            },
            {
                "type": "toggle",
                "messageKey": "SuppressStartupSounds",
                "label": "Suppress Startup Animation Sounds",
                "defaultValue": true
            },
            {
                "type": "toggle",
                "messageKey": "PlaySoundOnBTDisconnect",
                "label": "Play Sound On Bluetooth Disconnect",
                "defaultValue": false
            },
            {
                "type": "toggle",
                "messageKey": "PlaySoundEveryHour",
                "label": "Play Sound Every Hour",
                "defaultValue": false
            },
            {
                "type": "toggle",
                "messageKey": "SoundDuringQuietTime",
                "label": "Allow Sound During Quiet Time",
                "defaultValue": false
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Clock Format"
            },
            {
                "type": "select",
                "messageKey": "ClockMode",
                "label": "Time Format",
                "defaultValue": "0",
                "options": [
                    { "label": "System", "value": "0" },
                    { "label": "12-Hour", "value": "1" },
                    { "label": "24-Hour", "value": "2" }
                ]
            },
            {
                "type": "toggle",
                "messageKey": "LeadingZeros",
                "label": "Show Leading Zeros (08:00)",
                "defaultValue": true
            },
            {
                "type": "select",
                "messageKey": "AMPM_Position",
                "label": "A.M./P.M. Position",
                "defaultValue": "0",
                "options": [
                    { "label": "Left (A.M. 12:00)", "value": "0" },
                    { "label": "Right (12:00 A.M.)", "value": "1" },
                    { "label": "Off (12:00)", "value": "2" }
                ]
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Date Format"
            },
            {
                "type": "select",
                "messageKey": "DateFormat",
                "label": "Date Format",
                "defaultValue": "0",
                "options": [
                    { "label": "MM/DD", "value": "0" },
                    { "label": "DD/MM", "value": "1" }
                ]
            },
            {
                "type": "select",
                "messageKey": "DateSeparator",
                "label": "Date Separator",
                "defaultValue": "0",
                "options": [
                    { "label": ".", "value": "0" },
                    { "label": "/", "value": "1" },
                    { "label": "-", "value": "2" },
                    { "label": "Space", "value": "3" }
                ]
            },
            {
                "type": "select",
                "messageKey": "MonthFormat",
                "label": "Month Format",
                "defaultValue": "0",
                "options": [
                    { "label": "Numeric (12)", "value": "0" },
                    { "label": "Abbreviated (DEC)", "value": "1" }
                ]
            },
            {
                "type": "toggle",
                "messageKey": "DateLeadingZeros",
                "label": "Show Leading Zeros (01)",
                "defaultValue": true
            }
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save Settings"
    }
];