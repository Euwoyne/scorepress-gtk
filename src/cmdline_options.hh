
/*
  ScorePress - Music Engraving Software  (scorepress-gtk)
  Copyright (C) 2014 Dominik Lehmann
  
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

#ifndef CMDLINE_OPTIONS_HH
#define CMDLINE_OPTIONS_HH

#include <string>
#include <vector>

struct CmdlineOptions
{
    struct cLog
    {
        bool silent;
        bool debug;
        bool verbose;
    } log;
    
    struct cStdout
    {
        bool silent;
        bool debug;
        bool verbose;
    } stdout;
    
    std::vector<std::string> files;
};

int parse_cmdline(CmdlineOptions& opt, int argn, char* argv[]);

#endif

