static const char col_fg_norm[]      = "#ebdbb2";
static const char col_bg_norm[]      = "#1d2021";
static const char col_border_norm[]  = "#282828";

static const char col_fg_sel[]       = "#fbf1c7";
static const char col_bg_sel[]       = "#076678";
static const char col_border_sel[]   = "#83a598";


static const char col_fg_title[]     = "#fbf1c7";
static const char col_bg_title[]     = "#076678";
static const char col_border_title[] = "#fabd2f";


static const char *colors[][3]      = {
	/*                  fg              bg                  border   */
	[SchemeNorm]    = { col_fg_norm     , col_bg_norm       ,  col_border_norm },
	[SchemeSel]     = { col_fg_sel      , col_bg_sel        ,  col_border_sel  },
	[SchemeTitle]   = { col_fg_title    , col_bg_title      ,  col_border_title  },
};
