#ifndef __MIX_CONFIG_H__
#define __MIX_CONFIG_H__
#include "config.h"

#define MIX_CONFIG_XML  "./conf/mix_config.xml"

struct cluamanmixconfig
{
    struct cluamanconfiglist list;
    int tasknr;
};

#endif
