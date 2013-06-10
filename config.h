#include <X11/XF86keysym.h>

/* appearance */
//static const char font[]            = "WenQuanYi Micro Hei for Powerline-10";
static const char font[]            = "Pragmata TT for Powerline 10";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */
#define H_ARROW_L "\u2B83"
#define H_ARROW_R "\u2B81"
#define S_ARROW_L "\u2B82"
#define S_ARROW_R "\u2B80"

#define CYAN  "#1793D0"
#define DBLUE "#31658c"
#define ACTIVE "#EF6800"

//GRAYSCALE
//#define GRAY2 "#303030"
#define GRAY2 DGRAY
#define GRAY4 "#5a5a5a"
#define GRAY8 "#9e9e9e"
#define GRAY9 "#bcbcbc"
#define GRAY10 "#d0d0d0"

#define BLACK "#000000"
#define WHITE "#FFFFFF"
#define DGRAY "#222222"
#define GRAY  "#3a3a3a"
#define LGRAY "#e0e0e0"

#define RED   "#CF4F88"
#define DRED   "#870000"

#define DGREEN "#005f00"
#define BGREEN "#afd700"

#define BORANGE "#ff8700"
static const char color[NUMCOL][ColLast][8] = {
    /* border foreground background underline*/
    { DGRAY,  GRAY8,     DGRAY}, /* 0 = unselected, unoccupied */
    { CYAN,   LGRAY,	 GRAY}, /* 1 = selected, occupied */
    { RED,    LGRAY,     GRAY4}, /* 2 = urgent */
    { DGRAY,  LGRAY,   DGRAY}, /* 3 = unselected, occupied */
    { DGRAY,  LGRAY,     GRAY}, /* 4 = selected, unoccupied */
    { DGRAY,  LGRAY,     DGRAY}, /* 5 = title */
    { DGRAY,  GRAY4,     DGRAY}, /* 6 = SEP */

    { DGRAY,  GRAY10,    GRAY4}, /* 7 = \x08=\b */
    { DGRAY,  GRAY10,    GRAY4}, /* 8 = \x09=\t */
    { DGRAY,  GRAY10,    GRAY4}, /* 9 = \x0a=\n */
    { DGRAY,  GRAY10,    GRAY4}, /* 10 = \x0b=\v */
    { DGRAY,  GRAY10,    GRAY4}, /* 11 = \x0c=\f */
    { DGRAY,  GRAY10,    GRAY4}, /* 12 = \x0d=\r */

    { DGRAY,  GRAY9,    GRAY4}, /* 13 = \x0e=scroll percentage */
    { DGRAY,  GRAY10,   GRAY4}, /* 14 = \x0f SEP*/
    { DGRAY,  GRAY2,    GRAY10}, /*15 = \x10 line info */
    { DGRAY,  BLACK,     LGRAY} /* 16 = \x11 Orangle */
};

/* tagging */
#define NTAGS 4
const char tags[NTAGS][10] = {"term", "web", "dev", "misc"};
//const char underline_color[NTAGS][8] = {"#EA5800", "","",""};

static const Rule rules[] = {
    /* xprop(1):
     *    WM_CLASS(STRING) = instance, class
     *    WM_NAME(STRING) = title
     */
    /* class     instance    title         tags mask     isfloating   monitor */
    { "Firefox", "Navigator", NULL       , 1 << 1,       False,        -1 },
    { "Firefox", "Download",  "Downloads", 0,            True,        -1 },
    { "URxvt",	NULL,	     NULL       , 1 << 0,       False,        0 },
    { "Gvim",	NULL,	     NULL       , 1 << 2,       False,        0 },
    { "MPlayer",NULL,	     NULL       , 1 << 3,       False,        0 },
    { "feh",	NULL,	     NULL       , 1 << 3,       False,        0 },
    { "matlab 2012a",	NULL,	     NULL       , 1 << 2,       False,        0 },
    { "wine",	NULL,	     NULL       , 1 << 3,       True,        -1 },
    //{ "VBoxSDL", "VBoxSDL",   NULL,        0,            True,        -1 },
};

/* layout(s) */
static const float mfact      = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
    { tile      },    /* first entry is default */
    { bstack    },
    { grid      },
    { monocle   },
    { NULL      },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-fn", "pragmata-12", "-nb", color[0][ColBG], "-nf", color[0][ColFG], "-sb", color[1][ColBG], "-sf", color[1][ColFG], NULL };
static const char *termcmd[]  = { "urxvt", NULL };
static const char *rangercmd[]  = { "urxvt", "-e", "ranger" };
static const char *weechatcmd[]  = { "urxvt", "-e", "weechat-curses" };
static const char *surfcmd[]  = { "firefox", NULL };
static const char *killxcmd[]  = { "urxvt", "-e", "pkill", "firefox" };
/* audio volume */
static const char *voltogglecmd[]  = { "amixer", "-q", "sset", "Master", "toggle", NULL };
static const char *voldowncmd[]    = { "amixer", "-q", "sset", "Master", "1-", "unmute", NULL };
static const char *volupcmd[]      = { "amixer", "-q", "sset", "Master", "1+", "unmute", NULL };

static const char *cmustogglecmd[] = { "ncmpcpp", "toggle", NULL };
static const char *cmusprevcmd[]   = { "ncmpcpp", "prev", NULL };
static const char *cmusnextcmd[]   = { "ncmpcpp", "next", NULL };

static Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
    { MODKEY,                       XK_p,      spawn,          {.v = rangercmd } },
    { MODKEY,			    XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,			    XK_f,      spawn,          {.v = surfcmd } },
    { MODKEY|Mod1Mask,		    XK_c,      spawn,	       {.v = weechatcmd}},
    { MODKEY|ShiftMask,             XK_q,      spawn,          {.v = killxcmd} },
    { MODKEY,                       XK_n,      togglebar,      {-1} },
    { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
    { MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
    { MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
    { MODKEY|ShiftMask,		    XK_i,      incnmaster,     {.i = +1 } },
    { MODKEY|ShiftMask,		    XK_d,      incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
    { MODKEY,                       XK_Tab,    view,           {0} },
    { MODKEY,		            XK_d,      killclient,     {0} },
    { MODKEY,                       XK_t,      setlayout,      {.i = 0} },
    { MODKEY,                       XK_b,      setlayout,      {.i = 1} },
    { MODKEY,                       XK_g,      setlayout,      {.i = 2} },
    { MODKEY,                       XK_m,      setlayout,      {.i = 3} },
    { MODKEY,                       XK_f,      setlayout,      {.i = 4} },
    { MODKEY,                       XK_space,  setlayout,      {.i = -1} },
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { Mod1Mask,             XK_Left,       spawn,             {.v = cmusprevcmd}},
    { Mod1Mask,             XK_Right,      spawn,             {.v = cmusnextcmd}},
    {  0,                0x1008ff11,    spawn,             {.v = voldowncmd}},
    {  0,                0x1008ff13,    spawn,             {.v = volupcmd}},
    {  0,                0x1008ff12,    spawn,             {.v = voltogglecmd}},
    {  0,                0x1008ff14,    spawn,             {.v = cmustogglecmd}},
    {  0,                0x1008ff16,    spawn,             {.v = cmusprevcmd}},
    {  0,                0x1008ff17,    spawn,             {.v = cmusnextcmd}},
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    //{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
};
