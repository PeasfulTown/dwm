static const char col_fg_norm[]      = "#989898";
static const char col_bg_norm[]      = "#0d0e1c";
static const char col_border_norm[]  = "#61647a";

static const char col_fg_sel[]       = "#ffffff";
static const char col_bg_sel[]       = "#4a4f69";
static const char col_border_sel[]   = "#338fff";


static const char col_fg_title[]     = "#ffffff";
static const char col_bg_title[]     = "#4a4f69";
static const char col_border_title[] = "#79a8ff";


static const char *colors[][3]      = {
	/*                  fg              bg                  border   */
	[SchemeNorm]    = { col_fg_norm     , col_bg_norm       ,  col_border_norm },
	[SchemeSel]     = { col_fg_sel      , col_bg_sel        ,  col_border_sel  },
	[SchemeTitle]   = { col_fg_title    , col_bg_title      ,  col_border_title  },
};

