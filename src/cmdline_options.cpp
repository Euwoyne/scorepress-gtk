
/*
  ScorePress - Music Engraving Software  (scorepress-gtk)
  Copyright (C) 2014 Dominik Lehmann
  
  Licensed under the EUPL, Version 1.1 or - as soon they
  will be approved by the European Commission - subsequent
  versions of the EUPL (the "Licence");
  You may not use this work except in compliance with the
  Licence. You may obtain a copy of the Licence at
  <http://ec.europa.eu/idabc/eupl/>.
  
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
#include "config.hh"

int parse_cmdline(CmdlineOptions& opt, int argn, char* argv[])
{
    // reset target
    opt.log.silent  = false;
    opt.log.debug   = false;
    opt.log.verbose = false;
    opt.log.file.clear();
    opt.stdout.silent  = false;
    opt.stdout.debug   = false;
    opt.stdout.verbose = false;
    opt.files.clear();
    
    // parse arguments
    int  pos;
    bool version = false;
    bool dirs = false;
    bool help = false;
    std::string arg;
    
    // check other arguments
    for (int i = 1; i < argn; ++i)
    {
        arg = argv[i];
        if (arg[0] != '-') opt.files.push_back(arg);    // check for option
        
        if (arg[1] == '-')  // if we got a long option
        {
                 if (arg == "--nolog"     ) opt.log.silent = true;
            else if (arg == "--debuglog"  ) opt.log.debug = true;
            else if (arg == "--verboselog") opt.log.verbose = true;
            else if (arg == "--logfile"   && i < argn-1) opt.log.file = argv[++i];
            else if (arg == "--silent"    ) opt.stdout.silent = true;
            else if (arg == "--debug"     ) opt.stdout.debug = true;
            else if (arg == "--verbose"   ) opt.stdout.verbose = true;
            else if (arg == "--version"   ) version = true;
            else if (arg == "--dirs"      ) dirs = true;
            else if (arg == "--help"      ) help = true;
            else {printf(_("Warning: Unrecognized option \"%s\".\n"), arg.c_str());};
        }
        else                // otherwise check for short option string
        {
            for (pos = 1; argv[i][pos] != 0; ++pos)
            {
                switch (argv[i][pos])
                {
                case 's': opt.stdout.silent = true; break;
                case 'd': opt.stdout.debug = true; break;
                case 'v': opt.stdout.verbose = true; break;
                case 'V': version = true; break;
                case 'D': dirs = true; break;
                case 'h': help = true; break;
                default:  printf(_("Warning: Unrecognized option \"-%c\".\n"), argv[i][pos]); break;
                };
            };
        };
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
   --logfile <file>   write log to file\n\
\n\
 General Options:\n\
   -s, --silent       emit no output on stdout\n\
   -d, --debug        generate debug output\n\
   -v, --verbose      generate verbose output\n\
   -h, --help         print this help\n\
   -V, --version      print program version\n\
")      , stdout);
        fputs("\n\
" SCOREPRESS_COPYRIGHT "\n\
This application is licensed under the EUPL, Version 1.1\n\n\
"       , stdout);
    };
    
    if (version)
    {
        puts(_("Libraries:"));
        print_library_info();
        puts("");
    };
    
    if (dirs)
    {
        puts(_("Directories:"));
        print_directory_info();
        puts("");
    };
    
    if (help) return 1;
    if (dirs || version) return 2;
    return 0;
}

