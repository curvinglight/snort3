/*
** Copyright (C) 2014 Cisco and/or its affiliates. All rights reserved.
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License Version 2 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
// ips_priority.cc author Russ Combs <rucombs@cisco.com>

#include <sys/types.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "snort_types.h"
#include "detection/treenodes.h"
#include "snort_debug.h"
#include "snort.h"
#include "detection/detection_defines.h"
#include "framework/ips_option.h"
#include "framework/parameter.h"
#include "framework/module.h"

static const char* s_name = "priority";

//-------------------------------------------------------------------------
// module
//-------------------------------------------------------------------------

static const Parameter s_params[] =
{
    { "~", Parameter::PT_INT, "1:", nullptr,
      "generator id" },

    { nullptr, Parameter::PT_MAX, nullptr, nullptr, nullptr }
};

static const char* s_help =
    "rule option for prioritizing events";

class PriorityModule : public Module
{
public:
    PriorityModule() : Module(s_name, s_help, s_params) { };
    bool set(const char*, Value&, SnortConfig*);
    int priority;
};

bool PriorityModule::set(const char*, Value& v, SnortConfig*)
{
    if ( !v.is("~") )
        return false;

    priority = v.get_long();
    return true;
}

//-------------------------------------------------------------------------
// api methods
//-------------------------------------------------------------------------

static Module* mod_ctor()
{
    return new PriorityModule;
}

static void mod_dtor(Module* m)
{
    delete m;
}

static IpsOption* priority_ctor(Module* p, OptTreeNode* otn)
{
    PriorityModule* m = (PriorityModule*)p;
    otn->sigInfo.priority = m->priority;
    return nullptr;
}

static const IpsApi priority_api =
{
    {
        PT_IPS_OPTION,
        s_name,
        s_help,
        IPSAPI_PLUGIN_V0,
        0,
        mod_ctor,
        mod_dtor
    },
    OPT_TYPE_META,
    1, PROTO_BIT__NONE,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    priority_ctor,
    nullptr,
    nullptr
};

#ifdef BUILDING_SO
SO_PUBLIC const BaseApi* snort_plugins[] =
{
    &priority_api.base,
    nullptr
};
#else
const BaseApi* ips_priority = &priority_api.base;
#endif
