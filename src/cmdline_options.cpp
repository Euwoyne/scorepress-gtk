
/*
  ScorePress - Music Engraving Software  (scorepress-gtk)
  Copyright (C) 2012 Dominik Lehmann
  
  Licensed under the EUPL, Version 1.1 or - as soon they
  will be approved by the European Commission - subsequent
  versions of the EUPL (the "Licence");
  You may not use this work except in compliance with the
  Licence.
 
  Unless required by applicable law or agreed to in
  writing, software distributed under the Licence is
  distributed on an "AS IS" basis, WITHOUT WARRANTIES OR
  CONDITIONS OF ANY KIND, either expressed or implied.
  See the Licence for the specific language governing
  permissions and limitations under the Licence.
*/

#include <cstdio>

#include "cmdline_options.hh"
#include "i18n.hh"
#include "version.hh"

int parse_cmdline(CmdlineOptions& opt, int argn, char* argv[])
{
    // reset target
    opt.log.silent  = false;
    opt.log.debug   = false;
    opt.log.verbose = false;
    opt.stdout.silent  = false;
    opt.stdout.debug   = false;
    opt.stdout.verbose = false;
    opt.files.clear();
    
    // parse arguments
    bool version = false;
    bool help = false;
    std::string arg;
    
    // check other arguments
    for (int i = 1; i < argn; ++i)
    {
        arg = argv[i];
        if (arg[0] != '-') opt.files.push_back(arg);
        else if (arg == "--nolog"                   ) opt.log.silent = true;
        else if (arg == "--debuglog"                ) opt.log.debug = true;
        else if (arg == "--verboselog"              ) opt.log.verbose = true;
        else if (arg == "--silent"    || arg == "-s") opt.stdout.silent = true;
        else if (arg == "--debug"     || arg == "-d") opt.stdout.debug = true;
        else if (arg == "--verbose"   || arg == "-v") opt.stdout.verbose = true;
        else if (arg == "--version"   || arg == "-V") version = true;
        else if (arg == "--help"      || arg == "-h") help = true;
        else {printf(_("Warning: Unrecognized option \"%s\"."), arg.c_str()); puts("");};
    };
    
    if (version)
    {
        puts(_("Libraries:"));
        print_library_info();
        return 2;
    };
    
    if (help)
    {
        fputs(_("\
Usage: score [options] [filename]\n\
\n\
 Log Options:\n\
   --nolog            don't create any log\n\
   --debuglog         log debug output\n\
   --verboselog       log verbose output\n\
\n\
 General Options:\n\
   -s, --silent       emit no output on stdout\n\
   -d, --debug        generate debug output\n\
   -v, --verbose      generate verbose output\n\
   -h, --help         print this help\n\
   -V, --version      print program version\n\
")      , stdout);
        fputs("\n\
Copyright (C) 2012 Dominik Lehmann\n\
This application is licensed under the EUPL, Version 1.1\n\
"       , stdout);
        return 1;
    };
    
    return 0;
}

