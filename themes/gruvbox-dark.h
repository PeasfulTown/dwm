static const char col_fg_norm[]      = "#fbf1c7";
static const char col_bg_norm[]      = "#1d2021";
static const char col_border_norm[]  = "#928374";

static const char col_fg_sel[]       = "#fe8019";
static const char col_bg_sel[]       = "#1d2021";
static const char col_border_sel[]   = "#d65d0e";


static const char col_fg_title[]     = "#fabd2f";
static const char col_bg_title[]     = "#1d2021";
static const char col_border_title[] = "#d79921";


static const char *colors[][3]      = {
	/*                  fg              bg                  border   */
	[SchemeNorm]    = { col_fg_norm     , col_bg_norm       ,  col_border_norm },
	[SchemeSel]     = { col_fg_sel      , col_bg_sel        ,  col_border_sel  },
	[SchemeTitle]   = { col_fg_title    , col_bg_title      ,  col_border_title  },
};
