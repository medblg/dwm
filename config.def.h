/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include "gaplessgrid.c"
/*static const char *upvol[]   = { "amixer", "set", "Master", "5+",     NULL };
static const char *downvol[] = { "amixer", "set", "Master", "5-",     NULL };
static const char *mutevol[] = { "amixer", "set", "Master", "toggle", NULL };
static const char *mutemic[] = { "amixer", "set", "Capture", "toggle", NULL };
*/

/* using pactl instead (amixer some error don't want to toggle) */

/*static const char *upvol[]   = { "pactl", "set-sink-volume", "0", "5+%",     NULL };*/
static const char *upvol[]   = { "amixer", "set", "Master", "5%+",     NULL };
/*static const char *downvol[] = { "pactl", "set-sink-volume", "0", "-5%",     NULL };*/
static const char *downvol[] = { "amixer", "set", "Master", "5%-",     NULL };
static const char *mutevol[] = { "pactl", "set-sink-mute", "0", "toggle", NULL };
static const char *mutemic[] = { "amixer", "set", "Capture", "toggle", NULL };

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
/*  Display modes of the tab bar: never shown, always shown, shown only in  */
/*  monocle mode in the presence of several windows.                        */
/*  Modes after showtab_nmodes are disabled.                                */
enum showtab_modes { showtab_never, showtab_auto, showtab_nmodes, showtab_always};
static const int showtab			= showtab_nmodes;        /* Default tab bar show mode */
static const int toptab				= False;               /* False means bottom tab bar */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const char *fonts[]          = { "FontAwesome:size=12", "monospace:size=11" };
static const char dmenufont[]       = "monospace:size=11";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#a30005"; /*005577 E45039 745039 090a9a 2e2726 */
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* dwm-scratchpads */
typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = { "st", "-t", "scratchpad", "-g", "120x34", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"TermScratch",      spcmd1},
};

/* tagging */
static const char *tags[] = { " ", "", "", " " }; 

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	    /* class      instance    title       tags mask     isfloating   monitor */
	{ "Firefox",  NULL,       NULL,                    1,	   0,           -1 },
	{ "Opera",     NULL,       NULL,                   1,	   0,           -1 },
	{ "Google-chrome",  NULL,       NULL,              1,	   0,           -1 },
	{ "Chromium-browser",  NULL,       NULL,           1,	   0,           -1 },
	{ "Geany",  NULL,       NULL,                   1<<1,      0,           -1 },
	{ "Mousepad",  NULL,       NULL,                1<<1,      0,           -1 },
	{ "Terminator",     NULL,       NULL,		1<<2,      0,           -1 },
	{ "Xfce4-terminal",     NULL,       NULL,		1<<2,      0,           -1 },
	/*{ "TermScratch",     NULL,       NULL,		   0,      1,           -1 },*/
	{ "TermScratch",     NULL,       NULL,	    SPTAG(0),      1,           -1 },
	{ "burp-StartBurp",     NULL,       NULL,	1<<3,      1,           -1 },
	{ "Thunar",     NULL,       NULL,	        1<<3,      0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "###",      gaplessgrid },
	{ "[D]",      deck },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
/*static const char *termcmd[]  = { "st", NULL };*/
static const char *termcmd[]  = { "xfce4-terminal", NULL };
/*static const char *termcmd[]  = { "terminator", NULL };*/
static const char *firefox[]  = { "firefox", NULL };
static const char *chromium[]     = { "google-chrome", NULL };
/*static const char *chromium[]     = { "chromium", NULL };*/
static const char *gvim[]     = { "gvim", NULL };
static const char *xkill[]    = { "xkill", NULL };
static const char *burp[]     = { "exo-open", "/home/s1m0x/.config/dots_sec/burp3.desktop", NULL };
static const char *gromit[]   = {"gromit-mpx", NULL };

static const char scratchpadname[] = "TermScratch";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
        { 0,                       XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
        { 0,                       XF86XK_AudioMute, spawn, {.v = mutevol } },
        { 0,                       XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },
        { 0,                       XF86XK_AudioMicMute, spawn, {.v = mutemic   } },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_w,      tabmode,        {-1} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_x,      killunsel,      {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	/*{ MODKEY|ShiftMask,           XK_0,      tag,            {.ui = ~0 } },*/
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_s,      togglesticky,   {0} },
	/*{ MODKEY,                     XK_o,      incrihgaps,     {.i = -1 } },*/
	/* gaps handling keys */
	{ Mod4Mask,			XK_equal,  defaultgaps,    {0} },
	{ Mod4Mask,                     XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
	{ MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },
	/*{ MODKEY,                     XK_y,      incrihgaps,     {.i = +1 } },*/
	/*{ MODKEY,                     XK_o,      incrihgaps,     {.i = -1 } },*/
	{ MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} }, 
	{ MODKEY,                       XK_o,      winview,        {0} },
	/* run programs */
	{ Mod4Mask,                     XK_f,      spawn,          {.v = firefox} },
	{ Mod4Mask,                     XK_c,      spawn,          {.v = chromium} },
	{ Mod4Mask,                     XK_g,      spawn,          {.v = gvim} },
	{ Mod4Mask,                     XK_x,      spawn,          {.v = xkill} },
	{ Mod4Mask,                     XK_b,      spawn,          {.v = burp} },
	{ MODKEY,                       XK_F9,      spawn,          {.v = gromit} },

	{ MODKEY,                       XK_minus, scratchpad_show, {1} },
	{ MODKEY|ShiftMask,             XK_minus, scratchpad_hide, {0} },
	{ MODKEY,                       XK_equal,scratchpad_remove,{0} },
/*{ MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } },*/
	{ MODKEY,            		XK_grave,  	   togglescratch,  {.ui = 0 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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
	{ ClkTabBar,            0,              Button1,        focuswin,       {0} },
};

