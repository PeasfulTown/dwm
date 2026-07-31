/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

// dunst volume scripts
static const char *mutecmd[]    =   { "volume_control", "mute", NULL };
static const char *volupcmd[]   =   { "volume_control", "up", NULL};
static const char *voldowncmd[] =   { "volume_control", "down", NULL};

// media player controls
static const char *mediaplaycmd[]     = { "playerctl", "play-pause", NULL };  
static const char *mediapausecmd[]    = { "playerctl", "play-pause", NULL };
static const char *medianextcmd[]     = { "playerctl", "next", NULL };
static const char *mediapreviouscmd[] = { "playerctl", "previous", NULL };

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 0;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 0;       /* vert inner gap between windows */
static const unsigned int gappoh    = 0;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 0;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Pragmata Pro Mono:style=Bold:size=14", };
static const char dmenufont[]       = "Pragmata Pro Mono:size=14";

// colorscheme
#include "themes/modus-operandi.h"

// Sticky indicator drawing
static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};	/* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const unsigned int ulinepad	= 8;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
    /* class         instance           title           tags mask  iscentered   isfloating      isterminal  noswallow   monitor  scratch_key */
    {"Gimp",         NULL,              NULL,           0,         0,           1,              0,          0,          -1,      0},
    {"Firefox",      NULL,              NULL,           1<<8,      0,           0,              0,          -1,         -1,      0},
    {"Alacritty",    NULL,              NULL,           0,         0,           0,              1,          0,          -1,      0},
    //Gaming
    {"Steam",        NULL,              NULL,           1<<8,      0,           0,              0,          0,          0,       0},
    {NULL,           "steamwebhelper",  NULL,           1<<8,      0,           0,              0,          0,          0,       0},
    {"Lutris",       NULL,              NULL,           1<<7,      0,           0,              0,          0,          0,       0},
    //Communication
    {"thunderbird",  NULL,              NULL,           1<<8,      0,           0,              0,          0,          0,       0},
    {NULL,           NULL,              "Event Tester", 0,         0,           0,              0,          1,          -1,      0},
    //Scratchpads
    {NULL,           NULL,              "notesscratch", 0,         1,           1,              1,          1,          -1,      'n'},
    {NULL,           NULL,              "auctrl",       0,         1,           1,              1,          1,          -1,      'a'},
    {NULL,           "keepassxc",       NULL,           0,         1,           1,              0,          1,          -1,      'k'},
    {NULL,           NULL,              "muplr",        0,         1,           1,              1,          1,          -1,      'm'},
};

/* layout(s) */
static const float mfact        = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster        = 1;    /* number of clients in master area */
static const int resizehints    = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0;    /* 1 will force focus on the fullscreen window */
static const int refreshrate    = 60;  /* refresh rate (per second) for client move/resize */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	// { "[@]",      spiral },
	// { "[\\]",     dwindle },
	// { "H[]",      deck },
	// { "TTT",      bstack },
	{ "===",      bstackhoriz },
	// { "HHH",      grid },
	// { "###",      nrowgrid },
	// { "---",      horizgrid },
	// { ":::",      gaplessgrid },
	// { "|M|",      centeredmaster },
	// { ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *brn_up_cmd[]             = { "brightness_control", "up", NULL };
static const char *brn_dn_cmd[]             = { "brightness_control", "down", NULL };
static char dmenumon[2]                     = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]               = { "dmenu_run"
                                              , "-m", dmenumon
                                              , "-fn", dmenufont
                                              , "-nb", col_bg_norm
                                              , "-nf", col_fg_norm
                                              , "-sb", col_bg_sel
                                              , "-sf", col_fg_sel
                                              , NULL };
static const char *termcmd[]                = { "alacritty", NULL };
static const char *screenshotselectcmd[]    = { "scrots", NULL };
static const char *dmenuunicode[]           = { "dmenuunicode", NULL };
static const char *dunstcontext[]           = { "dunstctl", "context", NULL };
static const char *dunstclose[]             = { "dunstctl", "close", NULL };
static const char *rofi[]                   = { "rofi", "-show", "drun", NULL };
static const char *xcolor[]                 = { "xcolor", "-s", NULL };
static const char *filemanager[]            = { "pcmanfm-qt", NULL };

/*First arg only serves to match against key in rules*/
static const char *peanotescmd[]    = { "n", "alacritty", "-T", "notesscratch", "--working-directory", "./Documents/peanotes", "-e", "nvim", NULL };
static const char *auctrlcmd[]      = { "a", "alacritty", "-T", "auctrl", "-e", "pulsemixer", NULL };
static const char *muplrcmd[]       = { "m", "alacritty", "-T", "muplr", "-e", "ncmpcpp", NULL };
static const char *keepassxc[]      = { "k", "keepassxc", NULL };


#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key        function        argument */

    // Program Controls
    { MODKEY,                       XK_p,      spawn,           {.v = dmenucmd } },
    { MODKEY,                       XK_Return, spawn,           {.v = termcmd } },
    { MODKEY,			    XK_grave,  spawn,	        {.v = dmenuunicode } },
    { MODKEY,			    XK_d,      spawn,	        {.v = dunstcontext } },
    { MODKEY|ShiftMask,		    XK_d,      spawn,	        {.v = dunstclose } },
    { MODKEY,                       XK_c,      spawn,           {.v = rofi } },
    { MODKEY,                       XK_q,      killclient,      {0} },
    { MODKEY|ShiftMask,             XK_s,      spawn,           {.v = screenshotselectcmd } },
    { MODKEY|ShiftMask,             XK_c,      spawn,           {.v = xcolor } },
    { MODKEY,                       XK_e,      spawn,           {.v = filemanager } },
    
    // Screen Brightness Controls
    { 0,                            XF86XK_MonBrightnessUp,    spawn, {.v = brn_up_cmd } },
    { 0,                            XF86XK_MonBrightnessDown,  spawn, {.v = brn_dn_cmd } },
    
    // Media Controls
    { 0,                            XF86XK_AudioMute,          spawn, {.v = mutecmd } },
    { 0,                            XF86XK_AudioLowerVolume,   spawn, {.v = voldowncmd } },
    { 0,                            XF86XK_AudioRaiseVolume,   spawn, {.v = volupcmd } },
    { 0,                            XF86XK_AudioPlay,          spawn, {.v = mediaplaycmd } },
    { 0,                            XF86XK_AudioPause,         spawn, {.v = mediapausecmd } },
    { 0,                            XF86XK_AudioNext,          spawn, {.v = medianextcmd } },
    { 0,                            XF86XK_AudioPrev,          spawn, {.v = mediapreviouscmd } },

    // WM Controls
    { MODKEY,                       XK_b,      togglebar,      {0} },
    { MODKEY,                       XK_s,      togglesticky,   {0} },
    
    { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
    
    { MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
    { MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
    
    { MODKEY|ControlMask,           XK_i,      incnmaster,     {.i = +1 } },
    { MODKEY|ControlMask,           XK_d,      incnmaster,     {.i = -1 } },
    
    { MODKEY,                       XK_Tab,    view,           {0} },
    { MODKEY,                       XK_space,  zoom,           {0} },
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },

    // Multi-monitor Controls
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { MODKEY,                       XK_bracketright,  viewnext,       {0} },
    { MODKEY,                       XK_bracketleft,   viewprev,       {0} },
    { MODKEY|ShiftMask,             XK_bracketright,  tagtonext,      {0} },
    { MODKEY|ShiftMask,             XK_bracketleft,   tagtoprev,      {0} },

    // Resize Clients
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
    { MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
    { MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },

    // Layouts
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
    { MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
    { MODKEY|ControlMask,	    XK_comma,  cyclelayout,    {.i = -1 } },
    { MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
    { MODKEY|ControlMask,           XK_space,  setlayout,      {0} },
    
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY,                       XK_f,      togglefullscr,  {0} },

    // Client Resize
    { MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
    { MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
    { MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
    { MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
    { MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
    { MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
    { MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
    { MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
    { MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
    { MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
    { MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
    { MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },
    { MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
    { MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
    { MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
    { MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },
    
    { MODKEY,            	    XK_y,      togglescratch,  {.v = muplrcmd } },
    { MODKEY,            	    XK_n,      togglescratch,  {.v = peanotescmd } },
    { MODKEY,            	    XK_a,      togglescratch,  {.v = auctrlcmd } },
    { MODKEY,            	    XK_i,      togglescratch,  {.v = keepassxc } },

    // Gaps
    { MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
    { MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
    { MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
    { MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
    { MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
    { MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
    { MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
    { MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
    { MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
    { MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
    { MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
    { MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
    { MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
    { MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
    { MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
    { MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },

    // Tags
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)

    // Quit/Restart dwm
    { MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {0} },
    { MODKEY|ShiftMask,             XK_q,      quit,           {1} }, 
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

