/* See LICENSE file for copyright and license details. */

/* appearance */
static const char *fonts[] = {
    "Terminus:size=9",
    "xbmicons:size=9"
};
static const char dmenufont[] = "-*-terminus-medium-r-*-*-12-*-*-*-*-*-*-*";
#define NUMCOLORS 12
static const char colors[NUMCOLORS][MAXCOLORS][8] = {
  // border foreground background
  /* { "#282a2e", "#373b41", "#1d1f21" }, // 1 = normal (grey on black) */
  /* { "#f0c674", "#c5c8c6", "#1d1f21" }, // 2 = selected (white on black) */
  /* { "#dc322f", "#1d1f21", "#f0c674" }, // 3 = urgent (black on yellow) */
  { "#444444", "#bbbbbb", "#282a2e" }, // 1 = normal
  { "#990000", "#eeeeee", "#990000" }, // 2 = selected
  { "#ffffff", "#000000", "#ffffff" }, // 3 = urgent
  { "#282a2e", "#bbbbbb", "#282a2e" }, // 4 = grey
  { "#282a2e", "#1d1f21", "#282a2e" }, // 5 = black
  { "#282a2e", "#cc6666", "#282a2e" }, // 6 = red
  { "#282a2e", "#b5bd68", "#282a2e" }, // 7 = green
  { "#282a2e", "#de935f", "#282a2e" }, // 8 = orange
  { "#282a2e", "#f0c674", "#282a2e" }, // 9 = yellow
  { "#282a2e", "#81a2be", "#282a2e" }, // A = blue
  { "#282a2e", "#b294bb", "#282a2e" }, // B = magenta
  { "#282a2e", "#8abeb7", "#282a2e" }, // C = cyan
};
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const Bool systraypinningfailfirst = True;   /* True: if pinning fails, display systray on the first monitor, False: display systray on the last onitor*/
static const Bool showsystray       = True;     /* False means no systray */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* patches */
#include "push.c"

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	/* { "Gimp",     NULL,       NULL,       0,            True,        -1 }, */
	/* { "Firefox",  NULL,       NULL,       1 << 8,       False,       -1 }, */
    { "Qalculate-gtk", NULL,  NULL,       0,            True,        -1 },
    { NULL,       NULL,       "PGPLOT Server", 0,       True,        -1 },
    { NULL,       NULL,       "Ediff",         0,       True,        -1 },
};

/* layout(s) */
static const float mfact      = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
    { "[]=",      tile },
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#include <X11/XF86keysym.h>

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", "#222222", "-nf", "#bbbbbb", "-sb", "#990000", "-sf", "#eeeeee", NULL };

static const char *termcmd[]  = { "st", "-e", "tmux", NULL };
static const char *ncmpcpp[]  = { "st", "-e", "ncmpcpp", NULL };
static const char *htop[]     = { "st", "-e", "htop", NULL };
static const char *glances[]  = { "st", "-e", "glances", NULL };

static const char *scrot[]    = { "scrot", NULL };
static const char *scrot_i[]    = { "scrot", "-s", NULL };
static const char *xscreensaver_lock[] = { "xscreensaver-command", "-lock", NULL };

static const char *emacs[]       = { "emacsclient", "-c", "-a=", NULL };
static const char *spacefm[]     = { "spacefm", NULL };
static const char *firefox[]     = { "firefox", NULL };
static const char *pavucontrol[] = { "pavucontrol", NULL };
static const char *qalculate[] = { "qalculate-gtk", NULL };

static const char *pulseaudio_ctl_up[]   = { "pulseaudio-ctl", "up", NULL };
static const char *pulseaudio_ctl_down[] = { "pulseaudio-ctl", "down", NULL };
static const char *pulseaudio_ctl_mute[] = { "pulseaudio-ctl", "mute", NULL };
static const char *pulseaudio_ctl_mic_mute[] = { "pulseaudio-ctl", "mute-input", NULL };

static const char *mpc_toggle[]   = { "mpc", "toggle", NULL };
static const char *mpc_prev[]     = { "mpc", "prev", NULL };
static const char *mpc_next[]     = { "mpc", "next", NULL };
static const char *mpc_seek_for[] = { "mpc", "seek", "+", NULL };
static const char *mpc_seek_bac[] = { "mpc", "seek", "-", NULL };
static const char *mpc_vol_up[]   = { "mpc", "volume", "+2", NULL };
static const char *mpc_vol_dn[]   = { "mpc", "volume", "-2", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	/* programs */
	{ MODKEY,                       XK_e,      spawn,          {.v = emacs } },
	{ MODKEY,                       XK_w,      spawn,          {.v = spacefm } },
	{ MODKEY,                       XK_p,      spawn,          {.v = pavucontrol } },
	{ MODKEY,                       XK_q,      spawn,          {.v = qalculate } },

	{ MODKEY,                       XK_F12,    spawn,          {.v = xscreensaver_lock } },
	{ 0,                            XK_Print,  spawn,          {.v = scrot } },
	{ ALTKEY,                       XK_Print,  spawn,          {.v = scrot_i } },
	{ MODKEY,                       XK_n,      spawn,          {.v = ncmpcpp } },
	{ MODKEY,                       XK_b,      spawn,          {.v = glances } },

    /* greysky's "pulseaudio-ctl" */
	{ 0,                            XF86XK_AudioRaiseVolume, spawn, {.v = pulseaudio_ctl_up } },
	{ 0,                            XF86XK_AudioLowerVolume, spawn, {.v = pulseaudio_ctl_down } },
	{ 0,                            XF86XK_AudioMute,        spawn, {.v = pulseaudio_ctl_mute } },
	{ 0,                            XF86XK_AudioMicMute,     spawn, {.v = pulseaudio_ctl_mic_mute } },

	/* mpd controls */
	{ MODKEY|ALTKEY,                XK_Down,   spawn,          {.v = mpc_toggle } },
	{ MODKEY|ALTKEY,                XK_Left,   spawn,          {.v = mpc_prev } },
	{ MODKEY|ALTKEY,                XK_Right,  spawn,          {.v = mpc_next } },
	{ MODKEY|ALTKEY|ControlMask,    XK_Left,   spawn,          {.v = mpc_seek_for } },
	{ MODKEY|ALTKEY|ControlMask,    XK_Right,  spawn,          {.v = mpc_seek_bac } },
    { MODKEY|ALTKEY,                XK_equal,  spawn,          {.v = mpc_vol_up } },
    { MODKEY|ALTKEY,                XK_minus,  spawn,          {.v = mpc_vol_dn } },

	/* wm commands */
	{ MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      pushdown,       {0} },
	{ MODKEY|ShiftMask,             XK_k,      pushup,         {0} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setmfact,       {.f = -0.01} },
	{ MODKEY|ShiftMask,             XK_l,      setmfact,       {.f = +0.01} },
	{ MODKEY,                       XK_Return, zoom,           {0} },

	/* { MODKEY,                       XK_Tab,    view,           {0} }, */
	{ MODKEY,                       XK_Tab,    focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Tab,    tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_o,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },

	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
    { MODKEY,                       XK_v,      togglefullscreen, {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },

	{ MODKEY,                       XK_comma,  cycle,          {.i = -1 } },
	{ MODKEY,                       XK_period, cycle,          {.i = +1 } },
	{ MODKEY,                       XK_Left,   cycle,          {.i = -1 } },
	{ MODKEY,                       XK_Right,  cycle,          {.i = +1 } },
	{ 0,                            XF86XK_Back, cycle,        {.i = -1 } },
	{ 0,                            XF86XK_Forward, cycle,     {.i = +1 } },

	{ MODKEY|ShiftMask,             XK_comma,  tagcycle,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagcycle,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Left,   tagcycle,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Right,  tagcycle,       {.i = +1 } },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	/* { MODKEY|ShiftMask,             XK_q,      quit,           {0} }, */
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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
};
